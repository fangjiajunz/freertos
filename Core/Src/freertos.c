/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
typedef struct {
    uint8_t x;
    uint8_t y;
    char name[16];
} OledMess;

OledMess oled1 = {0, 0, "hello"};
OledMess oled2 = {0, 16, "ggggg"};
OledMess oled3 = {0, 32, "fffff"};
static uint8_t OledCanUsed = 1;

void DisplayTask(void *pvParameters) {
    OledMess *oled = pvParameters;
    int count = 0;
    // 初始化显示内容
    OLED_Clear(); // 清屏
    while (1) {
        // 显示字符串内容
        if (OledCanUsed) {
            if (count < 50) {
                OledCanUsed = 0;
                //OLED_ShowString(oled->x, oled->y, oled->name, OLED_8X16);
                //OLED_ShowString((oled->x) + 1, oled->y, ":", OLED_8X16);
                OLED_ShowNum((oled->x) + 2, oled->y, count++, 6,OLED_8X16);
                // 更新屏幕
                OLED_Update();
                OledCanUsed = 1;
            } else {
                OLED_ClearArea(oled->x, oled->y,OLED_8X16 * 8, 16); // 清屏
                // OLED_ShowString(oled->x, oled->y, oled->name, OLED_8X16);
                vTaskDelete(NULL);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // 延时 1000ms，避免任务占用过多 CPU 时间
    }
}

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) {
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}

/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* definition and creation of defaultTask */
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    TaskHandle_t oeldtask_handle = NULL;
    xTaskCreate(
            DisplayTask, // 任务函数
            "Display Task1", // 任务名称
            128, // 任务栈大小（单位：字）
            &oled1, // 传递给任务的参数
            1, // 任务优先级
            &oeldtask_handle // 任务句柄（可选）
            );
    xTaskCreate(
            DisplayTask, // 任务函数
            "Display Task2", // 任务名称
            128, // 任务栈大小（单位：字）
            &oled2, // 传递给任务的参数
            1, // 任务优先级
            NULL // 任务句柄（可选）
            );
    xTaskCreate(
            DisplayTask, // 任务函数
            "Display Task3", // 任务名称
            128, // 任务栈大小（单位：字）
            &oled3, // 传递给任务的参数
            1, // 任务优先级
            NULL // 任务句柄（可选）
            );
    /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument) {
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;) {
        osDelay(1);
        // OLED_Init();
        // OLED_ShowString(0, 0, "asdfdsf", OLED_8X16);
        // OLED_ShowString(0, 16, "Hello, World!", OLED_8X16);
        // // 更新屏幕
        // OLED_Update();
    }
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

