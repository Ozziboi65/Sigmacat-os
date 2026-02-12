#pragma once
#include <stdint.h>

// ===== Input Type Identifiers =====
enum InputType {
    INPUT_NONE     = 0,
    INPUT_KEYBOARD = 1,
    INPUT_MOUSE    = 2
};

// ===== Device-Specific Input Data =====

struct KeyboardInput {
    const char* key;    // Mapped key string (e.g. "a", "!ENTER!")
};

struct MouseInput {
    int16_t X_delta;     // Relative X movement
    int16_t Y_delta;     // Relative Y movement
    uint8_t LeftClick;
    uint8_t RightClick;
    uint8_t MiddleClick;
};

// ===== Unified Input Report =====

struct InputReport {
    InputType type;
    union {
        struct KeyboardInput keyboard;
        struct MouseInput    mouse;
    } data;
};

// ===== Event Queue =====

#define INPUT_QUEUE_SIZE 32

void PushInputEvent(const InputReport& event);
bool PopInputEvent(InputReport* out);

// ===== Input Source Registration (for polled drivers) =====

typedef bool (*InputPollFunc)(InputReport* out);

struct InputSource {
    const char* name;
    InputType type;
    InputPollFunc poll;
};

#define MAX_INPUT_SOURCES 8

void RegisterInputSource(const char* name, InputType type, InputPollFunc poll);
void PollAllInputSources();

