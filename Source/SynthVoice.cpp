#include "SynthVoice.h"

bool SynthVoice::canPlaySound(SynthesiserSound* sound)
{
	return dynamic_cast<SynthesiserSound*>(sound) != nullptr;
}


void SynthVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition)  
{
	hz = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	adsr.noteOn();
}


void SynthVoice::stopNote(float velocity, bool allowTailOff)  
{
	adsr.noteOff();

	if (!allowTailOff || !adsr.isActive())
	{
		clearCurrentNote();
	}
}


void SynthVoice::pitchWheelMoved(int newPitchWheelValue)  
{}


void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)  
{}


void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels)
{
	oscs = new DR_Oscillator*[NUM_OSCS];
	for (int i = 0; i < NUM_OSCS; i++)
	{
		oscs[i] = new DR_Oscillator();
	}
	

	adsr.setSampleRate(sampleRate);
	adsr_params.attack = 0.04f;
	adsr_params.decay = 0.0f;
	adsr_params.sustain = 1.0f;
	adsr_params.release = 0.16f;

	dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = numOutputChannels;

	gain.prepare(spec);
	gain.setGainLinear(0.2f);

	isPrepared = true;
}


void SynthVoice::renderNextBlock(AudioBuffer< float >& outputBuffer, int startSample, int numSamples)  
{
	jassert (isPrepared);

	if (!isVoiceActive())
	{
		return;
	}

	synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	synthBuffer.clear();

	dsp::AudioBlock<float> audioBlock(synthBuffer);
	// noise.processBlock(synthBuffer, 0, synthBuffer.getNumSamples(), 1.0f);

	gain.process(dsp::ProcessContextReplacing<float>(audioBlock));
	adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

	for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
	{
		outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
		
		if (!adsr.isActive())
		{
			clearCurrentNote();
		}
	}
}
