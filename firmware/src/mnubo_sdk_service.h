#ifndef _MNUBO_SDK_SERVICE_H    /* Guard against multiple inclusion */
#define _MNUBO_SDK_SERVICE_H

#include "mnubo_sdk_beans.h"
#include "system_config.h"
#include "utils.h"


char* eventToJson (Event* event);
char* objectToJson (Object* object, bool isShort);
char* postObject(char* request, char* hostPort, char* authorization, Object* object);
char* postEvent(char* request, char* hostPort, char* authorization, Event* event);

#endif /* MNUBO_SDK_SERVICE_H */