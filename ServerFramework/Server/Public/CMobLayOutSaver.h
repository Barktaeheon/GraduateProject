#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CMOBLAYOUTSAVER_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CMOBLAYOUTSAVER_H

BEGIN(Server)

class CMobLayOutSaver {
public:
	CMobLayOutSaver(const _string& _strPath, const VECTOR< MOBSERVERDATA>& _MobData);
};

class CStaticObjLayoutSaver {
public:
	CStaticObjLayoutSaver(const _string& _strPath, const VECTOR<STATICOBJSERVERDATA>& _ObjData);
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CMOBLAYOUTSAVER_H