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
#include "StringArrayModel.h"

#define ART_BANNER			"gfx/shell/head_vr"

class CMenuVRControls : public CMenuFramework
{
public:
	typedef CMenuFramework BaseClass;

    CMenuVRControls() : CMenuFramework("CMenuVRControls") { }

private:
	void _Init() override;
	void _VidInit() override;
	void GetConfig();
	void SaveAndPopMenu() override;

	CMenuSpinControl turnangle;
	CMenuCheckBox smoothturn;
	CMenuCheckBox rightHanded;
	CMenuSpinControl walkdir;
	CMenuSpinControl motion;
	CMenuSpinControl armlen;
	CMenuCheckBox haptics;
};

/*
=================
CMenuVRControls::GetConfig
=================
*/
void CMenuVRControls::GetConfig( void )
{
	turnangle.LinkCvar( "vr_turn_angle", CMenuEditable::CVAR_VALUE );
	smoothturn.LinkCvar( "vr_turn_type" );
	rightHanded.LinkCvar( "vr_righthand" );
	walkdir.LinkCvar( "vr_walkdirection", CMenuEditable::CVAR_VALUE );
	motion.LinkCvar( "vr_motion_activation", CMenuEditable::CVAR_VALUE );
	armlen.LinkCvar( "vr_arm_length", CMenuEditable::CVAR_VALUE );
	haptics.LinkCvar( "vr_haptics_enable" );
}

/*
=================
CMenuVRControls::SetConfig
=================
*/
void CMenuVRControls::SaveAndPopMenu()
{
	turnangle.WriteCvar();
	smoothturn.WriteCvar();
	rightHanded.WriteCvar();
	walkdir.WriteCvar();
	motion.WriteCvar();
	armlen.WriteCvar();
	haptics.WriteCvar();

	CMenuFramework::SaveAndPopMenu();
}

/*
=================
CMenuVRControls::Init
=================
*/
void CMenuVRControls::_Init( void )
{
	banner.SetPicture(ART_BANNER);

	haptics.szName = L( "Controller haptics" );
	haptics.onChanged = CMenuEditable::WriteCvarCb;
	haptics.SetCoord( 320, 280 );

	rightHanded.szName = L( "Right-handed mapping" );
	rightHanded.onChanged = CMenuEditable::WriteCvarCb;
	rightHanded.SetCoord( 320, 330 );

	smoothturn.szName = L( "Smooth turn" );
	smoothturn.onChanged = CMenuEditable::WriteCvarCb;
	smoothturn.SetCoord( 320, 380 );

	turnangle.szName = L( "Turn angle/speed" );
	turnangle.Setup( 5, 90, 5 );
	turnangle.onChanged = CMenuEditable::WriteCvarCb;
	turnangle.font = QM_SMALLFONT;
	turnangle.SetRect( 320, 460, 300, 32 );

	static const char *walkdirStr[] = {L( "Controller" ), L( "HMD" )};
	static CStringArrayModel model( walkdirStr, V_ARRAYSIZE( walkdirStr ));
	walkdir.szName = L( "Walk in direction of" );
	walkdir.Setup( &model );
	walkdir.onChanged = CMenuEditable::WriteCvarCb;
	walkdir.font = QM_SMALLFONT;
	walkdir.SetRect( 680, 280, 300, 32 );

	static const char *motionStr[] = {L( "Disabled" ), L( "Controller button" ), L( "Stretched arm" ), L( "Both options" )};
	static CStringArrayModel activator( motionStr, V_ARRAYSIZE( motionStr ));
	motion.szName = L( "Two hands weapon/hand action" );
	motion.Setup( &activator );
	motion.onChanged = CMenuEditable::WriteCvarCb;
	motion.font = QM_SMALLFONT;
	motion.SetRect( 680, 370, 300, 32 );

	static const char *armlenStr[] = {L( "Very short" ), L( "Short" ), L( "Normal" ), L( "Long" ), L( "Very long" )};
	static CStringArrayModel len( armlenStr, V_ARRAYSIZE( armlenStr ));
	armlen.szName = L( "Your arm length" );
	armlen.Setup( &len );
	armlen.onChanged = CMenuEditable::WriteCvarCb;
	armlen.font = QM_SMALLFONT;
	armlen.SetRect( 680, 470, 300, 32 );

	AddItem( banner );
	AddButton( L( "Done" ), nullptr, PC_DONE, VoidCb( &CMenuVRControls::SaveAndPopMenu ));
	AddItem( rightHanded );
	AddItem( haptics );
	AddItem( smoothturn );
	AddItem( turnangle );
	AddItem( walkdir );
	AddItem( motion );
	AddItem( armlen );
}

void CMenuVRControls::_VidInit( )
{
	GetConfig();
}

ADD_MENU( menu_vrcontrols, CMenuVRControls, UI_VR_Controls );
