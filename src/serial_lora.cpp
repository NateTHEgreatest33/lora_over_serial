
/*********************************************************************
*
*   NAME:
*       serial_lora.cpp
*
*   DESCRIPTION:
*       API for interfacing with Lora transciver over serial
*
*   Copyright 2025 Nate Lenze
*
*********************************************************************/

/*--------------------------------------------------------------------
                              INCLUDES
--------------------------------------------------------------------*/
#include "serial_lora.hpp"



/*--------------------------------------------------------------------
                          GLOBAL NAMESPACES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/
#define MAX_CMD_STR_SIZE (100)               /* max length of comand */

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/


/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/
/*********************************************************************
*
*   PROCEDURE NAME:
*       test::lora_serial (constructor)
*
*   DESCRIPTION:
*       lora_serial class constructor
*
*********************************************************************/
test::lora_serial::lora_serial
    (      
    uart_inst_t* uart_port,               /* uart port for console */
    core::loraInterface& lora_ref,        /* lora reference        */
    core::messageInterface& msg_ref       /* messageAPI reference  */
    ) : 
    p_uart( uart_port ),
    p_msg( msg_ref ),
    p_lora( lora_ref ),
    p_commands{ { "lora send",    [this](test::command_state s) { this->lora_tx(s); }    },
                { "lora get",     [this](test::command_state s) { this->lora_rx(s); }    },
                { "message send", [this](test::command_state s) { this->message_tx(s); } },
                { "message get",  [this](test::command_state s) { this->message_rx(s); } } },
    p_current_state( READING_COMMAND ),
    p_current_cmd( nullptr )
{
uart_init( p_uart, 115200 );

} /* test::lora_serial() */

/*********************************************************************
*
*   PROCEDURE NAME:
*       test::lora_serial::runtime()
*
*   DESCRIPTION:
*       lora serial runtime function. This was pulled directly from
*       core::console:::console_runtime(). 
*
*       PR: CORE-48 was created to track an improvement to create 
*       a common interface.
*
*********************************************************************/
void test::lora_serial::runtime
    (
    void   
    )
{
/*----------------------------------------------------------
local variables
----------------------------------------------------------*/
char c        = 0x00;             /*  temporary character */
bool new_line = false;            /* new line rx'ed (Y/N) */
auto itr      = p_commands.end(); /* iterator for command */  

/*----------------------------------------------------------
Check if console is clean
----------------------------------------------------------*/
if( uart_is_readable( p_uart ) )    
    {
    c = uart_getc( p_uart );
    /*------------------------------------------------------
    clean \r command if rx'ed
    ------------------------------------------------------*/
    if( c == '\r')
        {
        uart_putc_raw( p_uart, '\r' );
        uart_putc_raw( p_uart, '\n' );
        new_line = true;
        }
    else
        {
        /*--------------------------------------------------
        Add to buffer & print back to user to show that 
        input has been processed. 
        --------------------------------------------------*/
        p_buffer += c;
        uart_putc_raw( p_uart, c );
        }  

    /*------------------------------------------------------
    Don't allow p_buffer to grow exponentially. Reset
    console window and buffer
    ------------------------------------------------------*/
    if ( p_buffer.length() > MAX_CMD_STR_SIZE )
        {
        uart_putc_raw( p_uart, '\r' );
        uart_putc_raw( p_uart, '\n' );
        p_buffer="";
        }

    }

/*----------------------------------------------------------
Handle a new line char meaning a full command or args have 
been sent
----------------------------------------------------------*/
if( new_line )
    {
    if( p_current_state == READING_COMMAND )
        {
        /*-------------------------------------------------
        search for and run command if found
        -------------------------------------------------*/
        itr = p_commands.find( p_buffer );
        if( itr != p_commands.end() )
            {
            p_current_cmd = itr->second;
            p_current_cmd( p_current_state );
            }
        else
            {
            std::cout << "command not found" << std::endl;
            }
        }

    if( p_current_state == ARGS_COMMAND )
            {
            /*-------------------------------------------------
            search for and run command if found
            -------------------------------------------------*/
            p_current_cmd( ARGS_COMMAND );
            }




    /*------------------------------------------------------
    clear buffer
    ------------------------------------------------------*/
    p_buffer = "";
    }

} /* test::lora_serial::runtime() */



void test::lora_serial::lora_tx( test::command_state s )
{
p_current_state = ARGS_COMMAND;
}
void test::lora_serial::message_tx( test::command_state s )
{
p_current_state = ARGS_COMMAND;
}
void test::lora_serial::lora_rx( test::command_state s )
{
p_current_state = ARGS_COMMAND;
}
void test::lora_serial::message_rx( test::command_state s )
{
p_current_state = ARGS_COMMAND;
}

test::lora_serial::~lora_serial(){}

void test::lora_serial::starter_text( void ){}