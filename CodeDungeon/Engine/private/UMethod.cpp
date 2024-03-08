#include "EngineDefine.h"
#include "UMethod.h"
#include <fstream>
#include "DirectXTK/ResourceUploadBatch.h"
#include "UBase.h"

using namespace Engine;

int Engine::UMethod::ConvertStrToNum(const _string& _var)
{
	return std::stoi(_var.c_str());
}

int Engine::UMethod::ConvertStrToNum(const _wstring& _var)
{
	return std::stoi(_var.c_str());
}

_string Engine::UMethod::ConvertWToS(const _wstring& var)
{
	_string str;
	size_t size;
	str.resize(var.size());
	wcstombs_s(&size, &str[0], str.size() + 1, var.c_str(), var.size());
	return str;
}

_wstring Engine::UMethod::ConvertSToW(const _string& var)
{
	_wstring wstr;
	size_t size;
	wstr.resize(var.size());
	mbstowcs_s(&size, wstr.data(), wstr.size() + 1, var.c_str(), var.size());
	return wstr;
}

_string Engine::UMethod::ConvertBigToSmall(const _string& _var)
{
	_string str = "";
	for (auto& iter : _var)
		str += (_char)((_int)iter + 32);
	return str;
}

_string Engine::UMethod::ConvertSmallToBig(const _string& _var)
{
	_string str = "";
	for (auto& iter : _var)
		str += (_char)((_int)iter - 32);
	return str;
}

_wstring Engine::UMethod::ConvertBigToSmall(const _wstring& _var)
{
	_wstring str = L"";
	for (auto& iter : _var)
		str += (_tchar)((_int)iter + 32);
	return str;
}

_wstring Engine::UMethod::ConvertSmallToBig(const _wstring& _var)
{
	_wstring str = L"";
	for (auto& iter : _var)
		str += (_tchar)((_int)iter - 32);
	return str;
}

const _bool Engine::UMethod::Is_Same_Text(const _string& _strOriginText, const _string& _strFindText)
{
	for (_uint k = 0; k < _strOriginText.length();)
	{
		_bool bIsTrue = false;
		for (_uint i = 0; i < _strFindText.length(); ++i)
		{
			if (false == bIsTrue)
			{
				if (_strFindText[i] == _strOriginText[k])
				{
					bIsTrue = true;
					++k;
				}
				else
					break;
			}
			else
			{
				if (_strFindText[i] == _strOriginText[k])
				{
					++k;
				}
				else
				{
					bIsTrue = false;
					break;
				}
			}
		}
		if (true == bIsTrue)
			return true;
		++k;
	}
	return false;
}

const _bool Engine::UMethod::Is_Same_Text(const _wstring& _strOriginText, const _wstring& _strFindText)
{
	for (_uint k = 0; k < _strOriginText.length();)
	{
		_bool bIsTrue = false;
		for (_uint i = 0; i < _strFindText.length(); ++i)
		{
			if (false == bIsTrue)
			{
				if (_strFindText[i] == _strOriginText[k])
				{
					bIsTrue = true;
					++k;
				}
				else
					break;
			}
			else
			{
				if (_strFindText[i] == _strOriginText[k])
				{
					++k;
				}
				else
				{
					bIsTrue = false;
					break;
				}
			}
		}
		if (true == bIsTrue)
			return true;
		++k;
	}
	return false;
}

_string Engine::UMethod::Find_Extension(const _string& _str)
{
	_string str = _str;
	_uint iIndex = (_uint)str.find_last_of(".");
	_string returnStr = str.substr(iIndex + 1, str.length());
	return returnStr;
}

_wstring Engine::UMethod::Find_Extension(const _wstring& _str)
{
	_wstring str = _str;
	_uint iIndex = (_uint)str.find_last_of(L".");
	_wstring returnStr = str.substr(iIndex + 1, str.length());
	return returnStr;
}

