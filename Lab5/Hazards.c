#include "Hazards.h"
#include "math.h"
//#include "servo.h"
void move_servo(int degree);
void timer1_init();
void move_servo_C(int degree);


unsigned pulse_period;//320000; // pulse period in cycles   --------- change?
unsigned mid_width;//24000;
unsigned period_width;//48000;

void scan_Hazard(int MODE){
		int ir_d[90];
		int ping_d[90];
		double width;
		int dists[12];
		int P1,P2,P3,P4,i,Theta,Stheta;
		int a = 0;
		int adress=0;
		//int set = 0;
		//int fin = 0;
		//int count = 0;
		int is_rising;
		int was_rising=0;
		//int dist[10];
		//int angle[10];
		//double base[10];
		//int center[10];
		//double pie = (3.1415 /180);
		//double rads;
		//int smallest = 0;
		//char str[38];
		//uart_sendString("Degrees   IR Distance (cm)  Sonar Distance (cm) \n");
		//sprintf("%-10d %-18d %-10d \n", i, ir_d[i/2], ping_d[i/2]);
		move_servo_C(0);
		timer_waitMillis(256);
		while(1){
			if(a == 0) //gathering data
			{
				for(i = 0; i < 180; i = i + 2)
				{
					move_servo_C(i);
					timer_waitMillis(50);P1=ir_getDistance();P2=ir_getDistance();P3=ir_getDistance();P4=ir_getDistance();
					ir_d[i/2] = (P1+P2+P3+P4)/4;
					ping_trigger();
					timer_waitMillis(15);
					P1=(int) ping_getDistance();
					ping_trigger();
					timer_waitMillis(15);
					P2=(int) ping_getDistance();
					ping_d[i/2] = (((P1+P2)/2.0));
					if((ir_d[i/2]<50) && ping_d[i/2]<50){
					//lcd_printf("Dist found: %d ",ping_d[i/2]);
					timer_waitMillis(55);
					}
					//sprintf(str, "%-10d %-18d %-10d \n\r", i, ir_d[i/2], ping_d[i/2]);
					//uart_sendString(str);
				}
				a = 1;
			}
			lcd_printf("Out of loop one");
			timer_waitMillis(2000);
			for(i=0;i<180;i=i+2)
			{

				if(ir_d[i/2]<50 && ping_d[i/2]<50)//something is in range;
				{
					is_rising=1;
				}
				else//no rising edge here.
				{
					is_rising = 0;
					ir_d[i/2]=0;
				}
				if(is_rising>was_rising)//rising edge found
				{
					Stheta=i;
				}
				if(is_rising<was_rising)//falling edge
				{
					Theta = i-Stheta;
					if(Theta > 6)//&& ping_d[Stheta] < 50 && ping_d[i] <50
					{
						//if(ping_d[Stheta] < ping_d[i]){}
						if(ping_d[Stheta/2] > 50 || ping_d[i/2] > 50){
							if(ping_d[Stheta/2] > ping_d[i/2]){
								ping_d[Stheta/2]=ping_d[i/2];
							}
							else if(ping_d[Stheta/2] < ping_d[i/2]){
							ping_d[i/2]=ping_d[Stheta/2];
							}
						}
						dists[adress]=(ping_d[Stheta/2]+ping_d[i/2])/2;
						if(dists[adress]<24){Theta=Theta-8;}
						else if(dists[adress]<32 && dists[adress]>=24){Theta=Theta-7;}
						width=(double) sqrt(((ping_d[Stheta/2]*ping_d[Stheta/2])+(ping_d[i/2]*ping_d[i/2]))-(2*ping_d[Stheta/2]*ping_d[i/2]*cos((3.14159265*Theta)/180)));//
						//angle[adress]=Theta;
						lcd_printf("object: %d \nWidth: %lf \nTheta: %d \nDist: %d",adress+1,width,Theta,dists[adress]);
						adress++;
						timer_waitMillis(10000);
					}
				}

				was_rising=is_rising;
			}
			adress=0;
			break;
			/*
			else if(a == 1)
			{
				i = 0;
				move_servo_C(i);
				//timer_waitMillis(100);

				for(a = 1; a <90; a++) //data analyisis
				{
					if((ir_d[a] < 120) && (ping_d[a] < 80)) //object detection
					{
						if(set == 0) //finding the start of an object
						{
							set = a;
							count++;
							dist[count] = ping_d[a];
						}
						else //setting the end of an object
						{
							fin = a;
						}

					}
					else if( fin != 0) //calculating the center of an object
					{
						angle[count] = (2*fin) - (2*set);
						center[count] = ((2*set) + (2*fin))/2;
						set = 0;
						fin = 0;
					}
				}
				for(a = 0; a <= count; a++) //finding the size of an object
				{
					rads = 0.5*pie*angle[a];
					base[a] = 2*dist[a]*(sin(rads));
				}
				for(a = 1; a <= count; a++) //comparing object sizes to select the smallest
				{
					if(base[a] <= base[smallest])
					{
						smallest = a;
					}
				}
				a = 0; //set a back to zero for data collection mode
				lcd_printf("count: %1d", count);
				//\nSmallest Angle %2d \nWidth: %2d", count, center[smallest], base[smallest])
				move_servo_C(center[smallest]);
				count = 0;
				smallest = 0;
				timer_waitMillis(9999);
			}
			*/
		}
}
	/*
	lcd_printf("Out of loop one");
	flag=0;
	flag1=0;//reusing flag to save on space.
	deg=0;
	while(flag==0){
		if(Valuesone[deg]<58){//if in valid range
			if(flag1!=1){//object rising edge
				flag1=1;
				D1=Valuestwo[deg];
				thetaStart=deg;
				ready=0;
			}
			thetaEnd=deg;
			stop = 1;
			D2=Valuestwo[deg];
		}
		else{
			flag1=0;
			stop=0;
		}
		if(stop==0&&ready==0){
			ready = 1;
			Dtheta=0.0019*(D1*D1*D1)-(0.2064*D1*D1)+(6.7322*D1)-58.274;
			if(Dtheta < -3){Dtheta = -3;}
			Theta = thetaEnd-thetaStart-Dtheta;
			if(Theta > 4 && thetaEnd-thetaStart>4){
				width = ((D1*D1)+(D2*D2))-(2*D1*D2*cos((3.14159265*Theta)/180));//SAS forumal to get width
				Nwidth = sqrt(width);
				obWids[countOb] = Nwidth;
				obAngs[countOb] = thetaStart+(Theta/2);
				countOb++;
				lcd_printf("Ojects found: %d \nWidth: %lf\nTheta: %lf\nD1: %lf",countOb,Nwidth,Theta,D1);
				timer_waitMillis(2500);
			}
		}
		deg=deg+move_by;
		if(deg > 180){
			flag=1;
		}
	}


//end of while loop

//if(MODE == 0){
//	uart_sendStr(obWids);//sends all object widths as strings to computer.
//	uart_sendStr(obAngs);//sends all object positions as stings to computer.
//}
//else if(MODE == 1){
//	wids = obWids;
//	angs = obAngs;
	//for(i=0;i<10;i++){
	//	send_Data(obWids[i],)  UNSURE. Depends on how we want to manage data within the bot/program
//}
	//}

	*/

	//if(amount < 10){uart_sendStr("WARNING: OBJECT OVERFLOW");}//let User know if too many objects found
	
	
	
