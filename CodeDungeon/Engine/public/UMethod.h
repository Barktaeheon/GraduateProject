#pragma once
namespace Engine
{
	class  UMethod {
	public:

		static int ConvertStrToNum(const _string& _var);
		static int ConvertStrToNum(const _wstring& _var);
		// Wstring�� string���� ��ȯ
		static _string ConvertWToS(const _wstring& var);
		// String�� Wstring���� ��ȯ
		static _wstring ConvertSToW(const _string& var);
		// �빮�ڿ��� �ҹ��ڷ� ��ȯ
		static _string ConvertBigToSmall(const _string& _var);
		// �ҹ��ڿ��� �빮�ڷ� ��ȯ
		static _string ConvertSmallToBig(const _string& _var);
		// �빮�ڿ��� �ҹ��ڷ� ��ȯ
		static _wstring ConvertBigToSmall(const _wstring& _var);
		// �ҹ��ڿ��� �빮�ڷ� ��ȯ
		static _wstring ConvertSmallToBig(const _wstring& _var);
		// ���� �̸� ã��(��ҹ��� ����) (Origin ��� ���ڿ�, Find �Է� ���ڿ�)
		static const _bool Is_Same_Text(const _string& _strOriginText, const _string& _strFindText);
		// ���� �̸� ã��(��ҹ��� ����) (Origin ��� ���ڿ�, Find �Է� ���ڿ�)
		static const _bool Is_Same_Text(const _wstring& _strOriginText, const _wstring& _strFindText);
		// .(���� ���ڿ��� ������)
		static _string Find_Extension(const _string& _str);
		// .(���� ���ڿ��� ������)
		static _wstring Find_Extension(const _wstring& _str);

		template<typename T>
		static void ClearToMemory(T& _value) { ZeroMemory(&_value, sizeof(T)); }
		template<typename T>
		static void ClearToMemory(T* _value) { ZeroMemory(_value, sizeof(T)); }

		static HRESULT CreateBufferToUpLoadGpu(const ComPtr<Dx12Device>& _pDevice, const ComPtr<Dx12GraphicsCommandList>& _pGpuCmd,
			const _uint& _iBufferSize, const void* _pData, ComPtr<Dx12Resource>& _pGpu, ComPtr<Dx12Resource>& _pUpLoad);

		static HRESULT CreateBufferResource(const ComPtr<Dx12Device>& _pDevice, const ComPtr<Dx12GraphicsCommandList>& _pGpuCmd,
			const _uint& _iBufferSize, const void* _pData, ComPtr<Dx12Resource>& _pBuffer, ComPtr<Dx12Resource>& _pUpLoad,
			const D3D12_HEAP_TYPE& _d3dHeapType = D3D12_HEAP_TYPE_UPLOAD,
			const D3D12_RESOURCE_STATES _d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		static _uint CalcConstantBufferByteSize(const _uint& _iByteSize);
		// Shader ������ Binary�� �о ����
		static ComPtr<DxBlob> LoadBinary(const _wstring& _strFileName);

		// void �����͸� T�� ĳ�����Ͽ� ����
		// VOIDDATAS(vector<void*>)�� �ִ� ���� �� �� ���� �����Ѵ�. 
		// VOIDDATAS(vector<void*>)�� �ִ� ���� �ε����� ��ȯ�Ͽ� ����
		template<typename T>
		static T ConvertTemplate_Index(const VOIDDATAS& _pDatas, const _uint& _iIndex);

		static void SaveString(std::ofstream& _os, const _string& _str);
		static void ReadString(std::ifstream& _if, _wstring& _wstr);
		static void ReadStringUnity(std::ifstream& _if, _wstring& _wstr);

		static void SaveString(std::ofstream& _os, const _wstring& _wstr);
		static void ReadString(std::ifstream& _if, _string& _str);
		static void ReadStringUnity(std::ifstream& _if, _string& _str);
		/*
		@ Date: 2024-02-25, Writer: ������
		@ Explain
		-  Save�� ���ϰ� ���� ���� �Լ�ȭ 
		*/
		template<class T>
		requires IsNotPointer<T>
		static void SaveOther(std::ofstream& _os, T*  _Others);
		/*
		@ Date: 2024-02-25, Writer: ������
		@ Explain
		-  Read�� ���� ���� �Լ�ȭ
		*/
		template<class T>
		requires IsNotPointer<T>
		static void ReadOther(std::ifstream& _if, T* _Others);


		// Read Save WString
		static void SaveString(HANDLE& _hFile, DWORD& _bytes, BOOL& _bTrue, const _wstring& _str);
		static void ReadString(HANDLE& _hFile, DWORD& _bytes, BOOL& _bTrue, _wstring& _str);
		// Read Save String
		static void SaveString(HANDLE& _hFile, DWORD& _bytes, BOOL& _bTrue, const _string& _str);
		static void ReadString(HANDLE& _hFile, DWORD& _bytes, BOOL& _bTrue, _string& _str);

		// OutClassName
		static _string OutClassName(class UBase* _spBase);

		template<class T1, class T2>
			requires CheckNumber<T1> || CheckNumber<T2> || CheckAtomicNumber<T1> || CheckAtomicNumber<T2>
		static void CheckOverNumbers(T1 & _number, const T2 & _CheckNumber);

		template<class T1, class T2>
			requires CheckNumber<T1> || CheckNumber<T2> || CheckAtomicNumber<T1> || CheckAtomicNumber<T2>
		static void CheckOverNumbers(T1 & _number, const T2 & _CheckNumber, const T2 & _ResetNumber);

		static void ChangeDepthStencilInfo(const DEPTH_STENCIL_TYPE& _eType, D3D12_DEPTH_STENCIL_DESC& _tDesc);

		static ComPtr<Dx12CommandQueue> CreateCommandQueue(const ComPtr<Dx12Device>& _cpDevice, const D3D12_COMMAND_LIST_TYPE& _eType);
		static _wstring MakeFolderAndReturnPath(const _wstring& _wstrPath, const _wstring& _wstrFolderName);
		static _wstring MakePath(const _wstring& _wstrPath, const _wstring& _wstrFolderName);
	};

	template<typename T>
	inline T UMethod::ConvertTemplate_Index(const VOIDDATAS& _pDatas, const _uint& _iIndex)
	{
		if (_pDatas.size() <= 0)
			return T();

		if (_pDatas.size() <= _iIndex)
			return T();

		T* data = static_cast<T*>((_pDatas)[_iIndex]);
		return *data;
	}

	template<class T>
		requires IsNotPointer<T>
	inline void UMethod::SaveOther(std::ofstream& _os, T* _Others)
	{
		_os.write((_char*)_Others, sizeof(T));
	}

	template<class T>
		requires IsNotPointer<T>
	inline void UMethod::ReadOther(std::ifstream& _if, T* _Others)
	{
		_if.read((_char*)_Others, sizeof(T));
	}

	template<class T1, class T2>
		requires CheckNumber<T1> || CheckNumber<T2> || CheckAtomicNumber<T1> || CheckAtomicNumber<T2>
	inline void UMethod::CheckOverNumbers(T1 & _number, const T2 & _CheckNumber)
	{
		if (_number >= _CheckNumber)
		{
			_number = 0;
		}
	}

	template<class T1, class T2>
		requires CheckNumber<T1> || CheckNumber<T2> || CheckAtomicNumber<T1> || CheckAtomicNumber<T2>
	inline void UMethod::CheckOverNumbers(T1 & _number, const T2 & _CheckNumber, const T2 & _ResetNumber)
	{
		if (_number >= _CheckNumber)
		{
			_number = _ResetNumber;
		}
	}

}