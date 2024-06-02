#pragma once


namespace Engine
{

    /*
    @ Date: 2024-02-05,  Writer: ������
    @ Explain
    - UStlAllocator�Ҵ��ڸ� ����ϴ� String, std::string, std::wstring�� ȣȯ�� �ʿ��ϴ�. 
    */
    template<class T>
    using STRING = std::basic_string<T, std::char_traits<T>, UStlAllocator<T>>;
    /*
    @ Date: 2024-02-05,  Writer: ������
    @ Explain
    - CustomString (UStlAllocator�� ���� MemoryPool�� ����ϱ� ����)
       std::string, std::wstring�� ȣȯ�� ���� CustomString�̴�. 
    */
    template<class TChar, class TString>
    requires CheckCustomString<TChar, TString>
    class CustomString : public STRING<TChar> {
        using ChangeType = std::conditional<std::is_same_v<TChar, char>, STRING<char>, STRING<wchar_t>>;
    public:
        using CUSSTRING = CustomString<TChar, TString>;
        CustomString() : STRING<TChar>() {}
        // �߰��� �ʿ��� �����ڵ��� �����ε��մϴ�.
        CustomString(const TString& str) : STRING<TChar>(str) {}
        // �߰��� �ʿ��� �����ڵ��� �����ε��մϴ�.
        CustomString(const TChar* str) : STRING<TChar>(str) {}
        // �߰��� �ʿ��� �����ڵ��� �����ε��մϴ�.
        CustomString(const CustomString& str) : STRING<TChar>(str) {}
        // �߰��� �ʿ��� �����ڵ��� �����ε��մϴ�.
        CustomString(const STRING<TChar>& str) :STRING<TChar>(str) {}

        CustomString(const std::filesystem::path& path) :STRING<TChar>(path.c_str()) {  }

        // �߰��� �ʿ��� �����ڵ��� �����ε��մϴ�.
        CustomString(TString&& str) noexcept  : STRING<TChar>(std::move(str)) {}
        // �߰��� �ʿ��� �����ڵ��� �����ε��մϴ�.
        CustomString(STRING<TChar>&& str) noexcept : STRING<TChar>(std::move(str)) {}
        // �̵� �����ڸ� �߰��մϴ�.
        CustomString(CUSSTRING&& other) noexcept: STRING<TChar>(std::move(other)) {}

        // �̵� �����ڸ� �߰��մϴ�.
        CustomString& operator=(CUSSTRING&& other) noexcept {
            if (this != &other) {
                STRING<TChar>::operator=(std::move(other));
            }
            return *this;
        }
        // �̵� �����ڸ� �߰��մϴ�.
        CustomString& operator=(STRING<TChar>&& other) noexcept {
            if (this != &other) {
                STRING<TChar>::operator=(std::move(other));
            }
            return *this;
        }
        // �̵� �����ڸ� �߰��մϴ�.
        CustomString& operator=(TString&& other) noexcept {
            if (this != &other) {
                STRING<TChar>::operator=(std::move(other));
            }
            return *this;
        }


        // std::filesystem::path�� CustomWString���� ��ȯ�ϴ� �����ڸ� �߰��մϴ�.
        CustomString& operator=(const std::filesystem::path& path) {
            STRING<TChar>::operator=(path.c_str());
            return *this;
        }

        CustomString& operator =(const STRING<TChar>& _str) {
            STRING<TChar>::operator=(_str);
            return *this;
        }



        CustomString& operator =(const TChar* _str) {
            STRING<TChar>::operator=(_str);
            return *this;
        }

        CustomString& operator =(const CUSSTRING& _str) {
            STRING<TChar>::operator=(_str.c_str());
            return *this;
        }

        // ���� ���, + �����ڸ� �߰��ϰڽ��ϴ�.
        CustomString operator+(const CUSSTRING& other) {
            CUSSTRING s(*this);
            s.append(other.c_str());
            return s;
        }

        // += �����ڸ� �߰��մϴ�.
        CustomString operator+(const STRING<TChar>& other) {
            CUSSTRING s(*this);
            s.append(other);
            return s;
        }

        CustomString operator+(const TString& other) {
            CUSSTRING s(*this);
            s.append(other);
            return s;
        }

        CustomString operator+(const TChar* cstr) {
            CUSSTRING s(*this);
            s.append(cstr);
            return s;
        }

        CustomString operator+(TChar ch) {
            CUSSTRING s(*this);
            s.push_back(ch);
            return s;
        }

        // += �����ڸ� �߰��մϴ�.
        CustomString& operator+=(const CUSSTRING& other) {
            this->append(other.c_str());
            return *this;
        }

        CustomString& operator+=(const TString& other) {
            this->append(other.c_str());
            return *this;
        }

