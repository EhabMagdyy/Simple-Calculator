/**
 **   File:   calculator.c
 **   Author: Ehab Magdy
 **   Created on 03 July, 2023
 */

#include "calculator.h"

Std_ReturnType ret = E_OK;  

uint8 keypad_value;     /* used to read keypad value */
uint8 lcd_cursor;       /* have the next position on the screen */
uint8 read_data[20];    /* Array to store data entered by keypad */
uint8 data_index;       /* length of input data from keypad */
uint8 equal_sign_flag;  /* after you press '=', click any button and the screen will be cleared */

int main()
{
    application_intialize();
    
    while(1)
    {
        keypad_value = keypad_read_value();
        /* clear the screen and reset variables */
        if('c' == keypad_value && 1 == equal_sign_flag)
        {
            ret = lcd_8bit_send_command(&lcd, LCD_CLEAR);
            memset(read_data, '\0', 20);
            lcd_cursor = 0;
            data_index = 0;
            equal_sign_flag = 0;
        }
        /* Delete the previous character if there already data on the screen */
        else if('c' == keypad_value && 0 < data_index)
        {
            ret = lcd_8bit_send_char_pos(&lcd, 1, lcd_cursor, ' ');
            data_index--;
            lcd_cursor--;
            read_data[data_index] = '\0';
        }
        /* calculate the result when the user hit '=' */
        else if('=' == keypad_value)
        {   
            if(!isInputValid(read_data, data_index))
            {
                ret = lcd_8bit_send_command(&lcd, LCD_CLEAR);
                ret = lcd_8bit_send_string_pos(&lcd, 1, 3, "Invalid operation");
                ret = lcd_8bit_send_string_pos(&lcd, 2, 6, "Try again");
                __delay_ms(1000);
                ret = lcd_8bit_send_command(&lcd, LCD_CLEAR);
                memset(read_data, '\0', 20);
                lcd_cursor = 0;
                data_index = 0;
            }
            else{
                ret = lcd_8bit_send_char_pos(&lcd, 2, 1, '=');
                calculate_display_result(read_data, data_index);
                equal_sign_flag = 1;
            }
        }
        /* Storing input from keypad if the end of the screen not reached yet  */
        else if('c' != keypad_value && (20 != lcd_cursor))
        {
            ret = lcd_8bit_send_char_pos(&lcd, 1, ++lcd_cursor, keypad_value);
            read_data[data_index++] = keypad_value;
        }
        else { /* Nothing */ }
    }
    
    return (EXIT_SUCCESS);
}

/**************** Functions *****************/

void application_intialize(){
    ret = ecu_layer_intialize();
}

/* Read keypad value */
uint8 keypad_read_value()
{
    uint8 keypad_pressed_value = 0;
    do {
        __delay_ms(20);
        ret = keypad_get_value(&keypad, &keypad_pressed_value);
    } while(0 == keypad_pressed_value);
    __delay_ms(DEBOUNE_TIME);
    return keypad_pressed_value;  
}

void calculate_display_result(uint8* operation, uint8 len)
{
    /* check if the operation has high(*,/) and low(+,-) operators */
    uint8 high_priority_op = 0;
    uint8 low_priority_op = 0;
    uint8 different_priority_flag = 0;
    for(uint8 op_index = 1 ; op_index < len ; op_index++){
        if('*' == operation[op_index] || '/' == operation[op_index]){
            high_priority_op = 1;
            if(low_priority_op){
                different_priority_flag = 1;
                break;
            }
            else{/* Nothing */}
        }
        else if('+' == operation[op_index] || ('-' == operation[op_index] && '0' <= operation[op_index-1])){
            low_priority_op = 1;
            if(high_priority_op){
                different_priority_flag = 1;
                break;
            }
            else{/* Nothing */}
        }
        else{/* Nothing */}
    }
    
    if(different_priority_flag){
        /* Getting operation after making the multiplication and division  */
        len = get_operation_after_mult_div(operation, len);
    }
    else {/* Nothing */}
    
    uint8 num1_text[8] = {[0] = operation[0],[1 ... 7] = '\0'};
    uint8 num2_text[8] = {0};
    uint8 num1_index = 1, n2_index = 0;

    for(uint8 op_index = 1 ; op_index < len ; op_index++)
    {
        if('0' <= operation[op_index] && '9' >= operation[op_index])
        {
            /* Store number at n1 */
            num1_text[num1_index++] = operation[op_index];
        }
        else if('0' > operation[op_index])
        {
            uint8 op = operation[op_index];
            /* check if the first char is '-' store it or else it will store a number easily */
            num2_text[n2_index++] = operation[++op_index];
            op_index++;
            /* Scan the rest of number 2 */
            while('0' <= operation[op_index] && '9' >= operation[op_index]){
                num2_text[n2_index++] = operation[op_index++];
            }
            op_index--;
            /* Get result and store it at number 1 */
            get_two_operands_result(num1_text, num2_text, op, &num1_index);
            n2_index = 0;
            memset(num2_text, '\0', 8);
        }
    }
    /* Displaying result */
    ret = lcd_8bit_send_string_pos(&lcd, 2, 3, num1_text);
}

