// Asked AI to go through and organize/comment the code without modifying it
// Prompt: Can you go through my code and organize it to be more readable and add comments where necessary. Do not add explanations for things like "why a destructor is needed", do not modify the actual code.

#pragma once
#include <SFML/Graphics.hpp>

class UIElement
{
public:
	virtual ~UIElement() = default;

	virtual void drawTo(sf::RenderWindow& window) = 0;
	virtual bool isMouseOver(sf::RenderWindow& window) = 0;
};