#include <Windows.h>
#include "DX3DManager.h"
#include "Framework.h"
#include "FBX.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ObjectManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "Camera.h"
#include "CameraManager.h"
#include "resource.h"
#include "Image.h"
#include "Box.h"
#include "DX2DManager.h"
#include "FontText.h"
#include "InputManager.h"

#pragma comment(lib, "dxgi.lib")

namespace {
	inline bool canShowBoxWindow_ = false;
}

namespace MochinekoEngine {
	inline HWND mainWindowHandle_ = {};
	inline bool canShutdown_ = false;

	HWND GetGameWindowHandle() {
		return mainWindowHandle_;
	}

	bool CanShutdown() {
		return canShutdown_;
	}

	void Shutdown() {
		canShutdown_ = true;
	}
}

using namespace MochinekoEngine;
using namespace DX3DManager;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitImGUI();
void DrawDebugImGUI();
void DrawCreateBox();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	InitDX3D();
	InitImGUI();
	InputManager::Init(hInstance, MochinekoEngine::GetGameWindowHandle());
	SoundManager::Init();
	ObjectManager::Init();
	SceneManager::Init();
	CameraManager::Init();
	DX2DManager::Init();

	//const std::string fontName, const int fontSize, const std::string& text, const Color& color) 
	FontText* text = new FontText(L"メイリオ", 30, L"ああ", {0.0f, 0.0f, 0.0f, 1.0f});
	text->Init();


	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (MochinekoEngine::CanShutdown()) {
			break;
		}

		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			ID3D11RenderTargetView* renderTargetView = GetRTV();

			float BACKGROUND_COLOR[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			EnableZDepthWrite();
			GetDeviceContext()->ClearRenderTargetView(renderTargetView,  BACKGROUND_COLOR);
			GetDeviceContext()->ClearDepthStencilView(GetDepthView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

			ImGuiIO& io = ImGui::GetIO();
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			InputManager::Update();
			SceneManager::Update();
			ObjectManager::Update();

			text->Draw();

			DrawDebugImGUI();
			DrawCreateBox();
			SoundManager::DrawDebugImGUI();

			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}

			GetSwapChain()->Present(1, 0);
		}
	}

	return 0;
}

void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEXW wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEXW);
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = L"Game";
	wndClass.lpfnWndProc = WndProc;
	wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);
	RegisterClassExW(&wndClass);

	mainWindowHandle_ = CreateWindowW(
		L"Game",
		L"GameTitle",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1280, 720,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(mainWindowHandle_, nCmdShow);
	UpdateWindow(mainWindowHandle_);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam)) {
		return true;
	}

	switch (message) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case ID_40001: {
					char fileNameBuffer[MAX_PATH] = {};
					OPENFILENAMEA openFileName = {};
					openFileName.lStructSize = sizeof(OPENFILENAMEA);
					openFileName.hwndOwner = MochinekoEngine::GetGameWindowHandle();
					openFileName.lpstrFilter = "FBXファイル(.fbx)\0*.fbx\0画像ファイル(.png)\0*.png\0音声ファイル(.wav、.mp3)\0*.wav;*.mp3\0";
					openFileName.lpstrFile = fileNameBuffer;
					openFileName.nFilterIndex = 1;
					openFileName.nMaxFile = MAX_PATH;
					openFileName.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
					if (GetOpenFileNameA(&openFileName)) {
						std::string fileNameString = fileNameBuffer;
						if (fileNameString.ends_with(".fbx")) {
							FBX* fbx = new FBX(fileNameBuffer);
							ObjectManager::AddObject(fbx);
						}
						else if (fileNameString.ends_with(".png")) {
							Image* image = new Image(fileNameBuffer);
							ObjectManager::AddObject(image);
						}
						else if (fileNameString.ends_with(".wav") || fileNameString.ends_with(".mp3")) {
							SoundManager::Load(fileNameBuffer);
							SoundManager::Play(fileNameString);
						}
					}

					break;
				}
				case ID_40002: {
					MochinekoEngine::Shutdown();
					break;
				}
				case ID_40003: {
					canShowBoxWindow_ = true;
					break;
				}
				default:
					break;
			}
		}
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void InitImGUI() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(mainWindowHandle_);
	ID3D11Device* device = (ID3D11Device*)GetDevice();
	ID3D11DeviceContext* deviceContext = (ID3D11DeviceContext*)GetDeviceContext();
	ImGui_ImplDX11_Init(device, deviceContext);

	io.Fonts->AddFontFromFileTTF(
		"C:/Windows/Fonts/meiryo.ttc",
		18.0f,
		nullptr,
		io.Fonts->GetGlyphRangesJapanese()
	);
}

