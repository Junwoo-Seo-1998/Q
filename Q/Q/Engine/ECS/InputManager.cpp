/*
    File Name: InputManager.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "InputManager.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "Engine/Engine.h"
namespace q_engine
{
    Bindings InputManager::m_bindings;
    Callbacks InputManager::m_callbacks;
    Callbacks InputManager::m_releaseCallbacks;
    std::vector<BindingID> InputManager::m_removed_bindings;
    std::vector<CallbackID> InputManager::m_removed_callbacks;
    KeyCheckers InputManager::m_key_checkers;

    int ConvertKeyToIndex(Key key)
    {
        if (key == Key::SPACE)
        {
            return 0;
        }
        if (key == Key::ESC)
        {
            return 42;
        }
        if(key == Key::ENTER)
        {
            return 43;
        }
        if (key == Key::L_CTRL)
        {
            return 27;
        }
        if (key == Key::L_ALT)
        {
            return 28;
        }
        if (key >= Key::A && key <= Key::Z)
        {
            return 1 + static_cast<int>(key) - static_cast<int>(Key::A);
        }
        if (key >= Key::F1 && key <= Key::F12)
        {
            return 28 + static_cast<int>(key) - static_cast<int>(Key::F1);
        }
        if (key >= Key::RIGHT_ARROW && key <= Key::UP_ARROW)
        {
            return 44 + static_cast<int>(key) - static_cast<int>(Key::RIGHT_ARROW);
        }
        return -1;
    }

    void InputManager::Update([[maybe_unused]] double deltaTime)
    {
        old_key_buffer = new_key_buffer;

#ifdef QEDITOR
        const GameStateType updateStatus = GameStateType::COUNT;
#else
        const GameStateType updateStatus = Engine::GetEngineComponent<GameStateManager>().GetUpdateStatus();
#endif
        for (auto& key_checker_itr : m_key_checkers)
        {
            key_checker_itr.second->Update();
        }

        for (auto& bind_itr : m_bindings)
        {
            Binding& binding = *bind_itr.second;
            if (binding.mControlledState == GameStateType::COUNT || binding.mControlledState <= updateStatus)
            {
                for (auto& input_itr : binding.m_inputs) {
                    switch (input_itr.first) {
                    case InputType::KEY_DOWN: {
                        if (glfwGetKey(p_window, input_itr.second.m_code) == GLFW_PRESS) {
                            ++binding.input_ctr;
                        }
                        break;
                    }
                    case InputType::MOUSE_BUTTON_DOWN: {
                        if (glfwGetMouseButton(p_window, input_itr.second.m_code) == GLFW_PRESS) {
                            ++binding.input_ctr;
                        }
                        break;
                    }
                    default:
                    {
                        break;
                    }
                    }
                }
                if (static_cast<int>(binding.m_inputs.size()) == binding.input_ctr) {
                    auto callback_itr = m_callbacks.find(bind_itr.first);
                    if (callback_itr != m_callbacks.end()) {
                        callback_itr->second();
                    }
                }
                binding.input_ctr = 0;
            }
        }

        for (auto& bind : m_removed_bindings)
        {
            m_bindings.erase(bind);
        }
        m_removed_bindings.clear();

        for (auto& callback : m_removed_callbacks)
        {
            m_callbacks.erase(callback);
        }
        m_removed_callbacks.clear();
    }

    bool InputManager::AddBinding(Binding* l_binding_ptr) {
        if (m_bindings.find(l_binding_ptr->m_bindingID) != m_bindings.end()) {
            std::cerr << " Trying to add bind already exist!.";
            delete l_binding_ptr;
            return false;
        }
        return m_bindings.emplace(l_binding_ptr->m_bindingID, l_binding_ptr).second;
    }

    bool InputManager::RemoveBinding(const std::string& l_key_val) {
        auto remove_itr = m_bindings.find(get_hash_id(l_key_val));
        if (remove_itr == m_bindings.end()) {
            std::cerr << "Failed to find " << l_key_val << " to remove.";
            return false;
        }
        m_removed_bindings.push_back(remove_itr->first);
        return true;
    }

    bool InputManager::AddCallback(const std::string& l_callback_name, std::function<void(void)> callback)
    {
        return m_callbacks.emplace(get_hash_id(l_callback_name), callback).second;
    }

    bool InputManager::RemoveCallback(std::string l_key_val) {
        auto remove_itr = m_callbacks.find(get_hash_id(l_key_val));
        if (remove_itr == m_callbacks.end()) {
            std::cerr << "Failed to find " << l_key_val << " to remove.";
            return false;
        }
        m_removed_callbacks.push_back(remove_itr->first);
        return true;
    }

    void InputManager::HandleKeyCallback([[maybe_unused]] GLFWwindow* p_win, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mod)
    {
        const int key_index_for_buffer = ConvertKeyToIndex(static_cast<Key>(key));
        if (key_index_for_buffer != -1)
        {
            if (action == GLFW_PRESS)
            {
                new_key_buffer[key_index_for_buffer] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                new_key_buffer[key_index_for_buffer] = false;
            }
        }

#ifdef QEDITOR
        const GameStateType updateStatus = GameStateType::COUNT;
#else
        const GameStateType updateStatus = Engine::GetEngineComponent<GameStateManager>().GetUpdateStatus();
#endif
        for (auto& bind_itr : m_bindings)
        {
            Binding& binding = *bind_itr.second;
            if (binding.mControlledState != GameStateType::COUNT)
            {
                if (binding.mControlledState > updateStatus)
                {
                    break;
                }
            }
            for (auto& input_itr : binding.m_inputs)
            {
                const KeyCode keyCode = input_itr.second;
                if ((input_itr.first == InputType::KEY_PRESS && action == GLFW_PRESS)
                    || (input_itr.first == InputType::KEY_RELEASE && action == GLFW_RELEASE))
                {
                    if (keyCode == key)
                    {
                        ++binding.input_ctr;
                    }
                }
            }
        }
    }

    void InputManager::HandleMouseButtonCallback([[maybe_unused]] GLFWwindow* p_win, int button, int action, [[maybe_unused]] int mod)
    {
#ifdef QEDITOR
        const GameStateType updateStatus = GameStateType::COUNT;
#else
        const GameStateType updateStatus = Engine::GetEngineComponent<GameStateManager>().GetUpdateStatus();
#endif
        for (auto& bind_itr : m_bindings)
        {
            Binding& binding = *bind_itr.second;
            if (binding.mControlledState == GameStateType::COUNT || binding.mControlledState <= updateStatus)
            {
                for (auto& input_itr : binding.m_inputs)
                {
                    const KeyCode keyCode = input_itr.second;
                    if ((input_itr.first == InputType::MOUSE_BUTTON_PRESS && action == GLFW_PRESS)
                        || (input_itr.first == InputType::MOUSE_BUTTON_RELEASE && action == GLFW_RELEASE))
                    {
                        if (keyCode == button)
                        {
                            ++binding.input_ctr;
                        }
                    }
                }
            }
        }
    }

    void InputManager::HandleMouseWheelCallback(GLFWwindow* p_win, double/* xoffset*/, double/* yoffset*/)
    {
#ifdef QEDITOR
        const GameStateType updateStatus = GameStateType::COUNT;
#else
        const GameStateType updateStatus = Engine::GetEngineComponent<GameStateManager>().GetUpdateStatus();
#endif
        for (auto& bind_itr : m_bindings)
        {
            Binding& binding = *bind_itr.second;
            if (binding.mControlledState == GameStateType::COUNT || binding.mControlledState <= updateStatus)
            {
                for (auto& input_itr : binding.m_inputs) {
                    const KeyCode keyCode = input_itr.second;
                    if (input_itr.first == InputType::MOUSE_WHEEL_MOVE && keyCode == static_cast<int>(M_button::WHEEL))
                    {
                        ++binding.input_ctr;
                    }
                }
            }
        }
    }

    bool InputManager::IsKeyDown(Key keycode)
    {
        int idx = ConvertKeyToIndex(keycode);
        return new_key_buffer[idx] == true;
    }

    bool InputManager::IsKeyPressed(Key keycode)
    {
        int idx = ConvertKeyToIndex(keycode);
        return old_key_buffer[idx] == false && new_key_buffer[idx] == true;
    }

    bool InputManager::IsMouseButtonPressed(M_button button)
    {
        auto state = glfwGetMouseButton(q_engine::q_impl::p_window, static_cast<int>(button));
        return state == GLFW_PRESS;
    }

    bool InputManager::IsKeyReleased(Key keycode)
    {
        int idx = ConvertKeyToIndex(keycode);
        return new_key_buffer[idx] == false && old_key_buffer[idx] == true;
    }

    void InputManager::SetKeyDown(Key keycode)
    {
        int idx = ConvertKeyToIndex(keycode);
        new_key_buffer[idx] = true;
    }

    void InputManager::CallAllReleaseCallbacks()
    {
        for(const auto& callback : m_releaseCallbacks)
        {
            callback.second();
        }
    }

    void InputManager::Init() const
    {
        glfwMakeContextCurrent(p_window);
        glfwSetKeyCallback(p_window, HandleKeyCallback);
        glfwSetMouseButtonCallback(p_window, HandleMouseButtonCallback);
        glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetScrollCallback(p_window, HandleMouseWheelCallback);
    }

    //Helper Functions for LoadBindings method
    std::pair<bool, InputType> ConvertToInputType(std::string& s_inputType)
    {
        if (s_inputType == "KeyPress")
        {
            return std::make_pair(true, InputType::KEY_PRESS);
        }
        if (s_inputType == "KeyRelease")
        {
            return std::make_pair(true, InputType::KEY_RELEASE);
        }
        if (s_inputType == "KeyDown")
        {
            return std::make_pair(true, InputType::KEY_DOWN);
        }
        if (s_inputType == "MButtonPress")
        {
            return std::make_pair(true, InputType::MOUSE_BUTTON_PRESS);
        }
        if (s_inputType == "MButtonRelease")
        {
            return std::make_pair(true, InputType::MOUSE_BUTTON_RELEASE);
        }
        if (s_inputType == "MButtonDown")
        {
            return std::make_pair(true, InputType::MOUSE_BUTTON_DOWN);
        }
        return std::make_pair(false, InputType::COUNT);
    }
    std::pair<bool, Key> ConvertToKey(std::string& s_keyValue)
    {
        if (s_keyValue.length() == 1)
        {
            const int difference_Ascii_KeyVal = static_cast<char>(Key::A) - 'A';
            int key_val = toupper(static_cast<int>(s_keyValue.front()));
            key_val += difference_Ascii_KeyVal;
            if (key_val < static_cast<int>(Key::A) || key_val > static_cast<int>(Key::Z))
                return std::make_pair(false, Key::COUNT);
            return std::make_pair(true, static_cast<Key>(key_val));

        }

        if (s_keyValue.front() == 'F')
        {
            const std::string key_code = s_keyValue.substr(1, s_keyValue.size() - 1);
            const int key_val = atoi(key_code.c_str());
            if (key_val < 1 || key_val > 12)
            {
                return std::make_pair(false, Key::COUNT);
            }
            return std::make_pair(true, static_cast<Key>(static_cast<int>(Key::F1) + key_val - 1));
        }

        if (s_keyValue == "Space" || s_keyValue == "SpaceBar")
        {
            return std::make_pair(true, Key::SPACE);
        }
        if (s_keyValue == "L_Ctrl")
        {
            return std::make_pair(true, Key::L_CTRL);
        }
        if (s_keyValue == "L_Alt")
        {
            return std::make_pair(true, Key::L_ALT);
        }
        if (s_keyValue == "Esc" || s_keyValue == "ESC")
        {
            return std::make_pair(true, Key::ESC);
        }
        return std::make_pair(false, Key::COUNT);
    }
    std::pair<bool, M_button> ConvertToMButton(std::string& sKeyValue)
    {
        if (sKeyValue == "Left")
        {
            return std::make_pair(true, M_button::LEFT);
        }
        if (sKeyValue == "Right")
        {
            return std::make_pair(true, M_button::RIGHT);
        }
        if (sKeyValue == "Wheel")
        {
            return std::make_pair(true, M_button::WHEEL);
        }
        return std::make_pair(false, M_button::COUNT);
    }

    void InputManager::LoadBindings(const std::string& filename)
    {
        const char separator = '/';

        std::ifstream bindings;
        bindings.open("resource/keys/" + filename);

        if (!bindings.is_open())
        {
            Debugger::GetLogger().AddMessage("Failed to load " + filename, SeverityLevel::ERROR);
            return;
        }
        std::string line;
        while (std::getline(bindings, line))
        {
            std::stringstream keyStream(line);
            std::string callbackName;
            keyStream >> callbackName;
            Binding* bind = new Binding(std::move(callbackName));
            while (!keyStream.eof())
            {
                std::string inputDetail;
                keyStream >> inputDetail;
                size_t end = inputDetail.find(separator);
                if (end == std::string::npos)
                {
                    delete bind;
                    bind = nullptr;
                    break;
                }
                std::string inputType = inputDetail.substr(0, end);
                InputType type = ConvertToInputType(inputType).second;
                std::string keyCode = inputDetail.substr(end + 1,
                    inputDetail.find(separator, end + 1));
                int keyVal = -1;
                if (type == InputType::KEY_PRESS || type == InputType::KEY_RELEASE || type == InputType::KEY_DOWN)
                {
                    keyVal = static_cast<int>(ConvertToKey(keyCode).second);
                }
                else if (type == InputType::MOUSE_BUTTON_PRESS || type == InputType::MOUSE_BUTTON_RELEASE || type == InputType::MOUSE_BUTTON_DOWN)
                {
                    keyVal = static_cast<int>(ConvertToMButton(keyCode).second);
                }
                else
                {
                    break;
                }
                bind->BindInput(type, keyVal);
            }
            if (!AddBinding(bind)) { delete bind; }
            bind = nullptr;
        }
        bindings.close();
    }

    KeyChecker::KeyChecker(std::string&& name, Binding* bind) : m_name(name), m_is_true(false)
    {
        Engine::GetEngineComponent<InputManager>().AddBinding(bind);
        Engine::GetEngineComponent<InputManager>().AddCallback(m_name, &KeyChecker::MakeTrue, this);
    }

    KeyChecker::~KeyChecker()
    {
        Engine::GetEngineComponent<InputManager>().RemoveCallback(m_name);
        Engine::GetEngineComponent<InputManager>().RemoveBinding(m_name);
    }

}
