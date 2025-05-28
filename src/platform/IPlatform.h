// src/platform/IPlatform.h
#pragma once

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

    // We can add more platform-specific functions here later, e.g.:
    // virtual void GetMousePosition(int& x, int& y) = 0;
};