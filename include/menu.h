#include "../lib/imgui/imgui.h"

class Menu
{
  public:
	struct Colors
	{
		ImColor line = {255, 255, 255, 255};
	} col;

	int currentTab = 0;

  public:
	void ShowMenu();
};

extern ::Menu *menu;