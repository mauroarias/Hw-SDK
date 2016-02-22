#ifndef _MNUBO_SDK_BEANS_H    /* Guard against multiple inclusion */
#define _MNUBO_SDK_BEANS_H

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

typedef struct {
    char * key;
    void * value;
    char * type;
} KeyValue;

typedef struct {
    char * username;
    char * password;
    unsigned long registrationDate;
    KeyValue * attributes;
    uuid_t eventId;
} Owner;

typedef struct {
    char * objectType;
    char * deviceId;
    uuid_t objectId;
    unsigned long registrationDate;
    Owner * owner;
    KeyValue * attributes;
    uuid_t eventId;   
} Object;

typedef struct {
    uuid_t eventId;
    Object * object;
    char * eventType;
    unsigned long timestamp;
    KeyValue * timeseries;
    unsigned int timeseriesSize;
} Event;

typedef enum {
    OBJECTS=0,
    EVENTS=1,
    OWNERS=2,
} BodyType;

#endif /* _EXAMPLE_FILE_NAME_H */
