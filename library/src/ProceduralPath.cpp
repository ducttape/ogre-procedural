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
#include "ProceduralStableHeaders.h"
#include "ProceduralPath.h"

namespace Procedural
{

//-----------------------------------------------------------------------
Path CatmullRomSpline3::realizePath()
	{
		Path path;

		int numPoints = closed?points.size():points.size()-1;		
		for (int i=0;i<numPoints;i++)
		{			
			const Ogre::Vector3& P1 = safeGetPoint(i-1);
			const Ogre::Vector3& P2 = safeGetPoint(i);
			const Ogre::Vector3& P3 = safeGetPoint(i+1);
			const Ogre::Vector3& P4 = safeGetPoint(i+2);

			for (int j=0;j<numSeg;j++)
			{				
				Ogre::Real t = (Ogre::Real)j/(Ogre::Real)numSeg;
				Ogre::Real t2 = t*t;
				Ogre::Real t3 = t*t2;
				Ogre::Vector3 P = 0.5f*((-t3+2.f*t2-t)*P1 + (3.f*t3-5.f*t2+2.f)*P2 + (-3.f*t3+4.f*t2+t)*P3 + (t3-t2)*P4);
				path.addPoint(P);
			}
			if (i==points.size()-2 && !closed)
			{
				path.addPoint(P3);
			}

		}
		if (closed)
			path.close();

		return path;
	}
}