#pragma once
#include <map>
#include <string>
#include <vector>

#include "Vector/Vector4.h"

#include "API/DLLExportRendering.h"

class UI
{
private:
	inline static UI* m_singleton = nullptr;
public:
	UI() = default;

	RENDERING_LIBRARY_API static UI* GetUI();
		
	RENDERING_LIBRARY_API void InitDocking();

	RENDERING_LIBRARY_API std::pair<float, float> GetCursorPos();
	RENDERING_LIBRARY_API float GetCursorPosX();
	RENDERING_LIBRARY_API float GetCursorPosY();

	RENDERING_LIBRARY_API std::pair<float, float> GetContentRegionAvail();
	RENDERING_LIBRARY_API float GetContentRegionAvailX();
	RENDERING_LIBRARY_API float GetContentRegionAvailY();

	RENDERING_LIBRARY_API std::pair<float, float> GetWindowPos();

	RENDERING_LIBRARY_API std::pair<float, float> GetWindowSize();
	RENDERING_LIBRARY_API float GetWindowHeight();
	RENDERING_LIBRARY_API float GetWindowWidth();

	RENDERING_LIBRARY_API std::pair<float, float> GetIOSize();
	RENDERING_LIBRARY_API float GetIOWidth();
	RENDERING_LIBRARY_API float GetIOHeight();

	RENDERING_LIBRARY_API void HoverHandCursor();

	RENDERING_LIBRARY_API void SetCursorPos(float posX, float posY);
	RENDERING_LIBRARY_API void SetCursorPosX(float posX);
	RENDERING_LIBRARY_API void SetCursorPosY(float posY);

	RENDERING_LIBRARY_API bool AddCheckbox(std::string_view label, bool& isSelected);
	RENDERING_LIBRARY_API bool AddButton(std::string_view label, float sizeX, float sizeY);
	RENDERING_LIBRARY_API void AddImage(void *& textureID, float sizeX, float sizeY);
	RENDERING_LIBRARY_API void AddText(std::string_view text);
	RENDERING_LIBRARY_API void AddTextDisabled(std::string_view text);
	RENDERING_LIBRARY_API void AddTextColored(std::string_view text, LibMath::Vector4 color);
	RENDERING_LIBRARY_API void AddMarker(std::string_view text);
	RENDERING_LIBRARY_API bool AddInt(std::string_view label, int& nbr, bool positiveOnly = false);
	RENDERING_LIBRARY_API bool AddFloat(std::string_view label, float& nbr, bool positiveOnly = false);
	RENDERING_LIBRARY_API bool AddFloat2(std::string_view label, float* vec, bool positiveOnly = false);
	RENDERING_LIBRARY_API bool AddFloat3(std::string_view label, float* vec, bool positiveOnly = false);
	RENDERING_LIBRARY_API bool AddInputText(std::string_view label, std::string_view hint, std::string& buf, bool readOnly = false);
	RENDERING_LIBRARY_API bool AddCombo(std::string_view label, std::vector<std::string>& items, std::string& itemSelected);
	RENDERING_LIBRARY_API bool AddComboAdder(std::string_view label, std::vector<std::string>& items, std::string& itemSelected);
	RENDERING_LIBRARY_API bool AddComboFilter(std::string_view label, std::vector<std::string>& items, std::string& itemSelected, std::string_view filter);
	RENDERING_LIBRARY_API int AddPopup(std::string_view label, std::vector<std::string>& items);
	RENDERING_LIBRARY_API int AddPopupWindow(std::string_view label, std::vector<std::string>& items);
	RENDERING_LIBRARY_API bool AddSelectable(std::string_view label, float sizeX, float sizeY);
	RENDERING_LIBRARY_API bool AddSelectableDoubleClick(std::string_view label, float sizeX, float sizeY);
	RENDERING_LIBRARY_API bool AddColorPicker3(std::string_view label, float* color);
	RENDERING_LIBRARY_API bool AddColorPicker4(std::string_view label, float* color);
	
	RENDERING_LIBRARY_API std::pair<float, float> CalcTextSize(std::string_view text);

	RENDERING_LIBRARY_API void Separator();
	
	RENDERING_LIBRARY_API void AutoScroll();

	RENDERING_LIBRARY_API void OpenPopup(std::string_view label);

