#ifndef _OverScene_H_
#define _OverScene_H_
#include "cocos2d.h"

USING_NS_CC;

class OverScene:public Layer{
public:
	static Scene* createScene();

	CREATE_FUNC(OverScene);

	virtual bool init();
	void restartMenu(Ref *pSender);
};
#endif