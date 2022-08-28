#include "PluginProcessor.h"
#include "PluginEditor.h"

DR1AudioProcessorEditor::DR1AudioProcessorEditor(DR1AudioProcessor& p, AudioProcessorValueTreeState& valTreeState)
    : AudioProcessorEditor(&p), audioProcessor(p), vts(valTreeState)
{
    this->addMouseListener(new MouseListener, false);

    double ratio = 4.0 / 7.0;
    setResizeLimits(400, 400 / ratio, 1600, 1600 / ratio);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize(400.0, 400.0 / ratio);
    
    dark = Colour(35, 35, 35);
    light = Colour(230, 230, 230);
    initLookAndFeels();

    initializeKnob(knob00, knob00P, "knob00");
    initializeKnob(knob01, knob01P, "knob01");
    initializeKnob(knob10, knob10P, "knob10");
    initializeKnob(knob11, knob11P, "knob11");
    initializeKnob(knob20, knob20P, "knob20");
    initializeKnob(knob21, knob21P, "knob21");

    initializeKnobSensitivities();

    resized();
}

DR1AudioProcessorEditor::~DR1AudioProcessorEditor()
{
    knob00.setLookAndFeel(nullptr);
    knob01.setLookAndFeel(nullptr);
    knob10.setLookAndFeel(nullptr);
    knob11.setLookAndFeel(nullptr);
    knob20.setLookAndFeel(nullptr);
    knob21.setLookAndFeel(nullptr);

    delete customDial;
    delete customButton;
}


void DR1AudioProcessorEditor::paint(Graphics& g)
{
    // int borderW = getWidth() * 0.02;
    // int borderH = getHeight() * 0.03;
    // Image background = ImageCache::getFromMemory(BinaryData::ui_sketch_bg_dot_png, BinaryData::ui_sketch_bg_dot_pngSize);
    // Rectangle<int> bounds = Rectangle<int>(borderW, borderH, getWidth() - 2 * borderW, getHeight() - 2 * borderH);

    g.fillAll(dark);
    // g.drawImage(background, bounds.toFloat());
}


void DR1AudioProcessorEditor::resized()
{
    double w = getWidth();
    double h = getHeight();

    int largeDialSize = w * 0.20;
    int mediumDialSize = w * 0.1;
    int smallDialSize = w * 0.075;
    int sliderW = h * 0.63;
    int sliderH = w * 0.05;
    int buttonSize = w * 0.045;

    // toggle1.setBounds(w * 0.06, h * 0.1, buttonSize, buttonSize);
    // toggle2.setBounds(w * 0.06, h * 0.25, buttonSize, buttonSize);
    // toggle3.setBounds(w * 0.06, h * 0.40, buttonSize, buttonSize);

    // bigKnob.setBounds(w * 0.325, h * 0.375, largeDialSize, largeDialSize);
    // mediumKnob.setBounds(w * 0.26, h * 0.17, mediumDialSize, mediumDialSize);
    // smallKnob1.setBounds(w * 0.65, h * 0.1, smallDialSize, smallDialSize);
    // smallKnob2.setBounds(w * 0.75, h * 0.1, smallDialSize, smallDialSize);
    // smallKnob3.setBounds(w * 0.85, h * 0.1, smallDialSize, smallDialSize);
}


void DR1AudioProcessorEditor::initLookAndFeels()
{
    getLookAndFeel().setColour(PopupMenu::ColourIds::backgroundColourId, dark);
    getLookAndFeel().setColour(PopupMenu::ColourIds::highlightedBackgroundColourId, dark.darker(0.95));
    getLookAndFeel().setColour(PopupMenu::ColourIds::textColourId, light);
    getLookAndFeel().setColour(PopupMenu::ColourIds::highlightedTextColourId, light);

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


void DR1AudioProcessorEditor::initializeKnob(Slider& obj, std::unique_ptr<SliderAttachment>& objP, std::string name)
{
    addAndMakeVisible(obj);
    obj.setVelocityBasedMode(true);
    obj.setSliderStyle(Slider::LinearBarVertical);
    // objP.reset(new SliderAttachment(vts, name, obj));
    obj.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    obj.setPopupDisplayEnabled(false, false, this);
    obj.setLookAndFeel(customDial);
}



void DR1AudioProcessorEditor::initializeButton(ToggleButton&obj, int toggleId)
{
    addAndMakeVisible(obj);
    obj.onClick = [this, toggleId] { /*updateToggleState(toggleId);*/ };
    obj.setRadioGroupId(1001);
    obj.setClickingTogglesState(true);
    obj.setLookAndFeel(customButton);
}


void DR1AudioProcessorEditor::initializeKnobSensitivities()
{
    float knobSensitivity = 0.03;

    knob00.setVelocityModeParameters(1.0, 0, knobSensitivity, false);
    knob01.setVelocityModeParameters(1.0, 0, knobSensitivity, false);
    knob10.setVelocityModeParameters(1.0, 0, knobSensitivity, false);
    knob11.setVelocityModeParameters(1.0, 0, knobSensitivity, false);
    knob20.setVelocityModeParameters(1.0, 0, knobSensitivity, false);
    knob21.setVelocityModeParameters(1.0, 0, knobSensitivity, false);
}