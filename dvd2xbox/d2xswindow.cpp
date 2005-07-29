#include "d2xswindow.h"


D2Xswin::D2Xswin()
{
}

D2Xswin::~D2Xswin()
{
}

void D2Xswin::initScrollWindow(char* array[],int lines2show,bool sortitems)
{
	showlines = lines2show;
	sort = sortitems;
	for(int i=0;i<itemscount;i++)
	{
		if(items[i] != NULL)
		{
			delete[] items[i];
			items[i]=NULL;
		}
	}
	itemscount = 0;
	while(array[itemscount] != NULL)
	{
		items[itemscount] = new char[strlen(array[itemscount])+1];
		strcpy(items[itemscount],array[itemscount]);
		itemscount++;
	}
	cbrowse = 1;
	crelbrowse = 1;
	coffset = 0;
}


int compare_swin( const void *arg1, const void *arg2 )
{
   // Compare all of both strings:
   return _stricmp( * ( char** ) arg1, * ( char** ) arg2 );
}

SWININFO D2Xswin::processScrollWindow(XBGAMEPAD pad)
{
	
	
	info.button = NO_PRESSED;

	if(sort)
        qsort( (void *)items, (size_t)itemscount, sizeof( char * ), compare_swin );
	

	if(pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP) {
		if(cbrowse > 1)
            cbrowse--;
		if(crelbrowse>1)
		{
            crelbrowse--;
		} else {
			if(coffset > 0)
				coffset--;
		}
	}
	if((pad.fY1 > 0.5)) {
		Sleep(100);
		if(cbrowse > 1)
            cbrowse--;
		if(crelbrowse>1)
		{
            crelbrowse--;
		} else {
			if(coffset > 0)
				coffset--;
		}
	}
	if(pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
		if(cbrowse < itemscount)
            cbrowse++;
		if(crelbrowse<showlines)
		{
            crelbrowse++;
		} else {
			if(coffset < (itemscount-showlines))
				coffset++;
		}
	}
	if(pad.fY1 < -0.5) {
		Sleep(100);
		if(cbrowse < itemscount)
            cbrowse++;
		if(crelbrowse<showlines)
		{
            crelbrowse++;
		} else {
			if(coffset < (itemscount-showlines))
				coffset++;
		}
	}

	strcpy(info.item,items[cbrowse-1]);
	info.item_nr = cbrowse-1;
	
	return info;
}

void D2Xswin::showScrollWindow(float x,float y,int width,DWORD fc,DWORD hlfc, CXBFont &font)
{
	WCHAR text[128];
	float tmpy=0;
	int c=0;

	for(int i=0;i<showlines;i++)
	{
		c = i+coffset;
		tmpy = i*font.m_fFontHeight;
		if(c >= itemscount)
			break;
		
		char tname[128];
		strncpy(tname,items[c],width);
		if(width <= strlen(items[c]))
			tname[width] = '\0';
		wsprintfW(text,L"%hs",tname);
		 
		if((i+coffset) == (cbrowse-1))
		{
            font.DrawText( x, y+tmpy, hlfc, text );
		} else {
			font.DrawText( x, y+tmpy, fc, text );
		}

	}
}

void D2Xswin::showMainScrollWindow(float x,float y,int width,DWORD fc,DWORD hlfc, CXBFont &font)
{
	WCHAR text[128];
	float tmpy=0;
	int c=0;
	D2Xgraphics p_graph;

	for(int i=0;i<showlines;i++)
	{
		c = i+coffset;
		tmpy = i*font.m_fFontHeight;
		if(c >= itemscount)
			break;
		
		char tname[128];
		strncpy(tname,items[c],width);
		if(width <= strlen(items[c]))
			tname[width] = '\0';
		wsprintfW(text,L"%hs",tname);
		 
		if((i+coffset) == (cbrowse-1))
		{
			p_graph.RenderBar(x-10,y+tmpy,font.m_fFontHeight,320);
            font.DrawText( x, y+tmpy, hlfc, text );
		} else {
			font.DrawText( x, y+tmpy, fc, text );
		}

	}
}



/// std::string

void D2Xswin::initScrollWindowSTR(int lines2show,map<int,string>& array)
{
	showlines = lines2show;
	cbrowse = 1;
	crelbrowse = 1;
	coffset = 0;
	str_items.clear();
	str_items = array;
	itemscountSTR = array.size();
}

void D2Xswin::initScrollWindowSTR(int lines2show)
{
	showlines = lines2show;
	cbrowse = 1;
	crelbrowse = 1;
	coffset = 0;
	str_items.clear();
}

