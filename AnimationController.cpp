#include "AnimationController.h"
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")
using namespace Gdiplus;
using namespace std;

AnimaionController::AnimaionController()
{
	GdiplusStartupInput gdiplusStartupInput;

	GdiplusStartup
	(
		&gdiplusToken,
		&gdiplusStartupInput,
		NULL
	);
}

AnimaionController::~AnimaionController()
{
	for (auto& it : animations)
	{
		for (auto img : it.second.frames)
		{
			delete img;
		}
	}
	GdiplusShutdown(gdiplusToken);
}

void AnimaionController::LoadAnimations(const wstring& basePath)
{
	WIN32_FIND_DATA ffd;
	wstring searchPath = basePath + L"\\*";
	HANDLE hFind = FindFirstFile(searchPath.c_str(), &ffd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do
	{
		if (
			(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			&&
			wcscmp(ffd.cFileName, L".") != 0 
			&&
			wcscmp(ffd.cFileName, L"..") != 0)
		{

			wstring folderName = ffd.cFileName;
			wstring folderPath = basePath + L"\\" + folderName;
			LoadAnimationFromFolder(folderPath, folderName);
		}

	} while (FindNextFile(hFind, &ffd) != 0);
	
	FindClose(hFind);
}

void AnimaionController::LoadAnimationFromFolder(const wstring& folderPath,
	const wstring& animName)
{
	WIN32_FIND_DATA ffd;

	wstring searchPath = folderPath + L"\\*.png";
	HANDLE hFind = FindFirstFile(searchPath.c_str(), &ffd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	Animation anim;

	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			wstring filePath = folderPath + L"\\" + ffd.cFileName;
			anim.frames.push_back(new Image(filePath.c_str()));
		}


	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);

	if (!anim.frames.empty())
		animations[animName] = anim;


}

void AnimaionController::Play(const wstring& name)
{
	if (animations.find(name) != animations.end())
		currentAnim = name;
}

void AnimaionController::Update() {
	if (animations.find(currentAnim) == animations.end())
		return;

	tick++;

	Animation& anim = animations[currentAnim];

	if (tick >= anim.frameDelay) 
	{
		tick = 0;
		Animation& anim = animations[currentAnim];

		anim.currentFrame = 
			(anim.currentFrame + 1) % anim.frames.size();
	}
}

Gdiplus::Image* AnimaionController::GetCurrentFrame()
{
	if (animations.find(currentAnim) != animations.end())
	{
		return animations[currentAnim].frames[animations
			[currentAnim].currentFrame];
	}
	return nullptr;
}

