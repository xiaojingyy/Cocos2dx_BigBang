#ifndef _GameScene_H_
#define _GameScene_H_
#include "cocos2d.h"

USING_NS_CC;
class GameScene:public Scene{
public:
	CREATE_FUNC(GameScene);
	virtual bool init();
};
#endif