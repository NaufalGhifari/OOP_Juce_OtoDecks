/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 14 Jul 2022 12:18:25pm
    Author:  naufa

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DJAudioPlayer : public AudioSource
{
    public:
        DJAudioPlayer(AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        void loadURL(URL audioURL);
        void setGain(double gain);
        void setSpeed(double ratio);
        void setPosition(double posInSecs);

        void setPositionRelative(double pos);

        void start();
        void stop();

        /** get the relative position of playhead */
        double getPositionRelative();

    private:
        AudioFormatManager& formatManager;
        std::unique_ptr<AudioFormatReaderSource> readerSource;
        AudioTransportSource transportSource;
        ResamplingAudioSource resampleSource{&transportSource, false, 2};

};