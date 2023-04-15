#include "GameLayer.h"
#include "Tiled.h"
#include "OverScene.h"
#include <time.h>
#include <random>
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "2d/CCRenderTexture.h"
#include"2d/CCNode.h"
#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"
#include <Windows.h>

USING_NS_CC;

using namespace CocosDenshion;//��������Ƶ
using namespace cocos2d;

#define RC_CONVERT_TO_X(rc) (rc*153+130)
#define RC_CONVERT_TO_Y(rc) (rc*160+345)

int  GameLayer::score = 0;


//�̳� Layer �ಢ��д�� init() ������
//ʹ�� CC_BREAK_IF ���ж� Layer �ĳ�ʼ���Ƿ�ɹ������ʧ����ֱ�ӷ��� false��
//ʹ�� schedule() ������ʱ���� cloudMove() �����������Ʋ��ƶ���
//����ͼ����Դ�ļ� sucai.plist �� sucai.png����������ӵ� SpriteFrameCache �У��������ʹ�á�
//ʹ�� cocostudio::GUIReader::getInstance() �� UI �����ļ� go_main_scene_1.json �ж�ȡ UI �ؼ������˳������¿�ʼ��ť��������ʾ�ȣ���������ӵ���ǰ�������С�
//��ӱ��ⱳ�����˳������¿�ʼ��ť��������ʾ����ϷԪ�أ���������λ�á���С����Ӧ�����ȡ�
//ʹ�� UserDefault::getInstance() ��ȡ�洢�ڱ��ص���߷֣���ʾ����Ϸ�����С�
//��ʼ����Ϸ���棬����Ӵ�������������Ӧ��ҵĲ�����
//���� true����ʾ��ʼ���ɹ���
bool GameLayer::init() {


	bool bRet = false;
	do {
		CC_BREAK_IF(!Layer::init());//û�г�ʼ���ɹ�����������

		//schedule����ע��һ����ʱ���ص������������ƶ��˶�������
		schedule(schedule_selector(GameLayer::cloudMove), 0.0f);
		num_cloud = 0;
		number = 0;
		//��������
		auto cache = SpriteFrameCache::getInstance();     //�����֡����
		auto size = Director::getInstance()->getVisibleSize();
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sucai.plist", "sucai.png");



		//���ر���ͼ
		_ui = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("go_main_scene_1.json");
		this->addChild(_ui);//һ��Ҫ�ǵ����

		/////////////////////////////////////////////////////////////////////////////////////////////////////
			//��ӱ��ⱳ��
		auto headBg = Sprite::create("splash_logo1.png");
		headBg->setAnchorPoint(Vec2(0, 1));
		headBg->setPosition(Vec2(0, 1260));
		this->addChild(headBg, 1);


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//�����˳������¿�ʼ��ť         
		auto exitItem = MenuItemSprite::create(Sprite::create("exit1.png"), Sprite::create("exit1.png"), nullptr, [](Ref* sender) {
			auto visibleSize = Director::getInstance()->getVisibleSize();
		auto origin = Director::getInstance()->getVisibleOrigin();
		auto centerPos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
		auto dialogBg = Sprite::create("dialog_bg.png");
		dialogBg->setPosition(centerPos);
		dialogBg->setZOrder(1000);
		auto confirmItem = MenuItemSprite::create(Sprite::create("confirm5.png"), Sprite::create("confirm5.png"), [](Ref* sender) {
			Director::getInstance()->end();
			});

		auto cancelItem = MenuItemSprite::create(Sprite::create("cencel1.png"), Sprite::create("cencel1.png"), [=](Ref* sender) {
			dialogBg->removeFromParentAndCleanup(true);
			});
		auto menu = Menu::create(confirmItem, cancelItem, nullptr);
		menu->setPosition(centerPos + Vec2(0, 0));
		menu->setZOrder(1000);
		confirmItem->setPosition(Vec2(100, -500));
		cancelItem->setPosition(Vec2(-200, -500));
		dialogBg->addChild(menu);

		Director::getInstance()->getRunningScene()->addChild(dialogBg);
			});

		auto restartItem = MenuItemSprite::create(Sprite::create("restart6.png"), Sprite::create("restart6.png"), nullptr, [](Ref* sender) {
			auto visibleSize = Director::getInstance()->getVisibleSize();
		auto origin = Director::getInstance()->getVisibleOrigin();

		auto centerPos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

		auto dialogBg = Sprite::create("dialog_bg.png");
		dialogBg->setPosition(centerPos);
		dialogBg->setZOrder(1000);

		auto confirmItem = MenuItemSprite::create(Sprite::create("confirm5.png"), Sprite::create("confirm5.png"), [](Ref* sender) {
			Director::getInstance()->replaceScene(GameScene::create());
			});

		auto cancelItem = MenuItemSprite::create(Sprite::create("cencel1.png"), Sprite::create("cencel1.png"), [=](Ref* sender) {
			dialogBg->removeFromParentAndCleanup(true);
			});

		auto menu = Menu::create(confirmItem, cancelItem, nullptr);
		menu->setPosition(centerPos + Vec2(0, 0));
		menu->setZOrder(1000);
		confirmItem->setPosition(Vec2(100, -500));
		cancelItem->setPosition(Vec2(-200, -500));
		dialogBg->addChild(menu);

		Director::getInstance()->getRunningScene()->addChild(dialogBg);
			});

		exitItem->setPosition(Point(300, 1200));
		restartItem->setPosition(Point(420, 1200));

		auto menu = Menu::create(exitItem, restartItem, nullptr);
		menu->setAnchorPoint(Point::ZERO);
		menu->setPosition(Point::ZERO);
		this->addChild(menu, 2);




		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// ���ر�������
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic("background.mp3");

		// ���ű������ֲ�ѭ������
		SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);

		// ������Ƶ�ؼ�
		auto audioButton = ui::Button::create("music1.png", "music1.png");
		audioButton->setPosition(Vec2(540, 1200));
		this->addChild(audioButton);
		this->setZOrder(1000);
		// ������Ƶ�ؼ��Ļص�����
		audioButton->addClickEventListener([=](Ref* sender) {
			// ��ͣ��������
			if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
				SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
				audioButton->loadTextures("unmusic1.png", "unmusic1.png", "");
			}
			else {
				// ���ű�������
				SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
				audioButton->loadTextures("music1.png", "music1.png", "");
			}
			});

		/////////////////////////////////////////////////////////////////////////////////////////////////////
				//��ӷ�����ʾ
		lScore = Label::create("", "Arial", 40);
		lScore->setPosition(Point(size.width / 2, 1000));
		this->addChild(lScore);
		_best_score = (TextAtlas*)ui::Helper::seekWidgetByName(_ui, "best_score");
		_score = (TextAtlas*)ui::Helper::seekWidgetByName(_ui, "score");
		//�����߷���ʾ
		int high = UserDefault::getInstance()->getIntegerForKey("HighScore", 0);
		_best_score->setString(String::createWithFormat("%d", high)->getCString());
		this->setZOrder(1000);
		//ʱ����ͼƬ
		title_sprite = Sprite::create("level1.png");
		title_sprite->setPosition(126, 1030);
		this->addChild(title_sprite);

		title_imageview = (ImageView*)ui::Helper::seekWidgetByName(_ui, "Image_8");
		title_imageview->loadTexture("t_1.png");//��ʱ��������
		best_level = 1;//��ʼ��Ϊ1


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//��ͼ����
		// ������ͼ��ť
		auto screenshotButton = ui::Button::create("camera_nor.png", "camera_nor.png", "");
		screenshotButton->setPosition(Vec2(365, 100));
		this->addChild(screenshotButton);

		screenshotButton->loadTexturePressed("camera_click.png");

		Size visibleSize = Director::getInstance()->getVisibleSize();
		// ��ӽ�ͼ��ť�ĵ���¼�
		screenshotButton->addClickEventListener([=](Ref* sender) {
			// ���� RenderTexture ����
			auto renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888);
		renderTexture->retain();
		// ��ʼ��׽����
		renderTexture->begin();
		// ��Ⱦ����
		this->visit();
		Director::getInstance()->getRenderer()->render();
		CCLOG("Capture the screenshot of the game scene");
		// ������׽����
		renderTexture->end();

		//����ʹ�ü�������¼ͼƬ��Ϣ��
		//// ��ȡ����������
		//int count = UserDefault::getInstance()->getIntegerForKey("screenshot_count", 1);


		//���������������ڱ��ص�xml�ļ��У��ļ�·������,���п����޸���ʷ��߷ֺ������ͼ�ļ�����
		//C:\Users\14002\AppData\Local\MyCppGame

		auto image = renderTexture->newImage();

		//ʹ�ü���������
		//image->saveToFile("E:/cocos2dx/helloworld/screenshot/screenshot.png" + std::to_string(count) + ".png");

		// ��ȡ��ǰʱ��
		time_t now = time(nullptr);
		tm* time = localtime(&now);

		// ����ǰʱ��ת��Ϊ�ַ�����ʽ
		char timeStr[32];
		strftime(timeStr, sizeof(timeStr), "%Y%m%d%H%M%S", time);

		// ���ý�ͼ�ļ���Ϊ��ǰʱ����ַ������ϡ�.png����׺
		//����ı���·��Ӧ������Ϊ�ͳ����ܹ�һֱ��һ����ļ�������Resources�У������洴���ļ��д�Ž�ͼ��Ҫ��Ȼ���˵���֮���޷�����

		std::string fileName = "screenshot/" + std::string(timeStr) + ".png";

		image->saveToFile(fileName);


		//ʵ���������ֻ�һ����һ����Ч��
		// �������ֲ�
		auto mask = LayerColor::create(Color4B(255, 255, 255, 255), visibleSize.width, visibleSize.height);
		mask->setPosition(Vec2::ZERO);
		this->addChild(mask);
		// ��ʾ���ֲ㣬���ڶ���ʱ���������
		mask->runAction(Sequence::create(
			FadeOut::create(0.5),
			CallFunc::create([=]() {
				mask->removeFromParentAndCleanup(true);
				}),
			nullptr
					));
		//����̨�����ͼ��Ϣ
		CCLOG("Screenshot saved");
		auto node = cocos2d::Node::create();
		node->setPosition(Vec2(100, 100));
		this->addChild(node);



		//�������Ĵ���
		//// ���¼���������,�ӱ���XML�ļ���ȡ
		//count++;
		//UserDefault::getInstance()->setIntegerForKey("screenshot_count", count);
		//UserDefault::getInstance()->flush();
		//������֮ǰʹ�ü���������ͼ��ʱ��Ĵ��룬Ŀǰʹ��ʱ���������ͼ

		//�ͷ�ͼ��
		renderTexture->release();
		image->release();
			});


		/////////////////////////////////////////////////
		//���������ܣ����ð�ť
		auto sharebutton = ui::Button::create("share.png", "share.png");
		sharebutton->setPosition(Vec2(600, 110));
		sharebutton->addClickEventListener(CC_CALLBACK_1(GameLayer::onButtonClicked, this));
		this->addChild(sharebutton);


		/////////////////////////////////////////////////
		// ����about��ť
		auto button = ui::Button::create("about_nor.png", "about_click.png");
		button->setPosition(Vec2(100, 100));
		this->addChild(button);

		// �󶨵���¼�
		button->addClickEventListener([=](Ref* sender) {
			// ��������ͼƬ
			auto bg = Sprite::create("dialog_bg.png");
		bg->setPosition(Vec2(350, 600));
		this->addChild(bg, 999);

		// �����ı���ǩ
		auto label = Label::createWithSystemFont("", "fonts/Alimama_DongFangDaKai_Regular.ttf", 40);
		label->setPosition(Vec2(350, 600));
		label->setColor(Color3B::RED);
		this->addChild(label, 1000);

		// ������Ҫ��ӡ���ַ���
		std::u16string text = u"�������\n����ʹ��Cocos 2dx��������Ϸ\nϣ������Ŀ���\n��ϵ�ң�VRѧԺ214��--����";

		// ���ö�ʱ����ÿ��һ��ʱ�����һ���ַ�
		int index = 0;
		this->schedule([=](float dt) mutable {
			if (index < text.length())
			{
				std::u16string substr = text.substr(index, 1);
				std::string utf8;
				if (cocos2d::StringUtils::UTF16ToUTF8(substr, utf8))
				{
					label->setString(label->getString() + utf8);
					index++;
				}
				else
				{
					CCLOG("Failed to convert character to UTF-8.");
				}
			}
			else
			{
				this->unschedule("typingEffect");  // ȡ����ʱ��
			}
			}, 0.1, "typingEffect");

		// �����رհ�ť
		auto closeButton = ui::Button::create("exit3.png", "exit3.png");
		closeButton->setPosition(Vec2(580, 735));
		closeButton->addClickEventListener([=](Ref* sender) {
			this->unschedule("typingEffect"); // ֹͣ��ʱ��
		this->removeChild(bg);
		this->removeChild(label);
		this->removeChild(closeButton);
			});
		this->addChild(closeButton, 1000);
			});


		/////////////////////////////////////////////////////////////////////////
		//����һ����������
		// ������ť
		auto changecloth_button = ui::Button::create("images/changecloth.png");
		changecloth_button->setPosition(Vec2(660, 1200));
		this->addChild(changecloth_button);

		// �󶨻ص�����
		changecloth_button->addClickEventListener(CC_CALLBACK_0(GameLayer::onchangeButtonClicked, this));


		// ���� Tiled ����
		Tiled* tiled = Tiled::create();
		this->addChild(tiled);

		// �� Tiled ����ָ�븳ֵ�� tiled ����
		this->tiled = tiled;




		//��ʼ����Ϸ����
		gameInit();

		//��Ӽ�����
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
		//listener->onTouchMoved=CC_CALLBACK_2(GameLayer::onTouchMoved,this);
		listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		bRet = true;
	} while (0);
	return bRet;
}

