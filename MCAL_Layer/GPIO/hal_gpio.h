/* 
 * File:   hal_gpio.h
 * Author: Ehab
 *
 * Created on 03 ?????, 2023, 07:29 ?
 */

#ifndef HAL_GPIO_H
#define	HAL_GPIO_H

/* Includes */
#include "../device_config.h"

/* Macro Declarations */
#define BIT_MASK    (uint8)  1
#define PORT_PIN_MAX_NUMBER  8
#define PORT_MAX_NUMBER      5

/* Macro Function Declarations */
#define     SET_BIT(REG_ADDRESS, BIT_POS)       (REG_ADDRESS |=  (BIT_MASK << BIT_POS))   
#define     CLEAR_BIT(REG_ADDRESS, BIT_POS)     (REG_ADDRESS &= ~(BIT_MASK << BIT_POS))   
#define     TOGGLE_BIT(REG_ADDRESS, BIT_POS)    (REG_ADDRESS ^=  (BIT_MASK << BIT_POS))
#define     READ_BIT(REG_ADDRESS, BIT_POS)      ((REG_ADDRESS >> BIT_POS) & BIT_MASK)

/* Data Types Declarations */
typedef enum{
   GPIO_LOW = 0,
   GPIO_HIGH
}logic_t;

typedef enum{
    GPIO_direction_output = 0,
    GPIO_direction_input
}direction_t;

typedef enum{
    GPIO_PIN0 = 0,
    GPIO_PIN1,
    GPIO_PIN2,
    GPIO_PIN3,
    GPIO_PIN4,
    GPIO_PIN5,
    GPIO_PIN6,
    GPIO_PIN7,       
}pin_index_t;

typedef enum{
    PORTA_INDEX = 0, 
    PORTB_INDEX,
    PORTC_INDEX,
    PORTD_INDEX,
    PORTE_INDEX
}port_index_t;

typedef struct{
    uint8 port      :3;     /* @ref port_index_t */
    uint8 pin       :3;     /* @ref pin_index_t  */
    uint8 direction :1;     /* @ref direction_t  */
    uint8 logic     :1;     /* @ref logic_t      */
}pin_config_t;

/* Function Declarations */
Std_ReturnType gpio_pin_direction_intialize(const pin_config_t *_pin_config);
Std_ReturnType gpio_pin_write_logic(const pin_config_t *_pin_config, logic_t logic);
Std_ReturnType gpio_pin_read_logic(const pin_config_t *_pin_config, logic_t *logic);
Std_ReturnType gpio_pin_intialize(const pin_config_t *_pin_config);

#endif	/* HAL_GPIO_H */

