/* See the file "LICENSE" for the full license governing this code. */

#include <Arduino.h>
#include "PCA9634.h"

PCA9634::PCA9634(uint8_t addr){
    _addr = addr;
}

void PCA9634::begin(uint8_t mode1, uint8_t mode2){
    // all leds default OFF; PWM set to 0
    write_reg(PCA9634_MODE1,0);
    write_reg(PCA9634_MODE2,mode2); 
	_reg0C=0;
	_reg0D=0;
	_reg0Cg=0;
	_reg0Dg=0;
	_mode1=mode1;
	_mode2=mode2;
}

uint8_t PCA9634::set_mode1(uint8_t mode1){
    return write_reg(PCA9634_MODE1,mode1);
}

uint8_t PCA9634::set_mode2(uint8_t mode2){
    return write_reg (PCA9634_MODE2,mode2);
}

uint8_t PCA9634::set_pwm(uint8_t led, uint8_t duty_cycle){
    if (led>7) return -1;
	return write_reg(PCA9634_PWM+led,duty_cycle);
}

uint8_t PCA9634::set_global_pwm(uint8_t duty_cycle){
    return write_reg(PCA9634_GRPPWM, duty_cycle );
}

uint8_t PCA9634::set_blink_freq(uint8_t frequency){
    return write_reg(PCA9634_GRPFREQ, frequency);
}

void PCA9634::swReset(void){
    for(uint8_t n=0; n<8; n++) write_reg(PCA9634_PWM+n,0);
	write_reg(PCA9634_GRPPWM,0);
    write_reg(PCA9634_GRPFREQ,0);
    write_reg(PCA9634_MODE1, _mode1);
    write_reg(PCA9634_MODE2, _mode2); 
}

uint8_t PCA9634::led_mode(uint8_t led, uint8_t mode){
 if(led>7) return 255;
 switch(led)
 {
  case 0: {_reg0C &=0xFC; break;}
  case 1: {_reg0C &=0xF3; break;}
  case 2: {_reg0C &=0xCF; break;}
  case 3: {_reg0C &=0x3F; break;}
  case 4: {_reg0D &=0xFC; break;}
  case 5: {_reg0D &=0xF3; break;}
  case 6: {_reg0D &=0xCF; break;}
  case 7: {_reg0D &=0x3F; break;}
 }
 if(led<4)
  { _reg0C|=mode<<(led*2); write_reg(0x0C, _reg0C); _reg0Cg=_reg0C;}
  else
  { _reg0D|=mode<<(led*2); write_reg(0x0D, _reg0D);  _reg0Dg=_reg0D;}
}

uint8_t PCA9634::set_led_global(uint8_t led, uint8_t mode){
 if(led>7) return 255;
 switch(led)
 {
  case 0: {_reg0Cg &=0xFC; break;}
  case 1: {_reg0Cg &=0xF3; break;}
  case 2: {_reg0Cg &=0xCF; break;}
  case 3: {_reg0Cg &=0x3F; break;}
  case 4: {_reg0Dg &=0xFC; break;}
  case 5: {_reg0Dg &=0xF3; break;}
  case 6: {_reg0Dg &=0xCF; break;}
  case 7: {_reg0Dg &=0x3F; break;}
 }
 if(led<4){ _reg0Cg|=mode<<(led*2); _reg0C=_reg0Cg;}
  else    { _reg0Dg|=mode<<(led*2); _reg0D=_reg0Dg;}
}

uint8_t PCA9634::apply_led_global(void){
 write_reg(0x0C, _reg0Cg); _reg0C=_reg0Cg;
 write_reg(0x0D, _reg0Dg); _reg0D=_reg0Dg;
}

uint8_t PCA9634::write_reg(uint8_t reg, uint8_t value){
    Wire.beginTransmission( _addr );
    Wire.write( reg );
    Wire.write( value );
    return Wire.endTransmission( );
}
