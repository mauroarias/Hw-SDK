#ifndef MNUBO_SDK_SERVICE_H    /* Guard against multiple inclusion */
#define MNUBO_SDK_SERVICE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

#define EVENT_ID          "event_id"
#define OBJECT            "x_object"
#define EVENT_TYPE        "x_event_type"
#define TIMESTAMP         "x_timestamp"
#define DEVICE_ID         "x_device_id"
#define OBJECT_TYPE       "x_object_type"
#define REGISTRATION_DATE "x_registration_date"
#define OWNER             "x_owner"
#define PASSWORD          "x_password"
#define USERNAME          "username"

#define MAX_KEY_SIZE      64
#define MAX_TYPE_SIZE     64
#define MAX_VALUE_SIZE    1024

//-----------------------------
// Data structure definitions
//-----------------------------
typedef unsigned char uuid_t[16];

typedef struct KeyValue_t {
    char * key;
    void * value;
    char * type;
} KeyValue;

typedef struct Owner_t {
    char * username;
    char * password;
    unsigned long registrationDate;
    KeyValue * attributes;
    uuid_t eventId;
} Owner;

typedef struct Object_t {
    char * deviceId;
    uuid_t objectId;
    char * objectType;
    unsigned long registrationDate;
    Owner * owner;
    KeyValue * attributes;
    uuid_t eventId;   
} Object;

typedef struct Event_t {
    uuid_t eventId;
    Object * object;
    char * eventType;
    unsigned long timestamp;
    KeyValue * timeseries;
    unsigned int timeseriesSize;
} Event;

#endif /* MNUBO_SDK_SERVICE_H */