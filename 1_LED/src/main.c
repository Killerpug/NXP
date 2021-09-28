/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#include "S32K144.h" /* include peripheral declarations S32K144 */
void delay (void)		//Delay chafa
{
	unsigned long i=10000000;
	do{}while (--i);

}


int main(void)
{
	unsigned int counter = 0;
	unsigned int arreglo[3] ={~1,~(1<<15),~(1<<16)};	 //niega la asignacion pasada para prender ~ apagar el LED RGB

	PCC->PCCn[PCC_PORTD_INDEX] = (1<<30);		//Activa Clock Gate Control, para habilitar el reloj para el periferico (PUERTO D)
	PCC->PCCn[PCC_PORTC_INDEX] = (1<<30);		//Activa clock para entradas (PUERTO C)

	//PCR0,15,16 son las entradas del led RGB
	PORTD->PCR[0] =(1<<8) ;		//Settear el bit 8 (001 en el MUX configura el pin como GPIO)
	PORTD->PCR[15] =(1<<8) ;
	PORTD->PCR[16] =(1<<8) ;

	PORTC->PCR[12] = (1<<8);	//settea el bit 8 del puerto par aconfigurar el pin 12 como GPIO
	PTD->PDOR= (1<<16)+(1<<15)+1;	//Apagar pines del led
	PTD->PDDR = (1<<16) + (1<<15) +(1) ; 	//Declarar como salidas pines del LED RGB
	for(;;) {

		//PTD-> PDOR = 0;
		PTD-> PDOR = arreglo[counter%3];		//prende el led acorde al arreglo
		delay();
		PTD->PDOR=(1<<16)+(1<<15)+1;	//apagar todos los leds

		 counter++;
	}

	return 0;
}
