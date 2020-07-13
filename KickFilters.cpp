/*
 FILENAME:	KickFilters.cpp
 AUTHOR:	Orlando S. Hoilett and Akio K. Fujita
 EMAIL:		orlandohoilett@gmail.com
 VERSION:	1.3.0
 
 
 AFFILIATIONS
 Linnes Lab, Weldon School of Biomedical Engineering,
 Purdue University, West Lafayette, IN 47907
 
 
 DESCRIPTION
 This is a static class so funciton calls must be preceeded with "KickFilters::".
 
 This class includes a few basic digital filter funcions.
 
 
 UPDATES
 Version 0.0.0
 2020/02/19:1200>
			- Outlining
 Version 1.0.0
 2020/03/04:1200>
 			- Initial release.
 Version 1.0.1
 2020/03/12:1748>
 			- Updated descriptions and comments. Also reorganized folder
 			structures to comply wit Arduino library guidelines by using
 			"extras" folder to store supplementary info for the class.
 2020/03/12:1813>
 			- Fixed moving average filter by resetting the value of sum.
 			- Made change to lowpass filer function to multiply the first value of
 				the output by alpha in accordance with the definition of the function.
 			- Fixed highpass filter function to add tau to dt not multiply
 2020/03/23:1032>
 			- Updated comments.
 			- Added future updates section.
 Version 1.1.0
 2020/04/23:1200>
 			- Added some peak detector functions
 Version 1.2.0
 2020/05/21:1200>
 			- Added a bandpass filter function that simply calls the lowpass
				filter and highpass filter functions.
 Version 1.2.1
 2020/06/14:1500> (UTC-5)
 			- fixed an error in the bandpass filter function by adding an
 			additional tempArray input that allows going from the lowpass to the
 			highpass filter functions.
 Version 1.3.0
 2020/06/15:1616> (UTC-5)
 			- added a non-functioning median filter function.
 2020/07/11:0707> (UTC-)
 			- updated comments.


 
 FUTURE UPDATES TO INCLUDE
 1. Making this a templated class, meaning it will accept any data type for the
 	data to be filtered.
 2. Open-sourcing and moving to GitHub instead of lab's private BitBucket.
 3. A moving average filter function that takes cut-off frequency as an input.
 4. Adjusting moving average filter such that it appropriately deals with
 	samples on the trailing end of the filter (or leading end). Right now, the
 	filter works by averaging samples in front of the current index, which leaves
 	a few samples hanging at the end of the input data array.
 5. Considering returning a bode plot, phase lag, or something with the functions.
 6. Look into making bandpass filter function more efficient.
 7. Add notch filter
 
 
 
 DISCLAIMER
 Linnes Lab code, firmware, and software is released under the
 MIT License (http://opensource.org/licenses/MIT).
 
 The MIT License (MIT)
 
 Copyright (c) 2020 Linnes Lab, Purdue University
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
*/



#include "KickFilters.h"



//void KickFilters::highpass(const int16_t input[], int16_t output[], uint16_t samples, float fc, uint16_t dt)
//input			data array...declared as const so it's read-only
//output		result of filter gets stored in this array. Not declared as
//					const so it is eligible for both read and write
//samples		number of samples in array
//fc			desired cut-off frequency of the filter (in Hertz)
//dt			sampling period (ms)...time between samples in milliseconds
//
//Implements a simple first-order high pass filter according to the algorithm
//here: https://en.wikipedia.org/wiki/High-pass_filter
//The Wikipedia article is also located in the library in the "extras/references/" folder.
void KickFilters::highpass(const int16_t input[], int16_t output[], uint16_t samples, float fc, uint16_t dt)
{
	//Tau = Resistance(R)*Capacitance(C)
	//re-arranging the cut-off frequency equaion [1/(2*pi*R*C)] to solve for R*C
	float tau = 1/(2.0*PI*fc);
	float alpha = tau / (tau + (dt/1000.0));

	output[0] = input[0];
	for (uint16_t i = 1; i < samples; i++)
	{
		output[i] = alpha*(output[i-1] + input[i] - input[i-1]);
	}
}



