add_executable(main 
    ./cpp/main.cpp 
    ./cpp/Ball.cpp 
    ./cpp/Client.cpp
    ./cpp/NetClient.cpp
    ./cpp/PingPong.cpp
    ./cpp/Player.cpp
    ./cpp/Server.cpp
)

set(MY_SFML_PATH D:\\Development\\Libs\\SFML-2.6.0\\lib\\cmake\\SFML)
set(SFML_DIR ${MY_SFML_PATH})
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 COMPONENTS system window graphics REQUIRED)
target_link_libraries(main PRIVATE sfml-system sfml-window sfml-graphics sfml-network)
target_include_directories(main PRIVATE ./hdr)

install(TARGETS main
        CONFIGURATIONS Release
        DESTINATION ${CMAKE_INSTALL_PREFIX}/ping-pong
)

install(FILES ${PROJECT_SOURCE_DIR}/src/tms.ttf
        CONFIGURATIONS Release
        DESTINATION ${CMAKE_INSTALL_PREFIX}/ping-pong
)
