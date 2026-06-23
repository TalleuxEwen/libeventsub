//
// Created by Ewen TALLEUX on 10/11/2025.
//

#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct data_ {
    char *event_name;
} Data;

typedef struct event_linked_list {
    Data *data;
    struct event_linked_list *next;
    struct event_linked_list *previous;
} EventLinkedList;

typedef struct subscription_list {
    void *(*function)(void *);
    EventLinkedList *event;
    struct subscription_list *next;
    struct subscription_list *previous;
} SubscriptionList;

typedef struct event_subscriptions {
    EventLinkedList *event_list;
    SubscriptionList *subscription_list;
} EventSubscription;

/** Initialize the shared event bus. Must be called once before any other function. Returns 0 on success. */
int init_event_subscriptions(void);

/** Register a named event. Returns 0 on success, 1 if already registered or not initialized. */
int register_event(const char *event_name);

/** Unregister a named event. Returns 0 on success, 1 if not found. */
int unregister_event(const char *event_name);

/** Subscribe a callback to a named event. Returns 0 on success. */
int subscribe_to_event(const char *event_name, void *(*function)(void *));

/** Unsubscribe a specific callback from a named event. Returns 0 on success. */
int unsubscribe_from_event(const char *event_name, void *(*function)(void *));

/**
 * Dispatch an event to all matching subscribers.
 * @param getValue  If true, returns the value of the first matching callback; otherwise returns NULL.
 */
void *callEvent(const char *event_name, void *args, bool getValue);

/** Free all subscriptions and events, and release the bus. Returns 0 on success. */
int free_event_subscriptions(void);

#ifdef __cplusplus
}
#endif
