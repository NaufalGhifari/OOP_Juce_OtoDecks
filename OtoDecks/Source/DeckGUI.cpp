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

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(loadButton);

    addAndMakeVisible(waveformDisplay);


    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);

}

DeckGUI::~DeckGUI()
{
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double rowH = getHeight() / 8;
    // (step 3)
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);

    volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH);

    waveformDisplay.setBounds(0, rowH*5, getWidth(), rowH*2);

    loadButton.setBounds(0, rowH * 7, getWidth(), rowH);

    

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
