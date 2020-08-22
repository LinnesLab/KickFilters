/*
 * FILENAME: EXAMPLE03_notch.ino
 * AUTHOR:   Orlando S. Hoilett
 * CONTACT:  orlandohoilett@gmail.com
 * VERSION:  1.0.0
 * 
 * 
 * AFFILIATIONS
 * Linnes Lab, Weldon School of Biomedical Engineering,
 * Purdue University, West Lafayette, IN 47907
 * 
 * 
 * DESCRIPTION
 * Basic test of the KickFilters class to evaluate the notch filter.
 * 
 * The input signal is a simulated photoplethysmogram used to
 * measure heart rate. Each major peak is a new heart beat. The
 * smaller peak is a dicrotic notch which is generated when
 * the aortic valve closes.
 * 
 * 
 * UPDATES
 * Version 1.0.0
 * 2020/03/23:1055>
 *           - Initial release.
 * 
 * 
 * DISCLAIMER
 * Linnes Lab code, firmware, and software is released under the
 * MIT License (http://opensource.org/licenses/MIT).
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Linnes Lab, Purdue University
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */


#include "KickFilters.h"


const float sinefreq = 60;
const float fs = 15*sinefreq; //samples at 15x the frequency of the wave
const float dt = 1.0/fs;
const uint16_t samples = 20*(1/sinefreq)/dt; //prints 20 cycles of the wave

const float scale = 100;


void setup()
{
  Serial.begin(9600); //Use SerialUSB for SparkFun SAMD21 boards
  while(!Serial); //will not run until Serial Monitor or Plotter is open

  
  //Generate input sine wave
  int16_t input[samples] = {0};
  for(uint16_t i = 0; i < samples; i++)
  {
    input[i] = scale*sin(2*PI*sinefreq*i*dt);
  }


  int16_t filtered1[samples] = {0};
  int16_t filtered2[samples] = {0};
  int16_t filtered3[samples] = {0};

  KickFilters<int16_t>::notch(input, filtered1, samples, 50, fs);
  KickFilters<int16_t>::notch(input, filtered2, samples, 58, fs);
  KickFilters<int16_t>::notch(input, filtered3, samples, 60, fs);


  //Print input and output waves to Serial Plotter to compare
  for(uint16_t i = 0; i < samples; i++)
  {
    //Use SerialUSB for SparkFun SAMD21 boards
    Serial.print(input[i]);
    Serial.print(",");
    Serial.print(filtered1[i]); //should be least filtered
    Serial.print(",");
    Serial.print(filtered2[i]);
    Serial.print(",");
    Serial.print(filtered3[i]); //should be most filtered
    Serial.println();
  }

}


void loop()
{
}
