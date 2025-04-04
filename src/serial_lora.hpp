#ifndef LORA_OVER_SER_HPP
#define LORA_OVER_SER_HPP
/*********************************************************************
*
*   HEADER:
*       header file for lora over serial
*
*   Copyright 2025 Nate Lenze
*
****************a*****************************************************/
/*--------------------------------------------------------------------
                              INCLUDES
--------------------------------------------------------------------*/
#include "pico/stdlib.h"

#include "LoraAPI.hpp"
#include "messageAPI.hpp"

#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>


/*--------------------------------------------------------------------
                          GLOBAL NAMESPACES
--------------------------------------------------------------------*/
namespace test {

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/
enum command_state
{
READING_COMMAND,
ARGS_COMMAND,
NUM_COMMANDS
};


struct parsed_args{
uint8_t args[100];
uint8_t size;
};

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

/*--------------------------------------------------------------------
                               CLASSES
--------------------------------------------------------------------*/
class lora_serial 
    {
    public:
        lora_serial( uart_inst_t* uart_port, core::loraInterface& lora_ref, core::messageInterface& msg_ref );
        ~lora_serial();

        void runtime( void );

        lora_serial (const lora_serial&) = delete;
        lora_serial& operator= (const lora_serial&) = delete;

    private:
        void lora_tx( std::string args ); //send data 
        void message_tx( std::string args );
        void lora_rx( std::string args );
        void message_rx( std::string args );
        void message_set_src( std::string args );
        void message_set_key( std::string args );


        void command_arg_parser( std::string s);
        parsed_args arg_parser(std::string& args);
        void starter_text( void );
        bool p_first_run;
        
        std::string p_buffer; 
        std::function< void(test::command_state s )> p_current_cmd;


        uart_inst_t* p_uart;
        core::loraInterface& p_lora;
        core::messageInterface& p_msg;
        
        const std::unordered_map< std::string, std::function< void( std::string args ) > > p_commands;


    };




} /* component test */
#endif