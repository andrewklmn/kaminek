/* 
 * File:   adc_pic16.h
 * Author: user
 *
 * Created on 6 березня 2017, 23:53
 */

#define _XTAL_FREQ 4000000    // частота генератора - ПОПРАВИТЬ!!!

#ifndef ADC_PIC16_H
#define	ADC_PIC16_H

void ADC_Init(void);
void ADC_Start(unsigned char);

#endif	/* ADC_PIC16_H */

