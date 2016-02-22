/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    mnubo.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "restapi.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

RESTAPI_DATA restapiData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback funtions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void MNUBO_Initialize ( void )

  Remarks:
    See prototype in mnubo.h.
 */

void RESTAPI_Initialize ( void )
{
    restapiData.state = RESTAPI_STATE_INIT;
}

/******************************************************************************
  Function:
    void MNUBO_Tasks ( void )

  Remarks:
    See prototype in mnubo.h.
 */

void RESTAPI_Tasks ( void )
{
    restapiData.random++;
    /* Check the application's current state. */
    switch ( restapiData.state )
    {
        /* Application's initial state. */
        case RESTAPI_STATE_INIT:
        {
            restapiData.state = RESTAPI_STATE_WAITING_CREATE_OBJECT;
            restapiData.button1 = BSP_SwitchStateGet(BSP_SWITCH_1);
            restapiData.button2 = BSP_SwitchStateGet(BSP_SWITCH_2);
            restapiData.button3 = BSP_SwitchStateGet(BSP_SWITCH_3);
            restapiData.remotePort = 21029; 
            restapiData.remoteAddress = malloc(sizeof(IP_MULTI_ADDRESS));
            restapiData.mySocket = INVALID_SOCKET;
            restapiData.tick_second = 5;
            restapiData.remoteAddress->v4Add.Val = 0x6E00A8C0; //192.168.0.110
            restapiData.connectionRetry = 0;
            restapiData.bodyType=OBJECTS;

            Object * object = malloc(sizeof(Object));
            object->objectType = "111";
            
            Event * event = malloc(sizeof(Event));
            event->object = object;
            event->eventType = "222";
            
            event->timeseries = malloc(sizeof(KeyValue)*3);
            event->timeseriesSize = 3;
            event->timeseries[0].type  = "text";
            event->timeseries[1].type  = "text";
            event->timeseries[2].type  = "text";
            event->timeseries[0].key   = "switch1";
            event->timeseries[1].key   = "switch2";
            event->timeseries[2].key   = "switch3";
            event->timeseries[0].value = malloc(sizeof(char)*50);
            event->timeseries[1].value = malloc(sizeof(char)*50);
            event->timeseries[2].value = malloc(sizeof(char)*50);
            
            restapiData.event = event;
            restapiData.authorization = "Authorization: Bearer eyJjdHkiOiJKV1QiLCJlbmMiOiJBMTI4Q0JDLUhTMjU2IiwiYWxnIjoiZGlyIn0..wMioLbXiUNOYGlM83eCDTg.WQaN13VTLfKJmk8mMo6FEFSaVJCLkTbtl_aitFe5gt05v2uzdUnA9vQ4OSMzPkr0E3O9dwdP0NoxRF0V2zIDpHtYZZLblSNRxqPvarhXSyPObuW5gQ_OozLFdUrKqcqfwS1yuK9i5nk3j0KQKFoDI_YcfP3y8JzdJ-ddYRIZQ35sgqfMlB19fB3Mmnsu3-s31CSpXe-4mhcQ5W9JmyGh-_KAiQ8MeG7UDUhJqXkTpwNMC-iUQqF6wSaQgZVEeXuQnNwfZMvR_VqUkTF7QU0LLL3TAw_sdO_TIeZaEhgd98ecUjxezgJOjC1w3Zh_AFHxNZ_LGtasG79ZEd-ND8WS2UHLwUOR2xriZiOOIlVi5Hq3m5xcXTFOVzlMlMp5tQFEfRPlMV104mDjD_rlrpzDGPR6dS9k25_Vysunk30218j1RHbFlOLSoIh0zJl-tigsi-fHl1CfBrecdd7CBGsY1JddCcGqmtLHtYzZqOkRVtkpSwFVJE_pj4Cgowk2mi1qsl_VMwDiedDzz19EO1WdqvUggYtPI_8PweR59cMGNwv04CirlBIttL4efzo0IRa9SegWx1TVe2VPyokTz63xdX1WJiolqQrNVP2X-wJ3-YJhsSaE2apQ2ftAWUv34LNLQ2RDqcxjNTE5SZawI6XuzN96wK621ANoL-fj-71yYgOgmFB8DTQVqRqbBhZ6iN3R.1ITPVWmOWUF4xy6U9wTT5w";
            restapiData.hostPort = "192.168.0.110:21029";
            break;
        }
        case RESTAPI_STATE_WAITING_CREATE_OBJECT:
		{
            if(isSwitch1StateChanged() ||
               isSwitch2StateChanged() ||
               isSwitch3StateChanged())
            {
                char * deviceId = (char*)malloc(20*sizeof(char));
                generateDeviceId(deviceId, restapiData.random);
                restapiData.event->object->deviceId = deviceId;
            }
			break;
        }
        case RESTAPI_STATE_HOME:
            restapiData.mySocket = TCPIP_TCP_ClientOpen(IP_ADDRESS_TYPE_IPV4, 
                     restapiData.remotePort, restapiData.remoteAddress);
			
			if(restapiData.mySocket == INVALID_SOCKET)
				break;

			restapiData.state = RESTAPI_STATE_SOCKET_OBTAINED;
            restapiData.connectionTimeOut = getDelay(5);
			break;

		case RESTAPI_STATE_SOCKET_OBTAINED:
			// Wait for the remote server to accept our connection request
			if(!TCPIP_TCP_IsConnected(restapiData.mySocket))
			{
                if((int32_t)(restapiData.connectionTimeOut - SYS_TMR_TickCountGet()) < 0)
                {
					TCPIP_TCP_Disconnect(restapiData.mySocket);
					restapiData.mySocket = INVALID_SOCKET;
					restapiData.state = RESTAPI_STATE_HOME;
                    if(restapiData.connectionRetry++ == 3)
                    {
                        restapiData.state = RESTAPI_STATE_WAITING_FOR_EVENT;
                    }
				}
				break;
			}

			if(TCPIP_TCP_PutIsReady(restapiData.mySocket) < 125u)
            {
				break;
            }
            
            char * request = (char*)malloc(2000*sizeof(char));
            
            switch(restapiData.bodyType)
            {
                case OBJECTS:
                {
                    TCPIP_TCP_StringPut(
                            restapiData.mySocket, 
                            postObject(
                                request+2, 
                                restapiData.hostPort, 
                                restapiData.authorization, 
                                restapiData.event->object));
                    restapiData.bodyType=EVENTS;
                    break;
                }
                case EVENTS:
                {
                    TCPIP_TCP_StringPut(
                            restapiData.mySocket, 
                            postEvent(
                                request, 
                                restapiData.hostPort, 
                                restapiData.authorization, 
                                restapiData.event));
                    break;
                }
                case OWNERS:
                {
                    break;
                }
                default:
                {
                    restapiData.state = RESTAPI_STATE_DISCONNECT;
                }
            }
            
			TCPIP_TCP_Flush(restapiData.mySocket);
			restapiData.state = RESTAPI_STATE_PROCESS_RESPONSE;
			break;

		case RESTAPI_STATE_PROCESS_RESPONSE:
			if(!TCPIP_TCP_IsConnected(restapiData.mySocket))
			{
				restapiData.state = RESTAPI_STATE_DISCONNECT;
			}
	
			uint16_t answerSize = TCPIP_TCP_GetIsReady(restapiData.mySocket);	
	
            uint8_t vBuffer[200];
            uint8_t i = sizeof(vBuffer)-1;
			vBuffer[i] = '\0';
			while(answerSize)
			{
				if(answerSize < i)
				{
					i = answerSize;
					vBuffer[i] = '\0';
				}
				answerSize -= TCPIP_TCP_ArrayGet(restapiData.mySocket, vBuffer, i);
			}

            restapiData.state = RESTAPI_STATE_DISCONNECT;
			break;
	
		case RESTAPI_STATE_DISCONNECT:
			TCPIP_TCP_Disconnect(restapiData.mySocket);

			restapiData.mySocket = INVALID_SOCKET;
			restapiData.state = RESTAPI_STATE_WAITING_FOR_EVENT;
			break;
	
		case RESTAPI_STATE_WAITING_FOR_EVENT:

            if(isSwitch1StateChanged())
            {
                restapiData.event->timeseries[0].value = "SWITCH_STATE_PRESSED";
                restapiData.event->timeseries[1].value = "SWITCH_STATE_RELEASED";
                restapiData.event->timeseries[2].value = "SWITCH_STATE_RELEASED";
                break;
            }
            if(isSwitch2StateChanged())
            {
                restapiData.event->timeseries[1].value = "SWITCH_STATE_PRESSED";
                restapiData.event->timeseries[0].value = "SWITCH_STATE_RELEASED";
                restapiData.event->timeseries[2].value = "SWITCH_STATE_RELEASED";
                break;
            }
            if(isSwitch3StateChanged())
            {
                restapiData.event->timeseries[2].value = "SWITCH_STATE_PRESSED";
                restapiData.event->timeseries[0].value = "SWITCH_STATE_RELEASED";
                restapiData.event->timeseries[1].value = "SWITCH_STATE_RELEASED";
            }
			break;

        default:
        {
            break;
        }
    }
}

