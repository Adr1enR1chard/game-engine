#include <GLFW/glfw3.h>

#include <engine/utils/key_mapping.hpp>
#include <engine/utils/types.hpp>

Key GLFWKeyToEngineKey(int glfwKey)
{
    switch (glfwKey) {
    // Letters
    case GLFW_KEY_A:
        return Key::A;
    case GLFW_KEY_B:
        return Key::B;
    case GLFW_KEY_C:
        return Key::C;
    case GLFW_KEY_D:
        return Key::D;
    case GLFW_KEY_E:
        return Key::E;
    case GLFW_KEY_F:
        return Key::F;
    case GLFW_KEY_G:
        return Key::G;
    case GLFW_KEY_H:
        return Key::H;
    case GLFW_KEY_I:
        return Key::I;
    case GLFW_KEY_J:
        return Key::J;
    case GLFW_KEY_K:
        return Key::K;
    case GLFW_KEY_L:
        return Key::L;
    case GLFW_KEY_M:
        return Key::M;
    case GLFW_KEY_N:
        return Key::N;
    case GLFW_KEY_O:
        return Key::O;
    case GLFW_KEY_P:
        return Key::P;
    case GLFW_KEY_Q:
        return Key::Q;
    case GLFW_KEY_R:
        return Key::R;
    case GLFW_KEY_S:
        return Key::S;
    case GLFW_KEY_T:
        return Key::T;
    case GLFW_KEY_U:
        return Key::U;
    case GLFW_KEY_V:
        return Key::V;
    case GLFW_KEY_W:
        return Key::W;
    case GLFW_KEY_X:
        return Key::X;
    case GLFW_KEY_Y:
        return Key::Y;
    case GLFW_KEY_Z:
        return Key::Z;

    // Numbers (top row)
    case GLFW_KEY_0:
        return Key::D0;
    case GLFW_KEY_1:
        return Key::D1;
    case GLFW_KEY_2:
        return Key::D2;
    case GLFW_KEY_3:
        return Key::D3;
    case GLFW_KEY_4:
        return Key::D4;
    case GLFW_KEY_5:
        return Key::D5;
    case GLFW_KEY_6:
        return Key::D6;
    case GLFW_KEY_7:
        return Key::D7;
    case GLFW_KEY_8:
        return Key::D8;
    case GLFW_KEY_9:
        return Key::D9;

    // Controls / navigation
    case GLFW_KEY_ESCAPE:
        return Key::Escape;
    case GLFW_KEY_ENTER:
        return Key::Enter;
    case GLFW_KEY_TAB:
        return Key::Tab;
    case GLFW_KEY_BACKSPACE:
        return Key::Backspace;
    case GLFW_KEY_SPACE:
        return Key::Space;

    case GLFW_KEY_LEFT:
        return Key::Left;
    case GLFW_KEY_RIGHT:
        return Key::Right;
    case GLFW_KEY_UP:
        return Key::Up;
    case GLFW_KEY_DOWN:
        return Key::Down;

    case GLFW_KEY_LEFT_SHIFT:
        return Key::LShift;
    case GLFW_KEY_RIGHT_SHIFT:
        return Key::RShift;
    case GLFW_KEY_LEFT_CONTROL:
        return Key::LCtrl;
    case GLFW_KEY_RIGHT_CONTROL:
        return Key::RCtrl;
    case GLFW_KEY_LEFT_ALT:
        return Key::LAlt;
    case GLFW_KEY_RIGHT_ALT:
        return Key::RAlt;

    case GLFW_KEY_LEFT_SUPER:
        return Key::LSuper;
    case GLFW_KEY_RIGHT_SUPER:
        return Key::RSuper;
    case GLFW_KEY_MENU:
        return Key::Menu;

    case GLFW_KEY_INSERT:
        return Key::Insert;
    case GLFW_KEY_DELETE:
        return Key::Delete;
    case GLFW_KEY_HOME:
        return Key::Home;
    case GLFW_KEY_END:
        return Key::End;
    case GLFW_KEY_PAGE_UP:
        return Key::PageUp;
    case GLFW_KEY_PAGE_DOWN:
        return Key::PageDown;

    // Locks & misc
    case GLFW_KEY_CAPS_LOCK:
        return Key::CapsLock;
    case GLFW_KEY_SCROLL_LOCK:
        return Key::ScrollLock;
    case GLFW_KEY_NUM_LOCK:
        return Key::NumLock;
    case GLFW_KEY_PRINT_SCREEN:
        return Key::PrintScreen;
    case GLFW_KEY_PAUSE:
        return Key::Pause;

    // Function keys
    case GLFW_KEY_F1:
        return Key::F1;
    case GLFW_KEY_F2:
        return Key::F2;
    case GLFW_KEY_F3:
        return Key::F3;
    case GLFW_KEY_F4:
        return Key::F4;
    case GLFW_KEY_F5:
        return Key::F5;
    case GLFW_KEY_F6:
        return Key::F6;
    case GLFW_KEY_F7:
        return Key::F7;
    case GLFW_KEY_F8:
        return Key::F8;
    case GLFW_KEY_F9:
        return Key::F9;
    case GLFW_KEY_F10:
        return Key::F10;
    case GLFW_KEY_F11:
        return Key::F11;
    case GLFW_KEY_F12:
        return Key::F12;

    case GLFW_KEY_F13:
        return Key::F13;
    case GLFW_KEY_F14:
        return Key::F14;
    case GLFW_KEY_F15:
        return Key::F15;
    case GLFW_KEY_F16:
        return Key::F16;
    case GLFW_KEY_F17:
        return Key::F17;
    case GLFW_KEY_F18:
        return Key::F18;
    case GLFW_KEY_F19:
        return Key::F19;
    case GLFW_KEY_F20:
        return Key::F20;
    case GLFW_KEY_F21:
        return Key::F21;
    case GLFW_KEY_F22:
        return Key::F22;
    case GLFW_KEY_F23:
        return Key::F23;
    case GLFW_KEY_F24:
        return Key::F24;

    // Punctuation
    case GLFW_KEY_MINUS:
        return Key::Minus;
    case GLFW_KEY_EQUAL:
        return Key::Equal;
    case GLFW_KEY_LEFT_BRACKET:
        return Key::LeftBracket;
    case GLFW_KEY_RIGHT_BRACKET:
        return Key::RightBracket;
    case GLFW_KEY_BACKSLASH:
        return Key::Backslash;
    case GLFW_KEY_SEMICOLON:
        return Key::Semicolon;
    case GLFW_KEY_APOSTROPHE:
        return Key::Apostrophe;
    case GLFW_KEY_GRAVE_ACCENT:
        return Key::GraveAccent;
    case GLFW_KEY_COMMA:
        return Key::Comma;
    case GLFW_KEY_PERIOD:
        return Key::Period;
    case GLFW_KEY_SLASH:
        return Key::Slash;

    // Keypad
    case GLFW_KEY_KP_0:
        return Key::KP0;
    case GLFW_KEY_KP_1:
        return Key::KP1;
    case GLFW_KEY_KP_2:
        return Key::KP2;
    case GLFW_KEY_KP_3:
        return Key::KP3;
    case GLFW_KEY_KP_4:
        return Key::KP4;
    case GLFW_KEY_KP_5:
        return Key::KP5;
    case GLFW_KEY_KP_6:
        return Key::KP6;
    case GLFW_KEY_KP_7:
        return Key::KP7;
    case GLFW_KEY_KP_8:
        return Key::KP8;
    case GLFW_KEY_KP_9:
        return Key::KP9;

    case GLFW_KEY_KP_DECIMAL:
        return Key::KPDecimal;
    case GLFW_KEY_KP_DIVIDE:
        return Key::KPDivide;
    case GLFW_KEY_KP_MULTIPLY:
        return Key::KPMultiply;
    case GLFW_KEY_KP_SUBTRACT:
        return Key::KPSubtract;
    case GLFW_KEY_KP_ADD:
        return Key::KPAdd;
    case GLFW_KEY_KP_ENTER:
        return Key::KPEnter;
    case GLFW_KEY_KP_EQUAL:
        return Key::KPEqual;

    default:
        return Key::Unknown;
    }
}

KeyAction GLFWActionToEngineAction(int glfwAction)
{
    switch (glfwAction) {
    case GLFW_PRESS:
        return KeyAction::Pressed;
    case GLFW_RELEASE:
        return KeyAction::Released;
    case GLFW_REPEAT:
        return KeyAction::Down;
    default:
        return KeyAction::Released;
    }
}