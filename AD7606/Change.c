#include "stm32f4xx.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

void B_Change()
{
	HAL_GPIO_WritePin(Change_GPIO_Port, Change_Pin,!HAL_GPIO_ReadPin(Change_GPIO_Port, Change_Pin));
}