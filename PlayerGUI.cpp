#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton, &backwardButton, &fowardButton, &muteButton, &loopButton, &pauseButton, &goStartButton, &goEndButton})
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    // Volume slider
    volumeSlider.setRange(0.0, 1, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
}

void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(130, y, 80, 40);
    stopButton.setBounds(220, y, 80, 40);
    backwardButton.setBounds(310, y, 80, 40);
    forwardButton.setBounds(400, y, 80, 40);
    /*prevButton.setBounds(340, y, 80, 40);
    nextButton.setBounds(440, y, 80, 40);*/
    muteButton.setBounds(490, y, 80, 40);
    loopButton.setBounds(580, y, 80, 40);
    pauseButton.setBounds(670, y, 80, 40);
    goStartButton.setBounds(760, y, 80, 40);
    goEndButton.setBounds(850, y, 80, 40);
  
    volumeSlider.setBounds(20, 100, getWidth() - 100, 40);
}

PlayerGUI::~PlayerGUI()
{
}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);    
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                }
            });
    }

    if (button == &restartButton)
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }

    if (button == &stopButton)
    {
        transportSource.stop();
        transportSource.setPosition(0.0);
        pauseButton.setButtonText("Start");
        isPlaying = false;
    }

        if (button == &backwardButton)
    {
        if (playerAudio.getPosition() - 10 < 0)
        {
            playerAudio.setPosition(0);
        }
        else
            playerAudio.setPosition(playerAudio.getPosition() - 10);
    }

        if (button == &forwardButton)
    {
        if (playerAudio.getPosition() + 10 > playerAudio.getLength())
        {
            playerAudio.setPosition(playerAudio.getLength());
        }
        else
        {
            playerAudio.setPosition(playerAudio.getPosition() + 10);
        }
    }
    if (button == &muteButton)
    {
        isMuted = !isMuted;
        if (isMuted)
        {
            oldVolume = transportSource.getGain();
            transportSource.setGain(0.0);
            muteButton.setButtonText("Unmute");
        }
        else
        {
            transportSource.setGain(oldVolume);
            muteButton.setButtonText("Mute");
        }
    }
    if (button == &loopButton)
    {
        isLooping = !isLooping;
        if (readerSource)
            readerSource->setLooping(isLooping);
        loopButton.setButtonText(isLooping ? "Unloop" : "Loop");
    }
    if (button == &pauseButton)
    {
        if (isPlaying)
        {
            transportSource.stop();
            pauseButton.setButtonText("Start");
        }
        else
        {
            transportSource.start();
            pauseButton.setButtonText("Pause");
        }
        isPlaying = !isPlaying;
    }
    if (button == &goEndButton)
    {
        if (readerSource && readerSource->getAudioFormatReader())
        {
            auto* reader = readerSource->getAudioFormatReader();
            double audioLengthInSeconds = static_cast<double>(reader->lengthInSamples) / reader->sampleRate;

            double endPosition = audioLengthInSeconds - 0.1;
            transportSource.setPosition(endPosition);

            if (!isPlaying)
                transportSource.start();
            isPlaying = !isPlaying;
        }
    }

}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());

}
