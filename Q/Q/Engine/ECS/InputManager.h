/*
    File Name: InputManager.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <unordered_map>
#include <utility>                //std::forward
#include <functional>             //std::function
#include <GLFW/glfw3.h>
#include "../Engine/Engine.h"
#include "../Core/StringHash.h"
#include "../Debugger/Debugger.h"
#include "../EngineComponent.h"
#include "../Graphics/Window.h"
#include "Engine/GameStateManager.h"

enum class Key
{
    SPACE = GLFW_KEY_SPACE,
    A = GLFW_KEY_A,
    B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    L_CTRL = GLFW_KEY_LEFT_CONTROL, L_ALT = GLFW_KEY_LEFT_ALT,
    F1 = GLFW_KEY_F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    ESC = GLFW_KEY_ESCAPE,
    ENTER = GLFW_KEY_ENTER,
    RIGHT_ARROW = GLFW_KEY_RIGHT, LEFT_ARROW, DOWN_ARROW, UP_ARROW,
    COUNT = 48,
};

enum class M_button
{
    LEFT = GLFW_MOUSE_BUTTON_1, RIGHT = GLFW_MOUSE_BUTTON_2, WHEEL = GLFW_MOUSE_BUTTON_3,
    COUNT,
};

//Types of Inputs
enum class InputType {
    // These Inputs are handled by Events
    KEY_PRESS,
    KEY_RELEASE,
    MOUSE_BUTTON_PRESS,
    MOUSE_BUTTON_RELEASE,
    // These are not
    KEY_DOWN,
    MOUSE_BUTTON_DOWN,
    MOUSE_WHEEL_MOVE,
    COUNT
};

namespace q_engine {
    //Matching Keycodes corresponding Inputs
    using BindingID = size_t;
    using CallbackID = size_t;

    struct KeyCode {
        KeyCode() = default;
        KeyCode(int l_keyCode) : m_code(l_keyCode) {}
        KeyCode(Key l_keyCode) : m_code(static_cast<int>(l_keyCode)) {}
        KeyCode(M_button l_keyCode) : m_code(static_cast<int>(l_keyCode)) {}

        operator int() { return m_code; }
        bool operator== (int l_keycode) const { return m_code == l_keycode; }
        int m_code{ 0 };
    };

    //These two have to paired together
    using Input = std::pair<InputType, KeyCode>;
    using Inputs = std::vector<Input>;

    struct Binding {
        Binding() = delete;
        Binding(std::string&& l_name) : m_bindingID(get_hash_id(l_name)), input_ctr(0) {}
        Binding(std::string&& l_name, GameStateType place) : m_bindingID(get_hash_id(l_name)), mControlledState(place), input_ctr(0) {}
        ~Binding() {  }
        void BindInput(InputType l_type, KeyCode code = KeyCode())
        {
            m_inputs.emplace_back(std::make_pair(l_type, code));
        }

        BindingID m_bindingID;

        GameStateType mControlledState = GameStateType::COUNT;
        Inputs m_inputs;
        int input_ctr;      //Counter for happening events
    };

    class KeyChecker
    {
    public:
        KeyChecker(std::string&& name, Binding* bind);
        ~KeyChecker();

        void Update()
        {
            m_is_true = false;
        }

        [[nodiscard]] bool IsTrue() const
        {
            return m_is_true;
        }

    private:
        void MakeTrue()
        {
            m_is_true = true;
        }

        std::string m_name;
        bool m_is_true;
    };

    //Bindings and Callbacks Key string have to be identical to call them
    using Bindings = std::unordered_map<BindingID, std::unique_ptr<Binding>>;
    using Callbacks = std::unordered_map<CallbackID, std::function<void()>>;
    using KeyCheckers = std::unordered_map<size_t, std::shared_ptr<KeyChecker>>;

    class InputManager : public EngineComponent<InputManager> {
    public:
        InputManager() : p_window(q_impl::p_window) {};
        virtual void Load() override
        {
            Init();
        }

        void LoadBindings(const std::string& filename);

        virtual void Update([[maybe_unused]] double deltaTime) override;
        virtual void Unload() override
        {
            m_key_checkers.clear();
        };
        ~InputManager() = default;

        bool AddBinding(Binding* l_binding_ptr);
        bool RemoveBinding(const std::string& l_key_val);

        //for member
        template<typename T>
        bool AddCallback(const std::string& l_callback_name, void(T::* m_function)(void), T* instance);
        //for non-member
        bool AddCallback(const std::string& l_callback_name, std::function<void(void)> func);
        void AddKeyChecker(std::string&& l_name, Binding* bind) const
        {
            m_key_checkers[get_hash_id(l_name)] = std::make_shared<KeyChecker>(std::forward<std::string>(l_name), bind);
        }

        static std::shared_ptr<KeyChecker> GetKeyChecker(std::string&& l_name)
        {
            return m_key_checkers.at(get_hash_id(l_name));
        }

        static void RemoveKeyChecker(std::string&& l_name)
        {
            m_key_checkers.erase(get_hash_id(l_name));
        }

        bool RemoveCallback(std::string l_key_val);

        static void HandleKeyCallback(GLFWwindow* p_win, int key, int scancode, int action, int mod);
        static void HandleMouseButtonCallback(GLFWwindow* p_win, int button, int action, int mod);
        static void HandleMouseWheelCallback(GLFWwindow* p_win, double xoffset, double yoffset);


        //for script expose purpose.
        static bool IsKeyDown(Key keycode);
        static bool IsKeyPressed(Key keycode);
        static bool IsMouseButtonPressed(M_button button);

        static bool IsKeyReleased(Key keycode);

        static void SetKeyDown(Key keycode);

        static void CallAllReleaseCallbacks();
    private:
        void Init() const;


        GLFWwindow* p_window = nullptr;
        static Bindings m_bindings;
        static Callbacks m_callbacks;
        static KeyCheckers m_key_checkers;

        static Callbacks m_releaseCallbacks;

        static std::vector<BindingID> m_removed_bindings;
        static std::vector<CallbackID> m_removed_callbacks;

        inline static std::array<bool, static_cast<int>(Key::COUNT)> old_key_buffer{ false };
        inline static std::array<bool, static_cast<int>(Key::COUNT)> new_key_buffer{ false };


    };

    template <typename T>
    inline bool InputManager::AddCallback(const std::string& l_callback_name,
        void (T::* m_function)(void), T* instance)
    {
        static_assert(std::is_class_v<T>);
        auto temp_callback = std::bind(m_function, instance);

        if(l_callback_name.find("Released") < l_callback_name.length())
        {
            m_releaseCallbacks.emplace(get_hash_id(l_callback_name), temp_callback);
        }

        return m_callbacks.emplace(get_hash_id(l_callback_name), temp_callback).second;
    }
}

/*example usage how to bind input
 *  Binding* bind = (new Binding("ToNextStage"));
  bind->BindInput(InputType::KEY_PRESS, KeyCode(sf::Keyboard::A));
  bind->BindInput(InputType::KEY_DOWN, KeyCode(sf::Keyboard::B));
  engine.GetInputManager().AddBinding(bind);
  engine.GetInputManager().AddCallback<GameStateManager>(
      "ToNextStage", &GameStateManager::ToNextState, &engine.GetGameStateManager());
 */

