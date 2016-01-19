/**
  ******************************************************************************
  * @file    stm32f4xx_hal_tim_ex.h
  * @author  MCD Application Team
  * @version V1.1.0RC2
  * @date    14-May-2014
  * @brief   Header file of TIM HAL Extension module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_HAL_TIM_EX_H
#define __STM32F4xx_HAL_TIM_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"

/** @addtogroup STM32F4xx_HAL
  * @{
  */

/** @addtogroup TIMEx
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/ 

/** 
  * @brief  TIM Hall sensor Configuration Structure definition  
  */

typedef struct
{
                                  
  uint32_t IC1Polarity;            /*!< Specifies the active edge of the input signal.
                                        This parameter can be a value of @ref TIM_Input_Capture_Polarity */
                                                                   
  uint32_t IC1Prescaler;        /*!< Specifies the Input Capture Prescaler.
                                     This parameter can be a value of @ref TIM_Input_Capture_Prescaler */
                                  
  uint32_t IC1Filter;           /*!< Specifies the input capture filter.
                                     This parameter can be a number between Min_Data = 0x0 and Max_Data = 0xF */  
  uint32_t Commutation_Delay;  /*!< Specifies the pulse value to be loaded into the Capture Compare Register. 
                                    This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF */                              
} TIM_HallSensor_InitTypeDef;

/** 
  * @brief  TIM Master configuration Structure definition  
  */ 
typedef struct {
  uint32_t  MasterOutputTrigger;   /*!< Trigger output (TRGO) selection. 
                                      This parameter can be a value of @ref TIMEx_Master_Mode_Selection */ 
  uint32_t  MasterSlaveMode;       /*!< Master/slave mode selection. 
                                      This parameter can be a value of @ref TIMEx_Master_Slave_Mode */
}TIM_MasterConfigTypeDef;

/** 
  * @brief  TIM Break and Dead time configuration Structure definition  
  */ 
typedef struct
{
  uint32_t OffStateRunMode;	        /*!< TIM off state in run mode.
                                         This parameter can be a value of @ref TIMEx_OSSR_Off_State_Selection_for_Run_mode_state */
  uint32_t OffStateIDLEMode;	      /*!< TIM off state in IDLE mode.
                                         This parameter can be a value of @ref TIMEx_OSSI_Off_State_Selection_for_Idle_mode_state */
  uint32_t LockLevel;	 	            /*!< TIM Lock level.
                                         This parameter can be a value of @ref TIMEx_Lock_level */                             
  uint32_t DeadTime;	 	            /*!< TIM dead Time. 
                                         This parameter can be a number between Min_Data = 0x00 and Max_Data = 0xFF */
  uint32_t BreakState;	 	          /*!< TIM Break State. 
                                         This parameter can be a value of @ref TIMEx_Break_Input_enable_disable */
  uint32_t BreakPolarity;	 	        /*!< TIM Break input polarity. 
                                         This parameter can be a value of @ref TIMEx_Break_Polarity */
  uint32_t AutomaticOutput;	 	      /*!< TIM Automatic Output Enable state. 
                                         This parameter can be a value of @ref TIMEx_AOE_Bit_Set_Reset */           
}TIM_BreakDeadTimeConfigTypeDef;

/* Exported constants --------------------------------------------------------*/
/** @defgroup TIMEx_Exported_Constants
  * @{
  */
/** @defgroup TIMEx_OSSR_Off_State_Selection_for_Run_mode_state
  * @{
  */  
#define TIM_OSSR_ENABLE 	      (TIM_BDTR_OSSR)
#define TIM_OSSR_DISABLE              ((uint32_t)0x0000)

#define IS_TIM_OSSR_STATE(STATE) (((STATE) == TIM_OSSR_ENABLE) || \
                                  ((STATE) == TIM_OSSR_DISABLE))
/**
  * @}
  */
  
/** @defgroup TIMEx_OSSI_Off_State_Selection_for_Idle_mode_state 
  * @{
  */
#define TIM_OSSI_ENABLE	 	    (TIM_BDTR_OSSI)
#define TIM_OSSI_DISABLE            ((uint32_t)0x0000)

#define IS_TIM_OSSI_STATE(STATE) (((STATE) == TIM_OSSI_ENABLE) || \
                                  ((STATE) == TIM_OSSI_DISABLE))
/**
  * @}
  */
/** @defgroup TIMEx_Lock_level 
  * @{
  */
#define TIM_LOCKLEVEL_OFF	   ((uint32_t)0x0000)
#define TIM_LOCKLEVEL_1            (TIM_BDTR_LOCK_0)
#define TIM_LOCKLEVEL_2            (TIM_BDTR_LOCK_1)
#define TIM_LOCKLEVEL_3            (TIM_BDTR_LOCK)

