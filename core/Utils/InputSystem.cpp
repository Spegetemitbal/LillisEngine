#include "InputSystem.h"

#include "Events/ControllerAxisEvent.h"
#include "Events/ControllerButtonEvent.h"
#include "Events/MouseEvent.h"

std::vector<GLFWgamepadstate> InputSystem::_gamepadStates;

unsigned int InputSystem::numControllersConnected = 0;

void InputSystem::addKeyCodes(const std::vector<LILLIS::KeyCode>& keysToMonitor)
{
    for (int key : keysToMonitor) {
        _keys[key] = false;
    }
}

bool InputSystem::getIsKeyDown(int key) {
    bool result = false;
    if (_isEnabled) {
        std::map<int, bool>::iterator it = _keys.find(key);
        if (it != _keys.end()) {
            result = _keys[key];
        }
    }
    return result;
}

bool InputSystem::getIsMouseButtonDown(int key)
{
    if (key > 0 && key < 3)
    {
        return _mouseButtons[key];
    }
    return false;
}


void InputSystem::setIsKeyDown(int key, bool isDown) {
    std::map<int, bool>::iterator it = _keys.find(key);
    if (it != _keys.end()) {
        _keys[key] = isDown;
        EventSystem::getInstance()->fireEvent(InputEvent(key, isDown));
    }
}

void InputSystem::setMouseIsKeyDown(int key, bool isDown)
{
    _mouseButtons[key] = isDown;
    EventSystem::getInstance()->fireEvent(MouseEvent(key, isDown));
}


void InputSystem::Init(const LillisWindow* window) {
    _window = window;
    glfwSetKeyCallback(window->window, InputSystem::keyboard_callback);
    glfwSetCursorPosCallback(window->window, InputSystem::mouse_pos_callback);
    glfwSetMouseButtonCallback(window->window, InputSystem::mouse_button_callback);
    glfwSetJoystickCallback(InputSystem::joystick_callback);
}

//Eventually add an ENUM abstraction.
void InputSystem::keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Send key event to all KeyInput instances
    setIsKeyDown(key, action != GLFW_RELEASE);
}

void InputSystem::mouse_pos_callback(GLFWwindow *window, double xPos, double yPos)
{
    mouseXpos = xPos;
    mouseYPos = yPos;
}

void InputSystem::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    setMouseIsKeyDown(button, action != GLFW_RELEASE);
}

void InputSystem::joystick_callback(int jid, int event)
{
    if (event == GLFW_CONNECTED)
    {
        checkNumControllers();
    }
    if (event == GLFW_DISCONNECTED)
    {
        checkNumControllers();
    }
    //Add event for this eventually.
}


void InputSystem::setMouseType(LILLIS::MouseSetting ms)
{
    if (_window == nullptr)
    {
        return;
    }

    mouseSetting = ms;

    switch (ms)
    {
        case LILLIS::NORMAL_CURSOR:
            glfwSetInputMode(_window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case LILLIS::HIDDEN_CURSOR:
            glfwSetInputMode(_window->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        case LILLIS::DISABLED_CURSOR:
            glfwSetInputMode(_window->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
    }
}

void InputSystem::checkNumControllers()
{
    numControllersConnected = 0;
    for (int i = 0; i < GLFW_JOYSTICK_LAST; i++)
    {
        int present = glfwJoystickIsGamepad(i);
        numControllersConnected += present;
    }

    while (_gamepadStates.size() > numControllersConnected)
    {
        _gamepadStates.pop_back();
    }
}

void InputSystem::UpdateControllers()
{
    if (numControllersConnected == 0)
    {
        return;
    }

    for (int i = 0; i < numControllersConnected; i++)
    {
        GLFWgamepadstate pastState = _gamepadStates[i];
        if (glfwGetGamepadState(i, &_gamepadStates[i]))
        {
            for (int j = 0; j < GLFW_GAMEPAD_BUTTON_LAST; j++)
            {
                if (pastState.buttons[j] != _gamepadStates[i].buttons[j])
                {
                    EventSystem::getInstance()->fireEvent(ControllerButtonEvent(j, _gamepadStates[i].buttons[j] != GLFW_RELEASE, i));
                }
            }
            for (int j = 0; j < GLFW_GAMEPAD_AXIS_LAST; j++)
            {
                EventSystem::getInstance()->fireEvent(ControllerAxisEvent(j, _gamepadStates[i].axes[j], i));
            }
        }
    }
}




