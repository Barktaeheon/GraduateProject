#pragma once
#include "ToolDefines.h"
#include "Scene.h"

class ToolGridObject;

class ToolScene : public CScene
{
public:
	virtual void BuildObjects(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const ComPtr<ID3D12RootSignature>& _RootSignature);
};

