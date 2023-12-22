#ifndef _PTH_FRAMEWORK_TOOL_PUBLIC_TASSIMPBONENODE_H
#define _PTH_FRAMEWORK_TOOL_PUBLIC_TASSIMPBONENODE_H

#include "UBase.h"

BEGIN(Tool)

class TAssimpBoneNode : public UBase {
public:
	TAssimpBoneNode();
	TAssimpBoneNode(const TAssimpBoneNode& _rhs);
	DESTRUCTOR(TAssimpBoneNode)
public:
public:
	const _float4x4& GetCombinedTransformMatrix() const { return m_mCombinedTransformMatrix; }
	const _float4x4& GetOffsetMatrix() const { return m_mOffsetMatrix; }
	void SetTransformMatrix(const _xmmatrix& _transformMatrix) { this->m_mTransformMatrix = _transformMatrix; }
	void SetOffsetMatrix(const _xmmatrix& _offsetMatrix) { this->m_mOffsetMatrix = _offsetMatrix; }
	const _wstring& GetBoneName() const { return m_wstrBoneName; }
	const _float GetDepth() const { return m_fDepth; }
public:
	virtual void Free() override;
	HRESULT NativeConstruct(CSHPTRREF<TAssimpBoneNode> _spParentsBone, 
		const _string& _strName, const _float4x4& _mTransformMatrix, const _float& _fDepths);
	void UpdateCombineMatrix();

	void GetData(BONENODEDESC& _stBoneNodeDesc);
private:
	SHPTR<TAssimpBoneNode>		m_spParentsBoneNode;

	_wstring											m_wstrBoneName;
	_float4x4										m_mTransformMatrix;
	_float4x4										m_mSaveTransformMatrix;
	_float4x4										m_mCombinedTransformMatrix;
	_float4x4										m_mOffsetMatrix;
	_float												m_fDepth;
};

END

#endif // _PTH_FRAMEWORK_TOOL_PUBLIC_TASSIMPBONENODE_H

