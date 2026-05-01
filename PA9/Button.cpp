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

	// Set the text origin to be the center
	text.setOrigin({
		textBounds.position.x + (textBounds.size.x / 2.0f),
		textBounds.position.y + (textBounds.size.y / 2.0f)
		});

	// Set the text position to be the center, which should work since the origin is the center now as well
	text.setPosition({
		pos.x + (buttonShape.getSize().x / 2.0f),
		pos.y + (buttonShape.getSize().y / 2.0f)
		});
}

void Button::drawTo(sf::RenderWindow& window)
{
	window.draw(buttonShape);
	window.draw(text);
}

bool Button::isMouseOver(sf::RenderWindow& window)
{
	// Get the current mouse coordinates and map to world coordinates
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	// Check if the button bounds contains the current mouse position
	sf::FloatRect bounds = buttonShape.getGlobalBounds();
	if (bounds.contains(mousePos))
	{
		return true;
	}
	return false;
}

void Button::setTextColor(sf::Color color)
{
	text.setFillColor(color);
}