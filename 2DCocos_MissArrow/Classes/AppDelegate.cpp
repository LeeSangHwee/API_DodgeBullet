#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();

	if (!glview)
	{
		glview = GLViewImpl::createWithRect("SSoDDuck_02_2", Rect(0, 0, 720, 960));
		director->setOpenGLView(glview);
	}

	glview->setDesignResolutionSize(720, 960, ResolutionPolicy::EXACT_FIT);

	director->setDisplayStats(false);
	director->setAnimationInterval(1.0 / 60);

	auto scene = HelloWorld::createScene();
	director->runWithScene(scene);

	return true;
}

void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
}
