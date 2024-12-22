#include <Arduino.h>
#include <GamePad.h>
#include <ps2dev.h> 
#include <config.hpp>

//
// 5 4 3 2 1
//  9 8 7 6

// DB9 Pin 1 --> Arduino Pin 2
// DB9 Pin 2 --> Arduino Pin 3
// DB9 Pin 3 --> Arduino Pin 4
// DB9 Pin 4 --> Arduino Pin 5
// DB9 Pin 5 --> Arduino +5V
// DB9 Pin 6 --> Arduino Pin 6
// DB9 Pin 7 --> Arduino Pin 7 Select
// DB9 Pin 8 --> Arduino GND
// DB9 Pin 9 --> Arduino Pin 8

GamePad controller(7, 2, 3, 4, 5, 6, 8);




// Crear una instancia de PS2dev para el teclado PS2pn
PS2dev keyboard(PS2_CLK_PIN, PS2_DATA_PIN);

// Controller states
word currentState = 0;
word lastState = 0;

bool selectPressed = false;
bool recPressed = false;
bool playPressed = false;
bool rewPressed = false;
bool ffPressed = false;
bool stopEjectPressed = false;
bool pausePressed = false;

void sendGamePadAction(int scancode, bool press) {
  keyboard.write(0xE2); // Prefijo para gamepad
  delay(15);
  if (!press) {
    keyboard.write(0xF0); // Código de liberación
    delay(15);
  }
  keyboard.write(scancode);
  delay(15);
}

void sendKeyBoardAction(int scancode, bool press) {
  if (press) {
    keyboard.write(scancode);
  } else {
    keyboard.write(0xF0); // Código de liberación
    delay(15);
    keyboard.write(scancode);
  }
  delay(15);
}

void sendSpecialKeyBoardAction(int scancode, bool press) {
  keyboard.write(0xE0); // Prefijo para teclas especiales
  delay(15);
  if (!press) {
    keyboard.write(0xF0); // Código de liberación
    delay(15);
  }
  keyboard.write(scancode);
  delay(15);
}

void EjectCassette() {
    sendKeyBoardAction(PS2dev::LEFT_SHIFT, true);
    delay(100);
    sendKeyBoardAction(PS2dev::F6, true);    
}

void cassetteState() {

    byte swichState = digitalRead(SELECT_SWICH);
    if (swichState == LOW && !selectPressed) {
        Serial.println("SELECT is pressed");
        // sendKeyBoardAction(PS2dev::F5, true);
        //sendSpecialKeyBoardAction(PS2dev::SpecialScanCodes::UP_ARROW, true);  // Presionar UP_ARROW
        sendKeyBoardAction(PS2dev::LEFT_SHIFT, true);  // Presionar SHIFT
        delay(100);
        sendKeyBoardAction(PS2dev::F6, true);    
        selectPressed = true;
    } else if (swichState == HIGH) {
        sendSpecialKeyBoardAction(PS2dev::SpecialScanCodes::UP_ARROW, false);
        selectPressed = false;
    }

    swichState = digitalRead(REC_SWICH);
    if (swichState == LOW && !recPressed) {
        Serial.println("REC is pressed");
        recPressed = true;
    } else if (swichState == HIGH) {
        recPressed = false;
    }

    swichState = digitalRead(PLAY_SWICH);
    if (swichState == LOW && !playPressed) {
        Serial.println("PLAY is pressed");
        playPressed = true;
    } else if (swichState == HIGH) {
        playPressed = false;
    }

    swichState = digitalRead(REW_SWICH);
    if (swichState == LOW && !rewPressed) {
        Serial.println("FF is pressed");
        rewPressed = true;
    } else if (swichState == HIGH) {
        rewPressed = false;
    }

    swichState = digitalRead(FF_SWICH);
    if (swichState == LOW && !ffPressed) {
        Serial.println("FF is pressed");
        ffPressed = true;
    } else if (swichState == HIGH) {
        ffPressed = false;
    }

    swichState = digitalRead(STOP_EJECT_SWICH);
    if (swichState == LOW && !stopEjectPressed) {
        Serial.println("STOP is pressed");
        stopEjectPressed = true;
    } else if (swichState == HIGH) {
        stopEjectPressed = false;
    }

    swichState = digitalRead(PAUSE_SWICH);
    if (swichState == LOW && !pausePressed) {
        Serial.println("PAUSE is pressed");
        pausePressed = true;
    } else if (swichState == HIGH) {
        pausePressed = false;
    }
}

