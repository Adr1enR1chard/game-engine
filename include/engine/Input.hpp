#pragma once

#include <cstdint>

enum class KeyAction {
    Pressed  = 1,
    Released = 0,
    Down     = 2,
};

enum class Key : uint16_t {
    Unknown = 0,

    // Letters
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    // Numbers (top row)
    D0,
    D1,
    D2,
    D3,
    D4,
    D5,
    D6,
    D7,
    D8,
    D9,

    // Controls / navigation
    Escape,
    Enter,
    Tab,
    Backspace,
    Space,

    Left,
    Right,
    Up,
    Down,

    LShift,
    RShift,
    LCtrl,
    RCtrl,
    LAlt,
    RAlt,
    LSuper,
    RSuper, // Windows / Command key
    Menu,   // "Apps" / menu key

    Insert,
    Delete,
    Home,
    End,
    PageUp,
    PageDown,

    // Locks & misc
    CapsLock,
    ScrollLock,
    NumLock,
    PrintScreen,
    Pause,

    // Function keys
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,

    // Punctuation
    Minus,
    Equal,
    LeftBracket,
    RightBracket,
    Backslash,
    Semicolon,
    Apostrophe,
    GraveAccent,
    Comma,
    Period,
    Slash,

    // Keypad
    KP0,
    KP1,
    KP2,
    KP3,
    KP4,
    KP5,
    KP6,
    KP7,
    KP8,
    KP9,
    KPDecimal,
    KPDivide,
    KPMultiply,
    KPSubtract,
    KPAdd,
    KPEnter,
    KPEqual,
};

#include <engine/Service.hpp>

#include <glm/glm.hpp>
#include <string>
#include <unordered_set>
#include <vector>

class Input : public Service
{
  private:
    static std::vector<Input*> s_activeInputs;

  private:
    friend class Window;

    static void KeyCallback(Key key, Key scancode, KeyAction action, int mods);
    static void MousePositionCallback(double xpos, double ypos);

  public:
    Input()
    {
        s_activeInputs.push_back(this);
    };
    ~Input()
    {
        s_activeInputs.erase(std::remove(s_activeInputs.begin(), s_activeInputs.end(), this), s_activeInputs.end());
    };

  public:
    bool      isKeyPressed(Key key) const;
    bool      isKeyReleased(Key key) const;
    bool      isKeyDown(Key key) const;
    glm::vec2 getMousePosition() const;
    glm::vec2 getMouseDelta() const;

  private:
    friend class Engine;

    void clear();

  private:
    void        processKey(Key key, Key scancode, KeyAction action, int mods);
    void        processMousePosition(double xpos, double ypos);
    std::string toString(Key key) const;
    std::string toString(KeyAction action) const;

    std::unordered_set<Key> m_pressedKeys;
    std::unordered_set<Key> m_releasedKeys;
    std::unordered_set<Key> m_downKeys;

    glm::vec2 m_lastMousePos = glm::vec2(0.0f);
    glm::vec2 m_mouseDelta   = glm::vec2(0.0f);
    bool      m_firstMouse   = true;
};