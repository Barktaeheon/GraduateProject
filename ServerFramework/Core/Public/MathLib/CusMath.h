//-------------------------------------------------------------------------------------
// PTH.h -- Simplified C++ Math wrapper for DirectXMath
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//-------------------------------------------------------------------------------------

#pragma once

#ifdef MATHDLL_ENABLE
#define MATH_DLL   
#else
#define MATH_DLL  
#endif


#include <cassert>
#include <cstddef>
#include <cstring>
#include <functional>

#if (__cplusplus >= 202002L)
#include <compare>
#endif

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"
#endif

namespace DirectX
{
    namespace PTH
    {
        struct Vector2;
        struct Vector4;
        struct Matrix;
        struct Quaternion;
        struct Plane;
        struct uInt3;
        struct RECTANGLE;
        struct OUTMATRIX;


        typedef enum EMatrixRow : unsigned int
        {
            MATROW_RIGHT, MATROW_UP, MATROW_LOOK, MATROW_POS, MATROW_END
        }MATRIXROW;

        //------------------------------------------------------------------------------
        // 2D vector
        struct MATH_DLL Vector2 : public XMFLOAT2
        {
            Vector2() noexcept : XMFLOAT2(0.f, 0.f) {}
            constexpr explicit Vector2(const float& ix) noexcept : XMFLOAT2(ix, ix) {}
            constexpr Vector2(const float& ix, const float& iy) noexcept : XMFLOAT2(ix, iy) {}
            explicit Vector2(_In_reads_(2) const float* pArray) noexcept : XMFLOAT2(pArray) {}
            Vector2(FXMVECTOR V) noexcept { XMStoreFloat2(this, V); }
            Vector2(const XMFLOAT2& V) noexcept { this->x = V.x; this->y = V.y; }
            explicit Vector2(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; }

            Vector2(const Vector2&) = default;
            Vector2& operator=(const Vector2&) = default;

            Vector2(Vector2&&) = default;
            Vector2& operator=(Vector2&&) = default;

            operator XMVECTOR() const noexcept { return XMLoadFloat2(this); }

            // Comparison operators
            bool operator == (const Vector2& V) const noexcept;
            bool operator != (const Vector2& V) const noexcept;

            // Assignment operators
            Vector2& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; return *this; }
            Vector2& operator+= (const Vector2& V) noexcept;
            Vector2& operator-= (const Vector2& V) noexcept;
            Vector2& operator*= (const Vector2& V) noexcept;
            Vector2& operator*= (float S) noexcept;
            Vector2& operator/= (float S) noexcept;

            // Unary operators
            Vector2 operator+ () const noexcept { return *this; }
            Vector2 operator- () const noexcept { return Vector2(-x, -y); }

            // Vector operations
            bool InBounds(const Vector2& Bounds) const noexcept;

            float Length() const noexcept;
            float LengthSquared() const noexcept;
            static float Length(const Vector2& _v);
            static float LengthSquared(const Vector2& _v);

            float Dot(const Vector2& V) const noexcept;
            static float Dot(const Vector2& _v1, const Vector2& _v2);

            void Cross(const Vector2& V, Vector2& result) const noexcept;
            Vector2 Cross(const Vector2& V) const noexcept;
            static void Cross(const Vector2& v1, const Vector2& v2, Vector2& result)  noexcept;
            static Vector2 Cross(const Vector2& v1, const Vector2& v2)  noexcept;

            void Normalize() noexcept;
            void Normalize(Vector2& result) const noexcept;
            static void Normalize(const Vector2& value, Vector2& result);
            static Vector2 NormalizeValue(const Vector2& value);

            void Clamp(const Vector2& vmin, const Vector2& vmax) noexcept;
            void Clamp(const Vector2& vmin, const Vector2& vmax, Vector2& result) const noexcept;
            static void Clamp(const Vector2& value, const Vector2& vmin, const Vector2& vmax, Vector2& result);
            static Vector2 Clamp(const Vector2& value, const Vector2& vmin, const Vector2& vmax);

            // Static functions
            static float Distance(const Vector2& v1, const Vector2& v2) noexcept;
            static float DistanceSquared(const Vector2& v1, const Vector2& v2) noexcept;
            float Distance(const Vector2& v) noexcept;
            float DistanceSquared(const Vector2& v) noexcept;

            static void Min(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept;
            static Vector2 Min(const Vector2& v1, const Vector2& v2) noexcept;

            static void Max(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept;
            static Vector2 Max(const Vector2& v1, const Vector2& v2) noexcept;

            static void Lerp(const Vector2& v1, const Vector2& v2, const float& t, Vector2& result) noexcept;
            static Vector2 Lerp(const Vector2& v1, const Vector2& v2, const float& t) noexcept;

            static void SmoothStep(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept;
            static Vector2 SmoothStep(const Vector2& v1, const Vector2& v2, float t) noexcept;

            static void Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, const float& f, const float& g, Vector2& result) noexcept;
            static Vector2 Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, const float& f, const float& g) noexcept;

            static void CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, const float& t, Vector2& result) noexcept;
            static Vector2 CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, const float& t) noexcept;