uint8 isInputValid(uint8* operation, uint8 len)
{
    uint8 operation_valid = 1;
    
    if(('0' > operation[0] && '-' != operation[0]) || '0' > operation[len-1]){
        operation_valid = 0;
    }
    else{
        /* check for invalid operation (xx,x/,x+,//,/x,/+,/0,-*,-/,-+,+*,+/,++) */
        uint8 last_char = operation[0];
        
        for(uint8 op_index = 1 ; op_index < len ; op_index++)
        {
            if(('0' > last_char) && ('0' > operation[op_index]) && ('-' != operation[op_index])){
                operation_valid = 0;
                break;
            }
            else if('/' == last_char && '0' == operation[op_index]){
                operation_valid = 0;
                break;
            }
            last_char = operation[op_index];
        }
    }
    
    return operation_valid;
}

uint8 get_operation_after_mult_div(uint8* operation, uint8 len)
{
    uint8 result[9] = {0};
    uint8 result_index = 0;
    uint8 num1_text[8] = {[0] = operation[0],[1 ... 7] = '\0'}, num2_text[8] = {0};
    uint8 num1_index = 1, num2_index = 0;

    for(int op_index = 1 ; op_index < len ; op_index++)
    {
        if('0' <= operation[op_index] && '9' >= operation[op_index])
        {
            num1_text[num1_index++] = operation[op_index];
            /* Reached the end */
            if(len-1 == op_index){
                while(num1_index--){
                    result[result_index+num1_index] = num1_text[num1_index];
                }
            }
        }
        else if('-' == operation[op_index] || '+' == operation[op_index])
        {
            uint8 temp = num1_index;
            while(num1_index--){
                result[result_index+num1_index] = num1_text[num1_index];
            }
            result_index += temp;
            result[result_index++] = operation[op_index];

            num1_index = 0;
            memset(num1_text, '\0', 8);
            num1_text[num1_index++] = operation[++op_index];

            if(len-1 == op_index){
                result[result_index] = num1_text[num1_index-1];
            }
        }
        else if('*' == operation[op_index] || '/' == operation[op_index])
        {
            uint8 op = operation[op_index];
            /* if the first char is '-' store it */
            num2_text[num2_index++] = operation[++op_index];
            op_index++;
            /* Scan the rest of number 2 */
            while('0' <= operation[op_index] && '9' >= operation[op_index]){
                num2_text[num2_index++] = operation[op_index++];
            }
            op_index--;
            /* Get result and store it at number 1 */
            get_two_operands_result(num1_text, num2_text, op, &num1_index);
            num2_index = 0;
            memset(num2_text, '\0', 8);
            
            if(len-1 == op_index){
                while(num1_index--){
                    result[result_index+num1_index] = num1_text[num1_index];
                }
            }
        }
        else{/* Nothing */}
    }
    memset(operation, '\0', 20);
    strcpy(operation, result);
    return strlen(operation);
}

void get_two_operands_result(uint8* num1_text, uint8* num2_text, uint8 op, uint8* num1_len)
{
    sint32 num1_value = 0, num2_value = 0, result = 0;
    num1_value = atol(num1_text);
    num2_value = atol(num2_text);

    switch (op)
    {
        case '+': result = num1_value + num2_value; break;
        case '-': result = num1_value - num2_value; break;
        case '*': result = num1_value * num2_value; break;
        case '/': result = num1_value / num2_value; break;
        default:                                    break;
    }
    
    memset(num1_text, '\0', 8);
    sprintf(num1_text, "%lu", result);

    *num1_len = strlen(num1_text);
}