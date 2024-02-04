#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ALUALOADER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ALUALOADER_H

BEGIN(Core)
/*
@ Date: 2024-01-13, Writer: ������
@ Explain
- Lua Script�� �ε��ϴ� Loader Ŭ�����Դϴ�. 
 �ش� lua Script�� �ε��� ��, ��ο� .lua�� �����ϰ� �̸��� ��� �ε��մϴ�.
*/
class ALuaLoader final {
public:
	// ���� �̸��� ������ �ε尡 �˴ϴ�. 
	ALuaLoader(const _string& _strFilename);
	~ALuaLoader();
public:
	void LuaFunctionCall(const _char* _FunctionName);
	
	template<class T>
	requires CheckNumber<T>
	void PushNumber(T _number){
		lua_tonumber(m_pLuaState, _number);
	}
	void PushString(const _char* _String);
	void PushBoolean(_bool _boolean);

	void LuaFunctionEnd(const _int _parameterNum, const _int _returnNum);

	template<class T>
	requires CheckNumber<T>
	// -1�� ���� �ֻ����� �ִٴ� �̾߱��̴�.
	T ToNumber(_int _order = -1) 
	{ 
		T value = lua_tonumber(m_pLuaState, _order); 
		lua_pop(m_pLuaState, _order);
		return value;
	}
	_bool ToBoolean(_int _order = -1);
	void* ToPointer(_int _order = -1);
public: /* get set */
	lua_State* GetLuaState() const { return m_pLuaState; }

private:
	_bool LuaCreate(const _string& _strFilename);

private:
	static constexpr const char* LUA_PATH{ "..\\..\\LuaScripts\\" };
	static constexpr const char* LUA_EXT{ ".lua" };
private:
	lua_State*		m_pLuaState;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ULUALOADER_H