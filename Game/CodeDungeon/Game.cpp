#include "stdafx.h"
#include "Game.h"
#include "Engine.h"

void Game::Init(HINSTANCE hInstance, HWND hWnd)
{
	gGameFrameWork->OnCreate(hInstance,hWnd);
	gGameFrameWork->BuildObjects();
}

void Game::Update()
{
	//2024-01-07 �̼���
	//Tool������ ������ ������ RenderBegin�� RenderEnd ���̿� �����Ű�� ����
	//Render�� RenderBegin, RenderEnd�� �и�.
	gGameFrameWork->RenderBegin();
	gGameFrameWork->Render();
	gGameFrameWork->RenderEnd();
}

void Game::Destroy()
{
	gGameFrameWork->OnDestroy();
}

LRESULT CALLBACK Game::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	return gGameFrameWork->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
}

void Game::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	gGameFrameWork->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
}

void Game::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	gGameFrameWork->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
}

