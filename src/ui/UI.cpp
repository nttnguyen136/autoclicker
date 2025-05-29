// src/ui/UI.cpp

#include "UI.h"
#include "common/AppState.h"
#include "imgui.h"
#include <vector> // Required for std::vector

// Helper function to convert ActionType enum to a string for display
const char *ActionTypeToString(ActionType type)
{
    switch (type)
    {
    case ActionType::LeftClick:
        return "Left Click";
    case ActionType::RightClick:
        return "Right Click";
    case ActionType::DoubleClick:
        return "Double Click";
    default:
        return "Unknown";
    }
}

// Constructor initializes the state reference
UI::UI(AppState &appState) : m_appState(appState) {}

void UI::Render()
{
    // We will use one main, non-movable window as the application's body
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                                    ImGuiWindowFlags_MenuBar;

    ImGui::Begin("AutoMouseClick", nullptr, window_flags);

    // --- Main Menu Bar ---
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save Script"))
            { /* TODO: Logic to save actions */
            }
            if (ImGui::MenuItem("Load Script"))
            { /* TODO: Logic to load actions */
            }
            if (ImGui::MenuItem("Customizer"))
            {
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
            {
                m_appState.quitRequested = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // --- 1. Add / Edit Action Panel ---
    ImGui::Text("Add / Edit Action");
    ImGui::Separator();

    // Input fields for X and Y coordinates
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("X-Coordinate", &m_appState.currentEditAction.x);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("Y-Coordinate", &m_appState.currentEditAction.y);
    ImGui::SameLine();
    if (ImGui::Button(m_appState.isPickingCoordinate ? "Picking... (Click Anywhere)" : "Pick Coordinate"))
    {
        if (m_appState.isPickingCoordinate)
        {
            // If already picking, cancel it
            m_appState.isPickingCoordinate = false;
        }
        else
        {
            // Start picking mode
            m_appState.isPickingCoordinate = true;
        }
    }

    // Action Type Dropdown (Combo box)
    const char *currentActionTypeStr = ActionTypeToString(m_appState.currentEditAction.type);
    ImGui::SetNextItemWidth(210);
    if (ImGui::BeginCombo("Action Type", currentActionTypeStr))
    {
        for (int i = 0; i < 5; ++i)
        {
            ActionType type = static_cast<ActionType>(i);
            const bool is_selected = (m_appState.currentEditAction.type == type);
            if (ImGui::Selectable(ActionTypeToString(type), is_selected))
            {
                m_appState.currentEditAction.type = type;
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    // Other controls
    // ImGui::Checkbox("Cursor Back", &m_appState.currentEditAction.cursorBack);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("Delay before Action", &m_appState.currentEditAction.delayMs);
    ImGui::Text("Milli Second(s)");

    char commentBuf[128];
    strncpy(commentBuf, m_appState.currentEditAction.comment.c_str(), sizeof(commentBuf) - 1);
    commentBuf[sizeof(commentBuf) - 1] = 0;
    ImGui::SetNextItemWidth(300);
    if (ImGui::InputText("Comment", commentBuf, sizeof(commentBuf)))
    {
        m_appState.currentEditAction.comment = commentBuf;
    }

    // Action Buttons
    if (ImGui::Button("Add"))
    { /* TODO: Add logic in Application::Update */
    }
    ImGui::SameLine();
    if (ImGui::Button("Update"))
    { /* TODO: Add logic */
    }
    ImGui::SameLine();
    if (ImGui::Button("Load"))
    { /* TODO: Add logic */
    }
    ImGui::SameLine();
    if (ImGui::Button("Save"))
    { /* TODO: Add logic */
    }

    ImGui::Spacing();
    ImGui::Spacing();

    // --- 2. Action List Panel ---
    ImGui::Text("List Of Action(s) to Execute in Sequence");
    ImGui::Separator();

    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing() * 2;
    if (ImGui::BeginChild("ActionListChild", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_None))
    {
        // Table for actions
        if (ImGui::BeginTable("ActionTable", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
        {
            // Headers
            ImGui::TableSetupColumn("Sr");
            ImGui::TableSetupColumn("Action Type");
            ImGui::TableSetupColumn("X");
            ImGui::TableSetupColumn("Y");
            ImGui::TableSetupColumn("Cursor Back");
            ImGui::TableSetupColumn("Delay (ms)");
            ImGui::TableSetupColumn("Repeat");
            ImGui::TableHeadersRow();

            // Rows
            for (int i = 0; i < m_appState.actions.size(); ++i)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                // Use a Selectable to make the whole row clickable
                char label[32];
                sprintf(label, "%d", i + 1);
                if (ImGui::Selectable(label, m_appState.selectedActionIndex == i, ImGuiSelectableFlags_SpanAllColumns))
                {
                    m_appState.selectedActionIndex = i;
                    m_appState.currentEditAction = m_appState.actions[i]; // Load data to edit panel
                }

                // The rest of the columns
                ImGui::TableNextColumn();
                ImGui::Text("%s", ActionTypeToString(m_appState.actions[i].type));
                ImGui::TableNextColumn();
                ImGui::Text("%d", m_appState.actions[i].x);
                ImGui::TableNextColumn();
                ImGui::Text("%d", m_appState.actions[i].y);
                // ImGui::TableNextColumn();
                // ImGui::Text("%s", m_appState.actions[i].cursorBack ? "Yes" : "No");
                ImGui::TableNextColumn();
                ImGui::Text("%d", m_appState.actions[i].delayMs);
                // ImGui::TableNextColumn();
                // ImGui::Text("%d", m_appState.actions[i].repeatCount);
            }
            ImGui::EndTable();
        }
    }
    ImGui::EndChild();

    // --- 3. Control Buttons for the list ---
    if (ImGui::Button("Start"))
    { /* TODO: Logic */
    }
    ImGui::SameLine();
    if (ImGui::Button("Move Up"))
    { /* TODO: Logic */
    }
    ImGui::SameLine();
    if (ImGui::Button("Move Down"))
    { /* TODO: Logic */
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete"))
    { /* TODO: Logic */
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete All"))
    { /* TODO: Logic */
    }

    // --- 4. Status Bar ---
    ImGui::Separator();
    ImGui::Text("Currently Mouse Cursor At X = %d, Y = %d", m_appState.currentMouseX, m_appState.currentMouseY);

    ImGui::End(); // End of the main window
}