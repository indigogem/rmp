#include "input_device_keyboard_mouse.h"
#include "base/logging/log.h"

//-------------------------------------------------------------------------

namespace kmp
{
    namespace input
    {
        namespace KeyMap
        {
            // Virtual key code to EE keyboard buttons
            static HashMap<uint32_t, KeyboardButton> g_keyMappings;

            static bool GetButtonForKeyMessage(const SDL_KeyboardEvent &event, KeyboardButton &buttonID)
            {

                auto const keyIter = KeyMap::g_keyMappings.find((uint32_t)event.keysym.sym);
                if (keyIter != KeyMap::g_keyMappings.end())
                {
                    buttonID = keyIter->second;
                    return true;
                }

                return false;
            }

            static void Initialize()
            {
                g_keyMappings[SDLK_a] = KeyboardButton::Key_A;
                g_keyMappings[SDLK_b] = KeyboardButton::Key_B;
                g_keyMappings[SDLK_c] = KeyboardButton::Key_C;
                g_keyMappings[SDLK_d] = KeyboardButton::Key_D;
                g_keyMappings[SDLK_e] = KeyboardButton::Key_E;
                g_keyMappings[SDLK_f] = KeyboardButton::Key_F;
                g_keyMappings[SDLK_g] = KeyboardButton::Key_G;
                g_keyMappings[SDLK_h] = KeyboardButton::Key_H;
                g_keyMappings[SDLK_i] = KeyboardButton::Key_I;
                g_keyMappings[SDLK_j] = KeyboardButton::Key_J;
                g_keyMappings[SDLK_k] = KeyboardButton::Key_K;
                g_keyMappings[SDLK_l] = KeyboardButton::Key_L;
                g_keyMappings[SDLK_m] = KeyboardButton::Key_M;
                g_keyMappings[SDLK_n] = KeyboardButton::Key_N;
                g_keyMappings[SDLK_o] = KeyboardButton::Key_O;
                g_keyMappings[SDLK_p] = KeyboardButton::Key_P;
                g_keyMappings[SDLK_q] = KeyboardButton::Key_Q;
                g_keyMappings[SDLK_r] = KeyboardButton::Key_R;
                g_keyMappings[SDLK_s] = KeyboardButton::Key_S;
                g_keyMappings[SDLK_t] = KeyboardButton::Key_T;
                g_keyMappings[SDLK_u] = KeyboardButton::Key_U;
                g_keyMappings[SDLK_v] = KeyboardButton::Key_V;
                g_keyMappings[SDLK_w] = KeyboardButton::Key_W;
                g_keyMappings[SDLK_x] = KeyboardButton::Key_X;
                g_keyMappings[SDLK_y] = KeyboardButton::Key_Y;
                g_keyMappings[SDLK_z] = KeyboardButton::Key_Z;
                g_keyMappings[SDLK_0] = KeyboardButton::Key_0;
                g_keyMappings[SDLK_1] = KeyboardButton::Key_1;
                g_keyMappings[SDLK_2] = KeyboardButton::Key_2;
                g_keyMappings[SDLK_3] = KeyboardButton::Key_3;
                g_keyMappings[SDLK_4] = KeyboardButton::Key_4;
                g_keyMappings[SDLK_5] = KeyboardButton::Key_5;
                g_keyMappings[SDLK_6] = KeyboardButton::Key_6;
                g_keyMappings[SDLK_7] = KeyboardButton::Key_7;
                g_keyMappings[SDLK_8] = KeyboardButton::Key_8;
                g_keyMappings[SDLK_9] = KeyboardButton::Key_9;
                g_keyMappings[SDLK_COMMA] = KeyboardButton::Key_Comma;
                g_keyMappings[SDLK_PERIOD] = KeyboardButton::Key_Period;
                g_keyMappings[SDLK_SLASH] = KeyboardButton::Key_ForwardSlash;
                g_keyMappings[SDLK_SEMICOLON] = KeyboardButton::Key_SemiColon;

                // g_keyMappings[SDLK_APOSTROPHE] = KeyboardButton::Key_Quote;
                g_keyMappings[SDLK_QUOTE] = KeyboardButton::Key_Quote;

                g_keyMappings[SDLK_LEFTBRACKET] = KeyboardButton::Key_LBracket;
                g_keyMappings[SDLK_RIGHTBRACKET] = KeyboardButton::Key_RBracket;
                g_keyMappings[SDLK_BACKSLASH] = KeyboardButton::Key_BackSlash;
                g_keyMappings[SDLK_MINUS] = KeyboardButton::Key_Minus;
                g_keyMappings[SDLK_EQUALS] = KeyboardButton::Key_Plus;
                g_keyMappings[SDLK_BACKSPACE] = KeyboardButton::Key_Backspace;

                // g_keyMappings[SDLK_GRAVE] = KeyboardButton::Key_Tilde;
                g_keyMappings[SDLK_BACKQUOTE] = KeyboardButton::Key_Tilde;

                g_keyMappings[SDLK_TAB] = KeyboardButton::Key_Tab;
                g_keyMappings[SDLK_CAPSLOCK] = KeyboardButton::Key_CapsLock;
                g_keyMappings[SDLK_RETURN] = KeyboardButton::Key_Enter;
                g_keyMappings[SDLK_ESCAPE] = KeyboardButton::Key_Escape;
                g_keyMappings[SDLK_SPACE] = KeyboardButton::Key_Space;
                g_keyMappings[SDLK_LEFT] = KeyboardButton::Key_Left;
                g_keyMappings[SDLK_UP] = KeyboardButton::Key_Up;
                g_keyMappings[SDLK_RIGHT] = KeyboardButton::Key_Right;
                g_keyMappings[SDLK_DOWN] = KeyboardButton::Key_Down;
                g_keyMappings[SDLK_NUMLOCKCLEAR] = KeyboardButton::Key_NumLock;
                g_keyMappings[SDLK_KP_0] = KeyboardButton::Key_Numpad0;
                g_keyMappings[SDLK_KP_1] = KeyboardButton::Key_Numpad1;
                g_keyMappings[SDLK_KP_2] = KeyboardButton::Key_Numpad2;
                g_keyMappings[SDLK_KP_3] = KeyboardButton::Key_Numpad3;
                g_keyMappings[SDLK_KP_4] = KeyboardButton::Key_Numpad4;
                g_keyMappings[SDLK_KP_5] = KeyboardButton::Key_Numpad5;
                g_keyMappings[SDLK_KP_6] = KeyboardButton::Key_Numpad6;
                g_keyMappings[SDLK_KP_7] = KeyboardButton::Key_Numpad7;
                g_keyMappings[SDLK_KP_8] = KeyboardButton::Key_Numpad8;
                g_keyMappings[SDLK_KP_9] = KeyboardButton::Key_Numpad9;
                g_keyMappings[SDLK_KP_ENTER] = KeyboardButton::Key_NumpadEnter;
                g_keyMappings[SDLK_KP_MULTIPLY] = KeyboardButton::Key_NumpadMultiply;
                g_keyMappings[SDLK_KP_PLUS] = KeyboardButton::Key_NumpadPlus;
                g_keyMappings[SDLK_KP_MINUS] = KeyboardButton::Key_NumpadMinus;
                g_keyMappings[SDLK_KP_PERIOD] = KeyboardButton::Key_NumpadPeriod;
                g_keyMappings[SDLK_KP_DIVIDE] = KeyboardButton::Key_NumpadDivide;
                g_keyMappings[SDLK_F1] = KeyboardButton::Key_F1;
                g_keyMappings[SDLK_F2] = KeyboardButton::Key_F2;
                g_keyMappings[SDLK_F3] = KeyboardButton::Key_F3;
                g_keyMappings[SDLK_F4] = KeyboardButton::Key_F4;
                g_keyMappings[SDLK_F5] = KeyboardButton::Key_F5;
                g_keyMappings[SDLK_F6] = KeyboardButton::Key_F6;
                g_keyMappings[SDLK_F7] = KeyboardButton::Key_F7;
                g_keyMappings[SDLK_F8] = KeyboardButton::Key_F8;
                g_keyMappings[SDLK_F9] = KeyboardButton::Key_F9;
                g_keyMappings[SDLK_F10] = KeyboardButton::Key_F10;
                g_keyMappings[SDLK_F11] = KeyboardButton::Key_F11;
                g_keyMappings[SDLK_F12] = KeyboardButton::Key_F12;
                g_keyMappings[SDLK_F13] = KeyboardButton::Key_F13;
                g_keyMappings[SDLK_F14] = KeyboardButton::Key_F14;
                g_keyMappings[SDLK_F15] = KeyboardButton::Key_F15;
                g_keyMappings[SDLK_PRINTSCREEN] = KeyboardButton::Key_PrintScreen;
                g_keyMappings[SDLK_SCROLLLOCK] = KeyboardButton::Key_ScrollLock;
                g_keyMappings[SDLK_PAUSE] = KeyboardButton::Key_Pause;
                g_keyMappings[SDLK_INSERT] = KeyboardButton::Key_Insert;
                g_keyMappings[SDLK_DELETE] = KeyboardButton::Key_Delete;
                g_keyMappings[SDLK_HOME] = KeyboardButton::Key_Home;
                g_keyMappings[SDLK_END] = KeyboardButton::Key_End;
                g_keyMappings[SDLK_PAGEUP] = KeyboardButton::Key_PageUp;
                g_keyMappings[SDLK_PAGEDOWN] = KeyboardButton::Key_PageDown;
                g_keyMappings[SDLK_APPLICATION] = KeyboardButton::Key_Application;
                g_keyMappings[SDLK_LSHIFT] = KeyboardButton::Key_LShift;
                g_keyMappings[SDLK_RSHIFT] = KeyboardButton::Key_RShift;
                g_keyMappings[SDLK_LCTRL] = KeyboardButton::Key_LCtrl;
                g_keyMappings[SDLK_RCTRL] = KeyboardButton::Key_RCtrl;
                g_keyMappings[SDLK_LALT] = KeyboardButton::Key_LAlt;
                g_keyMappings[SDLK_RALT] = KeyboardButton::Key_RAlt;
            }

