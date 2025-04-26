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

    InputSystem() = default;
    ~InputSystem() = default;
    // If this KeyInput is enabled and the given key is monitored,
    // returns pressed state.  Else returns false.
    static bool getIsKeyDown(int key);
    static bool getIsMouseButtonDown(int key);
    // See _isEnabled for details
    static inline bool getIsEnabled() { return _isEnabled; }
    static inline void setIsEnabled(bool value) { _isEnabled = value; }

    static void addKeyCodes(const std::vector<LILLIS::KeyCode>& keysToMonitor);

    static glm::vec2 getMousePosition() {return {mouseXpos, mouseYPos};}

    static void setMouseType(LILLIS::MouseSetting ms);
    static LILLIS::MouseSetting getMouseType() { return mouseSetting; }

    static void UpdateControllers();

    // Must be called before any KeyInput instances will work
    static void Init(const LillisWindow* window);

private:

    // Used internally to update key states.  Called by the GLFW callback.
    static void setIsKeyDown(int key, bool isDown);
    static void setMouseIsKeyDown(int key, bool isDown);
    // Map from monitored keyes to their pressed states
    static inline std::map<int, bool> _keys = std::map<int, bool>();
    //3 mouse buttons, left right and center! why is this a vector? good question.
    //TODO make this not a vector.
    static inline std::vector<bool> _mouseButtons = {false, false, false};
    // If disabled, KeyInput.getIsKeyDown always returns false
    static inline bool _isEnabled = true;

    static inline double mouseXpos = 0, mouseYPos = 0;

    static unsigned int numControllersConnected;
    static std::vector<GLFWgamepadstate> _gamepadStates;

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

    static inline const LillisWindow* _window = nullptr;

    static inline LILLIS::MouseSetting mouseSetting = LILLIS::MouseSetting::NORMAL_CURSOR;
};
