// src/platform/macos/MacOSPlatform.cpp
#include "MacOSPlatform.h"
#include <iostream>

// Include the macOS-specific CoreGraphics framework for simulating mouse events.
#include <ApplicationServices/ApplicationServices.h>

MacOSPlatform::MacOSPlatform()
    : m_eventTap(nullptr), m_runLoopSource(nullptr), m_eventTapRunLoop(nullptr), m_clickCallback(nullptr), m_stopEventTapThread(false) {}

MacOSPlatform::~MacOSPlatform()
{
    StopGlobalMouseListener();
}

void MacOSPlatform::SimulateMouseClick()
{
    // Get the current location of the mouse cursor.
    CGEventRef locationEvent = CGEventCreate(NULL);
    CGPoint cursorLocation = CGEventGetLocation(locationEvent);
    CFRelease(locationEvent);

    std::cout << "Simulating a click at (" << cursorLocation.x << ", " << cursorLocation.y << ")" << std::endl;

    // Create a left mouse down event.
    CGEventRef mouseDownEvent = CGEventCreateMouseEvent(
        NULL, kCGEventLeftMouseDown,
        cursorLocation, kCGMouseButtonLeft);

    // Create a left mouse up event.
    CGEventRef mouseUpEvent = CGEventCreateMouseEvent(
        NULL, kCGEventLeftMouseUp,
        cursorLocation, kCGMouseButtonLeft);

    // Post the events to the system event queue.
    // The events are posted to the location of the current process, which is what we want.
    CGEventPost(kCGHIDEventTap, mouseDownEvent);
    CGEventPost(kCGHIDEventTap, mouseUpEvent);

    // Release the event objects.
    CFRelease(mouseDownEvent);
    CFRelease(mouseUpEvent);
}

// The C-style callback function for the event tap
CGEventRef MacOSPlatform::HandleTapEvent(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userInfo)
{
    // 'userInfo' is the pointer we passed when creating the tap (our MacOSPlatform instance)
    MacOSPlatform *self = static_cast<MacOSPlatform *>(userInfo);

    if (self && self->m_clickCallback)
    {
        // We are interested in left mouse down events for picking
        if (type == kCGEventLeftMouseDown)
        {
            CGPoint point = CGEventGetLocation(event);
            // Invoke the C++ callback function
            self->m_clickCallback(static_cast<int>(point.x), static_cast<int>(point.y));
        }
    }
    // We must return the event for it to be passed on to other applications.
    // If we wanted to consume/block the event, we could return NULL.
    return event;
}

bool MacOSPlatform::StartGlobalMouseListener(GlobalMouseClickCallback callback)
{
    if (m_eventTap)
    { // Already listening
        return true;
    }

    m_clickCallback = callback;
    m_stopEventTapThread.store(false);

    // Define which events we want to listen for.
    // For picking coordinates, we typically care about a mouse down event.
    CGEventMask eventMask = (1 << kCGEventLeftMouseDown) | (1 << kCGEventRightMouseDown);

    // Create the event tap.
    // kCGHIDEventTap: Taps into hardware-generated events.
    // kCGHeadInsertEventTap: Insert our tap at the head of the event stream.
    // kCGEventTapOptionDefault: Listen to events, don't modify them by default.
    // HandleTapEvent: Our C-style callback function.
    // this: Pass a pointer to the current MacOSPlatform instance as userInfo.
    m_eventTap = CGEventTapCreate(
        kCGHIDEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionListenOnly, // kCGEventTapOptionDefault allows modification
        eventMask,
        HandleTapEvent,
        this);

    if (!m_eventTap)
    {
        std::cerr << "ERROR: Failed to create event tap. Check Accessibility Permissions!" << std::endl;
        std::cerr << "Go to System Settings > Privacy & Security > Accessibility, and add this application." << std::endl;
        m_clickCallback = nullptr; // Clear callback if failed
        return false;
    }

    // Create a run loop source for the event tap.
    m_runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, m_eventTap, 0);
    if (!m_runLoopSource)
    {
        std::cerr << "ERROR: Failed to create run loop source." << std::endl;
        CFRelease(m_eventTap);
        m_eventTap = nullptr;
        m_clickCallback = nullptr;
        return false;
    }

    // Start a new thread to run the event tap's run loop.
    // This is crucial because CFRunLoopRun() is a blocking call.
    // If we ran it on the main thread, our UI would freeze.
    m_eventTapThread = std::thread([this]()
                                   {
        // Get the run loop for this new thread.
        m_eventTapRunLoop = CFRunLoopGetCurrent();

        // Add the event tap source to this thread's run loop.
        CFRunLoopAddSource(m_eventTapRunLoop, m_runLoopSource, kCFRunLoopDefaultMode);

        // Enable the event tap.
        CGEventTapEnable(m_eventTap, true);

        std::cout << "Global mouse listener started on a dedicated thread." << std::endl;

        // Run the loop until StopGlobalMouseListener is called.
        while (!m_stopEventTapThread.load()) {
            // Process events for a short period, then check the stop flag.
            // This makes the loop responsive to the stop signal.
            CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.1, true); // true = return after source handled
        }

        // Cleanup when the loop exits
        CGEventTapEnable(m_eventTap, false);
        CFRunLoopRemoveSource(m_eventTapRunLoop, m_runLoopSource, kCFRunLoopDefaultMode);
        std::cout << "Global mouse listener run loop exited." << std::endl; });

    return true;
}

void MacOSPlatform::StopGlobalMouseListener()
{
    if (!m_eventTap)
    { // Not listening
        return;
    }

    // Signal the event tap thread to stop
    m_stopEventTapThread.store(true);

    // If the run loop reference is valid, stop it.
    // This will cause CFRunLoopRunInMode or CFRunLoopRun to return.
    if (m_eventTapRunLoop)
    {
        CFRunLoopStop(m_eventTapRunLoop);
    }

    // Wait for the event tap thread to finish its cleanup and exit.
    if (m_eventTapThread.joinable())
    {
        m_eventTapThread.join();
    }

    // Release Core Foundation objects if they were created.
    // The m_runLoopSource is invalidated when removed from the run loop,
    // and the tap itself is invalidated when the mach port is.
    // Check if they are not NULL before releasing.
    if (m_runLoopSource)
    {
        CFRelease(m_runLoopSource);
        m_runLoopSource = nullptr;
    }
    if (m_eventTap)
    {
        CFRelease(m_eventTap);
        m_eventTap = nullptr;
    }

    m_clickCallback = nullptr;
    m_eventTapRunLoop = nullptr;
    std::cout << "Global mouse listener stopped and cleaned up." << std::endl;
}