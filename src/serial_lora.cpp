
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
#include "sys_def.h"


/*--------------------------------------------------------------------
                          GLOBAL NAMESPACES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/
#define MAX_CMD_W_ARGS_STR_SIZE (100) /* max len of command w/ args */
#define MAX_CMD_STR_SIZE        (20)  /* max len of command         */

const std::string starter_str = "Welcome to Lora/MsgAPI IO Tool."\
                 " Usage is as follows:\r\n"\
                 " - lora rx:           lora get\r\n"\
                 " - lora tx:           lora send 0x00 0x00....\r\n"\
                 " - msgAPI tx:         message send 0x00 0x00....\r\n"\
                 " - msgAPI rx:         message get\r\n"\
                 " - msgAPI set source: message set source 0x00\r\n"\
                 " - msgAPI set key:    message set key 0x00\r\n";

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
    p_commands{ { "lora send",          [this](std::string s) { this->lora_tx(s);         } },
                { "lora get",           [this](std::string s) { this->lora_rx(s);         } },
                { "message send",       [this](std::string s) { this->message_tx(s);      } },
                { "message get",        [this](std::string s) { this->message_rx(s);      } },
                { "message set source", [this](std::string s) { this->message_set_src(s); } },
                { "message set key",    [this](std::string s) { this->message_set_key(s); } },
                { "help",               [this](std::string s) { this->starter_text();     } }},

    p_current_cmd( nullptr )
{

starter_text();

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
int return_val = 0x00;
bool new_line = false;            /* new line rx'ed (Y/N) */
auto itr      = p_commands.end(); /* iterator for command */

/*----------------------------------------------------------
Check if console is clean
----------------------------------------------------------*/
return_val = getchar_timeout_us(0);

if( return_val != PICO_ERROR_TIMEOUT )
    {
    
    c = (char)return_val;
    /*------------------------------------------------------
    clean \r command if rx'ed
    ------------------------------------------------------*/
    if( c == '\r')
        {
        // uart_putc_raw( p_uart, '\r' );
        // uart_putc_raw( p_uart, '\n' );
        putchar( '\r' );
        putchar( '\n' );
        new_line = true;
        }
    else
        {
        /*--------------------------------------------------
        Add to buffer & print back to user to show that 
        input has been processed. 
        --------------------------------------------------*/
        p_buffer += c;
        // uart_putc_raw( p_uart, c );
        putchar( c );
        }  

    /*------------------------------------------------------
    Don't allow p_buffer to grow exponentially. Reset
    console window and buffer
    ------------------------------------------------------*/
    if ( p_buffer.length() > MAX_CMD_W_ARGS_STR_SIZE )
        {
        // uart_putc_raw( p_uart, '\r' );
        // uart_putc_raw( p_uart, '\n' );
        putchar( '\r' );
        putchar( '\n' );
        p_buffer="";
        }

    }

/*----------------------------------------------------------
Handle a new line char meaning a full command & args have 
been sent
----------------------------------------------------------*/
if( new_line )
    {
    /*------------------------------------------------------
    parser args and command
    ------------------------------------------------------*/
    // printf(" new line hit...\n");
    // std::cout << " new line hit...\n";
    command_arg_parser( p_buffer );

    /*------------------------------------------------------
    clear buffer
    ------------------------------------------------------*/
    p_buffer = "";
    }

} /* test::lora_serial::runtime() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       test::lora_serial::x()
*
*   DESCRIPTION:
*       transmit raw byte array over lora
*
*********************************************************************/
void test::lora_serial::lora_tx( std::string args )
{
std::cout << "Not implemented yet" << std::endl;
return;
}/* test::lora_serial::lora_tx() */

/*********************************************************************
*
*   PROCEDURE NAME:
*       test::lora_serial::lora_rx()
*
*   DESCRIPTION:
*       return any lora messages since last call
*
*********************************************************************/
void test::lora_serial::lora_rx( std::string args )
{ /* no args */
std::cout << "Not implemented yet" << std::endl;
return;
} /* test::lora_serial::lora_rx() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       test::lora_serial::message_tx()
*
*   DESCRIPTION:
*       transmit messageAPI message
*
*********************************************************************/
void test::lora_serial::message_tx( std::string args )
{
    /* args:
    1) byte 1 - dest
    2) byte 2 - size
    3) byte X - data...  
    */

    tx_message tx_msg;
    int i{0};
    uint8_t msg_idx{0};

    /* Verify and set destination + size */
    if ( args.length() < 9  )
        {
        std::cout << "argument list is too small for messageTX";
        }
    else
        {
        tx_msg.destination = (location) std::stoi( args.substr(0,4), 0, 16 );
        tx_msg.size = (uint8_t) std::stoi( args.substr(5,4), 0, 16 );
        std::cout << "Dest: " << (int)tx_msg.destination << " Size: " << (int)tx_msg.size << std::endl;
        }
    /* set i to start of data stream */
    i = 10;
    
    /* loop until data is filled in*/
    while( (i+4) <= args.length() && msg_idx < 10 )
        {
        tx_msg.message[msg_idx++] = std::stoi( args.substr(i,4), 0, 16 );
        i += 5;
        }
    
    /* send data and report errors */
    if( !p_msg.send_message( tx_msg ) )
        {
        std::cout << "Error sending message" << std::endl;
        }

} /* test::lora_serial::message_tx() */

