#ifndef _Definitions_H_
#define _Definitions_H_

const byte weldPin = 5;
const byte ledPin = A3;
const byte weldButtonPin = 7;
const byte upButtonPin = 9;
const byte downButtonPin = 8;
const byte selectButtonPin = 10;
const byte foodSwitchPin = A5;
const byte zeroCrossPin = 2;

const byte TFT_RSTpin = A1; // set in user.h, todo: do in TFT_ILI9341 constructor
const byte TFT_DCpin = A0; 
const byte TFT_CSpin = A2; 

const int step_ms = 50;
const int sinusMax_us = 4583; // average between 0.25/50Hz and 0.25/60Hz

#endif
