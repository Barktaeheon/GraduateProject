#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UACTOR_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UACTOR_H

#include "UCloneObject.h"

BEGIN(Engine)
class UComponent;
class UTransform;
class UResource;
class UCommand;
class UTableDescriptor;

class ENGINE_DLL UActor abstract : public UCloneObject {
public:
	UActor(CSHPTRREF<UDevice> _spDevice,	const _wstring& _wstrLayer,
		const CLONETYPE& _eCloneType, const BACKINGTYPE _eBackingType, 
		const USECOLLISIONTYPE _eCollType = USECOLLISIONTYPE::DISABLE);
	UActor(const UActor& _rhs);
	DESTRUCTOR(UActor)
public:
	void SetActive(const _bool  _isActvie);
	void SetTickActive(const _bool _isActvie);
	void SetRenderActive(const _bool _isActvie);
	void SetParentsActor(CSHPTRREF<UActor> _spActor);
	const _bool IsActive() const {return m_isActive;	}
	const _bool IsTickActive() const { return m_isTickActive; }
	const _bool IsRenderActive() const { return m_isRenderActive; }

	CSHPTRREF<UTransform> GetTransform() const { return m_spTranfsorm; }
	CSHPTRREF<const CLONEDATA> GetCloneData() const { return m_spCloneData; }

	SHPTR<UComponent> GetComponent(const _wstring& _wstrTag);
	SHPTR<UResource> GetResource(const _wstring& _wstrTag);

	const BACKINGTYPE GetBackingType() const { return m_eBackingType; }
	const USECOLLISIONTYPE GetUseCollisionType() const { return m_eUseCollType; }

	// UObject을(를) 통해 상속됨
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _stDatas) override PURE;
	virtual void Free() override PURE;
	virtual HRESULT NativeConstruct() override PURE;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _stDatas) override PURE;

	void AwakeTick(const _double& _dTimeDelta) { (this->*m_pAwakeTick)(_dTimeDelta); }
	void Tick(const _double& _dTimeDelta) {(this->*m_pTick)(_dTimeDelta);}
	void LateTick(const _double& _dTimeDelta) {	(this->*m_pLateTick)(_dTimeDelta);}
	HRESULT Render(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) {	return (this->*m_pRender)(_spCommand, _spTableDescriptor);	}


protected:
	void AwakeTickNonActive(const _double& _dTimeDelta){}
	void TickNonActive(const _double& _dTimeDelta) {}
	void LateTickNonActive(const _double& _dTimeDelta) {}
	HRESULT RenderNonActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) { return S_OK; }

	template<class T>
	SHPTR<T> AddResource(const _wstring& _wstrProtoTag, const _wstring& _wstrTag,
		const VOIDDATAS& _vecDatas = VOIDDATAS{})
	{
		SHPTR<UResource> RESOURCE = nullptr;
		CreateResource(_wstrProtoTag, _wstrTag, RESOURCE, _vecDatas);
		return static_pointer_cast<T>(RESOURCE);
	}
	template<class T>
	SHPTR<T> AddComponent(const _wstring& _wstrProtoTag, const _wstring& _wstrTag, const VOIDDATAS& _vecDatas = VOIDDATAS{})
	{
		SHPTR<UComponent> COMP = nullptr;
		CreateComponent(_wstrProtoTag, _wstrTag, COMP, _vecDatas);
		return static_pointer_cast<T>(COMP);
	}

	virtual  void AwakeTickActive(const _double& _dTimeDelta);
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) PURE;
	virtual void LateTickActive(const _double& _dTimeDelta) PURE;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) PURE;

private:
	HRESULT ReadyTransform();
	HRESULT CreateResource(const _wstring& _wstrProtoTag,
		const _wstring& _strResTag, SHPTR<UResource>& _pResource, const VOIDDATAS& _tDatas = VOIDDATAS{});
	HRESULT CreateComponent(const _wstring& _strProtoTag, const _wstring& _wstrTag,
		SHPTR<UComponent>& _pComponent, const VOIDDATAS& _tDatas = VOIDDATAS{});
	HRESULT ReadyCloneType(const _wstring& _strCloneLayer, const CLONETYPE _eCloneType);
#ifdef _USE_IMGUI
public:
	virtual void ShowObjectInfo() override;
#endif
private:
	using  COMPONENTS = UNORMAP<_wstring, SHPTR<UComponent>>;
	using	RESOURCES = UNORMAP<_wstring, SHPTR<UResource>>;
	// Containers
	COMPONENTS										m_Components;
	RESOURCES											m_Resources;
	// Parents
	WKPTR<UActor>									m_wpParentsActor;
	SHPTR<UTransform>							m_spTranfsorm;
	//Clone Data 
	SHPTR<const CLONEDATA>				m_spCloneData;
	// Active Series
	_bool														m_isActive;
	_bool														m_isTickActive;
	_bool														m_isRenderActive;
	// Active And NonActive Methods
	void															(UActor::* m_pAwakeTick)(const _double&);
	void															(UActor::* m_pTick)(const _double&);
	void															(UActor::* m_pLateTick)(const _double&);
	HRESULT												(UActor::* m_pRender)(CSHPTRREF<UCommand>, CSHPTRREF<UTableDescriptor>);

	// Backing Type
	BACKINGTYPE										m_eBackingType;
	USECOLLISIONTYPE							m_eUseCollType;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UACTOR_H