#include "pch.h"
#include "InputSystem.h"

std::vector<InputSystem*> InputSystem::_instances;

InputSystem::InputSystem(std::vector<LILLIS::KeyCode> keysToMonitor) : _isEnabled(true) {
    for (int key : keysToMonitor) {
        _keys[key] = false;
    }
    // Add this instance to the list of instances
    InputSystem::_instances.push_back(this);
    evSys = EventSystem::getInstance();
}

InputSystem::~InputSystem() {
    // Remove this instance from the list of instances
    _instances.erase(std::remove(_instances.begin(), _instances.end(), this), _instances.end());
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

void InputSystem::setIsKeyDown(int key, bool isDown) {
    std::map<int, bool>::iterator it = _keys.find(key);
    if (it != _keys.end()) {
        _keys[key] = isDown;
        evSys->fireEvent(InputEvent(key, isDown));
    }
}

void InputSystem::setupKeyInputs(const LillisWindow* window) {
    glfwSetKeyCallback(window->window, InputSystem::callback);
}

//Eventually add an ENUM abstraction.
void InputSystem::callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Send key event to all KeyInput instances
    for (InputSystem* keyInput : _instances) {
        keyInput->setIsKeyDown(key, action != GLFW_RELEASE);
    }
}