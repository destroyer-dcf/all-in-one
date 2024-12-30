#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "GamePad.h"


GamePad::GamePad(uint db9_pin_7, uint db9_pin_1, uint db9_pin_2, uint db9_pin_3, uint db9_pin_4, uint db9_pin_6, uint db9_pin_9)
{
    // Set pins
    _selectPin = db9_pin_7;

    _inputPins[0] = db9_pin_1;
    _inputPins[1] = db9_pin_2;
    _inputPins[2] = db9_pin_3;
    _inputPins[3] = db9_pin_4;
    _inputPins[4] = db9_pin_6;
    _inputPins[5] = db9_pin_9;

    // Setup output pin
    gpio_init(_selectPin);
    gpio_set_dir(_selectPin, GPIO_OUT);
    gpio_put(_selectPin, 1);

    // Setup input pins
    for (uint i = 0; i < SC_INPUT_PINS; i++)
    {
        gpio_init(_inputPins[i]);
        gpio_set_dir(_inputPins[i], GPIO_IN);
        gpio_pull_up(_inputPins[i]);
    }

    _currentState = 0;
    _sixButtonMode = false;
    _lastReadTime = to_ms_since_boot(get_absolute_time());
}

uint16_t GamePad::getState()
{
    if (to_ms_since_boot(get_absolute_time()) - _lastReadTime < SC_READ_DELAY_MS)
    {
        // Not enough time has elapsed, return previously read state
        return _currentState;
    }

    // Clear current state
    _currentState = 0;

    for (uint cycle = 0; cycle < SC_CYCLES; cycle++)
    {
        readCycle(cycle);
    }

    // When a controller disconnects, revert to three-button polling
    if (!(_currentState & SC_CTL_ON))
    {
        _sixButtonMode = false;
    }

    _lastReadTime = to_ms_since_boot(get_absolute_time());

    return _currentState;
}

void GamePad::readCycle(uint cycle)
{
    // Set the select pin low/high
    gpio_put(_selectPin, cycle % 2);

    // Read flags
    switch (cycle)
    {
        case 2:
            // Check that a controller is connected
            _currentState |= (gpio_get(_inputPins[2]) == 0 && gpio_get(_inputPins[3]) == 0) * SC_CTL_ON;

            // Check controller is connected before reading A/Start to prevent bad reads when inserting/removing cable
            if (_currentState & SC_CTL_ON)
            {
                // Read input pins for A, Start
                if (gpio_get(_inputPins[4]) == 0) { _currentState |= SC_BTN_A; }
                if (gpio_get(_inputPins[5]) == 0) { _currentState |= SC_BTN_START; }
            }
            break;
        case 3:
            // Read input pins for Up, Down, Left, Right, B, C
            if (gpio_get(_inputPins[0]) == 0) { _currentState |= SC_BTN_UP; }
            if (gpio_get(_inputPins[1]) == 0) { _currentState |= SC_BTN_DOWN; }
            if (gpio_get(_inputPins[2]) == 0) { _currentState |= SC_BTN_LEFT; }
            if (gpio_get(_inputPins[3]) == 0) { _currentState |= SC_BTN_RIGHT; }
            if (gpio_get(_inputPins[4]) == 0) { _currentState |= SC_BTN_B; }
            if (gpio_get(_inputPins[5]) == 0) { _currentState |= SC_BTN_C; }
            break;
        case 4:
            _sixButtonMode = (gpio_get(_inputPins[0]) == 0 && gpio_get(_inputPins[1]) == 0);
            break;
        case 5:
            if (_sixButtonMode)
            {
                // Read input pins for X, Y, Z, Mode
                if (gpio_get(_inputPins[0]) == 0) { _currentState |= SC_BTN_Z; }
                if (gpio_get(_inputPins[1]) == 0) { _currentState |= SC_BTN_Y; }
                if (gpio_get(_inputPins[2]) == 0) { _currentState |= SC_BTN_X; }
                if (gpio_get(_inputPins[3]) == 0) { _currentState |= SC_BTN_MODE; }
            }
            break;
    }
}

int GamePad::detectJoystickType() {
    // Establecer el pin SELECT en LOW y leer las respuestas
    gpio_put(_selectPin, 0);
    sleep_ms(10);
    bool segaResponseLow = (gpio_get(_inputPins[4]) == 0 || gpio_get(_inputPins[5]) == 0);

    // Establecer el pin SELECT en HIGH y leer las respuestas
    gpio_put(_selectPin, 1);
    sleep_ms(10);
    bool segaResponseHigh = (gpio_get(_inputPins[0]) == 0 || gpio_get(_inputPins[1]) == 0);

    if (segaResponseLow && segaResponseHigh) {
        return 1; // Sega Joystick detectado
    } else if (!segaResponseLow && !segaResponseHigh) {
        return 2; // Amstrad Joystick detectado o no responde a SELECT
    } else {
        return 0; // Joystick no detectado o desconocido
    }
}