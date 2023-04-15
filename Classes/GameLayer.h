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
	//��ʼ��
	void gameInit();
	//ȫ�ֱ���--����
	static int score;
	//�����ƶ�����
	void cloudMove(float dt);
	//��ͣ��ť
	void onPauseClicked(Ref* pSender);
	//��ͣ�ĵ���
	void showPauseLayer(std::string message);
	//�رյ���İ�ť
	void onCloseClicked(Ref* pSender);
	//�����ܵİ�ť
	void onButtonClicked(Ref* sender);
	//���Ƶ����а�ĺ���
	void copyLinkToClipboard(const std::string& link);
	//���ص������Ѿ����ơ�����Ϣ
	void hideDialog(float dt);
	//���ص����Ĺ��ͼƬ
	void hidePopupImage(float dt);
	//�������ͼƬ
	void popupImage(float dt);
	// ���һ���������洢ͼƬ����
	int imageIndex; 
	// ���һ���������洢���е���ͼƬ
	Vector<Sprite*> imagePopupArray; 
	//���һ����������
	void onchangeButtonClicked();

	// ����ȫ�ֱ���
	string imageFolderPath = ""; // ��ʼ�ļ���·��

	string m_anima_path = "level%d.png"; 
	bool m_isImageSet1 = true;

	//���岼���ͱ�������loading���濪ʼ��ѡ������
	bool spring_scene = false;

	//���ָ��Tiled��ָ��
	Tiled* tiled;

private:
	//�����漰����Ϸ���棬Ҫ������
	Tiled* tables[4][4];
	int move_flag[4][4];
	Vec2 vec[4][4];
	int num_cloud;//�ƶ�
	int number;
	Vector<Sprite *> allCloud;
	Widget* _ui;
	
	//��껬��
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	//�ĸ��ƶ����򣬷����Ƿ���ש���ƶ���
	bool moveToTop();
	bool moveToDown();
	bool moveToLeft();
	bool moveToRight();

	

	void addTiled();
	//����
	bool isOver();
	
	TextAtlas* _best_score, *_score;
	Sprite* title_sprite;
	ImageView * title_imageview;
	int best_level;



	




};
#endif