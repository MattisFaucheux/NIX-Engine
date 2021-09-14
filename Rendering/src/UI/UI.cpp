#include "pch.h"
#include "UI/UI.h"

#include "UI/UIManager.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_internal.h"

UI* UI::GetUI()
{
	if (m_singleton == nullptr)
	{
		m_singleton = new UI();
	}
	
	return m_singleton;
}

void UI::InitDocking()
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
}

std::pair<float, float> UI::GetCursorPos()
{
	return std::pair(ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
}

float UI::GetCursorPosX()
{
	return ImGui::GetCursorPosX();
}

float UI::GetCursorPosY()
{
	return ImGui::GetCursorPosY();
}

std::pair<float, float> UI::GetContentRegionAvail()
{
	ImVec2 region = ImGui::GetContentRegionAvail();
	return std::pair(region.x, region.y);
}

float UI::GetContentRegionAvailX()
{
	return ImGui::GetContentRegionAvail().x;
}

float UI::GetContentRegionAvailY()
{
	return ImGui::GetContentRegionAvail().y;
}

std::pair<float, float> UI::GetWindowSize()
{
	ImVec2 size = ImGui::GetWindowSize();
	return std::pair(size.x, size.y);
}

float UI::GetWindowWidth()
{
	return ImGui::GetWindowWidth();
}

std::pair<float, float> UI::GetWindowPos()
{
	ImVec2 pos = ImGui::GetWindowPos();
	return std::pair(pos.x, pos.y);
}

float UI::GetWindowHeight()
{
	return ImGui::GetWindowHeight();
}

std::pair<float, float> UI::GetIOSize()
{
	ImVec2 size = ImGui::GetIO().DisplaySize;
	return std::pair(size.x, size.y);
}

float UI::GetIOWidth()
{
	return ImGui::GetIO().DisplaySize.x;
}

float UI::GetIOHeight()
{
	return ImGui::GetIO().DisplaySize.y;
}

void UI::HoverHandCursor()
{
	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

void UI::SetCursorPos(float posX, float posY)
{
	ImGui::SetCursorPos(ImVec2(posX, posY));
}

void UI::SetCursorPosX(float posX)
{
	ImGui::SetCursorPosX(posX);
}

void UI::SetCursorPosY(float posY)
{
	ImGui::SetCursorPosY(posY);
}

bool UI::AddCheckbox(std::string_view label, bool& isSelected)
{
	bool result;

	if (std::strlen(label.data()) == 0)
	{
		const float posY = ImGui::GetCursorPosY();
		result = ImGui::Checkbox(label.data(), &isSelected);
		ImGui::SetCursorPos(ImVec2(31.f, posY));
	}
	else
	{
		result = ImGui::Checkbox(label.data(), &isSelected);
	}

	HoverHandCursor();

	return result;
}

bool UI::AddButton(std::string_view label, float sizeX, float sizeY)
{
	bool result = ImGui::Button(label.data(), ImVec2(sizeX, sizeY));
	UI::HoverHandCursor();
	return result;
}

void UI::AddImage(void *& textureID, float sizeX, float sizeY)
{
	ImGui::Image(textureID, ImVec2(sizeX, sizeY));
}

void UI::AddText(std::string_view text)
{
	ImGui::Text(text.data());
}

void UI::AddTextDisabled(std::string_view text)
{
	ImGui::TextDisabled(text.data());
}

void UI::AddTextColored(std::string_view text, LibMath::Vector4 color)
{
	ImGui::TextColored({color.x, color.y, color.z, color.w}, text.data());
}

void UI::AddMarker(std::string_view text)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(text.data());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

bool UI::AddInt(std::string_view label, int& nbr, bool positiveOnly)
{
	if (positiveOnly && nbr < 0)
		nbr = 0;

	bool result = ImGui::InputInt(label.data(), &nbr);

	return result;
}

bool UI::AddFloat(std::string_view label, float& nbr, bool positiveOnly)
{
	if (positiveOnly && nbr < 0.f)
		nbr = 0.f;

	bool result = ImGui::InputFloat(label.data(), &nbr, 0.0f, 0.0f, "%.4f");

	return result;
}

bool UI::AddFloat2(std::string_view label, float* vec, bool positiveOnly)
{
	if (positiveOnly)
	{
		if (vec[0] < 0.f)
			vec[0] = 0.f;
		if (vec[1] < 0.f)
			vec[1] = 0.f;
		if (vec[2] < 0.f)
			vec[2] = 0.f;
	}

	bool result = ImGui::InputFloat2(label.data(), vec, "%.4f");

	return result;
}

bool UI::AddFloat3(std::string_view label, float* vec, bool positiveOnly)
{
	if (positiveOnly)
	{
		if (vec[0] < 0.f)
			vec[0] = 0.f;
		if (vec[1] < 0.f)
			vec[1] = 0.f;
		if (vec[2] < 0.f)
			vec[2] = 0.f;
	}

	bool result = ImGui::InputFloat3(label.data(), vec, "%.4f");

	return result;
}

bool UI::AddInputText(std::string_view label, std::string_view hint, std::string& buf, bool readOnly)
{
	char c[256];
	strcpy_s(c, buf.c_str());
	const bool result = ImGui::InputTextWithHint(label.data(), hint.data(), c, IM_ARRAYSIZE(c), readOnly ? ImGuiInputTextFlags_ReadOnly : 0);
	buf = std::string(c);
	return result;
}

bool UI::AddCombo(std::string_view label, std::vector<std::string>& items, std::string& itemSelected)
{
	std::string_view temp = itemSelected;

	if (ImGui::BeginCombo(label.data(), itemSelected.data()))
	{
		for (int n = 0; n < items.size(); n++)
		{
			const bool is_selected = (itemSelected == items[n].data());
			if (ImGui::Selectable(items[n].data(), is_selected))
				itemSelected = items[n].data();

			if (is_selected)
				ImGui::SetItemDefaultFocus();

			HoverHandCursor();
		}
		ImGui::EndCombo();
	}

	HoverHandCursor();
	return temp != itemSelected;
}

bool UI::AddComboAdder(std::string_view label, std::vector<std::string>& items, std::string& itemSelected)
{
	std::string_view temp = itemSelected;

	if (ImGui::BeginCombo(label.data(), itemSelected.data()))
	{
		for (int n = 0; n < items.size() - 1; n++)
		{
			const bool is_selected = (itemSelected == items[n].data());
			if (ImGui::Selectable(items[n].data(), is_selected))
				itemSelected = items[n].data();

			if (is_selected)
				ImGui::SetItemDefaultFocus();

			HoverHandCursor();
		}

		ImGui::Text("__________");
		if (ImGui::Selectable(items[items.size() - 1].data()))
			itemSelected = items[items.size() - 1].data();
		HoverHandCursor();
		ImGui::EndCombo();
	}

	HoverHandCursor();
	return temp != itemSelected;
}

bool UI::AddComboFilter(std::string_view label, std::vector<std::string>& items, std::string& itemSelected, std::string_view filter)
{
	std::string_view temp = itemSelected;

	if (ImGui::BeginCombo(label.data(), itemSelected.data()))
	{
		for (int n = 0; n < items.size(); n++)
		{
			if (items[n].find(filter) == std::string::npos)
				continue;

			const bool is_selected = (itemSelected == items[n].data());
			if (ImGui::Selectable(items[n].data(), is_selected))
				itemSelected = items[n].data();

			if (is_selected)
				ImGui::SetItemDefaultFocus();

			HoverHandCursor();
		}
		ImGui::EndCombo();
	}

	HoverHandCursor();
	return temp != itemSelected;
}

int UI::AddPopup(std::string_view label, std::vector<std::string>& items)
{
	int result = -2;

	if (ImGui::BeginPopup(label.data()))
	{
		result++;
		for (int i = 0; i < items.size(); i++)
		{
			if (items[i] == "##Separator")
				ImGui::Separator();
			else if (ImGui::Selectable(items[i].data()))
				result = i;
		}
		ImGui::EndPopup();
	}

	return result;
}

RENDERING_LIBRARY_API int UI::AddPopupWindow(std::string_view label, std::vector<std::string>& items)
{
	int result = -2;

	if (ImGui::BeginPopupContextWindow(label.data()))
	{
		result++;
		for (int i = 0; i < items.size(); i++)
		{
			if (items[i] == "##Separator")
				ImGui::Separator();
			else if (ImGui::Selectable(items[i].data()))
				result = i;
		}
		ImGui::EndPopup();
	}

	return result;
}

bool UI::AddSelectable(std::string_view label, float sizeX, float sizeY)
{
	bool result = ImGui::Selectable(label.data(), false, 0, ImVec2(sizeX, sizeY));

	HoverHandCursor();

	return result;
}

bool UI::AddSelectableDoubleClick(std::string_view label, float sizeX, float sizeY)
{
	bool result = false;
	
	if (ImGui::Selectable(label.data(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(sizeX, sizeY)))
	{
		result = ImGui::IsMouseDoubleClicked(0);
	}

	HoverHandCursor();
	
	return result;
}

bool UI::AddColorPicker3(std::string_view label, float* color)
{
	bool result = ImGui::ColorEdit3(label.data(), color, ImGuiColorEditFlags_Float);
	HoverHandCursor();
	return result;
}

bool UI::AddColorPicker4(std::string_view label, float* color)
{
	bool result = ImGui::ColorEdit4(label.data(), color, ImGuiColorEditFlags_Float);
	HoverHandCursor();
	return result;
}

void UI::OpenPopup(std::string_view label)
{
	ImGui::OpenPopup(label.data());
}

std::pair<float, float> UI::CalcTextSize(std::string_view text)
{
	const ImVec2 vec = ImGui::CalcTextSize(text.data());
	return std::pair(vec.x, vec.y);
}

void UI::Separator()
{
	ImGui::Separator();
}

void UI::AutoScroll()
{
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);
}

bool UI::AddMenuItem(std::string_view label, bool isSelected)
{
	return ImGui::MenuItem(label.data(), NULL, isSelected);
}

bool UI::AddMenuBar()
{
	return ImGui::BeginMenuBar();
}

void UI::CloseMenuBar()
{
	ImGui::EndMenuBar();
}

bool UI::AddMainMenuBar()
{
	return ImGui::BeginMainMenuBar();
}

void UI::CloseMainMenuBar()
{
	ImGui::EndMainMenuBar();
}

bool UI::AddMenu(std::string_view label)
{
	bool result = ImGui::BeginMenu(label.data());
	UI::HoverHandCursor();
	return result;
}

void UI::CloseMenu()
{
	ImGui::EndMenu();
}

bool UI::AddNode(std::string_view label, bool isSelected, bool openOnArrow, bool defaultOpen)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;

	if (isSelected)
		flags |= ImGuiTreeNodeFlags_Selected;
	if (openOnArrow)
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	if (defaultOpen)
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
	
	bool result = ImGui::TreeNodeEx(label.data(), flags);

	HoverHandCursor();
	return result;
}

void UI::CloseNode()
{
	ImGui::TreePop();
}

bool UI::AddLeaf(std::string_view label, bool isSelected)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (isSelected)
		flags |= ImGuiTreeNodeFlags_Selected;

	bool result = ImGui::TreeNodeEx(label.data(), flags);

	HoverHandCursor();
	return result;
}

