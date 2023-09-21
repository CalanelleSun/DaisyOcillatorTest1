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
	

	Led led1;	
	led1.Init(hw.GetPin(1), false); //false = uninverted value

	AdcChannelConfig adcConfig; //adc cofiguration stored here
	adcConfig.InitSingle(hw.GetPin(21)); //configure GPIO21 as adc input 
	
	hw.adc.Init(&adcConfig,1); //initialize the adc with the above config
	hw.adc.Start(); //start reading values	
}

void OscSettings(){
	//fmOsc1.Init(samplerate);

}

int main(void)
{
	hw.Configure();	
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	float samplerate = hw.AudioSampleRate();

	ProcessADC();	
	
	hw.StartAudio(AudioCallback);

	
	
	

	while(1) {
		fmOsc1.SetFrequency(hw.adc.GetFloat(0));
	}
}
