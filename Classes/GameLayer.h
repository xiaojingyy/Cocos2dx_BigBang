#ifndef _GameLayer_H_
#define _GameLayer_H_
#include "cocos2d.h"
#include "Tiled.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace cocostudio;
using namespace ui;
USING_NS_CC;
using namespace std;

class GameLayer:public Layer{
private:
	virtual bool init();
	Vec2 touchDown;
	Label *lScore;

public:	
	CREATE_FUNC(GameLayer);
	//初始化
	void gameInit();
	//全局变量--分数
	static int score;
	//背景移动的云
	void cloudMove(float dt);
	//暂停按钮
	void onPauseClicked(Ref* pSender);
	//暂停的弹框
	void showPauseLayer(std::string message);
	//关闭弹框的按钮
	void onCloseClicked(Ref* pSender);
	//分享功能的按钮
	void onButtonClicked(Ref* sender);
	//复制到剪切板的函数
	void copyLinkToClipboard(const std::string& link);
	//隐藏弹出“已经复制”的信息
	void hideDialog(float dt);
	//隐藏弹出的广告图片
	void hidePopupImage(float dt);
	//弹出广告图片
	void popupImage(float dt);
	// 添加一个变量来存储图片索引
	int imageIndex; 
	// 添加一个容器来存储所有弹出图片
	Vector<Sprite*> imagePopupArray; 
	//添加一键换肤功能
	void onchangeButtonClicked();

	// 声明全局变量
	string imageFolderPath = ""; // 初始文件夹路径

	string m_anima_path = "level%d.png"; 
	bool m_isImageSet1 = true;

	//定义布尔型变量，从loading界面开始就选择主题
	bool spring_scene = false;

	//添加指向Tiled的指针
	Tiled* tiled;

private:
	//这里涉及到游戏界面，要保护好
	Tiled* tables[4][4];
	int move_flag[4][4];
	Vec2 vec[4][4];
	int num_cloud;//云朵
	int number;
	Vector<Sprite *> allCloud;
	Widget* _ui;
	
	//鼠标滑动
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	//四个移动方向，返回是否有砖块移动过
	bool moveToTop();
	bool moveToDown();
	bool moveToLeft();
	bool moveToRight();

	

	void addTiled();
	//结束
	bool isOver();
	
	TextAtlas* _best_score, *_score;
	Sprite* title_sprite;
	ImageView * title_imageview;
	int best_level;



	




};
#endif