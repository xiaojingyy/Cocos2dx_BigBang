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

using namespace CocosDenshion;//包含了音频
using namespace cocos2d;

#define RC_CONVERT_TO_X(rc) (rc*153+130)
#define RC_CONVERT_TO_Y(rc) (rc*160+345)

int  GameLayer::score = 0;


//继承 Layer 类并重写其 init() 函数。
//使用 CC_BREAK_IF 宏判断 Layer 的初始化是否成功，如果失败则直接返回 false。
//使用 schedule() 函数定时调用 cloudMove() 函数，控制云层移动。
//加载图集资源文件 sucai.plist 和 sucai.png，并将其添加到 SpriteFrameCache 中，方便后续使用。
//使用 cocostudio::GUIReader::getInstance() 从 UI 配置文件 go_main_scene_1.json 中读取 UI 控件，如退出和重新开始按钮，分数显示等，并将其添加到当前场景层中。
//添加标题背景、退出和重新开始按钮、分数显示等游戏元素，并设置其位置、大小、响应函数等。
//使用 UserDefault::getInstance() 获取存储在本地的最高分，显示在游戏界面中。
//初始化游戏界面，并添加触摸监听器来响应玩家的操作。
//返回 true，表示初始化成功。
bool GameLayer::init() {


	bool bRet = false;
	do {
		CC_BREAK_IF(!Layer::init());//没有初始化成功就跳出函数

		//schedule函数注册一个定时器回调函数，控制云朵运动。参数
		schedule(schedule_selector(GameLayer::cloudMove), 0.0f);
		num_cloud = 0;
		number = 0;
		//加载纹理
		auto cache = SpriteFrameCache::getInstance();     //共享的帧序列
		auto size = Director::getInstance()->getVisibleSize();
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sucai.plist", "sucai.png");



		//加载背景图
		_ui = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("go_main_scene_1.json");
		this->addChild(_ui);//一定要记得添加

		/////////////////////////////////////////////////////////////////////////////////////////////////////
			//添加标题背景
		auto headBg = Sprite::create("splash_logo1.png");
		headBg->setAnchorPoint(Vec2(0, 1));
		headBg->setPosition(Vec2(0, 1260));
		this->addChild(headBg, 1);


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//创建退出和重新开始按钮         
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
		// 加载背景音乐
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic("background.mp3");

		// 播放背景音乐并循环播放
		SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);

		// 创建音频控件
		auto audioButton = ui::Button::create("music1.png", "music1.png");
		audioButton->setPosition(Vec2(540, 1200));
		this->addChild(audioButton);
		this->setZOrder(1000);
		// 设置音频控件的回调函数
		audioButton->addClickEventListener([=](Ref* sender) {
			// 暂停背景音乐
			if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
				SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
				audioButton->loadTextures("unmusic1.png", "unmusic1.png", "");
			}
			else {
				// 播放背景音乐
				SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
				audioButton->loadTextures("music1.png", "music1.png", "");
			}
			});

		/////////////////////////////////////////////////////////////////////////////////////////////////////
				//添加分数显示
		lScore = Label::create("", "Arial", 40);
		lScore->setPosition(Point(size.width / 2, 1000));
		this->addChild(lScore);
		_best_score = (TextAtlas*)ui::Helper::seekWidgetByName(_ui, "best_score");
		_score = (TextAtlas*)ui::Helper::seekWidgetByName(_ui, "score");
		//添加最高分显示
		int high = UserDefault::getInstance()->getIntegerForKey("HighScore", 0);
		_best_score->setString(String::createWithFormat("%d", high)->getCString());
		this->setZOrder(1000);
		//时代的图片
		title_sprite = Sprite::create("level1.png");
		title_sprite->setPosition(126, 1030);
		this->addChild(title_sprite);

		title_imageview = (ImageView*)ui::Helper::seekWidgetByName(_ui, "Image_8");
		title_imageview->loadTexture("t_1.png");//“时代”文字
		best_level = 1;//初始化为1


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//截图功能
		// 创建截图按钮
		auto screenshotButton = ui::Button::create("camera_nor.png", "camera_nor.png", "");
		screenshotButton->setPosition(Vec2(365, 100));
		this->addChild(screenshotButton);

		screenshotButton->loadTexturePressed("camera_click.png");

		Size visibleSize = Director::getInstance()->getVisibleSize();
		// 添加截图按钮的点击事件
		screenshotButton->addClickEventListener([=](Ref* sender) {
			// 创建 RenderTexture 对象
			auto renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888);
		renderTexture->retain();
		// 开始捕捉场景
		renderTexture->begin();
		// 渲染场景
		this->visit();
		Director::getInstance()->getRenderer()->render();
		CCLOG("Capture the screenshot of the game scene");
		// 结束捕捉场景
		renderTexture->end();

		//这是使用计数器记录图片信息的
		//// 读取计数器变量
		//int count = UserDefault::getInstance()->getIntegerForKey("screenshot_count", 1);


		//计数器变量保存在本地的xml文件中，文件路径如下,从中可以修改历史最高分和这个截图的计数器
		//C:\Users\14002\AppData\Local\MyCppGame

		auto image = renderTexture->newImage();

		//使用计数器计数
		//image->saveToFile("E:/cocos2dx/helloworld/screenshot/screenshot.png" + std::to_string(count) + ".png");

		// 获取当前时间
		time_t now = time(nullptr);
		tm* time = localtime(&now);

		// 将当前时间转换为字符串格式
		char timeStr[32];
		strftime(timeStr, sizeof(timeStr), "%Y%m%d%H%M%S", time);

		// 设置截图文件名为当前时间的字符串加上“.png”后缀
		//这里的保存路径应该设置为和程序能够一直在一起的文件，放在Resources中，在里面创建文件夹存放截图，要不然换了电脑之后无法保存

		std::string fileName = "screenshot/" + std::string(timeStr) + ".png";

		image->saveToFile(fileName);


		//实现类似于手机一样的一闪的效果
		// 创建遮罩层
		auto mask = LayerColor::create(Color4B(255, 255, 255, 255), visibleSize.width, visibleSize.height);
		mask->setPosition(Vec2::ZERO);
		this->addChild(mask);
		// 显示遮罩层，并在短暂时间后隐藏它
		mask->runAction(Sequence::create(
			FadeOut::create(0.5),
			CallFunc::create([=]() {
				mask->removeFromParentAndCleanup(true);
				}),
			nullptr
					));
		//控制台输出截图信息
		CCLOG("Screenshot saved");
		auto node = cocos2d::Node::create();
		node->setPosition(Vec2(100, 100));
		this->addChild(node);



		//计数器的代码
		//// 更新计数器变量,从本地XML文件读取
		//count++;
		//UserDefault::getInstance()->setIntegerForKey("screenshot_count", count);
		//UserDefault::getInstance()->flush();
		//这是在之前使用计数命名截图的时候的代码，目前使用时间戳命名截图

		//释放图像
		renderTexture->release();
		image->release();
			});


		/////////////////////////////////////////////////
		//新增分享功能，设置按钮
		auto sharebutton = ui::Button::create("share.png", "share.png");
		sharebutton->setPosition(Vec2(600, 110));
		sharebutton->addClickEventListener(CC_CALLBACK_1(GameLayer::onButtonClicked, this));
		this->addChild(sharebutton);


		/////////////////////////////////////////////////
		// 创建about按钮
		auto button = ui::Button::create("about_nor.png", "about_click.png");
		button->setPosition(Vec2(100, 100));
		this->addChild(button);

		// 绑定点击事件
		button->addClickEventListener([=](Ref* sender) {
			// 创建背景图片
			auto bg = Sprite::create("dialog_bg.png");
		bg->setPosition(Vec2(350, 600));
		this->addChild(bg, 999);

		// 创建文本标签
		auto label = Label::createWithSystemFont("", "fonts/Alimama_DongFangDaKai_Regular.ttf", 40);
		label->setPosition(Vec2(350, 600));
		label->setColor(Color3B::RED);
		this->addChild(label, 1000);

		// 定义需要打印的字符串
		std::u16string text = u"哈喽朋友\n这是使用Cocos 2dx制作的游戏\n希望你玩的开心\n联系我：VR学院214班--喻勇";

		// 设置定时器，每隔一定时间添加一个字符
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
				this->unschedule("typingEffect");  // 取消定时器
			}
			}, 0.1, "typingEffect");

		// 创建关闭按钮
		auto closeButton = ui::Button::create("exit3.png", "exit3.png");
		closeButton->setPosition(Vec2(580, 735));
		closeButton->addClickEventListener([=](Ref* sender) {
			this->unschedule("typingEffect"); // 停止定时器
		this->removeChild(bg);
		this->removeChild(label);
		this->removeChild(closeButton);
			});
		this->addChild(closeButton, 1000);
			});


		/////////////////////////////////////////////////////////////////////////
		//设置一键换肤功能
		// 创建按钮
		auto changecloth_button = ui::Button::create("images/changecloth.png");
		changecloth_button->setPosition(Vec2(660, 1200));
		this->addChild(changecloth_button);

		// 绑定回调函数
		changecloth_button->addClickEventListener(CC_CALLBACK_0(GameLayer::onchangeButtonClicked, this));


		// 创建 Tiled 对象
		Tiled* tiled = Tiled::create();
		this->addChild(tiled);

		// 将 Tiled 对象指针赋值给 tiled 变量
		this->tiled = tiled;




		//初始化游戏界面
		gameInit();

		//添加监听器
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
	//初始化砖块
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

	//获取两个随机坐标
	//c++11的随机数产生方式
	default_random_engine e(time(NULL));
	//这里是设定产生的随机数的范围，这里是0到3
	uniform_int_distribution<unsigned> u(0, 3);
	int row1 = u(e);
	int col1 = u(e);
	int row2 = u(e);
	int col2 = u(e);
	//这个循环是保证两个砖块的坐标不会重复
	do {
		row2 = u(e);
		col2 = u(e);
	} while (row1 == row2 && col1 == col2);

	//添加第一个砖块
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

	//添加第二个砖块
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


	// 初始化图片索引
	imageIndex = 1;

	//具有动画效果弹出图片
	for (int i = 1; i <= 5; i++) { // 假设有5张图片
		auto sprite = Sprite::create("vr" + std::to_string(i) + ".jpg");
		sprite->setPosition(Director::getInstance()->getVisibleSize() / 2);
		sprite->setVisible(false);
		sprite->setLocalZOrder(std::numeric_limits<int>::max());
		this->addChild(sprite);
		imagePopupArray.pushBack(sprite);

		// 添加放大动画
		auto scaleTo = ScaleTo::create(0.5f, 1.2f);
		auto reverseScaleTo = ScaleTo::create(0.5f, 1.0f);
		auto seq = Sequence::create(scaleTo, reverseScaleTo, nullptr);
		sprite->runAction(RepeatForever::create(seq));
	}
	// 开始弹出图片的计时器
	this->schedule(schedule_selector(GameLayer::popupImage), 180.0f);


}

