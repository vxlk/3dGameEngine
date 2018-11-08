/*
  ==============================================================================

    MainOverlay.cpp
    Created: 24 Oct 2018 4:20:28pm
    Author:  Tyler

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainOverlay.h"
#include "MainComponent.h"
#include "Shader.h"
//#include "Texture.h"
//==============================================================================

/*
MainOverlay::MainOverlay()
{
	
}
*/
MainOverlay::MainOverlay(MainComponent& m) : demo(m)
{
	addAndMakeVisible(statusLabel);
	statusLabel.setJustificationType(Justification::topLeft);
	statusLabel.setFont(Font(14.0f));

	addAndMakeVisible(sizeSlider);
	sizeSlider.setRange(0.0, 1.0, 0.001);
	sizeSlider.addListener(this);

	addAndMakeVisible(zoomLabel);
	zoomLabel.attachToComponent(&sizeSlider, true);

	addAndMakeVisible(speedSlider);
	speedSlider.setRange(0.0, 0.5, 0.001);
	speedSlider.addListener(this);
	speedSlider.setSkewFactor(0.5f);

	addAndMakeVisible(speedLabel);
	speedLabel.attachToComponent(&speedSlider, true);

	addAndMakeVisible(moveButton);
	moveButton.onClick = [this] { demo.doBackgroundDrawing = moveButton.getToggleState(); };

	addAndMakeVisible(tabbedComp);
	tabbedComp.setTabBarDepth(25);
	tabbedComp.setColour(TabbedButtonBar::tabTextColourId, Colours::grey);
	tabbedComp.addTab("Input", Colours::transparentBlack, &vertexEditorComp, false);
	tabbedComp.addTab("Log", Colours::transparentBlack, &fragmentEditorComp, false);

	inputDocument.addListener(this);
	logDocument.addListener(this);

	textures.add(new TextureFromUI("portmeirion.jpg"));
	textures.add(new TextureFromUI("tile_background.png"));
	textures.add(new TextureFromUI("juce_icon.png"));
	textures.add(new DynamicTexture());

	addAndMakeVisible(textureBox);
	textureBox.onChange = [this] { selectTexture(textureBox.getSelectedId()); };
	updateTexturesList();

	addAndMakeVisible(presetBox);
	presetBox.onChange = [this] { selectPreset(presetBox.getSelectedItemIndex()); };

	auto presets = getPresets();

	for (int i = 0; i < presets.size(); ++i)
		presetBox.addItem(presets[i].name, i + 1);

	addAndMakeVisible(presetLabel);
	presetLabel.attachToComponent(&presetBox, true);

	addAndMakeVisible(textureLabel);
	textureLabel.attachToComponent(&textureBox, true);

	lookAndFeelChanged();
}


MainOverlay::~MainOverlay()
{
}

void MainOverlay::updateShader()
{
	startTimer(10);
}

void MainOverlay::selectPreset(int preset)
{
	const auto& p = getPresets()[preset];

	inputDocument.replaceAllContent(p.vertexShader);
	logDocument.replaceAllContent(p.fragmentShader);

	startTimer(1);
}

void MainOverlay::updateTexturesList()
{
	textureBox.clear();

	for (int i = 0; i < textures.size(); ++i)
		textureBox.addItem(textures.getUnchecked(i)->name, i + 1);

#if JUCE_MODAL_LOOPS_PERMITTED
	textureBox.addSeparator();
	textureBox.addItem("Load from a file...", 1000);
#endif
}

void MainOverlay::selectTexture(int itemID)
{
#if JUCE_MODAL_LOOPS_PERMITTED
	if (itemID == 1000)
	{
		auto lastLocation = File::getSpecialLocation(File::userPicturesDirectory);

		FileChooser fc("Choose an image to open...", lastLocation, "*.jpg;*.jpeg;*.png;*.gif");

		if (fc.browseForFileToOpen())
		{
			lastLocation = fc.getResult();

			textures.add(new TextureFromFile(fc.getResult()));
			updateTexturesList();

			textureBox.setSelectedId(textures.size());
		}
	}
	else
#endif
	{
		if (auto* t = textures[itemID - 1])
			demo.setTexture(t);
	}
}

void MainOverlay::timerCallback()
{
	stopTimer();
	demo.setShaderProgram(inputDocument.getAllContent(),
		logDocument.getAllContent());
}

void MainOverlay::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
}

void MainOverlay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
	auto area = getLocalBounds().reduced(4);

	auto top = area.removeFromTop(75);

	auto sliders = top.removeFromRight(area.getWidth() / 2);
	moveButton.setBounds(sliders.removeFromBottom(25));
	speedSlider.setBounds(sliders.removeFromBottom(25));
	sizeSlider.setBounds(sliders.removeFromBottom(25));

	top.removeFromRight(70);
	statusLabel.setBounds(top);

	auto shaderArea = area.removeFromBottom(area.getHeight() / 2);

	auto presets = shaderArea.removeFromTop(25);
	presets.removeFromLeft(100);
	presetBox.setBounds(presets.removeFromLeft(150));
	presets.removeFromLeft(100);
	textureBox.setBounds(presets);

	shaderArea.removeFromTop(4);
	tabbedComp.setBounds(shaderArea);

}

void MainOverlay::initialize()
{
	moveButton.setToggleState (false, sendNotification);
            textureBox.setSelectedItemIndex (0);
            presetBox .setSelectedItemIndex (0);
            speedSlider.setValue (0.01);
            sizeSlider .setValue (0.5);
}
