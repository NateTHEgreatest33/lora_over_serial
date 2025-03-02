/*********************************************************************
*
*   MODULE:
*       main.cpp
*
*   DESCRIPTION:
*       main proccessing loops for module software on raspberry pi
*       pico
*
*   Copyright Nate Lenze 2023
*********************************************************************/

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/error.h"
#include "pico/multicore.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

#include "messageAPI.hpp"
#include "console.hpp"
#include "serial_lora.hpp"

#include <iostream>

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/
const location current_location = PICO_MODULE;

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/
/* --------------------------------
Note: both messsageAPI and Lora 
Require core::console. In our case
we are not using console but it is 
required to exist
-------------------------------- */
core::console console( uart1 );                     /* console API  */
core::loraInterface loRa( spi_default ,console );   /* Lora API     */
core::messageInterface messageAPI( loRa, console ); /* Message API  */

test::lora_serial lora_over_serial( uart0, loRa, messageAPI ); /* LoRa
                                                        over serial */ 

bool g_test_mode_enable = false;
/*--------------------------------------------------------------------
                               GLOBALS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/
int main                                   /* main proccessing loop */
    (
    void
    );

/*********************************************************************
*
*   PROCEDURE NAME:
*       main
*
*   DESCRIPTION:
*       main proccessing loop for module
*
*********************************************************************/
int main
    (
    void
    )
{
/*----------------------------------------------------------
Local variables
----------------------------------------------------------*/
bool             sdio_err_var; /* stdio init errors       */
pico_error_codes wifi_err_var; /* wifi init errors        */

/*----------------------------------------------------------
Initialize local variables
----------------------------------------------------------*/
sdio_err_var = false;
wifi_err_var = PICO_ERROR_GENERIC;

/*----------------------------------------------------------
Initialize all subsystems   
----------------------------------------------------------*/
sdio_err_var = stdio_init_all();
wifi_err_var = (pico_error_codes) cyw43_arch_init();


/*----------------------------------------------------------
If issue with subsystem initilization do not move forward.
No light will be on if this is the case
----------------------------------------------------------*/
if ( wifi_err_var != PICO_OK || !sdio_err_var )
    {
    }

/*----------------------------------------------------------
Set LED on to signify start of main process loop
----------------------------------------------------------*/
cyw43_arch_gpio_put( CYW43_WL_GPIO_LED_PIN, 1 );

/*----------------------------------------------------------
System Test procedure/replies
----------------------------------------------------------*/
while( true )
    {
	/*------------------------------------------------------
    Run lora_serial 
    ------------------------------------------------------*/
    lora_over_serial.runtime();

	} /* while(true) */

} /* main() */