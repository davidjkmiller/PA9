#include "Button.hpp"

// Need to use member initialization cause SFML doesn't like making Text objects without a font
Button::Button(const sf::Font& font, std::string buttonText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor)
	: text(font, buttonText, charSize)
{
	buttonShape.setSize(buttonSize);
	buttonShape.setFillColor(bgColor);

	text.setFillColor(textColor);
}

void Button::setPosition(sf::Vector2f pos)
{
	buttonShape.setPosition(pos);

	// Get local bounds for the text
	sf::FloatRect textBounds = text.getLocalBounds();

	// Center the text inside the button shape
	float xPos = (pos.x + buttonShape.getSize().x / 2.0f) - (textBounds.size.x / 2.0f);
	float yPos = (pos.y + buttonShape.getSize().y / 2.0f) - (textBounds.size.y / 2.0f);

	// Y position slight adjustment
	text.setPosition({ xPos, yPos - (textBounds.size.y / 2.0f) });
}

void Button::drawTo(sf::RenderWindow& window)
{
	window.draw(buttonShape);
	window.draw(text);
}

bool Button::isMouseOver(sf::RenderWindow& window)
{
	// Get the mouse position relative to the window
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	// Check if the mouse is inside the button's global bounds
	return buttonShape.getGlobalBounds().contains(mousePos);
}

void Button::setTextColor(sf::Color color)
{
	text.setFillColor(color);
}

void Button::setBackColor(sf::Color color)
{
	buttonShape.setFillColor(color);
}

