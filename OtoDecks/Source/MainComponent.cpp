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

    // add and make visible "playButton" (step 1)
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    
    addAndMakeVisible(loadButton);

    // add a listener (step 2)
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    posSlider.setRange(0.0, 1.0);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    player1.getNextAudioBlock(bufferToFill);
}

/*void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // set left and right audio channel
    auto* leftChan = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightChan = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

    // populate buffers with random values
    for (auto i=0; i < bufferToFill.numSamples; ++i)
    {
        double sample = sin(phase) * 0.1;

        leftChan[i] = sample;
        rightChan[i] = sample;

        phase += dphase;
    }

    //bufferToFill.clearActiveBufferRegion();
}*/

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()

    //transportSource.releaseResources();
    player1.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
    
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    // Call the DBG function to print out a message
    DBG("Hello World!");
    
    double rowH = getHeight() / 6;
    // (step 3)
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
   
    volSlider.setBounds(0, rowH*2, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH);

    loadButton.setBounds(0, rowH*5, getWidth(), rowH);
    
}

void MainComponent::buttonClicked(Button* button)
{
    // (step 4 - button)
    // play button clicked
    if (button == &playButton)
    {
        DBG("Play button was clicked");
        player1.start();
    }
    // stop button clicked
    else if (button == &stopButton)
    {
        DBG("Stop button was clicked");
        player1.stop();
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
            player1.loadURL(URL{ chosenFile });
        });
    }
    else
    {
        DBG("Unrecognized button clicked");
    }
}

void MainComponent::sliderValueChanged(Slider *slider)
{
    //(step 4 - slider)
    if (slider == &volSlider)
    {
        DBG("vol slider moved: " << slider->getValue());
        
        player1.setGain(slider->getValue());
    }
    else if (slider == &speedSlider)
    {
        DBG("speed slider moved: " << slider->getValue());

        player1.setSpeed(slider->getValue());
    }
    else if (slider == &posSlider)
    {
        DBG("position slider moved: " << slider->getValue());

        player1.setPositionRelative(slider->getValue());
    }
    else
    {
        DBG("Unrecognized slider moved");
    }
};


