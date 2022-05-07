/**
 *  !!! RTC à 1 Hz, HSI 24MHz
 *
	GENESE DU CODE
	- Cube Mx en LL avec un minimum de ressources activées
	- remplacement des 3 fichiers sources SRC et INC par ceux de l'exemple PWR_EnterStandbyMode ST
	- Reprise complète pour adaptation du main. Les configurations spécifiques type Cube Mx sont des copier-coller
	d'un autre projet (rien à voir avec le .ioc de l'actuel projet).
	Chaque conf émanant de cube sont précédées par  "From Cube :"

  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L4xx_LL_Examples
  * @{
  */

/** @addtogroup PWR_EnterStandbyMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUTTON_MODE_GPIO  0
#define BUTTON_MODE_EXTI  1
#define RTC_WUT_TIME 5 // timing wakeup

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t LedSpeed = LED_BLINK_FAST;

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_PWR(void);
void     LED_Init(void);
void     LED_Blinking(uint32_t Period);
void     UserButton_Init(uint32_t Button_Mode);
uint32_t UserButton_GetState(void);
void     EnterStandbyMode(void);

/* Private functions ---------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_RTC_Init(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	  /* From Cube :Reset of all peripherals, Initializes the Flash interface and the Systick. */
	  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	  /* From Cube : Configure the system clock */
	  SystemClock_Config();
	  /* From Cube : Initialize all configured peripherals */
	  MX_RTC_Init();

	  // Allumer LED durant 200ms
	  LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
	  LL_mDelay(200);




	  while(1);

}



/**************************************************************
		 From Cube : Configuration depuis Cube LL
		HSI RC 16MHz -> SYSCLK = 24 MHz
		LSI RC 32kHz

**************************************************************/
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
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 9, LL_RCC_PLLR_DIV_6);
  LL_RCC_PLL_EnableDomain_SYS();
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_Init1msTick(24000000);

  LL_SetSystemCoreClock(24000000);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
}


/**************************************************************
		From Cube : Configuration depuis Cube LL
		La fréquence de la RTC est fCK_SPRE = fRTCCLK /[(PREDIV_S+1)(PREDIV_A+1)]
		cad fCK_SPRE = 32kHz / (128*250) = 1Hz

**************************************************************/
static void MX_RTC_Init(void)
{


  LL_RTC_InitTypeDef RTC_InitStruct = {0};

  /* Peripheral clock enable */
  LL_RCC_EnableRTC();


  /** Initialize RTC and set the Time and Date
  */
  RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
  RTC_InitStruct.AsynchPrescaler = 127;
  RTC_InitStruct.SynchPrescaler = 249;
  LL_RTC_Init(RTC, &RTC_InitStruct);


  /** --------------------------------------------------------------------------
   *  SUITE DE LA CONF INSPIREE de  example LL : RTC_ExitStandbyWithWakeUpTimer
   ---------------------------------------------------------------------------*/
  /* Disable RTC registers write protection */
  LL_RTC_DisableWriteProtection(RTC);
  /* Disable wake up timer to modify it */
  LL_RTC_WAKEUP_Disable(RTC);
  /* Wait until it is allow to modify wake up reload value */
  while (LL_RTC_IsActiveFlag_WUTW(RTC) != 1)
  {
  }
  /* Setting the Wakeup time to RTC_WUT_TIME s
       If LL_RTC_WAKEUPCLOCK_CKSPRE is selected, the frequency is 1Hz,
       this allows to get a wakeup time equal to RTC_WUT_TIME s
       if the counter is RTC_WUT_TIME */
  LL_RTC_WAKEUP_SetAutoReload(RTC, RTC_WUT_TIME);
  LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_CKSPRE);

  /* Enable RTC registers write protection */
  LL_RTC_EnableWriteProtection(RTC);

}



/**************************************************************
		From example LL : RTC_ExitStandbyWithWakeUpTimer
		+ Modification perso

**************************************************************/
void EnterStandbyMode(void)
{
  /* ######## ENABLE WUT #################################################*/
  /* Disable RTC registers write protection */
  LL_RTC_DisableWriteProtection(RTC);

  /* Enable wake up counter and wake up interrupt */
  /* Note: Periodic wakeup interrupt should be enabled to exit the device
     from low-power modes.*/
  LL_RTC_EnableIT_WUT(RTC);
  LL_RTC_WAKEUP_Enable(RTC);

  /* Enable RTC registers write protection */
  LL_RTC_EnableWriteProtection(RTC);

  /* ######## ENTER IN STANDBY MODE ######################################*/
  /** Request to enter STANDBY mode
    * Following procedure describe in STM32L4xx Reference Manual
    * See PWR part, section Low-power modes, Standby mode
    */
  /* Reset Internal Wake up flag */
  LL_RTC_ClearFlag_WUT(RTC);

  /* Check that PWR Internal Wake-up is enabled */
  if (LL_PWR_IsEnabledInternWU() == 0)
  {
    /* Need to enable the Internal Wake-up line */
    LL_PWR_EnableInternWU();
  }

  /* Set Stand-by mode */
  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  LL_LPM_EnableDeepSleep();

  /* Request Wait For Interrupt */
  __WFI();
}



void UserButton_Callback(void)
{

}