#define IS_TIM_LOCK_LEVEL(LEVEL) (((LEVEL) == TIM_LOCKLEVEL_OFF) || \
                                  ((LEVEL) == TIM_LOCKLEVEL_1) || \
                                  ((LEVEL) == TIM_LOCKLEVEL_2) || \
                                  ((LEVEL) == TIM_LOCKLEVEL_3)) 
/**
  * @}
  */  
/** @defgroup TIMEx_Break_Input_enable_disable 
  * @{
  */                         
#define TIM_BREAK_ENABLE          (TIM_BDTR_BKE)
#define TIM_BREAK_DISABLE         ((uint32_t)0x0000)

#define IS_TIM_BREAK_STATE(STATE) (((STATE) == TIM_BREAK_ENABLE) || \
                                   ((STATE) == TIM_BREAK_DISABLE))
/**
  * @}
  */
/** @defgroup TIMEx_Break_Polarity 
  * @{
  */
#define TIM_BREAKPOLARITY_LOW        ((uint32_t)0x0000)
#define TIM_BREAKPOLARITY_HIGH       (TIM_BDTR_BKP)

#define IS_TIM_BREAK_POLARITY(POLARITY) (((POLARITY) == TIM_BREAKPOLARITY_LOW) || \
                                         ((POLARITY) == TIM_BREAKPOLARITY_HIGH))
/**
  * @}
  */
/** @defgroup TIMEx_AOE_Bit_Set_Reset 
  * @{
  */
#define TIM_AUTOMATICOUTPUT_ENABLE           (TIM_BDTR_AOE)
#define	TIM_AUTOMATICOUTPUT_DISABLE          ((uint32_t)0x0000)

#define IS_TIM_AUTOMATIC_OUTPUT_STATE(STATE) (((STATE) == TIM_AUTOMATICOUTPUT_ENABLE) || \
                                              ((STATE) == TIM_AUTOMATICOUTPUT_DISABLE))
/**
  * @}
  */  
  
/** @defgroup TIMEx_Master_Mode_Selection
  * @{
  */  
#define	TIM_TRGO_RESET            ((uint32_t)0x0000)             
#define	TIM_TRGO_ENABLE           (TIM_CR2_MMS_0)           
#define	TIM_TRGO_UPDATE           (TIM_CR2_MMS_1)             
#define	TIM_TRGO_OC1              ((TIM_CR2_MMS_1 | TIM_CR2_MMS_0))    
#define	TIM_TRGO_OC1REF           (TIM_CR2_MMS_2)           
#define	TIM_TRGO_OC2REF           ((TIM_CR2_MMS_2 | TIM_CR2_MMS_0))          
#define	TIM_TRGO_OC3REF           ((TIM_CR2_MMS_2 | TIM_CR2_MMS_1))           
#define	TIM_TRGO_OC4REF           ((TIM_CR2_MMS_2 | TIM_CR2_MMS_1 | TIM_CR2_MMS_0))   

#define IS_TIM_TRGO_SOURCE(SOURCE) (((SOURCE) == TIM_TRGO_RESET) || \
                                    ((SOURCE) == TIM_TRGO_ENABLE) || \
                                    ((SOURCE) == TIM_TRGO_UPDATE) || \
                                    ((SOURCE) == TIM_TRGO_OC1) || \
                                    ((SOURCE) == TIM_TRGO_OC1REF) || \
                                    ((SOURCE) == TIM_TRGO_OC2REF) || \
                                    ((SOURCE) == TIM_TRGO_OC3REF) || \
                                    ((SOURCE) == TIM_TRGO_OC4REF))
      
   
/**
  * @}
  */ 

/** @defgroup TIMEx_Master_Slave_Mode 
  * @{
  */

#define TIM_MASTERSLAVEMODE_ENABLE          ((uint32_t)0x0080)
#define TIM_MASTERSLAVEMODE_DISABLE         ((uint32_t)0x0000)
#define IS_TIM_MSM_STATE(STATE) (((STATE) == TIM_MASTERSLAVEMODE_ENABLE) || \
                                 ((STATE) == TIM_MASTERSLAVEMODE_DISABLE))
/**
  * @}
  */ 

/** @defgroup TIMEx_Commutation_Mode 
  * @{
  */
#define TIM_COMMUTATION_TRGI              (TIM_CR2_CCUS)
#define TIM_COMMUTATION_SOFTWARE          ((uint32_t)0x0000)
/**
  * @}
  */
  
/** @defgroup TIMEx_Remap 
  * @{
  */

#define TIM_TIM2_TIM8_TRGO                     (0x00000000)
#define TIM_TIM2_ETH_PTP                       (0x00000400)
#define TIM_TIM2_USBFS_SOF                     (0x00000800)
#define TIM_TIM2_USBHS_SOF                     (0x00000C00)
#define TIM_TIM5_GPIO                          (0x00000000)
#define TIM_TIM5_LSI                           (0x00000040)
#define TIM_TIM5_LSE                           (0x00000080)
#define TIM_TIM5_RTC                           (0x000000C0)
#define TIM_TIM11_GPIO                         (0x00000000)
#define TIM_TIM11_HSE                          (0x00000002)

