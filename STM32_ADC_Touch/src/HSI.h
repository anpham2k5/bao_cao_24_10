#ifndef SYSTEM_CLOCK_HSI_H
#define SYSTEM_CLOCK_HSI_H

#include <Arduino.h>

// Cấu hình HSI 8MHz/2 * PLL 12 = 48MHz, ADC = 12MHz
extern "C" void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  // 1. Cấu hình HSI + PLL: (8MHz / 2) * 12 = 48MHz
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12; 
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  // 2. Bus: AHB = 48MHz, APB1 = 24MHz (DIV2), APB2 = 48MHz (DIV1), Latency = 1 WS
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  // 3. ADC Clock = APB2 / 4 = 48MHz / 4 = 12MHz
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  // 4. Đồng bộ SystemCoreClock và tái tạo SysTick
  SystemCoreClockUpdate();
  HAL_InitTick(TICK_INT_PRIORITY);
}

inline void printSystemClockInfo(Stream &port) {
  SystemCoreClockUpdate();

  port.println("--- THONG SO XUNG NHIP HE THONG ---");
  port.print("SYSCLK : ");
  port.print(SystemCoreClock / 1000000.0, 2);
  port.println(" MHz");

  uint32_t sws = RCC->CFGR & RCC_CFGR_SWS;
  if (sws == RCC_CFGR_SWS_HSI) {
    port.println("Nguon Clock: HSI (Dao dong noi)");
  } else if (sws == RCC_CFGR_SWS_HSE) {
    port.println("Nguon Clock: HSE (Thach anh ngoai)");
  } else if (sws == RCC_CFGR_SWS_PLL) {
    if (RCC->CFGR & RCC_CFGR_PLLSRC) {
      port.println("Nguon Clock: PLL qua HSE");
    } else {
      port.println("Nguon Clock: PLL qua HSI (Dao dong noi 48MHz)");
    }
  }

  uint32_t pclk2 = SystemCoreClock; 
  uint32_t adc_div = (RCC->CFGR & RCC_CFGR_ADCPRE);
  float adc_clock = 0;
  if (adc_div == RCC_CFGR_ADCPRE_DIV2) adc_clock = (pclk2 / 2.0) / 1000000.0;
  else if (adc_div == RCC_CFGR_ADCPRE_DIV4) adc_clock = (pclk2 / 4.0) / 1000000.0;
  else if (adc_div == RCC_CFGR_ADCPRE_DIV6) adc_clock = (pclk2 / 6.0) / 1000000.0;
  else if (adc_div == RCC_CFGR_ADCPRE_DIV8) adc_clock = (pclk2 / 8.0) / 1000000.0;

  port.print("ADC Clock: ");
  port.print(adc_clock, 2);
  port.println(" MHz");
  port.println("------------------------------------");
}

#endif