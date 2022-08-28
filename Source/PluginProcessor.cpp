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
            // auto tremoloAmt = vts.getRawParameterValue("bigKnob");
            // auto tremoloHz = vts.getRawParameterValue("mediumKnob");
            // auto res1 = vts.getRawParameterValue("smallKnob1");
            // auto res2 = vts.getRawParameterValue("smallKnob2");
            // auto cutoffOffset = vts.getRawParameterValue("smallKnob3");
            // auto noiseLevel = vts.getRawParameterValue("slider");
            // auto crunchLevel= vts.getRawParameterValue("toggle");
            // 
            // voice->res1 = *res1;
            // voice->res2 = *res2;
            // voice->cutoffOffset = *cutoffOffset;
            // voice->setFilterCutoff(*filterCutoff);
            // voice->setFilterResonance(*filterResonance);
            // voice->setAttack(*attack);
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
    // auto bigKnob = std::make_unique<AudioParameterFloat>("bigKnob", "bK",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.5f);
    // 
    // auto mediumKnob = std::make_unique<AudioParameterFloat>("mediumKnob", "mK",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.4f);
    // 
    // auto smallKnob1 = std::make_unique<AudioParameterFloat>("smallKnob1", "sK1",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.9f);
    // 
    // auto smallKnob2 = std::make_unique<AudioParameterFloat>("smallKnob2", "sK2",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.9f);
    // 
    // auto smallKnob3 = std::make_unique<AudioParameterFloat>("smallKnob3", "sK3",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.9f);
    // 
    // auto slider = std::make_unique<AudioParameterFloat>("slider", "s",
    //     NormalisableRange<float>(0.0f, 1.0f, 0.0f), 0.3f);
    // 
    // auto toggle = std::make_unique<AudioParameterFloat>("toggle", "t", 1.0, 3.0, 3.0);
    // 
    // params.push_back(std::move(bigKnob));
    // params.push_back(std::move(mediumKnob));
    // params.push_back(std::move(smallKnob1));
    // params.push_back(std::move(smallKnob2));
    // params.push_back(std::move(smallKnob3));
    // params.push_back(std::move(slider));
    // params.push_back(std::move(toggle));
    // 
    return { params.begin(), params.end() };
}

juce::AudioProcessorEditor* DR1AudioProcessor::createEditor() { return new DR1AudioProcessorEditor(*this, vts); };
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new DR1AudioProcessor(); }
