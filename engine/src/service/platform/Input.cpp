#include <engine/service/platform/Input.hpp>

#include <chrono>

std::vector<Input *> Input::s_activeInputs;

void Input::KeyCallback(Key key, Key scancode, KeyAction action, int mods)
{
    for (Input *input : s_activeInputs)
    {
        input->processKey(key, scancode, action, mods);
    }
}

void Input::MousePositionCallback(double xpos, double ypos)
{
    for (Input *input : s_activeInputs)
    {
        input->processMousePosition(xpos, ypos);
    }
}

bool Input::isKeyPressed(Key key) const
{
    return m_pressedKeys.find(key) != m_pressedKeys.end();
}

bool Input::isKeyReleased(Key key) const
{
    return m_releasedKeys.find(key) != m_releasedKeys.end();
}

bool Input::isKeyDown(Key key) const
{
    return m_downKeys.find(key) != m_downKeys.end();
}

glm::vec2 Input::getMousePosition() const
{
    return m_lastMousePos;
}

glm::vec2 Input::getMouseDelta() const
{
    return m_mouseDelta;
}

void Input::clear()
{
    m_mouseDelta = glm::vec2(0.0f);
}

void Input::processKey(Key key, Key /*scancode*/, KeyAction action, int /*mods*/)
{
    switch (action)
    {
    case KeyAction::Pressed:
        m_pressedKeys.insert(key);
        m_downKeys.insert(key);
        m_releasedKeys.erase(key);
        break;
    case KeyAction::Released:
        m_releasedKeys.insert(key);
        m_downKeys.erase(key);
        m_pressedKeys.erase(key);
        break;
    case KeyAction::Down:
        m_downKeys.insert(key);
        m_pressedKeys.erase(key);
        m_releasedKeys.erase(key);
        break;
    default:
        break;
    }
}

void Input::processMousePosition(double xpos, double ypos)
{
    glm::vec2 currentPos = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));

    if (m_firstMouse)
    {
        m_firstMouse = false;
        m_lastMousePos = currentPos;
        return;
    }

    m_mouseDelta = currentPos - m_lastMousePos;
    m_lastMousePos = currentPos;
}

