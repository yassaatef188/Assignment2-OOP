#pragma once // PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"
class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener
{
public:
	PlayerGUI();
	~PlayerGUI() override;
	void resized() override;
	void paint(juce::Graphics& g) override;
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();
private:
	PlayerAudio playerAudio;
	// GUI elements
	
	juce::TextButton loadButton{ "Load File" };
	juce::TextButton restartButton{ "Restart" };
	juce::TextButton stopButton{ "Stop" };
	juce::TextButton muteButton{ "Mute" };
	juce::TextButton loopButton{ "Loop" };
	juce::TextButton pauseButton{ "Pause" };
	juce::TextButton goStartButton{ "Go To Start" };
	juce::TextButton goEndButton{ "Go To End" };
	juce::TextButton backwardButton{ "-10" };
	juce::TextButton forwardButton{ "+10" };
	juce::Slider volumeSlider;
	std::unique_ptr<juce::FileChooser> fileChooser;
	bool isMuted = false;
	bool isLooping = false;
	bool isPlaying = true;
	float oldVolume = 0.5f;
	// Event handlers
	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)

};

