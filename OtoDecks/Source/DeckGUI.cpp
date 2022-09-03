/*
  ==============================================================================

    DeckGUI.cpp
    Created: 19 Jul 2022 6:46:52am
    Author:  naufa

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse) : player(_player), waveformDisplay(formatManagerToUse, cacheToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // ### Add and make visible to display the items ###

    // buttons
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    // sliders
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    // change the design
    // source: https://www.youtube.com/watch?v=po46y8UKPOY
    volSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    volSlider.setColour(juce::Slider::rotarySliderOutlineColourId, Colours::darkred);
    volSlider.setColour(juce::Slider::rotarySliderFillColourId, Colours::orangered);
    volSlider.setColour(juce::Slider::thumbColourId, juce::Colours::antiquewhite);

    speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    speedSlider.setColour(juce::Slider::rotarySliderOutlineColourId, Colours::darkred);
    speedSlider.setColour(juce::Slider::rotarySliderFillColourId, Colours::orangered);
    speedSlider.setColour(juce::Slider::thumbColourId, juce::Colours::antiquewhite);
    
    posSlider.setColour(juce::Slider::thumbColourId, juce::Colours::antiquewhite);
    
    //addAndMakeVisible(posLabel);
    //posLabel.setText("Playback", juce::dontSendNotification);
    //posLabel.attachToComponent(&posSlider, true);

    addAndMakeVisible(waveformDisplay);

    addAndMakeVisible(volLabel);
    volLabel.setText("Volume", juce::dontSendNotification);

    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);

    addAndMakeVisible(playbackLabel);
    playbackLabel.setText("Playback Control", juce::dontSendNotification);

    // ### Add listeners to react to clicks ###

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    // set colours for textbuttons
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    
    
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.1, 10.0);
    posSlider.setRange(0.0, 1.0);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /*g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));*/   // clear the background
    g.fillAll(juce::Colours::black);

    g.setColour (juce::Colours::darkgrey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

}   

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double rowH = getHeight() / 20;
    double width = getWidth() / 20;
    auto margin = 50; // margin to make space between left and right edges

    // (step 3)
    waveformDisplay.setBounds(0, 0, getWidth(), rowH*4);
    
    playbackLabel.setBounds(0, rowH*0.9 * 4, getWidth() - margin, rowH * 2);
    posSlider.setBounds(0, rowH*1.25 * 4, getWidth(), rowH * 2);

    playButton.setBounds(getWidth()/10*1, rowH * 7, getWidth() / 3, rowH * 2);
    stopButton.setBounds(getWidth()/10*6, rowH * 7, getWidth() / 3, rowH * 2);
    
    //sliders position
    volLabel.setBounds(width*5.8, rowH * 10, width*3, rowH);
    speedLabel.setBounds(width*14, rowH *10, getWidth() / 2, rowH);

    volSlider.setBounds(margin, rowH * 11, getWidth()/2, rowH*5);
    speedSlider.setBounds(getWidth() / 2, rowH * 11, getWidth() / 2, rowH * 5);
    

    loadButton.setBounds(getWidth()/3, rowH * 17.5, getWidth() / 3, rowH * 1.5);    
}

void DeckGUI::buttonClicked(Button* button)
{
    // play button clicked
    if (button == &playButton)
    {
        DBG("Play button was clicked");
        player->start();
    }
    // stop button clicked
    else if (button == &stopButton)
    {
        DBG("Stop button was clicked");
        player->stop();
    }
    // load button clicked
    else if (button == &loadButton)
    {
        DBG("load button was clicked");

        // this does work in 6.1 but the syntax is a little funky
        // https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
        // - configure the dialogue
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        // - launch out of the main thread
        // - note how we use a lambda function which you've probably
        // not seen before. Please do not worry too much about that.
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            File chosenFile = chooser.getResult();
            player->loadURL(URL{ chosenFile });
            waveformDisplay.loadURL(URL{ chosenFile });

        });
    }

    else
    {
        DBG("Unrecognized button clicked");
    }
}

void DeckGUI::sliderValueChanged(Slider* slider)
{   
    if (slider == &volSlider)
    {
        DBG("vol slider moved: " << slider->getValue());

        player->setGain(slider->getValue());
    }
    else if (slider == &speedSlider)
    {
        DBG("speed slider moved: " << slider->getValue());

        player->setSpeed(slider->getValue());
    }
    else if (slider == &posSlider)
    {
        DBG("position slider moved: " << slider->getValue());

        player->setPositionRelative(slider->getValue());
    }
    else
    {
        DBG("Unrecognized slider moved");
    }
    
};

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;

};

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    DBG("DeckGUI::filesDropped");

    if (files.size() == 1)
    {
        player->loadURL(URL{ File{files[0]} });
    }
};

void DeckGUI::timerCallback()
{
    // print to check callback if it is working
    //DBG("DeckGUI::timerCallback()");
    waveformDisplay.setPositionRelative(player->getPositionRelative());
};

// pass private attributes to another class
DJAudioPlayer* DeckGUI::getPlayer()
{
    return player;
};

WaveformDisplay* DeckGUI::getWaveformDisplay()
{
    return &waveformDisplay;
};