#define IS_TIM_REMAP(TIM_REMAP)	 (((TIM_REMAP) == TIM_TIM2_TIM8_TRGO)||\
                                  ((TIM_REMAP) == TIM_TIM2_ETH_PTP)||\
                                  ((TIM_REMAP) == TIM_TIM2_USBFS_SOF)||\
                                  ((TIM_REMAP) == TIM_TIM2_USBHS_SOF)||\
                                  ((TIM_REMAP) == TIM_TIM5_GPIO)||\
                                  ((TIM_REMAP) == TIM_TIM5_LSI)||\
                                  ((TIM_REMAP) == TIM_TIM5_LSE)||\
                                  ((TIM_REMAP) == TIM_TIM5_RTC)||\
                                  ((TIM_REMAP) == TIM_TIM11_GPIO)||\
                                  ((TIM_REMAP) == TIM_TIM11_HSE))

/**
  * @}
  */ 

/**
  * @}
  */   
  
/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/*  Timer Hall Sensor functions  **********************************************/
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Init(TIM_HandleTypeDef* htim, TIM_HallSensor_InitTypeDef* sConfig);
HAL_StatusTypeDef HAL_TIMEx_HallSensor_DeInit(TIM_HandleTypeDef* htim);

void HAL_TIMEx_HallSensor_MspInit(TIM_HandleTypeDef* htim);
void HAL_TIMEx_HallSensor_MspDeInit(TIM_HandleTypeDef* htim);

 /* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start(TIM_HandleTypeDef* htim);
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop(TIM_HandleTypeDef* htim);
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start_IT(TIM_HandleTypeDef* htim);
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop_IT(TIM_HandleTypeDef* htim);
/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start_DMA(TIM_HandleTypeDef* htim, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop_DMA(TIM_HandleTypeDef* htim);

/*  Timer Complementary Output Compare functions  *****************************/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start(TIM_HandleTypeDef* htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop(TIM_HandleTypeDef* htim, uint32_t Channel);

/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start_IT(TIM_HandleTypeDef* htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop_IT(TIM_HandleTypeDef* htim, uint32_t Channel);

/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start_DMA(TIM_HandleTypeDef* htim, uint32_t Channel, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop_DMA(TIM_HandleTypeDef* htim, uint32_t Channel);

/*  Timer Complementary PWM functions  ****************************************/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start(TIM_HandleTypeDef* htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop(TIM_HandleTypeDef* htim, uint32_t Channel);

/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start_IT(TIM_HandleTypeDef* htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop_IT(TIM_HandleTypeDef* htim, uint32_t Channel);
/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start_DMA(TIM_HandleTypeDef* htim, uint32_t Channel, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop_DMA(TIM_HandleTypeDef* htim, uint32_t Channel);

/*  Timer Complementary One Pulse functions  **********************************/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Start(TIM_HandleTypeDef* htim, uint32_t OutputChannel);
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Stop(TIM_HandleTypeDef* htim, uint32_t OutputChannel);

/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Start_IT(TIM_HandleTypeDef* htim, uint32_t OutputChannel);
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Stop_IT(TIM_HandleTypeDef* htim, uint32_t OutputChannel);

/* Extnsion Control functions  ************************************************/
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutationEvent(TIM_HandleTypeDef* htim, uint32_t  InputTrigger, uint32_t  CommutationSource);
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutationEvent_IT(TIM_HandleTypeDef* htim, uint32_t  InputTrigger, uint32_t  CommutationSource);
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutationEvent_DMA(TIM_HandleTypeDef* htim, uint32_t  InputTrigger, uint32_t  CommutationSource);
HAL_StatusTypeDef HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef* htim, TIM_MasterConfigTypeDef * sMasterConfig);
HAL_StatusTypeDef HAL_TIMEx_ConfigBreakDeadTime(TIM_HandleTypeDef* htim, TIM_BreakDeadTimeConfigTypeDef *sBreakDeadTimeConfig);
HAL_StatusTypeDef HAL_TIMEx_RemapConfig(TIM_HandleTypeDef* htim, uint32_t Remap);

/* Extension Callback *********************************************************/
void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef* htim);
void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef* htim);
void HAL_TIMEx_DMACommutationCplt(DMA_HandleTypeDef *hdma);

/* Extension Peripheral State functions  **************************************/
HAL_TIM_StateTypeDef HAL_TIMEx_HallSensor_GetState(TIM_HandleTypeDef* htim);

/**
  * @}
  */ 

/**
  * @}
  */ 
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_TIM_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
