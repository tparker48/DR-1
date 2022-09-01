#include "PluginProcessor.h"
#include "PluginEditor.h"

DR1AudioProcessorEditor::DR1AudioProcessorEditor(DR1AudioProcessor& p, AudioProcessorValueTreeState& valTreeState)
    : AudioProcessorEditor(&p), audioProcessor(p), vts(valTreeState)
{
    this->addMouseListener(new MouseListener, false);

    double ratio = 4.0 / 6.0;
    setResizeLimits(400, 400 / ratio, 1600, 1600 / ratio);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize(400.0, 400.0 / ratio);
    
    dark = Colour(45, 45, 40);
    light = Colour(200, 200, 180);
    initLookAndFeels();

    initializeKnob(knob00, knob00P, "knob00");
    initializeKnob(knob01, knob01P, "knob01");
    initializeKnob(knob10, knob10P, "knob10");
    initializeKnob(knob11, knob11P, "knob11");
    initializeKnob(knob20, knob20P, "knob20");
    initializeKnob(knob21, knob21P, "knob21");
    initializeKnob(knob30, knob30P, "knob30");
    initializeKnob(knob31, knob31P, "knob31");
    initializeKnob(knob40, knob40P, "knob40");
    initializeKnob(knob41, knob41P, "knob41");

    initializeButton(toggle0, toggle0P, "toggle0");
    initializeButton(toggle1, toggle1P, "toggle1");
    initializeButton(toggle2, toggle2P, "toggle2");
    initializeButton(toggle3, toggle3P, "toggle3");
    initializeButton(toggle4, toggle4P, "toggle4");

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
    knob30.setLookAndFeel(nullptr);
    knob31.setLookAndFeel(nullptr);
    knob40.setLookAndFeel(nullptr);
    knob41.setLookAndFeel(nullptr);

    delete customDial;
    delete customButton;
}


void DR1AudioProcessorEditor::paint(Graphics& g)
{
    Image background = ImageCache::getFromMemory(BinaryData::pre_ui_sketch_png, BinaryData::pre_ui_sketch_pngSize);
    Rectangle<int> bounds = Rectangle<int>(0, 0, getWidth(), getHeight());
    g.drawImage(background, bounds.toFloat());
    // g.fillAll(Colour(189, 185, 172));
}


void DR1AudioProcessorEditor::resized()
{
    double w = getWidth();
    double h = getHeight();

    int dialSize = w * 0.20;
    int buttonSize = w * 0.10;

    int toggleX = w * 0.125;
    int col1X = w * 0.35f;
    int col2X = w * 0.675f;

    int offset = h * 0.125f;
    int row1Y = h * 0.0f;
    int row2Y = h * (.7f) * (0.25f);
    int row3Y = h * (.7f) * (0.5f);
    int row4Y = h * (.7f) * (.75f);
    int row5Y = h * .7f;

    int toggleOffset = offset + (h * 0.0325f);
    int toggle1Y = h * 0.0f;
    int toggle2Y = h * (.7f) * (0.25f);
    int toggle3Y = h * (.7f) * (0.5f);
    int toggle4Y = h * (.7f) * (.75f);
    int toggle5Y = h * .7f;

    

    toggle0.setBounds(toggleX, toggleOffset + toggle1Y, buttonSize, buttonSize);
    toggle1.setBounds(toggleX, toggleOffset + toggle2Y, buttonSize, buttonSize);
    toggle2.setBounds(toggleX, toggleOffset + toggle3Y, buttonSize, buttonSize);
    toggle3.setBounds(toggleX, toggleOffset + toggle4Y, buttonSize, buttonSize);
    toggle4.setBounds(toggleX, toggleOffset + toggle5Y, buttonSize, buttonSize);

    knob00.setBounds(col1X, offset + row1Y, dialSize, dialSize);
    knob01.setBounds(col2X, offset + row1Y, dialSize, dialSize);
    knob10.setBounds(col1X, offset + row2Y, dialSize, dialSize);
    knob11.setBounds(col2X, offset + row2Y, dialSize, dialSize);
    knob20.setBounds(col1X, offset + row3Y, dialSize, dialSize);
    knob21.setBounds(col2X, offset + row3Y, dialSize, dialSize);
    knob30.setBounds(col1X, offset + row4Y, dialSize, dialSize);
    knob31.setBounds(col2X, offset + row4Y, dialSize, dialSize);
    knob40.setBounds(col1X, offset + row5Y, dialSize, dialSize);
    knob41.setBounds(col2X, offset + row5Y, dialSize, dialSize);
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
    objP.reset(new SliderAttachment(vts, name, obj));
    obj.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    obj.setPopupDisplayEnabled(false, false, this);
    obj.setLookAndFeel(customDial);
}



void DR1AudioProcessorEditor::initializeButton(ToggleButton& obj, std::unique_ptr<ButtonAttachment>& objP, std::string name)
{
    addAndMakeVisible(obj);
    objP.reset(new ButtonAttachment(vts, name, obj));
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
    knob30.setVelocityModeParameters(1.0, 0, knobSensitivity, false);
    knob31.setVelocityModeParameters(1.0, 0, knobSensitivity, false);
    knob40.setVelocityModeParameters(1.0, 0, knobSensitivity, false);
    knob41.setVelocityModeParameters(1.0, 0, knobSensitivity, false);
}