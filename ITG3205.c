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

 * 	ITG3205.c
 *  Created on: Jan 30, 2022
 */

#include "ITG3205.h"

static const float Chip_GYRO_LSB = 14.375;
static const uint16_t Chip_TEMP_LSB = 280;

/*static inline CONCAT_BYTES(uint8_t msb, uint8_t lsb) {
	return (uint16_t)(((uint16_t)msb << 8) | (uint16_t)lsb);
}*/

uint8_t ITG3205_Init(I2C_Connection *_i2c, ITG3205_t *dev) {
	uint8_t dt[3];
	if (_i2c->status == PORT_FREE) {//send setup
        _i2c->addr = dev->addr;
        _i2c->mode = I2C_MODE_WRITE;
        switch (dev->step) {
		case 0://reset first
			dev->status = INIT;
			_i2c->reg = ITG3205_PWR_MGM;
			_i2c->len = 1;
            PutOne(&_i2c->buffer, ITG3205_PWR_MGM_RESET);
			dev->step = 1;
			break;
		case 1://setup clock
			_i2c->reg = ITG3205_PWR_MGM;
			_i2c->len = 1;
			PutOne(&_i2c->buffer, ITG3205_PWR_CLOCK_INTERNAL);
			dev->step = 2;
			break;
		case 2:{//setup sample rate, interrupt
			_i2c->reg = ITG3205_SMPLRT_DIV;
			_i2c->len = 3;
			dt[0] = 0xFF;//ITG3205_SMPLRT_DIV
			dt[1] = ITG3205_DLPF_FS_SEL | ITG3205_DLPF_CFG_256Hz;
			dt[2] = ITG3205_INT_CFG_INT_ANYRD_2CLEAR ;
			PutMulti(&_i2c->buffer, dt, 3);
			dev->step = 3;
			break;}
		case 3:
			dev->status = OK;
			dev->step = 0;
			return 1;
			break;
		default:
			dev->step = 0;
      break;
		}
        I2C_Start_IRQ(_i2c);
	}
	return 0;
}

uint8_t ITG3205_GetData(I2C_Connection *_i2c, ITG3205_t *dev) {
	uint8_t dt[8];
	if (_i2c->status == PORT_FREE) {
        if (dev->step == 0) {
        	_i2c->addr = dev->addr;
        	_i2c->reg = ITG3205_TEMP_OUT_H;
        	_i2c->len = 8;
        	_i2c->mode = I2C_MODE_READ;
        	dev->step = 1;
        } else if (dev->step == 1) {
        	GetMulti(&_i2c->buffer, dt, 8);
			dev->raw.temp = (int16_t)(((uint16_t)dt[0])<< 8 | (uint16_t)dt[1]);//CONCAT_BYTES(dt[0], dt[1]);
        	dev->raw.X = (int16_t)(((uint16_t)dt[2])<< 8 | (uint16_t)dt[3]);//CONCAT_BYTES(dt[2], dt[3]);
        	dev->raw.Y = (int16_t)(((uint16_t)dt[4])<< 8 | (uint16_t)dt[5]);//CONCAT_BYTES(dt[4], dt[5]);
        	dev->raw.Z = (int16_t)(((uint16_t)dt[6])<< 8 | (uint16_t)dt[7]);//CONCAT_BYTES(dt[6], dt[7]);
        	dev->data.temp = 35 + ((float)(dev->raw.temp + 13200)) / Chip_TEMP_LSB;
        	dev->data.X = (float)dev->raw.X / Chip_GYRO_LSB * M_PI / 180;
        	dev->data.Y = (float)dev->raw.Y / Chip_GYRO_LSB * M_PI / 180;
        	dev->data.Z = (float)dev->raw.Z / Chip_GYRO_LSB * M_PI / 180;
          dev->step = 0;
          return 1;
        }
		I2C_Start_IRQ(_i2c);
	}
	return 0;
}
