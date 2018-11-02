/*
  ==============================================================================

    Primitives.h
    Created: 31 Oct 2018 1:24:13pm
    Author:  Tyler

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Types.h"

class Plane
{
    
public:
    Plane();
    Plane(Vector3D<float> norm, Vector3D<float> pos, UINT axes);
    
    void drawNormal();
    void draw();
    UINT32 getTransformAxes();
    void setTransformAxes(UINT newAxes);
    
private:
    Vector3D<float> normal;
    Vector3D<float> pos;
    UINT32 transformAxes;
};

class Ray
{
 
public:
    Ray();
    Ray(Vector3D<float> p0_, Vector3D<float> p1_);
    Ray(int x2D, int y2D);
    
    void generate3DRay(int x2D, int y2D);
    int intersects(Plane* p, Vector3D<float>* i);
    
private:
    Vector3D<float> p0;
    Vector3D<float> p1;
};