bool UI::AddTable(std::string_view ID, int columns, bool showBorder)
{
	if(!showBorder)
		return ImGui::BeginTable(ID.data(), columns);
	else
		return ImGui::BeginTable(ID.data(), columns, ImGuiTableFlags_Borders);
}

void UI::CloseTable()
{
	ImGui::EndTable();
}

void UI::TableNextRow()
{
	ImGui::TableNextRow();
}

void UI::TableNextColumn()
{
	ImGui::TableNextColumn();
}

void UI::TableSetColumnIndex(int index)
{
	ImGui::TableSetColumnIndex(index);
}

void UI::TableSetUpColumn(std::string_view label, bool widthFixed, float width)
{
	if(widthFixed)
		ImGui::TableSetupColumn(label.data(), ImGuiTableColumnFlags_WidthFixed, width);
	else
		ImGui::TableSetupColumn(label.data(), ImGuiTableColumnFlags_WidthStretch, width);
}

void UI::Indent()
{
	ImGui::Indent();
}

void UI::Unindent()
{
	ImGui::Unindent();
}

void UI::SetNextItemOpen(bool isOpen)
{
	ImGui::SetNextItemOpen(isOpen);
}

bool UI::IsHover()
{
	return ImGui::IsItemHovered();
}

bool UI::IsClicked(int mouseButton)
{
	return ImGui::IsItemClicked(mouseButton);
}

