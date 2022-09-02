#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "UI/SliderLookAndFeel.h"
#include "UI/DialLookAndFeel.h"
#include "UI/ButtonLookAndFeel.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

class DR1AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DR1AudioProcessorEditor (DR1AudioProcessor& , AudioProcessorValueTreeState&);
    ~DR1AudioProcessorEditor() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void initializeSlider(Slider& obj, std::unique_ptr<SliderAttachment>& objP, std::string name);
    void initializeKnob(Slider& obj, std::unique_ptr<SliderAttachment>& objP, std::string name);
    void initializeButton(ToggleButton& obj, std::unique_ptr<ButtonAttachment>& objP, std::string name);
    void initializeKnobSensitivities();

    void initLookAndFeels();
    void updateLookAndFeels();

    DR1AudioProcessor& audioProcessor;
    AudioProcessorValueTreeState& vts;
    
    Colour dark, light;

    static const int NUM_OSCS = 5;

    Slider glide[NUM_OSCS];
    Slider oct[NUM_OSCS];
    Slider detune[NUM_OSCS];
    ToggleButton toggle[NUM_OSCS];
    Slider filterCutoff, filterResonance, filterCrunch;
    Slider vibratoAmount, vibratoHz;

    std::unique_ptr<SliderAttachment> glideP[NUM_OSCS];
    std::unique_ptr<SliderAttachment> octP[NUM_OSCS];
    std::unique_ptr<SliderAttachment> detuneP[NUM_OSCS];
    std::unique_ptr<ButtonAttachment> toggleP[NUM_OSCS];
    std::unique_ptr<SliderAttachment> filterCutoffP, filterResonanceP, filterCrunchP;
    std::unique_ptr<SliderAttachment> vibratoAmountP, vibratoHzP;

    SliderLookAndFeel* customSlider;
    DialLookAndFeel* customDial;
    ButtonLookAndFeel* customButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DR1AudioProcessorEditor)
};
