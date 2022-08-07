#pragma once

enum class PressedKey {
	left, right, space
};

const int screen_width = 800;
const int screen_height = 600;

const int panel_width = screen_width / 5;
const int panel_height = screen_height / 20;

const int panel_speed = screen_width;	// pixels in second

const int start_ball_speed = screen_width / 2;