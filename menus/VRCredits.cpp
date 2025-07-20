/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "Framework.h"
#include "Bitmap.h"
#include "PicButton.h"
#include "CheckBox.h"
#include "SpinControl.h"

#define ART_CREDITS		"gfx/shell/credits"

class CMenuVRCredits : public CMenuFramework
{
public:
	typedef CMenuFramework BaseClass;

    CMenuVRCredits() : CMenuFramework("CMenuVRCredits") { }

private:
	void _Init() override;
	void _VidInit() override;
	void SaveAndPopMenu() override;

    CMenuBitmap testImage;
};

/*
=================
CMenuVRCredits::SetConfig
=================
*/
void CMenuVRCredits::SaveAndPopMenu()
{
	CMenuFramework::SaveAndPopMenu();
}

/*
=================
CMenuVRCredits::Init
=================
*/
void CMenuVRCredits::_Init( void )
{
    testImage.SetRect( 0, 0, 1080, 810 );
    testImage.SetPicture( ART_CREDITS );
    testImage.iFlags |= QMF_NOTIFY;
    testImage.onReleased = VoidCb( &CMenuVRCredits::SaveAndPopMenu );
    AddItem( testImage );
}

void CMenuVRCredits::_VidInit( )
{
}

ADD_MENU( menu_vr_credits, CMenuVRCredits, UI_VR_Credits );
