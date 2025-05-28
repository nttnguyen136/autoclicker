// src/ui/UI.cpp

#include "UI.h"
#include "common/AppState.h"
#include "imgui.h"

// Constructor initializes the state reference
UI::UI(AppState &appState) : m_appState(appState) {}

void UI::Render()
{
    // --- Main Control Window ---
    ImGui::Begin("AutoClicker Controls");

    ImGui::Text("Hello! This is the main control panel.");
    ImGui::Spacing();

    // Checkbox to control the clicker state (will be used later)
    ImGui::Checkbox("Start Clicking (F6)", &m_appState.isClicking);

    // Slider to control the click interval
    ImGui::SliderInt("Click Interval (ms)", &m_appState.clickIntervalMs, 10, 2000);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Perform Test Click"))
    {
        // When this button is clicked, we set the flag in the shared state.
        // The Application class will see this and perform the action.
        m_appState.testClickRequested = true;
    }
    ImGui::Spacing();
    // Checkbox to toggle the visibility of the ImGui Demo Window
    ImGui::Checkbox("Show ImGui Demo Window", &m_appState.showDemoWindow);

    ImGui::End();

    // --- Other Windows ---

    // Conditionally render the demo window based on the state
    if (m_appState.showDemoWindow)
    {
        ImGui::ShowDemoWindow(&m_appState.showDemoWindow);
    }
}