void GameLayer::gameInit() {
	GameLayer::score = 0;
	auto cache = SpriteFrameCache::getInstance();
	//��ʼ��ש��
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			auto tiled = Tiled::create();
			//tiled->level=0;
			tiled->setAnchorPoint(Point::ZERO);
			tiled->setPosition(Point(RC_CONVERT_TO_X(j), RC_CONVERT_TO_Y(i)));
			tiled->setVisible(false);
			this->addChild(tiled, 1);
			tables[i][j] = tiled;

		}
	}

	//��ȡ�����������
	//c++11�������������ʽ
	default_random_engine e(time(NULL));
	//�������趨������������ķ�Χ��������0��3
	uniform_int_distribution<unsigned> u(0, 3);
	int row1 = u(e);
	int col1 = u(e);
	int row2 = u(e);
	int col2 = u(e);
	//���ѭ���Ǳ�֤����ש������겻���ظ�
	do {
		row2 = u(e);
		col2 = u(e);
	} while (row1 == row2 && col1 == col2);

	//��ӵ�һ��ש��
	auto tiled1 = tables[row1][col1];
	int isFour = e() % 10;
	if (isFour == 0) {
		tiled1->setLevel(2);
		tiled1->setVisible(true);
	}
	else {
		tiled1->setLevel(1);
		tiled1->setVisible(true);
	}

	//��ӵڶ���ש��
	auto tiled2 = tables[row2][col2];
	isFour = e() % 10;
	if (isFour == 0) {
		tiled2->setLevel(2);
		tiled2->setVisible(true);
	}
	else {
		tiled2->setLevel(1);
		tiled2->setVisible(true);
	}


	// ��ʼ��ͼƬ����
	imageIndex = 1;

	//���ж���Ч������ͼƬ
	for (int i = 1; i <= 5; i++) { // ������5��ͼƬ
		auto sprite = Sprite::create("vr" + std::to_string(i) + ".jpg");
		sprite->setPosition(Director::getInstance()->getVisibleSize() / 2);
		sprite->setVisible(false);
		sprite->setLocalZOrder(std::numeric_limits<int>::max());
		this->addChild(sprite);
		imagePopupArray.pushBack(sprite);

		// ��ӷŴ󶯻�
		auto scaleTo = ScaleTo::create(0.5f, 1.2f);
		auto reverseScaleTo = ScaleTo::create(0.5f, 1.0f);
		auto seq = Sequence::create(scaleTo, reverseScaleTo, nullptr);
		sprite->runAction(RepeatForever::create(seq));
	}
	// ��ʼ����ͼƬ�ļ�ʱ��
	this->schedule(schedule_selector(GameLayer::popupImage), 180.0f);


}

