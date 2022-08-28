#include "DR_Oscillator.h"

void DR_Oscillator::prepare(double sampleRate)
{
	fs = sampleRate;
	
	hz = 0.0;
	phase = 0.0;
	delta = 0.0;
}


void DR_Oscillator::setFrequency(float frequency)
{
	hz = frequency;
	delta = (hz * MathConstants<double>::twoPi) / fs;
}



void DR_Oscillator::processBlock(AudioSampleBuffer& buffer, int startSample, int numSamples, float gain)
{
	for (int sample = 0; sample < numSamples; sample++)
	{
		for (int channel = 0; channel < buffer.getNumChannels(); channel++)
		{
			buffer.addSample(channel, sample, gain * tri());
		}
		incrementPhase();
	}
}


void DR_Oscillator::incrementPhase()
{
	phase += delta;
	if (phase > MathConstants<double>::twoPi) 
	{ 
		phase -= MathConstants<double>::twoPi; 
	}
}


float DR_Oscillator::tri()
{
	return 0.0f;
}