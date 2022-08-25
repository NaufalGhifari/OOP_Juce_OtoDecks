/*
  ==============================================================================

    PlaylistComponent.h
    Created: 24 Aug 2022 7:53:44am
    Author:  naufa

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

using namespace juce;


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                            public TableListBoxModel,
                            public Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // 4 functions inherited from TableListBoxModel
    int getNumRows() override;
    
    void paintRowBackground(Graphics & g, 
                            int rowNumber, 
                            int width, 
                            int height, 
                            bool rowIsSelected) override;
    
    void paintCell(Graphics & g,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber, 
                                        int columnId, 
                                        bool isRowSelected, 
                                        Component* existingComponentToUpdate) override;
    
    void buttonClicked(Button* button) override;

private:
    TableListBox tableComponent;
    std::vector<std::string> trackTitles;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
