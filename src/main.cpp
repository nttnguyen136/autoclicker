// src/main.cpp

#include "core/Application.h"
#include <SDL3/SDL_main.h> // Required for cross-platform main function handling

int main(int argc, char *argv[])
{
    // Create an instance of our application
    Application app;

    // Run the application and return its exit code
    return app.Run();
}