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
#ifndef PROCEDURAL_SHAPE_INCLUDED
#define PROCEDURAL_SHAPE_INCLUDED

#include "OgreVector2.h"
#include "ProceduralPlatform.h"
#include "ProceduralUtils.h"
#include "OgreMesh.h"
#include "OgreManualObject.h"
#include "ProceduralRoot.h"
#include "ProceduralMultiShape.h"

namespace Procedural
{
enum Side {SIDE_LEFT, SIDE_RIGHT};

/** Describes a succession of interconnected 2D points.
 * It can be closed or not, and there's always an outside and an inside
 */
class _ProceduralExport Shape
{
	std::vector<Ogre::Vector2> mPoints;
	bool mClosed;
	Side mOutSide;

public:
	/// Default constructor
	Shape() : mClosed(false), mOutSide(SIDE_RIGHT) {}

	//---------------------------------------------------------------------------
	/// Adds a point to the shape
	inline Shape& addPoint(const Ogre::Vector2& pt)
	{
		mPoints.push_back(pt);
		return *this;
	}	
	//---------------------------------------------------------------------------
	/// Adds a point to the shape
	inline Shape& addPoint(Ogre::Real x, Ogre::Real y)
	{
		mPoints.push_back(Ogre::Vector2(x, y));
		return *this;
	}	
	//---------------------------------------------------------------------------
	/// Clears the content of the shape
	inline Shape& reset()
	{
		mPoints.clear();
		return *this;
	}
	//---------------------------------------------------------------------------
	/// Gets raw vector data of this shape
	inline std::vector<Ogre::Vector2> getPoints() const
	{
		return mPoints;
	}
	
	/**
	 * Bounds-safe method to get a point : it will allow you to go beyond the bounds
	 */
	inline const Ogre::Vector2& getPoint(int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}
	
	/**
	 * Makes the shape a closed shape, ie it will automatically connect 
	 * the last point to the first point.
	 */
	inline Shape& close()
	{
		assert(mPoints.size()>0 && "Cannot close an empty shape");
		mClosed = true;
		return *this;
	}

	/**
	 * Sets which side (left or right) is on the outside of the shape.
	 * It is used for such things as normal generation
	 * Default is right, which corresponds to placing points anti-clockwise.
	 */
	inline Shape& setOutSide(Side side)
	{
		mOutSide = side;
		return *this;
	}	
	/* --------------------------------------------------------------------------- */
	/// Gets which side is out
	inline Side getOutSide() const
	{
		return mOutSide;
	}
	/* --------------------------------------------------------------------------- */
	/// Switches the inside and the outside
	inline Shape& switchSide()
	{
		mOutSide = (mOutSide == SIDE_LEFT)? SIDE_RIGHT: SIDE_LEFT;
		return *this;
	}	
	/// Gets the number of segments in that shape
	inline size_t getSegCount() const
	{
		return (mPoints.size()-1) + (mClosed?1:0);
	}	
	/// Gets whether the shape is closed or not
	inline bool isClosed() const
	{
	  return mClosed;
	}	

