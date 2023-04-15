#ifndef _Tiled_H_
#define _Tiled_H_
#include "cocos2d.h"

USING_NS_CC;

class Tiled:public Node{
private:
	int level;
	
	Label *label;
public:
	static const int nums[16];

	
	bool m_isAnimaPathSet1 = true;
	std::string m_anima_path = "level%d.png";

	Sprite *backround;
	Tiled();
	virtual bool init();
	CREATE_FUNC(Tiled);
	void setLevel(int l );
	int getLevel()
	{
		return level;
	};

	

};
#endif