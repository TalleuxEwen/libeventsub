//
// Created by Ewen TALLEUX on 10/11/2025.
//

#pragma once

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
    void (*function)(void *);
    EventLinkedList *event;
    struct subscription_list *next;
    struct subscription_list *previous;
} SubscriptionList;

typedef struct event_subscriptions {
    EventLinkedList *event_list;
    SubscriptionList *subscription_list;
} EventSubscription;

int init_event_subscriptions(void);
int register_event(const char *event_name);
int unregister_event(const char *event_name);
int subscribe_to_event(const char *event_name, void (*function)(void *));
int unsubscribe_from_event(const char *event_name, void (*function)(void *));
void callEvent(const char *event_name, void *args);
int free_event_subscriptions(void);

#ifdef __cplusplus
}
#endif