bool isSwitch1StateChanged()
{
    restapiData.btn1 = BSP_SwitchStateGet(BSP_SWITCH_1);
    if(restapiData.btn1 != restapiData.button1 )
    {
        if(restapiData.btn1 == BSP_SWITCH_STATE_PRESSED )
        {
            restapiData.connectionRetry = 0;
            restapiData.state = RESTAPI_STATE_HOME;
            BSP_LEDToggle(BSP_LED_3);
            restapiData.button1 = restapiData.btn1;
            return true;
        }
        restapiData.button1 = restapiData.btn1;
    }
    return false;
}

bool isSwitch2StateChanged()
{
    restapiData.btn2 = BSP_SwitchStateGet(BSP_SWITCH_2);
    if(restapiData.btn2 != restapiData.button2)
    {
        if(restapiData.btn2 == BSP_SWITCH_STATE_PRESSED)
        {
            restapiData.connectionRetry = 0;
            restapiData.state = RESTAPI_STATE_HOME;
            BSP_LEDToggle(BSP_LED_3);
            restapiData.button2 = restapiData.btn2;
            return true;
        }
        restapiData.button2 = restapiData.btn2;
    }
    return false;
}

bool isSwitch3StateChanged()
{
    restapiData.btn3 = BSP_SwitchStateGet(BSP_SWITCH_3);
    if(restapiData.btn3 != restapiData.button3)
    {
        if(restapiData.btn3 == BSP_SWITCH_STATE_PRESSED)
        {
            restapiData.connectionRetry = 0;
            restapiData.state = RESTAPI_STATE_HOME;
            BSP_LEDToggle(BSP_LED_3);
            restapiData.button3 = restapiData.btn3;
            return true;
        }
        restapiData.button3 = restapiData.btn3;
    }
    return false;
}
/*******************************************************************************
 End of File
 */
