/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 26 Jul 2022 8:03:57pm
    Author:  naufa

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
                                 AudioThumbnailCache& cacheToUse) : audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::orange);
    
    if (fileLoaded)
    {
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
        g.setColour(Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    }
    else
    {
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));

    if (fileLoaded)
    {
        DBG("wfd: loaded!");
    }
    else
    {
        DBG("wfd: not loaded!");
    }


};

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    DBG("wfd: Change received!");

    repaint();
};

void WaveformDisplay::setPositionRelative(double pos)
{
    // Only repaint if the position has changed
    if (pos != position && pos > 0)
    {
        position = pos;
        repaint();
    }
};

