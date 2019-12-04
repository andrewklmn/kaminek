/*
 * File:   main.c
 * Author: user
 *
 * Created on 6 березня 2017, 22:35
 */
#define _XTAL_FREQ 4000000 

#include <xc.h>
#include <pic16f684.h>
#include "7-segment-x-3.h"
#include "adc_pic16.h"

#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Detect (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      //  Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

#define INPUT RA2   //ADC input 
#define BUTTON RA3   //temp indication enable input 


#define SHOWTIME 250
//#define SHOWTIME 100
#define TEMP_MAX 82
#define TEMP_1 68
#define TEMP_2 64
#define TEMP_3 55
#define TEMP_MIN 36

#define TEMP_STACK_SIZE 2

unsigned int u;
int i = 0;
char value = 0;
int counter = 0;

char temp[TEMP_STACK_SIZE];
char temp_index = 0;
char messured_temp = TEMP_1;
char already_messured = 1;
char temp_going_up = 0;
char change_counter = 0;

int get_temp_value(){
    int result = TEMP_STACK_SIZE/2;
    for (int i=0; i<TEMP_STACK_SIZE; i++) {
        result = result + temp[i];
    };
    return result/TEMP_STACK_SIZE;
};


void main(void) {
    
    ANSEL = 0b00000100;           // AN2 - enabled
    CMCON0 = 0x07;
    
    TRISA = 0b00001100;           //Port A as INPUT to AN2 RA2
    TRISC = 0b00000000;           //Port C is output to all
    
    ADC_Init();                   //Initialize ADC
    
    init_LED();                   // тестирование ЛЕД на полсекунды 888  
    
    
    for (char i=0; i<TEMP_STACK_SIZE; i++) {
        temp[i] = TEMP_1 + 5; 
    };
   
    
    while(1){
        
        // ===================== display temp =================================
        if (value > TEMP_MAX) {
            if (counter<(SHOWTIME/3)) {
               draw_to();
               //draw_hot();
            } else if (counter<(SHOWTIME/3*2)) {
               draw_hot();
               //draw_temp(value);
            } else {
               draw_temp(messured_temp);
            };
        } else if (value > TEMP_MIN) {
            if (temp_going_up==1) {
                if (BUTTON==1) {
                    draw_temp(messured_temp);
                } else {
                    draw_minus();
                };
            } else {
                if (counter<(SHOWTIME/3)) {
                    if (value < TEMP_1) {
                        draw_temp(messured_temp);
                        //draw_empty();
                    } else {
                        if (BUTTON==1) {
                            draw_temp(messured_temp);
                        } else {
                            draw_minus();
                        };
                    };
                } else if(counter<(SHOWTIME/3*2)) {
                    if (value < TEMP_1){
                        draw_add();          
                    } else {
                        if (BUTTON==1) {
                            draw_temp(messured_temp);
                        } else {
                            draw_minus();
                        };
                    };
                } else {
                    if (value < TEMP_3) {
                        draw_needed_amount(3);
                    } else if (value < TEMP_2 ) {
                        draw_needed_amount(2);
                    } else if (value < TEMP_1 ) {
                        draw_needed_amount(1);
                    } else {
                        if (BUTTON==1) {
                            draw_temp(messured_temp);
                        } else {
                            draw_minus();
                        };
                    };
                };
            };
        } else if (value < 10) {
            if (counter<(SHOWTIME/2)) {
                show_LED1(LETTER_E);
                show_LED2(LETTER_E);
                show_LED3(LETTER_E);
            } else {
                draw_temp(messured_temp);
            };
        } else {
            draw_temp(messured_temp);
        };
        counter = (counter>SHOWTIME) ? 0:(counter + 1);
        
        // ============================= get temp =============================
        
        if ( i==0 && already_messured==1 ) { 
            ADC_Start(2);
            already_messured = 0;
            value = (char)get_temp_value();
        };
        
        if (GO_nDONE) {
            
        } else {
            if (already_messured == 0) {
                
                messured_temp = (char)(((ADRESH << 8) + ADRESL)/2.048); //Read ADC 
                
                if (messured_temp > value) {
                    change_counter++;
                    if(change_counter > 3) {
                        temp_going_up = 1;
                        temp[temp_index] = messured_temp;
                        temp_index++;
                        change_counter=0;
                    };
                } else if(messured_temp < value ) {
                    change_counter++;
                    if(change_counter > 3) {
                        temp_going_up = 0;
                        temp[temp_index] = messured_temp;
                        temp_index++;
                        change_counter=0;
                    };
                } else {
                    change_counter=0;
                };
                if (temp_index == TEMP_STACK_SIZE) temp_index=0;
                
                already_messured=1;
            };
        };
        i++;
        
        
        if ( i> SHOWTIME ) {
            i=0;
        };
    };
    return;
};