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

static int g_Oled_Can_Used = 1;
static volatile int g_cal_end = 0;
static uint64_t g_time = 0;
static uint32_t g_sum = 0;

void SumTask(void *pvParameters) {
    uint32_t i = 0;
    TickType_t start_ticks = xTaskGetTickCount();
    for (i = 0; i < 10000000; i++) {
        g_sum += 1;
    }
    g_cal_end = 1;
    TickType_t end_ticks = xTaskGetTickCount();
    g_time = (end_ticks - start_ticks) * portTICK_PERIOD_MS;
    vTaskDelete(NULL);
}

void OledShow(void *params) {
    OLED_ShowString(0, 0, "Waiting..",OLED_8X16);
    OLED_Update();
    while (1) {
        vTaskDelay(3000);
        while (g_cal_end == 0);
        OLED_Clear();
        OLED_ShowString(0, 0, "Sum : ",OLED_8X16);
        OLED_UpdateArea(0, 0, 8, 16);
        if (g_Oled_Can_Used) {
            g_Oled_Can_Used = 0;
            OLED_ShowNum(0, 16, g_sum, 10,OLED_8X16);
            OLED_ShowNum(0, 32, g_time, 10,OLED_8X16);
            OLED_Update();
            g_Oled_Can_Used = 1;
        }
        vTaskDelete(NULL);
    }
}

void oledtest(void *params) {
    OLED_Clear(); // 初始化清屏
    while (1) {
        OLED_ShowString(0, 0, "Waiting....", OLED_8X16);
        OLED_Update(); // 刷新显示
        vTaskDelay(2000); // 延时 2 秒
        OLED_ClearArea(0, 0, 127, 16); // 清除特定区域
        OLED_Update(); // 刷新显示
        vTaskDelay(900); // 延时 2 秒
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
    // xTaskCreate(SumTask, "sum", 128, NULL, osPriorityNormal,NULL);
    // xTaskCreate(OledShow, "sum", 128, NULL, osPriorityNormal,NULL);
    xTaskCreate(oledtest, "oledtest", 128, NULL, osPriorityNormal,NULL);
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

