// Asked AI for guidance on how I should separate regular buttons and the main menu image buttons
// Also asked if it would be better to use inheritance or keep ImageButton and Button classes separate
// Separate seemed to be the better choice
// Prompt: "Would it be better to use inheritance for the Button and ImageButton classes or keep them separate?"
// Forgot about making a base class, changed to inherit from the UIElement class after I remembered

#pragma once

#include <SFML/Graphics.hpp>
#include "UIElement.hpp"

class ImageButton : public UIElement
{
public:
	// Need to assign the texture and position upon initialization
	ImageButton(const sf::Texture& texture, sf::Vector2f pos);

	// Draw the image button to the main window
	void drawTo(sf::RenderWindow& window) override;

	// Check if the mouse is currently over the image
	bool isMouseOver(sf::RenderWindow& window) override;

	// Set a custom hover effect
	void setHoverEffect(bool isHovering);

private:
	sf::Sprite sprite;
};