        CustomString& operator+=(const TChar* cstr) {
            this->append(cstr);
            return *this;
        }

        CustomString& operator+=(TChar ch) {
            this->push_back(ch);
            return *this;
        }


        CustomString& operator<<(const CUSSTRING& other) {
            *this += other;
            return *this;
        }

        CustomString& operator<<(const TChar* cstr) {
            *this += cstr;
            return *this;
        }

        // Convert �Լ� ����
        ChangeType convertstr() const {
            // std::wstring���� ��ȯ�Ͽ� ��ȯ
            return  std::move(ChangeType(this->begin(), this->end()));
        }


        // ���ڿ��� �ٸ� Ÿ������ ��ȯ�ϴ� �����ڸ� �߰��մϴ�.
        operator STRING<TChar>() const {
            return  STRING<TChar>(this->begin(), this->end());
        }

        operator const TChar* () const { return this->c_str(); }

        operator std::filesystem::path() const {
            return std::filesystem::path(this->begin(), this->end());
        }

        bool operator<(const CUSSTRING& str) {
            return this->compare(str.c_str()) < 0;
        }

        char& operator[](unsigned long long  index) { return STRING<TChar>::operator[](index); }

        bool operator ==(const TChar* _str)
        {
            if constexpr (std::is_same<TChar, char>())
            {
                return 0 == strcmp(_str, this->c_str());
            }
            else
            {
                return 0 == lstrcmp(_str, this->c_str());
            }
        }
        bool operator !=(const TChar* _str) 
        {
            if constexpr (std::is_same<TChar, char>())
            {
                return 0 != strcmp(_str, this->c_str());
            }
            else
            {
                return 0 != lstrcmp(_str, this->c_str());
            }
        }

        bool operator ==(const CUSSTRING& _str) 
        {
            if constexpr (std::is_same<TChar, char>())
            {
                return 0 == strcmp(_str.c_str(), this->c_str());
            }
            else
            {
                return 0 == lstrcmp(_str.c_str(), this->c_str());
            }
        }
        bool operator !=(const CUSSTRING& _str) 
        {
            if constexpr (std::is_same<TChar, char>())
            {
                return 0 != strcmp(_str->c_str(), this->c_str());
            }
            else
            {
                return 0 != lstrcmp(_str->c_str(), this->c_str());
            }
        }

        template<class T> 
        static CUSSTRING to_string(T index) {
            return  std::_Integral_to_string<TChar>(index);
        }
    };

    template<class TChar, class TString>
        requires CheckCustomString<TChar, TString>
    bool operator ==(const TString& _str1, const CustomString<TChar, TString>& _str2) { return _str2.contains(_str1.c_str()); }
    template<class TChar, class TString>
        requires CheckCustomString<TChar, TString>
    bool operator !=(const TString& _str1, const CustomString<TChar, TString>& _str2) { return false == _str2.contains(_str1.c_str()); }

    template<class TChar, class TString>
        requires CheckCustomString<TChar, TString>
    bool operator ==(const TChar* _str1, const CustomString<TChar, TString>& _str2) { return _str2.contains(_str1); }

    template<class TChar, class TString>
        requires CheckCustomString<TChar, TString>
    bool operator !=(const TChar* _str1, const CustomString<TChar, TString>& _str2) { return false == _str2.contains(_str1); }
}

namespace std
{
    template<class T>
    using STRING = Engine::STRING<T>;

    /*
    @ Date: 2024-02-05,  Writer: ������
    @ Explain
    - hash ���꿡 ���� Engine::CustomString<TChar, String> ������ �����̳ʿ� ����ϱ� ����
    */
    template<class TChar, class String>
     struct std::hash<Engine::CustomString<TChar, String>> {
        size_t operator()(const Engine::CustomString<TChar, String>& str) const {
            return std::hash<STRING<TChar>>{}(str.c_str());
        }
    };
     /*
    @ Date: 2024-02-05,  Writer: ������
    @ Explain
    - Equal ���꿡 ���� Engine::CustomString<TChar, String> ������ �����̳ʿ� ����ϱ� ����
    */
     template<class TChar, class String>
     struct std::equal_to<Engine::CustomString<TChar, String>> {
         bool operator()(const Engine::CustomString<TChar, String>& lhs, const Engine::CustomString<TChar, String>& rhs) const {
             return lhs == rhs;
         }
     };
     /*
     @ Date: 2024-02-05,  Writer: ������
     @ Explain
     - less ���꿡 ���� Engine::CustomString<TChar, String> ������ �����̳ʿ� ����ϱ� ����
     */
     template<class TChar, class String>
     struct std::less<Engine::CustomString<TChar, String>> {
         bool operator()(const Engine::CustomString<TChar, String>& lhs, const Engine::CustomString<TChar, String>& rhs) const {
             return lhs < rhs;
         }
     };
}