HRESULT UMethod::CreateBufferToUpLoadGpu(const ComPtr<Dx12Device>& _pDevice,
	const ComPtr<Dx12GraphicsCommandList>& _pGpuCmd, const _uint& _iBufferSize,
	const void* _pData, ComPtr<Dx12Resource>& _pGpu, ComPtr<Dx12Resource>& _pUpLoad)
{
	RETURN_CHECK(nullptr == _pDevice || nullptr == _pGpuCmd, E_FAIL);

	D3D12_HEAP_PROPERTIES GpuHeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_HEAP_PROPERTIES UpLoadHeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(_iBufferSize);

	// Gpu Resource ���� 
	RETURN_CHECK_FAILED(_pDevice->CreateCommittedResource(&GpuHeapProperty, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&_pGpu)), E_FAIL);

	// GPU Upload Resource ����
	RETURN_CHECK_FAILED(_pDevice->CreateCommittedResource(&UpLoadHeapProperty, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&_pUpLoad)), E_FAIL);

	D3D12_SUBRESOURCE_DATA tSubResourceData;
	tSubResourceData.pData = _pData;
	tSubResourceData.RowPitch = _iBufferSize;
	tSubResourceData.SlicePitch = _iBufferSize;

	// �׷��� Cmd ��� �غ�
	{
		// ������ ����
		CD3DX12_RESOURCE_BARRIER OpenBarrier = CD3DX12_RESOURCE_BARRIER::Transition(_pGpu.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		// Common -> CopyDesc�� ����
		_pGpuCmd->ResourceBarrier(1, &OpenBarrier);

		::UpdateSubresources<1>(_pGpuCmd.Get(), _pGpu.Get(),
			_pUpLoad.Get(), 0, 0, 1, &tSubResourceData);

		CD3DX12_RESOURCE_BARRIER CloseBarrier = CD3DX12_RESOURCE_BARRIER::Transition(_pGpu.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		_pGpuCmd->ResourceBarrier(1, &CloseBarrier);
	}

	return S_OK;
}

HRESULT UMethod::CreateBufferResource(const ComPtr<Dx12Device>& _pDevice,
	const ComPtr<Dx12GraphicsCommandList>& _pGpuCmd, const _uint& _iBufferSize,
	const void* _pData, ComPtr<Dx12Resource>& _pBuffer, ComPtr<Dx12Resource>& _pUpLoad,
	const D3D12_HEAP_TYPE& _d3dHeapType, const D3D12_RESOURCE_STATES _d3dResourceStates)
{
	D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc{ };
	::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
	d3dHeapPropertiesDesc.Type = _d3dHeapType;
	d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapPropertiesDesc.CreationNodeMask = 1;
	d3dHeapPropertiesDesc.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC d3dResourceDesc;
	::ZeroMemory(&d3dResourceDesc, sizeof(D3D12_RESOURCE_DESC));
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	d3dResourceDesc.Alignment = 0;
	d3dResourceDesc.Width = _iBufferSize;
	d3dResourceDesc.Height = 1;
	d3dResourceDesc.DepthOrArraySize = 1;
	d3dResourceDesc.MipLevels = 1;
	d3dResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	d3dResourceDesc.SampleDesc.Count = 1;
	d3dResourceDesc.SampleDesc.Quality = 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_RESOURCE_STATES d3dResourceInitialStates = D3D12_RESOURCE_STATE_COMMON;
	if (_d3dHeapType == D3D12_HEAP_TYPE_UPLOAD) d3dResourceInitialStates =
		D3D12_RESOURCE_STATE_GENERIC_READ;
	else if (_d3dHeapType == D3D12_HEAP_TYPE_READBACK) d3dResourceInitialStates =
		D3D12_RESOURCE_STATE_COPY_DEST;

	// Create Commited Resource
	RETURN_CHECK_DXOBJECT(_pDevice->CreateCommittedResource(&d3dHeapPropertiesDesc, D3D12_HEAP_FLAG_NONE,
		&d3dResourceDesc, d3dResourceInitialStates, NULL, IID_PPV_ARGS(_pBuffer.GetAddressOf())), E_FAIL);
	/*
	CreateCommittedResource -> �� �Լ��� ���� ���ø����̼��� GPU �޸𸮿� ���ҽ� �Ҵ��ϰ�, �ش� ���ҽ���
	�Ӽ��� �ʱ� ���¸� ������ �� �ֽ��ϴ�.
	*/
	// 
	// Data
	if (_pData) {
		// HeapType 
		switch (_d3dHeapType) {
		case D3D12_HEAP_TYPE_DEFAULT:
		{
			// ���ε� ���۸� �����Ѵ�. 
			d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
			_pDevice->CreateCommittedResource(&d3dHeapPropertiesDesc, D3D12_HEAP_FLAG_NONE,
				&d3dResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL,
				IID_PPV_ARGS(_pUpLoad.GetAddressOf()));
			// ���ε� ���۸� �����Ͽ� �ʱ�ȭ �����͸� ���ε� ���ۿ� �����Ѵ�. 
			D3D12_RANGE d3dReadRange{ 0, 0 };
			_ubyte* pBufferDataBegin{ NULL };
			// �����͸� �����´�. 
			_pUpLoad->Map(0, &d3dReadRange, (void**)&pBufferDataBegin);
			// �����͸� �����Ѵ�. 
			::memcpy(pBufferDataBegin, _pData, _iBufferSize);
			// �����͸� �ٽ� �ݾ��ش�. 
			_pUpLoad->Unmap(0, NULL);
			// ���ε� ������ ������ ����Ʈ ���ۿ� �����Ѵ�. 
			_pGpuCmd->CopyResource(_pBuffer.Get(), _pUpLoad.Get());
			// Resource Barrier
			D3D12_RESOURCE_BARRIER d3dResourceBarrier;
			::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
			d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			d3dResourceBarrier.Transition.pResource = _pBuffer.Get();
			d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			d3dResourceBarrier.Transition.StateAfter = _d3dResourceStates;
			d3dResourceBarrier.Transition.Subresource =
				D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			_pGpuCmd->ResourceBarrier(1, &d3dResourceBarrier);
		}
		break;
		case D3D12_HEAP_TYPE_UPLOAD:
		{
			// ���ε� ���۸� �����Ͽ� �ʱ�ȭ �����͸� ���ε� ���ۿ� �����Ѵ�. 
			D3D12_RANGE d3dReadRange{ 0, 0 };
			_ubyte* pBufferDataBegin{ NULL };
			// �����͸� �����´�. 
			_pBuffer->Map(0, &d3dReadRange, (void**)&pBufferDataBegin);
			// �����͸� �����Ѵ�. 
			::memcpy(pBufferDataBegin, _pData, _iBufferSize);
			// �����͸� �ٽ� �ݾ��ش�. 
			_pBuffer->Unmap(0, NULL);
		}
		break;
		}
	}
	return S_OK;
}

_uint UMethod::CalcConstantBufferByteSize(const _uint& _iByteSize)
{
	// Constant buffers must be a multiple of the minimum hardware
	// allocation size (usually 256 bytes).  So round up to nearest
	// multiple of 256.  We do this by adding 255 and then masking off
	// the lower 2 bytes which store all bits < 256.
	// Example: Suppose byteSize = 300.
	// (300 + 255) & ~255
	// 555 & ~255
	// 0x022B & ~0x00ff
	// 0x022B & 0xff00
	// 0x0200
	// 512
	return (_iByteSize + 255) & ~255;
}

ComPtr<DxBlob> UMethod::LoadBinary(const _wstring& _strFileName)
{
	std::ifstream fin(_strFileName.c_str(), std::ios::binary);
	if (!fin) return nullptr;

	fin.seekg(0, std::ios_base::end);
	std::ifstream::pos_type size = (_int)(fin.tellg());
	fin.seekg(0, std::ios_base::beg);

	ComPtr<DxBlob> Blob;
	RETURN_CHECK_FAILED(D3DCreateBlob(size, &Blob), nullptr);

	fin.read((_char*)Blob->GetBufferPointer(), size);
	fin.close();
	return Blob;
}

void UMethod::SaveString(std::ofstream& _os, const _string& _str) {
	int size = (int)_str.length() + 1;
	_os.write((char*)&size, sizeof(int));
	_os.write((char*)_str.c_str(), size);
}

void UMethod::ReadString(std::ifstream& _if, _wstring& _wstr) {

	int size = 0;
	_if.read((char*)&size, sizeof(int));
	char* pText = new char[size];
	_if.read((char*)pText, size);
	_wstr = ConvertSToW(pText);
	Safe_Delete_Array(pText);
}

void Engine::UMethod::SaveString(std::ofstream& _os, const _wstring& _wstr)
{
	SaveString(_os, ConvertWToS(_wstr));
}

void Engine::UMethod::ReadString(std::ifstream& _if, _string& _str)
{
	_wstring wstr;
	ReadString(_if, wstr);
	_str = ConvertWToS(wstr);
}

void Engine::UMethod::SaveString(HANDLE& _hFile, DWORD& _bytes, BOOL& _bTrue, const _wstring& _str)
{
	_uint SIZE = (_uint)_str.size();
	_bTrue = WriteFile(_hFile, &SIZE, sizeof(_uint), &_bytes, nullptr);
	_bTrue = WriteFile(_hFile, _str.c_str(), sizeof(_tchar) * SIZE, &_bytes, nullptr);
}

void Engine::UMethod::ReadString(HANDLE& _hFile, DWORD& _bytes, BOOL& _bTrue, _wstring& _str)
{
	_uint SIZE = 0;
	_bTrue = ReadFile(_hFile, &SIZE, sizeof(_uint), &_bytes, nullptr);
	_tchar* pNewChar = new _tchar[SIZE];
	_bTrue = ReadFile(_hFile, pNewChar, sizeof(_tchar) * SIZE, &_bytes, nullptr);
	_str = pNewChar;
	Safe_Delete_Array(pNewChar);
}

void Engine::UMethod::SaveString(HANDLE& _hFile, DWORD& _bytes, BOOL& _bTrue, const _string& _str)
{
	_uint SIZE = (_uint)_str.size();
	_bTrue = WriteFile(_hFile, &SIZE, sizeof(_uint), &_bytes, nullptr);
	_bTrue = WriteFile(_hFile, _str.c_str(), sizeof(_char) * SIZE, &_bytes, nullptr);
}

void Engine::UMethod::ReadString(HANDLE& _hFile, DWORD& _bytes, BOOL& _bTrue, _string& _str)
{
	_uint SIZE = 0;
	_bTrue = ReadFile(_hFile, &SIZE, sizeof(_uint), &_bytes, nullptr);
	_char* pNewChar = new _char[SIZE];
	_bTrue = ReadFile(_hFile, pNewChar, sizeof(_char) * SIZE, &_bytes, nullptr);
	_str = pNewChar;
	Safe_Delete_Array(pNewChar);
}

_string Engine::UMethod::OutClassName(UBase* _spBase)
{
	_string str = typeid(*_spBase).name();
	str = str.substr(str.find("::") + 2, str.length());
	return str;
}

void Engine::UMethod::ChangeDepthStencilInfo(const DEPTH_STENCIL_TYPE& _eType, D3D12_DEPTH_STENCIL_DESC& _tDesc)
{
	switch (_eType)
	{
	case DEPTH_STENCIL_TYPE::LESS:
		_tDesc.DepthEnable = TRUE;
		_tDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DEPTH_STENCIL_TYPE::LESS_EQUAL:
		_tDesc.DepthEnable = TRUE;
		_tDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::GREATER:
		_tDesc.DepthEnable = TRUE;
		_tDesc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
		break;
	case DEPTH_STENCIL_TYPE::GREATER_EQUAL:
		_tDesc.DepthEnable = TRUE;
		_tDesc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::NO_DEPTH_TEST:
		_tDesc.DepthEnable = FALSE;
		_tDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE:
		_tDesc.DepthEnable = FALSE;
		_tDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		break;
	case DEPTH_STENCIL_TYPE::LESS_NO_WRITE:
		_tDesc.DepthEnable = TRUE;
		_tDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		_tDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		break;
	}
}

ComPtr<Dx12CommandQueue> Engine::UMethod::CreateCommandQueue(const ComPtr<Dx12Device>& _cpDevice, const D3D12_COMMAND_LIST_TYPE& _eType)
{
	ComPtr<Dx12CommandQueue> cpCommandQueue;

	D3D12_COMMAND_QUEUE_DESC GraphicsCommandQueue{};
	GraphicsCommandQueue.Type = _eType;
	GraphicsCommandQueue.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	RETURN_CHECK_DXOBJECT(_cpDevice->CreateCommandQueue(&GraphicsCommandQueue,
		IID_PPV_ARGS(&cpCommandQueue)), nullptr);

	return cpCommandQueue;
}
