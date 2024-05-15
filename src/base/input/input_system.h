#pragma once
#include "defs.h"
#include "base/types/array.h"
#include "base/time/time.h"

#include "base/input/input_device.h"
#include "base/input/input_state.h"
#include "base/input/input_states/input_state_keyboard.h"
#include "base/input/input_states/input_state_mouse.h"
#include "base/input/input_devices/input_device_keyboard_mouse.h"
#include <SDL.h>

namespace kmp
{
    namespace input
    {

        class InputState;

        class InputSystem
        {
        public:
            static MouseInputState const s_emptyMouseState;
            static KeyboardInputState const s_emptyKeyboardState;

            bool Initialize();
            void Shutdown();
            void Update(Seconds deltaTime);
            void ClearFrameState();
            void ForwardInputMessageToInputDevices(const SDL_Event &input_event);

            // Keyboard & Mouse
            //-------------------------------------------------------------------------

            inline bool HasConnectedKeyboardAndMouse() { return GetKeyboardMouseDevice() != nullptr; }

            inline MouseInputState const *GetMouseState() const
            {
                if (auto pDevice = GetKeyboardMouseDevice())
                {
                    return &pDevice->GetMouseState();
                }

                return &s_emptyMouseState;
            }

            inline KeyboardInputState const *GetKeyboardState() const
            {
                if (auto pDevice = GetKeyboardMouseDevice())
                {
                    return &pDevice->GetKeyboardState();
                }

                return &s_emptyKeyboardState;
            }

            void ReflectState(Seconds const deltaTime, float timeScale, InputState &outReflectedState) const;

        private:
            KeyboardMouseInputDevice const *GetKeyboardMouseDevice() const;

        private:
            Vector<InputDevice *> m_inputDevices;
        };

        //-------------------------------------------------------------------------
        // Input State
        //-------------------------------------------------------------------------
        // A copy of the input state, used to contextually manage input state per system/world/etc...

        class KMP_BASE_API InputState
        {
            friend class InputSystem;

        public:
            InputState() = default;

            void Clear();

            KMP_FORCE_INLINE MouseInputState const *GetMouseState() const
            {
                return &m_mouseState;
            }

            KMP_FORCE_INLINE KeyboardInputState const *GetKeyboardState() const
            {
                return &m_keyboardState;
            }

        private:
            MouseInputState m_mouseState;
            KeyboardInputState m_keyboardState;
        };
    }
}