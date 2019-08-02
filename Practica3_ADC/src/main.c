/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "S32K144.h" /* include peripheral declarations S32K144 */
#define resolution 255		//8Bits ADC
 uint8_t resultADC;

void ADC0_IRQHandler(void)
{
	resultADC =  ADC0->R[0];		//Read ADC result, this process clears the flag automatically
	//ADC0->SC1[0] |= (1<<6); // Habilitar interrupciones
	DisplayLeds();

	ADC0->SC1[0] |= (1<<6);		//Interrupts are disable once a interruption is generated, then enable it

}
void DisplayLeds(void)
{
	if((resultADC >= 0) && (resultADC < 64))
		{
			PTD->PSOR = (1<<15) + 1 + (1<<16);				//turn off all leds, voltage 0-1.25V

		}
		else if((resultADC >= 64) && (resultADC < 128))		//, 1.25-2.5V
		{
			PTD->PSOR = (1<<15)+(1<<16);					//turn off
			PTD->PCOR = 1;									//turn on Blue
		}
		else if((resultADC >= 128) && (resultADC < 191))	//2.5-3.7V
		{
			PTD->PSOR=(1<<15)+1;							//turn off all leds
			PTD->PCOR=(1<<16);								//turn on Green
		}
		else
		{
			PTD->PSOR=(1<<16)+1;	//turn off
			PTD->PCOR=(1<<15);	//turn on Red
		}
}




int main(void)

{	//Enable FIRCCLOCK for adc conversions
	SCG->FIRCDIV = (1<<8);						//Enable FIRC TIMER with divider 1
	//LED -PORTD, POT PORT C
	PCC->PCCn[PCC_PORTD_INDEX] |= (1<<30);		//Activa Clock Gate Control, para habilitar el reloj para el periferico (PUERTO D)
	PCC->PCCn[PCC_PORTC_INDEX] |= (1<<30);		//Activa clock para entradas (PUERTO C)

	PCC->PCCn[PCC_ADC0_INDEX] |= (3<<24); 		//Select clock FIRC 48Mhz
	PCC->PCCn[PCC_ADC0_INDEX] |= (1<<30);		//Enables clock for the ADC
	//ADC0->CFG1 |= (1<<5);							// , default 8-bit conv, FIRC/2
	ADC0->SC1[0] = (1<<6)+ (0b1100);				//Enable interrupts, select Channel 12(Potentiometer)
	S32_NVIC->ISER[1] = (1<<(39%32));			//ISER(INTERRUPT SERVICE ENABLE ) NVIC, interruptions vector of ARM
												// activates interruptions in PORTC(occupies id 61 of vector NVIC, that is divided in 4 sub-vectors(32 each) )
												//enables in ISER[1] because id is 39

	//PCR0,15,16 pins of Leds RGB

	//Set bit 8 (001 of MUX[bits 10-8] configures pin as GPIO)
	PORTD->PCR[0] = (1<<8) ;
	PORTD->PCR[15] = (1<<8) ;
	PORTD->PCR[16] = (1<<8) ;
	PTD->PDDR |= (1<<16) + (1<<15)+(1); 	//Declare pins of Led RGB as Outputs

	PTD->PSOR=(1<<16)+(1<<15)+1;			//Leds are initially turned off

        for(;;) {

        }


	return 0;
}
