#include "PluginProcessor.h"
#include "PluginEditor.h"

DR1AudioProcessor::DR1AudioProcessor()
     : AudioProcessor (BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
       vts(*this, nullptr, "parameters", createParameterLayout())
{
    // mono
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());

}

DR1AudioProcessor::~DR1AudioProcessor() {}
void DR1AudioProcessor::setCurrentProgram (int index) {}
const juce::String DR1AudioProcessor::getProgramName (int index) { return {}; }
void DR1AudioProcessor::changeProgramName (int index, const juce::String& newName) {}


void DR1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getNumOutputChannels());
        }
    }
}

void DR1AudioProcessor::releaseResources() {}

bool DR1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}


void DR1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; i++)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            
            // auto isOn0 = vts.getRawParameterValue("toggle0");
            // auto isOn1 = vts.getRawParameterValue("toggle1");
            // auto isOn2 = vts.getRawParameterValue("toggle2");
            // auto isOn3 = vts.getRawParameterValue("toggle3");
            // auto isOn4 = vts.getRawParameterValue("toggle4");
            // 
            // auto glide0 = vts.getRawParameterValue("knob00");
            // auto glide1 = vts.getRawParameterValue("knob10");
            // auto glide2 = vts.getRawParameterValue("knob20");
            // auto glide3 = vts.getRawParameterValue("knob30");
            // auto glide4 = vts.getRawParameterValue("knob40");
            // 
            // auto detune0 = vts.getRawParameterValue("knob01");
            // auto detune1 = vts.getRawParameterValue("knob11");
            // auto detune2 = vts.getRawParameterValue("knob21");
            // auto detune3 = vts.getRawParameterValue("knob31");
            // auto detune4 = vts.getRawParameterValue("knob41");
            // 
            // voice->setParameters(0, *isOn0, *glide0, *detune0);
            // voice->setParameters(1, *isOn1, *glide1, *detune1);
            // voice->setParameters(2, *isOn2, *glide2, *detune2);
            // voice->setParameters(3, *isOn3, *glide3, *detune3);
            // voice->setParameters(4, *isOn4, *glide4, *detune4);

            for (int i = 0; i < 5; i++)
            {
                auto toggle = vts.getRawParameterValue("toggle" + std::to_string(i));
                auto glide = vts.getRawParameterValue("glide" + std::to_string(i));
                auto oct = vts.getRawParameterValue("oct" + std::to_string(i));
                auto detune = vts.getRawParameterValue("detune" + std::to_string(i));
                voice->setOscParameters(i, *toggle, *glide, *oct, *detune);
            }

            auto filterCutoff = vts.getRawParameterValue("filterCutoff");
            auto filterResonance = vts.getRawParameterValue("filterResonance");
            auto filterCrunch = vts.getRawParameterValue("filterCrunch");
            auto vibratoAmount = vts.getRawParameterValue("vibratoAmount");
            auto vibratoHz = vts.getRawParameterValue("vibratoHz");
            voice->setGlobalParameters(*filterCutoff, *filterResonance, *filterCrunch, *vibratoAmount, *vibratoHz);
        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

void DR1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = vts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DR1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(vts.state.getType()))
            vts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

AudioProcessorValueTreeState::ParameterLayout DR1AudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    
    for (int i = 0; i < 5; i++)
    {
        auto glide = std::make_unique<AudioParameterFloat>("glide" + std::to_string(i), "g" + std::to_string(i),
            NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.6f);

        auto oct = std::make_unique<AudioParameterFloat>("oct" + std::to_string(i), "o" + std::to_string(i),
            NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.5f);

        auto detune = std::make_unique<AudioParameterFloat>("detune" + std::to_string(i), "d" + std::to_string(i),
            NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.5f);

        auto toggle = std::make_unique<AudioParameterFloat>("toggle" + std::to_string(i), "t" + std::to_string(i), 0.0f, 1.0f, 1.0f);

        params.push_back(std::move(glide));
        params.push_back(std::move(oct));
        params.push_back(std::move(detune));
        params.push_back(std::move(toggle));
    }
    
    auto filterCutoff = std::make_unique<AudioParameterFloat>("filterCutoff", "fc",
        NormalisableRange<float>(0.0f, 1.0f, 0.0f, 0.3f), 0.75f);

    auto filterResonance = std::make_unique<AudioParameterFloat>("filterResonance", "fr",
        NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.0f);

    auto filterCrunch = std::make_unique<AudioParameterFloat>("filterCrunch", "fcr",
        NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.0f);

    auto vibratoAmount = std::make_unique<AudioParameterFloat>("vibratoAmount", "va",
        NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.1f);

    auto vibratoHz = std::make_unique<AudioParameterFloat>("vibratoHz", "vhz",
        NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.1f);
    
    params.push_back(std::move(filterCutoff));
    params.push_back(std::move(filterResonance));
    params.push_back(std::move(filterCrunch));
    params.push_back(std::move(vibratoAmount));
    params.push_back(std::move(vibratoHz));

    return { params.begin(), params.end() };
}

juce::AudioProcessorEditor* DR1AudioProcessor::createEditor() { return new DR1AudioProcessorEditor(*this, vts); };
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new DR1AudioProcessor(); }
