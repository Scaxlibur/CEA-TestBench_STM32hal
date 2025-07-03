#include "ads1256.h"

uint8_t wreg = 0x50;
uint8_t regnumber = 0x00;
uint8_t IO = 0x00;

ADS1256_CMD_t cmd = ADS1256_CMD_SELFCAL;				// 启动失调和增益自校准
ADS1256_DRATE_t drate = ADS1256_DRATE_10SPS; 			// 10k采样率
ADS1256_GAIN_t gain = ADS1256_GAIN_1;					// 正负5V最大值
ADS1256_CLOCK_t clock = CLOCK_ON;						// 时钟不分频输出
ADS1256_SENSOR_CURRENT_SOURCE_t sensor = SENSOR_OFF;	// 关闭传感器检测电流源

void ADS1256WREG(uint8_t regaddr, uint8_t databyte)
{
	wreg += (regaddr & 0x0F);    
	//确保前四位为0101，同时后四位为regaddr的值

	CS_0();
	while(ADS1256_DRDY);

	HAL_SPI_Transmit(&hspi1,(uint8_t*)&wreg,1,100);
	//写入寄存器值时，第一个命令为 0101rrrr,第二个为 0000nnnn
	//同时注意n为需要写入的寄存器减一，写入一个寄存器，所以写入的为0x00
	//printf("%d\r\n",(uint8_t*)(ADS1256_CMD_WREG | (regaddr & 0x0F)));
	//写入一个寄存器
	HAL_SPI_Transmit(&hspi1,(uint8_t*)&regnumber,1,100);

	HAL_SPI_Transmit(&hspi1,(uint8_t*)&databyte,1,100);
	CS_1();
	wreg = 0x50;
}

void ADS1256_Init(ADS1256_CMD_t _cmd, ADS1256_DRATE_t _drate ,ADS1256_GAIN_t _gain ,ADS1256_CLOCK_t _clock ,ADS1256_SENSOR_CURRENT_SOURCE_t _sensor)
{
	//初始化过程参考手册
	cmd = _cmd;				// 启动失调和增益自校准
	drate = _drate; 			// 10k采样率
	gain = _gain;					// 正负5V最大值
	clock = _clock;						// 时钟不分频输出
	sensor = _sensor;	// 关闭传感器检测电流源
  	while(ADS1256_DRDY);
	CS_0();
	HAL_SPI_Transmit(&hspi1,(uint8_t *)&cmd,1,100);
	while(ADS1256_DRDY);
	CS_1();
	//**********************************

	ADS1256WREG(ADS1256_STATUS,0b00000110);	// 启用自校准和模拟输入缓冲区
	// ADS1256WREG(ADS1256_STATUS,0x04);
	// ADS1256WREG(ADS1256_MUX,0x08);
	ADS1256WREG(ADS1256_ADCON,gain | clock | sensor);
	ADS1256WREG(ADS1256_DRATE,drate);  
	ADS1256WREG(ADS1256_IO,IO);               
	while(ADS1256_DRDY);
	CS_0();
	HAL_SPI_Transmit(&hspi1,(uint8_t *)&cmd,1,100);
	while(ADS1256_DRDY);
	CS_1(); 
	//**********************************
}

uint32_t ADS1256ReadData(uint8_t channel)  
{
	uint8_t data1=0;
	uint8_t data2=0;
	uint8_t data3=0;
	uint32_t sum=0;
	ADS_1256_CMD_t cmd1 = ADS1256_CMD_SYNC;
	ADS_1256_CMD_t cmd2 = ADS1256_CMD_WAKEUP;
	ADS_1256_CMD_t cmd3 = ADS1256_CMD_RDATA;
	//uint8_t rdata[3];
	while(ADS1256_DRDY);//
	ADS1256WREG(ADS1256_MUX,channel);
	CS_0();
	//读数据前依次写入SYNC、WAKEUP、RDATA
	HAL_SPI_Transmit(&hspi1,(uint8_t *)&cmd1,1,100);
	HAL_SPI_Transmit(&hspi1,(uint8_t *)&cmd2,1,100);	               
	HAL_SPI_Transmit(&hspi1,(uint8_t *)&cmd3,1,100);
	//接收数据依次存入data中
  	HAL_SPI_TransmitReceive(&hspi1,(uint8_t *)0xff,(uint8_t *)&data1,1,100);
	HAL_SPI_TransmitReceive(&hspi1,(uint8_t *)0xff,(uint8_t *)&data2,1,100);
	HAL_SPI_TransmitReceive(&hspi1,(uint8_t *)0xff,(uint8_t *)&data3,1,100);
	sum |= (data1 <<16);
	sum |= (data2 <<8);
	sum |= data3;
	CS_1();

	if (sum>0x7FFFFF)           // if MSB=1, 
	{
		sum -= 0x1000000;       // do 2's complement
	}
    return sum;
}