#include "PluginProcessor.h"
#include "PluginEditor.h"

DR1AudioProcessorEditor::DR1AudioProcessorEditor(DR1AudioProcessor& p, AudioProcessorValueTreeState& valTreeState)
    : AudioProcessorEditor(&p), audioProcessor(p), vts(valTreeState)
{
    this->addMouseListener(new MouseListener, false);

    double ratio = 7.0 / 4.0;
    setResizeLimits(400, 400 / ratio, 1600, 1600 / ratio);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize(900.0, 900.0 / ratio);
    
    dark = Colour(45, 45, 40);
    light = Colour(200, 200, 180);
    initLookAndFeels();

    for (int i = 0; i < NUM_OSCS; i++)
    {
        initializeButton(toggle[i], toggleP[i], "toggle" + std::to_string(i));
        initializeKnob(glide[i], glideP[i], "glide" + std::to_string(i));
        initializeKnob(oct[i], octP[i], "oct" + std::to_string(i));
        initializeKnob(detune[i], detuneP[i], "detune" + std::to_string(i));
    }

    initializeKnob(filterCutoff, filterCutoffP, "filterCutoff");
    initializeKnob(filterResonance, filterResonanceP, "filterResonance");
    initializeKnob(filterCrunch, filterCrunchP, "filterCrunch");
    initializeSlider(vibratoAmount, vibratoAmountP, "vibratoAmount");
    initializeSlider(vibratoHz, vibratoHzP, "vibratoHz");

    initializeKnobSensitivities();

    resized();
}

DR1AudioProcessorEditor::~DR1AudioProcessorEditor()
{
    for (int i = 0; i < NUM_OSCS; i++)
    {
        glide[i].setLookAndFeel(nullptr);
        oct[i].setLookAndFeel(nullptr);
        detune[i].setLookAndFeel(nullptr);
    }

    filterCutoff.setLookAndFeel(nullptr);
    filterResonance.setLookAndFeel(nullptr);
    filterCrunch.setLookAndFeel(nullptr);
    vibratoAmount.setLookAndFeel(nullptr);
    vibratoHz.setLookAndFeel(nullptr);

    delete customDial;
    delete customButton;
}


void DR1AudioProcessorEditor::paint(Graphics& g)
{
    Image background = ImageCache::getFromMemory(BinaryData::ui_bg_png, BinaryData::ui_bg_pngSize);
    Rectangle<int> bounds = Rectangle<int>(0, 0, getWidth(), getHeight());
    g.drawImage(background, bounds.toFloat());
    // g.fillAll(Colour(189, 185, 172));
}


void DR1AudioProcessorEditor::resized()
{
    double w = getWidth();
    double h = getHeight();

    int dialSizeS = w * 0.07;
    int dialSizeM = w * 0.09;
    int dialSizeL = w * 0.11;
    int buttonSize = w * 0.05;

    int oscSpacingX = w * 0.14;
    int oscSpacingY = h * 0.25;
    int oscOffsetX = w * 0.05;
    int oscOffsetY = h * 0.12;

    for (int i = 0; i < NUM_OSCS; i++)
    {
        glide[i].setBounds  ( oscOffsetX + i * oscSpacingX, oscOffsetY + 0 * oscSpacingY, dialSizeM, dialSizeM   );
        oct[i].setBounds    ( oscOffsetX + i * oscSpacingX, oscOffsetY + 1 * oscSpacingY, dialSizeM, dialSizeM   );
        detune[i].setBounds ( oscOffsetX + i * oscSpacingX + 0.5*(dialSizeM-dialSizeS), oscOffsetY + 2 * oscSpacingY, dialSizeS, dialSizeS   );
        toggle[i].setBounds ( oscOffsetX + i * oscSpacingX + 0.5*(dialSizeM - buttonSize), oscOffsetY + 3 * oscSpacingY - 0.05*h, buttonSize, buttonSize );
    }

    int filterX = w * 0.8;
    filterCutoff.setBounds(filterX, h*0.15, dialSizeL, dialSizeL);
    filterResonance.setBounds(filterX + 0.5*(dialSizeL- dialSizeS) - w*0.0425, h * 0.36, dialSizeS, dialSizeS);
    filterCrunch.setBounds(filterX + 0.5 * (dialSizeL - dialSizeS) + w*0.0425, h * 0.36, dialSizeS, dialSizeS);
    
    int sliderW = w * 0.07;
    int sliderH = h * 0.35;
    int sliderX = w * 0.78;
    vibratoHz.setBounds(sliderX, h * .55, sliderW, sliderH);
    vibratoAmount.setBounds(sliderX + w * 0.082, h * .55, sliderW, sliderH);



}

