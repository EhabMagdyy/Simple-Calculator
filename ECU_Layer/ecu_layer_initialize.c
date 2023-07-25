/* 
 * File:   ecu_layer_intialize.c
 * Author: Ehab
 *
 * Created on 03 ?????, 2023, 08:04 ?
 */

#include "ecu_layer_intialize.h"

keypad_t keypad = {
    .keypad_rows_pins[0].port = PORTC_INDEX,
    .keypad_rows_pins[0].pin = GPIO_PIN0,
    .keypad_rows_pins[0].direction = GPIO_direction_output,
    .keypad_rows_pins[0].logic = GPIO_LOW,
    .keypad_rows_pins[1].port = PORTC_INDEX,
    .keypad_rows_pins[1].pin = GPIO_PIN1,
    .keypad_rows_pins[1].direction = GPIO_direction_output,
    .keypad_rows_pins[1].logic = GPIO_LOW,
    .keypad_rows_pins[2].port = PORTC_INDEX,
    .keypad_rows_pins[2].pin = GPIO_PIN2,
    .keypad_rows_pins[2].direction = GPIO_direction_output,
    .keypad_rows_pins[2].logic = GPIO_LOW,
    .keypad_rows_pins[3].port = PORTC_INDEX,
    .keypad_rows_pins[3].pin = GPIO_PIN3,
    .keypad_rows_pins[3].direction = GPIO_direction_output,
    .keypad_rows_pins[3].logic = GPIO_LOW,
    
    .keypad_coloumns_pins[0].port = PORTC_INDEX,
    .keypad_coloumns_pins[0].pin = GPIO_PIN4,
    .keypad_coloumns_pins[0].direction = GPIO_direction_input,
    .keypad_coloumns_pins[0].logic = GPIO_LOW,
    .keypad_coloumns_pins[1].port = PORTC_INDEX,
    .keypad_coloumns_pins[1].pin = GPIO_PIN5,
    .keypad_coloumns_pins[1].direction = GPIO_direction_input,
    .keypad_coloumns_pins[1].logic = GPIO_LOW,
    .keypad_coloumns_pins[2].port = PORTC_INDEX,
    .keypad_coloumns_pins[2].pin = GPIO_PIN6,
    .keypad_coloumns_pins[2].direction = GPIO_direction_input,
    .keypad_coloumns_pins[2].logic = GPIO_LOW,
    .keypad_coloumns_pins[3].port = PORTC_INDEX,
    .keypad_coloumns_pins[3].pin = GPIO_PIN7,
    .keypad_coloumns_pins[3].direction = GPIO_direction_input,
    .keypad_coloumns_pins[3].logic = GPIO_LOW,
};

lcd_8bit_t lcd = {
    .lcd_rs.port = PORTE_INDEX,
    .lcd_rs.pin = GPIO_PIN0,
    .lcd_rs.direction = GPIO_direction_output,
    .lcd_rs.logic = GPIO_LOW,
    .lcd_enable.port = PORTE_INDEX,
    .lcd_enable.pin = GPIO_PIN1,
    .lcd_enable.direction = GPIO_direction_output,
    .lcd_enable.logic = GPIO_LOW,
    
    .data_lines[0].port = PORTD_INDEX,
    .data_lines[0].pin = GPIO_PIN0,
    .data_lines[0].direction = GPIO_direction_output,
    .data_lines[0].logic = GPIO_LOW,
    .data_lines[1].port = PORTD_INDEX,
    .data_lines[1].pin = GPIO_PIN1,
    .data_lines[1].direction = GPIO_direction_output,
    .data_lines[1].logic = GPIO_LOW,
    .data_lines[2].port = PORTD_INDEX,
    .data_lines[2].pin = GPIO_PIN2,
    .data_lines[2].direction = GPIO_direction_output,
    .data_lines[2].logic = GPIO_LOW,
    .data_lines[3].port = PORTD_INDEX,
    .data_lines[3].pin = GPIO_PIN3,
    .data_lines[3].direction = GPIO_direction_output,
    .data_lines[3].logic = GPIO_LOW,
    .data_lines[4].port = PORTD_INDEX,
    .data_lines[4].pin = GPIO_PIN4,
    .data_lines[4].direction = GPIO_direction_output,
    .data_lines[4].logic = GPIO_LOW,
    .data_lines[5].port = PORTD_INDEX,
    .data_lines[5].pin = GPIO_PIN5,
    .data_lines[5].direction = GPIO_direction_output,
    .data_lines[5].logic = GPIO_LOW,
    .data_lines[6].port = PORTD_INDEX,
    .data_lines[6].pin = GPIO_PIN6,
    .data_lines[6].direction = GPIO_direction_output,
    .data_lines[6].logic = GPIO_LOW,
    .data_lines[7].port = PORTD_INDEX,
    .data_lines[7].pin = GPIO_PIN7,
    .data_lines[7].direction = GPIO_direction_output,
    .data_lines[7].logic = GPIO_LOW,
    
};

Std_ReturnType ecu_layer_intialize()
{
    Std_ReturnType ret = E_NOT_OK;
    
    ret = keypad_intialize(&keypad);
    ret = lcd_8bit_intialize(&lcd);
    
    return ret;
}