std::string Input::toString(Key key) const
{
    switch (key)
    {
    case Key::Unknown:
        return "Unknown";

    // Letters
    case Key::A:
        return "A";
    case Key::B:
        return "B";
    case Key::C:
        return "C";
    case Key::D:
        return "D";
    case Key::E:
        return "E";
    case Key::F:
        return "F";
    case Key::G:
        return "G";
    case Key::H:
        return "H";
    case Key::I:
        return "I";
    case Key::J:
        return "J";
    case Key::K:
        return "K";
    case Key::L:
        return "L";
    case Key::M:
        return "M";
    case Key::N:
        return "N";
    case Key::O:
        return "O";
    case Key::P:
        return "P";
    case Key::Q:
        return "Q";
    case Key::R:
        return "R";
    case Key::S:
        return "S";
    case Key::T:
        return "T";
    case Key::U:
        return "U";
    case Key::V:
        return "V";
    case Key::W:
        return "W";
    case Key::X:
        return "X";
    case Key::Y:
        return "Y";
    case Key::Z:
        return "Z";

    // Numbers
    case Key::D0:
        return "0";
    case Key::D1:
        return "1";
    case Key::D2:
        return "2";
    case Key::D3:
        return "3";
    case Key::D4:
        return "4";
    case Key::D5:
        return "5";
    case Key::D6:
        return "6";
    case Key::D7:
        return "7";
    case Key::D8:
        return "8";
    case Key::D9:
        return "9";

    // Controls / navigation
    case Key::Escape:
        return "Escape";
    case Key::Enter:
        return "Enter";
    case Key::Tab:
        return "Tab";
    case Key::Backspace:
        return "Backspace";
    case Key::Space:
        return "Space";
    case Key::Left:
        return "Left";
    case Key::Right:
        return "Right";
    case Key::Up:
        return "Up";
    case Key::Down:
        return "Down";

    case Key::LShift:
        return "Left Shift";
    case Key::RShift:
        return "Right Shift";
    case Key::LCtrl:
        return "Left Control";
    case Key::RCtrl:
        return "Right Control";
    case Key::LAlt:
        return "Left Alt";
    case Key::RAlt:
        return "Right Alt";
    case Key::LSuper:
        return "Left Super";
    case Key::RSuper:
        return "Right Super";
    case Key::Menu:
        return "Menu";

    case Key::Insert:
        return "Insert";
    case Key::Delete:
        return "Delete";
    case Key::Home:
        return "Home";
    case Key::End:
        return "End";
    case Key::PageUp:
        return "Page Up";
    case Key::PageDown:
        return "Page Down";

    // Locks & misc
    case Key::CapsLock:
        return "Caps Lock";
    case Key::ScrollLock:
        return "Scroll Lock";
    case Key::NumLock:
        return "Num Lock";
    case Key::PrintScreen:
        return "Print Screen";
    case Key::Pause:
        return "Pause";

    // Function keys
    case Key::F1:
        return "F1";
    case Key::F2:
        return "F2";
    case Key::F3:
        return "F3";
    case Key::F4:
        return "F4";
    case Key::F5:
        return "F5";
    case Key::F6:
        return "F6";
    case Key::F7:
        return "F7";
    case Key::F8:
        return "F8";
    case Key::F9:
        return "F9";
    case Key::F10:
        return "F10";
    case Key::F11:
        return "F11";
    case Key::F12:
        return "F12";
    case Key::F13:
        return "F13";
    case Key::F14:
        return "F14";
    case Key::F15:
        return "F15";
    case Key::F16:
        return "F16";
    case Key::F17:
        return "F17";
    case Key::F18:
        return "F18";
    case Key::F19:
        return "F19";
    case Key::F20:
        return "F20";
    case Key::F21:
        return "F21";
    case Key::F22:
        return "F22";
    case Key::F23:
        return "F23";
    case Key::F24:
        return "F24";

    // Punctuation
    case Key::Minus:
        return "-";
    case Key::Equal:
        return "=";
    case Key::LeftBracket:
        return "[";
    case Key::RightBracket:
        return "]";
    case Key::Backslash:
        return "\\";
    case Key::Semicolon:
        return ";";
    case Key::Apostrophe:
        return "'";
    case Key::GraveAccent:
        return "`";
    case Key::Comma:
        return ",";
    case Key::Period:
        return ".";
    case Key::Slash:
        return "/";

    // Keypad
    case Key::KP0:
        return "Keypad 0";
    case Key::KP1:
        return "Keypad 1";
    case Key::KP2:
        return "Keypad 2";
    case Key::KP3:
        return "Keypad 3";
    case Key::KP4:
        return "Keypad 4";
    case Key::KP5:
        return "Keypad 5";
    case Key::KP6:
        return "Keypad 6";
    case Key::KP7:
        return "Keypad 7";
    case Key::KP8:
        return "Keypad 8";
    case Key::KP9:
        return "Keypad 9";
    case Key::KPDecimal:
        return "Keypad .";
    case Key::KPDivide:
        return "Keypad /";
    case Key::KPMultiply:
        return "Keypad *";
    case Key::KPSubtract:
        return "Keypad -";
    case Key::KPAdd:
        return "Keypad +";
    case Key::KPEnter:
        return "Keypad Enter";
    case Key::KPEqual:
        return "Keypad =";

    default:
        return "Unknown";
    }
}

std::string Input::toString(KeyAction action) const
{
    switch (action)
    {
    case KeyAction::Pressed:
        return "Pressed";
        break;
    case KeyAction::Released:
        return "Released";
        break;
    case KeyAction::Down:
        return "Down";
        break;
    default:
        return "Unknown";
        break;
    }
}