void DR1AudioProcessorEditor::initLookAndFeels()
{
    getLookAndFeel().setColour(PopupMenu::ColourIds::backgroundColourId, dark);
    getLookAndFeel().setColour(PopupMenu::ColourIds::highlightedBackgroundColourId, dark.darker(0.95));
    getLookAndFeel().setColour(PopupMenu::ColourIds::textColourId, light);
    getLookAndFeel().setColour(PopupMenu::ColourIds::highlightedTextColourId, light);

    customSlider = new SliderLookAndFeel(dark, light);
    customDial = new DialLookAndFeel(dark, light);
    customButton = new ButtonLookAndFeel(dark, light);
}


void DR1AudioProcessorEditor::updateLookAndFeels()
{
    getLookAndFeel().setColour(PopupMenu::ColourIds::backgroundColourId, dark);
    getLookAndFeel().setColour(PopupMenu::ColourIds::highlightedBackgroundColourId, dark.darker(0.95));
    getLookAndFeel().setColour(PopupMenu::ColourIds::textColourId, light);
    getLookAndFeel().setColour(PopupMenu::ColourIds::highlightedTextColourId, light);

    customDial->setColors(dark, light);
    customButton->setColors(dark, light);
}


void DR1AudioProcessorEditor::initializeSlider(Slider& obj, std::unique_ptr<SliderAttachment>& objP, std::string name)
{
    addAndMakeVisible(obj);
    //objP.reset(new SliderAttachment(vts, name, obj));
    obj.setSliderStyle(Slider::LinearBarVertical);
    obj.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    obj.setPopupDisplayEnabled(false, false, this);
    obj.setLookAndFeel(customSlider);
}


void DR1AudioProcessorEditor::initializeKnob(Slider& obj, std::unique_ptr<SliderAttachment>& objP, std::string name)
{
    addAndMakeVisible(obj);
    obj.setVelocityBasedMode(true);
    obj.setSliderStyle(Slider::LinearBarVertical);
    //objP.reset(new SliderAttachment(vts, name, obj));
    obj.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    obj.setPopupDisplayEnabled(false, false, this);
    obj.setLookAndFeel(customDial);
}



void DR1AudioProcessorEditor::initializeButton(ToggleButton& obj, std::unique_ptr<ButtonAttachment>& objP, std::string name)
{
    addAndMakeVisible(obj);
    //objP.reset(new ButtonAttachment(vts, name, obj));
    obj.setClickingTogglesState(true);
    obj.setLookAndFeel(customButton);
}


void DR1AudioProcessorEditor::initializeKnobSensitivities()
{
    float knobSensitivity = 0.03;

    for (int i = 0; i < NUM_OSCS; i++)
    {
        glide[i].setVelocityModeParameters(1.0, 0, knobSensitivity, false);
        oct[i].setVelocityModeParameters(1.0, 0, knobSensitivity, false);
        detune[i].setVelocityModeParameters(1.0, 0, knobSensitivity, false);
    }

    filterCutoff.setVelocityModeParameters(1.0, 0, knobSensitivity, false);
    filterResonance.setVelocityModeParameters(1.0, 0, knobSensitivity, false);
    filterCrunch.setVelocityModeParameters(1.0, 0, knobSensitivity, false);
}