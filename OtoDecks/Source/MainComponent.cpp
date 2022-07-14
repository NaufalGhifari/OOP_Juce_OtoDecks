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
    addAndMakeVisible(loadButton);

    // add a listener (step 2)
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
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

    // read an audio file
    formatManager.registerBasicFormats();
    
    

    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    //transportSource.getNextAudioBlock(bufferToFill);
    resampleSource.getNextAudioBlock(bufferToFill);
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

    transportSource.releaseResources();
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
    // (step 3)
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
   
    volSlider.setBounds(0, rowH*2, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);

    loadButton.setBounds(0, rowH*4, getWidth(), rowH);
    
}

void MainComponent::buttonClicked(Button* button)
{
    // (step 4 - button)
    // play button clicked
    if (button == &playButton)
    {
        DBG("Play button was clicked");
        transportSource.start();
    }
    // stop button clicked
    else if (button == &stopButton)
    {
        DBG("Stop button was clicked");
        transportSource.stop();
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
            loadURL(URL{ chosenFile });
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
        
        transportSource.setGain(slider->getValue());
    }
    else if (slider == &speedSlider)
    {
        DBG("speed slider moved: " << slider->getValue());

        resampleSource.setResamplingRatio(slider->getValue());
    }
    else
    {
        DBG("Unrecognized slider moved");
    }
};

void MainComponent::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr)
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        
    }
};
