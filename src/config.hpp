#define PS2_CLK_PIN 1
#define PS2_DATA_PIN 0

enum INTERFACE {
    TAPE,
    SNAPSHOT,
    CUSMTON_SNAPSHOT
};

enum DEVICE {
    JOYSTICK,
    MOUSE
};

enum ACTION {
    REC,
    PLAY,
    REW,
    STOP,
    EJECT,
    FF,
    PAUSE,
    CLEAR
};

extern INTERFACE currentInterface;
extern DEVICE currentDevice;
extern ACTION currentAction;

extern uint8_t SmallFont[];
extern uint8_t disk32[];
extern uint8_t mouse32[];
extern uint8_t snapshot32[];
extern uint8_t cassette32[];
extern uint8_t joystick32[];
extern uint8_t play[];
extern uint8_t rew[];
extern uint8_t stop[];
extern uint8_t ff[];
extern uint8_t pause[];
extern uint8_t up[];
extern uint8_t down[];
extern uint8_t clearImage[];
extern uint8_t border24[];
extern uint8_t border32[];
