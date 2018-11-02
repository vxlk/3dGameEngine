/*
  ==============================================================================

    Shader.h
    Created: 24 Oct 2018 12:34:57pm
    Author:  Tyler

  ==============================================================================
*/

#pragma once

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Shader : public OpenGLShaderProgram
{
public:
	using OpenGLShaderProgram::OpenGLShaderProgram;
};

struct ShaderPreset
{
	const char* name;
	const char* vertexShader;
	const char* fragmentShader;
};

static Array<ShaderPreset> getPresets()
{
#define SHADER_DEMO_HEADER \
            "/*  This is a live OpenGL Shader demo.\n" \
            "    Edit the shader program below and it will be \n" \
            "    compiled and applied to the model above!\n" \
            "*/\n\n"

	ShaderPreset presets[] =
	{
		{
			"Texture + Lighting",

			SHADER_DEMO_HEADER
			"attribute vec4 position;\n"
		"attribute vec4 normal;\n"
		"attribute vec4 sourceColour;\n"
		"attribute vec2 textureCoordIn;\n"
		"\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"uniform vec4 lightPosition;\n"
		"\n"
		"varying vec4 destinationColour;\n"
		"varying vec2 textureCoordOut;\n"
		"varying float lightIntensity;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    destinationColour = sourceColour;\n"
		"    textureCoordOut = textureCoordIn;\n"
		"\n"
		"    vec4 light = viewMatrix * lightPosition;\n"
		"    lightIntensity = dot (light, normal);\n"
		"\n"
		"    gl_Position = projectionMatrix * viewMatrix * position;\n"
		"}\n",

		SHADER_DEMO_HEADER
#if JUCE_OPENGL_ES
		"varying lowp vec4 destinationColour;\n"
		"varying lowp vec2 textureCoordOut;\n"
		"varying highp float lightIntensity;\n"
#else
		"varying vec4 destinationColour;\n"
		"varying vec2 textureCoordOut;\n"
		"varying float lightIntensity;\n"
#endif
		"\n"
		"uniform sampler2D demoTexture;\n"
		"\n"
		"void main()\n"
		"{\n"
#if JUCE_OPENGL_ES
		"   highp float l = max (0.3, lightIntensity * 0.3);\n"
		"   highp vec4 colour = vec4 (l, l, l, 1.0);\n"
#else
		"   float l = max (0.3, lightIntensity * 0.3);\n"
		"   vec4 colour = vec4 (l, l, l, 1.0);\n"
#endif
		"    gl_FragColor = colour * texture2D (demoTexture, textureCoordOut);\n"
		"}\n"
		},

		{
			"Textured",

			SHADER_DEMO_HEADER
			"attribute vec4 position;\n"
		"attribute vec4 sourceColour;\n"
		"attribute vec2 textureCoordIn;\n"
		"\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"\n"
		"varying vec4 destinationColour;\n"
		"varying vec2 textureCoordOut;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    destinationColour = sourceColour;\n"
		"    textureCoordOut = textureCoordIn;\n"
		"    gl_Position = projectionMatrix * viewMatrix * position;\n"
		"}\n",

		SHADER_DEMO_HEADER
#if JUCE_OPENGL_ES
		"varying lowp vec4 destinationColour;\n"
		"varying lowp vec2 textureCoordOut;\n"
#else
		"varying vec4 destinationColour;\n"
		"varying vec2 textureCoordOut;\n"
#endif
		"\n"
		"uniform sampler2D demoTexture;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_FragColor = texture2D (demoTexture, textureCoordOut);\n"
		"}\n"
		},

		{
			"Flat Colour",

			SHADER_DEMO_HEADER
			"attribute vec4 position;\n"
		"attribute vec4 sourceColour;\n"
		"attribute vec2 textureCoordIn;\n"
		"\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"\n"
		"varying vec4 destinationColour;\n"
		"varying vec2 textureCoordOut;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    destinationColour = sourceColour;\n"
		"    textureCoordOut = textureCoordIn;\n"
		"    gl_Position = projectionMatrix * viewMatrix * position;\n"
		"}\n",

		SHADER_DEMO_HEADER
#if JUCE_OPENGL_ES
		"varying lowp vec4 destinationColour;\n"
		"varying lowp vec2 textureCoordOut;\n"
#else
		"varying vec4 destinationColour;\n"
		"varying vec2 textureCoordOut;\n"
#endif
		"\n"
		"void main()\n"
		"{\n"
		"    gl_FragColor = destinationColour;\n"
		"}\n"
		},

		{
			"Rainbow",

			SHADER_DEMO_HEADER
			"attribute vec4 position;\n"
		"attribute vec4 sourceColour;\n"
		"attribute vec2 textureCoordIn;\n"
		"\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"\n"
		"varying vec4 destinationColour;\n"
		"varying vec2 textureCoordOut;\n"
		"\n"
		"varying float xPos;\n"
		"varying float yPos;\n"
		"varying float zPos;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    vec4 v = vec4 (position);\n"
		"    xPos = clamp (v.x, 0.0, 1.0);\n"
		"    yPos = clamp (v.y, 0.0, 1.0);\n"
		"    zPos = clamp (v.z, 0.0, 1.0);\n"
		"    gl_Position = projectionMatrix * viewMatrix * position;\n"
		"}",

		SHADER_DEMO_HEADER
#if JUCE_OPENGL_ES
		"varying lowp vec4 destinationColour;\n"
		"varying lowp vec2 textureCoordOut;\n"
		"varying lowp float xPos;\n"
		"varying lowp float yPos;\n"
		"varying lowp float zPos;\n"
#else
		"varying vec4 destinationColour;\n"
		"varying vec2 textureCoordOut;\n"
		"varying float xPos;\n"
		"varying float yPos;\n"
		"varying float zPos;\n"
#endif
		"\n"
		"void main()\n"
		"{\n"
		"    gl_FragColor = vec4 (xPos, yPos, zPos, 1.0);\n"
		"}"
		},

		{
			"Changing Colour",

			SHADER_DEMO_HEADER
			"attribute vec4 position;\n"
		"attribute vec2 textureCoordIn;\n"
		"\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"\n"
		"varying vec2 textureCoordOut;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    textureCoordOut = textureCoordIn;\n"
		"    gl_Position = projectionMatrix * viewMatrix * position;\n"
		"}\n",

		SHADER_DEMO_HEADER
		"#define PI 3.1415926535897932384626433832795\n"
		"\n"
#if JUCE_OPENGL_ES
		"precision mediump float;\n"
		"varying lowp vec2 textureCoordOut;\n"
#else
		"varying vec2 textureCoordOut;\n"
#endif
		"uniform float bouncingNumber;\n"
		"\n"
		"void main()\n"
		"{\n"
		"   float b = bouncingNumber;\n"
		"   float n = b * PI * 2.0;\n"
		"   float sn = (sin (n * textureCoordOut.x) * 0.5) + 0.5;\n"
		"   float cn = (sin (n * textureCoordOut.y) * 0.5) + 0.5;\n"
		"\n"
		"   vec4 col = vec4 (b, sn, cn, 1.0);\n"
		"   gl_FragColor = col;\n"
		"}\n"
		},

		{
			"Simple Light",

			SHADER_DEMO_HEADER
			"attribute vec4 position;\n"
		"attribute vec4 normal;\n"
		"\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"uniform vec4 lightPosition;\n"
		"\n"
		"varying float lightIntensity;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    vec4 light = viewMatrix * lightPosition;\n"
		"    lightIntensity = dot (light, normal);\n"
		"\n"
		"    gl_Position = projectionMatrix * viewMatrix * position;\n"
		"}\n",

		SHADER_DEMO_HEADER
#if JUCE_OPENGL_ES
		"varying highp float lightIntensity;\n"
#else
		"varying float lightIntensity;\n"
#endif
		"\n"
		"void main()\n"
		"{\n"
#if JUCE_OPENGL_ES
		"   highp float l = lightIntensity * 0.25;\n"
		"   highp vec4 colour = vec4 (l, l, l, 1.0);\n"
#else
		"   float l = lightIntensity * 0.25;\n"
		"   vec4 colour = vec4 (l, l, l, 1.0);\n"
#endif
		"\n"
		"    gl_FragColor = colour;\n"
		"}\n"
		},

		{
			"Flattened",

			SHADER_DEMO_HEADER
			"attribute vec4 position;\n"
		"attribute vec4 normal;\n"
		"\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"uniform vec4 lightPosition;\n"
		"\n"
		"varying float lightIntensity;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    vec4 light = viewMatrix * lightPosition;\n"
		"    lightIntensity = dot (light, normal);\n"
		"\n"
		"    vec4 v = vec4 (position);\n"
		"    v.z = v.z * 0.1;\n"
		"\n"
		"    gl_Position = projectionMatrix * viewMatrix * v;\n"
		"}\n",

		SHADER_DEMO_HEADER
#if JUCE_OPENGL_ES
		"varying highp float lightIntensity;\n"
#else
		"varying float lightIntensity;\n"
#endif
		"\n"
		"void main()\n"
		"{\n"
#if JUCE_OPENGL_ES
		"   highp float l = lightIntensity * 0.25;\n"
		"   highp vec4 colour = vec4 (l, l, l, 1.0);\n"
#else
		"   float l = lightIntensity * 0.25;\n"
		"   vec4 colour = vec4 (l, l, l, 1.0);\n"
#endif
		"\n"
		"    gl_FragColor = colour;\n"
		"}\n"
		},

		{
			"Toon Shader",

			SHADER_DEMO_HEADER
			"attribute vec4 position;\n"
		"attribute vec4 normal;\n"
		"\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"uniform vec4 lightPosition;\n"
		"\n"
		"varying float lightIntensity;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    vec4 light = viewMatrix * lightPosition;\n"
		"    lightIntensity = dot (light, normal);\n"
		"\n"
		"    gl_Position = projectionMatrix * viewMatrix * position;\n"
		"}\n",

		SHADER_DEMO_HEADER
#if JUCE_OPENGL_ES
		"varying highp float lightIntensity;\n"
#else
		"varying float lightIntensity;\n"
#endif
		"\n"
		"void main()\n"
		"{\n"
#if JUCE_OPENGL_ES
		"    highp float intensity = lightIntensity * 0.5;\n"
		"    highp vec4 colour;\n"
#else
		"    float intensity = lightIntensity * 0.5;\n"
		"    vec4 colour;\n"
#endif
		"\n"
		"    if (intensity > 0.95)\n"
		"        colour = vec4 (1.0, 0.5, 0.5, 1.0);\n"
		"    else if (intensity > 0.5)\n"
		"        colour  = vec4 (0.6, 0.3, 0.3, 1.0);\n"
		"    else if (intensity > 0.25)\n"
		"        colour  = vec4 (0.4, 0.2, 0.2, 1.0);\n"
		"    else\n"
		"        colour  = vec4 (0.2, 0.1, 0.1, 1.0);\n"
		"\n"
		"    gl_FragColor = colour;\n"
		"}\n"
		}
	};

	return Array<ShaderPreset>(presets, numElementsInArray(presets));
};

