#pragma once
#include "../include/vector.h"
#include "../lib/imgui/imgui.h"

class Render
{
  public:
	::Vector2D WorldToScreen(::Vector pos);
	void LineToPlayer(::Vector pos, ::ImColor col, float thickness);
};

extern ::Render render;