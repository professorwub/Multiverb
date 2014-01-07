#include <lv2plugin.hpp>
#include "Mverb.peg"
#include "Multiverb.h"

#define ORDER 8

using namespace LV2;

class Mverb : public Plugin<Mverb> {
    public:

    Mverb(double rate) : Plugin<Mverb>(p_n_ports) {
	reverb.SetSampleRate(float(rate));
    }


    void activate() {
        //cout << "Entered activate" << endl;
        reverb.Reset();
    }

    void run(uint32_t nframes) {
	// Update user parameters
	reverb.SetLFReverbTime(*p(p_revTimeLF));
	reverb.SetHFReverbTime(*p(p_revTimeHF));
	reverb.SetDryGain(*p(p_dryGain));
	reverb.SetWetGain(*p(p_wetGain));
	reverb.SetStereoSep(*p(p_stereoSep));
	// update derived parameters
	reverb.Update();
        
        for (uint32_t n = 0; n < nframes; n++) {
            reverb.ClockProcess(&p(p_left_input)[n], &p(p_right_input)[n]);
	    p(p_left_output)[n] = p(p_left_input)[n];
	    p(p_right_output)[n] = p(p_right_input)[n];
        }
    }

    private:

    Multiverb reverb;

};

static int _ = Mverb::register_class(p_uri);


