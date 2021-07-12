#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <atomic>
#include <mutex>
#include <thread>
#include <unistd.h>
#include "bass.h"
using namespace std;

LPCSTR dllName = "tsp_music.dll";
string bassName = "bass.dll";
//-- Dynamic BASS.dll linking and initialization
	string dir;
	typedef int(*BASS_Init_PTR)(int, DWORD, DWORD, HWND, const GUID*); BASS_Init_PTR bass_Init;
	typedef int(*BASS_ErrorGetCode_PTR)(); BASS_ErrorGetCode_PTR bass_ErrorGetCode;
	typedef HSTREAM(*BASS_StreamCreateURL_PTR)(const char *url, DWORD offset, DWORD flags, DOWNLOADPROC *proc, void *user); BASS_StreamCreateURL_PTR bass_StreamCreateURL;
	typedef HSTREAM(*BASS_StreamCreateFile_PTR)(BOOL mem, void *file, QWORD offset, QWORD length, DWORD flags); BASS_StreamCreateFile_PTR bass_StreamCreateFile;
	typedef BOOL(*BASS_ChannelPlay_PTR)(DWORD handle, BOOL restart); BASS_ChannelPlay_PTR bass_ChannelPlay;
	typedef BOOL(*BASS_StreamFree_PTR)(HSTREAM handle); BASS_StreamFree_PTR bass_StreamFree;
	typedef BOOL(*BASS_ChannelSetAttribute_PTR)(DWORD handle, DWORD attrib, float value); BASS_ChannelSetAttribute_PTR bass_ChannelSetAttribute;
	typedef BOOL(*BASS_ChannelSet3DPosition_PTR)(DWORD handle, BASS_3DVECTOR *pos, BASS_3DVECTOR *orient, BASS_3DVECTOR *vel); BASS_ChannelSet3DPosition_PTR 
	bass_ChannelSet3DPosition;
	typedef void(*BASS_Apply3D_PTR)(); BASS_Apply3D_PTR bass_Apply3D;
	typedef void(*BASS_Set3DPosition_PTR)(BASS_3DVECTOR *pos, BASS_3DVECTOR *vel, BASS_3DVECTOR *front, BASS_3DVECTOR *top); BASS_Set3DPosition_PTR bass_Set3DPosition;
	typedef BOOL(*BASS_Set3DFactors_PTR)(float distf, float rollf, float doppf); BASS_Set3DFactors_PTR bass_Set3DFactors;
	typedef BOOL(*BASS_SetDevice_PTR)(DWORD device); BASS_SetDevice_PTR bass_SetDevice;
	typedef BOOL(*BASS_Get3DPosition_PTR)(BASS_3DVECTOR *pos, BASS_3DVECTOR *vel, BASS_3DVECTOR *front, BASS_3DVECTOR *top); BASS_Get3DPosition_PTR bass_Get3DPosition;
	HINSTANCE hInst;
	void loadBass() {  //-- Call this function to initialize BASS and BASS functions
		char path[MAX_PATH];
		GetModuleFileName( GetModuleHandle(dllName), path, MAX_PATH );  //-- Assign current directory to path
		dir = path;  //-- Convert to string
		dir = dir.substr(0, dir.find_last_of("\\/"));  //-- Remove filename from directory
		hInst = LoadLibrary(TEXT((dir + "\\" + bassName).c_str()));  //-- Load DLL file
		bass_Init = (BASS_Init_PTR)GetProcAddress(hInst, "BASS_Init");
		bass_ErrorGetCode = (BASS_ErrorGetCode_PTR)GetProcAddress(hInst, "BASS_ErrorGetCode");
		bass_StreamCreateURL = (BASS_StreamCreateURL_PTR)GetProcAddress(hInst, "BASS_StreamCreateURL");
		bass_StreamCreateFile = (BASS_StreamCreateFile_PTR)GetProcAddress(hInst, "BASS_StreamCreateFile");
		bass_ChannelPlay = (BASS_ChannelPlay_PTR)GetProcAddress(hInst, "BASS_ChannelPlay");
		bass_StreamFree = (BASS_StreamFree_PTR)GetProcAddress(hInst, "BASS_StreamFree");
		bass_ChannelSetAttribute = (BASS_ChannelSetAttribute_PTR)GetProcAddress(hInst, "BASS_ChannelSetAttribute");
		bass_ChannelSet3DPosition = (BASS_ChannelSet3DPosition_PTR)GetProcAddress(hInst, "BASS_ChannelSet3DPosition");
		bass_Apply3D = (BASS_Apply3D_PTR)GetProcAddress(hInst, "BASS_Apply3D");
		bass_Set3DPosition = (BASS_Set3DPosition_PTR)GetProcAddress(hInst, "BASS_Set3DPosition");
		bass_Set3DFactors = (BASS_Set3DFactors_PTR)GetProcAddress(hInst, "BASS_Set3DFactors");
		bass_SetDevice = (BASS_SetDevice_PTR)GetProcAddress(hInst, "BASS_SetDevice");
		bass_Get3DPosition = (BASS_Get3DPosition_PTR)GetProcAddress(hInst, "BASS_Get3DPosition");
		int bass = bass_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL);
		bass_Set3DFactors(1, 1, 1);
	};

