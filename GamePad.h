#ifndef GamePad_h
#define GamePad_h

#include "pico/stdlib.h"

enum
{
    SC_CTL_ON    = 1, // The controller is connected
    SC_BTN_UP    = 2,
    SC_BTN_DOWN  = 4,
    SC_BTN_LEFT  = 8,
    SC_BTN_RIGHT = 16,
    SC_BTN_START = 32,
    SC_BTN_A     = 64,
    SC_BTN_B     = 128,
    SC_BTN_C     = 256,
    SC_BTN_X     = 512,
    SC_BTN_Y     = 1024,
    SC_BTN_Z     = 2048,
    SC_BTN_MODE  = 4096,
    SC_BTN_1     = 128, // Master System compatibility
    SC_BTN_2     = 256  // Master System compatibility
};

const uint SC_INPUT_PINS = 6;
const uint SC_CYCLES = 8;
const uint SC_READ_DELAY_MS = 5; // Must be >= 3 to give 6-button controller time to reset

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
class GamePad {
    public:
        GamePad(uint db9_pin_7, uint db9_pin_1, uint db9_pin_2, uint db9_pin_3, uint db9_pin_4, uint db9_pin_6, uint db9_pin_9);
        uint16_t getState();
        int detectJoystickType();  // Nueva función para detectar el tipo de joystick

    private:
        void readCycle(uint cycle);
        uint16_t _currentState;
        uint _lastReadTime;
        bool _sixButtonMode;
        uint _selectPin; // output select pin
        uint _inputPins[SC_INPUT_PINS];
};
#endif

#ifdef __cplusplus
}
#endif

#endif // GamePad_h