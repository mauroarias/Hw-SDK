/* ************************************************************************** */
/** Descriptive File Name
  @Company
    Company Name
  @File Name
    filename.c
  @Summary
    Brief description of the file.
  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */
#include "mnubo_sdk_service.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
Event * buildEvent() {
    return (Event*)malloc(sizeof(Event));
}
Event * withEventId(Event * event, uuid_t eventId) {
    memcpy ((void*)&(event->eventId),(const void*)eventId,sizeof(uuid_t));
    return event;
}
void intToString(char * output, 
                 const unsigned long number) {
    sprintf(output, "%lu", number);
}
void floatToString(char * output, 
                   const float number) {
    sprintf(output, "%.2f", number);
}
void uuidToString(char * output, 
                  const uuid_t uuid) {
	if (uuid != NULL) {
        sprintf(output, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x", 
                uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
                uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);
    }
}
void line2String(char * input, 
                 const char * name, 
                 const char * value,
                 bool isString) {
    if (name != NULL) {
        strcat (input, "\"");
        strcat (input, name);
        strcat (input, "\"");
        strcat (input, ":");
        if (isString) strcat (input, "\"");
        strcat (input, ((value != NULL) ? value : ""));
        if (isString) strcat (input, "\"");
    }
}
void appendObject (char * output, 
                   Object * object) {
    char device[MAX_VALUE_SIZE];
    if (object != NULL) {
        strcat (device, OBJECT);
        strcat (device, "." );
        strcat (device, DEVICE_ID);
        line2String (output, device, object->deviceId, true);
    }
}
void appendEventId (char * output, 
                    Event * event) {
    char eventId[MAX_VALUE_SIZE];
    if (event != NULL) {
        uuidToString (eventId, event->eventId);
        line2String (output, EVENT_ID, eventId, true);
    }
}
void appendEventType (char * output, 
                      Event * event) {
    
    if (event != NULL) {
        line2String (output, EVENT_TYPE, event->eventType, true);
    }
}
void appendTimestamp (char * output,
                      Event * event) {
    char ts[MAX_VALUE_SIZE];                          
    if (event != NULL) {
        intToString (ts, event->timestamp);
        line2String (output, TIMESTAMP, ts, false);
    }
}
void appendTimeSeries (char * output,
                       Event * event) {
    int i=0;  
    while (i<event->timeseriesSize) {
    //for (int i=0; i<event->timeseriesSize; i++) {
        char value[MAX_VALUE_SIZE];
        if (strcmp(event->timeseries[i].type, "float") == 0) {
            floatToString (value, *((float *)(event->timeseries[i].value)));
            line2String (output, event->timeseries[i].key, value, false);       
        }
        else if (strcmp(event->timeseries[i].type, "int") == 0) {
            intToString (value, *((int *)(event->timeseries[i].value)));
            line2String (output, event->timeseries[i].key, value, false);       
        }
        else if (strcmp(event->timeseries[i].type, "uuid") == 0) {
            uuidToString (value, *((uuid_t *)(event->timeseries[i].value)));
            line2String (output, event->timeseries[i].key, value, true);
        }
        else {
            line2String (output, event->timeseries[i].key, event->timeseries[i].value, true);
        }
        if (i < (event->timeseriesSize-1)) strcat (output, ",");
        i++;
    }
}
                       
char * toJson (Event * event) {
    char * json = (char*)malloc(4096*sizeof(char));    
    char * current = json;
    strcat (current, "{");
    
    // Append object & deviceid
    appendObject (json, event->object);
    strcat (json, ",");
    	
    // Append eventid
    appendEventId (json, event);
    strcat (json, ",");
    
    // Append eventtype
    appendEventType (json, event);
    strcat (json, ",");
    
    // Append timestamp
    appendTimestamp (json, event);
    strcat (json, ",");
    
    // Append timeseries
    appendTimeSeries (json, event);
    
    strcat (current, "}\n");
    return json;
}
    
////-----------------------------
//// Main function
////-----------------------------
//int main() {
//    Event * event = malloc(sizeof(Event));
//    Object * object = malloc(sizeof(Object));
//    
//    object->deviceId = "TestDevice";
//    event->object = object;
//    event->eventType = "TestEventType";
//    event->timestamp = 1234567890;
//    strncpy ((char *)event->eventId, "1234567890ABCDEF", 16);
//    
//    event->timeseries = malloc(sizeof(KeyValue)*2);
//    event->timeseriesSize = 2;
//    event->timeseries[0].type  = malloc(sizeof(char)*MAX_TYPE_SIZE);
//    event->timeseries[1].type  = malloc(sizeof(char)*MAX_TYPE_SIZE);
//    event->timeseries[0].key   = malloc(sizeof(char)*MAX_KEY_SIZE);
//    event->timeseries[1].key   = malloc(sizeof(char)*MAX_KEY_SIZE);
//    event->timeseries[0].value = malloc(sizeof(char)*MAX_VALUE_SIZE);
//    event->timeseries[1].value = malloc(sizeof(char)*MAX_VALUE_SIZE);
//    
//    strcpy(event->timeseries[0].type, "float");
//    strcpy(event->timeseries[0].key, "temparature");
//    float value = 21.50;
//    event->timeseries[0].value = (void *)&value;
//    
//    strcpy(event->timeseries[1].type, "string");
//    strcpy(event->timeseries[1].key, "status");
//    strcpy(event->timeseries[1].value, "open");
//    char * json = toJson (event);
//    printf ("%s\n", json);
//    free (json);
//    free (event->timeseries);
//}