//�������ͼ
void GameLayer::popupImage(float dt) {
	// ȡ��֮ǰ������ͼƬ�ļ�ʱ��
	this->unschedule(schedule_selector(GameLayer::hidePopupImage));

	// ���ص�ǰ������ͼƬ
	auto prevSprite = imagePopupArray.at(imageIndex - 1);
	prevSprite->setVisible(false);

	// ����ͼƬ����
	imageIndex = (imageIndex % imagePopupArray.size()) + 1;

	// ��ʾ��һ��ͼƬ
	auto nextSprite = imagePopupArray.at(imageIndex - 1);
	nextSprite->setVisible(true);

	// ��ӷŴ󶯻�
	auto scaleTo = ScaleTo::create(0.5f, 1.2f);
	auto reverseScaleTo = ScaleTo::create(0.5f, 1.0f);
	auto seq = Sequence::create(scaleTo, reverseScaleTo, nullptr);
	nextSprite->stopAllActions();
	nextSprite->runAction(seq);

	// 5�������ͼƬ
	this->scheduleOnce(schedule_selector(GameLayer::hidePopupImage), 5.0f);
	// ���Դ���
	CCLOG("popupImage: index=%d, size=%d", imageIndex - 1, imagePopupArray.size());
}
//���ع��ͼ
void GameLayer::hidePopupImage(float dt) {
	if (imagePopupArray.size() == 0) {
		return;
	}
	int index = (imageIndex % imagePopupArray.size()) - 1;
	if (index < 0) {
		index += imagePopupArray.size();
	}
	if (index >= 0 && index < imagePopupArray.size()) {
		imagePopupArray.at(index)->setVisible(false);
	}
	else {
		CCLOG("hidePopupImage: invalid index=%d, size=%d", index, imagePopupArray.size());
	}

}


