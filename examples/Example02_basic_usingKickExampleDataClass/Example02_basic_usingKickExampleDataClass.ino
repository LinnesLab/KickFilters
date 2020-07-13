/*
 * FILENAME: Example02_basic_usingKickExampleDataClass.ino
 * AUTHOR:   Orlando S. Hoilett
 * CONTACT:  orlandohoilett@gmail.com
 * VERSION:  0.0.0
 * 
 * 
 * AFFILIATIONS
 * Linnes Lab, Weldon School of Biomedical Engineering,
 * Purdue University, West Lafayette, IN 47907
 * 
 * 
 * DESCRIPTION
 * Basic test of the KickFilters class to evaluate each filter
 * 
 * 
 * UPDATES
 * Version 0.0.0
 * 2020/03/23:1142>
 *           - Reimplemented previous example with data moved
 *           to KickExampleData class.
 * Version 1.0.1
 * 2020/05/22:0327>
 *           - Changed name to Example not EXAMPLE
 * 
 * 
 * (C) 2020 by Authors
 * (C) Linnes Lab, Purdue University, West Lafayette, IN, USA
 * All rights reserved
 * 
 */

#include "KickFilters.h"
#include "KickExampleData.h" //using this class for example data


void setup()
{
  Serial.begin(9600); //Use SerialUSB for SparkFun SAMD21 boards
  while(!Serial); //will not run until Serial Monitor or Plotter is open

  int16_t filtered[SAMPLES_256] = {0};
  //Uncomment each line to use the filter (not all at once)
  //KickFilters::highpass(PPG_HR_CLEAN_01, filtered, SAMPLES_256, 1, Fs1_MS_KICK);
  //KickFilters::lowpass(PPG_HR_CLEAN_01, filtered, SAMPLES_256, 1, Fs1_MS_KICK);
  //KickFilters::movingAverage(PPG_HR_CLEAN_01, filtered, SAMPLES_256, 5);

  
  //Print to Serial Plotter to display
  for(uint16_t i = 0; i < SAMPLES_256; i++)
  {
    Serial.print(PPG_HR_CLEAN_01[i]); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.print(","); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.print(filtered[i]); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.println(); //Use SerialUSB for SparkFun SAMD21 boards

    delay(25);
  }
}


void loop()
{
}
