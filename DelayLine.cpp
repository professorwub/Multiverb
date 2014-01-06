/* 
 * File:   DelayLine.cpp
 * Author: Warren
 * 
 * Created on January 2, 2014, 12:24 PM
 */

#include <cstdlib>
#include "DelayLine.h"

DelayLine::DelayLine() {
	ptr = 0;
	line = NULL;
}

DelayLine::~DelayLine() {
	delete [] line;
}

void DelayLine::create(int n) {
	line = new float[n];
	for (int i = 0; i < n; i++)
		line[i] = 0.0;
	del = n;
}

void DelayLine::push(float x) {
	line[ptr++] = x;
	if (ptr >= del)
		ptr = 0;
}

float DelayLine::get(void) {
	return(line[ptr]);
}

void DelayLine::clear(void) {
	for (int i = 0; i < del; i++)
		line[i] = 0.0;
	ptr = 0;
}