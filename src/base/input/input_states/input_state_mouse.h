#pragma once

#include "base/input/input_state.h"
#include "base/input/input_device.h"

//-------------------------------------------------------------------------

namespace kmp::input
{
    enum class MouseButton
    {
        Left = 0,
        Right,
        Middle,
        Back,
        Forward,
        Button5,
        Button6,
        Button7,
        Button8,
        Button9,
    };

    //-------------------------------------------------------------------------

    class MouseInputState : public ButtonStates<10>
    {
        friend class KeyboardMouseInputDevice;

    public:
        enum class WheelType
        {
            Vertical,
            Horizontal
        };

    public:
        void Clear() { ResetFrameState(ResetType::Full); }

        void ReflectFrom(Seconds const deltaTime, float timeScale, MouseInputState const &sourceState)
        {
            m_movementDelta = sourceState.m_movementDelta;
            m_position = m_position;
            m_verticalWheelDelta = m_verticalWheelDelta;
            m_horizontalWheelDelta = m_horizontalWheelDelta;

            ButtonStates<10>::ReflectFrom(deltaTime, timeScale, sourceState);
        }

        inline math::vec2 GetMovementDelta() const { return m_movementDelta; }
        inline int16_t GetWheelDelta(WheelType wheelType = WheelType::Vertical) const { return (wheelType == WheelType::Vertical) ? m_verticalWheelDelta : m_horizontalWheelDelta; }
        inline math::ivec2 GetCursorPosition() const { return m_position; }

        // Was the button just pressed (i.e. went from up to down this frame)
        KMP_FORCE_INLINE bool WasPressed(MouseButton buttonID) const { return ButtonStates::WasPressed((uint32_t)buttonID); }

        // Was the button just release (i.e. went from down to up this frame). Also optionally returns how long the button was held for
        KMP_FORCE_INLINE bool WasReleased(MouseButton buttonID, Seconds *pHeldDownDuration = nullptr) const { return ButtonStates::WasReleased((uint32_t)buttonID, pHeldDownDuration); }

        // Is the button being held down?
        KMP_FORCE_INLINE bool IsHeldDown(MouseButton buttonID, Seconds *pHeldDownDuration = nullptr) const { return ButtonStates::IsHeldDown((uint32_t)buttonID, pHeldDownDuration); }

        // How long has the button been held down for?
        KMP_FORCE_INLINE Seconds GetHeldDuration(MouseButton buttonID) const { return ButtonStates::GetHeldDuration((uint32_t)buttonID); }

    private:
        KMP_FORCE_INLINE void Press(MouseButton buttonID) { ButtonStates::Press((uint32_t)buttonID); }
        KMP_FORCE_INLINE void Release(MouseButton buttonID) { ButtonStates::Release((uint32_t)buttonID); }

        inline void ResetFrameState(ResetType resetType)
        {
            m_movementDelta = math::zero<math::vec2>();

            m_verticalWheelDelta = m_horizontalWheelDelta = 0;

            if (resetType == ResetType::Full)
            {
                ClearButtonState();
            }
        }

    private:
        math::vec2 m_movementDelta;
        math::ivec2 m_position;
        int16_t m_verticalWheelDelta = 0;
        int16_t m_horizontalWheelDelta = 0;
    };
}