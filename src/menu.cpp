#include "../include/menu.h"

#include "../include/hack.h"

::Menu *menu = new ::Menu();

auto ::Menu::Render(void) -> decltype(void())
{
	::ImGui::SetNextWindowPos({::ImGui::GetIO().DisplaySize.x / 2,
							   ::ImGui::GetIO().DisplaySize.y / 2},
							  ::ImGuiCond_Once, ::ImVec2{0.5, 0.5});
	::ImGui::SetNextWindowSize(::ImVec2{400.f, 150.f}, NULL);

	if (::GetAsyncKeyState(VK_DELETE) & 1)
		::hack->func.showMenu = !hack->func.showMenu;

	if (::hack->func.showMenu)
	{

		::ImGui::Begin("Stinky Balls", nullptr,
					   ::ImGuiWindowFlags_NoResize |
						   ::ImGuiWindowFlags_NoDecoration);
		{
			::ImGui::SetCursorPos(::ImVec2{5, 20});
			::ImGui::BeginGroup();
			{
				if (::ImGui::Button("aimbot", ::ImVec2{100, 30}))
					this->currentTab = 0;

				if (::ImGui::Button("visuals", ::ImVec2{100, 30}))
					this->currentTab = 1;

				if (::ImGui::Button("exploits", ::ImVec2{100, 30}))
					this->currentTab = 2;
			}
			::ImGui::EndGroup();

			switch (this->currentTab)
			{
			case 0: {
				::ImGui::SetCursorPos(::ImVec2{110, 20});
				::ImGui::BeginChild("##container0", ::ImVec2{200, 130}, false,
									::ImGuiWindowFlags_None);
				{
					::ImGui::Text("nothing is here for now...");
				}
				::ImGui::EndChild();
			}
			break;
			case 1: {
				::ImGui::SetCursorPos(::ImVec2{110, 20});
				::ImGui::BeginChild("##container1", ::ImVec2{200, 130}, false,
									::ImGuiWindowFlags_None);
				{
					::ImGui::Checkbox("Show line", &hack->func.line);
					::ImGui::SameLine();
					::ImGui::ColorEdit4("##line_color",
										reinterpret_cast<float *>(&col.line),
										::ImGuiColorEditFlags_NoInputs);
				}
				::ImGui::EndChild();
			}
			break;
			case 2: {
				::ImGui::SetCursorPos(::ImVec2{110, 20});
				::ImGui::BeginChild("##container2", ::ImVec2{200, 130}, false,
									::ImGuiWindowFlags_None);
				{
					::ImGui::Checkbox("Toggle godmode", &hack->func.godMode);
					::ImGui::Checkbox("Toggle teleport", &hack->func.teleport);
					//::ImGui::Checkbox("Toggle bhop", &hack->func.bhop);
					::ImGui::Checkbox("Toggle funny camera",
									  &hack->func.funnyCamera);
					::ImGui::Checkbox("Toggle noclip", &hack->func.noclip);
				}
				::ImGui::EndChild();
			}
			break;
			}
		}
		::ImGui::End();
	}
}