	/**
	 * Returns local direction after the current point
	 */
	inline Ogre::Vector2 getDirectionAfter(int i) const
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!mClosed && i == mPoints.size()-1 && i>0)
			return (mPoints[i] - mPoints[i-1]).normalisedCopy();
		else
			return (getPoint(i+1) - getPoint(i)).normalisedCopy();
	}

	/**
	 * Returns local direction after the current point
	 */
	inline Ogre::Vector2 getDirectionBefore(int i) const
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!mClosed && i == 1)
			return (mPoints[1] - mPoints[0]).normalisedCopy();
		else
			return (getPoint(i) - getPoint(i-1)).normalisedCopy();
	}	

	/// Gets the average between before direction and after direction
	inline Ogre::Vector2 getAvgDirection(int i) const
	{
		return (getDirectionAfter(i) + getDirectionBefore(i)).normalisedCopy();
	}	

	/// Gets the shape normal just after that point
	inline Ogre::Vector2 getNormalAfter(int i) const
	{
		if (mOutSide==SIDE_RIGHT)
		return -getDirectionAfter(i).perpendicular();
		return getDirectionAfter(i).perpendicular();
	}	

	/// Gets the shape normal just before that point
	inline Ogre::Vector2 getNormalBefore(int i) const
	{
		if (mOutSide==SIDE_RIGHT)
		return -getDirectionBefore(i).perpendicular();
		return getDirectionBefore(i).perpendicular();
	}	

	/// Gets the "normal" of that point ie an average between before and after normals
	inline Ogre::Vector2 getAvgNormal(int i) const
	{
		if (mOutSide==SIDE_RIGHT)
		return -getAvgDirection(i).perpendicular();
		return getAvgDirection(i).perpendicular();
	}

	/**
	 * Outputs a mesh representing the shape.
	 * Mostly for debugging purposes
	 */
	Ogre::MeshPtr realizeMesh(const std::string& name="");
	
	/**
	 * Appends the shape vertices to a manual object being edited
	 */
	void _appendToManualObject(Ogre::ManualObject* manual);

	/**
	 * Tells whether a point is inside a shape or not
	 * @arg point The point to check
	 * @return true if the point is inside this shape, false otherwise
	 */
	bool isPointInside(const Ogre::Vector2& point) const;
	 
	/**
	 * Computes the intersection between this shape and another one.
	 * Both shapes must be closed.
	 * @arg other The shape against which the intersection is computed
	 * @return The intersection of two shapes, as a new shape
	 */
	MultiShape booleanIntersect(const Shape& other) const;
	 
	/**
	 * Computes the union between this shape and another one.
	 * Both shapes must be closed.
	 */
	MultiShape booleanUnion(const Shape& other) const;
	 
	/**
	 * Computes the difference between this shape and another one.
	 * Both shapes must be closed.
	 */
	MultiShape booleanDifference(const Shape& other) const;
	 
	/**
	 * On a closed shape, find if the outside is located on the right
	 * or on the left. If the outside can easily be determined, 
	 * you'd rather use setOutside(), which doesn't need any computation.
	 */
	Side findRealOutSide() const;	

	/**
	 * Applies the given translation to all the points already defined.
	 * Has strictly no effect on the points defined after that
	 * @param translation the translation vector
	 */
	Shape& translate(const Ogre::Vector2& translation)
	{
		for (std::vector<Ogre::Vector2>::iterator it = mPoints.begin(); it!=mPoints.end(); it++)
			*it+=translation;
		return *this;
	}

	/**
	 * Applies the given rotation to all the points already defined.
	 * Has strictly no effect on the points defined after that
	 * @param angle angle of rotation
	 */
	Shape& rotate(Ogre::Radian angle)
	{
		Ogre::Real c = Ogre::Math::Cos(angle.valueRadians());
		Ogre::Real s = Ogre::Math::Sin(angle.valueRadians());
		for (std::vector<Ogre::Vector2>::iterator it = mPoints.begin(); it!=mPoints.end(); it++)
		{
			Ogre::Real x = it->x;
			Ogre::Real y = it->y;
			it->x = c * x - s * y;
			it->y = s * x + c * y;
		}
		return *this;
	}

	/**
	 * Applies the given scale to all the points already defined.
	 * Has strictly no effect on the points defined after that
	 * @param amount amount of scale
	 */	
	Shape& scale(Ogre::Real amount)
	{
		return scale(amount, amount);
	}

	/**
	 * Applies the given scale to all the points already defined.
	 * Has strictly no effect on the points defined after that
	 * @param scaleX amount of scale in the X direction
	 * @param scaleY amount of scale in the Y direction
	 */	
	Shape& scale(Ogre::Real scaleX, Ogre::Real scaleY)
	{
		for (std::vector<Ogre::Vector2>::iterator it = mPoints.begin(); it!=mPoints.end(); it++)
		{
			it->x *= scaleX;
			it->y *= scaleY;
		}
		return *this;
	}

	/**
	 * Applies the given scale to all the points already defined.
	 * Has strictly no effect on the points defined after that
	 * @param amount of scale
	 */	
	Shape& scale(Ogre::Vector2 amount)
	{
		return scale(amount.x, amount.y);
	}

	/// Returns the total lineic length of that shape
	Ogre::Real getTotalLength() const
	{
		Ogre::Real length = 0;
		for (unsigned int i=0;i<mPoints.size()-1;i++)
			length+=(mPoints[i+1]-mPoints[i]).length();
		if (mClosed)
			length+=(mPoints.back()-*mPoints.begin()).length();
		return length;
	}

	/// Gets a position on the shape with index of the point and a percentage of position on the segment
	/// @arg i index of the segment
	/// @arg coord a number between 0 and 1 meaning the percentage of position on the segment
	inline Ogre::Vector2 getPosition(int i, Ogre::Real coord) const
	{
		assert(mClosed||i<mPoints.size()-1 && "Out of Bounds");
		assert(coord>=0. && coord<=1. && "Coord must be comprised between 0 and 1");
		Ogre::Vector2 A = getPoint(i);
		Ogre::Vector2 B = getPoint(i+1);
		return A + coord*(B-A);
	}
	
	/// Gets a position on the shape from lineic coordinate
	/// @arg coord lineic coordinate
	inline Ogre::Vector2 getPosition(Ogre::Real coord) const
	{
		assert(mPoints.size()>=2 && "The shape must at least contain 2 points");
		int i=0;
		while(true)
		{
			Ogre::Real nextLen = (getPoint(i+1) - getPoint(i)).length();
			if (coord>nextLen)
				coord-=nextLen;
			else
				return getPosition(i, coord);
			if (!mClosed && i>= mPoints.size()-2)
				return mPoints.back();
			i++;
		}
	}

	Ogre::Real findBoundingRadius() const
	{
		Ogre::Real sqRadius=0.f;
		for (unsigned int i=0;i<mPoints.size();i++)
			sqRadius=std::max(sqRadius,mPoints[i].squaredLength());
		return Ogre::Math::Sqrt(sqRadius);
	}
		
	MultiShape thicken(Ogre::Real amount) 
	{		
		if (!mClosed)
		{
			Shape s;
			s.setOutSide(mOutSide);
			for (unsigned int i=0;i<mPoints.size();i++)
				s.addPoint(mPoints[i]+amount*getAvgNormal(i));
			for (int i=mPoints.size()-1;i>=0;i--)
				s.addPoint(mPoints[i]-amount*getAvgNormal(i));
			s.close();
			return MultiShape().addShape(s);
		} 
		else 
		{
			MultiShape ms;
			Shape s1;
			for (unsigned int i=0;i<mPoints.size();i++)
				s1.addPoint(mPoints[i]+amount*getAvgNormal(i));
			s1.close();
			s1.setOutSide(mOutSide);
			ms.addShape(s1);
			Shape s2;
			for (unsigned int i=0;i<mPoints.size();i++)			
				s2.addPoint(mPoints[i]-amount*getAvgNormal(i));
			s2.close();
			s2.setOutSide(mOutSide==SIDE_LEFT?SIDE_RIGHT:SIDE_LEFT);
			ms.addShape(s2);
			return ms;						
		}
	}

	private:

	enum BooleanOperationType { BOT_UNION, BOT_INTERSECTION, BOT_DIFFERENCE};

	MultiShape _booleanOperation(const Shape& other, BooleanOperationType opType) const;

	struct IntersectionInShape
	{
		int index[2];
		bool onVertex[2];
		Ogre::Vector2 position;
		IntersectionInShape(int i, int j, Ogre::Vector2 intersect) : position(intersect)
		{
			index[0] = i;
			index[1] = j;
			onVertex[0] = false;
			onVertex[1] = false;
		}		
	};

	bool _isLookingForOutside(BooleanOperationType opType, char shapeSelector) const;
	
	char _isIncreasing(Ogre::Real d, BooleanOperationType opType, char shapeSelector) const;
		
	bool _findWhereToGo(const Shape* inputShapes[], BooleanOperationType opType, IntersectionInShape intersection, Ogre::uint8& shapeSelector, char& isIncreasing, int& currentSegment) const;
	
	void _findAllIntersections(const Shape& other, std::vector<IntersectionInShape>& intersections) const;

};
}

#endif
