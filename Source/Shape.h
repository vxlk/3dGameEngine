/*
  ==============================================================================

    Shape.h
    Created: 24 Oct 2018 12:40:52pm
    Author:  Tyler

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WavefrontObjParser.h"
#include "Utils.h"

//==============================================================================
struct Vertex
{
	float position[3];
	float normal[3];
	float colour[4];
	float texCoord[2];
};

//==============================================================================
// This class just manages the attributes that the shaders use.
struct Attributes
{
	Attributes(OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
	{
		position.reset(createAttribute(openGLContext, shader, "position"));
		normal.reset(createAttribute(openGLContext, shader, "normal"));
		sourceColour.reset(createAttribute(openGLContext, shader, "sourceColour"));
		textureCoordIn.reset(createAttribute(openGLContext, shader, "textureCoordIn"));
	}

	void enable(OpenGLContext& openGLContext)
	{
		if (position.get() != nullptr)
		{
			openGLContext.extensions.glVertexAttribPointer(position->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			openGLContext.extensions.glEnableVertexAttribArray(position->attributeID);
		}

		if (normal.get() != nullptr)
		{
			openGLContext.extensions.glVertexAttribPointer(normal->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 3));
			openGLContext.extensions.glEnableVertexAttribArray(normal->attributeID);
		}

		if (sourceColour.get() != nullptr)
		{
			openGLContext.extensions.glVertexAttribPointer(sourceColour->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 6));
			openGLContext.extensions.glEnableVertexAttribArray(sourceColour->attributeID);
		}

		if (textureCoordIn.get() != nullptr)
		{
			openGLContext.extensions.glVertexAttribPointer(textureCoordIn->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 10));
			openGLContext.extensions.glEnableVertexAttribArray(textureCoordIn->attributeID);
		}
	}

	void disable(OpenGLContext& openGLContext)
	{
		if (position.get() != nullptr)        openGLContext.extensions.glDisableVertexAttribArray(position->attributeID);
		if (normal.get() != nullptr)          openGLContext.extensions.glDisableVertexAttribArray(normal->attributeID);
		if (sourceColour.get() != nullptr)    openGLContext.extensions.glDisableVertexAttribArray(sourceColour->attributeID);
		if (textureCoordIn.get() != nullptr)  openGLContext.extensions.glDisableVertexAttribArray(textureCoordIn->attributeID);
	}

	ScopedPointer<OpenGLShaderProgram::Attribute> position, normal, sourceColour, textureCoordIn;

private:
	static OpenGLShaderProgram::Attribute* createAttribute(OpenGLContext& openGLContext,
		OpenGLShaderProgram& shader,
		const char* attributeName)
	{
		if (openGLContext.extensions.glGetAttribLocation(shader.getProgramID(), attributeName) < 0)
			return nullptr;

		return new OpenGLShaderProgram::Attribute(shader, attributeName);
	}
};

//==============================================================================
// This class just manages the uniform values that the demo shaders use.
struct Uniforms
{
		Uniforms(OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
		{
			projectionMatrix.reset(createUniform(openGLContext, shader, "projectionMatrix"));
			viewMatrix.reset(createUniform(openGLContext, shader, "viewMatrix"));
			texture.reset(createUniform(openGLContext, shader, "demoTexture"));
			lightPosition.reset(createUniform(openGLContext, shader, "lightPosition"));
			bouncingNumber.reset(createUniform(openGLContext, shader, "bouncingNumber"));
		}

		ScopedPointer<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix, texture, lightPosition, bouncingNumber;

	private:
		static OpenGLShaderProgram::Uniform* createUniform(OpenGLContext& openGLContext,
			OpenGLShaderProgram& shader,
			const char* uniformName)
		{
			if (openGLContext.extensions.glGetUniformLocation(shader.getProgramID(), uniformName) < 0)
				return nullptr;

			return new OpenGLShaderProgram::Uniform(shader, uniformName);
		}
};


//==============================================================================
/** This loads a 3D model from an OBJ file and converts it into some vertex buffers
that we can draw.
*/
struct Shape
{
	Shape(OpenGLContext& openGLContext)
	{
		if (shapeFile.load(loadEntireAssetIntoString("ElvenLongBow.obj")).wasOk())
			for (auto* s : shapeFile.shapes)
				vertexBuffers.add(new VertexBuffer(openGLContext, *s));
	}

	void draw(OpenGLContext& openGLContext, Attributes& attributes)
	{
		for (auto* vertexBuffer : vertexBuffers)
		{
			vertexBuffer->bind();

			attributes.enable(openGLContext);
			glDrawElements(GL_TRIANGLES, vertexBuffer->numIndices, GL_UNSIGNED_INT, 0);
			attributes.disable(openGLContext);
		}
	}

private:
	struct VertexBuffer
	{
		VertexBuffer(OpenGLContext& context, WavefrontObjFile::Shape& shape) : openGLContext(context)
		{
			numIndices = shape.mesh.indices.size();

			openGLContext.extensions.glGenBuffers(1, &vertexBuffer);
			openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

			Array<Vertex> vertices;
			createVertexListFromMesh(shape.mesh, vertices, Colours::green);

			openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, vertices.size() * (int) sizeof(Vertex),
				vertices.getRawDataPointer(), GL_STATIC_DRAW);

			openGLContext.extensions.glGenBuffers(1, &indexBuffer);
			openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * (int) sizeof(juce::uint32),
				shape.mesh.indices.getRawDataPointer(), GL_STATIC_DRAW);
		}

		~VertexBuffer()
		{
			openGLContext.extensions.glDeleteBuffers(1, &vertexBuffer);
			openGLContext.extensions.glDeleteBuffers(1, &indexBuffer);
		}

		void bind()
		{
			openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		}

		GLuint vertexBuffer, indexBuffer;
		int numIndices;
		OpenGLContext& openGLContext;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VertexBuffer)
	};

	WavefrontObjFile shapeFile;
	OwnedArray<VertexBuffer> vertexBuffers;

	static void createVertexListFromMesh(const WavefrontObjFile::Mesh& mesh, Array<Vertex>& list, Colour colour)
	{
		auto scale = 0.2f;
		WavefrontObjFile::TextureCoord defaultTexCoord = { 0.5f, 0.5f };
		WavefrontObjFile::Vertex defaultNormal = { 0.5f, 0.5f, 0.5f };

		for (int i = 0; i < mesh.vertices.size(); ++i)
		{
			auto& v = mesh.vertices.getReference(i);

			auto& n = (i < mesh.normals.size() ? mesh.normals.getReference(i)
				: defaultNormal);

			auto& tc = (i < mesh.textureCoords.size() ? mesh.textureCoords.getReference(i)
				: defaultTexCoord);

			list.add({ { scale * v.x, scale * v.y, scale * v.z, },
			{ scale * n.x, scale * n.y, scale * n.z, },
			{ colour.getFloatRed(), colour.getFloatGreen(), colour.getFloatBlue(), colour.getFloatAlpha() },
			{ tc.x, tc.y } });
		}
	}
};