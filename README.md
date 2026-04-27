# libeventsub

A lightweight, header-only C11 event subscription library. Register events, subscribe callbacks, and dispatch them with arbitrary data — all with zero dependencies.

## Features

- Header-only: just include `libeventsub.h`
- Register/unregister named events
- Subscribe/unsubscribe typed callbacks (`void (*)(void *)`)
- Dispatch events with arbitrary payload
- No external dependencies

## Integration via CMake FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
    libeventsub
    GIT_REPOSITORY https://github.com/ewentalleux/libeventsub.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(libeventsub)

target_link_libraries(your_target PRIVATE libeventsub)
```

Then include the header:

```c
#include <libeventsub.h>
```

## API

| Function | Description |
|---|---|
| `init_event_subscriptions()` | Initialize the library (call once before anything else) |
| `register_event(name)` | Register a new named event |
| `unregister_event(name)` | Unregister an event and remove it |
| `subscribe_to_event(name, callback)` | Subscribe a callback to an event |
| `unsubscribe_from_event(name, callback)` | Unsubscribe a specific callback |
| `callEvent(name, data)` | Dispatch an event, invoking all subscribers with `data` |
| `free_event_subscriptions()` | Free all resources |

All functions return `0` on success, `1` on error (except `callEvent` and `free_*`).

## Example

```c
#include <stdio.h>
#include <libeventsub.h>

void on_message(void *data) {
    printf("Received: %s\n", (char *)data);
}

int main(void) {
    init_event_subscriptions();

    register_event("message");
    subscribe_to_event("message", on_message);

    callEvent("message", "hello world");

    free_event_subscriptions();
    return 0;
}
```

## Requirements

- C11 compiler (gcc, clang)
- CMake 3.14+ (for FetchContent support)
