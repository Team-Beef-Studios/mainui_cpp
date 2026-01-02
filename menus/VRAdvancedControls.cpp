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
#include "CheckBox.h"
#include "Slider.h"
#include "SpinControl.h"
#include "StringArrayModel.h"

#define ART_BANNER			"gfx/shell/head_advanced"

#define SAVE_CVAR(item, name) \
i = item.GetCurrentValue(); \
if ( i >= 0 && i < V_ARRAYSIZE( g_commands ) ) \
EngFuncs::CvarSetString( name, g_commands[i].value );

static struct
{
	const char *name;
	const char *value;
} g_commands[] = {
		{ "nothing", "" },
		{ "duck", "+duck" },
		{ "jump", "+jump" },
		{ "drop", "drop" },
		{ "scoreboard", "+vr_scoreboard" },
		{ "voice chat", "+voicerecord" },
		{ "quick menu", "exec touch/cmd/cmd" },
		{ "use", "+use" },
		{ "buy", "buy" },
		{ "use & buy", "+use;buy" },
		{ "reload", "+reload" },
		{ "primary attack", "+attack" },
		{ "secondary attack", "+attack2" },
		{ "spray", "impulse 201" },
		{ "night vision", "nightvision;toggle_light" },
		{ "text chat", "touch_hide say;touch_hide say2;messagemode" },
};

class CVRAdvancedControls : public CMenuFramework
{
public:
	typedef CMenuFramework BaseClass;
	CVRAdvancedControls() : CMenuFramework("CVRAdvancedControls") { }

private:
	void _Init( void ) override;
	void SaveAndPopMenu() override;

	void AddItem( CMenuSpinControl& item, const char* name, const char* cvar, int& x, int& y );

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
};

void CVRAdvancedControls::SaveAndPopMenu()
{
	int i = 0;
	SAVE_CVAR(buttonA, "vr_button_a");
	SAVE_CVAR(buttonB, "vr_button_b");
	SAVE_CVAR(buttonX, "vr_button_x");
	SAVE_CVAR(buttonY, "vr_button_y");
	SAVE_CVAR(primaryTrigger, "vr_button_trigger_right");
	SAVE_CVAR(primaryGrip, "vr_button_grip_right");
	SAVE_CVAR(primaryThumbstick, "vr_button_thumbstick_press_right");
	SAVE_CVAR(secondaryTrigger, "vr_button_trigger_left");
	SAVE_CVAR(secondaryGrip, "vr_button_grip_left");
	SAVE_CVAR(secondaryThumbstick, "vr_button_thumbstick_press_left");

	CMenuFramework::SaveAndPopMenu();
}

/*
=================
UI_AdvControls_Init
=================
*/
void CVRAdvancedControls::_Init( void )
{
	banner.SetPicture( ART_BANNER );
	CMenuFramework::AddItem( banner );

	done.szName = L( "Done" );
	done.SetPicture( PC_DONE );
	done.onReleased = VoidCb( &CVRAdvancedControls::SaveAndPopMenu );
	done.SetCoord( 72, 230 );
	CMenuFramework::AddItem( done );

	int x = 320;
	int y = 280;
	AddItem(primaryTrigger, "Primary trigger", "vr_button_trigger_right", x, y);
	AddItem(primaryGrip, "Primary grip", "vr_button_grip_right", x, y);
	AddItem(primaryThumbstick, "Primary thumbstick", "vr_button_thumbstick_press_right", x, y);
	AddItem(buttonA, "Button A", "vr_button_a", x, y);
	AddItem(buttonB, "Button B", "vr_button_b", x, y);
	x = 680; y = 280;
	AddItem(secondaryTrigger, "Secondary trigger", "vr_button_trigger_left", x, y);
	AddItem(secondaryGrip, "Secondary grip", "vr_button_grip_left", x, y);
	AddItem(secondaryThumbstick, "Secondary thumbstick", "vr_button_thumbstick_press_left", x, y);
	AddItem(buttonX, "Button X", "vr_button_x", x, y);
	AddItem(buttonY, "Button Y", "vr_button_y", x, y);
}

void CVRAdvancedControls::AddItem( CMenuSpinControl& item, const char* name, const char* cvar, int& x, int& y )
{
	static const char *itemlist[V_ARRAYSIZE( g_commands )];
	static CStringArrayModel model( itemlist, V_ARRAYSIZE( g_commands ) );
	for ( size_t i = 0; i < V_ARRAYSIZE( g_commands ); i++ )
		itemlist[i] = L( g_commands[i].name );

	item.szName = L( name );
	item.Setup( &model );
	item.SetCurrentValue( g_commands[0].name );

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

ADD_MENU( menu_vr_advcontrols, CVRAdvancedControls, UI_VR_AdvControls );
