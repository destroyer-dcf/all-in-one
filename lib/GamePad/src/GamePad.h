#ifndef GamePad_h
#define GamePad_h

#include <Arduino.h>

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

const byte SC_INPUT_PINS = 6;

const byte SC_CYCLES = 8;

const unsigned long SC_READ_DELAY_MS = 5; // Must be >= 3 to give 6-button controller time to reset

class GamePad {
    public:
        GamePad(byte db9_pin_7, byte db9_pin_1, byte db9_pin_2, byte db9_pin_3, byte db9_pin_4, byte db9_pin_6, byte db9_pin_9);
        
        word getState();
        int detectJoystickType();  // Nueva función para detectar el tipo de joystick

    private:
        void readCycle(byte cycle);

        word _currentState;

        unsigned long _lastReadTime;

        boolean _sixButtonMode;

        byte _selectPin; // output select pin
        byte _inputPins[SC_INPUT_PINS];
};

#endif