#include "GamePad.h"

extern "C" {
    GamePad* GamePad_create(uint db9_pin_7, uint db9_pin_1, uint db9_pin_2, uint db9_pin_3, uint db9_pin_4, uint db9_pin_6, uint db9_pin_9) {
        return new GamePad(db9_pin_7, db9_pin_1, db9_pin_2, db9_pin_3, db9_pin_4, db9_pin_6, db9_pin_9);
    }

    void GamePad_destroy(GamePad* gamepad) {
        delete gamepad;
    }

    uint16_t GamePad_getState(GamePad* gamepad) {
        return gamepad->getState();
    }
}