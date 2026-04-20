//
// Created by Ewen TALLEUX on 10/11/2025.
//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

typedef struct data_ {
    char *event_name;
}Data;

typedef struct event_linked_list {
    Data *data;
    struct event_linked_list *next;
    struct event_linked_list *previous;
}EventLinkedList;

typedef struct subscription_list {
    void (*function)(void *);
    EventLinkedList *event;
    struct subscription_list *next;
    struct subscription_list *previous;
}SubscriptionList;


typedef struct event_subscriptions {
    EventLinkedList *event_list;
    SubscriptionList *subscription_list;
} EventSubscription;

static EventSubscription *event_subscriptions ;

static inline int init_event_subscriptions() {
    event_subscriptions = (EventSubscription *) malloc(sizeof(EventSubscription));

    if (event_subscriptions == NULL) {
        perror("Failed to allocate memory for event subscriptions");
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return 1;
    }

    event_subscriptions->event_list = NULL;
    event_subscriptions->subscription_list = NULL;
    fprintf(stdout, "Event subscriptions initialized\n");
    return 0;
}

static inline SubscriptionList *createNewSubscription(void (*function)(void *), EventLinkedList *event) {
    SubscriptionList *new_subscription = (SubscriptionList *) malloc(sizeof(SubscriptionList));

    if (new_subscription == NULL) {
        perror("Failed to allocate memory for new subscription");
        return NULL;
    }

    new_subscription->function = function;
    new_subscription->event = event;

    return new_subscription;
}


static inline int subscribe_to_event(const char *event_name, void (*function)(void *)) {
    if (event_subscriptions == NULL) {
        printf("Event subscriptions not initialized\n");
        return 1;
    }

    //check if the event is registered
    EventLinkedList *event_registered = NULL;
    if (event_subscriptions->event_list == NULL) {
        printf("No event registered\n");
        return 1;
    } else {
        EventLinkedList *current = event_subscriptions->event_list;

        do {
            if (strcmp(event_name, current->data->event_name) == 0) {
                event_registered = current;
                break;
            }
        } while (current = current->next, current != event_subscriptions->event_list);
    }

    if (event_registered == NULL) {
        printf("Event '%s' not registered\n", event_name);
        return 1;
    }

    SubscriptionList *new_subscription = createNewSubscription(function, event_registered);

    if (new_subscription == NULL) {
        return 1;
    }

    if (event_subscriptions->subscription_list == NULL) {
        new_subscription->next = new_subscription;
        new_subscription->previous = new_subscription;
        event_subscriptions->subscription_list = new_subscription;
    } else {
        new_subscription->next = event_subscriptions->subscription_list;
        new_subscription->previous = event_subscriptions->subscription_list->previous;
        event_subscriptions->subscription_list->previous->next = new_subscription;
        event_subscriptions->subscription_list->previous = new_subscription;
    }
    fprintf(stdout, "Subscribed to event '%s' successfully\n", event_name);
    return 0;
}

static inline void free_subscription(SubscriptionList *subscription) {
    free(subscription);
}

static inline int unsubscribe_from_event(const char *event_name, void (*function)(void *)) {
    if (event_subscriptions == NULL) {
        printf("Event subscriptions not initialized\n");
        return 1;
    }

    SubscriptionList *subscription_to_remove = NULL;
    SubscriptionList *current = event_subscriptions->subscription_list;

    do {
        if (strcmp(event_name, current->event->data->event_name) == 0) {
            if (current->function == function) {
                if (event_subscriptions->subscription_list == current)
                {
                    if (current->next == current) {
                        event_subscriptions->subscription_list = NULL;
                    } else {
                        event_subscriptions->subscription_list = current->next;
                    }
                }
                subscription_to_remove = current;
                break;
            }
        }
    } while (current = current->next, current != event_subscriptions->subscription_list);

    if (subscription_to_remove == NULL) {
        printf("No subscription found for event '%s'\n", event_name);
        return 1;
    }
    subscription_to_remove->previous->next = subscription_to_remove->next;
    subscription_to_remove->next->previous = subscription_to_remove->previous;

    free_subscription(subscription_to_remove);

    printf("Unsubscribed from event '%s' successfully\n", event_name);
    return 0;
}

static inline EventLinkedList *createNewEvent(const char *event_name) {
    EventLinkedList *new_event = (EventLinkedList *) malloc(sizeof(EventLinkedList));

    if (new_event == NULL) {
        perror("Failed to allocate memory for new event");
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return NULL;
    }

    new_event->data = (Data *) malloc(sizeof(Data));

    if (new_event->data == NULL) {
        perror("Failed to allocate memory for event data");
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return NULL;
    }

    new_event->data->event_name = strdup(event_name);

    if (new_event->data->event_name == NULL) {
        perror("Failed to allocate memory for event name");
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return NULL;
    }

    return new_event;
}

