#include "../include/render.h"

#include "../include/hack.h"

::Render render;

auto ::Render::WorldToScreen(::Vector pos) -> decltype(::Vector2D())
{
	::Vector clip = {0.0f, 0.0f, 0.0f};
	::Vector2D ndc = {0.0f, 0.0f};
	::Vector2D result = {0.0f, 0.0f};
	::Vector2D base = {0.0f, 0.0f};

	float matrix[16]{};
	for (int i = 0; i < 16; i++)
	{
		float matrixElement = *reinterpret_cast<float *>(
			hack->offset.base + hack->offset.viewMatrix + 0x4 * i);
		matrix[i] = matrixElement;
	}

	clip.z =
		pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];
	if (clip.z < 0.2)
		return base;
	clip.x =
		pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clip.y =
		pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];

	ndc.x = clip.x / clip.z;
	ndc.y = clip.y / clip.z;

	result.x = ((::ImGui::GetIO().DisplaySize.x / 2 * ndc.x) +
				(ndc.x + ::ImGui::GetIO().DisplaySize.x / 2));
	result.y = (-(::ImGui::GetIO().DisplaySize.y / 2 * ndc.y) +
				(ndc.y + ::ImGui::GetIO().DisplaySize.y / 2));

	return result;
}

auto ::Render::LineToPlayer(::Vector pos, ::ImColor col, float thickness)
	-> decltype(void())
{

	::Vector2D translate = this->WorldToScreen(pos);

	if (translate.x == NULL && translate.y == NULL)
		return;

	::ImGui::GetBackgroundDrawList()->AddLine(
		::ImVec2(::ImGui::GetIO().DisplaySize.x / 2,
				 ::ImGui::GetIO().DisplaySize.y / 2),
		::ImVec2(translate.x, translate.y), col, thickness);
}
