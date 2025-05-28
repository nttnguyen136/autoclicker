// src/platform/macos/MacOSPlatform.cpp
#include "MacOSPlatform.h"
#include <iostream>

// Include the macOS-specific CoreGraphics framework for simulating mouse events.
#include <ApplicationServices/ApplicationServices.h>

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