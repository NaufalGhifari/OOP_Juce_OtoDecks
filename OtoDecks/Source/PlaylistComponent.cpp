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

    playlistCompFormatManager.registerBasicFormats();

    //#################
    addAndMakeVisible(searchButton);
    searchButton.addListener(this);
    searchButton.setName("search");

    addAndMakeVisible(reloadButton);
    reloadButton.addListener(this);
    reloadButton.setName("reload");

    addAndMakeVisible(searchLabel);
    searchLabel.setText("Search tracks: ", juce::dontSendNotification);
    //searchLabel.attachToComponent(&inputText, true);
    searchLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    searchLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(inputText);
    inputText.setEditable(true);
    inputText.setColour(juce::Label::backgroundColourId, juce::Colours::darkgrey);
    inputText.onTextChange = [this]
    { 
        // <!> update displayedFileVector here
        /*
            WIP ACTUAL
            matching algo works, but repainting the cell is an issue because of the vector index
        */

        String currentText = inputText.getText();
        DBG("Text changed: " << currentText);

        displayedFileVector.clear();

        for(int i=0; i < fileVector.size(); ++i)
        {
            DBG("Comparing: " << currentText << " to " << fileVector[i].getFileName());

            // if a substring is found, add file to displayedFileVector
            // source: https://java2blog.com/check-if-string-contains-substring-cpp/
            if (strstr(fileVector[i].getFileName().toStdString().c_str(), currentText.toStdString().c_str()))
            {
                DBG("Match found! " << currentText << " and " << fileVector[i].getFileName());
                displayedFileVector.push_back(fileVector[i]);
            }
        }
        tableComponent.resized();
    };
    //#################
}

PlaylistComponent::~PlaylistComponent()
{
    tableComponent.setModel(nullptr);

    // save playlist to a file
     
    // <!> uncomment to save the file upon closing
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

    double height = getHeight()/10;
    double width = getWidth()/10;

    searchLabel.setBounds(0, 0, width * 2, height);
    inputText.setBounds(width*2, 0, getWidth()-width*4, height);
    
    searchButton.setBounds(width*8, 0, width, height);
    reloadButton.setBounds(width * 9, 0, width, height);

    tableComponent.setBounds(0, height, getWidth(), getHeight());

    // position load new file button
    addNewFileButton.setBounds(0, height *9, getWidth(), height);
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
    //rowNumber = displayedFileVector.size();

    /* fix for crash on fullscreen. Source: https://world-class.github.io/REPL/kinks/level-5/cm-2005-object-oriented-programming/ */
    if (rowNumber < getNumRows())
    {   
        
        if (displayedFileVector.size() > 0)
        {
            if (columnId == 1)
            {
                g.drawText(displayedFileVector[rowNumber].getFileName(),
                    2, 0,
                    width - 4, height,
                    Justification::centredLeft,
                    true);
            }
            else if (columnId == 2)
            {
                double TL_double = getTracklength(displayedFileVector[rowNumber]);                
                std::string TL_string = std::to_string(TL_double) + " s";
                
                g.drawText(TL_string,
                    2, 0,
                    width - 4, height,
                    Justification::centredLeft,
                    true);
            }
        }
        else 
        {
            g.drawText("no tracks found", 2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
            
            
        }
    }
    else 
    {
        g.drawText("no tracks found", 2, 0,
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
    if (columnId == 3)
    {   
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{"Load to P1"};

            // Aside from IDs, we differenciate different 
            // buttons by assigning names using setName()

            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->setName("loadLeft");

            btn->addListener(this);
            existingComponentToUpdate = btn;

        }
    }
    else if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{"Load to P2"};

            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->setName("loadRight");

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    else if (columnId == 5)
    {
        TextButton* btn = new TextButton{ "Remove" };
        
        String id{ std::to_string(rowNumber) };
        btn->setComponentID(id);
        btn->setName("removeButton");

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
                    
                    // <!> check if file is ok

                    // add the selected file(s) to fileVector
                    fileVector.push_back(chosenFile);
                    displayedFileVector.push_back(chosenFile);

                    // get track length
                    double trackLength = getTracklength(chosenFile);
                    trackLenVector.push_back(trackLength);

                    for (int i = 0; i < fileVector.size(); ++i)
                    {
                        DBG(i + 1 << ". File name: " << fileVector[i].getFileName());
                        DBG(i + 1 << ". Track length: " << getTracklength(fileVector[i]));

                    }

                    // update the table by calling resized()
                    tableComponent.resized();
                });
        }
        else if (button == &searchButton)
        {
            DBG("PlaylistComponent::buttonClicked : search button clicked");

            displayedFileVector.clear();
            resized();

        }
        else if (button == &reloadButton)
        {
            DBG("PlaylistComponent::buttonClicked : reload button clicked");

            displayedFileVector.clear();
            fileVector.clear();

            loadPlaylistFromFile("playlist.csv");

            resized();
        }
    }
    else // button has an id
    { 
        String id_string = button->getComponentID().toStdString();
        String btn_name = button->getName().toStdString();
        int id_int = std::stoi(button->getComponentID().toStdString());
        
        if (btn_name == "removeButton")
        {
            DBG(btn_name << " clicked");

            // <!> remove track

            return;
        }       
        else if (btn_name == "loadLeft") // load to P1 
        {
            // <!> Error: ListBox::checkModelPtrIsValid() pointer hit despite destruction
            DBG("Load to P1: " << fileVector[id_int].getFileName());
            
            // load track and draw waveform
            // similar to loading a track straight to the deck
            deckOne->getPlayer()->loadURL(URL{fileVector[id_int]});
            deckOne->getWaveformDisplay()->loadURL(URL{ fileVector[id_int] });
        }
        else if (btn_name == "loadRight") // load to P2
        {
            DBG("Load to P2: " << fileVector[id_int].getFileName());

            deckTwo->getPlayer()->loadURL(URL{ fileVector[id_int] });
            deckTwo->getWaveformDisplay()->loadURL(URL{ fileVector[id_int] });
        }
        else
        {
            DBG("PlaylistComponent::buttonClicked : Unknown button clicked.");
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
    
    std::vector<std::string> lineToken;
    std::string line;

    // get the header line
    std::string headerLine;
    getline(saveFile, headerLine);
    DBG("Header line: " << headerLine);

    // tokenise the file line by line
    while (getline(saveFile, line))
    {
        // tokenise this line
        lineToken = tokenise(line, ',');

        DBG(lineToken[0] << lineToken[1]);
        
        // using the absolute path, find the correct file
        File currentFile(lineToken[1]);

        // add the file to the playlist
        fileVector.push_back(currentFile);
        displayedFileVector.push_back(currentFile);
    };

    // close the file
    saveFile.close();
};

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

double PlaylistComponent::getTracklength(File audioFile)
{
    double trackLen = 0.0;
    

    if (auto reader = playlistCompFormatManager.createReaderFor(audioFile))
    {
        auto lengthInSeconds = reader->lengthInSamples / reader->sampleRate;
        // lengthInSeconds now contains the length of the audio file in seconds...

        trackLen = lengthInSeconds;
    }

    return trackLen;
};
