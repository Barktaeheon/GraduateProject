// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/unittest_import_public_lite.proto
// Protobuf C++ Version: 4.25.1

#ifndef GOOGLE_PROTOBUF_INCLUDED_google_2fprotobuf_2funittest_5fimport_5fpublic_5flite_2eproto_2epb_2eh
#define GOOGLE_PROTOBUF_INCLUDED_google_2fprotobuf_2funittest_5fimport_5fpublic_5flite_2eproto_2epb_2eh

#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "google/protobuf/port_def.inc"
#if PROTOBUF_VERSION < 4025000
#error "This file was generated by a newer version of protoc which is"
#error "incompatible with your Protocol Buffer headers. Please update"
#error "your headers."
#endif  // PROTOBUF_VERSION

#if 4025001 < PROTOBUF_MIN_PROTOC_VERSION
#error "This file was generated by an older version of protoc which is"
#error "incompatible with your Protocol Buffer headers. Please"
#error "regenerate this file with a newer version of protoc."
#endif  // PROTOBUF_MIN_PROTOC_VERSION
#include "google/protobuf/port_undef.inc"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/arena.h"
#include "google/protobuf/arenastring.h"
#include "google/protobuf/generated_message_tctable_decl.h"
#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/metadata_lite.h"
#include "google/protobuf/message_lite.h"
#include "google/protobuf/repeated_field.h"  // IWYU pragma: export
#include "google/protobuf/extension_set.h"  // IWYU pragma: export
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"

#define PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2funittest_5fimport_5fpublic_5flite_2eproto

namespace google {
namespace protobuf {
namespace internal {
class AnyMetadata;
}  // namespace internal
}  // namespace protobuf
}  // namespace google

// Internal implementation detail -- do not use these members.
struct TableStruct_google_2fprotobuf_2funittest_5fimport_5fpublic_5flite_2eproto {
  static const ::uint32_t offsets[];
};
namespace protobuf_unittest_import {
class PublicImportMessageLite;
struct PublicImportMessageLiteDefaultTypeInternal;
extern PublicImportMessageLiteDefaultTypeInternal _PublicImportMessageLite_default_instance_;
}  // namespace protobuf_unittest_import
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google

