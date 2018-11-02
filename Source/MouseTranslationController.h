/*
  ==============================================================================

    MouseTranslationController.h
    Created: 31 Oct 2018 1:18:13pm
    Author:  Tyler

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Types.h"
#include "Primitives.h"

class MouseTranslationController
{

public:
	MouseTranslationController();

	/// The Translation Controller is initialized with
	///     - The current mouse pointer position (to generate a ray from)
	///     - A flag indicating the axis or plane that object movement
	///       has been restricted to by the user
	///     - The 3D ‘hit’ point on the object that was picked.
	///       The plane is moved so that this point lies on it.
	///       This is the start point of object translation.
	void init(int x2D, int y2D, UINT translationConstraint,
		      Vector3D<float> planePos);

	/// The Translation Controller is updated by
	///     -passing it the current mouse pointer position
	/// It generates the next displacement vector used to move the object to
	void update(int x2D, int y2D);
	void drawPlane();

private:
	void createPlane();
	void createRay();
	void setPlaneOrientation(UINT theTranslationConstraint);
	void applyTranslationConstraint(Vector3D<float> interesetPos);

	Ray ray;
	Plane plane;
	Vector3D<float> pos;
	UINT translationConstraint;

	Vector3D<float> displacement;
	static Vector3D<float> lastPos;
};