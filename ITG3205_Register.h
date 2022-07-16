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

 * 	ITG3205_Register.h
 *  Created on: Jan 30, 2022
 */

#ifndef ITG3205_REGISTER_H_
#define ITG3205_REGISTER_H_

enum ITG3205_Register {
	ITG3205_WHOAMI 		= 0x00,//ID chip
	ITG3205_SMPLRT_DIV 	= 0x15,//sample rate, simple value
	ITG3205_DLPF_FS 	= 0x16,//scale
	ITG3205_INT_CFG 	= 0x17,//interrupt configuration
	ITG3205_INT_STATUS 	= 0x1A,//interrupt status
	ITG3205_TEMP_OUT_H	= 0x1B,//temperature sensor hi byte
	ITG3205_TEMP_OUT_L 	= 0x1C,//temperature sensor lo byte
	ITG3205_GYRO_XOUT_H = 0x1D,//X-axis data hi byte
	ITG3205_GYRO_XOUT_L = 0x1E,//X-axis data lo byte
	ITG3205_GYRO_YOUT_H = 0x1F,//Y-axis data hi byte
	ITG3205_GYRO_YOUT_L	= 0x20,//Y-axis data lo byte
	ITG3205_GYRO_ZOUT_H = 0x21,//Z-axis data hi byte
	ITG3205_GYRO_ZOUT_L = 0x22,//Z-axis data lo byte
	ITG3205_PWR_MGM 	= 0x3E //power setup
};

enum ITG3205_DLPF_FS {
	ITG3205_DLPF_FS_SEL 	= 0x18,
	ITG3205_DLPF_CFG_256Hz	= 0x00,
	ITG3205_DLPF_CFG_188Hz 	= 0x01,
	ITG3205_DLPF_CFG_98Hz 	= 0x02,
	ITG3205_DLPF_CFG_42Hz 	= 0x03,
	ITG3205_DLPF_CFG_20Hz 	= 0x04,
	ITG3205_DLPF_CFG_10Hz 	= 0x05,
	ITG3205_DLPF_CFG_5Hz 	= 0x06
};

enum ITG3205_INT_CFG {
	ITG3205_INT_CFG_ACTL 							= 0x80,//Logic level for INT output pin – 1=active low, 0=active high
	ITG3205_INT_CFG_OPEN							= 0x40,//Drive type for INT output pin – 1=open drain, 0=push-pull
	ITG3205_INT_CFG_LATCH_INT_EN			= 0x20,//Latch mode – 1=latch until interrupt is cleared, 0=50us pulse
	ITG3205_INT_CFG_INT_ANYRD_2CLEAR	= 0x10,//Latch clear method – 1=any register read, 0=status register read only
	ITG3205_INT_CFG_INT_RDY_EN				= 0x04,//Enable interrupt when device is ready (PLL ready after changing clock source)
	ITG3205_INT_CFG_RAW_RDY_EN				= 0x01//Enable interrupt when data is available
};

enum ITG3205_INT_STATUS {
	ITG3205_STATUS_DEV_RDY 	= 0x04,
	ITG3205_STATUS_DATA_RDY	= 0x01
};

enum ITG3205_PWR_MGM {
	ITG3205_PWR_MGM_RESET		= 0x80,
	ITG3205_PWR_MGM_SLEEP		= 0x40,
	ITG3205_PWR_MGM_STBY_X		= 0x20,
	ITG3205_PWR_MGM_STBY_Y		= 0x10,
	ITG3205_PWR_MGM_STBY_Z		= 0x08,
	ITG3205_PWR_CLOCK_EXT_19MHz	= 0x05,
	ITG3205_PWR_CLOCK_EXT_32kHz	= 0x04,
	ITG3205_PWR_CLOCK_PLL_Z_REF	= 0x03,
	ITG3205_PWR_CLOCK_PLL_Y_REF	= 0x02,
	ITG3205_PWR_CLOCK_PLL_X_REF	= 0x01,
	ITG3205_PWR_CLOCK_INTERNAL	= 0x00,
};
#endif /* ITG3205_REGISTER_H_ */