//�������Ļص�����
//���������ѧ�ʡ�
//touch��ָ��Touch��һ��ָ�룬�������û��Ĵ�����Ϣ������λ�ú�ʱ��ȵ�
//unused_event��һ��ָ��Event�¼������ָ�룬�������������û���õ�����������Ϊunused_event
bool GameLayer::onTouchBegan(Touch* touch, Event* unused_event) {

	//���ص��Ǵ������ڵ�ǰ���ڵ�����ֵ
	this->touchDown = touch->getLocationInView();
	//���Խ���Ļ����ϵ�еĵ�ת��Ϊ��Ϸ��
	this->touchDown = Director::getInstance()->convertToGL(this->touchDown);
	return true;
}

//�ɿ����Ļص�����
//�����¼�����ʱ����
void GameLayer::onTouchEnded(Touch* touch, Event* unused_event) {
	//��Ĭ��û���ƶ�����ȷ���ƶ��������ש��
	bool hasMoved = false;
	Point touchUp = touch->getLocationInView();
	touchUp = Director::getInstance()->convertToGL(touchUp);


	//����ط��ж���ʼ��ͽ�����ļ���Ƿ����50�������ʵ����ȡ��Ч��
	if (touchUp.getDistance(touchDown) > 50) {
		//�ж����»�������
		if (abs(touchUp.x - touchDown.x) > abs(touchUp.y - touchDown.y)) {
			//���һ���
			if (touchUp.x - touchDown.x > 0) {
				//����
				CCLOG("toRight");
				hasMoved = moveToRight();
			}
			else {
				//����
				CCLOG("toLeft");
				hasMoved = moveToLeft();
			}
		}
		else {
			//���»���
			if (touchUp.y - touchDown.y > 0) {
				//����
				CCLOG("toTop");
				hasMoved = moveToTop();
			}
			else {
				//����
				CCLOG("toDown");
				hasMoved = moveToDown();
			}
		}

		if (hasMoved) {
			addTiled();
		}

		if (isOver()) {
			//��ŷ���
			int high = UserDefault::getInstance()->getIntegerForKey("HighScore", 0);
			if (GameLayer::score > high) {
				UserDefault::getInstance()->setIntegerForKey("HighScore", GameLayer::score);
				UserDefault::getInstance()->flush();
			}
			GameLayer::score = 0;
			//�л�����
			Director::getInstance()->replaceScene(TransitionSlideInB::create(1.0f, OverScene::createScene()));
		}

	}
}

