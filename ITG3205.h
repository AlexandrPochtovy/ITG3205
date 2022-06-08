/*********************************************************************************
   Original author: Alexandr Pochtovy<alex.mail.prime@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

 * 	ITG3205.h
 *  Created on: Jan 30, 2022
 */

#ifndef ITG3205_H_
#define ITG3205_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "I2C_Master/MyI2C.h"
#include "ITG3205_Register.h"
/* состояние процесса обмена данными с устройством как с отдельным элементом сети
 * 	применяется для отображения состояния процесса работы с устройством для главного кода
 */
typedef enum ITG3205_status_t {//состояние устройства
	ITG_Init,		//устройство не настроено
	ITG_OK,		//устройство готово к опросу
	ITG_Faulth	//устройство неисправно
} ITG3205_status;

/*	состояние обмена данными с устройством, использовать для завершения функции работы с устройством */
typedef enum ITG3205_Connect_Status_t {
	ITG3205_Processing, //выполняется работа с устройством: обмен данными, обработка результатов
	ITG3205_Complite	//работа с устройством завершена, данные считаны/записаны корректно
} ITG3205_Connect_Status;

typedef struct ITG3205_RAW_t {
	int16_t temp;
	int16_t X;
	int16_t Y;
	int16_t Z;
} ITG3205_RAW;



typedef struct ITG3205_data_t {
	float temp;
	float X;
	float Y;
	float Z;
} ITG3205_data;
//common data struct for sensor
typedef struct ITG3205_dev_t {
	const uint8_t addr;
	uint8_t step;
	ITG3205_status status;
	ITG3205_RAW raw;
	ITG3205_data data;
} ITG3205_dev;
//===========================================================================
ITG3205_Connect_Status ITG3205_Init(I2C_Connection *_i2c, ITG3205_dev *dev, uint8_t *pbuffer);
ITG3205_Connect_Status ITG3205_GetData(I2C_Connection *_i2c, ITG3205_dev *dev, uint8_t *pbuffer);


#ifdef __cplusplus
}
#endif
#endif /* ITG3205_H_ */
