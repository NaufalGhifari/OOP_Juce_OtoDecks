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

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public Button::Listener,
                 public Slider::Listener,
                 public FileDragAndDropTarget

{
public:
    DeckGUI(DJAudioPlayer* player);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** Implement Button::Listener */
    void buttonClicked(Button*) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

    bool isInterestedInFileDrag(const StringArray &files) override;
    void filesDropped(const StringArray &files, int x, int y) override;

private:

    juce::FileChooser fChooser{ "Select a file..." };

    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };

    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
