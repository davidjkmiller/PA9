#pragma once
#include <SFML/Graphics.hpp>

class UIElement
{
public:
	// A virtual destructor is REQUIRED for base classes to prevent memory leaks 
	// when deleting derived objects (like Button or ImageButton) through a base pointer.
	virtual ~UIElement() = default;

	// Pure virtual functions (= 0) force any inherited class to implement these methods.
	virtual void drawTo(sf::RenderWindow& window) = 0;
	virtual bool isMouseOver(sf::RenderWindow& window) = 0;
};