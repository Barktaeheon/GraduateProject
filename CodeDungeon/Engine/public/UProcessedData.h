#pragma once


BEGIN(Engine)

/*
@ Date: 2024-02-03,  Writer: ������
@ Explain
- Process�� �̿��ؼ� ó���� �����͵��� �����ϴ� Ŭ�����̴�.
*/
class UProcessedData {
public:
	UProcessedData(void * _pData, size_t _Size, _int _DataType);
	UProcessedData(const UProcessedData& _rhs);
	UProcessedData(UProcessedData&& _rhs) noexcept;
	~UProcessedData();

	UProcessedData& operator=(const UProcessedData& _other) = delete;
	UProcessedData& operator=(UProcessedData&& _other) noexcept;

	template<class T>
	T* ConvertData() { return reinterpret_cast<T*>(m_pData); }
public: /* get Set */
	_int GetDataType() const { return m_DataType; }
	_char* GetData() const { return m_pData; }
private:
	_char*							m_pData;                     
	_int								m_DataType;
	// �̷��� ���ϸ� �����Ͱ� �����Ǵ� ������ ����, �׷��ٰ� �ش� Ŭ������ �Ҵ��ϱ�� ����
	mutable _bool			m_isMake;
};

END

namespace std {
/*
@ Date: 2024-02-03,  Writer: ������
@ Explain
- HASH opeartor() ���꿡 ���� UProcessedData ������, concurrent_ �����̳ʿ� ����ϱ� ����
*/
	template <>
	struct std::hash<Engine::UProcessedData> {
		size_t operator()(Engine::UProcessedData&& ptr) const {
			return std::hash<char*>()(ptr.GetData());
		}
	};
	/*
	@ Date: 2024-02-03,  Writer: ������
	@ Explain
	- Equal ���꿡 ���� UProcessedData ������, concurrent_ �����̳ʿ� ����ϱ� ����
	*/
	template <>
	struct std::equal_to<Engine::UProcessedData> {
		bool operator()(Engine::UProcessedData&& lhs, Engine::UProcessedData&& rhs) const {
			return lhs.GetData() == rhs.GetData();
		}
	};
	/*
	@ Date: 2024-02-03,  Writer: ������
	@ Explain
	- less ���꿡 ���� SharedPtr ������, concurrent_ �����̳ʿ� ����ϱ� ����
	*/
	template <>
	struct std::less<Engine::UProcessedData> {
		bool operator()(Engine::UProcessedData&& lhs, Engine::UProcessedData&& rhs) const {
			return lhs.GetData() < rhs.GetData();
		}
	};
}