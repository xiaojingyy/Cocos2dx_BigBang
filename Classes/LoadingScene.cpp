#include "LoadingScene.h"
#include"GameLayer.h"

bool LoadingScene::init(){
	if(Scene::init()){
		//��ӱ���
		auto size=Director::getInstance()->getVisibleSize();
		//��ȡ���ӽ���
		Size visibleSize = Director::getInstance()->getVisibleSize();
		auto _ui = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("splash_1.json"); 
		//ʹ��Cocos Studio��GUIReader���һ��JSON�ļ��м���UI���棬��������洢��һ����Ϊ"_ui"��ָ������С�
		_ui->setTouchEnabled(false);
		this->addChild(_ui);

		//����һ����ť����JSON�ļ����ó���
		auto set_btn = (ui::Button*)ui::Helper::seekWidgetByName(_ui, "menu_btn");
		set_btn->setTouchEnabled(true);
		set_btn->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::ENDED)
			{
				// ��������ͼƬ
				auto bg = Sprite::create("dialog_bg.png");
				bg->setPosition(Vec2(350, 800));
				this->addChild(bg, 1, "bg");

				// �����ı���ǩ
				auto label = Label::createWithSystemFont("", "simsun.ttc", 40);
				label->setPosition(Vec2(350, 800));
				label->setColor(Color3B::YELLOW);
				this->addChild(label, 2, "textLabel");

				// ������Ҫ��ӡ���ַ���
				std::u16string text = u"���ѽ���ѣ�\n����һ��2048����Ϸ\nͨ������\n����ͬ����ϳɽ���\n�ڴ���ı�����~~~";

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
							//������һ�¹�����ͦ�߼�
							CCLOG("Failed to convert character to UTF-8.");
						}
					}
					else
					{
						this->unschedule("typingEffect");  // ȡ����ʱ��
					}
					}, 0.1, "typingEffect");

				// �����رհ�ť��ֱ�����˹ٷ��İ�ť
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
				this->addChild(closeBtn, 2, "closeBtn");//����ť��Z��ȼ��ᵽ2����ʾ��ǰ��
			}
			});

		//��ʼ��ť
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

