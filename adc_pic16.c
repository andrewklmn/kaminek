#include <xc.h>
#include "adc_pic16.h"

void ADC_Init()
{
  ADCON0 = 0x10000001;               //Turn ON ADC and Clock Selection
  ADCON1 = 0b001;
}

void ADC_Start(unsigned char channel )
{
    if(channel > 7) return;    //Channel range is 0 ~ 7
        
    ADCON0 = 0b10000001;         //Clearing channel selection bits
    ADCON0 |= channel<<2;        //Setting channel selection bits
    
    __delay_ms(3);               //Acquisition time to charge hold capacitor
    GO_nDONE = 1;                //Initializes A/D conversion
    //while(GO_nDONE) NOP();             //Waiting for conversion to complete
    //return ((ADRESH<<8)+ADRESL); //Return result
}
