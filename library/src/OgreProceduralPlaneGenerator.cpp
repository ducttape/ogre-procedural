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
#include "OgreProceduralStableHeaders.h"
#include "OgreProceduralPlaneGenerator.h"
#include "OgreProceduralUtils.h"

using namespace Ogre;

namespace OgreProcedural
{
void PlaneGenerator::addToTriangleBuffer(TriangleBuffer& buffer) const
{
	assert(numSegX>0 && numSegY>0 && "Num seg must be positive");
	assert(!normal.isZeroLength() && "Normal must not be null");
	assert(sizeX>0. && sizeY>0. && "Size must be positive");

	buffer.rebaseOffset();
	buffer.estimateVertexCount((numSegX+1)*(numSegY+1));
	buffer.estimateIndexCount(numSegX*numSegY*6);
	int offset = 0;

	Vector3 vX = normal.perpendicular();
	Vector3 vY = normal.crossProduct(vX);
	Vector3 delta1 = sizeX / (Real)numSegX * vX;
	Vector3 delta2 = sizeY / (Real)numSegY * vY;
	// build one corner of the square
	Vector3 orig = -0.5f*sizeX*vX - 0.5f*sizeY*vY;

	for (unsigned short i1 = 0; i1<=numSegX; i1++)
		for (unsigned short i2 = 0; i2<=numSegY; i2++)
		{
			addPoint(buffer, orig+i1*delta1+i2*delta2+position,
						     normal,
							 Vector2(i1/(Real)numSegX, i2/(Real)numSegY));
		}

	bool reverse = false;
	if (delta1.crossProduct(delta2).dotProduct(normal)>0)
		reverse= true;
	for (unsigned short n1 = 0; n1<numSegX; n1++)
	{
		for (unsigned short n2 = 0; n2<numSegY; n2++)
		{
			if (reverse)
			{
				buffer.index(offset+0);
				buffer.index(offset+(numSegY+1));
				buffer.index(offset+1);
				buffer.index(offset+1);
				buffer.index(offset+(numSegY+1));
				buffer.index(offset+(numSegY+1)+1);
			}
			else
			{
				buffer.index(offset+0);
				buffer.index(offset+1);
				buffer.index(offset+(numSegY+1));
				buffer.index(offset+1);
				buffer.index(offset+(numSegY+1)+1);
				buffer.index(offset+(numSegY+1));
			}
			offset++;
		}
		offset++;
	}
}
}
