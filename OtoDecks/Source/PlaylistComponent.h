/*
  ==============================================================================

    PlaylistComponent.h
    Created: 24 Aug 2022 7:53:44am
    Author:  naufa

  ==============================================================================
*/

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <JuceHeader.h>

// <!> still in testing
#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"



using namespace juce;


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                            public TableListBoxModel,
                            public Button::Listener
{
public:
    PlaylistComponent(DeckGUI* _deck_1, DeckGUI* _deck_2);
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

    /* 
        - saveToFile(<vector containing files>, <name of file to save into>)
        - Saves the content of a vector into a file (overwrites)
    */
    void saveToFile(std::vector<juce::File>, std::string fileName);

    /*
        - loadPlaylistFromFile(<name of file to load from>)
        - loads a playlist from a save file
    */ 
    void loadPlaylistFromFile(std::string fileName);

    /* Takes a line and return a vector of tokens (strings) */
    static std::vector<std::string> tokenise(std::string csvLine, char separator);

private:
    TableListBox tableComponent;
    
    std::vector<std::string> trackTitles;
    juce::FileChooser fChooser{ "Select a file to add to playlist..." };

    // Button to load new files to the playlist
    TextButton addNewFileButton{ "Add new file" };

    // A vector of juce::File
    std::vector <juce::File> fileVector;

    // pointers to different DeckGUIs
    DeckGUI* deckOne;
    DeckGUI* deckTwo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};

    /*
        Idea for persistent library:
        1. Have a vector of files std::vector<Juce::File> [DONE]

        2. Make a function such as addFile() to fetch and serve files [DONE]

        3. Make a function that takes said vector and writes to a file (XML or CSV) [DONE]

        4. Make a function that loads from this XML/CSV file into the playlistComponent [DONE]
    */
