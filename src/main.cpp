#include <Arduino.h>
#include <GamePad.h>
#include <ps2dev.h> 
#include <config.hpp>
#include <Keypad.h>
#include <OLED_I2C.h>
#include <avr/pgmspace.h>

// GAMEPAD
// 5 4 3 2 1
//  9 8 7 6

// DB9 Pin 1 --> Arduino Pin 15
// DB9 Pin 2 --> Arduino Pin 14
// DB9 Pin 3 --> Arduino Pin 4
// DB9 Pin 4 --> Arduino Pin 5
// DB9 Pin 5 --> Arduino +5V
// DB9 Pin 6 --> Arduino Pin 6
// DB9 Pin 7 --> Arduino Pin 7 Select
// DB9 Pin 8 --> Arduino GND
// DB9 Pin 9 --> Arduino Pin 8

GamePad controller(7, 15, 14, 4, 5, 6, 8);

// OLED
OLED  myOLED(SDA, SCL);

// Definir el teclado de la cinta
const byte filas = 2; 
const byte columnas = 4;
byte pinesFilas[]  = {10,16};
byte pinesColumnas[] = {A0,A1,A2,A3};
char teclas[2][4] = {{'1','2','3','4'},
                     {'5','6','7','8'}};

Keypad keypad = Keypad( makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas); 

// Crear una instancia de PS2dev para el teclado PS2
PS2dev keyboard(PS2_CLK_PIN, PS2_DATA_PIN);

int selectValue = 0;
String keyCapActive = "SELECT";

// Controller states Joystick
word currentState = 0;
word lastState = 0;

void displayTextOLED(const String& text) {
    myOLED.clrScr();
    myOLED.print(text, 0, 0);
    myOLED.update();
}

void gamePadState()
{
    // Verificar el cambio de estado para cada botón
    if ((currentState & SC_BTN_UP) != (lastState & SC_BTN_UP)) {
        if (currentState & SC_BTN_UP) {
            Serial.println("Botón UP presionado");
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1UP);
        } else {
            Serial.println("Botón UP soltado");
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1UP);
        }
    }

    if ((currentState & SC_BTN_DOWN) != (lastState & SC_BTN_DOWN)) {
        if (currentState & SC_BTN_DOWN) {
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1DOWN);
            Serial.println("Botón DOWN presionado");
        } else {
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1DOWN);
            Serial.println("Botón DOWN soltado");
        }
    }

    if ((currentState & SC_BTN_LEFT) != (lastState & SC_BTN_LEFT)) {
        if (currentState & SC_BTN_LEFT) {
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1LEFT);
            Serial.println("Botón LEFT presionado");
        } else {
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1LEFT);
            Serial.println("Botón LEFT soltado");
        }
    }

    if ((currentState & SC_BTN_RIGHT) != (lastState & SC_BTN_RIGHT)) {
        if (currentState & SC_BTN_RIGHT) {
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1RIGHT);
            Serial.println("Botón RIGHT presionado");
        } else {
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1RIGHT);
            Serial.println("Botón RIGHT soltado");
        }
    }

    if ((currentState & SC_BTN_START) != (lastState & SC_BTN_START)) {
        if (currentState & SC_BTN_START) {
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1START);
            Serial.println("Botón START presionado");
        } else {
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1START);
            Serial.println("Botón START soltado");
        }
    }

    if ((currentState & SC_BTN_A) != (lastState & SC_BTN_A)) {
        if (currentState & SC_BTN_A) {
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1A);
            Serial.println("Botón A presionado");
        } else {
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1A);
            Serial.println("Botón A soltado");
        }
    }

    if ((currentState & SC_BTN_B) != (lastState & SC_BTN_B)) {
        if (currentState & SC_BTN_B) {
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1B);
            Serial.println("Botón B presionado");
        } else {
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1B);
            Serial.println("Botón B soltado");
        }
    }

    if ((currentState & SC_BTN_C) != (lastState & SC_BTN_C)) {
        if (currentState & SC_BTN_C) {
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1C);
            Serial.println("Botón C presionado");
        } else {
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1C);
            Serial.println("Botón C soltado");
        }
    }

    if ((currentState & SC_BTN_X) != (lastState & SC_BTN_X)) {
        if (currentState & SC_BTN_X) {
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1X);
            Serial.println("Botón X presionado");
        } else {
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1X);
            Serial.println("Botón X soltado");
        }
    }

    if ((currentState & SC_BTN_Y) != (lastState & SC_BTN_Y)) {
        if (currentState & SC_BTN_Y) {
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1Y);
            Serial.println("Botón Y presionado");
        } else {
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1Y);
            Serial.println("Botón Y soltado");
        }
    }

    if ((currentState & SC_BTN_Z) != (lastState & SC_BTN_Z)) {
        if (currentState & SC_BTN_Z) {
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1Z);
            Serial.println("Botón Z presionado");
        } else {
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1Z);
            Serial.println("Botón Z soltado");
        }
    }

    if ((currentState & SC_BTN_1) != (lastState & SC_BTN_1)) {
        if (currentState & SC_BTN_1) {
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1A);
            Serial.println("Botón 1 presionado");
        } else {
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1A);
            Serial.println("Botón 1 soltado");
        }
    }

    if ((currentState & SC_BTN_2) != (lastState & SC_BTN_2)) {
        if (currentState & SC_BTN_2) {
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1Z);
            Serial.println("Botón 2 presionado");
        } else {
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1Z);
            Serial.println("Botón 2 soltado");
        }
    }

    if ((currentState & SC_BTN_MODE) != (lastState & SC_BTN_MODE)) {
        if (currentState & SC_BTN_MODE) {
            keyboard.keyboard_press_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1MODE);
            Serial.println("Botón MODE presionado");
        } else {
            keyboard.keyboard_release_ESPectrum_special(PS2dev::ESPectrumSpecialScanCodes::ESP_JOY1MODE);
            Serial.println("Botón MODE soltado");
        }
    }

    // Actualizar el último estado
    lastState = currentState;
}

