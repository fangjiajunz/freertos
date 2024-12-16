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
static SemaphoreHandle_t xOLEDMutex;

void OLED_LockInit() {
    xOLEDMutex = xSemaphoreCreateMutex();
}

void OLED_Lock() {
    xSemaphoreTake(xOLEDMutex, portMAX_DELAY);
}

void OLED_Unlock() {
    xSemaphoreGive(xOLEDMutex);
}

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
    OLED_Update(); // 刷新显示

    while (1) {
        // 显示 "Waiting...."
        OLED_ShowString(0, 0, "Waiting....", OLED_8X16);
        OLED_Update(); // 刷新显示
        vTaskDelay(pdMS_TO_TICKS(2000)); // 延时 2 秒

        // 清除显示的内容
        OLED_ClearArea(0, 0, 127, 16); // 清除区域 (0,0) 到 (127,16)
        OLED_Update(); // 刷新显示
        vTaskDelay(pdMS_TO_TICKS(900)); // 延时 0.9 秒
    }
}

// 定义队列句柄
QueueHandle_t xTestQueue;

// 发送任务
void vQueueSendTask(void *pvParameters) {
    int32_t lValueToSend = 0;
    while (1) {
        lValueToSend++;
        if (xQueueSendToBack(xTestQueue, &lValueToSend, pdMS_TO_TICKS(100)) == pdPASS) {
            OLED_Lock();
            OLED_ShowString(0, 0, "Sent:", OLED_8X16);
            OLED_ShowNum(40, 0, lValueToSend, 4, OLED_8X16);
            OLED_Unlock();
        } else {
            OLED_Lock();
            OLED_ShowString(0, 0, "Queue Full", OLED_8X16);
            OLED_Unlock();
        }
        OLED_Update();
        vTaskDelay(pdMS_TO_TICKS(10)); // 1 秒发送一次
    }
}

// 接收任务
void vQueueReceiveTask(void *pvParameters) {
    int32_t lReceivedValue;
    while (1) {
        if (xQueueReceive(xTestQueue, &lReceivedValue, pdMS_TO_TICKS(500)) == pdPASS) {
            OLED_Lock();
            OLED_ShowString(0, 16, "Recv:", OLED_8X16);
            OLED_ShowNum(40, 16, lReceivedValue, 4, OLED_8X16);
            OLED_Unlock();
        } else {
            OLED_Lock();
            OLED_ShowString(0, 16, "Queue Empty", OLED_8X16);
            OLED_Unlock();
        }
        OLED_Update();
        vTaskDelay(pdMS_TO_TICKS(2000)); // 延时以避免频繁刷新
    }
}

// 初始化队列和任务
void QueueTestInit(void) {
    // 创建队列，容量为 5，每个元素大小为 int32_t
    xTestQueue = xQueueCreate(5, sizeof(int32_t));
    if (xTestQueue == NULL) {
        // 队列创建失败，打印调试信息或其他处理
        OLED_Update();
        OLED_ShowString(0, 0, "Queue Fail", OLED_8X16);
        OLED_Update();
        OLED_Unlock();
        while (1); // 停止运行
    }
    // 创建发送任务
    xTaskCreate(vQueueSendTask, "SendTask", 200, NULL, 2, NULL);
    // 创建接收任务
    xTaskCreate(vQueueReceiveTask, "RecvTask", 200, NULL, 1, NULL);
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
    OLED_Init();
    OLED_LockInit();
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

    //xTaskCreate(oledtest, "oled", 128,NULL, 1,NULL);
    QueueTestInit();
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* definition and creation of defaultTask */
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */

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

    }
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

