#pragma once

#include <engine/model/Service.hpp>
#include <engine/utils/key_mapping.hpp>

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