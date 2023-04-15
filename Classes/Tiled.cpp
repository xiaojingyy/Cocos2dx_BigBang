#include "Tiled.h"
#include"GameLayer.h"

const int Tiled::nums[16]={0,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};

Tiled::Tiled(){
	level=0;
}

bool Tiled::init(){
	bool bRet=false;
	do{
		CC_BREAK_IF(!Node::init());

		auto cache=SpriteFrameCache::getInstance();

		this->backround=Sprite::create("level0.png");
		this->setScale(1.13);
		this->backround->setPosition(Point::ZERO);
		this->addChild(backround);
		// 设置场景的名称为"tiled_scene"
		this->setName("tiled_scene");
		bRet=true;
	}while(0);
	return bRet;
}

//输出最终图像
void Tiled::setLevel(int l ){
	auto cache = SpriteFrameCache::getInstance();
	this->level = l;

	//this->backround->setTexture();
	this->backround->initWithFile(String::createWithFormat(this->m_anima_path.c_str(), level)->getCString());
	if (m_anima_path == "level%d.png") {
		CCLOG("level.png");
	}
	else if (m_anima_path == "images/level%d.png") {
		CCLOG("images/level.png");
	}
	this->setScale(1.13);
}
