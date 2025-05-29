// src/platform/macos/MacOSPlatform.h
#pragma once

#include "platform/IPlatform.h"
#include <ApplicationServices/ApplicationServices.h> // For Core Graphics types
#include <functional>
#include <thread>

// This class is the concrete implementation of the IPlatform interface for macOS.
class MacOSPlatform : public IPlatform
{
public:
    MacOSPlatform();
    ~MacOSPlatform();

    void SimulateMouseClick() override;
    bool StartGlobalMouseListener(GlobalMouseClickCallback callback) override;
    void StopGlobalMouseListener() override;

private:
    // The Core Graphics event tap.
    CFMachPortRef m_eventTap;
    // The run loop source for the event tap.
    CFRunLoopSourceRef m_runLoopSource;
    // The run loop itself, will run on a dedicated thread.
    CFRunLoopRef m_eventTapRunLoop;
    // The dedicated thread for the event tap's run loop.
    std::thread m_eventTapThread;
    // Stores the callback function provided by the Application class.
    GlobalMouseClickCallback m_clickCallback;
    // Flag to signal the event tap thread to stop.
    std::atomic<bool> m_stopEventTapThread;

    // The C-style callback function that Core Graphics will invoke.
    static CGEventRef HandleTapEvent(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userInfo);
};