/*
 * adc.h
 *
 *  Created on: Jun 23, 2018
 *      Author: hr24612
 */

#ifndef ADC_H_
#define ADC_H_


unsigned int ADC_read(void);
void init_ADC(void);
int getIRAverage(int num);

#endif /* ADC_H_ */
