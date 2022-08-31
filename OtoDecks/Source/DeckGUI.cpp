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

    speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);


    // add labels for slider
    //addAndMakeVisible(volLabel);
    //volLabel.setText("Volume", juce::dontSendNotification);
    //volLabel.attachToComponent(&volSlider, true);

    //addAndMakeVisible(speedLabel);
    //speedLabel.setText("Speed", juce::dontSendNotification);
    //speedLabel.attachToComponent(&speedSlider, true);

    addAndMakeVisible(posLabel);
    posLabel.setText("Playback", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, true);

    addAndMakeVisible(waveformDisplay);

    // ### Add listeners to react to clicks ###

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
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

    double rowH = getHeight() / 14;
    auto margin = 50; // margin to make space between left and right edges

    // (step 3)
    waveformDisplay.setBounds(0, 0, getWidth(), rowH*2);
    posSlider.setBounds(margin, rowH * 2, getWidth() - margin, rowH*2);

    playButton.setBounds(0, rowH*4, getWidth(), rowH);
    stopButton.setBounds(0, rowH*5, getWidth(), rowH);
    

    //sliders position
    volSlider.setBounds(margin, rowH * 6.5, getWidth()/2, rowH*5);
    speedSlider.setBounds(getWidth() / 2, rowH * 6.5, getWidth() / 2, rowH * 5);
    

    loadButton.setBounds(0, rowH * 13, getWidth(), rowH);

    /*
    waveformDisplay.setBounds(0, rowH * 8, getWidth(), rowH * 2);

    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    loadButton.setBounds(0, rowH * 10, getWidth(), rowH);

    //sliders position
    volSlider.setBounds(margin, rowH * 2, getWidth() - margin, rowH);
    speedSlider.setBounds(margin, rowH * 3, getWidth() - margin, rowH);
    posSlider.setBounds(margin, rowH * 4, getWidth() - margin, rowH);
    */
    
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