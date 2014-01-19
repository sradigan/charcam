#include <iostream>
#include <OgreMeshManager.h>
#include "CharCam.h"
using namespace std;
using namespace Ogre;

/* Function: CharCam::CharCam(void)
 * Description: initialize values
 */
CharCam::CharCam(void)
{
    charBKW = false;
    charFWD = false;
    charRLF = false;
    charRRT = false;
    charAutoRun = false;
    camBKW = false;
    camFWD = false;
    camRLF = false;
    camRRT = false;
    camDWN = false;
    camUP  = false;
    cam1st = false;
    camRotBacktoCenter = false;
}

/* Function: CharCam::~CharCam(void)
 * Description: clean up anything that is not automatic
 */
CharCam::~CharCam(void)
{
    mCamera->setAutoTracking(false, NULL);
}

/* Function: void CharCam::createScene(void)
 * Description: setup the scenery, and character node with camera
 */
void CharCam::createScene(void)
{
    //setup the scenery
    mSceneMgr->setAmbientLight(ColourValue(0.5f, 0.5f, 0.5f));
    mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

    mPlane.normal = Vector3::UNIT_Y; //face the plane on the y axis
    mPlane.d = 0;   //set the plain at the origin along the normal

    //Create a plane mesh, of the name "floor" and scale it to desired size
    MeshManager::getSingleton().
        createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mPlane,
                    2000.0f, 2000.0f, 10, 10, true, 1, 50.0f, 50.0f, Vector3::UNIT_Z);

    //Rig the mesh to an entity call it "plane"
    Entity* planeEnt = mSceneMgr->createEntity("plane", "floor");

    //apply an image to the plane
    planeEnt->setMaterialName("Examples/GrassFloor");
    planeEnt->setCastShadows(false);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(planeEnt);

    //setup character
    mEntity = mSceneMgr->createEntity("Robot", "robot.mesh");
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("RobotNode", Vector3(0.0f, 0.0f, 0.0f));
    mCharDirNode = mNode->createChildSceneNode("CharDirNode", Vector3(0, 0, 0));
    mCharDirNode->attachObject(mEntity);

    //setup camera
    camDistMax = (1.0f - MeshManager::getSingleton().getBoundsPaddingFactor());
    camDistMax *= mEntity->getBoundingBox().getSize().y;
    camDistMax *=2;
    camDistMin = camDistMax * 0.5f;
    camTargetHeight = camDistMin * 0.85f;

    mYawNode = mNode->createChildSceneNode("YawNode", Vector3(0, camTargetHeight, 0));
    mCamDirNode = mYawNode->createChildSceneNode("CamDirNode",Vector3(0, (-1*camTargetHeight), 0) );
    mRollNode = mYawNode->createChildSceneNode("RollNode", Vector3(0, 0, 0));
    mCameraNode = mRollNode->createChildSceneNode("CameraNode", Vector3((camDistMin - camDistMax), 0, 0));

    mCamera->setPosition(Vector3(0,0,0)); //make the cam centered
    mCamera->setOrientation(mCameraNode->getOrientation());
    mCameraNode->attachObject(mCamera); 
    mCamera->lookAt(mYawNode->getPosition());
}

/* Function: void CharCam::createFrameListener(void)
 * Description: Creates the frame listener, and sets default values/states for rendering
 */
void CharCam::createFrameListener(void)
{
    BaseApplication::createFrameListener();

    //Set idle animation
    mAnimationState = mEntity->getAnimationState("Idle");
    mAnimationState->setLoop(true);
    mAnimationState->setEnabled(true);
}

/* Function: void CharCam::moveCharacter(const FrameEvent &evt)
 * Description: Sets the character's orientation and translates it accordingly
 */
