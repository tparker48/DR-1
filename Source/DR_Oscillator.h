#pragma once

#include "JuceHeader.h"

class DR_Oscillator
{
public:

	DR_Oscillator() {}

	~DR_Oscillator() {};

	void prepare(double sampleRate);

	void setFrequency(float frequency);

	void setVibrato(float vibratoAmount, float vibratoHz);

	void setParams(bool is_on, float glide, float octave, float detune) { this->is_on = is_on; this->glide = glide; this->octave = octave, this->detune = detune; }

	void processBlock(AudioSampleBuffer& buffer, int startSample, int numSamples, float gain);

	void setIsVibMaster(bool isVibMaster) { this->isVibMaster = isVibMaster; }

private:
	float saw();
	void incrementPhase();
	void updateDelta();

	// oscillator parameters
	bool is_on;
	float glide;
	float octave;
	float detune;

	double fs, hz;
	double phase, delta, delta_target, delta_speed;

	static double vibrato_phase;
	double vibrato_amt, vibrato_delta;
	bool isVibMaster = false;
};