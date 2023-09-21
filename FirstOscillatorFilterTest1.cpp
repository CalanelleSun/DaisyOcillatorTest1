#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

Fm2 fmOsc1;

float osc1CarFreq;
float osc1ModFreq;
float filterCutoff;


void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{

	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = fmOsc1.Process();
		
	}
}

void ProcessADC(){
	AdcChannelConfig adcConfig;
	adcConfig.InitSingle(hw.GetPin(21));
	hw.adc.Init(&adcConfig,1);
	hw.adc.Start();
	
}

int main(void)
{
	hw.Configure();	
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	
	
	hw.StartAudio(AudioCallback);

	fmOsc1.Init(hw.AudioSampleRate);
	
	

	while(1) {
		fmOsc1.SetFrequency(hw.adc.GetFloat(0));
	}
}
