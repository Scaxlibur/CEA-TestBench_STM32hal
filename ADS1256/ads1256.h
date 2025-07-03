#ifndef __ADS1256_H__
#define __ADS1256_H__

#include "main.h"
#include "spi.h"
#include "gpio.h"

typedef enum ADS1256_CMD_t{
    ADS1256_CMD_WAKEUP   =  0x00,   // 完成同步并退出待机模式
    ADS1256_CMD_RDATA    =  0x01,   // 读取数据
    ADS1256_CMD_RDATAC   =  0x03,   // 连续读取数据 
    ADS1256_CMD_SDATAC   =  0x0f,   // 停止连续读取数据
    ADS1256_CMD_RREG     =  0x10,   // 从 REG 读取
    ADS1256_CMD_WREG     =  0x50,   // 写入 REG
    ADS1256_CMD_SELFCAL  =  0xf0,   // 失调和增益自校准
    ADS1256_CMD_SELFOCAL =  0xf1,   // 偏移自校准
    ADS1256_CMD_SELFGCAL =  0xf2,   // 增益自校准
    ADS1256_CMD_SYSOCAL  =  0xf3,   // 系统偏移校准
    ADS1256_CMD_SYSGCAL  =  0xf4,   // 系统增益校准
    ADS1256_CMD_SYNC     =  0xfc,   // 同步 A/D 转换
    ADS1256_CMD_STANDBY  =  0xfd,   // 进入待机模式
    ADS1256_CMD_REST     =  0xfe,   // 重置为上电值
}ADS1256_CMD_t;

typedef enum ADS1256_MUXP_t{// define 多路复用 p为正通道，n为负通道
    ADS1256_MUXP_AIN0   =  0x00, 
    ADS1256_MUXP_AIN1   =  0x10, 
    ADS1256_MUXP_AIN2   =  0x20,
    ADS1256_MUXP_AIN3   =  0x30, 
    ADS1256_MUXP_AIN4   =  0x40, 
    ADS1256_MUXP_AIN5   =  0x50, 
    ADS1256_MUXP_AIN6   =  0x60, 
    ADS1256_MUXP_AIN7   =  0x70, 
    ADS1256_MUXP_AINCOM  = 0x80,
}ADS1256_MUXP_t;

typedef enum ADS1256_MUXN_t{// define 多路复用 p为正通道，n为负通道
    ADS1256_MUXN_AIN0 =  0x00, 
    ADS1256_MUXN_AIN1 =  0x01, 
    ADS1256_MUXN_AIN2 =  0x02, 
    ADS1256_MUXN_AIN3 =  0x03, 
    ADS1256_MUXN_AIN4 =  0x04, 
    ADS1256_MUXN_AIN5 =  0x05, 
    ADS1256_MUXN_AIN6 =  0x06, 
    ADS1256_MUXN_AIN7 =  0x07, 
    ADS1256_MUXN_AINCOM  = 0x08, 
}ADS1256_MUXN_t;

typedef enum ADS1256_GAIN_t{// 增益控制-对应满量程输入
    ADS1256_GAIN_1   =   0x00,  // vp = 5V
    ADS1256_GAIN_2   =   0x01,  // vp = 2.5V
    ADS1256_GAIN_4   =   0x02,  // vp = 1.25V
    ADS1256_GAIN_8   =   0x03,  // vp = 0.625V
    ADS1256_GAIN_16  =   0x04,  // vp = 312.5mV
    ADS1256_GAIN_32  =   0x05,  // vp = 156.25mV
    ADS1256_GAIN_64  =   0x06,  // vp = 78.125mV
}ADS1256_GAIN_t;

typedef enum ADS1256_DRATE_t{//AD数据速率
    ADS1256_DRATE_30000SPS = 0xF0, // 30,000次采样/秒（默认）
    ADS1256_DRATE_15000SPS = 0xE0, 
    ADS1256_DRATE_7500SPS =  0xD0,
    ADS1256_DRATE_3750SPS =  0xC0, 
    ADS1256_DRATE_2000SPS =  0xB0, 
    ADS1256_DRATE_1000SPS =  0xA1, 
    ADS1256_DRATE_500SPS  =  0x92, 
    ADS1256_DRATE_100SPS  =  0x82, 
    ADS1256_DRATE_60SPS   =  0x72, 
    ADS1256_DRATE_50SPS   =  0x63, 
    ADS1256_DRATE_30SPS   =  0x53, 
    ADS1256_DRATE_25SPS   =  0x43, 
    ADS1256_DRATE_15SPS   =  0x33, 
    ADS1256_DRATE_10SPS   =  0x23, 
    ADS1256_DRATE_5SPS    =  0x13, 
    ADS1256_DRATE_2_5SPS  =  0x03,
}ADS1256_DRATE_t;