void paintTape() {
    myOLED.drawBitmap(10, 26, cassette32, 32, 32);
    myOLED.update();
}

void paintSnap() {
    myOLED.drawBitmap(10, 26, snapshot32, 32, 29);
    myOLED.update();
}

void paintSnapCusmton() {
    myOLED.drawBitmap(10, 26, snapshot32, 32, 29);
    myOLED.print("C", 23, 51);
    myOLED.update();
}

void paintJoystick() {
    myOLED.drawBitmap(85, 26, joystick32, 32, 29);
    myOLED.update();
}

void paintMouse() {
    myOLED.drawBitmap(85, 26, mouse32, 32, 29);
    myOLED.update();
}

void paintPlay() {
    myOLED.drawBitmap(51, 28, rew, 24, 24);
    myOLED.update();
}

void paintRew() {
    myOLED.drawBitmap(51, 28, rew, 24, 24);
    myOLED.update();
}

void paintStop() {
    myOLED.drawBitmap(51, 28, stop, 24, 24);
    myOLED.update();
}

void paintFF() {
    myOLED.drawBitmap(51, 28, ff, 24, 24);
    myOLED.update();
}

void paintPause() {
    myOLED.drawBitmap(51, 28, pause, 24, 24);
    myOLED.update();
}

void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
        case PRESSED:
            switch (key) {
                case '1': // SELECT
                    selectValue = (selectValue + 1) % 3; // Incrementar y ciclar entre 0 y 2

                    switch (selectValue) {
                        case 0:
                            paintTape();
                            Serial.println("--- CASSETTE SELECTED, Value: " + String(selectValue));
                            keyboard.keyboard_press(PS2dev::ESCAPE);
                            break;
                        case 1:
                            paintSnap();
                            Serial.println("--- SNA SELECTED, Value: " + String(selectValue));
                            keyboard.keyboard_press(PS2dev::ESCAPE);
                            break;
                        case 2:
                            paintSnapCusmton();
                            Serial.println("--- CUSMTON SNA SELECTED, Value: " + String(selectValue));
                            keyboard.keyboard_press(PS2dev::ESCAPE);
                            break;
                    }
                    break;
                case '2': // REC
                    keyCapActive = "REC";
                    Serial.println("REC PRESSED");
                    keyboard.keyboard_press(PS2dev::F4);
                    break;
                case '3': // PLAY
                    keyCapActive = "PLAY";
                    Serial.println("PLAY PRESSED");
                    keyboard.keyboard_press(PS2dev::ENTER);
                    break;
                case '4': // REW
                    keyCapActive = "REW";
                    Serial.println("REW PRESSED");
                    keyboard.keyboard_press_special(PS2dev::SpecialScanCodes::DOWN_ARROW);
                    break;
                case '5': // FF
                    keyCapActive = "FF";
                    Serial.println("FF PRESSED");
                    keyboard.keyboard_press_special(PS2dev::SpecialScanCodes::UP_ARROW);
                    break;
                case '6': // STOP/EJECT
                    if (selectValue == 0) {
                        keyboard.keyboard_press(PS2dev::F5);
                    } else if (selectValue == 1) {
                        keyboard.keyboard_press(PS2dev::F2);
                    } else if (selectValue == 2) {
                        keyboard.keyboard_press(PS2dev::F3);
                    }
                    break;
                case '7': // PAUSE
                    Serial.println("PAUSE PRESSED");
                    keyboard.key_pause();
                    //keyboard.keyboard_press(PS2dev::ESCAPE);
                    keyCapActive = "PAUSE";
                    break;
                case '8': // NADA DE NADA
                    // keyboard.key_break();
                    keyboard.keyboard_press(PS2dev::ESCAPE);
                    break;
            }

            break;
        case RELEASED:
            switch (key) {
                case '1': // SELECT
                    keyboard.keyboard_release(PS2dev::ESCAPE);
                    break;
                case '2': // REC
                    Serial.println("REC RELEASED");
                    keyboard.keyboard_release(PS2dev::F4);
                    keyCapActive = "NADA";
                    break;
                case '3': // PLAY
                    keyCapActive = "PLAY";
                    Serial.println("PLAY RELEASED");
                    keyboard.keyboard_release(PS2dev::ENTER);
                    break;
                case '4': // REW
                    keyCapActive = "NADA";
                    Serial.println("REW RELEASED");
                    keyboard.keyboard_release_special(PS2dev::SpecialScanCodes::DOWN_ARROW);
                    break;
                case '5': // FF
                    keyCapActive = "FF";
                    Serial.println("FF RELEASED");
                    keyboard.keyboard_release_special(PS2dev::SpecialScanCodes::UP_ARROW);
                    break;
                case '6': // STOP/EJECT
                    if (selectValue == 0) {
                        keyboard.keyboard_release(PS2dev::F5);
                    } else if (selectValue == 1) {
                        keyboard.keyboard_release(PS2dev::F2);
                    } else if (selectValue == 2) {
                        keyboard.keyboard_release(PS2dev::F3);
                    }
                    break;
                case '7': // PAUSE
                    Serial.println("PAUSE RELEASED");
                    
                    break;
                case '8': // NADA DE NADA
                    keyboard.keyboard_release(PS2dev::ESCAPE);
                    // sendKeyBoardAction(PS2dev::LEFT_CONTROL, false);
                    // sendKeyBoardAction(PS2dev::ENTER, false);
                    break;
            }
            break;
        case HOLD:
            // Acción para mantener presionada una tecla    
            break;
        case IDLE:
            // Acción para cuando no se presiona ninguna tecla
            break;    }
}

void setup() {
    Serial.begin(9600);
    myOLED.begin(SSD1306_128X64);
    myOLED.setBrightness(255);
    myOLED.setFont(SmallFont);
    myOLED.clrScr();
    myOLED.drawRoundRect(0,63,127,16);
    paintTape();
    paintJoystick();
    keypad.addEventListener(keypadEvent);
}

void loop()
{
    keypad.getKey();

    currentState = controller.getState();
    gamePadState();


}
