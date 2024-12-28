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

String selectValue = "CASSETTE";
String Cursors = "UP-DOWN";
String keyCapActive = "SELECT";
String HardwareDisplay = "OLED"; //OLED O LCD

// Controller states Joystick
word currentState = 0;
word lastState = 0;

void displayTextOLED(const String& text) {
    myOLED.clrScr();
    myOLED.print(text, 0, 0);
    myOLED.update();
}

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

void StopEject() {
    if (selectValue == "CASSETTE") {
        sendKeyBoardAction(PS2dev::F5, true);
    } else if (selectValue == "SNA") {
        sendKeyBoardAction(PS2dev::F2, true);
    } else if (selectValue == "CUMSTON SNA") {
        sendKeyBoardAction(PS2dev::F3, true);
    } else if (selectValue == "RAPIDA") {
        sendKeyBoardAction(PS2dev::F4, true);
    }
    // sendKeyBoardAction(PS2dev::F5, true);
    // sendKeyBoardAction(PS2dev::LEFT_SHIFT, true);
    // delay(100);
    // sendKeyBoardAction(PS2dev::F6, true);    
}


void Insert() {
    keyCapActive = "INSERT";
    sendKeyBoardAction(PS2dev::LEFT_SHIFT, true);
    delay(100);
    sendKeyBoardAction(PS2dev::F5, true);    
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

void paintTape() {
    myOLED.drawBitmap(10, 28, cassette32, 32, 32);
    myOLED.print("      ", 11, 22);
    myOLED.print("N", 24, 22);
    myOLED.drawLine(10,26,22,26);
    myOLED.drawLine(32,26,42,26);
    myOLED.update();
}

void paintTapeRapid() {
    myOLED.drawBitmap(10, 28, cassette32, 32, 32);
    myOLED.print("      ", 11, 22);
    myOLED.print("R", 24, 22);
    myOLED.drawLine(10,26,22,26);
    myOLED.drawLine(32,26,42,26);
    myOLED.update();
}

void paintSnap() {
    myOLED.drawBitmap(10, 30, snapshot32, 32, 29);
    myOLED.print("SNAP", 11, 22);
    myOLED.update();
}

void paintSnapCumston() {
    myOLED.drawBitmap(10, 30, snapshot32, 32, 29);
    myOLED.print("SNAP", 11, 22);
    myOLED.print("C", 23, 53);
    myOLED.update();
}

void paintJoystick() {
    myOLED.drawBitmap(85, 28, joystick32, 32, 29);
    // myOLED.print("JOY", 90, 22);
    myOLED.update();
}

void paintMouse() {
    myOLED.drawBitmap(85, 30, mouse32, 32, 29);
    myOLED.print("MOUSE", 86, 22);
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

void sendKeyEnter() {
  sendKeyBoardAction(0x5A, true);  // Presionar la tecla ENTER
  delay(100);                      // Mantener la tecla presionada por 100 ms
  sendKeyBoardAction(0x5A, false); // Liberar la tecla ENTER
}

void sendKeyDown() {
  sendSpecialKeyBoardAction(PS2dev::SpecialScanCodes::DOWN_ARROW, true);  // Presionar la tecla DOWN_ARROW
  delay(15);
  sendSpecialKeyBoardAction(PS2dev::SpecialScanCodes::DOWN_ARROW, false); // Liberar la tecla DOWN_ARROW
}

void sendKeyUp() {
  sendSpecialKeyBoardAction(PS2dev::SpecialScanCodes::UP_ARROW, true);  // Presionar la tecla UP_ARROW
  delay(15);
  sendSpecialKeyBoardAction(PS2dev::SpecialScanCodes::UP_ARROW, false); // Liberar la tecla UP_ARROW
}

void sendKeyF1() {
  sendKeyBoardAction(PS2dev::F1, true);  // Presionar la tecla F1
  delay(15);
  sendKeyBoardAction(PS2dev::F1, false); // Liberar la tecla F1
}

void sendKeyEsc() {
  sendKeyBoardAction(PS2dev::ESCAPE, true);  // Presionar la tecla ESC
  delay(15);
  sendKeyBoardAction(PS2dev::ESCAPE, false); // Liberar la tecla ESC
}

void selectRapidLoad() {
    sendKeyF1();
    sendKeyDown();
    sendKeyDown();
    sendKeyDown();
    sendKeyDown();
    sendKeyEnter();
    sendKeyEnter();
    sendKeyDown();
    sendKeyEnter();
    // //Estando en No para si 
    sendKeyUp();
    sendKeyEnter();

    // Estando en SI para NO
    // sendKeyDown();
    // sendKeyEnter();  
    sendKeyEsc();
    sendKeyEsc();
    sendKeyEsc();
    sendKeyEsc();
}

void selectNormalLoad() {
    sendKeyF1();
    sendKeyDown();
    sendKeyDown();
    sendKeyDown();
    sendKeyDown();
    sendKeyEnter();
    sendKeyEnter();
    sendKeyDown();
    sendKeyEnter();
    // //Estando en No para si 
    // sendKeyUp();
    // sendKeyEnter();

    // Estando en SI para NO
    sendKeyDown();
    sendKeyEnter();  
    sendKeyEsc();
    sendKeyEsc();
    sendKeyEsc();
    sendKeyEsc();
}

// void select() {
//     if (selectValue == "CASSETTE") {
//         selectValue = "SNA";
//         paintSnap();
//     } else if (selectValue == "SNA") {
//         selectValue = "CUMSTON SNA";
//         paintSnapCumston();
//     } else if (selectValue == "CUMSTON SNA") {
//         selectValue = "CASSETTE";
//         selectNormalLoad();
//         paintTape();
//     }

//     Serial.println("--- " + selectValue + " SELECTED");
// }
void select() {
    if (selectValue == "CASSETTE") {
        selectValue = "SNA";
        paintSnap();
    } else if (selectValue == "SNA") {
        selectValue = "CUMSTON SNA";
        paintSnapCumston();
    } else if (selectValue == "CUMSTON SNA") {
        selectValue = "RAPID";
        selectNormalLoad();
        paintTapeRapid();
    } else if (selectValue == "RAPID") {
        selectValue = "CASSETTE";
        
        paintTapeRapid();
        paintTape();
    }

    Serial.println("--- " + selectValue + " SELECTED");
}


void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
        case PRESSED:
            switch (key) {
                case '1': // SELECT
                    select();
                    break;
                case '2': // REC
                    keyCapActive = "REC";
                    Serial.println("REC PRESSED");
                    sendKeyBoardAction(PS2dev::F4, true);
                    break;
                case '3': // PLAY
                    keyCapActive = "PLAY";
                    Serial.println("PLAY PRESSED");
                    sendKeyBoardAction(PS2dev::F6, true);
                    break;
                case '4': // REW
                    keyCapActive = "REW";
                    Serial.println("REW PRESSED");
                    sendSpecialKeyBoardAction(PS2dev::SpecialScanCodes::DOWN_ARROW, true);  // Presionar UP_ARROW
                    break;
                case '5': // FF
                    keyCapActive = "FF";
                    Serial.println("FF PRESSED");
                    sendSpecialKeyBoardAction(PS2dev::SpecialScanCodes::UP_ARROW, true);  // Presionar UP_ARROW
                    break;
                case '6': // STOP/EJECT
                    // Acción para la tecla '6'
                    Serial.println("Acción para la tecla 6");
                    break;
                case '7': // PAUSE
                    Serial.println("PAUSE PRESSED");
                    keyCapActive = "PAUSE";
                    keyboard.write(0xE1);
                    delay(15);
                    keyboard.write(0x14);
                    delay(15);
                    keyboard.write(0x77);
                    delay(15);
                    keyboard.write(0xE1);
                    delay(15);
                    keyboard.write(0xF0);
                    delay(15);
                    keyboard.write(0x14);
                    delay(15);
                    keyboard.write(0xF0);
                    delay(15);
                    keyboard.write(0x77);
                    delay(15);
                    break;
                case '8': // NADA DE NADA
                    //F1          
                    break;
            }

            break;
        case RELEASED:
            switch (key) {
                case '1': // REC
                    break;
                case '2': // REC
                    Serial.println("REC RELEASED");
                    sendKeyBoardAction(PS2dev::F4, false);
                    keyCapActive = "NADA";
                    break;
                case '3': // PLAY
                    keyCapActive = "PLAY";
                    Serial.println("PLAY RELEASED");
                    sendKeyBoardAction(PS2dev::F6, false);
                    break;
                case '4': // REW
                    keyCapActive = "NADA";
                    Serial.println("REW RELEASED");
                    sendSpecialKeyBoardAction(PS2dev::SpecialScanCodes::DOWN_ARROW, false);  // Presionar UP_ARROW
                    break;
                case '5': // FF
                    keyCapActive = "FF";
                    Serial.println("FF RELEASED");
                    sendSpecialKeyBoardAction(PS2dev::SpecialScanCodes::UP_ARROW, false);  // Presionar UP_ARROW
                    break;
                case '6': // STOP/EJECT
                    // Acción para la tecla '6'
                    Serial.println("Acción para la tecla 6");
                    break;
                case '7': // PAUSE
                    Serial.println("PAUSE RELEASED");
                    break;
                case '8': // NADA DE NADA
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
    selectValue = "CASSETTE";
    myOLED.begin(SSD1306_128X64);
    myOLED.setBrightness(255);
    myOLED.setFont(SmallFont);
    myOLED.clrScr();
    myOLED.drawRoundRect(0,63,127,16);
    paintTape();
    paintJoystick();
    // myOLED.drawBitmap(10, 30, snapshot32, 32, 29);
    // myOLED.drawBitmap(94, 30, mouse32, 24, 24);
    // myOLED.drawBitmap(51, 28, clearImage, 24, 24);
    // myOLED.print("SNAP", 11, 22);
    // myOLED.print("MOUSE", 86, 22);
    // myOLED.print("C", 23, 52);
    // myOLED.drawRoundRect(0,63,127,16);
    // myOLED.drawRoundRect(40,63,64,6);

    // myOLED.update();
    keypad.addEventListener(keypadEvent);
}

void loop()
{
    char key = keypad.getKey();
    // char cassetteSwich = cassette.getKey();
    currentState = controller.getState();
    gamePadState();


}
