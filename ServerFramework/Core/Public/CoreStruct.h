#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_CORESTRUCT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_CORESTRUCT_H

namespace Core {

	/*
	@ Date: 2024-04-07, Writer: ������
	@ Explain: ������ ���� ���� 
	*/
	struct SPACEINFO {
		Vector3		vCenter;
		Vector3		vExtents;
		_int				Depths;
		_int				Index;

		SPACEINFO() : vCenter{}, vExtents{}, Depths{ 0 }, Index{ 0 } {}
		SPACEINFO(const Vector3& _vCenter, const Vector3& _vExtents, _int _Depths, _int _SpaceIndex = 0) :
			vCenter{ _vCenter }, vExtents{ _vExtents }, Depths{ _Depths }, Index{ _SpaceIndex } {}
	};
	/*
	@ Date: 2024-06-15, Writer: ������
	@ Explain: �ݶ��̴��� ���� ���� ���� 
	*/
	struct COLLIDERINFO {
		COLLIDERTYPE		ColliderType;
		Vector3					vPos;
		Vector3					vScale;
	};
	/*
	@ Date: 2024-06-16, Writer: ������
	@ Explain: ĳ���� �������ͽ��� ���� ���� 
	*/
	struct CHARSTATUS
	{
		_float fPower;
		_float fDefensive;
		_float fHp;

		CHARSTATUS() : fPower{0.0f}, fDefensive{0.0f}, fHp{0.0f}{}
		CHARSTATUS(_float _fPower, _float _fDefensive, _float _fHp) : fPower{ _fPower }, fDefensive{ _fDefensive }, fHp{ _fHp } {}
	};
}
#endif // _SERVERFRAMEWORK_CORE_PUBLIC_CORESTRUCT_H