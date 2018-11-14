/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{

	if (auto* peer = getPeer())
		peer->setCurrentRenderingEngine(0);
	

	/*
	TODO: CREATE ASSET FOLDERS UPON 1ST LOAD IF NOT EXIST
	
	*/
	setOpaque(true);
	controlsOverlay.reset(new MainOverlay(*this));
	addAndMakeVisible(controlsOverlay.get());

	openGLContext.setRenderer(this);
	openGLContext.attachTo(*this);
	openGLContext.setContinuousRepainting(true);
	//renderOpenGL();
	controlsOverlay->initialize();

	if (!controlsOverlay->isMouseButtonDown())
		rotation += (float)rotationSpeed;

	toFront(true);
	setSize(1280, 720);

	
}

MainComponent::~MainComponent()
{
    // This shuts down the GL system and stops the rendering calls.
	openGLContext.detach();
}

void MainComponent::newOpenGLContextCreated()
{
	// nothing to do in this case - we'll initialise our shaders + textures
	// on demand, during the render callback.
	freeAllContextObjects();

	if (controlsOverlay.get() != nullptr)
		controlsOverlay->updateShader();
}

void MainComponent::openGLContextClosing()
{
	// When the context is about to close, you must use this callback to delete
	// any GPU resources while the context is still current.
	freeAllContextObjects();

	if (lastTexture != nullptr)
		setTexture(lastTexture);
}

void MainComponent::freeAllContextObjects()
{
	shape.reset();
	shader.reset();
	attributes.reset();
	uniforms.reset();
	texture.release();
}


//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // You can add your component specific drawing code here!
    // This will draw over the top of the openGL background.
	
	g.setColour(getLookAndFeel().findColour(Label::textColourId));
	g.setFont(20);
	g.drawText("Cling", 25, 25, 100, 30, Justification::left);
	g.drawLine(20, 20, 100, 20);
	g.drawLine(20, 50, 100, 50);
	
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	controlsOverlay->setBounds(getLocalBounds());
	draggableOrientation.setViewport(getLocalBounds());
}
