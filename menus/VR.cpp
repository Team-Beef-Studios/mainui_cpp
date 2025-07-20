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

#define ART_BANNER			"gfx/shell/head_vr"

class CMenuVR : public CMenuFramework
{
public:
	typedef CMenuFramework BaseClass;

    CMenuVR() : CMenuFramework("CMenuVR") { }

private:
	void _Init() override;
	void _VidInit() override;
	void GetConfig();
	void SaveAndPopMenu() override;

	CMenuSpinControl supersampling;
	CMenuCheckBox msaa;
	CMenuCheckBox refreshrate;
	CMenuCheckBox tracking;
	CMenuSpinControl turnangle;
	CMenuCheckBox smoothturn;
	CMenuCheckBox rightHanded;
};

/*
=================
CMenuVR::GetConfig
=================
*/
void CMenuVR::GetConfig( void )
{
	supersampling.LinkCvar( "vr_supersampling", CMenuEditable::CVAR_VALUE );
	msaa.LinkCvar( "vr_msaa" );
	refreshrate.LinkCvar( "vr_refreshrate" );
	tracking.LinkCvar( "vr_6dof" );
	turnangle.LinkCvar( "vr_turn_angle", CMenuEditable::CVAR_VALUE );
	smoothturn.LinkCvar( "vr_turn_type" );
	rightHanded.LinkCvar( "cl_righthand" );
}

/*
=================
CMenuVR::SetConfig
=================
*/
void CMenuVR::SaveAndPopMenu()
{
	supersampling.WriteCvar();
	msaa.WriteCvar();
	refreshrate.WriteCvar();
	tracking.WriteCvar();
	turnangle.WriteCvar();
	smoothturn.WriteCvar();
	rightHanded.WriteCvar();

	CMenuFramework::SaveAndPopMenu();
}

/*
=================
CMenuVR::Init
=================
*/
void CMenuVR::_Init( void )
{
	banner.SetPicture(ART_BANNER);

	supersampling.szName = L( "Supersampling" );
	supersampling.Setup( 0.9f, 1.3f, 0.1f );
	supersampling.SetDisplayPrecision(1);
	supersampling.onChanged = CMenuEditable::WriteCvarCb;
	supersampling.font = QM_SMALLFONT;
	supersampling.SetRect( 320, 280, 300, 32 );

	msaa.szName = L( "Multi-sample antialiasing" );
	msaa.onChanged = CMenuEditable::WriteCvarCb;
	msaa.SetCoord( 320, 330 );

	refreshrate.szName = L( "90hz refresh rate" );
	refreshrate.onChanged = CMenuEditable::WriteCvarCb;
	refreshrate.SetCoord( 320, 380 );

	tracking.szName = L( "6DoF world tracking" );
	tracking.onChanged = CMenuEditable::WriteCvarCb;
	tracking.SetCoord( 320, 430 );

	turnangle.szName = L( "Turn angle/speed" );
	turnangle.Setup( 5, 90, 5 );
	turnangle.onChanged = CMenuEditable::WriteCvarCb;
	turnangle.font = QM_SMALLFONT;
	turnangle.SetRect( 320, 580, 300, 32 );

	smoothturn.szName = L( "Smooth turn" );
	smoothturn.onChanged = CMenuEditable::WriteCvarCb;
	smoothturn.SetCoord( 320, 630 );

	rightHanded.szName = L( "Right-handed controller mapping" );
	rightHanded.onChanged = CMenuEditable::WriteCvarCb;
	rightHanded.SetCoord( 320, 680 );

	AddItem( banner );
	AddButton( L( "Done" ), nullptr, PC_DONE, VoidCb( &CMenuVR::SaveAndPopMenu ));
	AddItem( msaa );
	AddItem( supersampling );
	AddItem( refreshrate );
	AddItem( tracking );
	AddItem( turnangle );
	AddItem( smoothturn );
	AddItem( rightHanded );
}

void CMenuVR::_VidInit( )
{
	GetConfig();
}

ADD_MENU( menu_vr, CMenuVR, UI_VR_Menu );
