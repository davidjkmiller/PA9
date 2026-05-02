// Asked AI to go through and organize/comment the code without modifying it
// Prompt: Can you go through my code and organize it to be more readable and add comments where necessary. Do not add explanations for things like "why a destructor is needed", do not modify the actual code.

#pragma once

namespace Constants
{
	// Screen Dimensions
	inline constexpr int SCREEN_WIDTH = 1920;
	inline constexpr int SCREEN_HEIGHT = 1080;
	inline constexpr float SCREEN_WIDTH_FLOAT = 1920.f;
	inline constexpr float SCREEN_HEIGHT_FLOAT = 1080.f;

	// Card Rendering
	inline constexpr float CARD_SCALE = 2.5f;
	inline constexpr float CARD_SPACING_X = 175.f;

	// Board Layout
	inline constexpr float BOARD_START_X = 575.f;
	inline constexpr float BOARD_START_Y = 350.f;
	inline constexpr float PLAYER_HAND_START_X = 815.f;
	inline constexpr float PLAYER_HAND_START_Y = 650.f;

	// UI Element Layout
	inline constexpr float DEAL_BTN_X = 1600.f;
	inline constexpr float ACTION_BTN_START_X = 710.f;
	inline constexpr float ACTION_BTN_START_Y = 900.f;
	inline constexpr float ACTION_BTN_SPACING = 200.f;

	inline constexpr float LOG_TEXT_X = 650.f;
	inline constexpr float LOG_TEXT_Y = 50.f;
	inline constexpr float WINNER_TEXT_X = 750.f;
	inline constexpr float WINNER_TEXT_Y = 300.f;

	// Typography
	inline constexpr int BUTTON_TEXT_SIZE = 75;
	inline constexpr int FONT_SIZE = 50;

	// Game Settings
	inline constexpr int MAX_PLAYERS = 4;
	inline constexpr int DECK_SIZE = 52;
	inline constexpr float STARTING_BALANCE = 1000.0f;
	inline constexpr float STARTING_BET = 5.0f;

	// Networking
	inline constexpr unsigned short DEFAULT_PORT = 7777;
}