            static void Shutdown()
            {
                g_keyMappings.clear(true);
            }
        }

        void KeyboardMouseInputDevice::Initialize()
        {
            // SDL_SetWindowMouseGrab
            // SDL_ShowCursor

            KeyMap::Initialize();
        }

        void KeyboardMouseInputDevice::Shutdown()
        {
            KeyMap::Shutdown();
        }

        void KeyboardMouseInputDevice::UpdateState(Seconds deltaTime)
        {
            m_mouseState.Update(deltaTime);
            m_keyboardState.Update(deltaTime);
        }

        void KeyboardMouseInputDevice::ClearFrameState(ResetType resetType)
        {
            m_mouseState.ResetFrameState(resetType);
            m_keyboardState.ResetFrameState(resetType);
        }

        void KeyboardMouseInputDevice::ProcessMessage(const SDL_Event &input_event)
        {

            switch (input_event.type)
            {

            case SDL_EVENT_WINDOW_FOCUS_GAINED:
            case SDL_EVENT_WINDOW_FOCUS_LOST:
            {
                m_mouseState.ClearButtonState();
                m_keyboardState.ClearButtonState();
                break;
            }

            case SDL_EVENT_KEY_DOWN:
            {
                auto r = input_event.key;
                KeyboardButton buttonID;
                if (KeyMap::GetButtonForKeyMessage(input_event.key, buttonID))
                {
                    m_keyboardState.Press(buttonID);
                }
                break;
            }
            case SDL_EVENT_KEY_UP:
            {
                KeyboardButton buttonID;
                if (KeyMap::GetButtonForKeyMessage(input_event.key, buttonID))
                {
                    m_keyboardState.Release(buttonID);
                }
                break;
            }

            case SDL_EVENT_MOUSE_MOTION:
            {
                auto x = input_event.motion.x;
                auto y = input_event.motion.y;
                auto xrel = input_event.motion.xrel;
                auto yrel = input_event.motion.yrel;

                m_mouseState.m_position.x = x;
                m_mouseState.m_position.y = y;
                m_mouseState.m_movementDelta.x += (float)xrel;
                m_mouseState.m_movementDelta.y += (float)yrel;
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                if (input_event.button.button == SDL_BUTTON_LEFT)
                {
                    m_mouseState.Release(MouseButton::Left);
                }
                else if (input_event.button.button == SDL_BUTTON_RIGHT)
                {
                    m_mouseState.Release(MouseButton::Right);
                }
                else if (input_event.button.button == SDL_BUTTON_MIDDLE)
                {
                    m_mouseState.Release(MouseButton::Middle);
                }
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                if (input_event.button.button == SDL_BUTTON_LEFT)
                {
                    m_mouseState.Press(MouseButton::Left);
                }
                else if (input_event.button.button == SDL_BUTTON_RIGHT)
                {
                    m_mouseState.Press(MouseButton::Right);
                }
                else if (input_event.button.button == SDL_BUTTON_MIDDLE)
                {
                    m_mouseState.Press(MouseButton::Middle);
                }

                break;
            }

            case SDL_EVENT_MOUSE_WHEEL:
            {
                auto x = input_event.wheel.x;
                auto y = input_event.wheel.y;
                m_mouseState.m_verticalWheelDelta = y;
                m_mouseState.m_horizontalWheelDelta = x;

                break;
            }
            }
        }

    }
}