//-- Actual stuff
atomic<bool> main_working(false);
deque<vector <string>> instructionQueue;
vector<int> activeSourceIds;
vector<HSTREAM> activeSourceStreams;
vector<float> activeSourceVolumes;
vector<double> stringToVector(string input) {	
	vector<double> output;
	input.erase(0, 1); input.erase(input.size() - 1);
	stringstream ss(input);
	while (ss.good()) {
		string substr;
		getline(ss, substr, ',');
		output.push_back(atof(substr.c_str()));
	};
	return output;
};
void start(int mode, int id, string url, float vol) {
	HSTREAM stream;	
	if (mode == 0) {
		url = dir + "\\tracks\\" + url;
		cout << url << endl;
		stream = bass_StreamCreateFile(FALSE, &url[0], 0, 0, BASS_SAMPLE_3D | BASS_SAMPLE_MONO | BASS_SAMPLE_SOFTWARE);
	} else {
    	stream = bass_StreamCreateURL(url.c_str(), 0, BASS_SAMPLE_3D | BASS_SAMPLE_MONO | BASS_SAMPLE_SOFTWARE, NULL, 0);
	};
	bass_ChannelPlay(stream, false);
	bass_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, vol);
	activeSourceIds.push_back(id);
	activeSourceStreams.push_back(stream);
	activeSourceVolumes.push_back(vol);
};
void stop(int id, BOOL all) {
	for (int i = 0; i < activeSourceIds.size(); i++) {  //-- Stop streams
		if (activeSourceIds.at(i) == id || all) {
			bass_StreamFree(activeSourceStreams.at(i));
		};
	};
	for (int i = 0; i < activeSourceIds.size(); i++) {  //-- Then only delete, because we are erasing the vector that the for loop is using
		if (activeSourceIds.at(i) == id || all) {
			activeSourceIds.erase(activeSourceIds.begin() + i);
			activeSourceStreams.erase(activeSourceStreams.begin() + i);
			activeSourceVolumes.erase(activeSourceVolumes.begin() + i);
		};
	};
};
void volume(int id, float vol) {
	for (int i = 0; i < activeSourceIds.size(); i++) {
		if (activeSourceIds.at(i) == id) {
			bass_ChannelSetAttribute(activeSourceStreams.at(i), BASS_ATTRIB_VOL, vol);
			activeSourceVolumes.at(i) = vol;
		};
	};	
};
void update(int id, vector<double> pos, vector<double> dir) {
	for (int i = 0; i < activeSourceIds.size(); i++) {
		if (activeSourceIds.at(i) == id) {
			BASS_3DVECTOR posBASS(pos[0], pos[1], pos[2]);
			bass_ChannelSet3DPosition(activeSourceStreams.at(i), &posBASS, NULL, NULL);
		};
	};
	BASS_3DVECTOR posPlayer(0, 0, 0), velocity(0, 0, 0), dirPlayer(dir[0], dir[1], dir[2]), top(0, 0, -1);
	bass_Set3DPosition(&posPlayer, &velocity, &dirPlayer, &top);
	bass_Apply3D();
};
void mainThread() {
	main_working = true;
	while (main_working) {  //-- Run thread continuously
		while (instructionQueue.empty()) {usleep(100000);};  //-- Wait until new instruction exists
		
		//-- Grab and format arguments from queue (command, mode, id, url, vol, pos, dir)
		vector<string> parameter = instructionQueue.at(0);
		string command = parameter[0];
		int mode = stoi (parameter[1]);
		int id = stoi (parameter[2]);
		string url = parameter[3]; url.erase(0, 1); url.erase(url.size() - 1);
		float vol = stof(parameter[4]);
		vector<double> pos = stringToVector(parameter[5]); 
		vector<double> dir = stringToVector(parameter[6]);
		instructionQueue.pop_front();  //-- Remove instruction

		//-- Command caller switch
		if (command == "start") {start (mode, id, url, vol);
		} else if (command == "stop") {stop (id, false);	
		} else if (command == "stopAll") {stop (id, true);
		} else if (command == "volume") {volume (id, vol);
		} else if (command == "update") {update (id, pos, dir);};
	};
};

//-- ArmA stuff
extern "C" {	
	__attribute__((dllexport)) void __stdcall _RVExtension(char *output, int outputSize, const char *function);  //void __stdcall _RVExtension
	__attribute__((dllexport)) int __stdcall _RVExtensionArgs(char *output, int outputSize, const char *function, const char **argv, int argc);
	__attribute__((dllexport)) void __stdcall _RVExtensionVersion(char *output, int outputSize);
};
int __stdcall _RVExtensionArgs(char *output, int outputSize, const char *function, const char **argv, int argc) {
	//-- Assign parmeters to global variables
	vector<string> parameter;
	parameter.push_back(function);
	parameter.push_back(argv[0]);
	parameter.push_back(argv[1]);
	parameter.push_back(argv[2]);
	parameter.push_back(argv[3]);
	parameter.push_back(argv[4]);
	parameter.push_back(argv[5]);
	instructionQueue.push_back(parameter);

	//-- Get out of RVExtensionArgs as soon as possible
	if (!main_working) {  //-- If mainThread doesn't exist, make it		
		thread t(mainThread);
		t.detach();
	};
	
	return 0;
};
void __stdcall _RVExtensionVersion(char *output, int outputSize) {loadBass();};  //-- Link BASS.dll and initialize it
void __stdcall _RVExtension(char *output, int outputSize, const char *function) {};