void DrawDebugImGUI() {
	BaseScene* currentScene = SceneManager::GetCurrentScene();
	Camera* currentCamera = CameraManager::GetCurrentCamera();
	auto leftTilt = InputManager::GetControllerTiltLeft();

	ImGui::Begin("DebugInfo");
	ImGui::Text("Mouse Left: %d, Mouse Center: %d, Mouse Right: %d", InputManager::CheckPushMouse(0), InputManager::CheckPushMouse(1), InputManager::CheckPushMouse(2));
	ImGui::Text("Mouse Left: %d, Mouse Center: %d, Mouse Right: %d", InputManager::CheckDownMouse(0), InputManager::CheckDownMouse(1), InputManager::CheckDownMouse(2));
	ImGui::Text("Xbox %2.2f, %2.2f", InputManager::GetControllerLeftTrigger(), InputManager::GetControllerRightTrigger());
	ImGui::Text("XBox Tilt: X[%2.2f], Y[%2.2f]", leftTilt.x, leftTilt.y);
	ImGui::Text("Xbox A Key: %d", InputManager::IsControllerButtonPush(XINPUT_GAMEPAD_A));
	if (ImGui::Button("Start")) {
		InputManager::StartControllerVibration(65535, 65535);
	}
	if (ImGui::Button("Stop")) {
		InputManager::StopControllerVibration();
	}
	ImGui::Text("CurrentScene: %s", currentScene == nullptr ? "(nullptr)" : currentScene->GetName().c_str());
	ImGui::Text("ObjectCount: %d", ObjectManager::GetAllObject().size());
	ImGui::Text("CurrentCamera: %s", currentCamera == nullptr ? "(nullptr)" : currentCamera->GetName().c_str());
	ImGui::SliderFloat("Camera Postion X", &currentCamera->postion_.x, 0.0f, 10.0f);
	ImGui::SliderFloat("Camera Postion Y", &currentCamera->postion_.y, 0.0f, 10.0f);
	ImGui::SliderFloat("Camera Postion Z", &currentCamera->postion_.z, 0.0f, 10.0f);
	ImGui::SliderFloat("Camera Target X", &currentCamera->target_.x, 0.0f, 10.0f);
	ImGui::SliderFloat("Camera Target Y", &currentCamera->target_.y, 0.0f, 10.0f);
	ImGui::SliderFloat("Camera Target Z", &currentCamera->target_.z, 0.0f, 10.0f);

	ImGui::End();
}

void DrawCreateBox() {
	if (canShowBoxWindow_) {
		bool showModel = false;

		if (!showModel) {
			ImGui::OpenPopup("BoxCreate");
			showModel = true;
		}

		if (ImGui::BeginPopupModal("BoxCreate")) {
			static int width, height = 0;
			static float color[4] = {};
			ImGui::InputInt("Width", &width);
			ImGui::InputInt("Height", &height);
			ImGui::ColorPicker4("Color", color);
				
			if (ImGui::Button("Create")) {
				ImGui::CloseCurrentPopup();
				canShowBoxWindow_ = false;
				Box* box = new Box(width, height, {color[0], color[1], color[2], color[3]});
				ObjectManager::AddObject(box);
			}

			ImGui::EndPopup();
		}
	}
}
