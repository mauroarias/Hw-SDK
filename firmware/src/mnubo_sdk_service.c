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
                 int isString) {
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
    while (i < event->timeseriesSize) {
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
                       
char * eventToJson (Event * event) {
    char * json = (char*)malloc(400*sizeof(char));    
    char * current = json;
    strcat (current, "[{");

    //Append object
    line2String (json, OBJECT, objectToJson(event->object, true), false);
    strcat (json, ",");

    //Append event_type
    line2String (json, EVENT_TYPE, event->eventType, true);
    strcat (json, ",");
    
    // Append timeseries
    appendTimeSeries (json, event);
    
    strcat (current, "}]\n");
    return json;
}

char * objectToJson (Object * object, bool isShort) {
    char * json = (char*)malloc(200*sizeof(char));
    char * current = json;
    strcat (current, "{");
    
    //Append deviceid
    line2String (json, DEVICE_ID, object->deviceId, true);
    
    if(!isShort)
    {    
        strcat (json, ",");
    	
        // Append device_type
        line2String (json, OBJECT_TYPE, object->objectType, true);
    }
    
    strcat (current, "}");
    return json;
}

char* postObject(char* request, char* hostPort, char* authorization, Object* object)
{
    char * body = objectToJson (object, false);

    char size[10];
    sprintf(size,"%d",strlen(body));
	strcat(request, "POST /rest/objects HTTP/1.1\r\n");
	strcat(request, "Host: ");
    strcat(request, hostPort);
    strcat(request, "\r\n");
	strcat(request, "Accept: */*\r\n");
    strcat(request, authorization);
    strcat(request, "\r\n");
    strcat(request, "Content-Type: application/json\r\n");
	strcat(request, "Connection: keep-alive\r\n");
	strcat(request, "Content-Length: ");
	strcat(request, size);
	strcat(request, "\r\n");
	strcat(request, "\r\n");
	strcat(request, body);
    return request;
}

char* postEvent(char* request, char* hostPort, char* authorization, Event* event)
{
    char * body = eventToJson (event);

    char size[10];
    sprintf(size,"%d",strlen(body));
	strcat(request, "POST /rest/events HTTP/1.1\r\n");
	strcat(request, "Host: ");
    strcat(request, hostPort);
    strcat(request, "\r\n");
	strcat(request, "Accept: */*\r\n");
    strcat(request, authorization);
    strcat(request, "\r\n");
    strcat(request, "Content-Type: application/json\r\n");
	strcat(request, "Connection: keep-alive\r\n");
	strcat(request, "Content-Length: ");
	strcat(request, size);
	strcat(request, "\r\n");
	strcat(request, "\r\n");
	strcat(request, body);
    return request;
}