#include "PlayScene.h"
#include "AppDelegate.h"

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs{8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0; //flag for packages manager
}

// 현재 지원하려는 운영체제는 WINDOW와 ANDROID로 이외의 운영체제는 지원하지 않도록 한다.
// 지원 넘버 WINDOW(3), LINUX(5) = ANDROID

bool AppDelegate::applicationDidFinishLaunching()
{
	int			 nOSTag(CC_TARGET_PLATFORM);
	Size		 szFrameSize(0.0f, 0.0f);
	Scene*		 pScene(nullptr);
	GLView*		 pGLView(nullptr);
	Director*	 pDirector(nullptr);
	C_PlayScene* pMain(nullptr);

	if (!m_arOSTable[nOSTag])
	{
		Director::getInstance()->end(); // 지원되지 않는 운영체제는 강제 종료.

		return false;
	}

	pDirector = Director::getInstance();
	pGLView   = pDirector->getOpenGLView();

    if(!pGLView)
	{
		pGLView = GLViewImpl::createWithRect("test_TwoWeek", m_recWindow);

        pDirector->setOpenGLView(pGLView);
    }

    pDirector->setDisplayStats(true);

	pDirector->setAnimationInterval(1.0f / 1000.0f);

	pGLView->setDesignResolutionSize(m_recWindow.getMaxX(), m_recWindow.getMaxY(), ResolutionPolicy::SHOW_ALL);

    register_all_packages();

	pMain  = C_PlayScene::create();
	pScene = pMain->getMainScene();
	
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