void CharCam::moveCharacter(const FrameEvent &evt)
{
    Degree charRotDegree;
    Real charMvtSclr = Real(20 * evt.timeSinceLastFrame);
    Vector3 charRotV = 
        mCharDirNode->_getDerivedOrientation() * Vector3(charMvtSclr, 0, 0);
    //change node orientation
    mNode->setOrientation(mCamDirNode->_getDerivedOrientation());
    mYawNode->resetOrientation();
    mCharDirNode->resetOrientation();

    //if there character is destined for any direction except forward, rotate
    if ( charFWD && charRLF ) {
        charRotDegree = Degree(45);
    } else if ( charFWD && charRRT ) {
        charRotDegree = Degree(-45);
    } else if ( charBKW && charRLF ) {
        charRotDegree = Degree(135);
    } else if ( charBKW && charRRT ) {
        charRotDegree = Degree(-135);
    } else if ( charBKW ) { 
        charRotDegree = Degree(180);
    } else if ( charRLF ) {
        charRotDegree = Degree(90);
    } else if ( charRRT ) {
        charRotDegree = Degree(-90);
    }
    if (charRLF || charRRT || charBKW ) {
        mCharDirNode->yaw(charRotDegree); //different
    }
    //translate the node
    mNode->translate(charRotV, Node::TS_WORLD);
}

/* Function: void CharCam::moveCamera(const FrameEvent &evt)
 * Description: Logic for rotating the camera about the character, and zooming in and out
 */
void CharCam::moveCamera(const FrameEvent &evt)
{
    Real camMvtSclr = Real(40 * evt.timeSinceLastFrame);

    //camera rotation left and right
    if ( camRLF ) {
        mYawNode->yaw(Degree(-camMvtSclr));
    } else if ( camRRT ) {
        mYawNode->yaw(Degree(camMvtSclr));
    }

    //camera rotation up and down
    if (camDWN || camUP) {
        Radian camAngle = (mRollNode->getOrientation() *
                            Vector3::UNIT_X).angleBetween(Vector3::UNIT_Y);
        if ( camDWN ) {
            if ( camAngle > Degree(45) ) {
                mRollNode->roll(Degree(camMvtSclr));
            } else {
                mRollNode->roll(Degree(-camMvtSclr));
            }
        } else if ( camUP ) {
            if ( camAngle < Degree(150) ) {
                mRollNode->roll(Degree(-camMvtSclr));
            } else {
                mRollNode->roll(Degree(camMvtSclr));
            }
        }
    }

    //camera zoom
    if ( camFWD || camBKW ) {
        Real camDist = (mCameraNode->getPosition() -
                        mYawNode->getPosition()).normalise();
        if (camFWD && (camDistMin < camDist)) { //camera zooming in
            mCameraNode->translate(Vector3(camMvtSclr,0,0), Node::TS_LOCAL);
        } else if (camBKW && (camDist < camDistMax)) { //camera zooming out
            mCameraNode->translate(Vector3(-camMvtSclr,0,0), Node::TS_LOCAL);
        }
    }
}

/* Function: void CharCam::stepRstOrient(const Real speed, SceneNode *node)
 * Description: Uses interpolation to set the orientation of the node back to center over timer
 */
void CharCam::stepRstOrient(const Real speed, SceneNode *node)
{
    Quaternion dir = node->getOrientation();
    Quaternion rotQ =
        (dir * Vector3::UNIT_X).getRotationTo(Vector3::UNIT_X) * dir;
    node->setOrientation(Quaternion::Slerp(speed, dir, rotQ, true));
}

/* Function: bool CharCam::frameRenderingQueued(const FrameEvent &evt)
 * Description: Perform tasks on the frame to be rendered next
 */
bool CharCam::frameRenderingQueued(const FrameEvent &evt)
{
    if (camRotBacktoCenter) {
        Real rotSpeed = Real(1  * evt.timeSinceLastFrame);
        stepRstOrient(rotSpeed, mRollNode);
        stepRstOrient(rotSpeed, mYawNode);
    }

    //character movement
    if (charAutoRun || charFWD || charRLF || charRRT || charBKW) {
        moveCharacter(evt);
    }

    //camera movement
    if ( camRLF || camRRT || camDWN || camUP || camFWD || camBKW) {
        moveCamera(evt);
    }

    //The animation needs to take the frame rate into account to look normal
    mAnimationState->addTime(evt.timeSinceLastFrame);
    return BaseApplication::frameRenderingQueued(evt);
}

