// Reverb effect based on network of multi-port acoustic elements.
// Warren L. G. Koontz
#include "DelayLine.h"

#define ORDER 8 // number of waveguide pairs

class Multiverb
{
public:
Multiverb();
~Multiverb();

//Parameters
void SetSampleRate(float sRate);
void SetLFReverbTime(float t60lf);
float GetLFReverbTime(void){return m_t60lf;};
void SetHFReverbTime(float t60hf);
float GetHFReverbTime(void){return m_t60hf;};
void SetDryGain(float dryGain);
float GetDryGain(void){return m_dryGain;};
void SetWetGain(float wetGain);
float GetWetGain(void){return m_wetGain;};
void SetStereoSep(float stereoSep);
float GetStereoSep(void){return m_stereoSep;};

//Use
void Update(void);
void Reset(void);
void ClockProcess(float* LeftSample, float* RightSample); 

private:
float sampRate; // obtained from host
// user set parameters
float m_t60lf;
float m_t60hf;
float m_dryGain;
float m_wetGain;
float m_stereoSep;
// derived parameters
float dGain;
float wGain;
float right;
float left;
float atten[ORDER];
float damp[ORDER];
// delay line pairs
DelayLine east[ORDER];
DelayLine west[ORDER];
// state vectors
float xPlus[ORDER];
float yPlus[ORDER];
};