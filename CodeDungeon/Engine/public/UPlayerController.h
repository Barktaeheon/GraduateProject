#pragma once
#include "UController.h"

BEGIN(Engine)
/*
@ Date: 2024-04-28, Writer: ������
@ Explain
-  ������� �Է��� �޾Ƽ� �����̴� Ŭ����
*/
class UPlayerController abstract : public UController  {
public:
	// ���� ��ǲ�� ����
	enum class INPUTTYPE { DOWN, UP, PRESSING, INPUTTYPE_END };
public:
	struct INPUTDESC
	{
		INPUTTYPE		eInputType;
		_ubyte				ubInputKey;

		INPUTDESC() : eInputType{ INPUTTYPE::INPUTTYPE_END}, ubInputKey{0}{}
		// �ּҺ�
		_bool operator==(const INPUTDESC& _inputDesc) { return this == &_inputDesc; }
		_bool operator!=(const INPUTDESC& _inputDesc) { return this != &_inputDesc; }
	};
public:
	UPlayerController(CSHPTRREF<UDevice> _spDevice, const _int _iKeyLength);
	UPlayerController(const UPlayerController& _rhs);
	virtual ~UPlayerController() = default;
public:
	virtual void Free() override PURE;
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) PURE;
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override  PURE;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override  PURE;
	// Tick
	virtual void Tick(UCharacter* _pCharacter, const _double& _dTimeDelta) override  PURE;
	// InputTriggerData
	virtual _bool InputTriggerData(UCharacter* _pCharacter, const _int _Data) override  PURE;
protected:
	// CheckInput
	virtual void CheckInput(const INPUTDESC _InputDesc) PURE;
private:
	VECTOR<_bool>			m_PressingKeyContainer;
	VECTOR<_bool>			m_DownKeyContainer;
	VECTOR<_bool>			m_UpKeyContainer;
	// KeyLength
	_int									m_iKeyLength;
	// Desc
	SET<INPUTDESC>		m_InputContainer;
};

END