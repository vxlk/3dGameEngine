/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainOverlay.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   :	private OpenGLRenderer,
						private AsyncUpdater,
					    public Component
{
public:
	MainComponent();

	~MainComponent();

	void newOpenGLContextCreated() override;

	void openGLContextClosing() override;

	void freeAllContextObjects();

	// This is a virtual method in OpenGLRenderer, and is called when it's time
	// to do your GL rendering.
	void renderOpenGL() override
	{
		jassert(OpenGLHelpers::isContextActive());

		auto desktopScale = (float)openGLContext.getRenderingScale();

		OpenGLHelpers::clear(getUIColourIfAvailable(LookAndFeel_V4::ColourScheme::UIColour::windowBackground,
			Colours::lightblue));

		if (textureToUse != nullptr)
			if (!textureToUse->applyTo(texture))
				textureToUse = nullptr;

		// First draw our background graphics to demonstrate the OpenGLGraphicsContext class
		if (doBackgroundDrawing)
			drawBackground2DStuff(desktopScale);

		updateShader();   // Check whether we need to compile a new shader

		if (shader.get() == nullptr)
			return;

		// Having used the juce 2D renderer, it will have messed-up a whole load of GL state, so
		// we need to initialise some important settings before doing our normal GL 3D drawing..
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		openGLContext.extensions.glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		glViewport(0, 0, roundToInt(desktopScale * getWidth()), roundToInt(desktopScale * getHeight()));

		texture.bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		shader->use();

		if (uniforms->projectionMatrix.get() != nullptr)
			uniforms->projectionMatrix->setMatrix4(getProjectionMatrix().mat, 1, false);

		if (uniforms->viewMatrix.get() != nullptr)
			uniforms->viewMatrix->setMatrix4(getViewMatrix().mat, 1, false);

		if (uniforms->texture.get() != nullptr)
			uniforms->texture->set((GLint)0);

		if (uniforms->lightPosition.get() != nullptr)
			uniforms->lightPosition->set(-15.0f, 10.0f, 15.0f, 0.0f);

		if (uniforms->bouncingNumber.get() != nullptr)
			uniforms->bouncingNumber->set(bouncingNumber.getValue());

		shape->draw(openGLContext, *attributes);

		// Reset the element buffers so child Components draw correctly
		openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
		openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (!controlsOverlay->isMouseButtonDown())
			rotation += (float)rotationSpeed;
	}

	Matrix3D<float> getProjectionMatrix() const
	{
		auto w = 1.0f / (scale + 0.1f);
		auto h = w * getLocalBounds().toFloat().getAspectRatio(false);

		return Matrix3D<float>::fromFrustum(-w, w, -h, h, 4.0f, 30.0f);
	}

	Matrix3D<float> getViewMatrix() const
	{
		auto viewMatrix = draggableOrientation.getRotationMatrix()
			* Vector3D<float>(0.0f, 1.0f, -10.0f);

		auto rotationMatrix = Matrix3D<float>::rotation({ rotation, rotation, -0.3f });

		return rotationMatrix * viewMatrix;
	}

	void setTexture(DemoTexture* t)
	{
		lastTexture = textureToUse = t;
	}

	void setShaderProgram(const String& vertexShader, const String& fragmentShader)
	{
		newVertexShader = vertexShader;
		newFragmentShader = fragmentShader;
	}

	void paint(Graphics&) override;

	void resized() override;

	Draggable3DOrientation draggableOrientation;
	bool doBackgroundDrawing = false;
	float scale = 0.5f, rotationSpeed = 0.0f;
	BouncingNumber bouncingNumber;

private:
	void handleAsyncUpdate() override
	{
		controlsOverlay->statusLabel.setText(statusText, dontSendNotification);
	}

	void drawBackground2DStuff(float desktopScale)
	{
		// Create an OpenGLGraphicsContext that will draw into this GL window..
		ScopedPointer<LowLevelGraphicsContext> glRenderer(createOpenGLGraphicsContext(openGLContext,
			roundToInt(desktopScale * getWidth()),
			roundToInt(desktopScale * getHeight())));

		if (glRenderer.get() != nullptr)
		{
			Graphics g(*glRenderer);
			g.addTransform(AffineTransform::scale(desktopScale));

			for (auto s : stars)
			{
				auto size = 0.25f;

				// This stuff just creates a spinning star shape and fills it..
				Path p;
				p.addStar({ getWidth()  * s.x.getValue(),
					getHeight() * s.y.getValue() },
					7,
					getHeight() * size * 0.5f,
					getHeight() * size,
					s.angle.getValue());

				auto hue = s.hue.getValue();

				g.setGradientFill(ColourGradient(Colours::green.withRotatedHue(hue).withAlpha(0.8f),
					0, 0,
					Colours::red.withRotatedHue(hue).withAlpha(0.5f),
					0, (float)getHeight(), false));
				g.fillPath(p);
			}
		}
	}

	OpenGLContext openGLContext;

	ScopedPointer<MainOverlay> controlsOverlay;

	float rotation = 0.0f;

	ScopedPointer<OpenGLShaderProgram> shader;
	ScopedPointer<Shape> shape;
	ScopedPointer<Attributes> attributes;
	ScopedPointer<Uniforms> uniforms;

	OpenGLTexture texture;
	DemoTexture* textureToUse = nullptr;
	DemoTexture* lastTexture = nullptr;

	String newVertexShader, newFragmentShader, statusText;

	struct BackgroundStar
	{
		SlowerBouncingNumber x, y, hue, angle;
	};

	BackgroundStar stars[3];

	//==============================================================================
	void updateShader()
	{
		if (newVertexShader.isNotEmpty() || newFragmentShader.isNotEmpty())
		{
			ScopedPointer<OpenGLShaderProgram> newShader(new OpenGLShaderProgram(openGLContext));

			if (newShader->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(newVertexShader))
				&& newShader->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(newFragmentShader))
				&& newShader->link())
			{
				shape.reset();
				attributes.reset();
				uniforms.reset();

				shader.reset(newShader.release());
				shader->use();

				shape.reset(new Shape(openGLContext));
				attributes.reset(new Attributes(openGLContext, *shader));
				uniforms.reset(new Uniforms(openGLContext, *shader));

				statusText = "GLSL: v" + String(OpenGLShaderProgram::getLanguageVersion(), 2);
			}
			else
			{
				statusText = newShader->getLastError();
			}

			triggerAsyncUpdate();

			newVertexShader = {};
			newFragmentShader = {};
		}
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

//==============================================================================