//�ĸ�����
//����
bool GameLayer::moveToDown() {
	//���ڻ�û�п�ʼ�ƶ�
	bool hasMoved = false;

	//��ʼ�����б�־λ
	//���ƶ���־��λ�����������ʼ��
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			move_flag[i][j] = 0;
			vec[i][j] = Vec2(0, 0);
		}
	}

	//��������ͬ�ĸ��Ӻ�һ
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			//������ÿһ�λ�õķ���
			auto tiled = tables[row][col];
			//�ҵ���Ϊ�յķ���
			if (tiled->getLevel() != 0) {
				int k = row + 1;
				//����һ����û�еȼ����������ȼ���ͬ�� 
				while (k < 4) {
					auto nextTiled = tables[k][col];
					if (nextTiled->getLevel() != 0) {
						if (tiled->getLevel() == nextTiled->getLevel()) {
							//�ҵ��ȼ������ש��ȼ���ͬ�ľͰ����Ǻϲ�
							tiled->setLevel(nextTiled->getLevel() + 1);
							nextTiled->setLevel(0);
							nextTiled->setVisible(false);

							move_flag[row][col] = 1;//���Ӻϲ���ı�־λ
							vec[row][col] = nextTiled->getPosition();//�ϲ���ǰһ�����������

							//�÷�
							GameLayer::score += Tiled::nums[tiled->getLevel()];
							this->_score->setString(String::createWithFormat("%d", GameLayer::score)->getCString());
							hasMoved = true;
						}
						k = 4;
					}
					k++;
				}
			}
		}
	}


	//�������ĸ�������ո���
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			//����ÿһ�ε�ש��
			auto tiled = tables[row][col];
			//�ҵ��ո���
			if (tiled->getLevel() == 0 || move_flag[row][col] == 1) {
				int k = row;
				while (k < 4) {
					auto nextTiled = tables[k][col];
					if (nextTiled->getLevel() != 0) {
						//����Ϊ�յĸ����Ƶ�����
						if (k == row && move_flag[row][col] == 0)
						{
							continue;
						}
						tiled->setLevel(nextTiled->getLevel());

						int show_level = 0;
						Vec2 show_vec;
						if (move_flag[k][col] == 1)
						{
							show_level = nextTiled->getLevel() - 1;
							show_vec = vec[k][col];
						}
						else
						{
							show_level = nextTiled->getLevel();
							show_vec = nextTiled->getPosition();
						}

						/*Sprite* m_anima = Sprite::create(String::createWithFormat("level%d.png", show_level)->getCString());*/
						Sprite* m_anima = Sprite::create(String::createWithFormat(m_anima_path.c_str(), show_level)->getCString());

						m_anima->setScale(1.2);
						if (nextTiled != tiled)
						{
							nextTiled->setLevel(0);
						}


						nextTiled->setVisible(false);
						m_anima->setPosition(show_vec);
						this->addChild(m_anima);
						MoveBy* move = MoveBy::create(0.1, ccp(tiled->getPositionX() - show_vec.x, tiled->getPositionY() - show_vec.y));

						CallFunc* func = CallFunc::create([=]() {
							tiled->setVisible(true);
						m_anima->setVisible(false);
						m_anima->removeFromParentAndCleanup(true);


						//���Ϸ���ʱ����ͼƬ
						if (tiled->getLevel() > best_level)
						{
							best_level = tiled->getLevel();
							title_imageview->loadTexture(String::createWithFormat("t_%d.png", best_level)->getCString());
							title_sprite->setTexture(String::createWithFormat(m_anima_path.c_str(), best_level)->getCString());

							title_sprite->setScale(0.7);
							title_sprite->runAction(ScaleTo::create(0.1, 1.0f));
						}
							});
						Sequence* actions = Sequence::create(move, func, NULL);
						m_anima->runAction(actions);
						hasMoved = true;
						k = 4;
					}
					k++;
				}
			}
		}
	}

	return hasMoved;
}

//����
bool GameLayer::moveToLeft() {
	bool hasMoved = false;

	//��ʼ�����б�־λ
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			move_flag[i][j] = 0;
			vec[i][j] = Vec2(0, 0);
		}
	}

	//�ϳ�
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			auto tiled = tables[row][col];
			if (tiled->getLevel() != 0) {
				int k = col + 1;
				while (k < 4) {
					auto nextTiled = tables[row][k];
					if (nextTiled->getLevel() != 0) {
						if (tiled->getLevel() == nextTiled->getLevel()) {
							tiled->setLevel(nextTiled->getLevel() + 1);
							nextTiled->setLevel(0);
							nextTiled->setVisible(false);

							move_flag[row][col] = 1;//���Ӻϲ���ı�־λ
							vec[row][col] = nextTiled->getPosition();//�ϲ���ǰһ�����������

							GameLayer::score += Tiled::nums[tiled->getLevel()];
							this->_score->setString(String::createWithFormat("%d", GameLayer::score)->getCString());
							hasMoved = true;
						}
						k = 4;
					}
					k++;
				}
			}
		}
	}

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			auto tiled = tables[row][col];
			if (tiled->getLevel() == 0 || move_flag[row][col] == 1) {
				int k = col;
				while (k < 4) {
					auto nextTiled = tables[row][k];
					if (nextTiled->getLevel() != 0) {
						if (k == col && move_flag[row][col] == 0)
						{
							continue;
						}

						tiled->setLevel(nextTiled->getLevel());

						int show_level = 0;
						Vec2 show_vec;
						if (move_flag[row][k] == 1)
						{
							show_level = nextTiled->getLevel() - 1;
							show_vec = vec[row][k];
						}
						else
						{
							show_level = nextTiled->getLevel();
							show_vec = nextTiled->getPosition();
						}


						//Sprite* m_anima = Sprite::create(String::createWithFormat("level%d.png", show_level)->getCString());
						Sprite* m_anima = Sprite::create(String::createWithFormat(m_anima_path.c_str(), show_level)->getCString());



						m_anima->setScale(1.13);
						if (nextTiled != tiled)
						{
							nextTiled->setLevel(0);
						}
						//tiled->setVisible(true);
						nextTiled->setVisible(false);
						m_anima->setPosition(show_vec);
						this->addChild(m_anima);
						MoveBy* move = MoveBy::create(0.1, ccp((tiled->getPositionX() - show_vec.x), tiled->getPositionY() - show_vec.y));

						CallFunc* func = CallFunc::create([=]() {
							tiled->setVisible(true);
						m_anima->setVisible(false);
						m_anima->removeFromParentAndCleanup(true);

						if (tiled->getLevel() > best_level)
						{
							best_level = tiled->getLevel();
							title_imageview->loadTexture(String::createWithFormat("t_%d.png", best_level)->getCString());
							/*title_sprite->setTexture(String::createWithFormat("level%d.png", best_level)->getCString());*/
							title_sprite->setTexture(String::createWithFormat(m_anima_path.c_str(), best_level)->getCString());
							title_sprite->setScale(0.7);
							title_sprite->runAction(ScaleTo::create(0.1, 1.0f));
						}
							});
						Sequence* actions = Sequence::create(move, func, NULL);
						m_anima->runAction(actions);
						hasMoved = true;
						k = 4;
					}
					k++;
				}
			}
		}
	}

	return hasMoved;
}