void gamePadState()
{
    // Verificar el cambio de estado para cada botón
    if ((currentState & SC_BTN_UP) != (lastState & SC_BTN_UP)) {
        if (currentState & SC_BTN_UP) {
            Serial.println("Botón UP presionado");
            sendGamePadAction(PS2dev::ESP_JOY1UP, true);
        } else {
            Serial.println("Botón UP soltado");
            sendGamePadAction(PS2dev::ESP_JOY1UP, false);
        }
    }

    if ((currentState & SC_BTN_DOWN) != (lastState & SC_BTN_DOWN)) {
        if (currentState & SC_BTN_DOWN) {
            sendGamePadAction(PS2dev::ESP_JOY1DOWN, true);
            Serial.println("Botón DOWN presionado");
        } else {
            sendGamePadAction(PS2dev::ESP_JOY1DOWN, false);
            Serial.println("Botón DOWN soltado");
        }
    }

    if ((currentState & SC_BTN_LEFT) != (lastState & SC_BTN_LEFT)) {
        if (currentState & SC_BTN_LEFT) {
            sendGamePadAction(PS2dev::ESP_JOY1LEFT, true);
            Serial.println("Botón LEFT presionado");
        } else {
          sendGamePadAction(PS2dev::ESP_JOY1LEFT, false);
            Serial.println("Botón LEFT soltado");
        }
    }

    if ((currentState & SC_BTN_RIGHT) != (lastState & SC_BTN_RIGHT)) {
        if (currentState & SC_BTN_RIGHT) {
            sendGamePadAction(PS2dev::ESP_JOY1RIGHT, true);
            Serial.println("Botón RIGHT presionado");
        } else {
            sendGamePadAction(PS2dev::ESP_JOY1RIGHT, false);
            Serial.println("Botón RIGHT soltado");
        }
    }

    if ((currentState & SC_BTN_START) != (lastState & SC_BTN_START)) {
        if (currentState & SC_BTN_START) {
            sendGamePadAction(PS2dev::ESP_JOY1START, true);
            Serial.println("Botón START presionado");
        } else {
            sendGamePadAction(PS2dev::ESP_JOY1START, false);
            Serial.println("Botón START soltado");
        }
    }

    if ((currentState & SC_BTN_A) != (lastState & SC_BTN_A)) {
        if (currentState & SC_BTN_A) {
            sendGamePadAction(PS2dev::ESP_JOY1A, true);
            Serial.println("Botón A presionado");
        } else {
            sendGamePadAction(PS2dev::ESP_JOY1A, false);
            Serial.println("Botón A soltado");
        }
    }

    if ((currentState & SC_BTN_B) != (lastState & SC_BTN_B)) {
        if (currentState & SC_BTN_B) {
            sendGamePadAction(PS2dev::ESP_JOY1B, true);
            Serial.println("Botón B presionado");
        } else {
            sendGamePadAction(PS2dev::ESP_JOY1B, false);
            Serial.println("Botón B soltado");
        }
    }

    if ((currentState & SC_BTN_C) != (lastState & SC_BTN_C)) {
        if (currentState & SC_BTN_C) {
            sendGamePadAction(PS2dev::ESP_JOY1C, true);
            Serial.println("Botón C presionado");
        } else {
            sendGamePadAction(PS2dev::ESP_JOY1C, false);
            Serial.println("Botón C soltado");
        }
    }

    if ((currentState & SC_BTN_X) != (lastState & SC_BTN_X)) {
        if (currentState & SC_BTN_X) {
            sendGamePadAction(PS2dev::ESP_JOY1X, true);
            Serial.println("Botón X presionado");
        } else {
            sendGamePadAction(PS2dev::ESP_JOY1X, false);
            Serial.println("Botón X soltado");
        }
    }

    if ((currentState & SC_BTN_Y) != (lastState & SC_BTN_Y)) {
        if (currentState & SC_BTN_Y) {
            sendGamePadAction(PS2dev::ESP_JOY1Y, true);
            Serial.println("Botón Y presionado");
        } else {
            sendGamePadAction(PS2dev::ESP_JOY1Y, false);
            Serial.println("Botón Y soltado");
        }
    }

    if ((currentState & SC_BTN_Z) != (lastState & SC_BTN_Z)) {
        if (currentState & SC_BTN_Z) {
            sendGamePadAction(PS2dev::ESP_JOY1Z, true);
            Serial.println("Botón Z presionado");
        } else {
            sendGamePadAction(PS2dev::ESP_JOY1Z, false);
            Serial.println("Botón Z soltado");
        }
    }

    if ((currentState & SC_BTN_1) != (lastState & SC_BTN_1)) {
        if (currentState & SC_BTN_1) {
            sendGamePadAction(PS2dev::ESP_JOY1A, true);
            Serial.println("Botón 1 presionado");
        } else {
            sendGamePadAction(PS2dev::ESP_JOY1A, false);
            Serial.println("Botón 1 soltado");
        }
    }

    if ((currentState & SC_BTN_2) != (lastState & SC_BTN_2)) {
        if (currentState & SC_BTN_2) {
            sendGamePadAction(PS2dev::ESP_JOY1Z, true);
            Serial.println("Botón 2 presionado");
        } else {
            sendGamePadAction(PS2dev::ESP_JOY1Z, false);
            Serial.println("Botón 2 soltado");
        }
    }

    if ((currentState & SC_BTN_MODE) != (lastState & SC_BTN_MODE)) {
        if (currentState & SC_BTN_MODE) {
            sendGamePadAction(PS2dev::ESP_JOY1MODE, true);
            Serial.println("Botón MODE presionado");
        } else {
            sendGamePadAction(PS2dev::ESP_JOY1MODE, false);
            Serial.println("Botón MODE soltado");
        }
    }

    // Actualizar el último estado
    lastState = currentState;
}


void setup() {

    Serial.begin(9600);

    pinMode(SELECT_SWICH, INPUT_PULLUP);
    pinMode(REC_SWICH, INPUT_PULLUP);
    pinMode(PLAY_SWICH, INPUT_PULLUP);
    pinMode(REW_SWICH, INPUT_PULLUP);
    pinMode(FF_SWICH, INPUT_PULLUP);
    pinMode(STOP_EJECT_SWICH, INPUT_PULLUP);
    pinMode(PAUSE_SWICH, INPUT_PULLUP);
}

void loop()
{
    currentState = controller.getState();
    gamePadState();
    cassetteState();
    delay(100);

}
