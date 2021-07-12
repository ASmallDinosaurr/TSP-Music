tsp_music_playing = false;
tsp_fnc_music_playInstrument = {
	params ["_unit", "_track", "_duration", "_gesture", "_volume"];
	_originalWeapon = currentWeapon _unit;

	[_unit, "", 0, _gesture, -1, "tsp_common_stop", true, true] spawn tsp_fnc_playGesture;
    [_unit, _track, 0, _volume, false] remoteExec ["tsp_fnc_music_start", 0];

	tsp_music_playing = true;
	_countDown = _duration spawn {uiSleep _this; tsp_music_playing = false};
	while {sleep 0.5; tsp_music_playing} do {
        if (!alive _unit) exitWith {};
		if (currentWeapon _unit != _originalWeapon) exitWith {};   
        if (tsp_gesture_current != _gesture) exitWith {};
	};
    tsp_music_playing = false;
    
    terminate _countDown;
    [_unit] remoteExec ["tsp_fnc_music_stop", 0];
	if (tsp_gesture_current == _gesture) exitWith {tsp_common_stopFlag = true};
};
	
////---- EXTENSION STUFF BELOW
if (isNil "tsp_music_activeSources") then {tsp_music_activeSources = []};

{  //-- Turn on sources for JIP clients
    [_x, _x getVariable "url", 1, _x getVariable "volume", true] spawn tsp_fnc_music_start;
} forEach tsp_music_activeSources;

tsp_fnc_music_start = {
    params ["_source", "_url", ["_mode", 0], ["_vol", -1], ["_jip", false]];
    
    //-- If vehicle is already streaming, then stop it before making a new one
    if (_source in tsp_music_activeSources) then {  
        if (!_jip) then {["stop", 0, _source] call tsp_fnc_music_callExtension};
        sleep 1;
    } else {  //-- Else add it
        tsp_music_activeSources pushBack _source;
    };

    //-- Start stream with this ID
    if (_vol == -1) then {_vol = _source getVariable ["volume", tsp_cba_music_defaultVolume]};
    _source setVariable ["url", _url, true];
    _source setVariable ["volume", _vol, true];
    ["start", _mode, _source, _url, _vol] spawn tsp_fnc_music_callExtension;

    //-- When source dies
    _source addMPEventHandler ["MPKilled", {
        params ["_source", "_killer", "_instigator", "_useEffects"];
        [_source] spawn tsp_fnc_music_stop;        
        _source removeAllMPEventHandlers "MPKilled";
    }];

    [] spawn tsp_fnc_music_update;
};

tsp_fnc_music_stop = {
    params ["_source"];

    //-- Remove source object from active source list
    {
        if (_x == _source) exitWith {tsp_music_activeSources deleteAt _forEachIndex};
    } forEach tsp_music_activeSources;

    //-- Stop stream with this ID
    ["stop", 0, _source] spawn tsp_fnc_music_callExtension;
};

tsp_fnc_music_volume = {
    params ["_source", "_vol"];
    ["volume", 0, _source, "", _vol] spawn tsp_fnc_music_callExtension;  //-- Change volume of stream with this ID
    _source setVariable ["volume", _vol, true];
};

tsp_fnc_music_update = {
    if (isNil "tsp_music_updating") then {tsp_music_updating = false};
    if (tsp_music_updating) exitWith {};
    tsp_music_updating = true;    
    while {sleep tsp_cba_music_updateInterval; tsp_cba_music} do {
        if (count tsp_music_activeSources == 0) exitWith {};
        {
            _relativeSourcePos = [0,0,0];
            if (vehicle player != _x) then {
                _relativeSourcePos = getPosASL _x vectorDiff getPosASL player;
                _relativeSourcePos = [			
                    parseNumber ((_relativeSourcePos#0) toFixed 1),
                    parseNumber ((_relativeSourcePos#1) toFixed 1),
                    parseNumber ((_relativeSourcePos#2) toFixed 1)
                ];
            };
            ["update", 0, _x, "url", 3, _relativeSourcePos, vectorDir player] call tsp_fnc_music_callExtension;
        } forEach tsp_music_activeSources;
	};
    tsp_music_updating = false;
};

tsp_fnc_music_uid = {
    params ["_source"];    
    if (_source getVariable ["uid", -1] != -1) exitWith {_source getVariable "uid"};  //-- If vehicle has ID, just return it
    if (isNil "tsp_music_sourceIDCounter") then {tsp_music_sourceIDCounter = 1};  //-- Initialize counter
    tsp_music_sourceIDCounter = tsp_music_sourceIDCounter + 1;  //-- Increment
    publicVariable "tsp_music_sourceIDCounter";
    _source setVariable ["uid", tsp_music_sourceIDCounter, true];  //-- Set it on the vehicle
    tsp_music_sourceIDCounter
};

tsp_fnc_music_callExtension = {
    params [  "_command", ["_mode", 0], ["_source", objNull], ["_url", ""], ["_vol", 0], ["_pos", [0,0,0]], ["_dir", [0,0,0]]  ];
    _uid = [_source] call tsp_fnc_music_uid;
    if (!tsp_cba_music) exitWith {};
    "tsp_music" callExtension [_command, [_mode, _uid, _url, _vol*tsp_cba_music_gain, _pos, _dir]];
    profileNamespace setVariable ["tsp_music_oldSources", tsp_music_activeSources];
};

if (count (profileNamespace getVariable ["tsp_music_oldSources", []]) != 0) then {
    ["stopAll"] call tsp_fnc_music_callExtension; 
    profileNamespace setVariable ["tsp_music_oldSources", []];
};