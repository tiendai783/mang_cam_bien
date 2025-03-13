#include "stm32f10x.h"
#include "dht11.h"
#include "timer.h"
#include "stdio.h"
#include "stm32f10x_usart.h"

void UART_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    USART_Init(USART1, &USART_InitStruct);
    USART_Cmd(USART1, ENABLE);
}

void UART_SendChar(char c) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
}

void UART_SendString(char *str) {
    while (*str) {
        UART_SendChar(*str++);
    }
}

void UART_SendNumber(uint8_t number) {
    char buffer[10]; 
    sprintf(buffer, "%d", number); 
    UART_SendString(buffer); 
}

int main() {
    UART_Init();
    timer2_Init();
    uint8_t humidity, temperature;
    while (1) {
        DHT11_Init(&temperature, &humidity);
        UART_SendString("Temperature: ");
        UART_SendNumber(temperature);
        UART_SendString("*C\n");
        UART_SendString("Humidity: ");
        UART_SendNumber(humidity);
        UART_SendString("%\n");
        Delay_1ms(1000);
    }
}