#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "GamePadWrapper.h"
#include "stdio.h"
#include "GamePad.h"

// Controller DB9 pins (looking face-on to the end of the plug):
//
// 5 4 3 2 1
//  9 8 7 6

// DB9 Pin 1 --> Pico Pin 2
// DB9 Pin 2 --> Pico Pin 3
// DB9 Pin 3 --> Pico Pin 4
// DB9 Pin 4 --> Pico Pin 5
// DB9 Pin 5 --> Pico +5V
// DB9 Pin 6 --> Pico Pin 6
// DB9 Pin 7 --> Pico Pin 7 Select
// DB9 Pin 8 --> Pico GND
// DB9 Pin 9 --> Pico Pin 8

GamePad* controller;
uint16_t currentState;
uint16_t lastState;

void sendState()
{
    // Verificar el cambio de estado para cada botón
    if ((currentState & SC_BTN_UP) != (lastState & SC_BTN_UP)) {
        if (currentState & SC_BTN_UP) {
            printf("Botón UP presionado\n");
            //sendKeyAction(ESP_JOY1UP, true);
        } else {
            printf("Botón UP soltado\n");
            //sendKeyAction(ESP_JOY1UP, false);
        }
    }

    if ((currentState & SC_BTN_DOWN) != (lastState & SC_BTN_DOWN)) {
        if (currentState & SC_BTN_DOWN) {
            printf("Botón DOWN presionado\n");
        } else {
            //sendKeyAction(ESP_JOY1DOWN, false);
            printf("Botón DOWN soltado\n");
        }
    }

    if ((currentState & SC_BTN_LEFT) != (lastState & SC_BTN_LEFT)) {
        if (currentState & SC_BTN_LEFT) {
            //sendKeyAction(ESP_JOY1LEFT, true);
            printf("Botón LEFT presionado\n");
        } else {
          //sendKeyAction(ESP_JOY1LEFT, false);
            printf("Botón LEFT soltado\n");
        }
    }

    if ((currentState & SC_BTN_RIGHT) != (lastState & SC_BTN_RIGHT)) {
        if (currentState & SC_BTN_RIGHT) {
            //sendKeyAction(ESP_JOY1RIGHT, true);
            printf("Botón RIGHT presionado\n");
        } else {
            //sendKeyAction(ESP_JOY1RIGHT, false);
            printf("Botón RIGHT soltado\n");
        }
    }

    if ((currentState & SC_BTN_START) != (lastState & SC_BTN_START)) {
        if (currentState & SC_BTN_START) {
            //sendKeyAction(ESP_JOY1START, true);
            printf("Botón START presionado\n");
        } else {
            //sendKeyAction(ESP_JOY1START, false);
            printf("Botón START soltado\n");
        }
    }

    if ((currentState & SC_BTN_A) != (lastState & SC_BTN_A)) {
        if (currentState & SC_BTN_A) {
            //sendKeyAction(ESP_JOY1A, true);
            printf("Botón A presionado\n");
        } else {
            //sendKeyAction(ESP_JOY1A, false);
            printf("Botón A soltado\n");
        }
    }

    if ((currentState & SC_BTN_B) != (lastState & SC_BTN_B)) {
        if (currentState & SC_BTN_B) {
            //sendKeyAction(ESP_JOY1B, true);
            printf("Botón B presionado\n");
        } else {
            //sendKeyAction(ESP_JOY1B, false);
            printf("Botón B soltado\n");
        }
    }

    if ((currentState & SC_BTN_C) != (lastState & SC_BTN_C)) {
        if (currentState & SC_BTN_C) {
            //sendKeyAction(ESP_JOY1C, true);
            printf("Botón C presionado\n");
        } else {
            //sendKeyAction(ESP_JOY1C, false);
            printf("Botón C soltado\n");
        }
    }

    if ((currentState & SC_BTN_X) != (lastState & SC_BTN_X)) {
        if (currentState & SC_BTN_X) {
            //sendKeyAction(ESP_JOY1X, true);
            printf("Botón X presionado\n");
        } else {
            //sendKeyAction(ESP_JOY1X, false);
            printf("Botón X soltado\n");
        }
    }

    if ((currentState & SC_BTN_Y) != (lastState & SC_BTN_Y)) {
        if (currentState & SC_BTN_Y) {
            //sendKeyAction(ESP_JOY1Y, true);
            printf("Botón Y presionado\n");
        } else {
            //sendKeyAction(ESP_JOY1Y, false);
            printf("Botón Y soltado\n");
        }
    }

    if ((currentState & SC_BTN_Z) != (lastState & SC_BTN_Z)) {
        if (currentState & SC_BTN_Z) {
            //sendKeyAction(ESP_JOY1Z, true);
            printf("Botón Z presionado\n");
        } else {
            //sendKeyAction(ESP_JOY1Z, false);
            printf("Botón Z soltado\n");
        }
    }

    if ((currentState & SC_BTN_1) != (lastState & SC_BTN_1)) {
        if (currentState & SC_BTN_1) {
            //sendKeyAction(ESP_JOY1A, true);
            printf("Botón 1 presionado\n");
        } else {
            //sendKeyAction(ESP_JOY1A, false);
            printf("Botón 1 soltado\n");
        }
    }

    if ((currentState & SC_BTN_2) != (lastState & SC_BTN_2)) {
        if (currentState & SC_BTN_2) {
            //sendKeyAction(ESP_JOY1Z, true);
            printf("Botón 2 presionado\n");
        } else {
            //sendKeyAction(ESP_JOY1Z, false);
            printf("Botón 2 soltado\n");
        }
    }

    if ((currentState & SC_BTN_MODE) != (lastState & SC_BTN_MODE)) {
        if (currentState & SC_BTN_MODE) {
            //sendKeyAction(ESP_JOY1MODE, true);
            printf("Botón MODE presionado\n");
        } else {
            //sendKeyAction(ESP_JOY1MODE, false);
            printf("Botón MODE soltado\n");
        }
    }

    // Actualizar el último estado
    lastState = currentState;
}
#define LED_PIN 25
void setup() {
    printf("GamePad Wrapper\n");
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1); // Encender el LED
    stdio_init_all();
    controller = GamePad_create(7, 2, 3, 4, 5, 6, 8);

    // Usar el controlador
    currentState = GamePad_getState(controller);
    printf("Current state: %u\n", currentState);

    while (true) {
        currentState = GamePad_getState(controller);
        sendState(); // Llamar a sendState para verificar y enviar el estado
        // printf("GamePad Wrapper\n");
    }
}

int main() {
    setup();
}