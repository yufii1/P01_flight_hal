#ifndef INT_LED_H
#define INT_LED_H

#include "main.h"

typedef struct 
{
    GPIO_TypeDef *GPIOX;
    uint16_t GPIO_Pin;
}LED_Struct;

void LED_turn_on(LED_Struct *led);
void LED_turn_off(LED_Struct *led);

void Int_led_toggle(LED_Struct *led);

#endif /* INT_LED_H */
