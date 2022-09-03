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

#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

using namespace juce;

class PlaylistComponent  : public juce::Component,
                            public TableListBoxModel,
                            public Button::Listener,
                            public TextEditor::Listener
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

    double getTracklength(File audioFile);

private:
    TableListBox tableComponent;
    
    std::vector<std::string> trackTitles;
    juce::FileChooser fChooser{ "Select a file to add to playlist..." };

    // Button to load new files to the playlist
    TextButton addNewFileButton{ "Add new file" };

    
    /*
        There are 2 file vectors: fileVector and displayedFilevector.
        filevector: holds ALL files added to the playlist.
        displayedFilevector: holds files that are being displayed to the user
    */

    std::vector <juce::File> fileVector;
    std::vector <juce::File> displayedFileVector;
    std::vector <double> trackLenVector;

    // pointers to different DeckGUIs
    DeckGUI* deckOne;
    DeckGUI* deckTwo;

    juce::AudioFormatManager playlistCompFormatManager;

    //################
    Label searchLabel;
    Label inputText;
    
    //################

    TextEditor searchTextEditor;
    
    //################


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};

    
