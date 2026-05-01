#include "ImageButton.hpp"

ImageButton::ImageButton(const sf::Texture& texture, sf::Vector2f pos) : sprite(texture)
{
	// Get the local bounds of the image (SFML 3 uses .size.x and .size.y)
	sf::FloatRect bounds = sprite.getLocalBounds();

	// Set the origin to be the center of the image
	// Makes it easier to position in this case
	sprite.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });

	sprite.setPosition(pos);
}

void ImageButton::drawTo(sf::RenderWindow& window)
{
	window.draw(sprite);
}

bool ImageButton::isMouseOver(sf::RenderWindow& window)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	sf::FloatRect bounds = sprite.getGlobalBounds();
	if (bounds.contains(mousePos))
	{
		return true;
	}
	return false;
}

void ImageButton::setHoverEffect(bool isHovering)
{
	if (isHovering)
	{
		sprite.setColor(sf::Color(255, 255, 255, 150));
		sprite.setScale({ 1.15f, 1.15f });              
	}
	else
	{
		sprite.setColor(sf::Color::White);               
		sprite.setScale({ 1.0f, 1.0f });                
	}
}