#pragma once

#include "Events/EventSystem.h"
#include "../EngineStuffs/Graphics/LillisWindow.h"
#include "Events/InputEvent.h"

class Engine;

class InputSystem
{
    // Main KeyInput functionality
public:
    // Takes a list of which keys to keep state for
    InputSystem(std::vector<LILLIS::KeyCode> keysToMonitor);
    ~InputSystem();
    // If this KeyInput is enabled and the given key is monitored,
    // returns pressed state.  Else returns false.
    bool getIsKeyDown(int key);
    // See _isEnabled for details
    bool getIsEnabled() { return _isEnabled; }
    void setIsEnabled(bool value) { _isEnabled = value; }
private:
    // Used internally to update key states.  Called by the GLFW callback.
    void setIsKeyDown(int key, bool isDown);
    // Map from monitored keyes to their pressed states
    std::map<int, bool> _keys;
    // If disabled, KeyInput.getIsKeyDown always returns false
    bool _isEnabled;

    // Workaround for C++ class using a c-style-callback
public:
    // Must be called before any KeyInput instances will work
    static void setupKeyInputs(const LillisWindow* window);
private:
    // The GLFW callback for key events.  Sends events to all KeyInput instances
    static void callback(
        GLFWwindow* window, int key, int scancode, int action, int mods);
    // Keep a list of all KeyInput instances and notify them all of key events
    static std::vector<InputSystem*> _instances;

    EventSystem* evSys;
};
