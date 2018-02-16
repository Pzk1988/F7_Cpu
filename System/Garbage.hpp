#ifndef GARBAGE_HPP
#define GARBAGE_HPP

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void UdpWrite(const char *ptr, int len);

void _Error_Handler(const char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

#endif
