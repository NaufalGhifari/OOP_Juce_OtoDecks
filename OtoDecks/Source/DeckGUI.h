/*
  ==============================================================================

    DeckGUI.h
    Created: 19 Jul 2022 6:46:52am
    Author:  naufa

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public Button::Listener,
                 public Slider::Listener,
                 public FileDragAndDropTarget,
                 public Timer

{
public:
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager& formatManagerToUse, 
            AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** Implement Button::Listener */
    void buttonClicked(Button*) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

    bool isInterestedInFileDrag(const StringArray &files) override;
    void filesDropped(const StringArray &files, int x, int y) override;

    void timerCallback() override;

    /**/
    DJAudioPlayer* getPlayer();

    WaveformDisplay* getWaveformDisplay();
    /**/
    

private:

    juce::FileChooser fChooser{ "Select a file..." };

    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "PAUSE" };
    TextButton loadButton{ "LOAD" };
    

    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    Label volLabel;
    Label speedLabel;
    Label posLabel;
    Label playbackLabel;

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
