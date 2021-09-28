/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#include "S32K144.h" /* include peripheral declarations S32K144 */
unsigned char message[] = {"Hello world"};
unsigned char position = 0;
void LPUART1_RxTx_IRQHandler (void){
	LPUART1->DATA = message[position];
	position += 1;

	if(message[position]==0){
		LPUART1->CTRL =0;
		position=0;
	}

}

int main(void)
{
	//LPUART module works with FIRC clock, enable
	SCG->FIRCDIV = (3<<8);		//Enable FIRC TIMER with divider 4  	12 MHZ clock
	PCC->PCCn[PCC_PORTC_INDEX] |= (1<<30); 			//Enable PORTC, UART resides here
	PCC->PCCn[PCC_LPUART1_INDEX] |= (3<<24); 		//Select FIRC clock for UART
	PCC->PCCn[PCC_LPUART1_INDEX] |= (1<<30); 		//Enable system Clock for LPUART
	S32_NVIC->ISER[1] |= (1<<(33%32));				//ISER(INTERRUPT SERVICE ENABLE ) NVIC, interruptions vector of ARM
													//Enable interrupts for LPUART
	LPUART1->BAUD =78;								//BaudRate= SystemClock/(Oversampling*SBR)	where SBR is Selected Baud Rate
													//BR= 12Mhz/(16*9600) = 78.125

	PORTC->PCR[6] = (2<<8);		//Pin functionality is set to LPUART RX
	PORTC->PCR[7] = (2<<8);		//Pin functionality is set to LPUART TX
	LPUART1->CTRL |= (1<<23)+ (1<<19);	//Enable Transmitter and  interrupts from LPUART Transmitter

	for(;;) {

	}

	return 0;
}