//void KickFilters::lowpass(const int16_t input[], int16_t output[], uint16_t samples, float fc, uint16_t dt)
//input			data array...declared as const so it's read-only
//output		result of filter gets stored in this array. Not declared as
//					const so it is eligible for both read and write
//samples		number of samples in array
//fc			desired cut-off frequency of the filter (in Hertz)
//dt			sampling period (ms)...time between samples in milliseconds
//
//Implements a simple first-order low pass filter according to the algorithm
//here: https://en.wikipedia.org/wiki/Low-pass_filter
//The Wikipedia article is also located in the library in the "extras/references/" folder.
void KickFilters::lowpass(const int16_t input[], int16_t output[], uint16_t samples, float fc, uint16_t dt)
{
	//Tau = Resistance*Capacitance
	//re-arranging the cut-off frequency equaion [1/(2*pi*R*C)] to solve for R*C
	float tau = 1/(2.0*PI*fc);
	float alpha = (dt/1000.0) / (tau + (dt/1000.0));

	output[0] = alpha*input[0];
	for (uint16_t i = 1; i < samples; i++)
	{
		output[i] = output[i-1] + alpha*(input[i] - output[i-1]);
	}
}



//void KickFilters::bandpass(const int16_t input[], int16_t output[], int16_t tmpArray[],
//							const uint16_t samples, float fc1, float fc2, uint16_t dt)
//input			data array...declared as const so it's read-only
//output		result of filter gets stored in this array. Not declared as
//					const so it is eligible for both read and write
//tmpArray		tmpArray for transitioning from low pass to high pass filters
//samples		number of samples in array
//fc1			cutoff frequency of high pass filter (in Hertz)
//fc2			cutoff frequency of low pass filter (in Hertz)
//dt			sampling period (ms)...time between samples in milliseconds
//
//Implements a simple first-order bandpass filter by combing a lowpass and
//highpass filters described here and implemented in this class
//here: https://en.wikipedia.org/wiki/Low-pass_filter
//here: https://en.wikipedia.org/wiki/High-pass_filter
//The Wikipedia articles are also located in the library in the "extras/references/" folder.
void KickFilters::bandpass(const int16_t input[], int16_t output[], int16_t tmpArray[],
						   const uint16_t samples, float fc1, float fc2, uint16_t dt)
{
	lowpass(input, tmpArray, samples, fc2, dt);
	highpass(tmpArray, output, samples, fc1, dt);
}



//void KickFilters::movingAverage(const int16_t input[], int16_t output[], uint16_t samples, uint16_t order)
//input			data array...declared as const so it's read-only
//output		result of filter gets stored in this array. Not declared as
//					const so it is eligible for both read and write
//samples		number of samples in array
//order			filter order...how many samples should be averaged
//
//Implements a simple moving average filter. Implemented such that the last few
//outputs of the filter (equal to the order of the filter) are not usable.
void KickFilters::movingAverage(const int16_t input[], int16_t output[], uint16_t samples, uint16_t order)
{
	float sum = 0;
	
	for(uint16_t i = 0; i < samples-order; i++)
	{
		sum = 0;
		for(uint16_t j = i; j < i+order; j++)
		{
			sum += input[j];
		}
		
		output[i] = (int16_t)(sum / order);
	}
}


//https://www.mathworks.com/help/signal/ref/medfilt1.html
//void KickFilters::medianFilter1(const int16_t input[], int16_t output[], int16_t tempArray[],
//								const uint16_t samples, const uint16_t order)
//{
//	for(uint16_t i = order/2; i < samples-(order/2) i++)
//	{
//
//
//		output[i] = KickMath::calcMedian()
//	}
//}
