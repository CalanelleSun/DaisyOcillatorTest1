#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

Oscillator osc1;
// Led led1;
Switch button1;
AdEnv env1;

// float osc1ModFreq;
// float filterCutoff;


void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	float osc1_out, env_out, pot1;
	
	
	button1.Debounce();

	if(button1.RisingEdge())
    {
        env1.Trigger(); //trigger the envelope
    }

	// pot1 = hw.adc.GetFloat(0);		
	osc1.SetFreq(mtof(hw.adc.GetFloat(0) * 127)); //midi to freq
	
	
	for (size_t i = 0; i < size; i++)
	{
		env_out = env1.Process(); //get the next envelope value
		osc1.SetAmp(env_out);
		osc1_out = osc1.Process(); //get next oscillator sample
	
		out[0][i] = osc1_out;
		
	}
}


int main(void)
{
	hw.Configure();	
	hw.Init();
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.SetAudioBlockSize(4); // number of samples handled per callback	
	float samplerate = hw.AudioSampleRate();
	
	AdcChannelConfig adcConfig; //adc cofiguration stored here
	adcConfig.InitSingle(hw.GetPin(21)); //configure GPIO21 as adc input
		
	// // led1.Init(hw.GetPin(1), false); //init led, false = uninverted value	
	
	button1.Init(hw.GetPin(15), samplerate / 48.f); //initialize button1 with 1khz update rate

	hw.adc.Init(&adcConfig,1); //initialize the adc with the above config

	osc1.Init(samplerate);
	osc1.SetWaveform(osc1.WAVE_TRI);
	osc1.SetAmp(1.f);
	osc1.SetFreq(300);
	 
	env1.Init(samplerate);
	env1.SetTime(ADENV_SEG_ATTACK,0.01);
	env1.SetTime(ADENV_SEG_DECAY, 0.4);
	env1.SetMin(0.0);
	env1.SetMax(1.0);
	env1.SetCurve(0);// linear
	
	hw.adc.Start(); //start reading values
	hw.StartAudio(AudioCallback); //call audio callback
	

	while(1) {
	// led1.Set(pot1);
	// led1.Update();

	}
}
