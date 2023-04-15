#ifndef _LoadingScene_H_
#define _LoadingScene_H_
#include "cocos2d.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"
#include"GameLayer.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;
using namespace cocos2d;
class LoadingScene:public Scene{
public:
	virtual bool init();
	CREATE_FUNC(LoadingScene);
	GameLayer* gamelayer;
};
#endif
