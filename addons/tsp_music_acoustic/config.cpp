class CfgPatches {
	class tsp_music_acoustic {
		requiredAddons[] = {"cba_common", "tsp_music"};
		units[] = {};
	};
};

//-- FUNCTIONS
class CfgFunctions {
	class tsp_music_acoustic {
		class functions {
			class preInit {file = "\tsp_music_acoustic\preInit.sqf"; preInit = true;};
		};
	};
};
class Extended_PostInit_EventHandlers {
    class ace_acoustic {init = "['Acoustic Guitar','tsp_music_acoustic\gui\ace.paa',['tsp_acoustic','tsp_acoustic_dino'],tsp_cba_music_acoustic_songs] call tsp_fnc_music_addActions_ACE;";};
	class anim_acoustic {init = "['weapon', {[player, ['tsp_acoustic','tsp_acoustic_dino'], _this] spawn tsp_fnc_weaponGestureEvent}] call CBA_fnc_addPlayerEventHandler;";};
};

class CfgWeapons {
	class tsp_music_launcher;
	class tsp_acoustic: tsp_music_launcher {
		restGesture = "tsp_music_guitarRest";
		scope = 2;
		model="\tsp_music_acoustic\acoustic.p3d";
		picture="\tsp_music_acoustic\gui\ui.paa";
		displayname="Acoustic Guitar";
		descriptionShort="Basic, small sized acoustic guitar.";
		hiddenSelections[] = {"camo", "dino"};
		hiddenSelectionsTextures[] = {"\tsp_music_acoustic\tex\acoustic_co.paa", "\tsp_music_acoustic\tex\empty.paa"};
		hiddenSelectionsMaterials[] = {"\tsp_music_acoustic\tex\acoustic.rvmat", "\tsp_music_acoustic\tex\empty.rvmat"};	
		handAnim[] = {"OFP2_ManSkeleton","\tsp_music_acoustic\anim\guitar.rtm"};
	};
	class tsp_acoustic_dino: tsp_acoustic {
		displayname="Acoustic Guitar (Dino)";
		descriptionShort="Dino's Guitar";
		hiddenSelectionsTextures[] = {"\tsp_music_acoustic\tex\dino_co.paa", "\tsp_music_acoustic\tex\dino_co.paa"};
		hiddenSelectionsMaterials[] = {"\tsp_music_acoustic\tex\acoustic.rvmat", "\tsp_music_acoustic\tex\acoustic.rvmat"};	
	};
};