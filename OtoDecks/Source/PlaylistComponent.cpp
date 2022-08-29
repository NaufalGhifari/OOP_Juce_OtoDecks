/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 24 Aug 2022 7:53:44am
    Author:  naufa

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "DJAudioPlayer.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deck_1, 
                                    DeckGUI* _deck_2) : deckOne(_deck_1), deckTwo(_deck_2)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    //deckOne = _deck_1;
    //deckTwo = _deck_2;

    // add things to trackTitles
    /*
    trackTitles.push_back("track1");
    trackTitles.push_back("track2");
    trackTitles.push_back("track3");
    trackTitles.push_back("track4");
    trackTitles.push_back("track5");
    */

    // load playlist from a file
    loadPlaylistFromFile("playlist.csv");

    // playlistComponent's columns
    tableComponent.getHeader().addColumn("Title", 1, 300);
    tableComponent.getHeader().addColumn("Duration", 2, 100);
    tableComponent.getHeader().addColumn("Player 1", 3, 100);
    tableComponent.getHeader().addColumn("Player 2", 4, 100);
    tableComponent.getHeader().addColumn("Remove", 5, 100);


    tableComponent.setModel(this);

    // make playlistComponent visible
    addAndMakeVisible(tableComponent);

    // display and add listener for addNewFileButton
    addAndMakeVisible(addNewFileButton);
    addNewFileButton.addListener(this);
}

PlaylistComponent::~PlaylistComponent()
{
    tableComponent.setModel(nullptr);

    // save playlist to a file
    saveToFile(fileVector, "playlist.csv");
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

    double rowH = getHeight()/10;

    
    tableComponent.setBounds(0, 0, getWidth(), getHeight());

    // position load new file button
    addNewFileButton.setBounds(0, rowH*9, getWidth(), rowH);
}

int PlaylistComponent::getNumRows()
{
    //return trackTitles.size();
    return fileVector.size();
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
        
        if (fileVector.size() > 0)
        {
            g.drawText(fileVector[rowNumber].getFileName(),
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
        }
        


        /*
        g.drawText(trackTitles[rowNumber],
            2, 0,
            width - 4, height,
            Justification::centredLeft,
            true);
        */
    }
};

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{   
    if (columnId == 3)
    {   
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{"Load to P1"};

            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;

        }
    }
    else if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{"Load to P2"};

            /*
                Workaround:
                to differenciate between P1 and P2 load button: add 100 to the rowNumbers number. 
                Meaning any id above 100 belongs to P2.

                Limitations:
                P1 can only have 100 tracks while P2 can have an indefinite amount.
                To increase the limit for P1, simply increase the treshold.
                for the purpose of this project, 100 is sufficient.
            */

            const int treshold = 100;

            String id{ std::to_string(rowNumber + treshold) };
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    else if (columnId == 5)
    {
        TextButton* btn = new TextButton{ "Remove" };
        
        String id{"deleteButton"};
        btn->setComponentID(id);

        btn->addListener(this);
        existingComponentToUpdate = btn;
    }

    return existingComponentToUpdate;
};

// inherited from Button:Listener
void PlaylistComponent::buttonClicked(Button* button)
{
    
    if (button->getComponentID().toStdString() == "") // button does not have an id
    {
        if (button == &addNewFileButton)
        {
            DBG("load new file button was clicked");
            
            // choose a file
            auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
            fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
                {
                    File chosenFile = chooser.getResult();

                    // add the selected file(s) to fileVector
                    fileVector.push_back(chosenFile);

                    for (int i = 0; i < fileVector.size(); ++i)
                    {
                        DBG(i + 1 << ". File name: " << fileVector[i].getFileName());
                    }

                });
        }
    }
    else // button has an id
    { 
        String id_string = button->getComponentID().toStdString();
        
        if (id_string == "deleteButton")
        {
            DBG(id_string << " clicked");

            /*
                <!> 
                issue:
                delete button has no id, not an int at least. so cannot find the relevant track to delete.

                idea: 
                instead of using deleteButton as string, use button.getText() or something
            */

            return;
        }

        int id_int = std::stoi(button->getComponentID().toStdString());

        if (id_int < 100) // load to P1 
        {
            // <!> Error: ListBox::checkModelPtrIsValid() pointer hit despite destruction
            DBG("Load to P1: " << fileVector[id_int].getFileName());
            
            // load track and draw waveform
            // similar to loading a track straight to the deck
            deckOne->getPlayer()->loadURL(URL{fileVector[id_int]});
            deckOne->getWaveformDisplay()->loadURL(URL{ fileVector[id_int] });
        }
        else // load to P2
        {
            DBG("Load to P2: " << fileVector[id_int - 100].getFileName());

            deckTwo->getPlayer()->loadURL(URL{ fileVector[id_int - 100] });
            deckTwo->getWaveformDisplay()->loadURL(URL{ fileVector[id_int - 100] });
        }
    }
};

void PlaylistComponent::saveToFile(std::vector<juce::File> vector, std::string fileName)
{
    // create a file stream & open the save file
    std::ofstream saveFile;
    saveFile.open(fileName);

    // write header
    saveFile << "title,absolute_path" << std::endl;

    // write content
    for (int i = 0; i < vector.size(); ++i)
    {
        saveFile << vector[i].getFileNameWithoutExtension() << "," << vector[i].getFullPathName() << std::endl;
    }
    
    // close the file
    saveFile.close();
};

void PlaylistComponent::loadPlaylistFromFile(std::string fileName)
{
    
    // create a file stream & open the save file
    std::ifstream saveFile;
    saveFile.open(fileName);

    //<!> load playlist
    std::vector<std::string> lineToken;
    std::string line;
    while (getline(saveFile, line))
    {
        lineToken = tokenise(line, ',');

        // WIP: we can fetch files and titles in here. Load the files to the playlist
        File currentFile(lineToken[1]);

        // add the file to the playlist
        fileVector.push_back(currentFile);


    };

    // close the file
    saveFile.close();
    
};

/*##################################################################################################*/

std::vector<std::string> PlaylistComponent::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    start = csvLine.find_first_not_of(separator, 0);
    do {
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end) break;
        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);

    return tokens;
}
/*##################################################################################################*/




/*
Issue: playlistComponent cannot access DeckGUI's "player" attribute
Idea:

*/

/*
    saveToFile() pseudocode
    {
        
        open file;

        write header line;



    };
*/