/*
 FILENAME:	KickFilters.h
 AUTHOR:	Orlando S. Hoilett, Benjamin D. Walters, and Akio K. Fujita
 EMAIL:		orlandohoilett@gmail.com
 VERSION:	2.1.0
 
 
 AFFILIATIONS
 Linnes Lab, Weldon School of Biomedical Engineering,
 Purdue University, West Lafayette, IN 47907
 
 
 DESCRIPTION
 This is a static, templated class, so funciton calls must be preceeded with
 "KickFilters<variable_type>::" where variable_type should be replaced with
 int16_t, int, float, etc.
 
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
 Version 2.0.0
 2020/08/21:1542> (UTC-5)
			- Added a notch filter.
 			- Moved to a templated class.
 Version 2.1.0
 2020/08/22:1726> (UTC-5)
			- Added a median filter
 
 
 FUTURE UPDATES TO INCLUDE
 1. Making this a templated class, meaning it will accept any data type for the
 data to be filtered.
 2. A moving average filter function that takes cut-off frequency as an input.
 3. Adjusting moving average filter such that it appropriately deals with
 samples on the trailing end of the filter (or leading end). Right now, the
 filter works by averaging samples in front of the current index, which leaves
 a few samples hanging at the end of the input data array.
 4. Considering returning a bode plot, phase lag, or something with the functions.
 5. Look into making bandpass filter function more efficient.
 
 
 
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


#ifndef KickFilters_h
#define KickFilters_h


//Standard Arduino libraries
#include <Arduino.h>

//Kick LL Libraries
#include "KickMath.h"


template<typename Type>


class KickFilters
{
	
public:
	
	static void highpass(const Type input[], Type output[], uint16_t samples, float fc, uint16_t dt);
	
	static void lowpass(const Type input[], Type output[], uint16_t samples, float fc, uint16_t dt);
	
	static void bandpass(const Type input[], Type output[], Type tmpArray[], const uint16_t samples, float fc1, float fc2, uint16_t dt);
	
	static void movingAverage(const Type input[], Type output[], uint16_t samples, uint16_t order);
	
	static void notch(const Type input[], Type output[], uint16_t samples, float fc, float fs);
	
	static void notch(const Type input[], Type output[], uint16_t samples, float fc, float fs, float r_coeff);

	static void median(const Type input[], Type output[], Type tempArray[], Type tempArray2[],
					   const uint16_t samples, const uint16_t order, const uint16_t window);
	
};


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
template<typename Type>
void KickFilters<Type>::highpass(const Type input[], Type output[], uint16_t samples, float fc, uint16_t dt)
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
template<typename Type>
void KickFilters<Type>::lowpass(const Type input[], Type output[], uint16_t samples, float fc, uint16_t dt)
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
template<typename Type>
void KickFilters<Type>::bandpass(const Type input[], Type output[], Type tmpArray[],
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
template<typename Type>
void KickFilters<Type>::movingAverage(const Type input[], Type output[], uint16_t samples, uint16_t order)
{
	float sum = 0;
	
	for(uint16_t i = 0; i < samples-order; i++)
	{
		sum = 0;
		for(uint16_t j = i; j < i+order; j++)
		{
			sum += input[j];
		}
		
		output[i] = (Type)(sum / order);
	}
}


//void KickFilters<Type>::notch(const Type input[], Type output[], uint16_t samples, float fc, float fs)
//input			data array...declared as const so it's read-only
//output		result of filter gets stored in this array. Not declared as
//					const so it is eligible for both read and write
//samples		number of samples in array
//fc			desired center frequency of the notch filter (the frequency to
//					be filtered)
//fs			sampling frequency (Hz) of signal being filtered
//
//Implements a simple notch filter. Adapted from Wang and Xiao - 2013 - Second-Order
//IIR Notch Filter Design and Implementation of Digital Signal Processing System.
//Ppaer is also stored in extras folder.
template<typename Type>
void KickFilters<Type>::notch(const Type input[], Type output[], uint16_t samples, float fc, float fs)
{
	//Notch filter parameters & constants
	float r = 0.8;
	float b0 = 1;
	float b1 = -2*cos(2*PI*fc/fs); //{Equation: -2.0*cos(2*PI*fc/float(fs));}
	float b2 = 1;
	float a1 = 2*r*cos(2*PI*fc/fs); //{Equation: 2*r*cos(2*PI*fc/float(fs));}
	float a2 = -(r*r);//{Equation: -1.0 * pow(r,2);}
	
	
	float bs_filter[2] = {0,0}; //stores previous filter outputs
	float bs_Val = 0; //Variable to hold most recent digital bandstop filter value
	
	
	//calculate filter output
	bs_Val = input[0] + (b1 * 0) + (b2 * 0) + (a1 * bs_filter[1]) + (a2 * bs_filter[0]);
	//update filter output values
	bs_filter[0] = 0;
	bs_filter[1] = bs_Val;
	//update output array
	output[0] = bs_Val;
	
	
	//calculate filter output
	bs_Val = input[1] + (b1 * input[0]) + (b2 * 0) + (a1 * bs_filter[1]) + (a2 * bs_filter[0]);
	//update filter output values
	bs_filter[0] = bs_filter[1];
	bs_filter[1] = bs_Val;
	//update output array
	output[1] = bs_Val;
	
	
	for(uint16_t i = 2; i < samples; i++)
	{
		//calculate filter output
		bs_Val = input[i] + (b1 * input[i-1]) + (b2 * input[i-2]) + (a1 * bs_filter[1]) + (a2 * bs_filter[0]);
		//update filter output values
		bs_filter[0] = bs_filter[1];
		bs_filter[1] = bs_Val;
		//update output array
		output[i] = bs_Val;
	}
}


//void KickFilters<Type>::notch(const Type input[], Type output[], uint16_t samples, float fc, float fs, float r_coeff)
//input			data array...declared as const so it's read-only
//output		result of filter gets stored in this array. Not declared as
//					const so it is eligible for both read and write
//samples		number of samples in array
//fc			desired center frequency of the notch filter (the frequency to
//					be filtered)
//fs			sampling frequency (Hz) of signal being filtered
//r_coeff		controls filter bandwith (wideness and steepness of the notch)
//					0.8 appears be most ideal, but the user may experiment
//					with the value if they so choose
//
//Implements a simple notch filter. Adapted from Wang and Xiao - 2013 - Second-Order
//IIR Notch Filter Design and Implementation of Digital Signal Processing System.
//Ppaer is also stored in extras folder.
template<typename Type>
void KickFilters<Type>::notch(const Type input[], Type output[], uint16_t samples, float fc, float fs, float r_coeff)
{
	//Notch filter parameters & constants
	float r = r_coeff;
	float b0 = 1;
	float b1 = -2*cos(2*PI*fc/fs); //{Equation: -2.0*cos(2*PI*fc/float(fs));}
	float b2 = 1;
	float a1 = 2*r*cos(2*PI*fc/fs); //{Equation: 2*r*cos(2*PI*fc/float(fs));}
	float a2 = -(r*r);//{Equation: -1.0 * pow(r,2);}
	
	
	float bs_filter[2] = {0,0}; //stores previous filter outputs
	float bs_Val = 0; //Variable to hold most recent digital bandstop filter value
	
	
	//calculate filter output
	bs_Val = input[0] + (b1 * 0) + (b2 * 0) + (a1 * bs_filter[1]) + (a2 * bs_filter[0]);
	//update filter output values
	bs_filter[0] = 0;
	bs_filter[1] = bs_Val;
	//update output array
	output[0] = bs_Val;
	
	
	//calculate filter output
	bs_Val = input[1] + (b1 * input[0]) + (b2 * 0) + (a1 * bs_filter[1]) + (a2 * bs_filter[0]);
	//update filter output values
	bs_filter[0] = bs_filter[1];
	bs_filter[1] = bs_Val;
	//update output array
	output[1] = bs_Val;
	
	
	for(uint16_t i = 2; i < samples; i++)
	{
		//calculate filter output
		bs_Val = input[i] + (b1 * input[i-1]) + (b2 * input[i-2]) + (a1 * bs_filter[1]) + (a2 * bs_filter[0]);
		
		//update filter output values
		bs_filter[0] = bs_filter[1];
		bs_filter[1] = bs_Val;
		
		//update output array
		output[i] = bs_Val;
	}
}


//https://www.mathworks.com/help/signal/ref/medfilt1.html
template<typename Type>
void KickFilters<Type>::median(const Type input[], Type output[], Type tempArray[], Type tempArray2[],
							   const uint16_t samples, const uint16_t order, const uint16_t window = 1)
{
	uint16_t outputIndex = 0;
	
	//Explicilty set empty spots in output array to 0
	for(uint16_t i = 0; i < order; i++)
	{
		output[i] = 0;
	}
	
	for(uint16_t i = order; i < samples; i += window)
	{
		for(uint16_t j = i; j < i+order; j++)
		{
			tempArray[j-i] = input[j];
		}
		
		output[outputIndex] = KickMath<Type>::calcMedian(order, tempArray, tempArray2);
		outputIndex++;
	}
}



#endif /* KickFilters_h */