typedef enum ADS1256_CLOCK_t{// 时钟设置
    CLOCK_OFF       = 0x00, // 时钟输出关闭 
    CLOCK_ON        = 0x20, // 时钟输出频率 = fCLKIN（默认） 
    CLOCK_devided_2 = 0x40, // 时钟输出频率 = fCLKIN/2 
    CLOCK_devided_4 = 0x60, // 时钟输出频率 = fCLKIN/4
}ADS1256_CLOCK_t;

typedef enum ADS1256_SENSOR_CURRENT_SOURCE_t{//传感器检测电流源
    SENSOR_OFF  = 0x00, // 传感器检测关闭（默认）
    SENSOR_05U  = 0x8,  // 传感器检测电流 = 0.5μA
    SENSOR_2U   = 0x10, // 传感器检测电流 = 2μA 
    SENSOR_10U  = 0x18, // 传感器检测电流 = 10μA
}ADS1256_SENSOR_CURRENT_SOURCE_t;

#define ADS1256_STATUS      0x00    // 状态寄存器 |7 ID3 |6 ID2 |5 ID1 |4 ID0 |3 ORDER(MSB或LSB) |2 ACAL自动校准（默认禁用）|1 BUFEN模拟输入缓冲（默认禁用） |0 DRDY数据就绪（只读，同DRDY引脚）  

/**
 * 位7-4 PSEL3、PSEL2、PSEL1、PSEL0：正输入通道（AINP）选择 
 * ADS1256_MUXP_t
 */
/**
 * 位3-0 NSEL3、NSEL2、NSEL1、NSEL0：负输入通道（AINN）选择
 * ADS1256_MUXN_t
 */ 
#define ADS1256_MUX         0x01    // 输入多路复用器控制寄存器 |7 PSEL3 |6 PSEL2 |5 PSEL1 |4 PSEL0 |3 NSEL3 |2 NSEL2 |1 NSEL1 |0 NSE |

/**
 * 位7 保留，始终为0（只读）
 * 位6-5 CLK1, CLK0: D0/CLKOUT时钟输出速率设置
 * ADS1256_CLOCK_t 
 * 若不使用CLKOUT，建议将其关闭。这些位只能通过RESET引脚复位。
 */
/**
 * 位4-3 SDCS1、SCDS0：传感器检测电流源
 * ADS1256_SENSOR_CURRENT_SOURCE_t
 * 传感器检测电流源可被激活，用于验证向ADS1255/6提供信号的外部传感器的完整性。短路的传感器会产生非常小的信号，而开路的传感器则会产生非常大的信号。
 */
/**
 * 位2-0 PGA2、PGA1、PGA0：可编程增益放大器设置
 * ADS1256_GAIN_t
 */
#define ADS1256_ADCON       0x02    // A/D控制寄存器 |7 0 |6 CLK1 |5 CLK0 |4 SDCS1 |3 SDCS0 |2 PGA2 |1 PGA1 |0 PGA0 |

/**
 * ADS1256_DRATE_t
 */
#define ADS1256_DRATE       0x03    // A/D读取速率

#define ADS1256_IO          0x04   
#define ADS1256_OFC0        0x05    // OFC0：偏移校准字节0，最低有效字节
#define ADS1256_OFC1        0x06    // OFC1：偏移校准字节1
#define ADS1256_OFC2        0x07    // OFC2：偏移校准字节2
#define ADS1256_FSC0        0x08    // FSC0：全−量程校准字节0，最低有效字节
#define ADS1256_FSC1        0x09    // FSC1：全−量程校准字节1
#define ADS1256_FSC2        0x0A    // FSC2：全−量程校准字节2，最高有效字节

#define CS_0()				HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin,GPIO_PIN_RESET);
#define CS_1()				HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin,GPIO_PIN_SET);
#define ADS1256_DRDY  		(DRDY_GPIO_Port->IDR & DRDY_Pin) //DRDY引脚，
#define ADS1256_MUXN_AINCOM 0x08

void ADS1256WREG(uint8_t regaddr,uint8_t databyte);

void ADS1256_Init(ADS1256_CMD_t _cmd, ADS1256_DRATE_t _drate ,ADS1256_GAIN_t _gain ,ADS1256_CLOCK_t _clock ,ADS1256_SENSOR_CURRENT_SOURCE_t _sensor);

uint32_t ADS1256ReadData(uint8_t channel);

double ADS1256_convert2volt(uint8_t channel);

#endif