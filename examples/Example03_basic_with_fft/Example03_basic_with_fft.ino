/*
 * FILENAME: Example01_basic_with_fft.ino
 * AUTHOR:   Orlando S. Hoilett
 * CONTACT:  orlandohoilett@gmail.com / ohoilett@purdue.edu
 * VERSION:  1.0.0
 * 
 * 
 * AFFILIATIONS
 * Linnes Lab, Weldon School of Biomedical Engineering,
 * Purdue University, West Lafayette, IN 47907
 * 
 * 
 * DESCRIPTION
 * Basic test of the KickFilters class to evaluate each filter and
 * then running an FFT on the filtered and unfiltered signal
 * 
 * 
 * UPDATES
 * Version 1.0.0
 * 2020/06/19:1649>
 *           - Initiated code
 * 
 * 
 * (C) 2020 by Authors
 * (C) Linnes Lab, Purdue University, West Lafayette, IN, USA
 * All rights reserved
 * 
 */

#include "KickFilters.h"
#include "KickFFT.h"


const uint16_t samples = 256;
//input data is located in the Box: Box/L1FR_Linnes Fundamental Projects/Orlando Hoilett/Kick_LL/Data/2017-05-02/2017-05-02-baseline/goodbaselinedata.xlsx
//data is sampled every 42 milliseconds
const int16_t input[] = {773, 702, 515, 389, 370, 447, 554, 601, 561, 492, 455, 458, 480, 497, 545, 724, 801, 651, 460, 370, 396, 494, 579, 570, 507, 450, 433, 449, 480, 499, 500, 591, 756, 736, 548, 396, 356, 416, 522, 577, 549, 479, 436, 430, 455, 485, 502, 504, 496, 570, 737, 731, 540, 391, 362, 436, 539, 580, 543, 471, 428, 427, 450, 473, 486, 492, 485, 471, 486, 635, 748, 639, 455, 354, 363, 461, 560, 577, 518, 463, 443, 458, 482, 503, 521, 515, 499, 585, 735, 710, 536, 402, 368, 422, 520, 584, 551, 487, 444, 433, 448, 478, 496, 502, 505, 595, 753, 695, 514, 383, 366, 433, 529, 584, 562, 498, 447, 437, 454, 479, 494, 500, 530, 681, 752, 622, 450, 366, 384, 468, 557, 584, 534, 469, 435, 438, 464, 495, 517, 526, 634, 777, 730, 544, 402, 366, 424, 528, 602, 574, 499, 450, 440, 457, 481, 500, 520, 658, 781, 678, 485, 374, 374, 447, 547, 599, 563, 491, 442, 439, 459, 480, 495, 567, 742, 768, 599, 421, 350, 377, 481, 574, 590, 528, 458, 430, 438, 464, 488, 510, 640, 776, 689, 502, 379, 363, 437, 546, 605, 581, 507, 452, 438, 455, 478, 504, 633, 790, 740, 541, 390, 356, 418, 520, 588, 573, 506, 450, 430, 444, 472, 581, 770, 764, 558, 383, 327, 377, 477, 564, 573, 514, 452, 425, 428, 449, 556, 752, 762, 575, 398, 337, 385, 496, 595, 590, 522, 460, 440, 451, 484, 650, 810, 723, 521, 389};


void setup()
{
  Serial.begin(9600); //Use SerialUSB for SparkFun SAMD21 boards
  while(!Serial); //will not run until Serial Monitor or Plotter is open

  int16_t filtered[samples] = {0};
  int16_t tmpArr[samples] = {0};
  //Uncomment each line to use the filter (not all at once)
  //KickFilters::highpass(input, filtered, samples, 1, 42);
  //KickFilters::lowpass(input, filtered, samples, 1, 42);
  KickFilters::bandpass(input, filtered, tmpArr, samples, 0.5, 3.5, 42);
  //KickFilters::movingAverage(input, filtered, samples, 5);

  //Print to Serial Plotter to display
  for(uint16_t i = 0; i < samples; i++)
  {
    Serial.print(input[i]); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.print(","); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.print(filtered[i]); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.println(); //Use SerialUSB for SparkFun SAMD21 boards

    delay(25);
  }


  /////////////////////
  //Computing the FFT//
  /////////////////////
  uint16_t mag_input[256] = {0}; //raw input signal
  //KickFFT::fft(23.8, 0.5, 3.5, samples, input, mag_input); //computes FFT acros only frequencies of ineterest
  KickFFT::fft(samples, input, mag_input); //computes FFT across all frequencies
  
  uint16_t mag_filtered[256] = {0}; //filtered signal
  //KickFFT::fft(23.8, 0.5, 3.5, samples, filtered, mag_filtered); //computes FFT acros only frequencies of ineterest
  KickFFT::fft(samples, filtered, mag_filtered); //computes FFT across all frequencies
  
  //Print to Serial Plotter to display
  //Only print one half of samples since the FFT is goes from 0 to fs/2
  for(uint16_t i = 0; i < samples/2; i++)
  {
    Serial.print(mag_input[i]); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.print(","); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.print(mag_filtered[i]); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.println(); //Use SerialUSB for SparkFun SAMD21 boards

    delay(25);
  }

}


void loop()
{
}