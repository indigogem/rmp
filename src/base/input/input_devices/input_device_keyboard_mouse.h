#pragma once

#include "base/input/input_device.h"
#include "base/input/input_states/input_state_mouse.h"
#include "base/input/input_states/input_state_keyboard.h"

//-------------------------------------------------------------------------

namespace kmp::input
{
    class KeyboardMouseInputDevice : public InputDevice
    {

    public:

        KeyboardMouseInputDevice() = default;

        inline MouseInputState const& GetMouseState() const { return m_mouseState; }
        inline KeyboardInputState const& GetKeyboardState() const { return m_keyboardState; }

    private:

        virtual DeviceCategory GetDeviceCategory() const override final { return DeviceCategory::KeyboardMouse; }

        virtual void Initialize() override final;
        virtual void Shutdown() override final;

        virtual void UpdateState( Seconds deltaTime ) override final;
        virtual void ClearFrameState( ResetType resetType = ResetType::Partial ) override final;
        virtual void ProcessMessage(const SDL_Event& input_event) override final;

    private:

        MouseInputState                                         m_mouseState;
        KeyboardInputState                                      m_keyboardState;
    };
}