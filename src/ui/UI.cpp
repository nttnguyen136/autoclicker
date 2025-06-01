#include "UI.h"
#include "common/AppState.h"
#include "common/Action.h"
#include "imgui.h"
#include <vector>
#include <cstring>

// Define a named constant for status bar padding ratio
constexpr float STATUS_BAR_PADDING_RATIO = 1.3f;

UI::UI(AppState &appState) : m_appState(appState) {}

void UI::Render()
{
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                                    ImGuiWindowFlags_MenuBar;

    ImGui::Begin("AutoMouseClick", nullptr, window_flags);

    RenderMenuBar();
    RenderMainView();
    RenderStatusBar();

    ImGui::End();
}

void UI::RenderMenuBar()
{
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
                m_appState.quitRequested = true;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}
void UI::RenderMainView()
{
    // TODO: Implement the main UI content rendering logic here.
}

void UI::RenderStatusBar()
{

    float status_bar_height = ImGui::GetFrameHeightWithSpacing();
    float window_height = ImGui::GetWindowHeight();
    float cursor_y = ImGui::GetCursorPosY();
    float space = window_height - cursor_y - status_bar_height * STATUS_BAR_PADDING_RATIO;
    if (space > 0)
        ImGui::Dummy(ImVec2(0, space)); // Add space to fill the window

    ImGui::Separator();

    // Display the current mouse position in the status bar
    char statusText[128];
    snprintf(statusText, sizeof(statusText), "Currently Mouse Cursor At X = %d, Y = %d", m_appState.currentMouseX, m_appState.currentMouseY);

    // Set cursor position to the right side of the window
    float windowWidth = ImGui::GetWindowWidth();
    float textWidth = ImGui::CalcTextSize(statusText).x;
    ImGui::SetCursorPosX(windowWidth - textWidth - ImGui::GetStyle().ItemSpacing.x);

    ImGui::TextUnformatted(statusText);
}