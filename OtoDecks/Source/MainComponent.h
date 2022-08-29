#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

using namespace juce;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    

    
private:
    //==============================================================================
    // Your private member variables go here...
    
    juce::FileChooser fChooser{ "Select a file..." };

    // Topic 9 ######################################################
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100};
    // ##############################################################
    


    // Topic 8 ######################################################

    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{&player1, formatManager, thumbCache};
    
    DJAudioPlayer player2{ formatManager };
    DeckGUI deckGUI2{&player2, formatManager, thumbCache };

    MixerAudioSource mixerSource;
         
    // ##############################################################
    
    //<!> 'playlistComponent': unknown override specifier
    PlaylistComponent playlistComponent{ &deckGUI1, &deckGUI2 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