	RENDERING_LIBRARY_API bool AddMenuItem(std::string_view label, bool isSelected);

	RENDERING_LIBRARY_API bool AddMenuBar();
	RENDERING_LIBRARY_API void CloseMenuBar();

	RENDERING_LIBRARY_API bool AddMainMenuBar();
	RENDERING_LIBRARY_API void CloseMainMenuBar();

	RENDERING_LIBRARY_API bool AddMenu(std::string_view label);
	RENDERING_LIBRARY_API void CloseMenu();

	RENDERING_LIBRARY_API bool AddNode(std::string_view label, bool isSelected = false, bool openOnArrow = false, bool defaultOpen = false);
	RENDERING_LIBRARY_API void CloseNode();
	
	RENDERING_LIBRARY_API bool AddLeaf(std::string_view label, bool isSelected = false);

	RENDERING_LIBRARY_API bool AddTable(std::string_view ID, int columns, bool showBorder = false);
	RENDERING_LIBRARY_API void CloseTable();
	
	RENDERING_LIBRARY_API void TableNextRow();
	RENDERING_LIBRARY_API void TableNextColumn();
	RENDERING_LIBRARY_API void TableSetColumnIndex(int index);
	RENDERING_LIBRARY_API void TableSetUpColumn(std::string_view label, bool widthFixed = true, float width = -1.f);

	RENDERING_LIBRARY_API void Indent();
	RENDERING_LIBRARY_API void Unindent();

	RENDERING_LIBRARY_API void SetNextItemOpen(bool isOpen = true);
	
	RENDERING_LIBRARY_API bool IsHover();
	RENDERING_LIBRARY_API bool IsClicked(int mouseButton = 0);
	RENDERING_LIBRARY_API bool IsDoubleClicked();
	RENDERING_LIBRARY_API bool IsTreeLabelClicked(int mouseButton = 0);
	RENDERING_LIBRARY_API bool IsTreeLabelDoubleClicked();

	RENDERING_LIBRARY_API void CloseCurrentPopup();

	RENDERING_LIBRARY_API void GlobalDockSpace();
	
	RENDERING_LIBRARY_API bool NewWindow(std::string_view name, bool horizScrollbar = false, bool* isOpen = (bool*)0, bool isEditable = true, bool shipping = false);
	RENDERING_LIBRARY_API bool NewWindowWithMenuBar(std::string_view name, bool horizScrollbar = false, bool* isOpen = (bool*)0);

	RENDERING_LIBRARY_API bool NewChild(std::string_view ID, float sizeX = 0.f, float sizeY = 0.f, bool border = false, bool scroll = true);
	RENDERING_LIBRARY_API void EndChild();

	RENDERING_LIBRARY_API void EndWindow();

	RENDERING_LIBRARY_API void NoPadding();

	RENDERING_LIBRARY_API void SetNextItemWitdh(float width);

	RENDERING_LIBRARY_API void SetNextWindowPos(float x, float z);
	RENDERING_LIBRARY_API void SetNextWindowSize(float width, float height);
	RENDERING_LIBRARY_API void SetNextWindowSizeConstraints(float sizeMinX = -1.f, float sizeMinY = -1.f, float sizeMaxX = -1.f, float sizeMaxY = -1.f);

	RENDERING_LIBRARY_API void SameLine(float offset = 0.f);

	RENDERING_LIBRARY_API void AddFont(std::pair<std::string, int> font);
	RENDERING_LIBRARY_API bool PushFont(std::pair<std::string, int> font);
	RENDERING_LIBRARY_API void PopFont();

	RENDERING_LIBRARY_API void PushStyleColor(LibMath::Vector4 color);
	RENDERING_LIBRARY_API void PopStyleColor();

	RENDERING_LIBRARY_API void PushWindowBgColor(LibMath::Vector4 color);
	RENDERING_LIBRARY_API void PopWindowBgColor();

	RENDERING_LIBRARY_API void PushFramePadding(float width, float height);
	RENDERING_LIBRARY_API void PopStyle();

	RENDERING_LIBRARY_API bool DragDropSource(std::string_view type, std::string_view preview, int data = 0);
	RENDERING_LIBRARY_API bool DragDropTarget(std::string_view type);
		
	RENDERING_LIBRARY_API bool Test();
	RENDERING_LIBRARY_API void Demo();
};
