/******************************************************************
 * Robotics team Software Tutorial 2013 #1            *
 * HW #1: Controll the LED using Ligth Sensor             *
 * Designed by Prock Sept. 26 2013                        *
 ******************************************************************/
 
#include "main.h"
#define BUT_SEL GPIO_Pin_6
#define BUT_UP  GPIO_Pin_7
#define BUT_DOWN GPIO_Pin_8
#define BUT_LEFT GPIO_Pin_9
#define BUT_RIGHT GPIO_Pin_10
#define LED_ON 0
#define LED_OFF 1
#define LED_RED GPIO_Pin_6
#define LED_GREEN GPIO_Pin_7
u8 temp = 0;
u8 speed[] = {20,50,80};
u8 angle_speed[] = {5,10,15};
u16 cur_angle;
u32 cont;
 
void button_init(void){
 
    GPIO_InitTypeDef GPIO_InitStructure;
   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);    
 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;              
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin = BUT_SEL|BUT_UP|BUT_DOWN|BUT_LEFT|BUT_RIGHT; 
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

void led_init(void){
 
    GPIO_InitTypeDef LED_InitStructure;
   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE); 
 
    LED_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;            
    LED_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    LED_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;                                
    GPIO_Init(GPIOG, &LED_InitStructure);
	
	  GPIO_SetBits(GPIOG, LED_RED|LED_GREEN);                   // Set Red LED off at default init state
}

int main(void){
		servo_init();
		motor_init();
		button_init();
		led_init();
		temp=0;
		cur_angle = 1500;
	
		while(1){
			
        if(!GPIO_ReadInputDataBit(GPIOF,BUT_SEL)){
					temp=(temp+1)%3;
					while(!GPIO_ReadInputDataBit(GPIOF,BUT_SEL));
				}
				else if(!GPIO_ReadInputDataBit(GPIOF,BUT_UP)){
					_delay_ms(20);
					while(!GPIO_ReadInputDataBit(GPIOF,BUT_UP)){
							GPIO_SetBits(MOTOR_DIR_PORT,GPIO_Pin_2);
							TIM_SetCompare1(TIM3,speed[temp]);
					} 
					TIM_SetCompare1(TIM3,0);
				}
				else if(!GPIO_ReadInputDataBit(GPIOF,BUT_DOWN)){
					_delay_ms(20);
					while(!GPIO_ReadInputDataBit(GPIOF,BUT_DOWN)){
							GPIO_ResetBits(MOTOR_DIR_PORT,GPIO_Pin_2);
							TIM_SetCompare1(TIM3,speed[temp]);
					}
					TIM_SetCompare1(TIM3,0);
				}
				else if(!GPIO_ReadInputDataBit(GPIOF,BUT_LEFT)){
					cont=0;
					while(!GPIO_ReadInputDataBit(GPIOF,BUT_LEFT)){
							if(cont++>9999){cur_angle+=angle_speed[temp];cont=0;}
							if(cur_angle>2100)cur_angle=2100;
							TIM_SetCompare1(TIM4,cur_angle);
					}
				}
				else if(!GPIO_ReadInputDataBit(GPIOF,BUT_RIGHT)){
					cont=0;
					while(!GPIO_ReadInputDataBit(GPIOF,BUT_RIGHT)){
						if(cont++>9999){cur_angle-=angle_speed[temp];cont=0;}
						if(cur_angle<900)cur_angle=900;
						TIM_SetCompare1(TIM4,cur_angle);
					}    // Waiting button release 
				}
				else{
					TIM_SetCompare1(TIM3,0);
				}
        
    }
    return 0;
}
