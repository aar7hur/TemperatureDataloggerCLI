/*
 * DS18B20.h
 *
 *  Created on: Nov 4, 2019
 *      Author: L04347
 *
 *
 *  One wire é um protocolo de comunicação usando uma única linha de dados além do Gnd.
 * Cada dispositivo escravo tem um ID, que é unico e inalteravel. Este serve como o endereço
 * do dispositivo no protocolo One-Wire.
 * One-Wire é usado em ambientes de contatos momentaneos. Quando desconecntado o One-wire entra
 * em modo de reset e quando a tensão volta, o slave acorda e sinaliza sua presença. Dispositivos
 * one-wire sçao os mais economicos meios para adicionar funcionalidades eletronicas a objetos não
 * eletronicos para identificação, autenticaçao etc.
 *
 *
 * DS18B20 é um termometro digital que prove medição de temperatura em celsius de 9 a 12 bits de resolução
 * GPIO_PIN_14 será usado como DATA PIN
 *
 * --> De acordo com o Datasheet, é necessário inicializa-lo todo e para isso é necessário:
 * 1- colocar o pino de dados em 0 por 480us
 * 2- setar o pino como entrada e ler por 15us
 * 3- se o pino esta em 0 significa que o ds18 colocou o pino em nivel logico baixo indicando sua presença
 * 4- se o pino esta em +vcc, significa que o dispositivo não esta conectado corretamente
 *
 *
 *
 * 	Existem alguns endereços responsáveis por determinadas funções no DSB18. Dentre eles:
 *
 * OxCC ----> Usado pelo master para endereçar todos dispositivos sem enviar nenhum codigo de ROM.
 * O master pode solicitar que todos DSB18 no barramento iniciem as conversões de temperatura
 * simultaneamente  enviando o comando 0xCC (skip rom) e depois o comando de conversçao para celsius: 0x44
 *
 * 0x33 ----> Comando de leitura da ROM, somente usado quando só há um DS18B no barramento.
 * Permite ao master ler o codigo da ROM sem sar o processo de procura da ROM. Caso usado com varios
 * sensores no mesmo barramento, uma colisão de dados pode ocorrer.
 *
 * 0x44 ----> Comando inicializa uma unica conversão de temperatura. O resultado e guardado em um
 * registrador de temperatura de 2bytes na scratch-pad memory e o DS18B retorna ao seu estado de low-power.
 *
 * 0xBE ----> Comando permite ao master ler o conteudo do scratchpad. A transferencia de dados começa com o
 * bit menos significativo e continua até o 9byte ser lido.
 *
 * Portanto, para um leitua única, os seguintes passos devem ser seguidos:
 *
 * 0-	Inicializar o biblioteca do delay: DWT_Delay_Init()	e Init_DS18B20()
 * 1-	Chamar a função Write e enviar o endereço da skip ROM (0xCC)
 * 2-	Chamar a função write e enviar o endereço da conversão de temperatura (0x44)
 * 3-	Necessário delay
 * 4-	Incializar novamente o DS18B20
 * 5-	Chamar a função write e enviar o endereço do skip´rom (0xCC)
 * 6-	Chamar a função write e enviar o endereço do read scrathpad (0xBE)
 * 7-	fazer duas leituras para adquirir os dois bytes da conversão
 * 8-	Fazer o shifting dos bits. Começa pelo menos significativo então a segunda leitura decve ser shiftada <<8
 * 9-	fazer casting do resultado shiftado para floar e dividir por 16.
 */





#ifndef DS18B20_H_
#define DS18B20_H_

#include "main.h"
#include "stm32l4xx_hal.h"


#define SKIP_ROM 				0xCC
#define	CONVERT_TEMPERATURE 	0x44
#define	READ_SCRATCHPAD 		0xBE





void Gpio_set_input (uint16_t pin, GPIO_TypeDef* GPIOx);


/******************FUNÇÃO QUE SETA O GPIO COMO ENTRADA*******************************/


void Gpio_set_output (uint16_t pin, GPIO_TypeDef* GPIOx);



/******************FUNÇÃO QUE SETA O GPIO COMO SAÍDA*******************************/

uint8_t Init_ds18b20 (uint16_t pin, GPIO_TypeDef* GPIOx);

/******************FUNÇÃO QUE INICIALIZA O DS18B20*******************************/

/* De acordo com o datasheet, para inicializar o dispositivo é necessario:
 * 1- Setar o GPIO como saída
 * 2- Colocar o GPIO como low
 * 3- Dar um delay de 480us
 * 4- COlocar o pino como entrada
 * 5- Delay de 80us
 * 6- Ler o GPIO, se GPIO = low, entao o pulso de presença foi recebido e o dispositivo foi conectado corretamente
 */





void Write_ds18b20 (uint8_t data, uint16_t pin, GPIO_TypeDef* GPIOx);





/*******************FUNÇÃO QUE ESCREVE UM DADO NO DS18B20*************************/

/* Para escrever um bit no sensor é necessário fazer algumas operações no barramento de dados.
 * Para escrever um bit 1, após colcoar o barramento do one wire para 0, o master deve segurar
 * por 15us. Quando o barramento é lioberado, o resistor de pull up ira deixar o barramento em +vcc
 *
 * 1- Para escrever '0' é necessário colocar o pino de dados para 0 (setando para saida e para 0)
 * e dar um delay de 60us.
 * 2- Para escrever '1' é necessário coloccar o pino de dados para 0 (setando para saida e para 0),
 * dar um delay de no minimo 1us e libera-lo setando o pino como entrada.
 *
 *
 */

uint8_t Read_ds18b20 (uint16_t pin, GPIO_TypeDef* GPIOx);


/*******************FUNÇÃO QUE LÊ OS DADOS DO DS18B20************************

/*
 * Para ler é necessário que o master coloque o barramento one wire em 0 por no minimo 1us e depois libere o barramento
 * Depois de iniciar a leitura no tempo, o ds18 irá transmitir 1 ou 0 no barramento.1 se o barramento estiver em +vcc, 0 se estive rno GND.
 * Quando transmitindo, o sensor irá liberar o barramento até o fim do do tempo, e o barramento será colocado de volta ao nivel logico alto através
 * do resistor de pullup.
 * 1- Colcoar o pino de dados em 0 (setar para saída e para 0), colocar delay de no minimo 1uus e entao setar como entrada para ler.
 * 2- Se o pino for 1, significa que transmitiu 1,s e não: 0.
 * 3- Todos ciclos de leitura devem ser de no mínimo 60us.
 */






#endif /* DS18B20_H_ */