//����
bool GameLayer::moveToRight() {
	bool hasMoved = false;

	//��ʼ�����б�־λ
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			move_flag[i][j] = 0;
			vec[i][j] = Vec2(0, 0);
		}
	}

	//�ϳ�
	for (int row = 0; row < 4; row++) {
		for (int col = 3; col >= 0; col--) {
			auto tiled = tables[row][col];
			if (tiled->getLevel() != 0) {
				int k = col - 1;
				while (k >= 0) {
					auto nextTiled = tables[row][k];
					if (nextTiled->getLevel() != 0) {
						if (tiled->getLevel() == nextTiled->getLevel()) {
							tiled->setLevel(nextTiled->getLevel() + 1);
							nextTiled->setLevel(0);
							nextTiled->setVisible(false);

							move_flag[row][col] = 1;//���Ӻϲ���ı�־λ
							vec[row][col] = nextTiled->getPosition();//�ϲ���ǰһ�����������


							GameLayer::score += Tiled::nums[tiled->getLevel()];
							this->_score->setString(String::createWithFormat("%d", GameLayer::score)->getCString());
							hasMoved = true;
						}
						k = -1;
					}
					k--;
				}
			}
		}
	}

	for (int row = 0; row < 4; row++) {
		CCLOG(" row++   row   %d", row);
		for (int col = 3; col >= 0; col--) {
			auto tiled = tables[row][col];
			CCLOG(" col--   col   %d", col);
			if (tiled->getLevel() == 0 || move_flag[row][col] == 1) {
				int k = col;
				CCLOG(" int k=col-1   k   %d", k);
				while (k >= 0) {
					auto nextTiled = tables[row][k];
					if (nextTiled->getLevel() != 0) {
						if (k == col && move_flag[row][col] == 0)
						{
							continue;
						}


						tiled->setLevel(nextTiled->getLevel());

						int show_level = 0;
						Vec2 show_vec;
						if (move_flag[row][k] == 1)
						{
							show_level = nextTiled->getLevel() - 1;
							show_vec = vec[row][k];
						}
						else
						{
							show_level = nextTiled->getLevel();
							show_vec = nextTiled->getPosition();
						}

						//Sprite* m_anima = Sprite::create(String::createWithFormat("level%d.png", show_level)->getCString());
						Sprite* m_anima = Sprite::create(String::createWithFormat(m_anima_path.c_str(), show_level)->getCString());

						m_anima->setScale(1.13);
						if (nextTiled != tiled)
						{
							nextTiled->setLevel(0);
						}
						//tiled->setVisible(true);
						nextTiled->setVisible(false);

						m_anima->setPosition(show_vec);
						this->addChild(m_anima);
						MoveBy* move = MoveBy::create(0.1, ccp((tiled->getPositionX() - show_vec.x), tiled->getPositionY() - show_vec.y));

						CallFunc* func = CallFunc::create([=]() {
							//��������
							tiled->setVisible(true);
						m_anima->setVisible(false);
						m_anima->removeFromParentAndCleanup(true);

						if (tiled->getLevel() > best_level)
						{
							best_level = tiled->getLevel();
							title_imageview->loadTexture(String::createWithFormat("t_%d.png", best_level)->getCString());
							/*title_sprite->setTexture(String::createWithFormat("level%d.png", best_level)->getCString());*/
							title_sprite->setTexture(String::createWithFormat(m_anima_path.c_str(), best_level)->getCString());
							title_sprite->setScale(0.7);
							title_sprite->runAction(ScaleTo::create(0.1, 1.0f));
						}
							});
						Sequence* actions = Sequence::create(move, func, NULL);
						m_anima->runAction(actions);
						hasMoved = true;
						k = -1;
					}
					k--;
				}
			}
		}
	}

	return hasMoved;
}

