/*
 * steermodule.cpp
 *
 * Created: 2022-11-01 09:49:50
 * Author : edvsc779
 */ 

#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/sfr_defs.h>

using namespace std;

#define sbi(a, b) (a) |= (1 << (b)) // set bit
#define cbi(a, b) (a) &= ~(1 << (b)) // clear bit 
#define PWM_TOP (2499u)
#define PWM_MIN (0u)
#define PWM_MAX (4999u)
#define SERVO_DEF (125u)
#define DRIVE_CONSTANT (187u)
#define TICK_NUM (124u)

volatile int interrupt = 0;
volatile float gas_pvm = 270;
volatile unsigned long start = 0;
volatile unsigned long timer_diff = 0;
volatile unsigned long millis_value = 0;
uint8_t received_data = 0b00001111;
uint8_t previous_mode = 0;

uint8_t speed();
void accelerate();
void no_gas();

ISR (INT0_vect) //External interrupt_zero ISR
{
	if (interrupt == 0) {
		start = millis_value;
		interrupt++;
	}
	else {
		timer_diff = millis_value - start;
		start = millis_value;
		interrupt = 0;
	}
}

ISR (TIMER0_COMP_vect) {
	millis_value++;
}

ISR (SPI_STC_vect) {
	uint8_t data = speed();
	received_data = SPDR;
	
	SPDR = data;
}

void SPI_init()					/* SPI Initialize function */
{
	DDRB &= ~((1 << 5)|(1 << 7)|(1 << 4));  /* Make MOSI, SCK, SS as
 						input pins */
	DDRB |= (1 << 6);			/* Make MISO pin as 
						output pin */
	SPCR = (1 << SPE) | (1 << SPIE);			/* Enable SPI in slave mode */
}

void init() {
	DDRA = 0b00000100; // PA2, Brake. PA1 test signal to button

	DDRB = 0b01000000; // PB6, MISO, output. PB5, MOSI, input.
	DDRC = 0b00000000; // PC2,3,4,5 JTAG input.
	DDRD = 0b00110000; // PD4, PVM. PD5, Servo.
	
	/* Enable interrupts */
	GICR = (1 << INT0);
	MCUCR = (1 << ISC01) |(1 << ISC00);
	sei();

	/*set fast PWM mode with non-inverted output*/
	TIMSK = (1 << OCIE0);

	ICR1 = PWM_TOP;
	
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11) ;
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11);
	
	// For timer0
	TCCR0 = (1 << WGM01) | (1 << CS01);
	
	// 124 ticks to 1 ms
	OCR0 = TICK_NUM;
	
	SPI_init();
}

uint8_t speed(){
	float car_speed = (((0.26/(timer_diff))*1000));
	int speed_int = (int)(car_speed + 0.5);

	return (uint8_t)speed_int;
}

void update_servo_pwm(uint16_t i) {	
	OCR1A = i;
}

void update_gas_pwm(int gas_pvm) {
	OCR1B = gas_pvm;
}

void drive_straight(int& servo){
	servo = DRIVE_CONSTANT; 
	update_servo_pwm(servo);
}

void accelerate(){
	if (gas_pvm < 350) {
		gas_pvm += 0.25;
		update_gas_pwm(gas_pvm);
	}
}

void deaccelerate(){
	if (gas_pvm > 0) {
		gas_pvm -= 0.25;
		update_gas_pwm(gas_pvm);
	}
}

void no_gas(){
	gas_pvm = 0;
	update_gas_pwm(gas_pvm);
}

void stop(){
	no_gas();
	cbi(PORTA, 2);
}

/*
Set degree of servo with the help of PWM.
*/
void servo_set(uint8_t deg) {
	float deg_ = 6 * deg; 
	
	float set = SERVO_DEF + deg_ * SERVO_DEF/180;
	uint16_t point = (uint16_t)set;
	update_servo_pwm(point);
}

void set_autonome() {
	gas_pvm = 300;
}

void gas_control(uint8_t g){
	switch(g){
		case 0b00000000:
			stop();
			break;
		case 0b10000000:
			stop();
			break;
		case 0b00100000:
			accelerate();
			break;
		case 0b01000000:
			deaccelerate();
			break;
		case 0b10100000:
			set_autonome();
		default: // constant speed
			break;
	}
}

int main(void)
{
	init();
	
	uint8_t mode;
	uint8_t gas_control_;
	uint8_t turn_;
	
    while (1)
    {
		gas_control_ = received_data & 0b11100000;
		turn_ = received_data & 0b00011111;
		
		servo_set(turn_);
		gas_control(gas_control_);
    }
}
