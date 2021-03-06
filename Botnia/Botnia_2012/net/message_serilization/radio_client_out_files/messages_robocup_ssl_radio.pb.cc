// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "messages_robocup_ssl_radio.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* SSL_RadioPacket_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  SSL_RadioPacket_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_messages_5frobocup_5fssl_5fradio_2eproto() {
  protobuf_AddDesc_messages_5frobocup_5fssl_5fradio_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "messages_robocup_ssl_radio.proto");
  GOOGLE_CHECK(file != NULL);
  SSL_RadioPacket_descriptor_ = file->message_type(0);
  static const int SSL_RadioPacket_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SSL_RadioPacket, radioframe_),
  };
  SSL_RadioPacket_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      SSL_RadioPacket_descriptor_,
      SSL_RadioPacket::default_instance_,
      SSL_RadioPacket_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SSL_RadioPacket, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SSL_RadioPacket, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(SSL_RadioPacket));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_messages_5frobocup_5fssl_5fradio_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    SSL_RadioPacket_descriptor_, &SSL_RadioPacket::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_messages_5frobocup_5fssl_5fradio_2eproto() {
  delete SSL_RadioPacket::default_instance_;
  delete SSL_RadioPacket_reflection_;
}

void protobuf_AddDesc_messages_5frobocup_5fssl_5fradio_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::protobuf_AddDesc_messages_5frobocup_5fssl_5fcmd_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n messages_robocup_ssl_radio.proto\032\036mess"
    "ages_robocup_ssl_cmd.proto\"6\n\017SSL_RadioP"
    "acket\022#\n\nRadioFrame\030\001 \001(\0132\017.SSL_RadioFra"
    "me", 122);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "messages_robocup_ssl_radio.proto", &protobuf_RegisterTypes);
  SSL_RadioPacket::default_instance_ = new SSL_RadioPacket();
  SSL_RadioPacket::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_messages_5frobocup_5fssl_5fradio_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_messages_5frobocup_5fssl_5fradio_2eproto {
  StaticDescriptorInitializer_messages_5frobocup_5fssl_5fradio_2eproto() {
    protobuf_AddDesc_messages_5frobocup_5fssl_5fradio_2eproto();
  }
} static_descriptor_initializer_messages_5frobocup_5fssl_5fradio_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int SSL_RadioPacket::kRadioFrameFieldNumber;
#endif  // !_MSC_VER

SSL_RadioPacket::SSL_RadioPacket()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void SSL_RadioPacket::InitAsDefaultInstance() {
  radioframe_ = const_cast< ::SSL_RadioFrame*>(&::SSL_RadioFrame::default_instance());
}

SSL_RadioPacket::SSL_RadioPacket(const SSL_RadioPacket& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void SSL_RadioPacket::SharedCtor() {
  _cached_size_ = 0;
  radioframe_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

SSL_RadioPacket::~SSL_RadioPacket() {
  SharedDtor();
}

void SSL_RadioPacket::SharedDtor() {
  if (this != default_instance_) {
    delete radioframe_;
  }
}

void SSL_RadioPacket::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* SSL_RadioPacket::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SSL_RadioPacket_descriptor_;
}

const SSL_RadioPacket& SSL_RadioPacket::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_messages_5frobocup_5fssl_5fradio_2eproto();  return *default_instance_;
}

SSL_RadioPacket* SSL_RadioPacket::default_instance_ = NULL;

SSL_RadioPacket* SSL_RadioPacket::New() const {
  return new SSL_RadioPacket;
}

void SSL_RadioPacket::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_radioframe()) {
      if (radioframe_ != NULL) radioframe_->::SSL_RadioFrame::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool SSL_RadioPacket::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .SSL_RadioFrame RadioFrame = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_radioframe()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void SSL_RadioPacket::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional .SSL_RadioFrame RadioFrame = 1;
  if (has_radioframe()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->radioframe(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* SSL_RadioPacket::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional .SSL_RadioFrame RadioFrame = 1;
  if (has_radioframe()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->radioframe(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int SSL_RadioPacket::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional .SSL_RadioFrame RadioFrame = 1;
    if (has_radioframe()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->radioframe());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void SSL_RadioPacket::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const SSL_RadioPacket* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const SSL_RadioPacket*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void SSL_RadioPacket::MergeFrom(const SSL_RadioPacket& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_radioframe()) {
      mutable_radioframe()->::SSL_RadioFrame::MergeFrom(from.radioframe());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void SSL_RadioPacket::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SSL_RadioPacket::CopyFrom(const SSL_RadioPacket& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SSL_RadioPacket::IsInitialized() const {
  
  if (has_radioframe()) {
    if (!this->radioframe().IsInitialized()) return false;
  }
  return true;
}

void SSL_RadioPacket::Swap(SSL_RadioPacket* other) {
  if (other != this) {
    std::swap(radioframe_, other->radioframe_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata SSL_RadioPacket::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = SSL_RadioPacket_descriptor_;
  metadata.reflection = SSL_RadioPacket_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
