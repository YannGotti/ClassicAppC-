#include "framework.h"
#include "ClassicApp.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <memory>

#include "Character.h"
#include "Camera.h"
#include "GameWorld.h"
#include "CollisionManager.h"
#include "EntityController.h"

using namespace Gdiplus;
using namespace std;

// ----------------------------------------------------------
// Глобальные переменные
// ----------------------------------------------------------
#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING] = L"MegaGame";
WCHAR szWindowClass[MAX_LOADSTRING] = L"MegaGameClass";
static ULONGLONG lastTick = 0;
// ----------------------------------------------------------
// Основные игровые объекты
// ----------------------------------------------------------
unique_ptr<GameWorld> world;

// ----------------------------------------------------------
// Прототипы
// ----------------------------------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);

// ----------------------------------------------------------
// Главная функция WinAPI
// ----------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE,
    _In_ LPWSTR,
    _In_ int nCmdShow)
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    MyRegisterClass(hInstance);
    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
}

// ----------------------------------------------------------
// Регистрация окна
// ----------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    return RegisterClassExW(&wcex);
}

// ----------------------------------------------------------
// Инициализация
// ----------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 1280, 720,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

// ----------------------------------------------------------
// Оконная процедура
// ----------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        
        // Создаём мир
        world = make_unique<GameWorld>(1280, 720);
        world->InitAssets(L"Assets/Player");
        world->SetWorldBounds(0.f, 0.f, 16000, 16000); // ограничение камеры
        SetTimer(hWnd, 1, 16, NULL); // ~60 FPS
    }
    break;

    case WM_TIMER: 
    {
        ULONGLONG currentTick = GetTickCount64();
        float deltaTime = 0.0f;
        if (lastTick != 0)
            deltaTime = (currentTick - lastTick) / 1000.0f; // миллисекунды → секунды

        lastTick = currentTick;

        world->Update(deltaTime);
        InvalidateRect(hWnd, NULL, FALSE);
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rect;
        GetClientRect(hWnd, &rect);

        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
        HGDIOBJ oldBmp = SelectObject(memDC, memBitmap);

        Graphics g(memDC);
        g.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
        g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeNone);
        g.SetSmoothingMode(Gdiplus::SmoothingModeNone);
        g.Clear(Color(180, 200, 230));

        world->Draw(g, rect);

        BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
        SelectObject(memDC, oldBmp);
        DeleteObject(memBitmap);
        DeleteDC(memDC);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        KillTimer(hWnd, 1);
        EntityController::SaveEntities();
        world.reset();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
