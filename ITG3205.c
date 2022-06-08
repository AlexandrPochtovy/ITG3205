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

const uint8_t ITG3205_Chip_ID = 0xD0;
static const float Chip_GYRO_LSB = 14.375;
static const uint16_t Chip_TEMP_LSB = 280;

#define ITG3205_CONCAT_BYTES(msb, lsb)	(((int16_t)msb << 8) | lsb)

ITG3205_Connect_Status ITG3205_Init(I2C_Connection *_i2c, ITG3205_dev *dev, uint8_t *pbuffer) {
	if (_i2c->i2cStatus == I2C_Bus_Free) {//send setup
        _i2c->addr = dev->addr;
        _i2c->mode = I2C_MODE_WRITE;
        _i2c->rxtxp = pbuffer;
        switch (dev->step) {
		case 0://reset first
			dev->status = ITG_Init;
			_i2c->reg = ITG3205_PWR_MGM;
			_i2c->len = 1;
            _i2c->rxtxp[0] = ITG3205_PWR_MGM_RESET;
			dev->step = 1;
			break;

		case 1://setup clock
			_i2c->reg = ITG3205_PWR_MGM;
			_i2c->len = 1;
            _i2c->rxtxp[0] = ITG3205_PWR_CLOCK_INTERNAL;
			dev->step = 2;
			break;

		case 2://setup 	sample rate, interrupt
			_i2c->reg = ITG3205_SMPLRT_DIV;
			_i2c->len = 3;
			_i2c->rxtxp[0] = 0x0F;//ITG3205_SMPLRT_DIV
      _i2c->rxtxp[1] = ITG3205_DLPF_FS_SEL | ITG3205_DLPF_CFG_256Hz;
      _i2c->rxtxp[2] = ITG3205_INT_CFG_LATCH_INT_EN | ITG3205_INT_CFG_INT_RDY_EN | ITG3205_INT_CFG_RAW_RDY_EN;
			dev->step = 3;
			break;

		case 3:
			dev->status = ITG_OK;
      dev->step = 0;
			return ITG3205_Complite;
			break;
		default:
			dev->step = 0;
      break;
		}
        I2C_Start_IRQ(_i2c);
	}
	return ITG3205_Processing;
}

ITG3205_Connect_Status ITG3205_GetData(I2C_Connection *_i2c, ITG3205_dev *dev, uint8_t *pbuffer) {
	if (_i2c->i2cStatus == I2C_Bus_Free) {
        _i2c->rxtxp = pbuffer;
        if (dev->step == 0) {
        	_i2c->addr = dev->addr;
          _i2c->reg = ITG3205_TEMP_OUT_H;
          _i2c->len = 8;
          _i2c->mode = I2C_MODE_READ;
          dev->step = 1;
        } 
				else if (dev->step == 1) {
					dev->raw.temp = (int16_t)ITG3205_CONCAT_BYTES(_i2c->rxtxp[0], _i2c->rxtxp[1]);
        	dev->raw.X = ITG3205_CONCAT_BYTES(_i2c->rxtxp[2], _i2c->rxtxp[3]);
        	dev->raw.Y = ITG3205_CONCAT_BYTES(_i2c->rxtxp[4], _i2c->rxtxp[5]);
        	dev->raw.Z = ITG3205_CONCAT_BYTES(_i2c->rxtxp[6], _i2c->rxtxp[7]);
        	dev->data.temp = 35 + ((float)(dev->raw.temp + 13200)) / Chip_TEMP_LSB;
        	dev->data.X = (float)dev->raw.X / Chip_GYRO_LSB;
        	dev->data.Y = (float)dev->raw.Y / Chip_GYRO_LSB;
        	dev->data.Z = (float)dev->raw.Z / Chip_GYRO_LSB;
          dev->step = 0;
          return ITG3205_Complite;
        }
				I2C_Start_IRQ(_i2c);
	}
	return ITG3205_Processing;
}
