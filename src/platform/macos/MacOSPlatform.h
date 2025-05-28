// src/platform/macos/MacOSPlatform.h
#pragma once

#include "platform/IPlatform.h"

// This class is the concrete implementation of the IPlatform interface for macOS.
class MacOSPlatform : public IPlatform
{
public:
    // The 'override' keyword ensures we are correctly implementing the virtual function.
    void SimulateMouseClick() override;
};