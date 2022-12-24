/*
 * bme280_spi.h
 *
 *  Created on: Jun 12, 2022
 *      Author: Marcin
 */

#ifndef INC_BME280_SPI_H_
#define INC_BME280_SPI_H_
#define BME_MAX_TIMEOUT 1000
#define	BME280_DIG_T1		0x88
#define	BME280_DIG_T2		0x8A
#define	BME280_DIG_T3		0x8C

#define	BME280_DIG_P1		0x8E
#define	BME280_DIG_P2		0x90
#define	BME280_DIG_P3		0x92
#define	BME280_DIG_P4		0x94
#define	BME280_DIG_P5		0x96
#define	BME280_DIG_P6		0x98
#define	BME280_DIG_P7		0x9A
#define	BME280_DIG_P8		0x9C
#define	BME280_DIG_P9		0x9E

#define	BME280_DIG_H1		0xA1
#define	BME280_DIG_H2		0xE1
#define	BME280_DIG_H3		0xE3
#define	BME280_DIG_H4		0xE4
#define	BME280_DIG_H5		0xE5
#define	BME280_DIG_H6		0xE7
#define	BME280_CHIPID			0xD0
#define	BME280_VERSION			0xD1
#define	BME280_SOFTRESET		0xE0
#define	BME280_CAL26			0xE1  // R calibration stored in 0xE1-0xF0
#define	BME280_HUM_CONTROL		0xF2
#define	BME280_STATUS			0xF3
#define	BME280_CONTROL			0xF4
#define	BME280_CONFIG			0xF5
#define	BME280_PRESSUREDATA		0xF7
#define	BME280_TEMPDATA			0xFA
#define	BME280_HUMIDDATA		0xFD
typedef struct
{
	SPI_HandleTypeDef *bme_spi;
	GPIO_TypeDef *bme280_CsPort;
	uint8_t h1,h3;
	int8_t h6;
	int16_t t2, t3, p2, p3, p4, p5, p6, p7, p8, p9, h2, h4, h5;
	uint16_t t1, p1, bme280_CsPin;
	int32_t t_fine;
}BME280_t;

void BME280_Init(BME280_t* bme,SPI_HandleTypeDef *spi,GPIO_TypeDef *CsPort,uint16_t CsPin);
void BME280_ReadAll(BME280_t *bme, float *T,float *P,float *H);
#endif /* INC_BME280_SPI_H_ */
