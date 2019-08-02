/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "S32K144.h" /* include peripheral declarations S32K144 */

static int selectLed = 0;

void LPTMR0_IRQHandler(void)		//Timer has reached out #of counts
{
	//Since the timer is the unique way to generate interruption this if is not needed
	// if(LPTMR0->CSR&(1<<7)  )				//If overflow and interrupts are true (pag 1405) mascara para leer dichos pits
	if(selectLed == 0)
	{
		PTD-> PSOR = (1<<15);		//OFF led 15
		PTD-> PTOR = (1<<16);		//TOGGLE(on/off) led 16 Toggle operation does not affect other leds
	}
	if(selectLed == 1)
	{
		PTD-> PSOR = (1<<16);		//OFF led 16	,set Operation does not affect other leds
		PTD-> PTOR = (1<<15);		//TOGGLE(on/off) led 15
	}

	LPTMR0->CSR |=(1<<7);		//clear flag while maintaining everything else


}


void PORTC_IRQHandler(void)			//Interrupt from button
{
		if(PORTC->PCR[12]&(1<<24) )			// if Interrupt from PORTC
		{
			if(selectLed == 0)	selectLed = 1;
			else selectLed = 0;
			//PTD-> PDOR = ~(1<<15);		//TOGGLE(on/off) led 15

		}
		PORTC->PCR[12] |= (1<<24);

}



int main(void)
{
	PCC->PCCn[PCC_PORTD_INDEX] = (1<<30);		//Activa Clock Gate Control, Enables system clock in PORTD peripheral
	PCC->PCCn[PCC_PORTC_INDEX] = (1<<30);		//Enables clock for PORTC peripheral

	//To enable LPTMR0 the first step is to enable FIRC or SIRC timer because LPTMR works with that

	PCC->PCCn[PCC_LPTMR0_INDEX] = (1<<30);		//Enables clock GATE for the peripheral LPTMR0
	LPTMR0->PSR = 5;							//no preescaler, clock 1khz(1ms)
	LPTMR0->CMR = 500-1; 						//Counts 500 ms(1khz clock)
	LPTMR0->CSR = 1 +(1<<6);							//Enable low power timer and interrupts

	//Enable interrupts for BUTTON AND LPTMR
	S32_NVIC->ISER[1] |=1<<(61%32);				//ISER(INTERRUPT SERVICE ENABLE ) NVIC, interruptions vector of ARM
	S32_NVIC->ISER[1] |=1<<(58%32);				// activates interruptions in PORTC(occupies id 61 of vector NVIC, that is divided in 4 sub-vectors )
												//it uses ISER[1] because id 61 is in the second sub-vector (32-64)


	//PCR0,15,16 pins of Leds RGB
	//Set bit 8 (001 of MUX[bits 10-8] configures pin as GPIO)
	PORTD->PCR[15] = (1<<8) ;
	PORTD->PCR[16] = (1<<8) ;
	PORTC->PCR[12] = (1<<8) + (9<<16);				// GPIO button SW2 with interrupts, is preferable interrupt in rising edge

	PTD->PDOR=(1<<16)+(1<<15);			//Leds are initially turned off

	PTD->PDDR = (1<<16) + (1<<15); 	//Declare pins of Led RGB as Outputs
	PTC-> PDDR = (0<<12);				//Declare as output pin 12(boton integrado) 0 default
        for(;;) {
        	// unsigned long a=PTC-> PDIR;    //variable para observar en el debugger el valor que manda el boton


        }


	return 0;
}

		//Terminos
//PCC: Peripheral Clock Controler
//LPTMR: Low Power Timer
//FIRC
//CSR: Control Status Register:
//PCR:
//PSR: Prescale Register
