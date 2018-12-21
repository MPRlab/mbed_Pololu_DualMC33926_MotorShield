/*
 * This is a modified version of the DualMC33926MotorShield.h library ported by Chris Bove  for use with the individual 
 * motor controllers on the MotorShield after soldering on headers on the individual D2 and SF pins
 * 
 * Author: Sean O'Neil
*/

#include "SingleMC33926MotorController.h"

SingleMC33926MotorController::SingleMC33926MotorController(PinName DIR, PinName PWM, PinName FB, PinName nD2, PinName nSF){
	_DIR = DIR;
    _PWM = PWM;
    _FB = FB;

	_nD2 = nD2; // enable line
	_nSF = nSF; // fault input line. 

    _DirOut = new DigitalOut(_DIR);
    _nD2Out = new PwmOut(_nD2);
    _PwmOut = new PwmOut(_PWM);
    _FbAnalogIn = new AnalogIn(_FB);
    _nSfDigitalIn = new DigitalIn(_nSF); // TODO make this InterruptIn
}


SingleMC33926MotorController::~SingleMC33926MotorController(){
	delete _DirOut;
	delete _nD2Out;
	delete _PwmOut;
	delete _FbAnalogIn;
	delete _nSfDigitalIn;
}

void SingleMC33926MotorController::enable(){
	_nD2Out->write(1.0f);
}

void SingleMC33926MotorController::disable(){
	_nD2Out->write(0.0f);	
}

void SingleMC33926MotorController::setSpeedCoast(float speed){
	setSpeed(speed, true);
}

void SingleMC33926MotorController::setSpeedBrake(float speed){
	setSpeed(speed, false);
}

void SingleMC33926MotorController::setSpeed(float speed, bool coast){
	unsigned char reverse = 0;
  
  	// clamp and correct the speed for Pwm.write()
	if (speed < 0)
	{
	speed = -speed;  // Make speed a positive quantity
	reverse = 1;  // Preserve the direction
	}
	if (speed > 1)  // Max PWM dutycycle
	speed = 1;

	// check for coasting or braking to drive the PWM pin or the enable pin
	if(coast){
		_PwmOut->write(1.0f);
		_nD2Out->write(speed);
		_DirOut->write(reverse);
	}
	else{
		_PwmOut->write(speed);
		_nD2Out->write(1.0f);
		_DirOut->write(reverse);		
	}
}

float SingleMC33926MotorController::getCurrent(){
	//float volts = analogIn->read() * 3.3;
	// 3.3V / 0.525 V per A = 6.2857 A per 1.0 V
	return _FbAnalogIn->read() * 6.2857f;
}

// Return error status
bool SingleMC33926MotorController::hasFault()
{
  int result = _nSfDigitalIn->read();
  if(result)
    return false;
  else
    return true;
}