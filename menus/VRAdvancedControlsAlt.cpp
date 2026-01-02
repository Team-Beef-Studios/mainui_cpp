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
#include "kbutton.h"
#include "MenuStrings.h"
#include "Bitmap.h"
#include "PicButton.h"
#include "Action.h"
#include "CheckBox.h"
#include "Slider.h"
#include "SpinControl.h"
#include "StringArrayModel.h"

#define ART_BANNER			"gfx/shell/head_advanced"
#define ALT_KEY "XXX"

static struct
{
	const char *name;
	const char *value;
} g_commands[] = {
		{ "nothing", "" },
		{ "duck", "+duck" },
		{ "jump", "+jump" },
		{ "drop weapon", "drop" },
		{ "scoreboard", "+vr_scoreboard" },
		{ "voice chat", "+voicerecord" },
		{ "quick menu", "exec touch/cmd/cmd" },
		{ "use", "+use" },
		{ "buy", "buy" },
		{ "use & buy", "+use;buy" },
		{ "reload", "+reload" },
		{ "fire", "+attack" },
		{ "weapon action", "+attack2" },
		{ "spray", "impulse 201" },
		{ "night vision", "nightvision;toggle_light" },
		{ "text chat", "touch_hide say;touch_hide say2;messagemode" },
};

class CVRAdvancedControlsAlt : public CMenuFramework
{
public:
	typedef CMenuFramework BaseClass;
	CVRAdvancedControlsAlt() : CMenuFramework("CVRAdvancedControlsAlt") { }

private:
	void _Init( void ) override;
	void SaveAndPopMenu() override;

	void AddItem( CMenuSpinControl& item, const char* name, const char* cvar, int& x, int& y );
	void SetCvar( CMenuSpinControl& item, const char* cvar );

	CMenuPicButton done;
	CMenuSpinControl buttonA;
	CMenuSpinControl buttonB;
	CMenuSpinControl buttonX;
	CMenuSpinControl buttonY;
	CMenuSpinControl primaryTrigger;
	CMenuSpinControl primaryGrip;
	CMenuSpinControl primaryThumbstick;
	CMenuSpinControl secondaryTrigger;
	CMenuSpinControl secondaryGrip;
	CMenuSpinControl secondaryThumbstick;
	CMenuAction info;
};

void CVRAdvancedControlsAlt::SaveAndPopMenu()
{
	SetCvar(buttonA, "vr_button_a_alt");
	SetCvar(buttonB, "vr_button_b_alt");
	SetCvar(buttonX, "vr_button_x_alt");
	SetCvar(buttonY, "vr_button_y_alt");
	SetCvar(primaryTrigger, "vr_button_trigger_right_alt");
	SetCvar(primaryGrip, "vr_button_grip_right_alt");
	SetCvar(primaryThumbstick, "vr_button_thumbstick_press_right_alt");
	SetCvar(secondaryTrigger, "vr_button_trigger_left_alt");
	SetCvar(secondaryGrip, "vr_button_grip_left_alt");
	SetCvar(secondaryThumbstick, "vr_button_thumbstick_press_left_alt");

	CMenuFramework::SaveAndPopMenu();
}

/*
=================
UI_AdvControls_Init
=================
*/
void CVRAdvancedControlsAlt::_Init( void )
{
	banner.SetPicture( ART_BANNER );
	CMenuFramework::AddItem( banner );

	done.szName = L( "Done" );
	done.SetPicture( PC_DONE );
	done.onReleased = VoidCb( &CVRAdvancedControlsAlt::SaveAndPopMenu );
	done.SetCoord( 72, 230 );
	CMenuFramework::AddItem( done );

	int x = 320;
	int y = 280;
	AddItem(primaryTrigger, L("Primary trigger"), "vr_button_trigger_right_alt", x, y);
	AddItem(primaryGrip, L("Primary grip"), "vr_button_grip_right_alt", x, y);
	AddItem(primaryThumbstick, L("Primary thumbstick"), "vr_button_thumbstick_press_right_alt", x, y);
	AddItem(buttonA, L("Button A"), "vr_button_a_alt", x, y);
	AddItem(buttonB, L("Button B"), "vr_button_b_alt", x, y);

	info.szName = L( "These options are active only with combination with the alt key defined on the previous screen" );
	info.SetRect( x, y - 40, 675, 240 );
	CMenuFramework::AddItem( info );

	x = 680; y = 280;
	AddItem(secondaryTrigger, L("Secondary trigger"), "vr_button_trigger_left_alt", x, y);
	AddItem(secondaryGrip, L("Secondary grip"), "vr_button_grip_left_alt", x, y);
	AddItem(secondaryThumbstick, L("Secondary thumbstick"), "vr_button_thumbstick_press_left_alt", x, y);
	AddItem(buttonX, L("Button X"), "vr_button_x_alt", x, y);
	AddItem(buttonY, L("Button Y"), "vr_button_y_alt", x, y);
}

void CVRAdvancedControlsAlt::AddItem( CMenuSpinControl& item, const char* name, const char* cvar, int& x, int& y )
{
	static const char *itemlist[V_ARRAYSIZE( g_commands )];
	static CStringArrayModel model( itemlist, V_ARRAYSIZE( g_commands ) );
	for ( size_t i = 0; i < V_ARRAYSIZE( g_commands ); i++ )
		itemlist[i] = L( g_commands[i].name );

	item.szName = L( name );
	item.Setup( &model );
	item.SetCurrentValue( L( g_commands[0].name ) );

	const char* cvarValue = EngFuncs::GetCvarString( cvar );
	for ( auto & g_command : g_commands )
	{
		if ( strcmp( g_command.value, cvarValue ) == 0 )
		{
			item.SetCurrentValue( g_command.name );
		}
	}

	item.onChanged = CMenuEditable::WriteCvarCb;
	item.font = QM_SMALLFONT;
	item.SetRect( x, y, 300, 32 );
	CMenuFramework::AddItem( item );
	y += 90;
}

void CVRAdvancedControlsAlt::SetCvar( CMenuSpinControl& item, const char* cvar )
{
	int i = item.GetCurrentValue();
	if ( i >= 0 && i < V_ARRAYSIZE( g_commands ) )
	{
		EngFuncs::CvarSetString( cvar, g_commands[ i ] .value );
	}
}

ADD_MENU( menu_vr_advcontrols_alt, CVRAdvancedControlsAlt, UI_VR_AdvControlsAlt );