//����
bool GameLayer::moveToTop() {
	bool hasMoved = false;

	//��ʼ�����б�־λ
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			move_flag[i][j] = 0;
			vec[i][j] = Vec2(0, 0);
		}
	}

	//��������ͬ�ĸ��Ӻ�һ
	for (int col = 0; col < 4; col++) {
		for (int row = 3; row >= 0; row--) {
			auto tiled = tables[row][col];
			if (tiled->getLevel() != 0) {
				int k = row - 1;
				while (k >= 0) {
					auto nextTiled = tables[k][col];
					if (nextTiled->getLevel() != 0) {
						if (tiled->getLevel() == nextTiled->getLevel()) {
							tiled->setLevel(nextTiled->getLevel() + 1);
							nextTiled->setLevel(0);
							nextTiled->setVisible(false);

							move_flag[row][col] = 1;//���Ӻϲ���ı�־λ
							vec[row][col] = nextTiled->getPosition();//�ϲ���ǰһ�����������

							GameLayer::score += Tiled::nums[tiled->getLevel()];
							this->_score->setString(String::createWithFormat("%d", GameLayer::score)->getCString());
							hasMoved = true;
						}
						k = -1;
					}
					k--;
				}
			}
		}
	}


	//�������ĸ�������ո���
	for (int col = 0; col < 4; col++) {
		for (int row = 3; row >= 0; row--) {
			auto tiled = tables[row][col];
			if (tiled->getLevel() == 0 || move_flag[row][col] == 1) {
				int k = row;
				while (k >= 0) {
					auto nextTiled = tables[k][col];
					if (nextTiled->getLevel() != 0) {
						if (k == row && move_flag[row][col] == 0)
						{
							continue;
						}

						tiled->setLevel(nextTiled->getLevel());

						int show_level = 0;
						Vec2 show_vec;
						if (move_flag[k][col] == 1)
						{
							show_level = nextTiled->getLevel() - 1;
							show_vec = vec[k][col];
						}
						else
						{
							show_level = nextTiled->getLevel();
							show_vec = nextTiled->getPosition();
						}

						//Sprite* m_anima = Sprite::create(String::createWithFormat("level%d.png", show_level)->getCString());
						Sprite* m_anima = Sprite::create(String::createWithFormat(m_anima_path.c_str(), show_level)->getCString());

						m_anima->setScale(1.13);
						if (nextTiled != tiled)
						{
							nextTiled->setLevel(0);
						}

						//tiled->setVisible(true);
						nextTiled->setVisible(false);

						m_anima->setPosition(show_vec);
						this->addChild(m_anima);
						MoveBy* move = MoveBy::create(0.1, ccp((tiled->getPositionX() - show_vec.x), tiled->getPositionY() - show_vec.y));

						CallFunc* func = CallFunc::create([=]() {
							tiled->setVisible(true);
						m_anima->setVisible(false);
						m_anima->removeFromParentAndCleanup(true);

						if (tiled->getLevel() > best_level)
						{
							best_level = tiled->getLevel();
							title_imageview->loadTexture(String::createWithFormat("t_%d.png", best_level)->getCString());
							/*title_sprite->setTexture(String::createWithFormat("level%d.png", best_level)->getCString());*/
							title_sprite->setTexture(String::createWithFormat(m_anima_path.c_str(), best_level)->getCString());
							title_sprite->setScale(0.7);
							title_sprite->runAction(ScaleTo::create(0.1, 1.0f));
						}
							});
						Sequence* actions = Sequence::create(move, func, NULL);
						m_anima->runAction(actions);
						hasMoved = true;
						k = -1;
					}
					k--;
				}
			}
		}
	}
	return hasMoved;
}


//�����û��ש��ĸ�������ש��
void GameLayer::addTiled() {
	auto cache = SpriteFrameCache::getInstance();
	//��ȡ�����������
	default_random_engine e(time(NULL));
	uniform_int_distribution<unsigned> u(0, 3);
	int row = 0;
	int col = 0;
	do {
		row = u(e);
		col = u(e);
	} while (tables[row][col]->getLevel() != 0);
	//���ש��
	auto tiled = tables[row][col];
	int isFour = e() % 10;

	//��Ҫ���ɵķ�����СΪһ��
	auto func = [this, tiled]() {
		tiled->setVisible(true);
		tiled->setScale(0.5, 0.5);
	};
	//���������ifFour�����Ϊ0��������ש��ȼ�Ϊ2������Ϊ1
	if (isFour == 0) {
		tiled->setLevel(2);
	}
	else {
		tiled->setLevel(1);
	}
	//���ö���Ч���������ַ������С�Ľ��䵽�����ġ�
	//����DelayTime�ӳ�0.1�룬����CallFunc::create����func���ڻص������н�ש������Ϊ�ɼ�������С��ԭ����һ�롣
	//��ʹ��Sequence���ص�������ScaleTo�������ӣ�����ʵ������Ч��
	auto acts = Sequence::create(DelayTime::create(0.1f), CallFunc::create(func), ScaleTo::create(0.1f, 1.2f), NULL);
	tiled->runAction(acts);
}

//�����˼·Ҳ�ܼ򵥣�����ÿһ���ط����ж��������������Ƿ�����ͬ�ȼ���ש��
bool GameLayer::isOver() {
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			//�����ж��Ƿ���ڿո���
			if (tables[row][col]->getLevel() == 0) {
				//�пո��ӿ϶�����OVER
				return false;
			}
			//�ж���Χ����,���������ȵ�������OVER
			//��
			int c = col;
			int r = row + 1;
			if (r != -1 && r != 4) {
				if (tables[row][col]->getLevel() == tables[r][c]->getLevel()) {
					return false;
				}
			}
			//��
			c = col - 1;
			r = row;
			if (c != -1 && c != 4) {
				if (tables[row][col]->getLevel() == tables[r][c]->getLevel()) {
					return false;
				}
			}
			//��
			c = col + 1;
			r = row;
			if (c != -1 && c != 4) {
				if (tables[row][col]->getLevel() == tables[r][c]->getLevel()) {
					return false;
				}
			}
			//��
			c = col;
			r = row - 1;
			if (r != -1 && r != 4) {
				if (tables[row][col]->getLevel() == tables[r][c]->getLevel()) {
					return false;
				}
			}
		}
	}
	return true;
}

