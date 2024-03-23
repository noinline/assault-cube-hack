#include "../include/menu.h"

#include "../include/hack.h"

Menu *menu = new Menu();

auto
Menu::Render(void) -> decltype(void())
{
	ImGui::SetNextWindowPos(
		{ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2},
		ImGuiCond_Once, {0.5, 0.5});
	ImGui::SetNextWindowSize({400.f, 150.f}, NULL);

	if (::GetAsyncKeyState(VK_DELETE) & 1)
		hack->func.showMenu = !hack->func.showMenu;

	if (hack->func.showMenu)
	{
		ImGui::Begin("Stinky Balls");
		{
			ImGui::Checkbox("Toggle godmode", &hack->func.godMode);
			ImGui::Checkbox("Toggle teleport", &hack->func.teleport);
			ImGui::Checkbox("Toggle bhop", &hack->func.bhop);
			ImGui::Checkbox("Toggle Funny camera", &hack->func.funnyCamera);
			ImGui::Checkbox("Toggle Debug camera", &hack->func.noclip);
			ImGui::Checkbox("Show line", &hack->func.line);
			ImGui::SameLine();
			ImGui::ColorEdit4("##line_color",
							  reinterpret_cast<float *>(&col.line),
							  ImGuiColorEditFlags_NoInputs);
		}
		ImGui::End();
	}
}