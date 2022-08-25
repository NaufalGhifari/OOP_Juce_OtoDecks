/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 24 Aug 2022 7:53:44am
    Author:  naufa

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // add things to trackTitles
    trackTitles.push_back("track1");
    trackTitles.push_back("track2");
    trackTitles.push_back("track3");
    trackTitles.push_back("track4");
    trackTitles.push_back("track5");
    
    // playlistComponent's columns
    tableComponent.getHeader().addColumn("Title", 1, 500);
    tableComponent.getHeader().addColumn("Duration", 2, 200);
    tableComponent.getHeader().addColumn("Load track", 3, 200);


    tableComponent.setModel(this);

    // make playlistComponent visible
    addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
    tableComponent.setModel(nullptr);
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
};

// 4 functions inherited from TableListBoxModel
void PlaylistComponent::paintRowBackground(Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{   
    // if the row is selected paint it orange, else paint it grey
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else 
    {
        g.fillAll(Colours::darkgrey);
    }
};

void PlaylistComponent::paintCell(Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{   

    /* fix for crash on fullscreen. Source: https://world-class.github.io/REPL/kinks/level-5/cm-2005-object-oriented-programming/ */
    if (rowNumber < getNumRows())
    {
        g.drawText(trackTitles[rowNumber],
            2, 0,
            width - 4, height,
            Justification::centredLeft,
            true);
    }
};

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{   
    if (columnId == 2)
    {   
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{"Play"};

            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;

        }
    }

    return existingComponentToUpdate;
};

// inherited from Button:Listener
void PlaylistComponent::buttonClicked(Button* button)
{
    int id = std::stoi(button->getComponentID().toStdString());
    DBG("PlaylistComponent::buttonClicked : " +  trackTitles[id]);
};
