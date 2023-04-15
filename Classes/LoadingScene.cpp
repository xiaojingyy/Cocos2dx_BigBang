#include "LoadingScene.h"
#include"GameLayer.h"

bool LoadingScene::init(){
	if(Scene::init()){
		//添加背景
		auto size=Director::getInstance()->getVisibleSize();
		//获取可视界面
		Size visibleSize = Director::getInstance()->getVisibleSize();
		auto _ui = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("splash_1.json"); 
		//使用Cocos Studio的GUIReader类从一个JSON文件中加载UI界面，并将结果存储在一个名为"_ui"的指针变量中。
		_ui->setTouchEnabled(false);
		this->addChild(_ui);

		//设置一个按钮，从JSON文件中拿出来
		auto set_btn = (ui::Button*)ui::Helper::seekWidgetByName(_ui, "menu_btn");
		set_btn->setTouchEnabled(true);
		set_btn->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::ENDED)
			{
				// 创建背景图片
				auto bg = Sprite::create("dialog_bg.png");
				bg->setPosition(Vec2(350, 800));
				this->addChild(bg, 1, "bg");

				// 创建文本标签
				auto label = Label::createWithSystemFont("", "simsun.ttc", 40);
				label->setPosition(Vec2(350, 800));
				label->setColor(Color3B::YELLOW);
				this->addChild(label, 2, "textLabel");

				// 定义需要打印的字符串
				std::u16string text = u"你好呀朋友！\n这是一个2048类游戏\n通过滑动\n将相同生物合成进化\n期待你的表现噢~~~";

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
							//翻译了一下哈哈哈挺高级
							CCLOG("Failed to convert character to UTF-8.");
						}
					}
					else
					{
						this->unschedule("typingEffect");  // 取消定时器
					}
					}, 0.1, "typingEffect");

				// 创建关闭按钮，直接用了官方的按钮
				auto closeBtn = ui::Button::create("exit3.png", "exit3.png");
				closeBtn->setPosition(Vec2(600, 935));
				closeBtn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED)
					{
						this->removeChildByName("bg");
						this->removeChildByName("textLabel");
						this->removeChildByName("closeBtn");
						this->unschedule("typingEffect");
					}
					});
				this->addChild(closeBtn, 2, "closeBtn");//将按钮的Z轴等级提到2，显示在前面
			}
			});

		//开始按钮
		auto start_btn = (ui::Button*)ui::Helper::seekWidgetByName(_ui,"start_stn");
		start_btn->setTouchEnabled(true);
		start_btn->addTouchEventListener([this](Ref* send, Widget::TouchEventType type){
			CCLOG("111111111111");
			if(type == Widget::TouchEventType::ENDED)
			{
				auto gameLayer=GameScene::create();
				Director::getInstance()->replaceScene(TransitionFade::create(1.0f,gameLayer));
				
			}
		});
		return true;
	}else{
		return false;
			
	}
	return true;
}

