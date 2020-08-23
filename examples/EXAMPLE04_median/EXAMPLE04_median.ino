/*
 * FILENAME: EXAMPLE05_median.ino
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
 * Basic test of the KickFilters class to evaluate the median filter.
 * 
 * The input signal is a simulated photoplethysmogram used to
 * measure heart rate. Each major peak is a new heart beat. The
 * smaller peak is a dicrotic notch which is generated when
 * the aortic valve closes.
 * 
 * 
 * UPDATES
 * Version 1.0.0
 * 2020/08/22:1719>
 *           - Initial development
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


const uint16_t samples = 256;
//data is sampled every 42 milliseconds
//the input signal is a simulated photoplethysmogram used to measure
//heart rate. Each major peak is a new heart beat. The smaller peak is a
//dicrotic notch which is generated when the aortic valve closes.
const int16_t input[] = {773, 702, 515, 389, 370, 447, 554, 601, 561, 492, 455, 458, 480, 497, 545, 724, 801, 651, 460, 370, 396, 494, 579, 570, 507, 450, 433, 449, 480, 499, 500, 591, 756, 736, 548, 396, 356, 416, 522, 577, 549, 479, 436, 430, 455, 485, 502, 504, 496, 570, 737, 731, 540, 391, 362, 436, 539, 580, 543, 471, 428, 427, 450, 473, 486, 492, 485, 471, 486, 635, 748, 639, 455, 354, 363, 461, 560, 577, 518, 463, 443, 458, 482, 503, 521, 515, 499, 585, 735, 710, 536, 402, 368, 422, 520, 584, 551, 487, 444, 433, 448, 478, 496, 502, 505, 595, 753, 695, 514, 383, 366, 433, 529, 584, 562, 498, 447, 437, 454, 479, 494, 500, 530, 681, 752, 622, 450, 366, 384, 468, 557, 584, 534, 469, 435, 438, 464, 495, 517, 526, 634, 777, 730, 544, 402, 366, 424, 528, 602, 574, 499, 450, 440, 457, 481, 500, 520, 658, 781, 678, 485, 374, 374, 447, 547, 599, 563, 491, 442, 439, 459, 480, 495, 567, 742, 768, 599, 421, 350, 377, 481, 574, 590, 528, 458, 430, 438, 464, 488, 510, 640, 776, 689, 502, 379, 363, 437, 546, 605, 581, 507, 452, 438, 455, 478, 504, 633, 790, 740, 541, 390, 356, 418, 520, 588, 573, 506, 450, 430, 444, 472, 581, 770, 764, 558, 383, 327, 377, 477, 564, 573, 514, 452, 425, 428, 449, 556, 752, 762, 575, 398, 337, 385, 496, 595, 590, 522, 460, 440, 451, 484, 650, 810, 723, 521, 389};


void setup()
{
  Serial.begin(9600);
  while(!Serial); //will not run until Serial Monitor or Plotter is open


  int16_t filtered[samples] = {0};
  int16_t tmpArr[samples] = {0};
  int16_t tmpArr2[samples] = {0};
  
  //takes about 2.375 ms to run on an Arduino Due (SAM3X8E), Arduino v.1.8.12
  KickFilters<int16_t>::median(input, filtered, tmpArr, tmpArr2, samples, 3);

  
  //Print to Serial Plotter to display
  for(uint16_t i = 0; i < samples; i++)
  {
    Serial.print(input[i]);
    Serial.print(",");
    
    //adding some offset to make it visually
    //easier to observe in Serial Plotter
    Serial.print(400+filtered[i]);
    Serial.println();

    delay(15);
  }
}


void loop()
{
}
