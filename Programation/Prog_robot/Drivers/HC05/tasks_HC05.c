#include "tasks_HC05.h"

QueueHandle_t q_print_BT=NULL;
QueueHandle_t q_read_BT=NULL;
uint8_t buffer;

void print_BT_Task(void *pvParameters){
	uint8_t char_to_send;

	for(;;){
		xQueueReceive(q_print_BT,(void*)&char_to_send,portMAX_DELAY);
		HAL_UART_Transmit(&huart3, (uint8_t *)&char_to_send, 1, 0xFFFF);
	}
}

void send_text(void *pvParameters){
	static char input_char[]="Hello World\r\n";
	for(;;){
		fill_queue(q_print_BT,input_char);
		vTaskDelay(10000);
	}
}

void fill_queue(QueueHandle_t queue, const char* text){
	for(int i=0;i<strlen(text);i++){
		xQueueSend(q_print_BT,&text[i],portMAX_DELAY);
	}
}

void read_BT(void *pvParameters){
	uint8_t buffer;
	for(;;){
		HAL_UART_Receive(&huart3,&buffer,1,portMAX_DELAY);
		xQueueSend(q_read_BT,&buffer,portMAX_DELAY);
	}
}

void renvoie(void *pvParameters){
	uint8_t char_to_send;
	HAL_UART_Receive_IT(&huart3,&buffer,1);
	for(;;){
		xQueueReceive(q_read_BT,(uint8_t*)&char_to_send,portMAX_DELAY);
		HAL_UART_Transmit(&huart1, (uint8_t *)&char_to_send, 1, 0xFFFF);
	}
}


void Start_HC05_Tasks(void) {
	TaskHandle_t TaskHandle_print_BT;
	TaskHandle_t TaskHandle_send_text;
	//TaskHandle_t TaskHandle_read_BT;
	TaskHandle_t TaskHandle_renvoie;

	q_print_BT=xQueueCreate(Q_PRINT_BT_LENGTH,Q_PRINT_BT_SIZE);
	q_read_BT=xQueueCreate(Q_PRINT_BT_LENGTH,Q_PRINT_BT_SIZE);

    xTaskCreate(print_BT_Task,"print_BT_Task",configMINIMAL_STACK_SIZE,NULL,1,&TaskHandle_print_BT);
    xTaskCreate(send_text,"send_text",configMINIMAL_STACK_SIZE,NULL,1,&TaskHandle_send_text);
    //xTaskCreate(read_BT,"read_BT",configMINIMAL_STACK_SIZE,NULL,1,&TaskHandle_read_BT);
    xTaskCreate(renvoie,"renvoie",configMINIMAL_STACK_SIZE,NULL,1,&TaskHandle_renvoie);
}
