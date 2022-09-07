#pragma once

#include "JuceHeader.h"
#include "SynthSound.h"
#include "DR_Oscillator.h"

class SynthVoice : public SynthesiserVoice
{
public:
	const int NUM_OSCS = 5;

	bool canPlaySound(SynthesiserSound*) override;
	void startNote(int midiNoteNumber, float velocity, SynthesiserSound * sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels);
	void renderNextBlock(AudioBuffer< float > & outputBuffer, int startSample, int numSamples) override;

	void setOscParameters(int osc_idx, bool is_on, float glide, float oct, float detune) { oscs[osc_idx]->setParams(is_on, glide, oct, detune); }
	void setGlobalParameters(float filterCutoff, float filterResonance, float filterCrunch, float vibratoAmount, float vibratoHz);

private:
	AudioBuffer<float> synthBuffer;

	DR_Oscillator** oscs;
	ADSR adsr;
	ADSR::Parameters adsr_params = { 0.444f, 0.0f, 1.0f, 2.0f };
	dsp::LadderFilter<float> lpf;
	dsp::Gain<float> gain;
	
	float hz;
	bool isPrepared;
};