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
#include "Extrusion.h"
#include "OgreProcedural.h"

//-------------------------------------------------------------------------------------
void Sample_Extrusion::createScene(void)
{
		// Setup Procedural root (crappy init method, have to find another one)
		OgreProcedural::Root::getInstance()->sceneManager = mSceneMgr;
		// Test primitive generation
		OgreProcedural::PlaneGenerator().setNumSegX(20).setNumSegY(20).setSizeX(150).setSizeY(150).setUTile(5.0).setVTile(5.0).realizeMesh("planeMesh");
		putMesh2("planeMesh");
		//OgreProcedural::Path p = OgreProcedural::Path().addPoint(0,5,0).addPoint(0,4,10).addPoint(10,5,10).addPoint(20,3,0).close();
		OgreProcedural::Path p = OgreProcedural::CatmullRomSpline3().setNumSeg(8).addPoint(0,5,0).addPoint(0,4,10).addPoint(10,5,10).addPoint(20,3,0).close().realizePath();
		//OgreProcedural::Shape s = OgreProcedural::Shape().addPoint(-1,0).addPoint(0,1).addPoint(1,0).close();
		OgreProcedural::Shape s = OgreProcedural::CatmullRomSpline2().addPoint(-1,0).addPoint(0,1).addPoint(1,0).close().realizeShape();
		OgreProcedural::Extruder().setExtrusionPath(&p).setShapeToExtrude(&s).realizeMesh("extrudedMesh");
		putMesh("extrudedMesh");

		// Not-closed shape
		OgreProcedural::Shape s2 = OgreProcedural::CatmullRomSpline2().addPoint(0,4).addPoint(5,6).addPoint(1,10).addPoint(5,15).addPoint(0,20).setNumSeg(16).setOutSide(OgreProcedural::SIDE_LEFT).realizeShape();
		//OgreProcedural::Shape s2 = OgreProcedural::CatmullRomSpline2().addPoint(0,0).addPoint(5,5).addPoint(0,10).setNumSeg(4).setOutSide(OgreProcedural::SIDE_LEFT).realizeShape();
		//OgreProcedural::Shape s3 = OgreProcedural::KochanekBartelsSpline2().addPoint(0,0).addPoint(Ogre::Vector2(5,5),-1,0,-1).addPoint(0,10).addPoint(5,15).addPoint(0,20).setNumSeg(16).setOutSide(OgreProcedural::SIDE_LEFT).realizeShape();
		//OgreProcedural::Shape s3 = OgreProcedural::RectangleShape().setWidth(5.).setHeight(5.).realizeShape();
		OgreProcedural::Shape s3 = OgreProcedural::CircleShape().setRadius(3.).realizeShape();


		// Test Lathe generator
		OgreProcedural::Lathe().setShapeToExtrude(&s2).realizeMesh("lathedMesh");
		putMesh("lathedMesh");

		// Test triangulator
		//OgreProcedural::Triangulator::triangulate(s2).transformToMesh(mSceneMgr,"toto");
		//putMesh("toto");

		// Test display shapes
		/*s2.realizeMesh("shape2");
		putMesh("shape2");*/
		/*s3.realizeMesh("shape3");
		putMesh("shape3");*/

		// Test display path
		/*p.realizeMesh("path");
		putMesh("path");*/

}

void Sample_Extrusion::createCamera(void)
{
	BaseApplication::createCamera();
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	mSceneMgr->setShadowFarDistance(100.0);
	mSceneMgr->setShadowTextureSize(1024);
	mSceneMgr->setAmbientLight(ColourValue::Black);
	// Setup camera and light
	mCamera->setPosition(0,50,-50);
	mCamera->lookAt(0,0,0);
	// Slow down speed, as the scene is small
	mCameraMan->setTopSpeed(20);

	Light* l = mSceneMgr->createLight("myLight");
	l->setType(Light::LT_DIRECTIONAL);
	l->setDirection(Vector3(0,-1,1).normalisedCopy());
	l->setDiffuseColour(ColourValue(.7f,.5f,.5f));
	l->setSpecularColour(ColourValue::White);

	movingLight = mSceneMgr->createLight("movingLight");
	movingLight->setType(Light::LT_POINT);
	movingLight->setDiffuseColour(ColourValue(.5f,.5f,.7f));
	movingLight->setSpecularColour(ColourValue::White);
	movingLight->setPosition(mCamera->getPosition());
	movingLight->setCastShadows(false);
}

bool Sample_Extrusion::frameStarted(const FrameEvent& evt)
{
	movingLight->setPosition(mCamera->getPosition());
	return true;
}


void Sample_Extrusion::putMesh2(const std::string& meshName, const Vector3& position)
{
	Entity* ent2 = mSceneMgr->createEntity(meshName);
	SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	sn->attachObject(ent2);
	sn->setPosition(position);
	ent2->setMaterialName("Examples/Rockwall");
	ent2->setCastShadows(false);
}

void Sample_Extrusion::putMesh(const std::string& meshName, const Vector3& position)
{
	Entity* ent2 = mSceneMgr->createEntity(meshName);
	SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	sn->attachObject(ent2);
	sn->setPosition(position);
	ent2->setMaterialName("Examples/BeachStones");
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		Sample_Extrusion app;

		try {
			app.go();
		} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif
