#pragma once

BEGIN(Tool)

/*
@ Date: 2024-03-18, Writer: ������
@ Explain
- Unity���� �̾ƿ� �����͵��� Export�ϴ� Ŭ���� 
*/
class TUnityModelLoader {
public:

	static void LoadUnityAnimFBXData(CSHPTRREF<FILEGROUP> _spFileGroup,
		CSHPTRREF<FILEDATA> _spFileData, IN _wstring& _wstrConvertLoadPath);

};

END