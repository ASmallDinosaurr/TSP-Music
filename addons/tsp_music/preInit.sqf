[
	"tsp_cba_music", "CHECKBOX",
	["Enable Music", "Enables extension activation. Requires mission restart to change."],
	"TSP Music", false, true, {}
] call CBA_Settings_fnc_init;

[
	"tsp_cba_music_defaultVolume", "SLIDER",
	["Default Volume", "Default volume for radio."],
	"TSP Music", [0, 1, 0.5], false, {}
] call CBA_Settings_fnc_init;

[
	"tsp_cba_music_updateInterval", "SLIDER",
	["Update Interval", "How often sound positions are updated."],
	"TSP Music", [0, 1, 0.1], false, {}
] call CBA_Settings_fnc_init;

[
	"tsp_cba_music_gain", "SLIDER",
	["Gain", "Gain."],
	"TSP Music", [0, 1, 0.1], false, {}
] call CBA_Settings_fnc_init;

tsp_fnc_music_addActions_ACE = {  //-- Needs to be here so it gets initailized first
	params ["_name", "_icon", "_instruments", "_songs"];
	_songs = call compile _songs;  //-- Needed since CBA array settings don't initialize quickly enough

	//-- Instrument category
    _action = [_name,_name,_icon,{},{
		params ["_source", "_player", "_actionParams"];
		_actionParams params ["_instruments"];
		currentWeapon _player in _instruments && !tsp_music_playing
	},{},[_instruments],[0,0,0],5] call ace_interact_menu_fnc_createAction; 
    ["CAManBase", 1, ["ACE_SelfActions"], _action, true] call ace_interact_menu_fnc_addActionToClass;

	//-- Stop
	_action = [(_name+"Stop"),"Stop",_icon,{tsp_music_playing = false},{
		params ["_source", "_player", "_actionParams"];
		_actionParams params ["_instruments", "_playing"];
		currentWeapon _player in _instruments && tsp_music_playing
	},{},[_instruments],[0,0,0],5] call ace_interact_menu_fnc_createAction; 
    ["CAManBase", 1, ["ACE_SelfActions"], _action, true] call ace_interact_menu_fnc_addActionToClass;

	//-- Songs
    {
        _x params ["_songName", "_track", "_duration", "_volume", "_gesture"];
        _action = [_songName, _songName, "", {
            params ["_source", "_player", "_actionParams"];
            _actionParams params ["_track", "_duration", "_gesture", "_volume"];
			[_source, _track, _duration, _gesture, _volume] spawn tsp_fnc_music_playInstrument;
        }, {true}, {}, [_track, _duration, _gesture, _volume]] call ace_interact_menu_fnc_createAction;     
        ["CAManBase", 1, ["ACE_SelfActions",_name], _action, true] call ace_interact_menu_fnc_addActionToClass;
    } forEach _songs;
};