//������
void GameLayer::onButtonClicked(Ref* sender)
{
	// ����Ϣ���Ƶ����а���
	std::string link = "http://vr.jxufe.edu.cn/";
	copyLinkToClipboard(link);

	// ��ʾ�Զ�����ʾ��
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto centerPos = Vec2(visibleSize.width / 2, visibleSize.height / 2);
	auto dialog = LayerColor::create(Color4B(0, 0, 0, 128));
	dialog->setTag(100);
	dialog->setPosition(Vec2::ZERO);
	this->addChild(dialog, 100);

	auto label = Label::createWithSystemFont("Copied", "Arial", 24);
	label->setColor(Color3B::WHITE);
	label->setPosition(centerPos);
	dialog->addChild(label);

	auto scaleAction = ScaleTo::create(0.1f, 1.2f);
	auto reverseAction = ScaleTo::create(0.1f, 1.0f);
	auto seq = Sequence::create(scaleAction, reverseAction, nullptr);
	//   dialog->runAction(seq);
	   //// һ����Զ����ص���
	   //this->scheduleOnce(CC_SCHEDULE_SELECTOR(GameLayer::hideDialog), 1.0f);
	dialog->runAction(Sequence::create(seq, CallFunc::create([=]() {
		// ������ɺ��ָ����ҳ
		Application::getInstance()->openURL("http://vr.jxufe.edu.cn/");
		}), nullptr));
	// һ����Զ����ص���
	this->scheduleOnce(CC_SCHEDULE_SELECTOR(GameLayer::hideDialog), 1.0f);
}
void GameLayer::hideDialog(float dt)
{
	auto dialog = this->getChildByTag(100);
	if (dialog)
	{
		dialog->removeFromParentAndCleanup(true);
	}
}

void GameLayer::copyLinkToClipboard(const std::string& link)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// TODO: ʵ�� Android ƽ̨�ĸ��Ƶ������幦��
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	// TODO: ʵ�� iOS ƽ̨�ĸ��Ƶ������幦��
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (OpenClipboard(nullptr))
	{
		EmptyClipboard();
		HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, link.size() + 1);
		if (hglbCopy)
		{
			LPSTR lptstrCopy = static_cast<LPSTR>(GlobalLock(hglbCopy));
			if (lptstrCopy)
			{
				strcpy_s(lptstrCopy, link.size() + 1, link.c_str());
				GlobalUnlock(hglbCopy);
				SetClipboardData(CF_TEXT, hglbCopy);
			}
		}
		CloseClipboard();
		while (GetClipboardOwner() != nullptr)
		{
			Sleep(10);
		}
	}
#endif
}

// ��ť�ص�����
void GameLayer::onchangeButtonClicked() {
	auto imgBg = (ImageView*)ui::Helper::seekWidgetByName(_ui, "Image_3");
	auto imgBg1 = (ImageView*)ui::Helper::seekWidgetByName(_ui, "Image_7");
	auto imgBg2 = (ImageView*)ui::Helper::seekWidgetByName(_ui, "score_bg");
	auto imgBg3 = (ImageView*)ui::Helper::seekWidgetByName(_ui, "Image_9");
	auto imgBg4 = (ImageView*)imgBg2->getChildByName("Image_7");
	if (this->m_isImageSet1) {
		this->m_isImageSet1 = false;
		m_anima_path = "images/level%d.png";
		imgBg->loadTexture("theme_spring_bg.png");
		imgBg1->loadTexture("game_area_bg2.png");
		imgBg2->loadTexture("bg3.png");
		//imgBg2->setContentSize(Size(650,232));
		imgBg3->loadTexture("title2.png");
		imgBg4->loadTexture("best2.png");
		CCLOG("m_anima_path turn to images/level.png", );
	}
	else {
		this->m_isImageSet1 = true;
		m_anima_path = "level%d.png";
		imgBg->loadTexture("theme_default_bg.png");
		imgBg1->loadTexture("game_area_bg.png");
		imgBg2->loadTexture("bg.png");
		imgBg3->loadTexture("title.png");
		imgBg4->loadTexture("best.png");
		CCLOG("m_anima_path turn to level.png");
	}
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			//������ÿһ�λ�õķ���
			auto tiled = tables[row][col];
			tiled->m_anima_path = m_anima_path;
			tiled->setLevel(tiled->getLevel());
		}
	}
	auto children = this->getChildren();
	for (const auto& child : children) {
		auto sprite = dynamic_cast<Sprite*>(child);
		if (sprite) {
			title_sprite->setTexture(String::createWithFormat(m_anima_path.c_str(), best_level)->getCString());
		}
	}
}

//�߼��ı���������
void GameLayer::cloudMove(float dt) {

	//�ƶ������С��10�ʹ����µ��ƶ�
	if (num_cloud < 10 && number % 50 == 0)
	{
		num_cloud++;

		int num = CCRANDOM_0_1() * 3 + 1;
		auto m_sprite = Sprite::create(String::createWithFormat("cloud_%d.png", num)->getCString());
		int height = CCRANDOM_0_1() * 480 + 800;
		int width = CCRANDOM_0_1() * 260 - 360;
		m_sprite->setPosition(width, height);
		allCloud.pushBack(m_sprite);
		auto _bg = _ui->getChildByName("Image_3");
		_bg->addChild(m_sprite);
	}

	//���ƶ�sprite��ӵ����е��ƶ�������У�����ӵ���Ϸ�ı���ͼ����
	for (int i = 0; i < allCloud.size(); i++)
	{
		Sprite* mSprite = allCloud.at(i);
		mSprite->setPositionX(mSprite->getPositionX() + 1);

		if (mSprite->getPositionX() > 600)
		{
			int height = CCRANDOM_0_1() * 480 + 800;
			int width = CCRANDOM_0_1() * 260 - 400;
			mSprite->setPosition(width, height);
			//����ƶ䳬������Ļ�Ҳ࣬��������������ƶ��λ�ã��ص���Ļ���
		}
	}
	number++;
}