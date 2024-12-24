#include <Arduino.h>
#include <GamePad.h>
#include <ps2dev.h> 
#include <config.hpp>
#include <Keypad.h>
#include <OLED_I2C.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

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
extern uint8_t SmallFont[]; //Tipo de letra

//LCD
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// Definir el teclado de la cinta
const byte filas = 2; 
const byte columnas = 4;
byte pinesFilas[]  = {10,16};
byte pinesColumnas[] = {A0,A1,A2,A3};
char teclas[2][4] = {{'1','2','3','4'},
                     {'5','6','7','8'}};

Keypad cassette = Keypad( makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas); 

// Crear una instancia de PS2dev para el teclado PS2
PS2dev keyboard(PS2_CLK_PIN, PS2_DATA_PIN);

String selectValue = "CASSETTE";
String Cursors = "UP-DOWN";
String keyCapActive = "SELECT";
String HardwareDisplay = "OLED"; //OLED O LCD


byte esquina_izquierda[] = {
  B00111,
  B01000,
  B10010,
  B10000,
  B10001,
  B10001,
  B10001,
  B10001
};

byte linea[] = {
  B11111,
  B00000,
  B00000,
  B00000,
  B11111,
  B00000,
  B00000,
  B00000
};

byte vertical[] = {
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001
};

byte topLeft[] = {
  B00000,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte topRight[] = {
  B00000,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte bottomLeft[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000
};

byte bottomRight[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000
};

byte middle[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

// Controller states Joystick
word currentState = 0;
word lastState = 0;

void displayTextOLED(const String& text) {
    myOLED.clrScr();
    myOLED.print(text, 65, 8);
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

void Rec() {
    keyCapActive = "REC";
    sendKeyBoardAction(PS2dev::F4, true);
}   

void Insert() {
    keyCapActive = "INSERT";
    sendKeyBoardAction(PS2dev::LEFT_SHIFT, true);
    delay(100);
    sendKeyBoardAction(PS2dev::F5, true);    
}

void Play() {
    keyCapActive = "PLAY";
    sendKeyBoardAction(PS2dev::F6, true);
}
void Pause() {
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
}

void Rew() {
    keyCapActive = "REW";
    sendSpecialKeyBoardAction(PS2dev::SpecialScanCodes::DOWN_ARROW, true);  // Presionar UP_ARROW
}

void FF() {
    keyCapActive = "FF";
    sendSpecialKeyBoardAction(PS2dev::SpecialScanCodes::UP_ARROW, true);  // Presionar UP_ARROW
}

void select() {
    if (selectValue == "CASSETTE") {
        selectValue = "SNA";

    } else if (selectValue == "SNA") {
        selectValue = "CUMSTON SNA";
    } else if (selectValue == "CUMSTON SNA") {
        selectValue = "RAPIDA";
    } else if (selectValue == "RAPIDA") {
        selectValue = "CASSETTE";
    }

    Serial.println("--- " + selectValue + " SELECTED");

    if (HardwareDisplay == "OLED") {
        displayTextOLED(selectValue);
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
    if (HardwareDisplay == "OLED") {
        myOLED.begin(SSD1306_128X32);
        myOLED.setBrightness(255);
        myOLED.rotateDisplay(true);
        myOLED.setFont(SmallFont);
        displayTextOLED("  SNAPSHOT");
    }
    if (HardwareDisplay == "LCD") {
        lcd.init();                      // initialize the lcd 
        lcd.backlight();
        // lcd.print("SNAPSHOT");
        lcd.createChar (0,esquina_izquierda);
        lcd.createChar (1,linea);
        lcd.createChar (2,vertical);
        lcd.setCursor(0, 0);
        lcd.write (byte (0));
        lcd.setCursor(1, 0);
        lcd.write (byte (1));
        lcd.setCursor(2, 0);
        lcd.write (byte (1));
        lcd.setCursor(3, 0);
        lcd.write (byte (1));
        lcd.setCursor(4, 0);
        lcd.write (byte (1));
        lcd.setCursor(5, 0);
        lcd.write (byte (1));
        lcd.setCursor(6, 0);
        lcd.write (byte (1));
        lcd.setCursor(7, 0);
        lcd.write (byte (1));
        lcd.setCursor(8, 0);
        lcd.write (byte (1));
        lcd.setCursor(9, 0);
        lcd.write (byte (1));
        lcd.setCursor(10, 0);
        lcd.write (byte (1));
        lcd.setCursor(11, 0);
        lcd.write (byte (1));
        lcd.setCursor(0, 2);
        lcd.write (byte (2));
        lcd.setCursor(0, 1);
        lcd.write (byte (2));
        lcd.setCursor(0, 3);
        lcd.write (byte (2));
    lcd.createChar(3, topLeft); // Cargar el carácter personalizado en la posición 3
    lcd.createChar(4, topRight); // Cargar el carácter personalizado en la posición 4
    lcd.createChar(5, bottomLeft); // Cargar el carácter personalizado en la posición 5
    lcd.createChar(6, bottomRight); // Cargar el carácter personalizado en la posición 6
    lcd.createChar(7, middle); 
    lcd.setCursor(2, 2);
    lcd.write(byte(3)); // topLeft
    lcd.write(byte(7)); // middle
    lcd.write(byte(4)); // topRight

    // Dibujar la parte inferior del círculo
    lcd.setCursor(2, 3);
    lcd.write(byte(5)); // bottomLeft
    lcd.write(byte(7)); // middle
    lcd.write(byte(6)); // bottomRight
    }
}

void loop()
{

    char cassetteSwich = cassette.getKey();
    currentState = controller.getState();
    gamePadState();

    switch (cassetteSwich) {
        case '1': // SELECT
            select();
            break;
        case '2': // REC
            Rec();
            break;
        case '3': // PLAY
            Play();
            break;
        case '4': // REW
            Rew();
            break;
        case '5': // FF
            FF();
            break;
        case '6': // STOP/EJECT
            // Acción para la tecla '6'
            Serial.println("Acción para la tecla 6");
            break;
        case '7': // PAUSE
            Pause();
            break;
        case '8': // NADA DE NADA
            break;
    }
}
