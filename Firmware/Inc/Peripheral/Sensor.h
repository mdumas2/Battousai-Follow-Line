#ifndef __SENSOR_H
#define __SENSOR_H

#include "adc.h"

///3.3 / 1024.0 * ( 10000.0 + 3300.0 ) / 3300.0
#define BATT_CONVERTION_CONST 0.01298828125f

void Adc2Start(void);
float GetBatteryVoltage(void);
void Adc1Start(void);
void Adc1Dma2StartConvertion(void);
void AdcStartConvertion(void);
void AdcStopConvertion(void);

float SensingLineSensor(void);
void LineSensorInit(void);
void CalibrateSensor(void);
void GetSensorCalibrated(void);
void UpdateLineSensor(void);

void CheckAdcProcess(void);
void Adc1Dma2TransferCompleteCallback(void);
void ShowAdc(void); //IMPLEMENTAR

#endif /* __SENSOR_H */
