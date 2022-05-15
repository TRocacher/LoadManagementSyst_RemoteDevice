
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define RTC_WUT_TIME 10 // timing wakeup



/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void My_RTC_Init(void);

void My_EnterStandbyMode(void);


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Init clock */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  SystemClock_Config();


  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  My_RTC_Init();





  /* Gestion des modes */

  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);

  if ( ((RCC->CSR&RCC_CSR_PINRSTF) == RCC_CSR_PINRSTF) && ((PWR->CSR&PWR_CSR_SBF) != PWR_CSR_SBF))
  {
	  // reset reset flag
	  RCC->CSR|=RCC_CSR_RMVF;
	  // Allumer LED durant 6s
	  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
	  LL_mDelay(6000);
  }
  else if ( ((RCC->CSR&RCC_CSR_PINRSTF) == RCC_CSR_PINRSTF) && ((PWR->CSR&PWR_CSR_SBF) == PWR_CSR_SBF))
   // nRST from stdby mode PINRST =1, SBF=1
  {
	  // reset reset flag
	  RCC->CSR|=RCC_CSR_RMVF;
	  // clear SBF
	  PWR->CR|=PWR_CR_CSBF;
	  // Allumer LED durant 3s
	  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
	  LL_mDelay(3000);
  }

  else // retour d'un standby mode par le WUTF  // PINRST =0, SBF=1
  // Allumer LED durant 1s
  {
	  // clear SBF
	  PWR->CR|=PWR_CR_CSBF;
	  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
	  LL_mDelay(1000);
  }

  My_EnterStandbyMode();

	  // bug entrée Standby LED on
  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);


  while (1)
  {

  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_1)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1)
  {

  }
  LL_PWR_EnableBkUpAccess();
  if(LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSI)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
  }
  LL_RCC_EnableRTC();
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_3, LL_RCC_PLL_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }

  LL_Init1msTick(24000000);

  LL_SetSystemCoreClock(24000000);
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void My_RTC_Init(void)
{
  /* PREREQUIS
  On entre en supposant :
  PWR enable (main)
  LSI bien sélectionné en entrée RTC (fait dans SystemClock_Config)
  */

  /*
  OBJECTIF :
   Configurer l'ensemble du WUT (prescaler, etc...)
   A la fin, le WUT est disable.
   Il ne sera validé que lors de la mise en sommeil

   Au démarrage et à la fin on permet puis interdit l'accès (DBP - system reset et 0xCA pui 0x53 - bkp domain reset)
   */


  // levée de la protection après syst reset (DBP=1)
  LL_PWR_EnableBkUpAccess();
  //déverrouiller après backup domain reset
  LL_RCC_EnableRTC();
  LL_RTC_DisableWriteProtection(RTC);


  //Désactiver WUT (notamment pour pouvoir écrire dans WUTR)
  LL_RTC_WAKEUP_Disable(RTC);


  // réglage des prescalers
  LL_RTC_SetAsynchPrescaler(RTC, 127);
  LL_RTC_SetSynchPrescaler(RTC, 296);
  // sélectionner l'horloge RTC (par exemple LSI Valeur 37KHz)
  LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_CKSPRE);
  // Attendre l'autorisation d'écriture dans WUTR
  while (LL_RTC_IsActiveFlag_WUTW(RTC) != 1)
  {
  }
  LL_RTC_WAKEUP_SetAutoReload(RTC, RTC_WUT_TIME);
  // Autoriser le flag de sortie WUTF (WUTIE=1)
  LL_RTC_EnableIT_WUT(RTC);

  // A CE STADE LE WUT EST CONFIGURE, AUTORELOAD CHARGE. MANQUE PLUS QU'A LANCER WUT (LL_RTC_WAKEUP_Enable(RTC);)

  // Verrouiller backup domain
  // levée de la protection après syst reset (DBP=1)
  LL_PWR_DisableBkUpAccess();
  //déverrouiller après backup domain reset
  LL_RTC_EnableWriteProtection(RTC);
}

void My_EnterStandbyMode(void)
{
	  /* PREREQUIS
	  On entre en supposant :
	  PWR enable (fait dans main)
	  RTC enable ( fais dans My_RTC_Init)
	  */


	  /*
	  OBJECTIF :
	  Enchainer les actions pour mise en stby
	  -SLEEPDEEP core =1 , PDDS =1 (standby et pas stop)
	  - mettre WUF à 0 (flag du PWR CSR)
	  - Effacer le flag WUT
	  - Lancer le WUT
	  - WFI

	   Au démarrage et à la fin on permet puis interdit l'accès (DBP - system reset et 0xCA pui 0x53 - bkp domain reset)
	   */


  //activer le bit SLEEPDEEP (SCB → SCR),
  LL_LPM_EnableDeepSleep();
  // Activer le bit PDDS(PWR -> CR) pour être en standby mode qd deepsleep
  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);

  // Effacer WUF dans PWR->CSR
  LL_PWR_ClearFlag_WU();


  // levée de la protection après syst reset (DBP=1)
  LL_PWR_EnableBkUpAccess();
  //déverrouiller après backup domain reset
  LL_RTC_DisableWriteProtection(RTC);
  /* Reset Internal Wake up flag */
  LL_RTC_ClearFlag_WUT(RTC); // codage un peu curieux...
  // lancement WUT
  LL_RTC_WAKEUP_Enable(RTC);
  // Verrouiller backup domain
  // levée de la protection après syst reset (DBP=1)
  LL_PWR_DisableBkUpAccess();
  //déverrouiller après backup domain reset
  LL_RTC_EnableWriteProtection(RTC);


  /* Request Wait For Interrupt */
  __WFI();

}






/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