namespace protobuf_unittest_import {

// ===================================================================


// -------------------------------------------------------------------

class PublicImportMessageLite final :
    public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:protobuf_unittest_import.PublicImportMessageLite) */ {
 public:
  inline PublicImportMessageLite() : PublicImportMessageLite(nullptr) {}
  ~PublicImportMessageLite() override;
  template<typename = void>
  explicit PROTOBUF_CONSTEXPR PublicImportMessageLite(::google::protobuf::internal::ConstantInitialized);

  inline PublicImportMessageLite(const PublicImportMessageLite& from)
      : PublicImportMessageLite(nullptr, from) {}
  PublicImportMessageLite(PublicImportMessageLite&& from) noexcept
    : PublicImportMessageLite() {
    *this = ::std::move(from);
  }

  inline PublicImportMessageLite& operator=(const PublicImportMessageLite& from) {
    CopyFrom(from);
    return *this;
  }
  inline PublicImportMessageLite& operator=(PublicImportMessageLite&& from) noexcept {
    if (this == &from) return *this;
    if (GetArena() == from.GetArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const std::string& unknown_fields() const
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.unknown_fields<std::string>(::google::protobuf::internal::GetEmptyString);
  }
  inline std::string* mutable_unknown_fields()
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.mutable_unknown_fields<std::string>();
  }

  static const PublicImportMessageLite& default_instance() {
    return *internal_default_instance();
  }
  static inline const PublicImportMessageLite* internal_default_instance() {
    return reinterpret_cast<const PublicImportMessageLite*>(
               &_PublicImportMessageLite_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(PublicImportMessageLite& a, PublicImportMessageLite& b) {
    a.Swap(&b);
  }
  inline void Swap(PublicImportMessageLite* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() != nullptr &&
        GetArena() == other->GetArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() == other->GetArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::google::protobuf::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(PublicImportMessageLite* other) {
    if (other == this) return;
    ABSL_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  PublicImportMessageLite* New(::google::protobuf::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<PublicImportMessageLite>(arena);
  }
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)  final;
  void CopyFrom(const PublicImportMessageLite& from);
  void MergeFrom(const PublicImportMessageLite& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  ::size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::google::protobuf::internal::ParseContext* ctx) final;
  ::uint8_t* _InternalSerialize(
      ::uint8_t* target, ::google::protobuf::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const { return _impl_._cached_size_.Get(); }

  private:
  ::google::protobuf::internal::CachedSize* AccessCachedSize() const final;
  void SharedCtor(::google::protobuf::Arena* arena);
  void SharedDtor();
  void InternalSwap(PublicImportMessageLite* other);

  private:
  friend class ::google::protobuf::internal::AnyMetadata;
  static ::absl::string_view FullMessageName() {
    return "protobuf_unittest_import.PublicImportMessageLite";
  }
  protected:
  explicit PublicImportMessageLite(::google::protobuf::Arena* arena);
  PublicImportMessageLite(::google::protobuf::Arena* arena, const PublicImportMessageLite& from);
  public:

  std::string GetTypeName() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kEFieldNumber = 1,
  };
  // optional int32 e = 1;
  bool has_e() const;
  void clear_e() ;
  ::int32_t e() const;
  void set_e(::int32_t value);

  private:
  ::int32_t _internal_e() const;
  void _internal_set_e(::int32_t value);

  public:
  // @@protoc_insertion_point(class_scope:protobuf_unittest_import.PublicImportMessageLite)
 private:
  class _Internal;

  friend class ::google::protobuf::internal::TcParser;
  static const ::google::protobuf::internal::TcParseTable<
      0, 1, 0,
      0, 2>
      _table_;
  friend class ::google::protobuf::MessageLite;
  friend class ::google::protobuf::Arena;
  template <typename T>
  friend class ::google::protobuf::Arena::InternalHelper;
  using InternalArenaConstructable_ = void;
  using DestructorSkippable_ = void;
  struct Impl_ {

        inline explicit constexpr Impl_(
            ::google::protobuf::internal::ConstantInitialized) noexcept;
        inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                              ::google::protobuf::Arena* arena);
        inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                              ::google::protobuf::Arena* arena, const Impl_& from);
    ::google::protobuf::internal::HasBits<1> _has_bits_;
    mutable ::google::protobuf::internal::CachedSize _cached_size_;
    ::int32_t e_;
    PROTOBUF_TSAN_DECLARE_MEMBER
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_google_2fprotobuf_2funittest_5fimport_5fpublic_5flite_2eproto;
};

// ===================================================================




// ===================================================================


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// PublicImportMessageLite

// optional int32 e = 1;
inline bool PublicImportMessageLite::has_e() const {
  bool value = (_impl_._has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline void PublicImportMessageLite::clear_e() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.e_ = 0;
  _impl_._has_bits_[0] &= ~0x00000001u;
}
inline ::int32_t PublicImportMessageLite::e() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest_import.PublicImportMessageLite.e)
  return _internal_e();
}
inline void PublicImportMessageLite::set_e(::int32_t value) {
  _internal_set_e(value);
  // @@protoc_insertion_point(field_set:protobuf_unittest_import.PublicImportMessageLite.e)
}
inline ::int32_t PublicImportMessageLite::_internal_e() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.e_;
}
inline void PublicImportMessageLite::_internal_set_e(::int32_t value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_._has_bits_[0] |= 0x00000001u;
  _impl_.e_ = value;
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)
}  // namespace protobuf_unittest_import


// @@protoc_insertion_point(global_scope)

#include "google/protobuf/port_undef.inc"

#endif  // GOOGLE_PROTOBUF_INCLUDED_google_2fprotobuf_2funittest_5fimport_5fpublic_5flite_2eproto_2epb_2eh
