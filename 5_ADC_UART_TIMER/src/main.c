/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "S32K144.h" /* include peripheral declarations S32K144 */
#define resolution 255		//8Bits ADC
#define mv 19.6
 uint16_t resultADC;

void ADC0_IRQHandler(void)
{
	resultADC =  ADC0->R[0];		//Read ADC result, this process clears the flag automatically
	//ADC0->SC1[0] |= (1<<6); // Habilitar interrupciones
	DisplayUart();



}
void LPTMR0_IRQHandler(void)		//Timer has reached out #of counts
{
	//Since the timer is the unique way to generate interruption this if is not needed
	// if(LPTMR0->CSR&(1<<7)  )				//If overflow and interrupts are true (pag 1405) mascara para leer dichos pits

	ADC0->SC1[0] |= (1<<6);		//Interrupts are disable once a interruption is generated, then enable it
	LPTMR0->CSR |=(1<<7);		//clear flag while maintaining everything else


}
void DisplayUart(void)
{
	LPUART1->DATA = resultADC;
}


int main(void)

{	//Enable FIRCCLOCK for adc conversions
	SCG->FIRCDIV = (3<<8);						//Enable FIRC TIMER with divider 4, 12 MHZ clock
	//LED -PORTD, POT PORT C
	PCC->PCCn[PCC_PORTD_INDEX] |= (1<<30);		//Activa Clock Gate Control, para habilitar el reloj para el periferico (PUERTO D)
	PCC->PCCn[PCC_PORTC_INDEX] |= (1<<30);		//Activa clock para entradas (PUERTO C)
	//LPTMR0 Activation
	PCC->PCCn[PCC_LPTMR0_INDEX] = (1<<30);		//Enables clock GATE for the peripheral LPTMR0
	LPTMR0->PSR = 5;							//no preescaler, clock 1khz(1ms)
	LPTMR0->CMR = 50-1; 						//Counts 50 ms(1khz clock)
	LPTMR0->CSR = 1 +(1<<6);					//Enable low power timer and interrupts
	//ADC Activation
	PCC->PCCn[PCC_ADC0_INDEX] |= (3<<24); 		//Select clock FIRC 48Mhz
	PCC->PCCn[PCC_ADC0_INDEX] |= (1<<30);		//Enables clock for the ADC
	//ADC0->CFG1 |= (1<<5);						// , default 8-bit conv, FIRC/2
	ADC0->SC1[0] = (1<<6)+ (0b1100);			//Enable interrupts, select Channel 12(Potentiometer)


	//Enable interrupts for BUTTON AND LPTMR
	S32_NVIC->ISER[1] |=1<<(61%32);				//ISER(INTERRUPT SERVICE ENABLE ) NVIC, interruptions vector of ARM
	S32_NVIC->ISER[1] |=1<<(58%32);				// activates interruptions in PORTC(occupies id 61 of vector NVIC, that is divided in 4 sub-vectors )
													//it uses ISER[1] because id 61 is in the second sub-vector (32-64)
	S32_NVIC->ISER[1] = (1<<(39%32));			//NVIC ADC0
	S32_NVIC->ISER[1] |= (1<<(33%32));			//Enable interrupts for LPUART
	//PCR0,15,16 pins of Leds RGB

	//Set bit 8 (001 of MUX[bits 10-8] configures pin as GPIO)
	PORTD->PCR[0] = (1<<8) ;
	PORTD->PCR[15] = (1<<8) ;
	PORTD->PCR[16] = (1<<8) ;
	PTD->PDDR |= (1<<16) + (1<<15)+(1); 	//Declare pins of Led RGB as Outputs

	PTD->PSOR=(1<<16)+(1<<15)+1;			//Leds are initially turned off
	//LPUART module works with FIRC clock, enable

		PCC->PCCn[PCC_PORTC_INDEX] |= (1<<30); 			//Enable PORTC, UART resides here
		PCC->PCCn[PCC_LPUART1_INDEX] |= (3<<24); 		//Select FIRC clock for UART
		PCC->PCCn[PCC_LPUART1_INDEX] |= (1<<30); 		//Enable system Clock for LPUART

		LPUART1->BAUD =78;								//BaudRate= SystemClock/(Oversampling*SBR)	where SBR is Selected Baud Rate
														//BR= 12Mhz/(16*9600) = 78.125

		PORTC->PCR[6] = (2<<8);		//Pin functionality is set to LPUART RX
		PORTC->PCR[7] = (2<<8);		//Pin functionality is set to LPUART TX
		LPUART1->CTRL |= (1<<23)+ (1<<19);	//Enable Transmitter and  interrupts from LPUART Transmitter

        for(;;) {

        }


	return 0;
}