void D2Xswin::refreshScrollWindowSTR(map<int,string>& array)
{
	str_items = array;
	itemscountSTR = array.size();
}

SWININFO D2Xswin::processScrollWindowSTR(XBGAMEPAD pad)
{
	
	
	info.button = NO_PRESSED;

	if(pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP) {
		if(cbrowse > 1)
            cbrowse--;
		if(crelbrowse>1)
		{
            crelbrowse--;
		} else {
			if(coffset > 0)
				coffset--;
		}
	}
	if((pad.fY1 > 0.5)) {
		Sleep(100);
		if(cbrowse > 1)
            cbrowse--;
		if(crelbrowse>1)
		{
            crelbrowse--;
		} else {
			if(coffset > 0)
				coffset--;
		}
	}
	if(pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
		if(cbrowse < itemscountSTR)
            cbrowse++;
		if(crelbrowse<showlines)
		{
            crelbrowse++;
		} else {
			if(coffset < (itemscountSTR-showlines))
				coffset++;
		}
	}
	if(pad.fY1 < -0.5) {
		Sleep(100);
		if(cbrowse < itemscountSTR)
            cbrowse++;
		if(crelbrowse<showlines)
		{
            crelbrowse++;
		} else {
			if(coffset < (itemscountSTR-showlines))
				coffset++;
		}
	}
	info.item_nr = cbrowse-1;
	strcpy(info.item,str_items[cbrowse-1].c_str());
	return info;
}

void D2Xswin::showScrollWindowSTR(float x,float y,int width,DWORD fc,DWORD hlfc, CXBFont &font)
{
	WCHAR text[130];
	if(width > 128)
		width = 128;
	float tmpy=0;
	int c=0;

	for(int i=0;i<showlines;i++)
	{
		c = i+coffset;
		tmpy = i*font.m_fFontHeight;
		if(c >= itemscountSTR)
			break;
		
		char tname[130];
		strncpy(tname,str_items[c].c_str(),width);
		if(width <= str_items[c].size())
			tname[width] = '\0';
		wsprintfW(text,L"%hs",tname);
		 
		if((i+coffset) == (cbrowse-1))
		{
            font.DrawText( x, y+tmpy, hlfc, text );
		} else {
			font.DrawText( x, y+tmpy, fc, text );
		}

	} 
}

// gui changes

void D2Xswin::showScrollWindow2(float x,float y,int width,int lines,DWORD fc,DWORD hlfc,const CStdString& font)
{
	WCHAR text[256];
	float tmpy=0;
	int c=0;
	if(width > 255)
		width = 255;

	if(lines <= 0)
		lines = 60;

	showlines = lines;

	for(int i=0;i<showlines;i++)
	{
		c = i+coffset;
		tmpy = i*p_ml.getFontHeight(font);
		if(c >= itemscount)
			break;
		
		char tname[256];
		strncpy(tname,items[c],width);
		if(width <= strlen(items[c]))
			tname[width] = '\0';
		wsprintfW(text,L"%hs",tname);
		 
		if((i+coffset) == (cbrowse-1))
		{
			s_x = x;
			s_y = y+tmpy;
			p_ml.DrawText(font, x, y+tmpy, hlfc, text );
		} 
		else 
		{
			p_ml.DrawText(font, x, y+tmpy, fc, text );
		}

	}
}

void D2Xswin::showScrollWindowSTR2(float x,float y,int width,int lines,DWORD fc,DWORD hlfc,const CStdString& font)
{
	WCHAR text[256];
	if(width > 255)
		width = 255;
	if(lines <= 0)
		lines = 60;
	showlines = lines;
	float tmpy=0;
	int c=0;

	for(int i=0;i<showlines;i++)
	{
		c = i+coffset;
		tmpy = i*p_ml.getFontHeight(font);
		if(c >= itemscountSTR)
			break;
		
		char tname[256];
		strncpy(tname,str_items[c].c_str(),width);
		if(width <= str_items[c].size())
			tname[width] = '\0';
		wsprintfW(text,L"%hs",tname);
		 
		if((i+coffset) == (cbrowse-1))
		{
			s_x = x;
			s_y = y+tmpy;
            p_ml.DrawText(font, x, y+tmpy, hlfc, text );
		} 
		else 
		{
			p_ml.DrawText(font, x, y+tmpy, fc, text );
		}

	} 
}

void D2Xswin::getXY(float* posX, float* posY)
{
	*posX = s_x;
	*posY = s_y;
}
