/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

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

#include "app.h"
#include "GenericTypeDefs.h"
#include "tcpip/tcp.h"
#include "tcpip/tcpip.h"

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

APP_DATA appData;

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
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            break;
        }

        /* TODO: implement your application state machine.*/

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}
 
void GenericTCPClient(void)
{
	BYTE 				i;
	WORD				w;
	BYTE				vBuffer[9];
	static DWORD		Timer;
	static TCP_SOCKET	MySocket = INVALID_SOCKET;
	static enum _GenericTCPExampleState
	{
		SM_HOME = 0,
		SM_SOCKET_OBTAINED,
		SM_PROCESS_RESPONSE,
		SM_DISCONNECT,
		SM_DONE
	} GenericTCPExampleState = SM_DONE;

    TCP_PORT localPort = 80; 
    IP_MULTI_ADDRESS* localAddress = malloc(sizeof(IP_MULTI_ADDRESS));
    uint32_t TICK_SECOND = 5;
    localAddress->v4Add.Val = 0x0AEDE058;
            
	switch(GenericTCPExampleState)
	{
		case SM_HOME:
			// Connect a socket to the remote TCP server
            
             TCPIP_TCP_ServerOpen(IP_ADDRESS_TYPE_IPV4, localPort, localAddress);
			
			// Abort operation if no TCP socket of type TCP_PURPOSE_GENERIC_TCP_CLIENT is available
			// If this ever happens, you need to go add one to TCPIPConfig.h
			if(MySocket == INVALID_SOCKET)
				break;

			#if defined(STACK_USE_UART)
			putrsUART((char*)"\r\n\r\nConnecting using Microchip TCP API...\r\n");
			#endif

			GenericTCPExampleState++;
//			Timer = SYS_TICK_Get();
			break;

		case SM_SOCKET_OBTAINED:
			// Wait for the remote server to accept our connection request
			if(!TCPIP_TCP_IsConnected(MySocket))
			{
				// Time out if too much time is spent in this state
				/**if(Tick()-Timer > 5*TICK_SECOND)
				{
					// Close the socket so it can be used by other modules
					TCPIP_TCP_Disconnect(MySocket);
                    #if defined(STACK_USE_UART)
			        putrsUART((ROM char*)"Socket timed out...\r\n");
			        #endif
					MySocket = INVALID_SOCKET;
					GenericTCPExampleState--;
				}*/
				break;
			}

//			Timer = TickGet();

			// Make certain the socket can be written to
			if(TCPIP_TCP_PutIsReady(MySocket) < 125u)
				break;
            
            #if defined(STACK_USE_UART)
			putrsUART((ROM char*)"Socket ready...\r\n");
			#endif
			
			// Place the application protocol data into the transmit buffer.  For this example, we are connected to an HTTP server, so we'll send an HTTP GET request.
			TCPIP_TCP_StringPut(MySocket, (BYTE*)"GET ");
			TCPIP_TCP_StringPut(MySocket, "www.mnubo.com");
			TCPIP_TCP_StringPut(MySocket, (BYTE*)" HTTP/1.0\r\nHost: ");
			TCPIP_TCP_StringPut(MySocket, "mnubo.com");
			TCPIP_TCP_StringPut(MySocket, (BYTE*)"\r\nConnection: close\r\n\r\n");

			// Send the packet
			TCPIP_TCP_Flush(MySocket);
			GenericTCPExampleState++;
            #if defined(STACK_USE_UART)
			putrsUART((ROM char*)"Sent HTTP data...\r\n");
			#endif
            #if defined(STACK_USE_UART)
            putsUART("...Response from Server:");
			#endif
			break;

		case SM_PROCESS_RESPONSE:
			// Check to see if the remote node has disconnected from us or sent us any application data
			// If application data is available, write it to the UART
			if(!TCPIP_TCP_IsConnected(MySocket))
			{
				GenericTCPExampleState = SM_DISCONNECT;
				// Do not break;  We might still have data in the TCP RX FIFO waiting for us
			}
	
			// Get count of RX bytes waiting
			w = TCPIP_TCP_GetIsReady(MySocket);	
	
			// Obtian and print the server reply
			i = sizeof(vBuffer)-1;
			vBuffer[i] = '\0';
			while(w)
			{
				if(w < i)
				{
					i = w;
					vBuffer[i] = '\0';
				}
				w -= TCPIP_TCP_ArrayGet(MySocket, vBuffer, i);
				#if defined(STACK_USE_UART)
                putsUART((char*)vBuffer);
                //putcUART('x');
				#endif
				
				// putsUART is a blocking call which will slow down the rest of the stack 
				// if we shovel the whole TCP RX FIFO into the serial port all at once.  
				// Therefore, let's break out after only one chunk most of the time.  The 
				// only exception is when the remote node disconncets from us and we need to 
				// use up all the data before changing states.
				if(GenericTCPExampleState == SM_PROCESS_RESPONSE)
					break;
			}
	
			break;
	
		case SM_DISCONNECT:
			// Close the socket so it can be used by other modules
			// For this application, we wish to stay connected, but this state will still get entered if the remote server decides to disconnect
			TCPIP_TCP_Disconnect(MySocket);

            #if defined(STACK_USE_UART)
			putrsUART((ROM char*)"Disconnected.\r\n");
			#endif

			MySocket = INVALID_SOCKET;
			GenericTCPExampleState = SM_DONE;
			break;
	
		case SM_DONE:
			// Do nothing unless the user pushes BUTTON1 and wants to restart the whole connection/download process
			//if(BUTTON1_IO == 0u)
				GenericTCPExampleState = SM_HOME;
			break;
	}
}
/*******************************************************************************
 End of File
 */
