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
    // 
    // auto knob00 = std::make_unique<AudioParameterFloat>("knob00", "k00",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.9f);
    // 
    // auto knob01 = std::make_unique<AudioParameterFloat>("knob01", "k01",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.9f);
    // 
    // auto knob10 = std::make_unique<AudioParameterFloat>("knob10", "k10",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.9f);
    // 
    // auto knob11 = std::make_unique<AudioParameterFloat>("knob11", "k11",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.9f);
    // 
    // auto knob20 = std::make_unique<AudioParameterFloat>("knob20", "k20",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.9f);
    // 
    // auto knob21 = std::make_unique<AudioParameterFloat>("knob21", "k21",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.9f);
    // 
    // auto knob30 = std::make_unique<AudioParameterFloat>("knob30", "k30",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.9f);
    // 
    // auto knob31 = std::make_unique<AudioParameterFloat>("knob31", "k31",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.9f);
    // 
    // auto knob40 = std::make_unique<AudioParameterFloat>("knob40", "k40",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.9f);
    // 
    // auto knob41 = std::make_unique<AudioParameterFloat>("knob41", "k41",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.9f);
    // 
    // 
    // auto toggle0 = std::make_unique<AudioParameterFloat>("toggle0", "t0", 0.0f, 1.0f, 1.0f);
    // auto toggle1 = std::make_unique<AudioParameterFloat>("toggle1", "t1", 0.0f, 1.0f, 1.0f);
    // auto toggle2 = std::make_unique<AudioParameterFloat>("toggle2", "t2", 0.0f, 1.0f, 1.0f);
    // auto toggle3 = std::make_unique<AudioParameterFloat>("toggle3", "t3", 0.0f, 1.0f, 1.0f);
    // auto toggle4 = std::make_unique<AudioParameterFloat>("toggle4", "t4", 0.0f, 1.0f, 1.0f);
    
    // params.push_back(std::move(knob00));
    // params.push_back(std::move(knob01));
    // params.push_back(std::move(knob10));
    // params.push_back(std::move(knob11));
    // params.push_back(std::move(knob20));
    // params.push_back(std::move(knob21));
    // params.push_back(std::move(knob30));
    // params.push_back(std::move(knob31));
    // params.push_back(std::move(knob40));
    // params.push_back(std::move(knob41));
    // params.push_back(std::move(toggle0));
    // params.push_back(std::move(toggle1));
    // params.push_back(std::move(toggle2));
    // params.push_back(std::move(toggle3));
    // params.push_back(std::move(toggle4));
    return { params.begin(), params.end() };
}

juce::AudioProcessorEditor* DR1AudioProcessor::createEditor() { return new DR1AudioProcessorEditor(*this, vts); };
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new DR1AudioProcessor(); }
