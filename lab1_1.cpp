/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
#include <atomic>
#include <cr_section_macros.h>
#define TICKRATE_HZ1 (1000)

static volatile std::atomic_int counter;

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

	void SysTick_Handler(void){
		if(counter > 0) counter--;
	}

#ifdef __cplusplus
}
#endif


	void Sleep(int ms){
		counter = ms;
		while(counter > 0) {
			__WFI();
		}
	}

int main(void) {
	uint32_t sysTickRate;
#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    //Set up the SysTick
    Chip_Clock_SetSysTickClockDiv(1);
    sysTickRate = Chip_Clock_GetSysTickClockRate();
	SysTick_Config(sysTickRate / TICKRATE_HZ1);
	//Set up SW1 here
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 17, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 17);

#endif
#endif

    // Initialize LED pins
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 25);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 3);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 1);

	// Infinite loop that listens to button press
    while(1){
    	//When button is pressed all LEDs turn off then on one by one
    	if(Chip_GPIO_GetPinState(LPC_GPIO, 0,17)){
    		//all of red on
    		LPC_GPIO->B[0][3] = true;
    	    LPC_GPIO->B[1][1] = true;
    	    LPC_GPIO->B[0][25] = false;
    	    Sleep(1000);
    	    //all of green on
    	    LPC_GPIO->B[0][25] = true;
    	    LPC_GPIO->B[1][1] = true;
    		LPC_GPIO->B[0][3] = false;
    	    Sleep(1000);
    	    // all of blue on
    	    LPC_GPIO->B[0][25] = true;
    	    LPC_GPIO->B[0][3] = true;
    	    LPC_GPIO->B[1][1] = false;
    	    Sleep(1000);
    	    //all on
    	    LPC_GPIO->B[0][25] = false;
    	    LPC_GPIO->B[0][3] = false;
    	    LPC_GPIO->B[1][1] = false;

    	 }
    	}

    return 0 ;
}
