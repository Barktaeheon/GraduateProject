

// // xcopy /�ɼ� .������ ������ ���ִ� ���      .���ϵ��� ������ ��� 


xcopy /y/s	.\Engine\Public\*.*				.\Reference\Headers\
xcopy /y/s	.\Engine\Bin\Debug\*.lib				.\Reference\Librarys\
xcopy /y/s	.\Engine\Bin\Release\*.lib				.\Reference\Librarys\

xcopy /y/s	.\Engine\Bin\Debug\*.dll				.\Client\Bin\
xcopy /y/s	.\Engine\Bin\Debug\*.dll				.\Tool\Bin\

xcopy /y/s	.\Engine\Bin\Release\*.dll				.\Client\Bin\
xcopy /y/s	.\Engine\Bin\Release\*.dll				.\Tool\Bin\

xcopy /y/s	.\Shader\Shaders\*.cso				.\Client\Bin\
xcopy /y/s	.\Shader\Shaders\*.cso				.\Tool\Bin\

xcopy /y/s	.\Engine\ThirdPartyLib\*.Lib	.\Reference\Librarys\