/* Function: bool CharCam::keyPressed(const OIS::KeyEvent& arg)
 * Description: Perform operations when a key is pressed down
 */
bool CharCam::keyPressed(const OIS::KeyEvent& arg)
{
    switch(arg.key) {
    //character movement
    case OIS::KC_W:
        if (!charAutoRun) {
            charFWD = true;
        }
        break;
    case OIS::KC_S:
        charBKW = true;
        charAutoRun = false;
        charFWD = false;
        if (mCharDirNode->getOrientation().getYaw(false) != Degree(180)) {
            mCharDirNode->resetOrientation();
            mCharDirNode->rotate(Quaternion(0,0,1,0));
        }
        break;
    case OIS::KC_A:
        charRLF = true;
        break;
    case OIS::KC_D:
        charRRT = true;
        break;
    case OIS::KC_R:
        if (charFWD == true) {
            charAutoRun = !charAutoRun;
            charFWD = charAutoRun;
            camRotBacktoCenter = true;
        }
        break;
    //Camera Movement
    case OIS::KC_UP:
        camUP = true;
        break;
    case OIS::KC_DOWN:
        camDWN = true;
        break;
    case OIS::KC_LEFT:
        camRLF = true;
        break;
    case OIS::KC_RIGHT:
        camRRT = true;
        break;
    case OIS::KC_I:
        camFWD = true;
        break;
    case OIS::KC_O:
        camBKW = true;
        break;
    default:
        break;
    }

    if (charFWD || charRLF || charRRT || charBKW || charAutoRun) {
        mAnimationState = mEntity->getAnimationState("Walk");
        mAnimationState->setLoop(true);
        mAnimationState->setEnabled(true);
    } else if (camUP || camDWN || camRLF || camRRT) {
        camRotBacktoCenter = false;
    }

    //then we return the base app keyPressed function 
    //so that we get all of the functionality
return BaseApplication::keyPressed(arg);
}

/* Function: bool CharCam::keyReleased(const OIS::KeyEvent& arg)
 * Description: Perform operations when a key is released
 */
bool CharCam::keyReleased(const OIS::KeyEvent& arg)
{
    switch(arg.key) {
    //character movement
    case OIS::KC_W:
        if (!charAutoRun) {
            charFWD = false;
        }
        break;
    case OIS::KC_S:
        charBKW = false;
        break;
    case OIS::KC_A:
        charRLF = false;
        break;
    case OIS::KC_D:
        charRRT = false;
        break;
    //camera movement
    case OIS::KC_UP:
        camUP = false;
        break;
    case OIS::KC_DOWN:
        camDWN = false;
        break;
    case OIS::KC_LEFT:
        camRLF = false;
        break;
    case OIS::KC_RIGHT:
        camRRT = false;
        break;
    case OIS::KC_V:
        cam1st = !cam1st;
        if ( cam1st ) {
            mCameraNode->setPosition(Vector3(0,0,0));
            mNode->setOrientation(mCharDirNode->_getDerivedOrientation());
            mRollNode->resetOrientation();
            mYawNode->resetOrientation();
        } else {
            mCameraNode->setPosition(Vector3((-1*camTargetHeight),0,0));
            mCharDirNode->resetOrientation();
        }
        mNode->flipVisibility(false); //false = cascade to not flip children
        break;
    case OIS::KC_I:
        camFWD = false;
        break;
    case OIS::KC_O:
        camBKW = false;
        break;
    default:
        break;
    }

    if (!(charFWD || charRLF || charRRT || charBKW || charAutoRun)) {
        mAnimationState = mEntity->getAnimationState("Idle");
        mAnimationState->setLoop(true);
        mAnimationState->setEnabled(true);
    } else if (!(camUP || camDWN || camRLF || camRRT)) {
        camRotBacktoCenter = true;
    }

    //then we return the base app keyPressed function 
    //so that we get all of the functionality
    return BaseApplication::keyPressed(arg);
}

int main(int argc, char *argv[])
{
    // Create application object
    CharCam app;
    try {
        app.go();
    } catch( Exception& e ) {
        std::cerr << "An exception has occured: " <<
        e.getFullDescription().c_str() << std::endl;
    }

    return 0;
}
