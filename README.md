# libeventsub

A lightweight C11 event subscription library. Register events, subscribe callbacks, and dispatch them with arbitrary data — all with zero dependencies.

Built as a **shared library** (`.so`/`.dylib`), which means multiple independent libraries within the same process share a single event bus automatically.

## Features

- Register/unregister named events
- Subscribe/unsubscribe typed callbacks (`void (*)(void *)`)
- Dispatch events with arbitrary payload
- Shared event bus across all libraries in the same process
- No external dependencies

## Integration via CMake FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
    libeventsub
    GIT_REPOSITORY https://github.com/TalleuxEwen/libeventsub.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(libeventsub)

target_link_libraries(your_target PRIVATE libeventsub)
```

Then include the header:

```c
#include "libeventsub.h"
```

## Multi-library usage

The main benefit of building libeventsub as a shared library is that all libraries in the same process share the same event bus. The OS loads the `.so`/`.dylib` only once per process, so subscriptions from any library are visible to all others.

**In the top-level project, declare libeventsub before the libraries that depend on it:**

```cmake
# Top-level CMakeLists.txt — libeventsub FIRST
FetchContent_Declare(libeventsub ...)
FetchContent_MakeAvailable(libeventsub)

# Then the libs that use it
add_subdirectory(lib_audio)
add_subdirectory(lib_network)
```

Each sub-library can keep its own `FetchContent_Declare(libeventsub ...)` for standalone builds. When integrated into the parent project, CMake's deduplication kicks in and reuses the already-declared target.

**Example — two independent libraries sharing one bus:**

```c
// lib_network.c — owns the bus lifecycle and registers events
void lib_network_init(void) {
    if (init_event_subscriptions() != 0)
        return;
    register_event("on_connect");
}
void lib_network_on_connected(void) {
    callEvent("on_connect", &connection_info, false);  // triggers lib_audio's callback
}

// lib_audio.c — subscribes only, relies on bus already being initialized
void lib_audio_init(void) {
    subscribe_to_event("on_connect", audio_handle_connect);
}

// main.c
int main(void) {
    lib_network_init();  // initializes bus + registers events
    lib_audio_init();    // subscribes

    lib_network_on_connected();  // lib_audio receives the event
    free_event_subscriptions();
}
```

## API

| Function | Description |
|---|---|
| `init_event_subscriptions()` | Initialize the library — call once at startup |
| `register_event(name)` | Register a new named event |
| `unregister_event(name)` | Unregister an event |
| `subscribe_to_event(name, callback)` | Subscribe a callback to an event |
| `unsubscribe_from_event(name, callback)` | Unsubscribe a specific callback |
| `callEvent(name, data, getValue)` | Dispatch an event; if `getValue` is `true`, returns the value of the first matching callback |
| `free_event_subscriptions()` | Free all resources |

All functions return `0` on success, `1` on error (except `callEvent`).

## Basic example

```c
#include <stdio.h>
#include "libeventsub.h"

void on_message(void *data) {
    printf("Received: %s\n", (char *)data);
}

int main(void) {
    init_event_subscriptions();

    register_event("message");
    subscribe_to_event("message", on_message);

    callEvent("message", "hello world", false);

    free_event_subscriptions();
    return 0;
}
```

## Requirements

- C11 compiler (gcc, clang)
- CMake 3.14+
