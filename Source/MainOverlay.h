/*
  ==============================================================================

    MainOverlay.h
    Created: 24 Oct 2018 4:20:28pm
    Author:  Tyler

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
//#include "MainComponent.h"
#include "Shader.h"
#include "Shape.h"
#include "Texture.h"

class MainComponent;

//==============================================================================
/*
*/
class MainOverlay    : public Component,
					   private CodeDocument::Listener,
	                   private Slider::Listener,
	                   private Timer
{
public:
	MainOverlay();
	MainOverlay(MainComponent& m);
    ~MainOverlay();

	/*
	void mouseDown(const MouseEvent& e) override
	{
		demo.draggableOrientation.mouseDown(e.getPosition());
	}

	void mouseDrag(const MouseEvent& e) override
	{
		demo.draggableOrientation.mouseDrag(e.getPosition());
	}

	void mouseWheelMove(const MouseEvent&, const MouseWheelDetails& d) override
	{
		sizeSlider.setValue(sizeSlider.getValue() + d.deltaY);
	}

	void mouseMagnify(const MouseEvent&, float magnifyAmmount) override
	{
		sizeSlider.setValue(sizeSlider.getValue() + magnifyAmmount - 1.0f);
	}
	*/
    void paint (Graphics&) override;
	void initialize();
    void resized() override;

	//void selectTexture(int itemID);id selectTexture(int itemID);
	void updateTexturesList();
	void updateShader();
	void selectPreset(int preset);
	

	Label statusLabel;

private:
	void selectTexture(int itemID);

	void codeDocumentTextInserted(const String& /*newText*/, int /*insertIndex*/) override
	{
	}

	void codeDocumentTextDeleted(int /*startIndex*/, int /*endIndex*/) override
	{
	}

	void timerCallback() override;

	void sliderValueChanged(Slider*) override
	{
		//zoom maybe
	}

	void lookAndFeelChanged() override
	{
		auto editorBackground = getUIColourIfAvailable(LookAndFeel_V4::ColourScheme::UIColour::windowBackground,
			Colours::white);

		for (int i = tabbedComp.getNumTabs(); i >= 0; --i)
			tabbedComp.setTabBackgroundColour(i, editorBackground);

		vertexEditorComp.setColour(CodeEditorComponent::backgroundColourId, editorBackground);
		fragmentEditorComp.setColour(CodeEditorComponent::backgroundColourId, editorBackground);
	}

	MainComponent& demo;

	Label speedLabel { {}, "Speed:" },
		  zoomLabel  { {}, "Zoom:" };

	CodeDocument vertexDocument, fragmentDocument;
	CodeEditorComponent vertexEditorComp{ vertexDocument,   nullptr },
		fragmentEditorComp{ fragmentDocument, nullptr };

	TabbedComponent tabbedComp{ TabbedButtonBar::TabsAtLeft };

	ComboBox presetBox, textureBox;

	Label presetLabel{ {}, "Shader Preset:" },
		textureLabel{ {}, "Texture:" };

	Slider speedSlider, sizeSlider;

	ToggleButton moveButton{ "Move" };
	ToggleButton rotateButton{ "Rotate" };

	OwnedArray<DemoTexture> textures;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainOverlay)
};
