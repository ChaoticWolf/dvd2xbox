#ifndef D2XINPUT
#define D2XINPUT

#include <xtl.h>
#include "xbApplicationEx.h"

#define GP_A		10200
#define GP_B		10210
#define GP_X		10220
#define GP_Y		10230
#define GP_LTRIGGER	10240
#define GP_RTRIGGER	10250
#define GP_BLACK	10260
#define GP_WHITE	10270
#define GP_BACK		10280
#define GP_START	10290
#define GP_TL_LEFT	10300
#define GP_TL_RIGHT	10310

class D2Xinput
{
protected:
	XBGAMEPAD	gp;
	XBIR_REMOTE ir;

public:
	D2Xinput();
	~D2Xinput();

	void update(XBGAMEPAD gamepad, XBIR_REMOTE iremote);
	void update(XBGAMEPAD gamepad);
	bool pressed(int button);
};

#endif