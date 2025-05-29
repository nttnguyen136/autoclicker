// src/platform/IPlatform.h
#pragma once
#include <functional>

// This is an interface class that defines a "contract" for platform-specific functionality.
// Any class that implements this interface must provide its own version of these functions.
class IPlatform
{
public:
    // Virtual destructor is required for interface classes.
    virtual ~IPlatform() = default;

    // A pure virtual function to simulate a mouse click at the current cursor position.
    // '= 0' makes this a pure virtual function, turning this class into an interface.
    virtual void SimulateMouseClick() = 0;

    // Type alias for our callback function
    // The callback will receive x, y coordinates of the click
    using GlobalMouseClickCallback = std::function<void(int x, int y)>;

    // Starts listening for global mouse clicks.
    // Takes a callback function that will be invoked when a click occurs.
    // Returns true if successful, false otherwise (e.g., permissions issue).
    virtual bool StartGlobalMouseListener(GlobalMouseClickCallback callback) = 0;

    // Stops listening for global mouse clicks.
    virtual void StopGlobalMouseListener() = 0;
};