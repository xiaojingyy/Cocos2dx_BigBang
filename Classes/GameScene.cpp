#include "GameScene.h"
#include "GameLayer.h"

bool GameScene::init(){
	bool bRet=false;
	do{
		CC_BREAK_IF(!Scene::init());

		auto layer=GameLayer::create();
		CC_BREAK_IF(!layer);

		this->addChild(layer);

		bRet=true;
	}while(0);
	return bRet;
}