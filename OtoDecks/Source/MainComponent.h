#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::Button::Listener,
                       public juce::Slider::Listener
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

    /** Implement Button::Listener */
    void buttonClicked(Button*) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider *slider) override;

private:
    //==============================================================================
    // Your private member variables go here...
    
    TextButton playButton{"PLAY"};
    TextButton stopButton{ "STOP" };
    Slider volSlider;

    /** a random value variable */
    Random rand;

    double phase;
    double dphase;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
