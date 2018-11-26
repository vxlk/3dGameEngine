/*
  ==============================================================================

    Texture.h
    Created: 24 Oct 2018 4:32:37pm
    Author:  Tyler

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Utils.h"

inline static Image resizeImageToPowerOfTwo(Image image)
{
	if (!(isPowerOfTwo(image.getWidth()) && isPowerOfTwo(image.getHeight())))
		return image.rescaled(jmin(1024, nextPowerOfTwo(image.getWidth())),
			jmin(1024, nextPowerOfTwo(image.getHeight())));

	return image;
}


struct DemoTexture
{
	virtual ~DemoTexture() {}
	virtual bool applyTo(OpenGLTexture&) = 0;

	String name;
};

struct DynamicTexture : public DemoTexture
{
	DynamicTexture() { name = "Dynamically-generated texture"; }

	Image image;
	BouncingNumber x, y;

	bool applyTo(OpenGLTexture& texture) override
	{
		int size = 128;

		if (!image.isValid())
			image = Image(Image::ARGB, size, size, true);

		{
			Graphics g(image);
			g.fillAll(Colours::lightcyan);

			g.setColour(Colours::darkred);
			g.drawRect(0, 0, size, size, 2);

			g.setColour(Colours::green);
			g.fillEllipse(x.getValue() * size * 0.9f, y.getValue() * size * 0.9f, size * 0.1f, size * 0.1f);

			g.setColour(Colours::black);
			g.setFont(40);
			g.drawFittedText(String(Time::getCurrentTime().getMilliseconds()), image.getBounds(), Justification::centred, 1);
		}

		texture.loadImage(image);
		return true;
	}
};

struct BuiltInTexture : public DemoTexture
{
	BuiltInTexture(const char* nm, const void* imageData, size_t imageSize)
		: image(resizeImageToPowerOfTwo(ImageFileFormat::loadFrom(imageData, imageSize)))
	{
		name = nm;
	}

	Image image;

	bool applyTo(OpenGLTexture& texture) override
	{
		texture.loadImage(image);
		return false;
	}
};

struct TextureFromFile : public DemoTexture
{
	TextureFromFile(const File& file)
	{
		name = file.getFileName();
		image = resizeImageToPowerOfTwo(ImageFileFormat::loadFrom(file));
	}

	Image image;

	bool applyTo(OpenGLTexture& texture) override
	{
		texture.loadImage(image);
		return false;
	}
};

struct TextureFromAsset : public DemoTexture
{
	TextureFromAsset(const char* assetName)
	{
		name = assetName;
		image = resizeImageToPowerOfTwo(getImageFromAssets(assetName));
	}

	Image image;

	bool applyTo(OpenGLTexture& texture) override
	{
		texture.loadImage(image);
		return false;
	}
};

struct TextureFromUI : public DemoTexture
{
	TextureFromUI(const char* assetName)
	{
		name = assetName;
		image = resizeImageToPowerOfTwo(getImageFromUI(assetName));
	}

	Image image;

	bool applyTo(OpenGLTexture& texture) override
	{
		texture.loadImage(image);
		return false;
	}
};