#pragma once

#include "JuceHeader.h"

class DR_Oscillator
{
public:

	DR_Oscillator() {}

	~DR_Oscillator() {};

	void prepare(double sampleRate);

	void setFrequency(float frequency);

	void setParams(bool is_on, float glide, float octave, float detune) { this->is_on = is_on; this->glide = glide; this->octave = octave, this->detune = detune; }

	void processBlock(AudioSampleBuffer& buffer, int startSample, int numSamples, float gain);

private:
	float saw();
	void incrementPhase();
	void updateDelta();

	// oscillator parameters
	bool is_on;
	float glide;
	float octave;
	float detune;

	double fs, hz, phase;
	double delta, delta_target, delta_speed;
};