//弹出广告图
void GameLayer::popupImage(float dt) {
	// 取消之前的隐藏图片的计时器
	this->unschedule(schedule_selector(GameLayer::hidePopupImage));

	// 隐藏当前弹出的图片
	auto prevSprite = imagePopupArray.at(imageIndex - 1);
	prevSprite->setVisible(false);

	// 更新图片索引
	imageIndex = (imageIndex % imagePopupArray.size()) + 1;

	// 显示下一张图片
	auto nextSprite = imagePopupArray.at(imageIndex - 1);
	nextSprite->setVisible(true);

	// 添加放大动画
	auto scaleTo = ScaleTo::create(0.5f, 1.2f);
	auto reverseScaleTo = ScaleTo::create(0.5f, 1.0f);
	auto seq = Sequence::create(scaleTo, reverseScaleTo, nullptr);
	nextSprite->stopAllActions();
	nextSprite->runAction(seq);

	// 5秒后隐藏图片
	this->scheduleOnce(schedule_selector(GameLayer::hidePopupImage), 5.0f);
	// 调试代码
	CCLOG("popupImage: index=%d, size=%d", imageIndex - 1, imagePopupArray.size());
}
//隐藏广告图
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


//按下鼠标的回调函数
//这个函数有学问。
//touch是指向Touch的一个指针，包含了用户的触摸信息，比如位置和时间等等
//unused_event是一个指向Event事件对象的指针，但是这个函数中没有用到，所以命名为unused_event
bool GameLayer::onTouchBegan(Touch* touch, Event* unused_event) {

	//返回的是触摸点在当前窗口的坐标值
	this->touchDown = touch->getLocationInView();
	//可以将屏幕坐标系中的点转换为游戏场
	this->touchDown = Director::getInstance()->convertToGL(this->touchDown);
	return true;
}

