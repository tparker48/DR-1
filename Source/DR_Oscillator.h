#pragma once

#include "JuceHeader.h"

class DR_Oscillator
{
public:

	DR_Oscillator() {}

	~DR_Oscillator() {};

	void prepare(double sampleRate);

	void setFrequency(float frequency);

	void setParams(bool is_on, float glide, float detune) { this->is_on = is_on; this->glide = glide; this->detune = detune; }

	void processBlock(AudioSampleBuffer& buffer, int startSample, int numSamples, float gain);

private:
	float tri();
	void incrementPhase();

	bool is_on;
	float glide;
	float detune;

	double fs, hz, phase, delta;
};