#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    // add and make visible "playButton"
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(volSlider);

    playButton.addListener(this);
    stopButton.addListener(this);
    volSlider.addListener(this);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    phase = 0.0;
    dphase = 0.0001;

}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // set left and right audio channel
    auto* leftChan = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightChan = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

    // populate buffers with random values
    for (auto i=0; i < bufferToFill.numSamples; ++i)
    {
        //double sample = rand.nextDouble() * 0.25;
        // double sample = fmod(phase, 0.2);

        double sample = sin(phase) * 0.1;

        leftChan[i] = sample;
        rightChan[i] = sample;

        phase += dphase;
    }

    //bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
    
    /*g.setFont(20.0f);
    g.drawText("Hello from Perth!", getLocalBounds(), Justification::centred, true);*/
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    // Call the DBG function to print out a message
    DBG("Hello World!");
    
    double rowH = getHeight() / 5;

    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    volSlider.setBounds(0, rowH*2, getWidth(), rowH);
    
}

void MainComponent::buttonClicked(Button* button)
{
    // play button clicked
    if (button == &playButton)
    {
        DBG("Play button was clicked");
    }
    // stop button clicked
    else if (button == &stopButton)
    {
        DBG("Stop button was clicked");
    }
    else
    {
        DBG("Unrecognized button clicked");
    }
}

void MainComponent::sliderValueChanged(Slider *slider)
{
    if (slider == &volSlider)
    {
        //DBG("vol slider moved: " << slider->getValue());
        dphase = volSlider.getValue() * 0.01;
    }
    else
    {
        DBG("Unrecognized slider moved");
    }
};
