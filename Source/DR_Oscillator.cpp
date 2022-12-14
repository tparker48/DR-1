#include "DR_Oscillator.h"

double DR_Oscillator::vibrato_phase = 0.0;

void DR_Oscillator::prepare(double sampleRate)
{
	fs = sampleRate;
	
	hz = 0.0;
	phase = 0.0;
	delta = 0.0;

	vibrato_delta = 0.0;
}


void DR_Oscillator::setFrequency(float frequency)
{
	hz = frequency;
	delta_target = (hz * MathConstants<double>::twoPi) / fs;
	delta_speed = (delta_target - delta) / (3.0 * (glide+0.01) * fs);

	if (isVibMaster)
	{
		vibrato_phase = 0.0f;
	}
}


void DR_Oscillator::setVibrato(float vibratoAmount, float vibratoHz)
{
	vibrato_amt = vibratoAmount * 0.3;
	vibrato_delta = ((15.0*vibratoHz) * MathConstants<double>::twoPi) / fs;
	
	if (vibratoHz == 0.0f)
	{
		vibrato_amt = 0.0f;
	}
}


void DR_Oscillator::processBlock(AudioSampleBuffer& buffer, int startSample, int numSamples, float gain)
{
	for (int sample = 0; sample < numSamples; sample++)
	{
		for (int channel = 0; channel < buffer.getNumChannels(); channel++)
		{
			buffer.addSample(channel, sample, (float)is_on * gain * saw());
		}
		updateDelta();
		incrementPhase();
	}
}


void DR_Oscillator::incrementPhase()
{
	float oct_scale[5] = { 0.25f, 0.5f, 1.0f, 2.0f, 4.0f };
	float oct_multiplier = oct_scale[(int)(octave * 4)];
	float real_delta = delta * oct_multiplier;

	real_delta = real_delta + (vibrato_amt * sin(vibrato_phase) * real_delta);

	phase += real_delta + ((detune-0.5) * real_delta);
	if (phase > MathConstants<double>::twoPi) 
	{ 
		phase -= MathConstants<double>::twoPi; 
	}

	if (isVibMaster)
	{
		vibrato_phase += vibrato_delta;
		if (vibrato_phase > MathConstants<double>::twoPi)
		{
			vibrato_phase -= MathConstants<double>::twoPi;
		}
	}
}

void DR_Oscillator::updateDelta()
{
	if ((delta_speed > 0.0 && delta >= delta_target) || (delta_speed <= 0.0 && delta <= delta_target))
	{
		return;
	}

	delta += delta_speed;
}


float DR_Oscillator::saw()
{
	return 2.0f * (phase / MathConstants<float>::twoPi) - 1.0f;
}