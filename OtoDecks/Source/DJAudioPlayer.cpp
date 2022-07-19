/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 14 Jul 2022 12:18:25pm
    Author:  naufa

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer()
{

};

DJAudioPlayer::~DJAudioPlayer()
{

};



/* prepareToPlay, getNextAudioBlock, releaseResources - audio life cycle */
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) 
{
    formatManager.registerBasicFormats();
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
};
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) 
{
    //transportSource.getNextAudioBlock(bufferToFill);
    resampleSource.getNextAudioBlock(bufferToFill);
};
void DJAudioPlayer::releaseResources() 
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
};



void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr)
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());

    }
};
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1)
    {
        DBG("DJAudioPlayer::setGain -> gain is out of range, must be between 0 and 1 instead. gain: " << gain);
    }
    else
    {
        transportSource.setGain(gain);
    }
    
};
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 100)
    {
        DBG("DJAudioPlayer::setSpeed -> ratio is out of range, must be between 0 and 100 instead. ratio: " << ratio);
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
};
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
};

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        DBG("DJAudioPlayer::setPositionRelative -> pos is out of range, must be between 0 and 1 instead. pos: " << pos);
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
};

void DJAudioPlayer::start()
{
    transportSource.start();
};
void DJAudioPlayer::stop()
{
    transportSource.stop();
};