bool UI::IsDoubleClicked()
{
	return ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);
}

bool UI::IsTreeLabelClicked(int mouseButton)
{
	return ImGui::IsItemClicked(mouseButton) && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing();
}

bool UI::IsTreeLabelDoubleClicked()
{
	return UI::IsDoubleClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing();
}

void UI::CloseCurrentPopup()
{
	ImGui::CloseCurrentPopup();
}

void UI::GlobalDockSpace()
{
	ImGui::DockSpaceOverViewport();
}

bool UI::NewWindow(std::string_view name, bool horizScrollbar, bool* isOpen, bool isEditable, bool shipping)
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

	if (horizScrollbar)
		flags |= ImGuiWindowFlags_HorizontalScrollbar;
	if(!isEditable)
		flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	if(shipping)
	{
		flags |= ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiDockNodeFlags_NoResize;

		SetNextWindowPos(0.0f, 0.0f);
		SetNextWindowSize(GetUI()->GetIOWidth(), GetUI()->GetIOHeight());
	}

	bool result = ImGui::Begin(name.data(), isOpen, flags);

	if (ImGui::GetWindowDockID())
	{
		ImGuiDockNode* Node = ImGui::DockBuilderGetNode(ImGui::GetWindowDockID());
		Node->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton;
		if(isEditable)
			Node->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton;
	}

	return result;
}

