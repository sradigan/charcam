#include "BaseApplication.h"
using namespace Ogre;

class CharCam : public BaseApplication
{
    public:
        CharCam(void);
        virtual ~CharCam(void);
        void stepRstOrient(const Real speed, SceneNode *node);

    protected:
        virtual void createScene(void);
        virtual void createFrameListener(void);
        virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

        //key listener
        virtual bool keyPressed(const OIS::KeyEvent& arg);
        virtual bool keyReleased(const OIS::KeyEvent& arg);

        void moveCharacter(const FrameEvent &evt);
        void moveCamera(const FrameEvent &evt);

        //boolean triggers from key input
        bool charBKW, charFWD, charRLF, charRRT, charAutoRun;
        bool camBKW, camFWD, camUP, camDWN, camRLF, camRRT;
        bool cam1st;
        bool camRotBacktoCenter;

        Ogre::Real camDistMax, camDistMin, camTargetHeight;

        // The current animation state of the object
        Ogre::AnimationState *mAnimationState;

        // The Entity we are animating
        Ogre::Entity *mEntity;
        // The SceneNode that the Entity is attached to
        Ogre::SceneNode *mNode;
        Ogre::SceneNode *mCharDirNode;
        Ogre::SceneNode *mCameraNode;
        Ogre::SceneNode *mYawNode;
        Ogre::SceneNode *mRollNode;
        Ogre::SceneNode *mCamDirNode;

        Ogre::Plane mPlane;  //Make a plane
};
 
