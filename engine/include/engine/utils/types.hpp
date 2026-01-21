#pragma once

#include <concepts>
#include <variant>

#include <glm/glm.hpp>

#include <engine/model/Bundle.hpp>
#include <engine/model/Service.hpp>
#include <engine/model/System.hpp>

template <typename T>
concept SystemType = std::derived_from<T, System>;

template <typename T>
concept ServiceType = std::derived_from<T, Service>;

template <typename T>
concept BundleType = std::derived_from<T, Bundle>;

struct UDirectionalLight {
    glm::vec3 direction;
    glm::vec3 color     = glm::vec3(1.0f);
    float     ambient   = 0.1f;
    float     intensity = 1.0f;
};

struct UPointLight {
    glm::vec3 position;
    glm::vec3 color     = glm::vec3(1.0f);
    float     intensity = 1.0f;
};

using UniformValue =
    std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4, UDirectionalLight, UPointLight>;

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