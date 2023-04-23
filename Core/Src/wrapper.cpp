//robomaster用
#include <main.h>
#include "stm32f1xx_hal_uart.h"		// SBDBTとUART通信をするためのライブラリ

// UART用構造体変数
extern UART_HandleTypeDef huart2;
extern uint8_t RxBuffer[8];

extern uint8_t B;
extern uint8_t A;
extern uint8_t X;
extern uint8_t Y;
extern uint8_t RIGHT;
extern uint8_t DOWN;
extern uint8_t LEFT;
extern uint8_t UP;
extern uint8_t R1;
extern uint8_t R2;
extern uint8_t L1;
extern uint8_t L2;
extern uint8_t START;
extern uint8_t BACK;
extern uint8_t RightAxisX;
extern uint8_t RightAxisY;
extern uint8_t LeftAxisX;
extern uint8_t LeftAxisY;


double Vx = 0;
double Vy = 0;


double v1 = 0;
double v2 = 0;
double v3 = 0;
double v4 = 0;

int32_t V1 = 0;
int32_t V2 = 0;
int32_t V3 = 0;
int32_t V4 = 0;
// タイマ用構造体変数
TIM_HandleTypeDef htim1;

void bldc_update(){
	Vx = (LeftAxisX - 64)*250/64+708;
	Vy = (64 - LeftAxisY)*250/64+708;

	v1 = (Vx-708) + (Vy-708)+708;
	v2 = (Vx-708) - (Vy-708)+708;
	v3 = -((Vx-708) + (Vy-708))+708;
	v4 = -(Vx-708) + (Vy-708)+708;

	V1 = (int32_t)v1;
	V2 = (int32_t)v2;
	V3 = (int32_t)v3;
	V4 = (int32_t)v4;
}

void wheel1(){
	if(V1 >= 1000){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 999);
	}
	if(V1 <= 499){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 417);
	}
	else{
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, V1);
	}
}

void wheel2(){
	if(V2 >= 1000){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 999);
	}
	if(V2 <= 499){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 417);
	}
	else{
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, V2);
	}
}

void wheel3(){
	if(V3 >= 1000){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 999);
	}
	if(V3 <= 499){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 417);
	}
	else{
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, V3);
	}
}

void wheel4(){
	if(V4 >= 1000){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 999);
	}
	if(V4 <= 499){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 417);
	}
	else{
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, V4);
	}
}
// メイン関数
void main_cpp(void)
{
	// UART開始
	HAL_UART_Receive_IT(&huart2, RxBuffer, 8);

	// LED緑を点灯
	HAL_GPIO_WritePin(GPIOC, GREEN_LED_Pin, GPIO_PIN_SET);

	// 無効化後、黄色LEDを消灯、赤色LEDを点灯
	HAL_GPIO_WritePin(GPIOC, YELLOW_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, RED_LED_Pin, GPIO_PIN_SET);

	while(1){
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);

		//パルス幅max
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 999);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 999);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 999);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 999);

		if(A == 1){
			//電源付けてから3sec以内にAを押す
			break;
		}

	}

	//HAL_Delay(100);

	//パルス幅min
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 417);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 417);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 417);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 417);

	// 有効化後、黄色LEDを点灯、赤色LEDを消灯
	HAL_GPIO_WritePin(GPIOC, YELLOW_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, RED_LED_Pin, GPIO_PIN_RESET);

	//ちょっと待つ
	//HAL_Delay(100);

	//回らないようにする
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 708);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 708);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 708);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 708);


	// メインループ
	while(1)
	{

		// もし、BACKボタンが押されている(BACK == 1)なら、
		if(BACK == 1)
		{

			// 無効化後、黄色LEDを消灯、赤色LEDを点灯
			HAL_GPIO_WritePin(GPIOC, YELLOW_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, RED_LED_Pin, GPIO_PIN_SET);

			// LED緑を点灯
			HAL_GPIO_WritePin(GPIOC, GREEN_LED_Pin, GPIO_PIN_SET);

			//PWM無効化
			//tim1
			HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
			HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);
			HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_4);


		}

		// もし、STARTボタンが押されている(START == 1)なら、
		if(START == 1)
		{
			// 有効化後、黄色LEDを点灯、赤色LEDを消灯
			HAL_GPIO_WritePin(GPIOC, YELLOW_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, RED_LED_Pin, GPIO_PIN_RESET);

			// LED緑を点灯
			HAL_GPIO_WritePin(GPIOC, GREEN_LED_Pin, GPIO_PIN_SET);

			//PWM有効化
			//tim1
			HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
			HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
			HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
		}

		//コントローラーの値を取得、いい感じに計算
		bldc_update();

		wheel1();
		wheel2();
		wheel3();
		wheel4();

		if(L1 == 1){
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 550);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 550);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 550);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 550);
		}

		if(R1 == 1){
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 850);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 850);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 850);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 850);
		}

		//初期化ダンス用？？？？
		if(L2 == 1){
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 417);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 417);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 417);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 417);
		}

		if(R2 == 1){
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 999);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 999);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 999);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 999);
		}

	}
}