static inline int register_event(const char *event_name) {
    EventSubscription *es = event_subscriptions;
    if (event_subscriptions == NULL) {
        printf("Event subscriptions not initialized\n");
        return 1;
    }

    if (event_subscriptions->event_list == NULL) {
        EventLinkedList *new_event = createNewEvent(event_name);
        if (new_event == NULL) {
            return 1;
        }

        new_event->next = new_event;
        new_event->previous = new_event;

        event_subscriptions->event_list = new_event;

    } else {
        //if only one event is registered
        if (event_subscriptions->event_list->next == event_subscriptions->event_list) {
            if (strcmp(event_name, event_subscriptions->event_list->data->event_name) == 0) {
                fprintf(stdout, "Event already registered\n");
                return 1;
            }
        } else {
            //if multiple events are registered
            EventLinkedList *current = event_subscriptions->event_list;

            do {
                if (strcmp(event_name, current->data->event_name) == 0) {
                    printf("Event already registered\n");
                    return 1;
                }
            } while (current = current->next, current != event_subscriptions->event_list);
        }

        //register the new event
        EventLinkedList *new_event = createNewEvent(event_name);

        if (new_event == NULL) {
            return 1;
        }

        new_event->next = event_subscriptions->event_list;
        new_event->previous = event_subscriptions->event_list->previous;
        event_subscriptions->event_list->previous->next = new_event;
        event_subscriptions->event_list->previous = new_event;
    }

    fprintf(stdout, "Event '%s' registered successfully\n", event_name);
    return 0;
}

static inline void free_event_registered(EventLinkedList *event) {
    free(event->data->event_name);
    free(event->data);
    free(event);
}

static inline int unregister_event(const char *event_name) {
    if (event_subscriptions == NULL) {
        printf("Event subscriptions not initialized\n");
        return 1;
    }

    if (event_subscriptions->event_list == NULL) {
        printf("No event registered\n");
        return 1;
    }

    EventLinkedList *current = event_subscriptions->event_list;
    do {
        if (strcmp(event_name, current->data->event_name) == 0) {
            current->previous->next = current->next;
            current->next->previous = current->previous;

            if (event_subscriptions->event_list == current) {
                if (current->next == current) {
                    event_subscriptions->event_list = NULL;
                } else {
                    event_subscriptions->event_list = current->next;
                }
            }
            free_event_registered(current);
            printf("Event '%s' unregistered successfully\n", event_name);
            return 0;
        }
    } while (current = current->next, current != event_subscriptions->event_list);

    printf("Event '%s' not registered\n", event_name);
    return 1;
}

static inline int free_event_subscriptions() {
    if (event_subscriptions == NULL) {
        printf("Event subscriptions not initialized\n");
        return 1;
    }

    EventSubscription *event_subscriptions_to_free = event_subscriptions;

    //Free all subscriptions
    if (event_subscriptions_to_free->subscription_list != NULL) {
        SubscriptionList *current_subscription = event_subscriptions_to_free->subscription_list;

        do {
            SubscriptionList* next_subscription = current_subscription->next;
            free_subscription(current_subscription);
            current_subscription = next_subscription;
        } while (current_subscription != event_subscriptions_to_free->subscription_list);

        event_subscriptions_to_free->subscription_list = NULL;
    } else {
        printf("No subscriptions to free\n");
    }

    //Free all events
    if (event_subscriptions_to_free->event_list != NULL) {
        EventLinkedList *current_event = event_subscriptions_to_free->event_list;

        do {
            EventLinkedList* next_event = current_event->next;
            free_event_registered(current_event);
            current_event = next_event;
        } while (current_event != event_subscriptions_to_free->event_list);
    } else {
        printf("No events to free\n");
    }

    return 0;
}

static inline void callEvent(const char *event_name, void *args) {
    if (event_subscriptions == NULL) {
        printf("Event subscriptions not initialized\n");
        return;
    }

    if (event_subscriptions->subscription_list == NULL) {
        printf("No subscriptions available\n");
        return;
    }

    SubscriptionList *current = event_subscriptions->subscription_list;

    do {
        if (strcmp(event_name, current->event->data->event_name) == 0) {
            current->function(args);
        }
    } while (current = current->next, current != event_subscriptions->subscription_list);
}
