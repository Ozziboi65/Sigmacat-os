#include "KernelInputHandler.h"

// ===== Event Ring Buffer =====

static InputReport eventQueue[INPUT_QUEUE_SIZE];
static int queueHead = 0;  // next read position
static int queueTail = 0;  // next write position

void PushInputEvent(const InputReport& event) {
    int nextTail = (queueTail + 1) % INPUT_QUEUE_SIZE;
    if (nextTail == queueHead) return;  // queue full, drop event
    eventQueue[queueTail] = event;
    queueTail = nextTail;
}

bool PopInputEvent(InputReport* out) {
    if (queueHead == queueTail) return false;  // empty
    *out = eventQueue[queueHead];
    queueHead = (queueHead + 1) % INPUT_QUEUE_SIZE;
    return true;
}

// ===== Input Source Registration (for polled drivers) =====

static InputSource inputSources[MAX_INPUT_SOURCES];
static int inputSourceCount = 0;

void RegisterInputSource(const char* name, InputType type, InputPollFunc poll) {
    if (inputSourceCount >= MAX_INPUT_SOURCES) return;
    inputSources[inputSourceCount].name = name;
    inputSources[inputSourceCount].type = type;
    inputSources[inputSourceCount].poll = poll;
    inputSourceCount++;
}

void PollAllInputSources() {
    for (int i = 0; i < inputSourceCount; i++) {
        InputReport report = {INPUT_NONE};
        if (inputSources[i].poll(&report)) {
            PushInputEvent(report);
        }
    }
}