/*********************************************************************
*
*   PROCEDURE NAME:
*       test::lora_serial::message_rx()
*
*   DESCRIPTION:
*       return any messageAPI messages since last call
*
*********************************************************************/
void test::lora_serial::message_rx( std::string args )
{
auto rtn = p_msg.get_multi_message();

/* error handling*/
if( rtn.global_errors != MSG_NO_ERROR )
    {
    std::cout << "global errors on Rx: " << rtn.global_errors << std::endl;
    return;
    }

/* no msg case */
if( rtn.num_messages == 0 )
    {
    std::cout << "No Msg's Rx'ed"<< std::endl;
    return;
    }

/* messages case */
for( int i = 0; i < rtn.num_messages; i++ )
    {
    rx_message curr_msg = rtn.messages[i];
    std::cout << "Msg " << i << " - S: " << curr_msg.source << " V: " << curr_msg.valid;
    for( int j = 0; j < curr_msg.size; j++ )
        {
        std::cout << "["<< std::hex << curr_msg.message[j] << "] ";
        }
    std::cout << std::endl << std::dec; 
    }



} /* test::lora_serial::message_rx() */

/*********************************************************************
*
*   PROCEDURE NAME:
*       test::lora_serial::message_set_src()
*
*   DESCRIPTION:
*       set source location (messageAPI)
*
*********************************************************************/
void test::lora_serial::message_set_src( std::string args )
{
uint8_t src;
src = std::stoul( args, nullptr, 16);

if( src >= NUM_OF_MODULES )
    std::cout << "Error: module ID does not match sys_def\r\n";
else
    current_location = (location)src;
} /* test::lora_serial::message_set_src() */

/*********************************************************************
*
*   PROCEDURE NAME:
*       test::lora_serial::message_set_key()
*
*   DESCRIPTION:
*       set key (messageAPI)
*
*********************************************************************/
void test::lora_serial::message_set_key( std::string args )
{
uint8_t key;

key = std::stoul( args, nullptr, 16);
p_msg.update_key( key );
std::cout << "Key updated to: " << key << "\r\n";
} /* test::lora_serial::message_set_key() */

/*********************************************************************
*
*   PROCEDURE NAME:
*       test::lora_serial::~lora_serial()
*
*   DESCRIPTION:
*       deconstructor
*
*********************************************************************/
test::lora_serial::~lora_serial(){}

/*********************************************************************
*
*   PROCEDURE NAME:
*       test::lora_serial::starter_text()
*
*   DESCRIPTION:
*       initial text lora serial will display
*
*********************************************************************/
void test::lora_serial::starter_text( void ){
    std::cout << starter_str;
} /* test::lora_serial::starter_text() */

/*********************************************************************
*
*   PROCEDURE NAME:
*       test::lora_serial::command_arg_parser()
*
*   DESCRIPTION:
*       parses command and arguments sections & calls corisponding
*       command function
*
*********************************************************************/
void test::lora_serial::command_arg_parser( std::string s )
{
bool command_found = false;
int length = 1;
auto itr = p_commands.end(); /* iterator for command */  
std::cout << "starting arg parse...\n\r";

/* dont overrun length, dont go past max cmd str size, stop if found */
while( length <= s.length() && command_found != true && length < MAX_CMD_STR_SIZE )
    {
    /* attempt to find command */
    itr = p_commands.find( s.substr( 0, length ) );

    if( itr != p_commands.end() )
        command_found = true;
        
    //update length
    length++;

    }

/* fast exit, no command found */
if( !command_found )
    {
    std::cout << "Command not found\n\r";
    return;
    }

/* sanitize string for non argument commands */
if( length > s.length() )
    length = 0;

/* no +1 is needed as we already increment @ the end */
itr->second( s.substr( length )) ;

} /* test::lora_serial::command_arg_parser() */