//松开鼠标的回调函数
//触摸事件结束时触发
void GameLayer::onTouchEnded(Touch* touch, Event* unused_event) {
	//先默认没有移动，等确定移动了再添加砖块
	bool hasMoved = false;
	Point touchUp = touch->getLocationInView();
	touchUp = Director::getInstance()->convertToGL(touchUp);


	//这个地方判断起始点和结束点的间距是否大于50，变相的实现了取消效果
	if (touchUp.getDistance(touchDown) > 50) {
		//判断上下还是左右
		if (abs(touchUp.x - touchDown.x) > abs(touchUp.y - touchDown.y)) {
			//左右滑动
			if (touchUp.x - touchDown.x > 0) {
				//向右
				CCLOG("toRight");
				hasMoved = moveToRight();
			}
			else {
				//向左
				CCLOG("toLeft");
				hasMoved = moveToLeft();
			}
		}
		else {
			//上下滑动
			if (touchUp.y - touchDown.y > 0) {
				//向上
				CCLOG("toTop");
				hasMoved = moveToTop();
			}
			else {
				//向下
				CCLOG("toDown");
				hasMoved = moveToDown();
			}
		}

		if (hasMoved) {
			addTiled();
		}

		if (isOver()) {
			//存放分数
			int high = UserDefault::getInstance()->getIntegerForKey("HighScore", 0);
			if (GameLayer::score > high) {
				UserDefault::getInstance()->setIntegerForKey("HighScore", GameLayer::score);
				UserDefault::getInstance()->flush();
			}
			GameLayer::score = 0;
			//切换画面
			Director::getInstance()->replaceScene(TransitionSlideInB::create(1.0f, OverScene::createScene()));
		}

	}
}