bool UI::NewWindowWithMenuBar(std::string_view name, bool horizScrollbar, bool* isOpen)
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse;

	if(horizScrollbar)
		flags |= ImGuiWindowFlags_HorizontalScrollbar;

	bool result = ImGui::Begin(name.data(), isOpen, flags);

	if (ImGui::GetWindowDockID())
	{
		ImGuiDockNode* Node = ImGui::DockBuilderGetNode(ImGui::GetWindowDockID());
		Node->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;
	}

	return result;
}

bool UI::NewChild(std::string_view ID, float sizeX, float sizeY, bool border, bool scroll)
{
	if(scroll)
		return ImGui::BeginChild(ID.data(), ImVec2(sizeX, sizeY), border);
	else
		return ImGui::BeginChild(ID.data(), ImVec2(sizeX, sizeY), border, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

}

void UI::EndChild()
{
	ImGui::EndChild();
}

void UI::EndWindow()
{
	ImGui::End();
}

void UI::NoPadding()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}


void UI::SetNextItemWitdh(float width)
{
	ImGui::SetNextItemWidth(width);
}

void UI::SetNextWindowPos(float x, float z)
{
	 ImGui::SetNextWindowPos(ImVec2(x, z));
}

void UI::SetNextWindowSize(float width, float height)
{
	ImGui::SetNextWindowSize(ImVec2(width, height));
}

void UI::SetNextWindowSizeConstraints(float sizeMinX, float sizeMinY, float sizeMaxX, float sizeMaxY)
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(sizeMinX, sizeMinY), ImVec2(sizeMaxX, sizeMaxY));
}

void UI::SameLine(float offset)
{
	return ImGui::SameLine(offset);
}

void UI::AddFont(std::pair<std::string, int> font)
{
	NIX::Rendering::UIManager::AddFontToLoad(font);
}

bool UI::PushFont(std::pair<std::string, int> font)
{
	if (font.first == "") return false;

	auto fontResult = NIX::Rendering::UIManager::GetFont(font);

	if (fontResult)
	{
		ImGui::PushFont(fontResult);
		return true;
	}

	AddFont(font);
	return false;
}

void UI::PopFont()
{
	ImGui::PopFont();
}

void UI::PushStyleColor(LibMath::Vector4 color)
{
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, {color.x, color.y, color.z, color.w});
}

void UI::PopStyleColor()
{
	ImGui::PopStyleColor();
}

void UI::PushWindowBgColor(LibMath::Vector4 color)
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(color.x, color.y, color.z, color.w));
}

void UI::PopWindowBgColor()
{
	ImGui::PopStyleColor();
}

void UI::PushFramePadding(float width, float height)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(width, height));
}

void UI::PopStyle()
{
	ImGui::PopStyleVar();
}

bool UI::DragDropSource(std::string_view type, std::string_view preview, int data)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload(type.data(), &data, sizeof(int));
		ImGui::Text(preview.data());
		ImGui::EndDragDropSource();

		return true;
	}

	return false;
}

bool UI::DragDropTarget(std::string_view type)
{
	bool result = false;

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type.data()))
		{
			result = true;
		}
		ImGui::EndDragDropTarget();
	}

	return result;
}

bool UI::Test()
{
	return true;
}

void UI::Demo()
{
	ImGui::ShowDemoWindow();
}