            static void Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, const float& t, Vector2& result) noexcept;
            static Vector2 Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, const float& t) noexcept;

            static void Reflect(const Vector2& ivec, const Vector2& nvec, Vector2& result) noexcept;
            static Vector2 Reflect(const Vector2& ivec, const Vector2& nvec) noexcept;

            static void Refract(const Vector2& ivec, const Vector2& nvec, const float& refractionIndex, Vector2& result) noexcept;
            static Vector2 Refract(const Vector2& ivec, const Vector2& nvec, const float& refractionIndex) noexcept;

            static void Transform(const Vector2& v, const Quaternion& quat, Vector2& result) noexcept;
            static Vector2 Transform(const Vector2& v, const Quaternion& quat) noexcept;

            static void TransformCoord(const Vector2& v, const Matrix& m, Vector2& result) noexcept;
            static Vector2 TransformCoord(const Vector2& v, const Matrix& m) noexcept;
            static void TransformCoordStream(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector2* resultArray) noexcept;

            static void Transform(const Vector2& v, const Matrix& m, Vector4& result) noexcept;
            static void Transform(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray) noexcept;

            static void TransformNormal(const Vector2& v, const Matrix& m, Vector2& result) noexcept;
            static Vector2 TransformNormal(const Vector2& v, const Matrix& m) noexcept;
            static void TransformNormalStream(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector2* resultArray) noexcept;

            static void SplineCurve(const Vector2& _prevPos, const Vector2& _vStandrardPos, const Vector2& _vNextPos1,
                const Vector2& _vNextPos2, const float& _fValue, Vector2& _result);
            static Vector2 SplineCurve(const Vector2& _prevPos, const Vector2& _vStandrardPos, const Vector2& _vNextPos1,
                const Vector2& _vNextPos2, const float& _fValue);

            static void BezierCurve(const Vector2& _vStandard1, const Vector2& _vControl1, const Vector2& _vStandard2,
                const Vector2& _vControl2, const float& _fValue, Vector2& result);
            static Vector2 BezierCurve(const Vector2& _vStandard1, const Vector2& _vControl1, const Vector2& _vStandard2,
                const Vector2& _vControl2, const float& _fValue);

            // Constants
            static const Vector2 Zero;
            static const Vector2 One;
            static const Vector2 UnitX;
            static const Vector2 UnitY;
        };

        // Binary operators
        Vector2 operator+ (const Vector2& V1, const Vector2& V2) noexcept;
        Vector2 operator- (const Vector2& V1, const Vector2& V2) noexcept;
        Vector2 operator* (const Vector2& V1, const Vector2& V2) noexcept;
        Vector2 operator* (const Vector2& V, float S) noexcept;
        Vector2 operator/ (const Vector2& V1, const Vector2& V2) noexcept;
        Vector2 operator/ (const Vector2& V, float S) noexcept;
        Vector2 operator* (float S, const Vector2& V) noexcept;

        //------------------------------------------------------------------------------
        // 3D vector
        struct MATH_DLL Vector3 : public XMFLOAT3
        {
            Vector3() noexcept : XMFLOAT3(0.f, 0.f, 0.f) {}
            constexpr explicit Vector3(const float& ix) noexcept : XMFLOAT3(ix, ix, ix) {}
            constexpr Vector3(const float& ix, const float& iy, const float& iz) noexcept : XMFLOAT3(ix, iy, iz) {}
            explicit Vector3(_In_reads_(3) const float* pArray) noexcept : XMFLOAT3(pArray) {}
            Vector3(FXMVECTOR V) noexcept { XMStoreFloat3(this, V); }
            Vector3(const XMFLOAT3& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; }
            explicit Vector3(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; }

            Vector3(const Vector3&) = default;
            Vector3& operator=(const Vector3&) = default;

            Vector3(Vector3&&) = default;
            Vector3& operator=(Vector3&&) = default;

            operator XMVECTOR() const noexcept { return XMLoadFloat3(this); }

            // Comparison operators
            bool operator == (const Vector3& V) const noexcept;
            bool operator != (const Vector3& V) const noexcept;

            // Assignment operators
            Vector3& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; return *this; }
            Vector3& operator+= (const Vector3& V) noexcept;
            Vector3& operator-= (const Vector3& V) noexcept;
            Vector3& operator*= (const Vector3& V) noexcept;
            Vector3& operator*= (float S) noexcept;
            Vector3& operator/= (float S) noexcept;

            // Unary operators
            Vector3 operator+ () const noexcept { return *this; }
            Vector3 operator- () const noexcept;

            // Vector operations
            bool InBounds(const Vector3& Bounds) const noexcept;

            float Length() const noexcept;
            float LengthSquared() const noexcept;
            static float Length(const Vector3& _v);
            static float LengthSquared(const Vector3& _v);

            float Dot(const Vector3& V) const noexcept;
            static float Dot(const Vector3& _v1, const Vector3& _v2);

            void Cross(const Vector3& V, Vector3& result) const noexcept;
            Vector3 Cross(const Vector3& V) const noexcept;
            static void Cross(const Vector3& v1, const Vector3& v2, Vector3& result)  noexcept;
            static Vector3 Cross(const Vector3& v1, const Vector3& v2)  noexcept;

            void Normalize() noexcept;
            void Normalize(Vector3& result) const noexcept;
            static void Normalize(const Vector3& value, Vector3& result);
            static Vector3 NormalizeValue(const Vector3& value);

            void Clamp(const Vector3& vmin, const Vector3& vmax) noexcept;
            void Clamp(const Vector3& vmin, const Vector3& vmax, Vector3& result) const noexcept;
            static void Clamp(const Vector3& value, const Vector3& vmin, const Vector3& vmax, Vector3& result);
            static Vector3 Clamp(const Vector3& value, const Vector3& vmin, const Vector3& vmax);

            // Static functions
            float Distance(const Vector3& v) noexcept;
            float DistanceSquared(const Vector3& v) noexcept;
            static float Distance(const Vector3& v1, const Vector3& v2) noexcept;
            static float DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept;

            static void Min(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept;
            static Vector3 Min(const Vector3& v1, const Vector3& v2) noexcept;

            static void Max(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept;
            static Vector3 Max(const Vector3& v1, const Vector3& v2) noexcept;

            static void Lerp(const Vector3& v1, const Vector3& v2, const float& t, Vector3& result) noexcept;
            static Vector3 Lerp(const Vector3& v1, const Vector3& v2, const float& t) noexcept;

            static void SmoothStep(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept;
            static Vector3 SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept;

            static void Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, const float& f, const float& g, Vector3& result) noexcept;
            static Vector3 Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, const float& f, const float& g) noexcept;

            static void CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, const float& t, Vector3& result) noexcept;
            static Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, const float& t) noexcept;

            static void Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, const float& t, Vector3& result) noexcept;
            static Vector3 Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, const float& t) noexcept;

            static void Reflect(const Vector3& ivec, const Vector3& nvec, Vector3& result) noexcept;
            static Vector3 Reflect(const Vector3& ivec, const Vector3& nvec) noexcept;

            static void Refract(const Vector3& ivec, const Vector3& nvec, const float& refractionIndex, Vector3& result) noexcept;
            static Vector3 Refract(const Vector3& ivec, const Vector3& nvec, const float& refractionIndex) noexcept;

            static void Transform(const Vector3& v, const Quaternion& quat, Vector3& result) noexcept;
            static Vector3 Transform(const Vector3& v, const Quaternion& quat) noexcept;

            static void TransformCoord(const Vector3& v, const Matrix& m, Vector3& result) noexcept;
            static Vector3 TransformCoord(const Vector3& v, const Matrix& m) noexcept;
            static void TransformCoordStream(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector3* resultArray) noexcept;

            static void Transform(const Vector3& v, const Matrix& m, Vector4& result) noexcept;
            static void Transform(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray) noexcept;

            static void TransformNormal(const Vector3& v, const Matrix& m, Vector3& result) noexcept;
            static Vector3 TransformNormal(const Vector3& v, const Matrix& m) noexcept;
            static void TransformNormalStream(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector3* resultArray) noexcept;

            static void SplineCurve(const Vector3& _prevPos, const Vector3& _vStandrardPos, const Vector3& _vNextPos1,
                const Vector3& _vNextPos2, const float& _fValue, Vector3& _result);
            static Vector3 SplineCurve(const Vector3& _prevPos, const Vector3& _vStandrardPos, const Vector3& _vNextPos1,
                const Vector3& _vNextPos2, const float& _fValue);

            static void BezierCurve(const Vector3& _vStandard1, const Vector3& _vControl1, const Vector3& _vStandard2,
                const Vector3& _vControl2, const float& _fValue, Vector3& result);
            static Vector3 BezierCurve(const Vector3& _vStandard1, const Vector3& _vControl1, const Vector3& _vStandard2,
                const Vector3& _vControl2, const float& _fValue);

            // Constants
            static const Vector3 Zero;
            static const Vector3 One;
            static const Vector3 UnitX;
            static const Vector3 UnitY;
            static const Vector3 UnitZ;
            static const Vector3 Up;
            static const Vector3 Down;
            static const Vector3 Right;
            static const Vector3 Left;
            static const Vector3 Forward;
            static const Vector3 Backward;
        };

        // Binary operators
        Vector3 operator+ (const Vector3& V1, const Vector3& V2) noexcept;
        Vector3 operator- (const Vector3& V1, const Vector3& V2) noexcept;
        Vector3 operator* (const Vector3& V1, const Vector3& V2) noexcept;
        Vector3 operator* (const Vector3& V, float S) noexcept;
        Vector3 operator/ (const Vector3& V1, const Vector3& V2) noexcept;
        Vector3 operator/ (const Vector3& V, float S) noexcept;
        Vector3 operator* (float S, const Vector3& V) noexcept;

        //------------------------------------------------------------------------------
        // 4D vector
        struct MATH_DLL Vector4 : public XMFLOAT4
        {
            Vector4() noexcept : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
            constexpr explicit Vector4(const float& ix) noexcept : XMFLOAT4(ix, ix, ix, ix) {}
            constexpr Vector4(const float& ix, const float& iy, const float& iz, const float& iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
            explicit Vector4(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
            Vector4(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
            Vector4(const XMFLOAT4& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; this->w = V.w; }
            explicit Vector4(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

            Vector4(const Vector4&) = default;
            Vector4& operator=(const Vector4&) = default;

            Vector4(Vector4&&) = default;
            Vector4& operator=(Vector4&&) = default;

            operator XMVECTOR() const  noexcept { return XMLoadFloat4(this); }

            // Comparison operators
            bool operator == (const Vector4& V) const noexcept;
            bool operator != (const Vector4& V) const noexcept;

            // Assignment operators
            Vector4& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
            Vector4& operator+= (const Vector4& V) noexcept;
            Vector4& operator-= (const Vector4& V) noexcept;
            Vector4& operator*= (const Vector4& V) noexcept;
            Vector4& operator*= (float S) noexcept;
            Vector4& operator/= (float S) noexcept;
            Vector4& operator =(const Vector3& _vPos) { x = _vPos.x; y = _vPos.y; z = _vPos.z;  return *this; }

            // Unary operators
            Vector4 operator+ () const noexcept { return *this; }
            Vector4 operator- () const noexcept;

            // Vector operations
            bool InBounds(const Vector4& Bounds) const noexcept;

            float Length() const noexcept;
            float LengthSquared() const noexcept;
            static float Length(const Vector4& _v);
            static float LengthSquared(const Vector4& _v);

            float Dot(const Vector4& V) const noexcept;
            static float Dot(const Vector4& _v1, const Vector4& _v2);

            void Cross(const Vector4& v1, const Vector4& v2, Vector4& result) const noexcept;
            Vector4 Cross(const Vector4& v1, const Vector4& v2) const noexcept;
            static void Cross(const Vector4& v1, const Vector4& v2, const Vector4& v3, Vector4& result)  noexcept;
            static Vector4 Cross(const Vector4& v1, const Vector4& v2, const Vector4& v3) noexcept;

            void Normalize() noexcept;
            void Normalize(Vector4& result) const noexcept;
            static void Normalize(const Vector4& value, Vector4& result);
            static Vector4 NormalizeValue(const Vector4& value);

            void Clamp(const Vector4& vmin, const Vector4& vmax) noexcept;
            void Clamp(const Vector4& vmin, const Vector4& vmax, Vector4& result) const noexcept;
            static void Clamp(const Vector4& value, const Vector4& vmin, const Vector4& vmax, Vector4& result);
            static Vector4 Clamp(const Vector4& value, const Vector4& vmin, const Vector4& vmax);


            // Static functions
            float Distance(const Vector4& v) noexcept;
            float DistanceSquared(const Vector4& v) noexcept;
            static float Distance(const Vector4& v1, const Vector4& v2) noexcept;
            static float DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept;

            static void Min(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept;
            static Vector4 Min(const Vector4& v1, const Vector4& v2) noexcept;

            static void Max(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept;
            static Vector4 Max(const Vector4& v1, const Vector4& v2) noexcept;

            static void Lerp(const Vector4& v1, const Vector4& v2, const float& t, Vector4& result) noexcept;
            static Vector4 Lerp(const Vector4& v1, const Vector4& v2, const float& t) noexcept;

            static void SmoothStep(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept;
            static Vector4 SmoothStep(const Vector4& v1, const Vector4& v2, float t) noexcept;

            static void Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, const float& f, const float& g, Vector4& result) noexcept;
            static Vector4 Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, const float& f, const float& g) noexcept;

            static void CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, const float& t, Vector4& result) noexcept;
            static Vector4 CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, const float& t) noexcept;

            static void Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, const float& t, Vector4& result) noexcept;
            static Vector4 Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, const float& t) noexcept;

            static void Reflect(const Vector4& ivec, const Vector4& nvec, Vector4& result) noexcept;
            static Vector4 Reflect(const Vector4& ivec, const Vector4& nvec) noexcept;

            static void Refract(const Vector4& ivec, const Vector4& nvec, const float& refractionIndex, Vector4& result) noexcept;
            static Vector4 Refract(const Vector4& ivec, const Vector4& nvec, const float& refractionIndex) noexcept;

            static void Transform(const Vector2& v, const Quaternion& quat, Vector4& result) noexcept;
            static Vector4 Transform(const Vector2& v, const Quaternion& quat) noexcept;

            static void Transform(const Vector3& v, const Quaternion& quat, Vector4& result) noexcept;
            static Vector4 Transform(const Vector3& v, const Quaternion& quat) noexcept;

            static void Transform(const Vector4& v, const Quaternion& quat, Vector4& result) noexcept;
            static Vector4 Transform(const Vector4& v, const Quaternion& quat) noexcept;

            static void Transform(const Vector4& v, const Matrix& m, Vector4& result) noexcept;
            static Vector4 Transform(const Vector4& v, const Matrix& m) noexcept;
            static void Transform(_In_reads_(count) const Vector4* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray) noexcept;

            static void SplineCurve(const Vector4& _prevPos, const Vector4& _vStandrardPos, const Vector4& _vNextPos1,
                const Vector4& _vNextPos2, const float& _fValue, Vector4& _result);
            static Vector4 SplineCurve(const Vector4& _prevPos, const Vector4& _vStandrardPos, const Vector4& _vNextPos1,
                const Vector4& _vNextPos2, const float& _fValue);

            static void BezierCurve(const Vector4& _vStandard1, const Vector4& _vControl1, const Vector4& _vStandard2,
                const Vector4& _vControl2, const float& _fValue, Vector4& result);
            static Vector4 BezierCurve(const Vector4& _vStandard1, const Vector4& _vControl1, const Vector4& _vStandard2,
                const Vector4& _vControl2, const float& _fValue);

            // Constants
            static const Vector4 Zero;
            static const Vector4 One;
            static const Vector4 UnitX;
            static const Vector4 UnitY;
            static const Vector4 UnitZ;
            static const Vector4 UnitW;
        };

        // Binary operators
        Vector4 operator+ (const Vector4& V1, const Vector4& V2) noexcept;
        Vector4 operator- (const Vector4& V1, const Vector4& V2) noexcept;
        Vector4 operator* (const Vector4& V1, const Vector4& V2) noexcept;
        Vector4 operator* (const Vector4& V, float S) noexcept;
        Vector4 operator/ (const Vector4& V1, const Vector4& V2) noexcept;
        Vector4 operator/ (const Vector4& V, float S) noexcept;
        Vector4 operator* (float S, const Vector4& V) noexcept;

        // OutMatrix
#pragma region OUTMATRIX
        struct OUTMATRIX
        {
            Vector3 vPos;
            Vector3 vScale;
            Vector3 vRot;
            Vector4 vQuaternionRot;
        };
#pragma endregion OUTMATRIX

        //------------------------------------------------------------------------------
        // 4x4 Matrix (assumes right-handed cooordinates)
        struct MATH_DLL Matrix : public XMFLOAT4X4
        {
            Matrix() noexcept
                : XMFLOAT4X4(1.f, 0, 0, 0,
                    0, 1.f, 0, 0,
                    0, 0, 1.f, 0,
                    0, 0, 0, 1.f)
            {
            }
            constexpr Matrix(const float& m00, const float& m01, const float& m02, const float& m03,
                const float& m10, const float& m11, const float& m12, const float& m13,
                const float& m20, const float& m21, const float& m22, const float& m23,
                const float& m30, const float& m31, const float& m32, const float& m33) noexcept
                : XMFLOAT4X4(m00, m01, m02, m03,
                    m10, m11, m12, m13,
                    m20, m21, m22, m23,
                    m30, m31, m32, m33)
            {
            }
            explicit Matrix(const Vector3& r0, const Vector3& r1, const Vector3& r2) noexcept
                : XMFLOAT4X4(r0.x, r0.y, r0.z, 0,
                    r1.x, r1.y, r1.z, 0,
                    r2.x, r2.y, r2.z, 0,
                    0, 0, 0, 1.f)
            {
            }
            explicit Matrix(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3) noexcept
                : XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
                    r1.x, r1.y, r1.z, r1.w,
                    r2.x, r2.y, r2.z, r2.w,
                    r3.x, r3.y, r3.z, r3.w)
            {
            }
            Matrix(const XMFLOAT4X4& M) noexcept { memcpy(this, &M, sizeof(XMFLOAT4X4)); }
            Matrix(const XMFLOAT3X3& M) noexcept;
            Matrix(const XMFLOAT4X3& M) noexcept;

            explicit Matrix(_In_reads_(16) const float* pArray) noexcept : XMFLOAT4X4(pArray) {}
            Matrix(CXMMATRIX M) noexcept { XMStoreFloat4x4(this, M); }

            Matrix(const Matrix&) = default;
            Matrix& operator=(const Matrix&) = default;

            Matrix(Matrix&&) = default;
            Matrix& operator=(Matrix&&) = default;

            operator XMMATRIX() const noexcept { return XMLoadFloat4x4(this); }

            // Comparison operators
            bool operator == (const Matrix& M) const noexcept;
            bool operator != (const Matrix& M) const noexcept;

            // Assignment operators
            Matrix& operator= (const XMFLOAT3X3& M) noexcept;
            Matrix& operator= (const XMFLOAT4X3& M) noexcept;
            Matrix& operator+= (const Matrix& M) noexcept;
            Matrix& operator-= (const Matrix& M) noexcept;
            Matrix& operator*= (const Matrix& M) noexcept;
            Matrix& operator*= (float S) noexcept;
            Matrix& operator/= (float S) noexcept;

            Matrix& operator/= (const Matrix& M) noexcept;
            // Element-wise divide

        // Unary operators
            Matrix operator+ () const noexcept { return *this; }
            Matrix operator- () const noexcept;


            const Vector3 Get_Right() const  { return *((Vector3*)&this->m[MATROW_RIGHT][0]); }
            const Vector3 Get_Left() const { return *((Vector3*)&this->m[MATROW_RIGHT][0]) * -1.f; }
            const Vector3 Get_Up() const  { return *((Vector3*)&this->m[MATROW_UP][0]); }
            const Vector3 Get_Down() const  { return *((Vector3*)&this->m[MATROW_UP][0]) * -1.f; }
            const Vector3 Get_Look() const  { return *((Vector3*)&this->m[MATROW_LOOK][0]); }
            const Vector3 Get_Back() const  { return *((Vector3*)&this->m[MATROW_LOOK][0]) * -1.f; }
            const Vector3 Get_Pos() const  { return *((Vector3*)&this->m[MATROW_POS][0]); }

            void Set_Right(const Vector3& _vState) const  {  *(Vector3*)&this->m[MATROW_RIGHT][0] = _vState; }
            void Set_Up(const Vector3& _vState) const  {  *(Vector3*)&this->m[MATROW_UP][0] = _vState; }
            void Set_Look(const Vector3& _vState) const  { *(Vector3*)&this->m[MATROW_LOOK][0] = _vState; }
            void Set_Pos(const Vector3& _vState) const  { *(Vector3*)&this->m[MATROW_POS][0] = _vState; }

            // Matrix operations
            bool Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) noexcept;

            Matrix Transpose() const noexcept;
            void Transpose(Matrix& result) const noexcept;

            Matrix Invert() const noexcept;
            void Invert(Matrix& result) const noexcept;

            float Determinant() const noexcept;

            // Computes rotation about y-axis (y), then x-axis (x), then z-axis (z)
            Vector3 ToEuler() const noexcept;

            // Static functions
            static Matrix CreateBillboard(
                const Vector3& object, const Vector3& cameraPosition, const Vector3& cameraUp, _In_opt_ const Vector3* cameraForward = nullptr) noexcept;

            static Matrix CreateConstrainedBillboard(
                const Vector3& object, const Vector3& cameraPosition, const Vector3& rotateAxis,
                _In_opt_ const Vector3* cameraForward = nullptr, _In_opt_ const Vector3* objectForward = nullptr) noexcept;

            static Matrix CreateTranslation(const Vector3& position) noexcept;
            static Matrix CreateTranslation(const float& x, const float& y, const float& z) noexcept;

            static Matrix CreateScale(const Vector3& scales) noexcept;
            static Matrix CreateScale(const float& xs, const float& ys, const float& zs) noexcept;
            static Matrix CreateScale(const float& scale) noexcept;

            static Matrix CreateRotationX(const float& radians) noexcept;
            static Matrix CreateRotationY(const float& radians) noexcept;
            static Matrix CreateRotationZ(const float& radians) noexcept;

            static Matrix CreateFromAxisAngle(const Vector3& axis, const float& angle) noexcept;

            static Matrix CreatePerspectiveFieldOfViewRH(const float& fov, const float& aspectRatio, const float& nearPlane, const float& farPlane) noexcept;
            static Matrix CreatePerspectiveRH(const float& width, const float& height, const float& nearPlane, const float& farPlane) noexcept;
            static Matrix CreatePerspectiveOffCenterRH(const float& left, const float& right, const float& bottom, const float& top, const float& nearPlane, const float& farPlane) noexcept;
            static Matrix CreateOrthographicRH(const float& width, const float& height, const float& zNearPlane, const float& zFarPlane) noexcept;
            static Matrix CreateOrthographicOffCenterRH(const float& left, const float& right, const float& bottom, const float& top, const float& zNearPlane, const float& zFarPlane) noexcept;
            static Matrix CreatePerspectiveForRH(const float& fov, const float& aspect, const float& Near, const float& Far);

            static Matrix CreatePerspectiveFieldOfViewLH(const float& fov, const float& aspectRatio, const float& nearPlane, const float& farPlane) noexcept;
            static Matrix CreatePerspectiveLH(const float& width, const float& height, const float& nearPlane, const float& farPlane) noexcept;
            static Matrix CreatePerspectiveOffCenterLH(const float& left, const float& right, const float& bottom, const float& top, const float& nearPlane, const float& farPlane) noexcept;
            static Matrix CreateOrthographicLH(const float& width, const float& height, const float& zNearPlane, const float& zFarPlane) noexcept;
            static Matrix CreateOrthographicOffCenterLH(const float& left, const float& right, const float& bottom, const float& top, const float& zNearPlane, const float& zFarPlane) noexcept;
            static Matrix CreatePerspectiveForLH(const float& fov, const float& aspect, const float& Near, const float& Far);

            static Matrix CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up) noexcept;
            static Matrix CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up) noexcept;

            static Matrix CreateFromQuaternion(const Quaternion& quat) noexcept;

            // Rotates about y-axis (yaw), then x-axis (pitch), then z-axis (roll)
            static Matrix CreateFromYawPitchRoll(const float& yaw, const float& pitch, const float& roll) noexcept;

            // Rotates about y-axis (angles.y), then x-axis (angles.x), then z-axis (angles.z)
            static Matrix CreateFromYawPitchRoll(const Vector3& angles) noexcept;

            static Matrix CreateShadow(const Vector3& lightDir, const Plane& plane) noexcept;

            static Matrix CreateReflection(const Plane& plane) noexcept;

            static void Lerp(const Matrix& M1, const Matrix& M2, const float& t, Matrix& result) noexcept;
            static Matrix Lerp(const Matrix& M1, const Matrix& M2, const float& t) noexcept;

            static void Transform(const Matrix& M, const Quaternion& rotation, Matrix& result) noexcept;
            static Matrix Transform(const Matrix& M, const Quaternion& rotation) noexcept;

            OUTMATRIX Get_OutMatrix() noexcept;
            static OUTMATRIX Get_OutMatrix(const Matrix& _matrix) noexcept;
            static const Vector4 MatrixGetFloat4(const Matrix& _Matrix, const MATRIXROW& _eMatrixRow) noexcept;
            static const Vector3 MatrixGetFloat3(const Matrix& _Matrix, const MATRIXROW& _eMatrixRow) noexcept;
            static const Matrix MatrixSetFloat4(const Vector4& _vSetVector, const MATRIXROW& _eMatrixRow, const Matrix& _Matrix) noexcept;
            static const Matrix MatrixSetFloat3(const Vector3& _vSetVector, const MATRIXROW& _eMatrixRow, const Matrix& _Matrix) noexcept;
            static const  Matrix MatrixSetScaling(const Vector3& _v3Scaling, const Matrix& _Matrix) noexcept;
            static const  void MatrixSetRotationFix(const Vector3& _vRotation,  Matrix& _Matrix)noexcept;
            static  const void MatrixSetRotationTurn(const Vector3& _vRotation,  Matrix& _Matrix)noexcept;
            static  const Vector3 Get_Scaling(const Matrix& _Matrix)noexcept;
            static const Matrix Combine_Matrix(const Vector3& _vScale, const Vector3& _vPos, const Quaternion& _vRotation)noexcept;
            static const Matrix Combine_Matrix(const OUTMATRIX& _OutMatrix)noexcept;

            const  Matrix MatrixSetScaling(const Vector3& _v3Scaling) noexcept;
            const  Matrix MatrixSetRotationFix(const Vector3& _vRotation)noexcept;
            const Matrix MatrixSetRotationTurn(const Vector3& _vRotation)noexcept;
            const Vector3 Get_Scaling()noexcept;
            const void Combine(const Vector3& _vScale, const Vector3& _vPos, const Quaternion& _vRotation)noexcept;
            const void Combine(const OUTMATRIX& _OutMatrix)noexcept;

            Matrix LookAt(const Vector3& _vPos);

            // Move 
            void MoveForward(const double& _dTimeDelta, const float& _fSpeed);
            void MoveBack(const double& _dTimeDelta, const float& _fSpeed);
            void MoveLeft(const double& _dTimeDelta, const float& _fSpeed);
            void MoveRight(const double& _dTimeDelta, const float& _fSpeed);
            // Move Not Y
            void MoveForwardNotY(const double& _dTimeDelta, const float& _fSpeed);
            void MoveBackNotY(const double& _dTimeDelta, const float& _fSpeed);
            void MoveLeftNotY(const double& _dTimeDelta, const float& _fSpeed);
            void MoveRightNotY(const double& _dTimeDelta, const float& _fSpeed);
            // Translate To Pos
            void TranslatePos(const Vector3& _vPos, const double& _dTimeDelta, const float& _fSpeed,
                const float& _fLimitDistance = 0.1f);
            void TranslateDir(const Vector3& _vDir, const double& _dTimeDelta, const float& _fSpeed);
            // Translate To Pos Not Y
            void TranslatePosNotY(const Vector3& _vPos, const double& _dTimeDelta, const float& _fSpeed,
                const float& _fLimitDistance = 0.1f);
            void TranslateDirNotY(const Vector3& _vDir, const double& _dTimeDelta, const float& _fSpeed);
            // Compute Distance
            const float ComputeDistance(const Vector3& _vPos);
            const float ComputeDistanceSq(const Vector3& _vPos);

            // Constants
            static const Matrix Identity;
        };

        // Binary operators
        Matrix operator+ (const Matrix& M1, const Matrix& M2) noexcept;
        Matrix operator- (const Matrix& M1, const Matrix& M2) noexcept;
        Matrix operator* (const Matrix& M1, const Matrix& M2) noexcept;
        Matrix operator* (const Matrix& M, float S) noexcept;
        Matrix operator/ (const Matrix& M, float S) noexcept;
        Matrix operator/ (const Matrix& M1, const Matrix& M2) noexcept;
        // Element-wise divide
        Matrix operator* (float S, const Matrix& M) noexcept;


        //-----------------------------------------------------------------------------
        // Plane
        struct MATH_DLL Plane : public XMFLOAT4
        {
            Plane() noexcept : XMFLOAT4(0.f, 1.f, 0.f, 0.f) {}
            constexpr Plane(const float& ix, const float& iy, const float& iz, const float& iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
            Plane(const Vector3& normal, float d) noexcept : XMFLOAT4(normal.x, normal.y, normal.z, d) {}
            Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3) noexcept;
            Plane(const Vector3& point, const Vector3& normal) noexcept;
            explicit Plane(const Vector4& v) noexcept : XMFLOAT4(v.x, v.y, v.z, v.w) {}
            explicit Plane(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
            Plane(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
            Plane(const XMFLOAT4& p) noexcept { this->x = p.x; this->y = p.y; this->z = p.z; this->w = p.w; }
            explicit Plane(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

            Plane(const Plane&) = default;
            Plane& operator=(const Plane&) = default;

            Plane(Plane&&) = default;
            Plane& operator=(Plane&&) = default;

            operator XMVECTOR() const noexcept { return XMLoadFloat4(this); }

            // Comparison operators
            bool operator == (const Plane& p) const noexcept;
            bool operator != (const Plane& p) const noexcept;

            // Assignment operators
            Plane& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }

            // Properties
            Vector3 Normal() const noexcept { return Vector3(x, y, z); }
            void Normal(const Vector3& normal) noexcept { x = normal.x; y = normal.y; z = normal.z; }

            float D() const noexcept { return w; }
            void D(float d) noexcept { w = d; }

            // Plane operations
            void Normalize() noexcept;
            void Normalize(Plane& result) const noexcept;

            float Dot(const Vector4& v) const noexcept;
            float DotCoordinate(const Vector3& position) const noexcept;
            float DotNormal(const Vector3& normal) const noexcept;

            // Static functions
            static void Transform(const Plane& plane, const Matrix& M, Plane& result) noexcept;
            static Plane Transform(const Plane& plane, const Matrix& M) noexcept;

            static void Transform(const Plane& plane, const Quaternion& rotation, Plane& result) noexcept;
            static Plane Transform(const Plane& plane, const Quaternion& rotation) noexcept;
            // Input quaternion must be the inverse transpose of the transformation
        };

        //------------------------------------------------------------------------------
        // Quaternion
        struct Quaternion : public XMFLOAT4
        {
            Quaternion() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
            constexpr Quaternion(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
            Quaternion(const Vector3& v, float scalar) noexcept : XMFLOAT4(v.x, v.y, v.z, scalar) {}
            explicit Quaternion(const Vector4& v) noexcept : XMFLOAT4(v.x, v.y, v.z, v.w) {}
            explicit Quaternion(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
            Quaternion(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
            Quaternion(const XMFLOAT4& q) noexcept { this->x = q.x; this->y = q.y; this->z = q.z; this->w = q.w; }
            explicit Quaternion(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

            Quaternion(const Quaternion&) = default;
            Quaternion& operator=(const Quaternion&) = default;

            Quaternion(Quaternion&&) = default;
            Quaternion& operator=(Quaternion&&) = default;

            operator XMVECTOR() const noexcept { return XMLoadFloat4(this); }

            // Comparison operators
            bool operator == (const Quaternion& q) const noexcept;
            bool operator != (const Quaternion& q) const noexcept;

            // Assignment operators
            Quaternion& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
            Quaternion& operator+= (const Quaternion& q) noexcept;
            Quaternion& operator-= (const Quaternion& q) noexcept;
            Quaternion& operator*= (const Quaternion& q) noexcept;
            Quaternion& operator*= (float S) noexcept;
            Quaternion& operator/= (const Quaternion& q) noexcept;

            // Unary operators
            Quaternion operator+ () const  noexcept { return *this; }
            Quaternion operator- () const noexcept;

            // Quaternion operations
            float Length() const noexcept;
            float LengthSquared() const noexcept;

            void Normalize() noexcept;
            void Normalize(Quaternion& result) const noexcept;

            void Conjugate() noexcept;
            void Conjugate(Quaternion& result) const noexcept;

            void Inverse(Quaternion& result) const noexcept;

            float Dot(const Quaternion& Q) const noexcept;

            void RotateTowards(const Quaternion& target, const float& maxAngle) noexcept;
            void __cdecl RotateTowards(const Quaternion& target, const float& maxAngle, Quaternion& result) const noexcept;

            // Computes rotation about y-axis (y), then x-axis (x), then z-axis (z)
            Vector3 ToEuler() const noexcept;

            // Static functions
            static Quaternion CreateFromAxisAngle(const Vector3& axis, const float& angle) noexcept;

            // Rotates about y-axis (yaw), then x-axis (pitch), then z-axis (roll)
            static Quaternion CreateFromYawPitchRoll(const float& yaw, const float& pitch, const float& roll) noexcept;

            // Rotates about y-axis (angles.y), then x-axis (angles.x), then z-axis (angles.z)
            static Quaternion CreateFromYawPitchRoll(const Vector3& angles) noexcept;

            static Quaternion CreateFromRotationMatrix(const Matrix& M) noexcept;

            static void Lerp(const Quaternion& q1, const Quaternion& q2, const float& t, Quaternion& result) noexcept;
            static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, const float& t) noexcept;

            static void Slerp(const Quaternion& q1, const Quaternion& q2, const float& t, Quaternion& result) noexcept;
            static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, const float& t) noexcept;

            static void Concatenate(const Quaternion& q1, const Quaternion& q2, Quaternion& result) noexcept;
            static Quaternion Concatenate(const Quaternion& q1, const Quaternion& q2) noexcept;

            static void __cdecl FromToRotation(const Vector3& fromDir, const Vector3& toDir, Quaternion& result) noexcept;
            static Quaternion FromToRotation(const Vector3& fromDir, const Vector3& toDir) noexcept;

            static void __cdecl LookRotation(const Vector3& forward, const Vector3& up, Quaternion& result) noexcept;
            static Quaternion LookRotation(const Vector3& forward, const Vector3& up) noexcept;

            static float Angle(const Quaternion& q1, const Quaternion& q2) noexcept;

            // Constants
            static const Quaternion Identity;
        };

        // Binary operators
        Quaternion operator+ (const Quaternion& Q1, const Quaternion& Q2) noexcept;
        Quaternion operator- (const Quaternion& Q1, const Quaternion& Q2) noexcept;
        Quaternion operator* (const Quaternion& Q1, const Quaternion& Q2) noexcept;
        Quaternion operator* (const Quaternion& Q, float S) noexcept;
        Quaternion operator/ (const Quaternion& Q1, const Quaternion& Q2) noexcept;
        Quaternion operator* (float S, const Quaternion& Q) noexcept;

        //------------------------------------------------------------------------------
        // Uint4
        struct UInt4 : public XMUINT4
        {
            UInt4() = default;
            UInt4(const unsigned int& ix, const unsigned int& iy, const unsigned int& iz, const unsigned int& iw) noexcept : XMUINT4(ix, iy, iz, iw) {}
            explicit UInt4(const XMUINT4& v) noexcept : XMUINT4(v.x, v.y, v.z, v.w) {}

            UInt4(const UInt4&) = default;
            UInt4& operator=(const UInt4&) = default;

            UInt4(UInt4&&) = default;
            UInt4& operator=(UInt4&&) = default;
        };

        //------------------------------------------------------------------------------
        // Uint3
        struct UInt3 : public XMUINT3
        {
            UInt3() = default;
            UInt3(const unsigned int& ix, const unsigned int& iy, const unsigned int& iz) noexcept : XMUINT3(ix, iy, iz) {}

            UInt3(const UInt3&) = default;
            UInt3& operator=(const UInt3&) = default;

            UInt3(UInt3&&) = default;
            UInt3& operator=(UInt3&&) = default;
        };

        //------------------------------------------------------------------------------
        // Ray
        class Ray
        {
        public:
            Vector3 position;
            Vector3 direction;

            Ray() noexcept : position(0, 0, 0), direction(0, 0, 1) {}
            Ray(const Vector3& pos, const Vector3& dir) noexcept : position(pos), direction(dir) {}

            Ray(const Ray&) = default;
            Ray& operator=(const Ray&) = default;

            Ray(Ray&&) = default;
            Ray& operator=(Ray&&) = default;

            // Comparison operators
            bool operator == (const Ray& r) const noexcept;
            bool operator != (const Ray& r) const noexcept;

            // Ray operations
            bool Intersects(const BoundingSphere& sphere, _Out_ float& Dist) const noexcept;
            bool Intersects(const BoundingBox& box, _Out_ float& Dist) const noexcept;
            bool Intersects(const Vector3& tri0, const Vector3& tri1, const Vector3& tri2, _Out_ float& Dist) const noexcept;
            bool Intersects(const Plane& plane, _Out_ float& Dist) const noexcept;
        };

#include "CusMath.inl"

    } // namespace PTH

} // namespace DirectX

//------------------------------------------------------------------------------
// Support for PTH and Standard C++ Library containers
namespace std
{

    template<> struct less<DirectX::PTH::Vector2>
    {
        bool operator()(const DirectX::PTH::Vector2& V1, const DirectX::PTH::Vector2& V2) const noexcept
        {
            return ((V1.x < V2.x) || ((V1.x == V2.x) && (V1.y < V2.y)));
        }
    };

    template<> struct less<DirectX::PTH::Vector3>
    {
        bool operator()(const DirectX::PTH::Vector3& V1, const DirectX::PTH::Vector3& V2) const noexcept
        {
            return ((V1.x < V2.x)
                || ((V1.x == V2.x) && (V1.y < V2.y))
                || ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z < V2.z)));
        }
    };

    template<> struct less<DirectX::PTH::Vector4>
    {
        bool operator()(const DirectX::PTH::Vector4& V1, const DirectX::PTH::Vector4& V2) const noexcept
        {
            return ((V1.x < V2.x)
                || ((V1.x == V2.x) && (V1.y < V2.y))
                || ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z < V2.z))
                || ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z == V2.z) && (V1.w < V2.w)));
        }
    };

    template<> struct less<DirectX::PTH::Matrix>
    {
        bool operator()(const DirectX::PTH::Matrix& M1, const DirectX::PTH::Matrix& M2) const noexcept
        {
            if (M1._11 != M2._11) return M1._11 < M2._11;
            if (M1._12 != M2._12) return M1._12 < M2._12;
            if (M1._13 != M2._13) return M1._13 < M2._13;
            if (M1._14 != M2._14) return M1._14 < M2._14;
            if (M1._21 != M2._21) return M1._21 < M2._21;
            if (M1._22 != M2._22) return M1._22 < M2._22;
            if (M1._23 != M2._23) return M1._23 < M2._23;
            if (M1._24 != M2._24) return M1._24 < M2._24;
            if (M1._31 != M2._31) return M1._31 < M2._31;
            if (M1._32 != M2._32) return M1._32 < M2._32;
            if (M1._33 != M2._33) return M1._33 < M2._33;
            if (M1._34 != M2._34) return M1._34 < M2._34;
            if (M1._41 != M2._41) return M1._41 < M2._41;
            if (M1._42 != M2._42) return M1._42 < M2._42;
            if (M1._43 != M2._43) return M1._43 < M2._43;
            if (M1._44 != M2._44) return M1._44 < M2._44;

            return false;
        }
    };

    template<> struct less<DirectX::PTH::Plane>
    {
        bool operator()(const DirectX::PTH::Plane& P1, const DirectX::PTH::Plane& P2) const noexcept
        {
            return ((P1.x < P2.x)
                || ((P1.x == P2.x) && (P1.y < P2.y))
                || ((P1.x == P2.x) && (P1.y == P2.y) && (P1.z < P2.z))
                || ((P1.x == P2.x) && (P1.y == P2.y) && (P1.z == P2.z) && (P1.w < P2.w)));
        }
    };

    template<> struct less<DirectX::PTH::Quaternion>
    {
        bool operator()(const DirectX::PTH::Quaternion& Q1, const DirectX::PTH::Quaternion& Q2) const noexcept
        {
            return ((Q1.x < Q2.x)
                || ((Q1.x == Q2.x) && (Q1.y < Q2.y))
                || ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z < Q2.z))
                || ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z == Q2.z) && (Q1.w < Q2.w)));
        }
    };

    template<> struct less<DirectX::PTH::Ray>
    {
        bool operator()(const DirectX::PTH::Ray& R1, const DirectX::PTH::Ray& R2) const noexcept
        {
            if (R1.position.x != R2.position.x) return R1.position.x < R2.position.x;
            if (R1.position.y != R2.position.y) return R1.position.y < R2.position.y;
            if (R1.position.z != R2.position.z) return R1.position.z < R2.position.z;

            if (R1.direction.x != R2.direction.x) return R1.direction.x < R2.direction.x;
            if (R1.direction.y != R2.direction.y) return R1.direction.y < R2.direction.y;
            if (R1.direction.z != R2.direction.z) return R1.direction.z < R2.direction.z;

            return false;
        }
    };

} // namespace std

#ifdef __clang__
#pragma clang diagnostic pop
#endif
