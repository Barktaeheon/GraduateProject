

// // xcopy /�ɼ� .������ ������ ���ִ� ���      .���ϵ��� ������ ��� 

xcopy /y/s	.\Core\Public\*.*														.\Reference\Headers\
xcopy /y/s	.\Core\Bin\*.lib															.\Reference\Librarys\

xcopy /y/s	.\Core\Bin\*.dll															.\Server\Bin\
xcopy /y/s	.\Core\Bin\*.dll															.\TestClient\Bin\

xcopy /y/s	.\Core\ThirdPartyLib\*.Lib										.\Reference\Librarys\
xcopy /y/s	.\ProtocolBuffer\ServerProtocol\*.*						.\Reference\Headers\
xcopy /y/s	.\ProtocolBuffer\Public\*.*										.\Reference\Headers\
xcopy /y/s	.\ProtocolBuffer\Bin\*.lib											.\Reference\Librarys\
xcopy /y/s	.\ProtocolBuffer\ThirdPartyLib\*.lib						.\Reference\Librarys\


xcopy /y/s	.\Core\Public\CorePacket.h										.\(OutPut)\ServerReference\Headers\
xcopy /y/s	.\ProtocolBuffer\ServerProtocol\*.*						.\(OutPut)\ServerReference\Headers\
xcopy /y/s	.\ProtocolBuffer\Public\*.*										.\(OutPut)\ServerReference\Headers\
xcopy /y/s	.\ProtocolBuffer\Bin\*.lib											.\(OutPut)\ServerReference\Librarys\
xcopy /y/s	.\ProtocolBuffer\ThirdPartyLib\*.lib						.\(OutPut)\ServerReference\Librarys\




