#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <map>
#include <vector>

#pragma comment (lib, "gdiplus.lib")

using namespace std;

struct Animation {
	vector<Gdiplus::Image*> frames;
	int currentFrame = 0;
	int frameDelay = 10;
};

class AnimaionController
{
public:
	AnimaionController();
	~AnimaionController();

	void LoadAnimations(const wstring& basePath);

	void Play(const wstring& name);

	void Update();

	Gdiplus::Image* GetCurrentFrame();

	Animation* GetCurrentAnimation() { return &animations[currentAnim]; }

private:
	map<wstring, Animation> animations;
	wstring currentAnim;

	int tick = 0;
	int frameDelay = 15;

	ULONG_PTR gdiplusToken;

	void LoadAnimationFromFolder(
		const wstring& folderPath,
		const wstring& animName
	);


};
