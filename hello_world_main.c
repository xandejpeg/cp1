#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// --- Ajuste aqui seu nome para aparecer na saída:
static const char *ALUNO = "alessandro chiarele";

// Semáforos binários (um para cada etapa do ciclo)
static SemaphoreHandle_t s1 = NULL;
static SemaphoreHandle_t s2 = NULL;
static SemaphoreHandle_t s3 = NULL;

static void tarefa1(void *pvParameters)
{
    (void) pvParameters;
    for (;;)
    {
        xSemaphoreTake(s1, portMAX_DELAY);
        printf("[Tarefa 1] Executou - %s\n", ALUNO);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 segundo entre tarefas
        xSemaphoreGive(s2);
    }
}

static void tarefa2(void *pvParameters)
{
    (void) pvParameters;
    for (;;)
    {
        xSemaphoreTake(s2, portMAX_DELAY);
        printf("[Tarefa 2] Executou - %s\n", ALUNO);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 segundo entre tarefas
        xSemaphoreGive(s3);
    }
}

static void tarefa3(void *pvParameters)
{
    (void) pvParameters;
    for (;;)
    {
        xSemaphoreTake(s3, portMAX_DELAY);
        printf("[Tarefa 3] Executou - %s\n", ALUNO);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 segundo entre tarefas
        xSemaphoreGive(s1);
    }
}

void app_main(void)
{
    // Cria os semáforos binários
    s1 = xSemaphoreCreateBinary();
    s2 = xSemaphoreCreateBinary();
    s3 = xSemaphoreCreateBinary();

    if (s1 == NULL || s2 == NULL || s3 == NULL)
    {
        printf("Falha ao criar semáforos.\n");
        return;
    }

    // Inicia o ciclo liberando a Tarefa 1
    xSemaphoreGive(s1);

    // Cria as 3 tarefas
    xTaskCreate(tarefa1, "tarefa1", 2048, NULL, 5, NULL);
    xTaskCreate(tarefa2, "tarefa2", 2048, NULL, 5, NULL);
    xTaskCreate(tarefa3, "tarefa3", 2048, NULL, 5, NULL);
}
