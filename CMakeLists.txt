cmake_minimum_required(VERSION 3.13)
project(pong VERSION 1.0.0)

add_subdirectory("extern")

add_executable(${PROJECT_NAME}
	"main.cpp"
	"Game.h"
	"Game.cpp")

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		SDL2::SDL2-static
		SDL2::SDL2main
)