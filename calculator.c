/* 
 * File:   calculator.c
 * Author: Ehab
 *
 * Created on 03 July, 2023
 */

#include "calculator.h"

#define DEBOUNE_TIME    150

Std_ReturnType ret = E_OK;

uint8 keypad_value;
uint8 lcd_output_char_col = 0;
uint8 read_data[20];
uint8 data_index = 0;

int main()
{
    application_intialize();
    
    while(1)
    {
        keypad_value = keypad_read_value();
        /* clear the screen and reset variables */
        if('c' == keypad_value)
        {
            ret = lcd_8bit_send_command(&lcd, LCD_CLEAR);
            lcd_output_char_col = 0;
            data_index = 0;
        }
        /* calculate the result when the user hit '=' */
        else if('=' == keypad_value)
        {
            ret = lcd_8bit_send_char_pos(&lcd, 2, 1, '=');
            
            uint8 first_num_text[9] = {[0 ... 8] = '\0'};
            uint8 second_num_text[9] = {[0 ... 8] = '\0'};
            uint8 index = 0;
            /* Storing first number into string */
            first_num_text[0] = read_data[0];
            for(index = 1 ; ('*' != read_data[index] && '+' != read_data[index] 
                            && '-' != read_data[index] && '/' != read_data[index]) ; index++){
                first_num_text[index] = read_data[index];
            }
            /* Storing operator */
            uint8 operator = read_data[index];
            /* Storing second number into string */
            uint8 second_value_index = 0;
            while(++index < data_index){
                second_num_text[second_value_index++] = read_data[index];
            }
            /* Converting num1 & num2 string into integer */
            sint32 num1_val = atol(first_num_text);
            sint32 num2_val = atol(second_num_text);
            /* calculating result */
            sint32 result = ('+' == operator)? (num1_val + num2_val): 
                            ('-' == operator)?(num1_val - num2_val):
                            ('*' == operator)? (num1_val * num2_val):
                            (num1_val / num2_val);
            uint8 result_text[11];
            
            ret = lcd_convert_int_to_string(result, result_text);
            ret = lcd_8bit_send_string_pos(&lcd, 2, 3, result_text);
        }
        /* Storing input from keypad if the end of the screen not reached yet  */
        else if(keypad_value && (20 != lcd_output_char_col))
        {
            ret = lcd_8bit_send_char_pos(&lcd, 1, ++lcd_output_char_col, keypad_value);
            read_data[data_index] = '\0';
            read_data[data_index++] = keypad_value;
            /* For clearing the multiple set of keypad_value caused by debouncing */
            ret = lcd_8bit_send_char_pos(&lcd, 1, lcd_output_char_col+1, ' ');
        }
        else { /* Nothing */ }
    }
    
    return (EXIT_SUCCESS);
}

void application_intialize(){
    ret = ecu_layer_intialize();
}

/* Read keypad value */
uint8 keypad_read_value()
{
    uint8 current_value;
    
    ret = keypad_get_value(&keypad, &current_value);
    __delay_ms(DEBOUNE_TIME);
    if('\0' == current_value){
        return 0;
    }
    uint8 new_value;
    ret = keypad_get_value(&keypad, &new_value);
    
    if (current_value == new_value) 
        return new_value;  
    
    return 0;  
}
