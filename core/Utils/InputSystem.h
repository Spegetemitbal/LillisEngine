#pragma once

#include <glm/vec2.hpp>

#include "Events/EventSystem.h"
#include "../EngineStuffs/Graphics/LillisWindow.h"
#include "Events/InputEvent.h"

class Engine;

namespace LILLIS
{
    enum MouseSetting
    {
        NORMAL_CURSOR,
        HIDDEN_CURSOR,
        DISABLED_CURSOR
    };
}

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
    bool getIsMouseButtonDown(int key);
    // See _isEnabled for details
    bool getIsEnabled() { return _isEnabled; }
    void setIsEnabled(bool value) { _isEnabled = value; }


    glm::vec2 getMousePosition() const {return {mouseXpos, mouseYPos};}

    void setMouseType(LILLIS::MouseSetting ms);
    LILLIS::MouseSetting getMouseType() { return mouseSetting; }

    void UpdateControllers();

private:
    // Used internally to update key states.  Called by the GLFW callback.
    void setIsKeyDown(int key, bool isDown);
    void setMouseIsKeyDown(int key, bool isDown);
    // Map from monitored keyes to their pressed states
    std::map<int, bool> _keys;
    std::vector<bool> _mouseButtons;
    // If disabled, KeyInput.getIsKeyDown always returns false
    bool _isEnabled;

    double mouseXpos = 0, mouseYPos = 0;

    static unsigned int numControllersConnected;
    static std::vector<GLFWgamepadstate> _gamepadStates;

    // Workaround for C++ class using a c-style-callback
public:
    // Must be called before any KeyInput instances will work
    void setupKeyInputs(const LillisWindow* window);
private:
    // The GLFW callback for key events.  Sends events to all KeyInput instances
    static void keyboard_callback(
        GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_pos_callback(
        GLFWwindow* window, double xPos, double yPos);
    static void mouse_button_callback(
        GLFWwindow* window, int button, int action, int mods);
    static void joystick_callback(
        int jid, int event);

    static void checkNumControllers();
    // Keep a list of all KeyInput instances and notify them all of key events
    static std::vector<InputSystem*> _instances;

    const LillisWindow* _window;

    LILLIS::MouseSetting mouseSetting = LILLIS::MouseSetting::NORMAL_CURSOR;

    EventSystem* evSys;
};
