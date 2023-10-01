#pragma once

class PongObserver {
public:
	virtual void sendCoordinates(int ball_x, int ball_y, int p1_x, int p1_y, int p2_x, int p2_y) = 0;
};