//code below can be relocated if need be.	
	
	
	
	
void timer1_init()
{

SYSCTL_RCGCGPIO_R |= 0x2; //starts system clock
GPIO_PORTB_DEN_R |= 0x20; //Enables PB5
GPIO_PORTB_DIR_R |= 0x20;//set PB5 to output
//GPIO_PORTB_DATA_R &= ~0x20;
GPIO_PORTB_PCTL_R |= 0x700000;//set PB5 alternate function
GPIO_PORTB_AFSEL_R |= 0x20;//enable PB5 alternate function

//***set GPIO PB5, turn on clk, alt. function, output, enable***
SYSCTL_RCGCTIMER_R |= 0x2; //turn on clk for timer0
TIMER1_CTL_R &= ~TIMER_CTL_TBEN; //disable timer to config
TIMER1_TBMR_R |= 0xA; //periodic and PWM enable
TIMER1_CFG_R |= TIMER_CFG_16_BIT; //set size of timer to 16
TIMER1_TBILR_R = 320000 & 0xFFFF; //lower 16 bits of the interval
TIMER1_TBPR_R = 320000 >> 16; //set the upper 8 bits of the interval
//TIMER1_TBMATCHR_R = ((320000 - 24000) & 0xFFFF); // if you want to move servo to the middle
//TIMER1_TBPMR_R = (320000 - 24000) >> 16;
TIMER1_CTL_R |= TIMER_CTL_TBEN; //enable timer
}


void move_servo(int degree)
{
double degree1 = 1 + ((double)degree/180);
int pulse_width = (int)(degree1 * 16000); // pulse width in cycles
// calculate pulse width in cycles
//TIMER1_CTL_R &= ~TIMER_CTL_TBEN; //disable timer to config
TIMER1_TBMATCHR_R = ((320000 - pulse_width) & 0xFFFF); // set pulse width
TIMER1_TBPMR_R = (320000 - pulse_width) >> 16;
// you need to call timer_waitMillis( ) here to enforce a delay for the servo to
// move to the position
timer_waitMillis(500);

}

void move_servo_C(int degree)
{

double degree2 = ((double)degree * 1.7778) - 100;

int pulse_width = (int)((1+(degree2/180)) * 16000); // pulse width in cycles
// calculate pulse width in cycles
//TIMER1_CTL_R &= ~TIMER_CTL_TBEN; //disable timer to config
TIMER1_TBMATCHR_R = ((320000 - pulse_width) & 0xFFFF); // set pulse width
TIMER1_TBPMR_R = (320000 - pulse_width) >> 16;
// you need to call timer_waitMillis( ) here to enforce a delay for the servo to
// move to the position
timer_waitMillis(100);

}




