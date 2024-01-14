#pragma once
#include "Timer.h"
#include "Shader.h"

class CbvSrvUavDescriptorHeap;
class CTexture;

class CScene
{
public:
	CScene();
	~CScene();
	//������ ���콺�� Ű���� �޽����� ó���Ѵ�. 
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);
	void BuildObjects(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList,const ComPtr<ID3D12RootSignature>& _RootSignature);
	void ReleaseObjects();
	bool ProcessInput(UCHAR* pKeysBuffer);
	void AnimateObjects(float fTimeElapsed);
	void Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, CCamera* pCamera);
	void ReleaseUploadBuffers();
	virtual void UpdateShaderVariables(const ComPtr<ID3D12GraphicsCommandList>& _CommandList) {}
	virtual void ReleaseShaderVariables() {}
	//�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�. 
	
protected:
	//���� ���̴����� �����̴�. ���̴����� ���� ��ü���� �����̴�.
	vector<shared_ptr<CGameObject>> m_ppObjects;

public:
public:
	static unique_ptr<CbvSrvUavDescriptorHeap> m_pDescriptorHeap;


	static void CreateCbvSrvDescriptorHeaps(const ComPtr<ID3D12Device>& _Device, int nConstantBufferViews, int nShaderResourceViews);
	static void CreateConstantBufferViews(const ComPtr<ID3D12Device>& _Device, int nConstantBufferViews, const ComPtr<ID3D12Resource>& _ConstantBuffers, UINT nStride);
	static D3D12_GPU_DESCRIPTOR_HANDLE CreateConstantBufferView(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12Resource>& _ConstantBuffer, UINT nStride);
	static D3D12_GPU_DESCRIPTOR_HANDLE CreateConstantBufferView(const ComPtr<ID3D12Device>& _Device, D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress, UINT nStride);
	static void CreateShaderResourceViews(const ComPtr<ID3D12Device>& _Device, CTexture* pTexture, UINT nDescriptorHeapIndex, UINT nRootParameterStartIndex);
	static void CreateShaderResourceView(const ComPtr<ID3D12Device>& _Device, CTexture* pTexture, int nIndex, UINT nRootParameterStartIndex);
	static void CreateShaderResourceView(const ComPtr<ID3D12Device>& _Device, CTexture* pTexture, int nIndex);
};