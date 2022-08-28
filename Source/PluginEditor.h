#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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
    void initializeKnob(Slider& obj, std::unique_ptr<SliderAttachment>& objP, std::string name);
    void initializeButton(ToggleButton& obj, int toggleId);
    void initializeKnobSensitivities();

    void initLookAndFeels();
    void updateLookAndFeels();

    DR1AudioProcessor& audioProcessor;
    AudioProcessorValueTreeState& vts;
    
    Colour dark, light;

    ToggleButton toggle0, toggle1, toggle2, toggle3, toggle5, toggle8;
    Slider knob00, knob01, knob10, knob11, knob20, knob21;
    std::unique_ptr<SliderAttachment> knob00P, knob01P, knob10P, knob11P, knob20P, knob21P;


    DialLookAndFeel* customDial;
    ButtonLookAndFeel* customButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DR1AudioProcessorEditor)
};
