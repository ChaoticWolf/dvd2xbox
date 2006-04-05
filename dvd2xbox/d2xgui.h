#ifndef D2XGUI
#define D2XGUI

#include <xtl.h>
#include "stdstring.h"
#include "d2xmedialib.h"
#include "d2xgraphics.h"
#include "d2xswindow.h"
#include "d2xgamemanager.h"
#include "d2xdbrowser.h"
#include "d2xguisettings.h"
#include "d2xviewer.h"
#include "..\keyboard\virtualkeyboard.h"
#include "..\..\xbox\tinyxml\tinyxml.h"
#include <vector>
#include <map>

#define X				0
#define Y				1

#define GUI_MAINMENU	0
#define GUI_GAMEMANAGER	1
#define GUI_FILEMANAGER	2
#define GUI_KEYBOARD	3
#define GUI_SETTINGS	4
#define GUI_VIEWER		5
#define GUI_DISKCOPY	6
#define	GUI_ERROR		7
#define GUI_STARTUP		8


class D2Xgui
{
protected:

	D2Xgui();
	D2Xmedialib*				p_ml;
	D2Xgraphics					p_graph;
	D2XGM*						p_gm;
	CXBVirtualKeyboard* 		p_vk;
	D2Xguiset*					p_sg;
	D2Xviewer*					p_v;
	vector<TiXmlDocument*>		vXML;
	map<CStdString,CStdString>	strcText;
	map<CStdString,int>			strcInt;
	map<int,D2Xswin*>			map_swin;
	vector<int>					v_showids;
	D2Xdbrowser*	a_browser[2];
	TiXmlDocument*	xmlgui;
	CStdString		strSkin;
	int				prev_id;
	bool			skip_frame;

	static std::auto_ptr<D2Xgui> sm_inst;

	int LoadXML(CStdString strSkinName);
	void DoClean();
	bool IsShowIDinList(int showid);
	float getMenuPosXY(int XY, int id, int showID);
	float getMenuOrigin(int XY, int id, int showID);
	int getMenuItems( int id, int showID, int* vspace=NULL);


public:
	

	static D2Xgui* Instance();

	int LoadSkin(CStdString strSkinName);
	void RenderGUI(int id);
	void SetKeyValue(CStdString key,CStdString value);
	void SetKeyInt(CStdString key, int value);
	int	 getKeyInt(CStdString key);
	CStdString getKeyValue(CStdString key);
	void SetWindowObject(int id, D2Xswin* win);
	void SetGMObject(D2XGM* gm);
	void SetVKObject(CXBVirtualKeyboard* vk);
	void SetBrowserObject(int id, D2Xdbrowser* b);
	void SetSGObject(D2Xguiset* sg);
	void SetViewObject(D2Xviewer* v);
	void SetShowIDs(int showid);


};

#endif