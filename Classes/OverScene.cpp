#include "OverScene.h"
#include "GameScene.h"

Scene* OverScene::createScene(){
	auto scene=Scene::create();

	auto layer=OverScene::create();

	scene->addChild(layer);

	return scene;
}
bool OverScene::init(){
	if(!Layer::init()){
		return false;
	}

	Size size=Director::getInstance()->getVisibleSize();

	auto label=Label::create("GAME OVER","Arial",50);
	label->setPosition(Point(size.width/2,size.height/2));
	this->addChild(label);

	auto mLabel=Label::create("Restart","Arial",30);
	auto uiRestart=MenuItemLabel::create(mLabel,CC_CALLBACK_1(OverScene::restartMenu,this));
	uiRestart->setAnchorPoint(Point::ZERO);
	uiRestart->setPosition(Point::ZERO.x,100);
	auto menu=Menu::create(uiRestart,NULL);
	menu->setAnchorPoint(Point::ZERO);
	menu->setPosition(Point(10,10));
	this->addChild(menu);
	return true;
}

void OverScene::restartMenu(Ref *pSender){
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,GameScene::create()));
}