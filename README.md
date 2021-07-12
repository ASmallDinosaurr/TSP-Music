# TSP Music
This is a mod for ArmA that contains musical instruments, a live radio that can be used in vehicles as well as an extension written in C++ to support it.

# TSP Music Extension
Below is some documentation for how to use the C++ extension component of this mod for your own purposes.

## What is it?
This is an extension for ArmA 3 written in C++ that allows you to play 3D audio in-game from either mp3/ogg files or streamed from a URL. Resources can be loaded dynamically at runtime allowing you to do things like stream live radio channels or load audio files on the fly. This extension makes use of the [BASS.dll audio library](http://www.un4seen.com/).

## How to Compile
The .dll file can be compiled using [Mingw-w64](http://mingw-w64.org/doku.php) in either 32 or 64-bit.

32-bit Build command: 
```"C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin\g++.exe" -g C:\radio\tsp_music.cpp -static -shared -o C:\radio\tsp_music.dll```

64-bit Build command:
```"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" -g C:\radio\tsp_music.cpp -static -shared -o C:\radio\tsp_music_x64.dll```

The exact build commands used for VSCode can be found in the .vscode folder for both 32/64-bit. It is important that bass.lib and bass.h are in the same directory as tsp_music.cpp for compilation to work.

## Installation/Usage
To install the extension, both the 32 and 64-bit dll files must be placed in a mod folder. You may also need to turn off BattleEye so it doesn't get blocked on game launch.

The code below is a simple implementation that will call the extension to perform a command that will run locally for each client. For a more complete implementation, check out the postInit.sqf file from the tsp_music mod.

```sqf
tsp_fnc_music_callExtension = {
    params [ "_command", ["_mode", 0], ["_uid", 0], ["_url", ""], ["_vol", 0], ["_pos", [0,0,0]], ["_dir", [0,0,0]]];
    "tsp_music" callExtension [_command, [_mode, _uid, _url, _vol, _pos, _dir]];
};
```

### Parameters
Command** - _ _Tells the extension what to do. Available commands and their required parameters are start (mode,uid,url,volume), stop(uid)", stopAll(), volume(uid,volume) and update(uid,position,direction)._ _

**Mode** - _ _Can be 0 or 1. 0 for a local file that must be stored in the tracks folder in the same directory as the dll, and 1 for a web stream._ _

**UID** - _ _A unique integer identifier for the sound._ _

**URL** - _ _Name or sound file (katyusha.ogg) or web stream url._ _

**Volume** - _ _Floating pointer value for volume of stream._ _

**Position** - _ _Relative position of sound source from player._ _

**Direction** - _ _Relative direction of sound source from player._ _