//四个方向
//向下
bool GameLayer::moveToDown() {
	//现在还没有开始移动
	bool hasMoved = false;

	//初始化所有标志位
	//将移动标志和位置向量数组初始化
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			move_flag[i][j] = 0;
			vec[i][j] = Vec2(0, 0);
		}
	}

	//将数字相同的格子合一
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			//遍历的每一次获得的方块
			auto tiled = tables[row][col];
			//找到不为空的方块
			if (tiled->getLevel() != 0) {
				int k = row + 1;
				//看这一列有没有等级和这个方块等级相同的 
				while (k < 4) {
					auto nextTiled = tables[k][col];
					if (nextTiled->getLevel() != 0) {
						if (tiled->getLevel() == nextTiled->getLevel()) {
							//找到等级和这个砖块等级相同的就把他们合并
							tiled->setLevel(nextTiled->getLevel() + 1);
							nextTiled->setLevel(0);
							nextTiled->setVisible(false);

							move_flag[row][col] = 1;//增加合并后的标志位
							vec[row][col] = nextTiled->getPosition();//合并的前一个精灵的坐标

							//得分
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


	//将有数的格子填入空格子
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			//遍历每一次的砖块
			auto tiled = tables[row][col];
			//找到空格子
			if (tiled->getLevel() == 0 || move_flag[row][col] == 1) {
				int k = row;
				while (k < 4) {
					auto nextTiled = tables[k][col];
					if (nextTiled->getLevel() != 0) {
						//将不为空的格子移到这里
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


						//左上方的时代的图片
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

//向左
bool GameLayer::moveToLeft() {
	bool hasMoved = false;

	//初始化所有标志位
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			move_flag[i][j] = 0;
			vec[i][j] = Vec2(0, 0);
		}
	}

	//合成
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

							move_flag[row][col] = 1;//增加合并后的标志位
							vec[row][col] = nextTiled->getPosition();//合并的前一个精灵的坐标

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

//向右
bool GameLayer::moveToRight() {
	bool hasMoved = false;

	//初始化所有标志位
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			move_flag[i][j] = 0;
			vec[i][j] = Vec2(0, 0);
		}
	}

	//合成
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

							move_flag[row][col] = 1;//增加合并后的标志位
							vec[row][col] = nextTiled->getPosition();//合并的前一个精灵的坐标


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
							//函数内容
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

//向上
bool GameLayer::moveToTop() {
	bool hasMoved = false;

	//初始化所有标志位
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			move_flag[i][j] = 0;
			vec[i][j] = Vec2(0, 0);
		}
	}

	//将数字相同的格子合一
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

							move_flag[row][col] = 1;//增加合并后的标志位
							vec[row][col] = nextTiled->getPosition();//合并的前一个精灵的坐标

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


	//将有数的格子填入空格子
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


//随机在没有砖块的格子生成砖块
void GameLayer::addTiled() {
	auto cache = SpriteFrameCache::getInstance();
	//获取两个随机坐标
	default_random_engine e(time(NULL));
	uniform_int_distribution<unsigned> u(0, 3);
	int row = 0;
	int col = 0;
	do {
		row = u(e);
		col = u(e);
	} while (tables[row][col]->getLevel() != 0);
	//添加砖块
	auto tiled = tables[row][col];
	int isFour = e() % 10;

	//将要生成的方块缩小为一半
	auto func = [this, tiled]() {
		tiled->setVisible(true);
		tiled->setScale(0.5, 0.5);
	};
	//设置随机数ifFour，如果为0，就设置砖块等级为2，否则为1
	if (isFour == 0) {
		tiled->setLevel(2);
	}
	else {
		tiled->setLevel(1);
	}
	//设置动画效果，将数字方块从缩小的渐变到正常的。
	//先用DelayTime延迟0.1秒，再用CallFunc::create调用func，在回调函数中将砖块设置为可见，并缩小到原来的一半。
	//再使用Sequence将回调函数和ScaleTo动作连接，可以实现缩放效果
	auto acts = Sequence::create(DelayTime::create(0.1f), CallFunc::create(func), ScaleTo::create(0.1f, 1.2f), NULL);
	tiled->runAction(acts);
}

//这里的思路也很简单，遍历每一个地方，判断他的上下左右是否有相同等级的砖块
bool GameLayer::isOver() {
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			//遍历判断是否存在空格子
			if (tables[row][col]->getLevel() == 0) {
				//有空格子肯定不会OVER
				return false;
			}
			//判断周围格子,如果存在相等的数字则不OVER
			//上
			int c = col;
			int r = row + 1;
			if (r != -1 && r != 4) {
				if (tables[row][col]->getLevel() == tables[r][c]->getLevel()) {
					return false;
				}
			}
			//左
			c = col - 1;
			r = row;
			if (c != -1 && c != 4) {
				if (tables[row][col]->getLevel() == tables[r][c]->getLevel()) {
					return false;
				}
			}
			//右
			c = col + 1;
			r = row;
			if (c != -1 && c != 4) {
				if (tables[row][col]->getLevel() == tables[r][c]->getLevel()) {
					return false;
				}
			}
			//下
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

//分享功能
void GameLayer::onButtonClicked(Ref* sender)
{
	// 将信息复制到剪切板中
	std::string link = "http://vr.jxufe.edu.cn/";
	copyLinkToClipboard(link);

	// 显示自定义提示框
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
	   //// 一秒后自动隐藏弹框
	   //this->scheduleOnce(CC_SCHEDULE_SELECTOR(GameLayer::hideDialog), 1.0f);
	dialog->runAction(Sequence::create(seq, CallFunc::create([=]() {
		// 动画完成后打开指定网页
		Application::getInstance()->openURL("http://vr.jxufe.edu.cn/");
		}), nullptr));
	// 一秒后自动隐藏弹框
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
	// TODO: 实现 Android 平台的复制到剪贴板功能
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	// TODO: 实现 iOS 平台的复制到剪贴板功能
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

// 按钮回调函数
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
			//遍历的每一次获得的方块
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

//高级的背景！！！
void GameLayer::cloudMove(float dt) {

	//云朵的数量小于10就创建新的云朵
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

	//将云朵sprite添加到所有的云朵的向量中，再添加到游戏的背景图像中
	for (int i = 0; i < allCloud.size(); i++)
	{
		Sprite* mSprite = allCloud.at(i);
		mSprite->setPositionX(mSprite->getPositionX() + 1);

		if (mSprite->getPositionX() > 600)
		{
			int height = CCRANDOM_0_1() * 480 + 800;
			int width = CCRANDOM_0_1() * 260 - 400;
			mSprite->setPosition(width, height);
			//如果云朵超过了屏幕右侧，则重新随机生成云朵的位置，回到屏幕左侧
		}
	}
	number++;
}