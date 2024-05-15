#include "input_system.h"
#include "base/memory/memory.h"
#include "base/input/input_devices/input_device_keyboard_mouse.h"

namespace kmp
{
    namespace input
    {

        MouseInputState const InputSystem::s_emptyMouseState = MouseInputState();
        KeyboardInputState const InputSystem::s_emptyKeyboardState = KeyboardInputState();

        bool InputSystem::Initialize()
        {

            // Create a keyboard and mouse device
            m_inputDevices.emplace_back(kmp::New<KeyboardMouseInputDevice>());

            // TODO: more controllers devices

            for (auto pDevice : m_inputDevices)
            {
                pDevice->Initialize();
            }

            return true;
        }

        void InputSystem::Shutdown()
        {
            for (auto &pInputDevice : m_inputDevices)
            {
                pInputDevice->Shutdown();
                kmp::Delete(pInputDevice);
            }

            m_inputDevices.clear();
        }

        void InputSystem::Update(Seconds deltaTime)
        {
            for (auto pInputDevice : m_inputDevices)
            {
                pInputDevice->UpdateState(deltaTime);
            }
        }

        void InputSystem::ClearFrameState()
        {
            for (auto pInputDevice : m_inputDevices)
            {
                pInputDevice->ClearFrameState();
            }
        }

        void InputSystem::ForwardInputMessageToInputDevices(const SDL_Event &input_event)
        {
            for (auto pInputDevice : m_inputDevices)
            {
                pInputDevice->ProcessMessage(input_event);
            }
        }

        KeyboardMouseInputDevice const *InputSystem::GetKeyboardMouseDevice() const
        {
            for (auto pDevice : m_inputDevices)
            {
                if (pDevice->GetDeviceCategory() == DeviceCategory::KeyboardMouse)
                {
                    return static_cast<KeyboardMouseInputDevice *>(pDevice);
                }
            }

            return nullptr;
        }

        void InputSystem::ReflectState(Seconds const deltaTime, float timeScale, InputState &outReflectedState) const
        {
            outReflectedState.m_mouseState.ReflectFrom(deltaTime, timeScale, *GetMouseState());
            outReflectedState.m_keyboardState.ReflectFrom(deltaTime, timeScale, *GetKeyboardState());
        }

        //-------------------------------------------------------------------------

        void InputState::Clear()
        {
            m_mouseState.Clear();
            m_keyboardState.Clear();
        }
    }
}