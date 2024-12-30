#ifndef GAMEPADWRAPPER_H
#define GAMEPADWRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GamePad GamePad;

GamePad* GamePad_create(uint db9_pin_7, uint db9_pin_1, uint db9_pin_2, uint db9_pin_3, uint db9_pin_4, uint db9_pin_6, uint db9_pin_9);
void GamePad_destroy(GamePad* gamepad);
uint16_t GamePad_getState(GamePad* gamepad);

#ifdef __cplusplus
}
#endif

#endif // GAMEPADWRAPPER_H