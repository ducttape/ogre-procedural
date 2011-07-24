/*
-----------------------------------------------------------------------------
This source file is part of ogre-procedural

For the latest info, see http://code.google.com/p/ogre-procedural/

Copyright (c) 2010 Michael Broutin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef PROCEDURAL_SHAPE_GENERATORS_INCLUDED
#define PROCEDURAL_SHAPE_GENERATORS_INCLUDED

#include "ProceduralShape.h"

namespace Procedural
{
//-----------------------------------------------------------------------
/// Base class for Shape generators
template<class T>
class BaseSpline2
{
protected:
	/// The number of segments between 2 control points
	unsigned int mNumSeg;
	/// Whether the shape will be closed or not
	bool mClosed;
	/// The "out" side of the shape
	Side mOutSide;
public:
	/// Default constructor
	BaseSpline2() : mNumSeg(4), mClosed(false), mOutSide(SIDE_RIGHT) {}
	
	/// Sets the out side of the shape
	T& setOutSide(Side outSide)
	{
		mOutSide = outSide;
		return (T&)*this;
	}

	/// Gets the out side of the shape
	Side getOutSide() const
	{
		return mOutSide;
	}

	/// Sets the number of segments between 2 control points
	T& setNumSeg(int numSeg)
	{
		assert(numSeg>=1);
		mNumSeg = numSeg;
		return (T&)*this;
	}

	/// Closes the spline
	T& close()
	{
		mClosed = true;
		return (T&)*this;
	}
};

//-----------------------------------------------------------------------
/**
 * Produces a shape from Cubic Hermite control points
 */
class _ProceduralExport CubicHermiteSpline2 : public BaseSpline2<CubicHermiteSpline2>
{	
	struct ControlPoint
	{
		Ogre::Vector2 position;
		Ogre::Vector2 tangentBefore;
		Ogre::Vector2 tangentAfter;	
		
		ControlPoint(Ogre::Vector2 p, Ogre::Vector2 before, Ogre::Vector2 after) : position(p), tangentBefore(before), tangentAfter(after) {}
	};

	std::vector<ControlPoint> mPoints;	
	
public:
	/// Adds a control point
	void addPoint(Ogre::Vector2 p, Ogre::Vector2 before, Ogre::Vector2 after)
	{
		mPoints.push_back(ControlPoint(p, before, after));
	}
	/// Safely gets a control point
	const ControlPoint& safeGetPoint(int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}

	/**
	 * Builds a shape from control points
	 */
	Shape realizeShape();
};

//-----------------------------------------------------------------------
/**
 * Builds a shape from a Catmull-Rom Spline.
 * A catmull-rom smoothly interpolates position between control points
 */
class _ProceduralExport CatmullRomSpline2 : public BaseSpline2<CatmullRomSpline2>
{	
	std::vector<Ogre::Vector2> mPoints;
	public:	
	/// Adds a control point
	CatmullRomSpline2& addPoint(const Ogre::Vector2& pt)
	{
		mPoints.push_back(pt);
		return *this;
	}

	/// Adds a control point
	CatmullRomSpline2& addPoint(Ogre::Real x, Ogre::Real y)
	{
		mPoints.push_back(Ogre::Vector2(x,y));
		return *this;
	}
	
	/// Safely gets a control point
	const Ogre::Vector2& safeGetPoint(int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}
	
	/**
	 * Build a shape from bezier control points
	 */
	Shape realizeShape();
};

//-----------------------------------------------------------------------
/**
 * Builds a shape from a Kochanek Bartels spline.
 *
 * More details here : http://en.wikipedia.org/wiki/Kochanek%E2%80%93Bartels_spline
 */
class _ProceduralExport KochanekBartelsSpline2 : public BaseSpline2<KochanekBartelsSpline2>
{	
	struct ControlPoint
	{
		Ogre::Vector2 position;
		Ogre::Real tension;
		Ogre::Real bias;
		Ogre::Real continuity;
		
		ControlPoint(Ogre::Vector2 p, Ogre::Real t, Ogre::Real b, Ogre::Real c) : position(p), tension(t), bias(b), continuity(c) {}
		ControlPoint(Ogre::Vector2 p) : position(p), tension(0.), bias(0.), continuity(0.) {}
	};

	std::vector<ControlPoint> mPoints;
	
public:
	/// Adds a control point
	KochanekBartelsSpline2& addPoint(Ogre::Real x, Ogre::Real y)
	{
		mPoints.push_back(ControlPoint(Ogre::Vector2(x,y)));
		return *this;
	}

	/// Adds a control point
	KochanekBartelsSpline2& addPoint(Ogre::Vector2 p)
	{
		mPoints.push_back(ControlPoint(p));
		return *this;
	}

	/// Safely gets a control point
	const ControlPoint& safeGetPoint(int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}

	/**
	 * Adds a control point to the spline
	 * @arg p Point position
	 * @arg t Tension    +1 = Tight            -1 = Round
	 * @arg b Bias       +1 = Post-shoot       -1 = Pre-shoot
	 * @arg c Continuity +1 = Inverted Corners -1 = Box Corners
	 */
	KochanekBartelsSpline2& addPoint(Ogre::Vector2 p, Ogre::Real t, Ogre::Real b, Ogre::Real c)
	{
		mPoints.push_back(ControlPoint(p,t,b,c));
		return *this;
	}

	/**
	 * Builds a shape from control points
	 */
	Shape realizeShape();
	
};

//-----------------------------------------------------------------------
/**
 * Builds a rectangular shape
 */
class _ProceduralExport RectangleShape
{
	Ogre::Real mWidth,mHeight;

	public:
	/// Default constructor
	RectangleShape() : mWidth(1.0), mHeight(1.0) {}

	/// Sets width
	RectangleShape& setWidth(Ogre::Real width)
	{
		mWidth = width;
		return *this;
	}

	/// Sets height
	RectangleShape& setHeight(Ogre::Real height)
	{
		mHeight = height;
		return *this;
	}

	/// Builds the shape
	Shape realizeShape()
	{
		Shape s;
		s.addPoint(-.5f*mWidth,-.5f*mHeight)
		 .addPoint(.5f*mWidth,-.5f*mHeight)
		 .addPoint(.5f*mWidth,.5f*mHeight)
		 .addPoint(-.5f*mWidth,.5f*mHeight)
		 .close();
		return s;
	}
};

//-----------------------------------------------------------------------
/**
 * Builds a circular shape
 */
class _ProceduralExport CircleShape
{
	Ogre::Real mRadius;
	unsigned int mNumSeg;

	public:
	/// Default constructor
	CircleShape() : mRadius(1.0), mNumSeg(8) {}

	/// Sets radius
	CircleShape& setRadius(Ogre::Real radius)
	{
		mRadius = radius;
		return *this;
	}

	/// Sets number of segments
	CircleShape& setNumSeg(unsigned int numSeg)
	{
		mNumSeg = numSeg;
		return *this;
	}

	/// Builds the shape
	Shape realizeShape()
	{
		Shape s;
		Ogre::Real deltaAngle = Ogre::Math::TWO_PI/(Ogre::Real)mNumSeg;
		for (unsigned int i = 0; i < mNumSeg; ++i)
		{
			s.addPoint(mRadius*cosf(i*deltaAngle), mRadius*sinf(i*deltaAngle));
		}
		s.close();
		return s;
	}
};
}

#endif
