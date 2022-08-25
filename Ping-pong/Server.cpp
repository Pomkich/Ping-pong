#include "Server.h"

Server::Server() {
	interrupt_b_list = false;
	interrupt_b_read = false;
	interrupt_b_send = false;
	running = false;
}

// threads starter
void Server::Run() {
	if (running) {
		std::cout << "server already running" << std::endl;
		return;
	}

	ping_pong = std::make_shared<PingPong>(shared_from_this());
	// disable interrupt
	interrupt_b_list = false;
	interrupt_b_read = false;
	interrupt_b_send = false;

	listener_thread = std::move(std::thread(&Server::AcceptConnections, &(*this)));
	read_thread = std::move(std::thread(&Server::ReadMessages, &(*this)));
	send_thread = std::move(std::thread(&Server::SendData, &(*this)));
	listener_thread.detach();
	read_thread.detach();
	send_thread.detach();

	running = true;
}


void Server::Stop() {
	if (!running) {
		std::cout << "server already stoped" << std::endl;
		return;
	}
	// enable interrupt
	std::unique_lock<std::mutex> lock_list(interrupt_m_list);
	std::unique_lock<std::mutex> lock_read(interrupt_m_read);
	std::unique_lock<std::mutex> lock_send(interrupt_m_send);

	interrupt_b_list = true;
	interrupt_b_read = true;
	interrupt_b_send = true;

	// wait when all threads interrupt
	threads_stoped.wait(lock_list, [&] { return !interrupt_b_list; });
	threads_stoped.wait(lock_read, [&] { return !interrupt_b_read; });
	no_data.notify_one();	// wake up without data
	threads_stoped.wait(lock_send, [&] { return !interrupt_b_send; });

	// disconnect players
	if (player_1) {
		player_1->setBlocking(true);
		player_1->disconnect();
		player_1.reset();
	}

	if (player_2) {
		player_2->setBlocking(true);
		player_2->disconnect();
		player_2.reset();
	}

	ping_pong->Stop();
	ping_pong.reset();

	running = false;
}

void Server::AcceptConnections() {
	std::unique_ptr<sf::TcpSocket> temp_socket = std::make_unique<sf::TcpSocket>();

	if (listener.listen(57000) != sf::Socket::Done)
		std::cout << "can't listen" << std::endl;
	else std::cout << "starting listen" << std::endl;

	listener.setBlocking(false);

	// this thread only accept new connections
	// and rejects them, if lobby is full
	while (true) {
		{	// interruption of thread
			std::unique_lock<std::mutex> lock(interrupt_m_list);
			if (interrupt_b_list) {
				std::cout << "interrupt listener" << std::endl;
				interrupt_b_list = false;
				lock.unlock();
				threads_stoped.notify_one();
				return;
			}
		}

		if (listener.accept(*temp_socket) == sf::Socket::Done) {
			std::unique_lock<std::mutex> lock(player_access_mut);
			// move pointer to any player
			if (player_1 == nullptr) {
				std::cout << "player 1 set" << std::endl;
				player_1 = std::move(temp_socket);
				player_1->setBlocking(false);
			}
			else if (player_2 == nullptr) {
				std::cout << "player 2 set" << std::endl;
				player_2 = std::move(temp_socket);
				player_2->setBlocking(false);
			}
			lock.unlock();

			// if nullptr -> pointer moved to any player
			// need to create new socket for listening
			if (!temp_socket) {
				temp_socket.reset();
				temp_socket = std::make_unique<sf::TcpSocket>();

				// if both player connected -> can start game
				lock.lock();
				if (player_1 != nullptr && player_2 != nullptr)
					OnReady();
				lock.unlock();
			}
			else {	// else lobby is full
				// if it case calls, then game will not run anyway or already running
				sf::Packet temp_packet;
				std::string err_message = "this lobby don't have slots";
				temp_packet << err_message;
				temp_socket->send(temp_packet);
				temp_socket->disconnect();
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(tick_rate));
	}
}

// reader thread
void Server::ReadMessages() {
	//std::string message;
	sf::Socket::Status status;

	while (true) {
		{	// interruption of thread
			std::unique_lock<std::mutex> lock(interrupt_m_read);
			if (interrupt_b_read) {
				std::cout << "interrupt reader" << std::endl;
				interrupt_b_read = false;
				lock.unlock();
				threads_stoped.notify_one();
				return;
			}
		}

		std::unique_lock<std::mutex> lock(player_access_mut);

		// getting data from players
		if (player_1 != nullptr) {
			status = player_1->receive(message_p_1);
			if (status == sf::Socket::Done) {
				bool left, right, space;
				message_p_1 >> left >> right >> space;
				message_p_1.clear();

				ping_pong->notifyKeyPress(PressedKey::left, left, 0);
				ping_pong->notifyKeyPress(PressedKey::right, right, 0);
				ping_pong->notifyKeyPress(PressedKey::space, space, 0);
				std::cout << "message player 1: " << left << right << space << std::endl;
			}
			else if (status == sf::Socket::Disconnected) {
				std::cout << "disconnected player 1" << std::endl;
				lock.unlock();	// there is deadlock if do not unlock this mutex
				ping_pong->Stop();
				lock.lock();
				player_1->setBlocking(true);
				player_1->disconnect();
				player_1.reset();
			}
		}
		if (player_2 != nullptr) {
			status = player_2->receive(message_p_2);
			if (status == sf::Socket::Done) {
				bool left, right, space;
				message_p_2 >> left >> right >> space;
				message_p_2.clear();

				ping_pong->notifyKeyPress(PressedKey::left, left, 1);
				ping_pong->notifyKeyPress(PressedKey::right, right, 1);
				ping_pong->notifyKeyPress(PressedKey::space, space, 1);
				std::cout << "message player 2: " << left << right << space << std::endl;
			}
			else if (status == sf::Socket::Disconnected) {
				std::cout << "disconnected player 2" << std::endl;
				lock.unlock();
				ping_pong->Stop();
				lock.lock();
				player_2->setBlocking(true);
				player_2->disconnect();
				player_2.reset();
			}
		}
		lock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(tick_rate));
	}
}

// sending data thread
void Server::SendData() {
	while (true) {
		{	// interruption of thread
			std::unique_lock<std::mutex> lock(interrupt_m_send);
			if (interrupt_b_send) {
				std::cout << "interrupt sender" << std::endl;
				interrupt_b_send = false;
				lock.unlock();
				threads_stoped.notify_one();
				return;
			}
		}

		std::unique_lock<std::mutex> lock(send_packet_mut);
		// the stream will be blocked if no data, so he can't be interrupted 
		// when packets don't come
		// for the reason above added one more condition to exit from block
		no_data.wait(lock, [&] {return (!data_to_send.empty() || interrupt_b_send); });

		// needed to lock player mutex for sending data
		std::unique_lock<std::mutex> player_lock(player_access_mut);
		while (!data_to_send.empty()) {
			sf::Packet packet = data_to_send.front();
			data_to_send.pop();

			if (player_1) player_1->send(packet);
			if (player_2) player_2->send(packet);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(tick_rate));
	}
}

void Server::OnReady() {
	ping_pong->Run();
}

void Server::sendCoordinates(int ball_x, int ball_y, int p1_x, int p1_y, int p2_x, int p2_y) {
	// for better perfomance should create data without mutex lock
	sf::Packet new_packet;
	new_packet << ball_x << ball_y << p1_x << p1_y << p2_x << p2_y;

	std::unique_lock<std::mutex> lock(send_packet_mut);
	data_to_send.push(new_packet);
	lock.unlock();
	no_data.notify_one();
}