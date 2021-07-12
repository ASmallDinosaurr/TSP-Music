class CfgPatches {
	class tsp_music {
		requiredAddons[] = {"cba_common"};		
		units[] = {};
	};
};

//-- FUNCTIONS
class CfgFunctions {
	class tsp_music {
		class functions {
			class preInit {file = "\tsp_music\preInit.sqf"; preInit = true};
			class postInit {file = "\tsp_music\postInit.sqf"; postInit = true;};
		};
	};
};

//-- BASE INSTRUMENTS
class CfgWeapons {
	//-- Launcher slot
	class launch_RPG32_F;
	class tsp_music_launcher: launch_RPG32_F {
		scope = 0;
		modelOptics = "";
		useModelOptics = true;
		fireLightIntensity = 0;		
		magazines[] = {};
		weaponInfoType = "RscWeaponEmpty";
	};
};

//-- ANIMATIONS
class CfgMovesBasic {
    class default;
    class DefaultDie;
    class ManActions {
		tsp_music_guitarRest[] = {"tsp_music_guitarRest", "Gesture"};
		tsp_music_guitarStrum[] = {"tsp_music_guitarStrum", "Gesture"};
		tsp_music_guitarPick[] = {"tsp_music_guitarPick", "Gesture"};
	};
};

class CfgGesturesMale {
	skeletonName = "OFP2_ManSkeleton";
	class Default {};
	class States {
		class GestureNo;
		class tsp_music_guitarRest: GestureNo {	
			file = "tsp_music\anim\guitarRest.rtm";
			mask = "guitar";
			interpolationSpeed = 10;
			speed = 0.0001;		
			looped = true;
			leftHandIKCurve[] = {0,1,0.00001,0};
			rightHandIKCurve[] = {0,1,0.00001,0};
			canPullTrigger = false;		
			enableBinocular = false;
			enableMissile = false;
			enableOptics = false;
		};
		class tsp_music_guitarStrum: tsp_music_guitarRest {	
			speed = 0.1;
			file = "tsp_music\anim\guitarStrum.rtm";
		};
		class tsp_music_guitarPick: tsp_music_guitarStrum {
			speed = 0.3;
			file = "tsp_music\anim\guitarPick.rtm";
		};
	};
	class BlendAnims {
		guitar[] = {
			"Spine",
			0.1,
			"Spine1",
			0.2,
			"Spine2",
			0.6,
			"Spine3",
			0.9,
			"launcher",
			1,
			"LeftShoulder",
			1,
			"LeftArm",
			1,
			"LeftArmRoll",
			1,
			"LeftForeArm",
			1,
			"LeftForeArmRoll",
			1,
			"LeftHand",
			1,
			"LeftHandRing",
			1,
			"LeftHandPinky1",
			1,
			"LeftHandPinky2",
			1,
			"LeftHandPinky3",
			1,
			"LeftHandRing1",
			1,
			"LeftHandRing2",
			1,
			"LeftHandRing3",
			1,
			"LeftHandMiddle1",
			1,
			"LeftHandMiddle2",
			1,
			"LeftHandMiddle3",
			1,
			"LeftHandIndex1",
			1,
			"LeftHandIndex2",
			1,
			"LeftHandIndex3",
			1,
			"LeftHandThumb1",
			1,
			"LeftHandThumb2",
			1,
			"LeftHandThumb3",			
			1,
			"RightShoulder",
			1,
			"RightArm",
			1,
			"RightArmRoll",
			1,
			"RightForeArm",
			1,
			"RightForeArmRoll",
			1,
			"RightHand",
			1,
			"RightHandRing",
			1,
			"RightHandPinky1",
			1,
			"RightHandPinky2",
			1,
			"RightHandPinky3",
			1,
			"RightHandRing1",
			1,
			"RightHandRing2",
			1,
			"RightHandRing3",
			1,
			"RightHandMiddle1",
			1,
			"RightHandMiddle2",
			1,
			"RightHandMiddle3",
			1,
			"RightHandIndex1",
			1,
			"RightHandIndex2",
			1,
			"RightHandIndex3",
			1,
			"RightHandThumb1",
			1,
			"RightHandThumb2",
			1,
			"RightHandThumb3",
			1
		};
	};
};