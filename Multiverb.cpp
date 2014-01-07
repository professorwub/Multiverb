#include "Multiverb.h"
#include "DelayLine.h"
#include <math.h>

static int delay[] = {// pre-determined sample delays
	439,
	997,
	1601,
	2251,
	2897,
	3583,
	4271,
	4999
};

Multiverb::Multiverb() {
	sampRate = 44100.0; // just in case!
	for (int i = 0; i < ORDER; i++) {// initialize delay lines and state vectors
		east[i].create(delay[i]);
		west[i].create(delay[i]);
		xPlus[i] = 0.0;
		yPlus[i] = 0.0;
	}
}
Multiverb::~Multiverb() {
}

void Multiverb::SetSampleRate(float sRate) {
	sampRate = sRate;
}

void Multiverb::SetLFReverbTime(float t60lf) {
	m_t60lf = t60lf;
}

void Multiverb::SetHFReverbTime(float t60hf) {
	m_t60hf = t60hf;
}

void Multiverb::SetDryGain(float dryGain) {
	m_dryGain = dryGain;
}

void Multiverb::SetWetGain(float wetGain) {
	m_wetGain = wetGain;
}

void Multiverb::SetStereoSep(float stereoSep) {
	m_stereoSep = stereoSep;
}

void Multiverb::Update()
{// update derived parameters
	float alf, ahf;

	dGain = float(pow(10.0, m_dryGain / 20.0));
	wGain = float(pow(10.0, m_wetGain / 20.0));
	right = wGain * (1.0 - m_stereoSep / 100.0) / 2.0;
	left = wGain * (1.0 + m_stereoSep / 100.0) / 2.0;
	for (int i = 0; i < ORDER; i++) { // update attenuation and damping
		alf = float(pow(10.0, -3.0 * float(delay[i]) / (sampRate * m_t60lf)));
		ahf = float(pow(10.0, -3.0 * float(delay[i]) / (sampRate * m_t60hf)));
		damp[i] = (alf - ahf) / (alf + ahf);
		atten[i] = alf * (1.0 - damp[i]);
	}
}

void Multiverb::Reset(void)
{
	for (int i = 0; i < ORDER; i++) {// reset buffers
		east[i].clear();
		west[i].clear();
		xPlus[i] = 0.0;
		yPlus[i] = 0.0;
	}
}

void Multiverb::ClockProcess(float* LeftSample, float* RightSample)
{// process one sampling period
	float xPlusTotal = 0.0;
	float yPlusTotal = 0.0;
	for (int i = 0; i < ORDER; i++) {
		xPlus[i] = damp[i] * xPlus[i] + atten[i] * west[i].get();
		yPlus[i] = damp[i] * yPlus[i] + atten[i] * east[i].get();
		xPlusTotal += xPlus[i];
		yPlusTotal += yPlus[i];
	}
	xPlusTotal /= float(ORDER);
	yPlusTotal /= float(ORDER);
	for (int i = 0; i < ORDER; i++) {
		east[i].push(xPlusTotal - xPlus[i] + *LeftSample);
		west[i].push(yPlusTotal - yPlus[i] + *RightSample);
	}
	*LeftSample = right * xPlusTotal + left * yPlusTotal + dGain * *LeftSample;
	*RightSample = left * xPlusTotal + right * yPlusTotal + dGain * *RightSample;
}