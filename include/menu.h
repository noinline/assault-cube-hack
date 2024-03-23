#include "../lib/imgui/imgui.h"

class Menu
{
  public:
	struct Colors
	{
		ImColor line = {255, 255, 255, 255};
	} col;

  public:
	void Render();
};

extern Menu *menu;