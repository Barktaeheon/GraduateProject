#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ALOGGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ALOGGER_H

#include "ACoreBase.h"

BEGIN(Core)

/*
@ Date: 2024-04-08, Writer: ������
@ Explain: �α׸� ��� ������ Ŭ����
*/
class  ALogger final : public ACoreBase  {
public:
	ALogger();
	NO_COPY(ALogger)
	DESTRUCTOR(ALogger)
public:
	void PrintOut(const char* _string);
	void FileOut(const char* _string);
private:
	virtual void Free() override;

private:
	std::ofstream		m_LogFileStream;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ALOGGER_H