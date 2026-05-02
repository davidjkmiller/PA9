#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "UIElement.hpp"

class Button : public UIElement
{
public:
	Button(const sf::Font& font, std::string buttonText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor);

	// Set the position of the button
	// The position is relative to the upper left corner of the window (0, 0)
	void setPosition(sf::Vector2f pos);

	// Draw the button to the main window
	void drawTo(sf::RenderWindow& window) override;

	// Check if the mouse is currently over the button
	bool isMouseOver(sf::RenderWindow& window) override;

	void setTextColor(sf::Color color);
	void setBackColor(sf::Color color);

private:
	sf::RectangleShape buttonShape;
	sf::Text text;
};