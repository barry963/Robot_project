// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: messages_robocup_ssl_cmd.proto

#ifndef PROTOBUF_messages_5frobocup_5fssl_5fcmd_2eproto__INCLUDED
#define PROTOBUF_messages_5frobocup_5fssl_5fcmd_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_messages_5frobocup_5fssl_5fcmd_2eproto();
void protobuf_AssignDesc_messages_5frobocup_5fssl_5fcmd_2eproto();
void protobuf_ShutdownFile_messages_5frobocup_5fssl_5fcmd_2eproto();

class SSL_RobotCmd;
class SSL_RadioFrame;

// ===================================================================

class SSL_RobotCmd : public ::google::protobuf::Message {
 public:
  SSL_RobotCmd();
  virtual ~SSL_RobotCmd();
  
  SSL_RobotCmd(const SSL_RobotCmd& from);
  
  inline SSL_RobotCmd& operator=(const SSL_RobotCmd& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const SSL_RobotCmd& default_instance();
  
  void Swap(SSL_RobotCmd* other);
  
  // implements Message ----------------------------------------------
  
  SSL_RobotCmd* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SSL_RobotCmd& from);
  void MergeFrom(const SSL_RobotCmd& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint32 team = 1;
  inline bool has_team() const;
  inline void clear_team();
  static const int kTeamFieldNumber = 1;
  inline ::google::protobuf::uint32 team() const;
  inline void set_team(::google::protobuf::uint32 value);
  
  // required uint32 id = 2;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 2;
  inline ::google::protobuf::uint32 id() const;
  inline void set_id(::google::protobuf::uint32 value);
  
  // required bool drib_dir = 3;
  inline bool has_drib_dir() const;
  inline void clear_drib_dir();
  static const int kDribDirFieldNumber = 3;
  inline bool drib_dir() const;
  inline void set_drib_dir(bool value);
  
  // required bool is_chip = 4;
  inline bool has_is_chip() const;
  inline void clear_is_chip();
  static const int kIsChipFieldNumber = 4;
  inline bool is_chip() const;
  inline void set_is_chip(bool value);
  
  // required bool send_back = 5;
  inline bool has_send_back() const;
  inline void clear_send_back();
  static const int kSendBackFieldNumber = 5;
  inline bool send_back() const;
  inline void set_send_back(bool value);
  
  // required bool forcekick = 6;
  inline bool has_forcekick() const;
  inline void clear_forcekick();
  static const int kForcekickFieldNumber = 6;
  inline bool forcekick() const;
  inline void set_forcekick(bool value);
  
  // required uint32 drib_speed = 7;
  inline bool has_drib_speed() const;
  inline void clear_drib_speed();
  static const int kDribSpeedFieldNumber = 7;
  inline ::google::protobuf::uint32 drib_speed() const;
  inline void set_drib_speed(::google::protobuf::uint32 value);
  
  // required float vx = 8;
  inline bool has_vx() const;
  inline void clear_vx();
  static const int kVxFieldNumber = 8;
  inline float vx() const;
  inline void set_vx(float value);
  
  // required float vy = 9;
  inline bool has_vy() const;
  inline void clear_vy();
  static const int kVyFieldNumber = 9;
  inline float vy() const;
  inline void set_vy(float value);
  
  // required float va = 10;
  inline bool has_va() const;
  inline void clear_va();
  static const int kVaFieldNumber = 10;
  inline float va() const;
  inline void set_va(float value);
  
  // required uint32 kick_time = 11;
  inline bool has_kick_time() const;
  inline void clear_kick_time();
  static const int kKickTimeFieldNumber = 11;
  inline ::google::protobuf::uint32 kick_time() const;
  inline void set_kick_time(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:SSL_RobotCmd)
 private:
  inline void set_has_team();
  inline void clear_has_team();
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_drib_dir();
  inline void clear_has_drib_dir();
  inline void set_has_is_chip();
  inline void clear_has_is_chip();
  inline void set_has_send_back();
  inline void clear_has_send_back();
  inline void set_has_forcekick();
  inline void clear_has_forcekick();
  inline void set_has_drib_speed();
  inline void clear_has_drib_speed();
  inline void set_has_vx();
  inline void clear_has_vx();
  inline void set_has_vy();
  inline void clear_has_vy();
  inline void set_has_va();
  inline void clear_has_va();
  inline void set_has_kick_time();
  inline void clear_has_kick_time();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint32 team_;
  ::google::protobuf::uint32 id_;
  bool drib_dir_;
  bool is_chip_;
  bool send_back_;
  bool forcekick_;
  ::google::protobuf::uint32 drib_speed_;
  float vx_;
  float vy_;
  float va_;
  ::google::protobuf::uint32 kick_time_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(11 + 31) / 32];
  
  friend void  protobuf_AddDesc_messages_5frobocup_5fssl_5fcmd_2eproto();
  friend void protobuf_AssignDesc_messages_5frobocup_5fssl_5fcmd_2eproto();
  friend void protobuf_ShutdownFile_messages_5frobocup_5fssl_5fcmd_2eproto();
  
  void InitAsDefaultInstance();
  static SSL_RobotCmd* default_instance_;
};
// -------------------------------------------------------------------

class SSL_RadioFrame : public ::google::protobuf::Message {
 public:
  SSL_RadioFrame();
  virtual ~SSL_RadioFrame();
  
  SSL_RadioFrame(const SSL_RadioFrame& from);
  
  inline SSL_RadioFrame& operator=(const SSL_RadioFrame& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const SSL_RadioFrame& default_instance();
  
  void Swap(SSL_RadioFrame* other);
  
  // implements Message ----------------------------------------------
  
  SSL_RadioFrame* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SSL_RadioFrame& from);
  void MergeFrom(const SSL_RadioFrame& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required double time_stamp = 1;
  inline bool has_time_stamp() const;
  inline void clear_time_stamp();
  static const int kTimeStampFieldNumber = 1;
  inline double time_stamp() const;
  inline void set_time_stamp(double value);
  
  // repeated .SSL_RobotCmd cmds = 2;
  inline int cmds_size() const;
  inline void clear_cmds();
  static const int kCmdsFieldNumber = 2;
  inline const ::SSL_RobotCmd& cmds(int index) const;
  inline ::SSL_RobotCmd* mutable_cmds(int index);
  inline ::SSL_RobotCmd* add_cmds();
  inline const ::google::protobuf::RepeatedPtrField< ::SSL_RobotCmd >&
      cmds() const;
  inline ::google::protobuf::RepeatedPtrField< ::SSL_RobotCmd >*
      mutable_cmds();
  
  // @@protoc_insertion_point(class_scope:SSL_RadioFrame)
 private:
  inline void set_has_time_stamp();
  inline void clear_has_time_stamp();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  double time_stamp_;
  ::google::protobuf::RepeatedPtrField< ::SSL_RobotCmd > cmds_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_messages_5frobocup_5fssl_5fcmd_2eproto();
  friend void protobuf_AssignDesc_messages_5frobocup_5fssl_5fcmd_2eproto();
  friend void protobuf_ShutdownFile_messages_5frobocup_5fssl_5fcmd_2eproto();
  
  void InitAsDefaultInstance();
  static SSL_RadioFrame* default_instance_;
};
// ===================================================================


// ===================================================================

// SSL_RobotCmd

// required uint32 team = 1;
inline bool SSL_RobotCmd::has_team() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SSL_RobotCmd::set_has_team() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SSL_RobotCmd::clear_has_team() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SSL_RobotCmd::clear_team() {
  team_ = 0u;
  clear_has_team();
}
inline ::google::protobuf::uint32 SSL_RobotCmd::team() const {
  return team_;
}
inline void SSL_RobotCmd::set_team(::google::protobuf::uint32 value) {
  set_has_team();
  team_ = value;
}

// required uint32 id = 2;
inline bool SSL_RobotCmd::has_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void SSL_RobotCmd::set_has_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void SSL_RobotCmd::clear_has_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void SSL_RobotCmd::clear_id() {
  id_ = 0u;
  clear_has_id();
}
inline ::google::protobuf::uint32 SSL_RobotCmd::id() const {
  return id_;
}
inline void SSL_RobotCmd::set_id(::google::protobuf::uint32 value) {
  set_has_id();
  id_ = value;
}

// required bool drib_dir = 3;
inline bool SSL_RobotCmd::has_drib_dir() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void SSL_RobotCmd::set_has_drib_dir() {
  _has_bits_[0] |= 0x00000004u;
}
inline void SSL_RobotCmd::clear_has_drib_dir() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void SSL_RobotCmd::clear_drib_dir() {
  drib_dir_ = false;
  clear_has_drib_dir();
}
inline bool SSL_RobotCmd::drib_dir() const {
  return drib_dir_;
}
inline void SSL_RobotCmd::set_drib_dir(bool value) {
  set_has_drib_dir();
  drib_dir_ = value;
}

// required bool is_chip = 4;
inline bool SSL_RobotCmd::has_is_chip() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void SSL_RobotCmd::set_has_is_chip() {
  _has_bits_[0] |= 0x00000008u;
}
inline void SSL_RobotCmd::clear_has_is_chip() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void SSL_RobotCmd::clear_is_chip() {
  is_chip_ = false;
  clear_has_is_chip();
}
inline bool SSL_RobotCmd::is_chip() const {
  return is_chip_;
}
inline void SSL_RobotCmd::set_is_chip(bool value) {
  set_has_is_chip();
  is_chip_ = value;
}

// required bool send_back = 5;
inline bool SSL_RobotCmd::has_send_back() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void SSL_RobotCmd::set_has_send_back() {
  _has_bits_[0] |= 0x00000010u;
}
inline void SSL_RobotCmd::clear_has_send_back() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void SSL_RobotCmd::clear_send_back() {
  send_back_ = false;
  clear_has_send_back();
}
inline bool SSL_RobotCmd::send_back() const {
  return send_back_;
}
inline void SSL_RobotCmd::set_send_back(bool value) {
  set_has_send_back();
  send_back_ = value;
}

// required bool forcekick = 6;
inline bool SSL_RobotCmd::has_forcekick() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void SSL_RobotCmd::set_has_forcekick() {
  _has_bits_[0] |= 0x00000020u;
}
inline void SSL_RobotCmd::clear_has_forcekick() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void SSL_RobotCmd::clear_forcekick() {
  forcekick_ = false;
  clear_has_forcekick();
}
inline bool SSL_RobotCmd::forcekick() const {
  return forcekick_;
}
inline void SSL_RobotCmd::set_forcekick(bool value) {
  set_has_forcekick();
  forcekick_ = value;
}

// required uint32 drib_speed = 7;
inline bool SSL_RobotCmd::has_drib_speed() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void SSL_RobotCmd::set_has_drib_speed() {
  _has_bits_[0] |= 0x00000040u;
}
inline void SSL_RobotCmd::clear_has_drib_speed() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void SSL_RobotCmd::clear_drib_speed() {
  drib_speed_ = 0u;
  clear_has_drib_speed();
}
inline ::google::protobuf::uint32 SSL_RobotCmd::drib_speed() const {
  return drib_speed_;
}
inline void SSL_RobotCmd::set_drib_speed(::google::protobuf::uint32 value) {
  set_has_drib_speed();
  drib_speed_ = value;
}

// required float vx = 8;
inline bool SSL_RobotCmd::has_vx() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void SSL_RobotCmd::set_has_vx() {
  _has_bits_[0] |= 0x00000080u;
}
inline void SSL_RobotCmd::clear_has_vx() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void SSL_RobotCmd::clear_vx() {
  vx_ = 0;
  clear_has_vx();
}
inline float SSL_RobotCmd::vx() const {
  return vx_;
}
inline void SSL_RobotCmd::set_vx(float value) {
  set_has_vx();
  vx_ = value;
}

// required float vy = 9;
inline bool SSL_RobotCmd::has_vy() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void SSL_RobotCmd::set_has_vy() {
  _has_bits_[0] |= 0x00000100u;
}
inline void SSL_RobotCmd::clear_has_vy() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void SSL_RobotCmd::clear_vy() {
  vy_ = 0;
  clear_has_vy();
}
inline float SSL_RobotCmd::vy() const {
  return vy_;
}
inline void SSL_RobotCmd::set_vy(float value) {
  set_has_vy();
  vy_ = value;
}

// required float va = 10;
inline bool SSL_RobotCmd::has_va() const {
  return (_has_bits_[0] & 0x00000200u) != 0;
}
inline void SSL_RobotCmd::set_has_va() {
  _has_bits_[0] |= 0x00000200u;
}
inline void SSL_RobotCmd::clear_has_va() {
  _has_bits_[0] &= ~0x00000200u;
}
inline void SSL_RobotCmd::clear_va() {
  va_ = 0;
  clear_has_va();
}
inline float SSL_RobotCmd::va() const {
  return va_;
}
inline void SSL_RobotCmd::set_va(float value) {
  set_has_va();
  va_ = value;
}

// required uint32 kick_time = 11;
inline bool SSL_RobotCmd::has_kick_time() const {
  return (_has_bits_[0] & 0x00000400u) != 0;
}
inline void SSL_RobotCmd::set_has_kick_time() {
  _has_bits_[0] |= 0x00000400u;
}
inline void SSL_RobotCmd::clear_has_kick_time() {
  _has_bits_[0] &= ~0x00000400u;
}
inline void SSL_RobotCmd::clear_kick_time() {
  kick_time_ = 0u;
  clear_has_kick_time();
}
inline ::google::protobuf::uint32 SSL_RobotCmd::kick_time() const {
  return kick_time_;
}
inline void SSL_RobotCmd::set_kick_time(::google::protobuf::uint32 value) {
  set_has_kick_time();
  kick_time_ = value;
}

// -------------------------------------------------------------------

// SSL_RadioFrame

// required double time_stamp = 1;
inline bool SSL_RadioFrame::has_time_stamp() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SSL_RadioFrame::set_has_time_stamp() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SSL_RadioFrame::clear_has_time_stamp() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SSL_RadioFrame::clear_time_stamp() {
  time_stamp_ = 0;
  clear_has_time_stamp();
}
inline double SSL_RadioFrame::time_stamp() const {
  return time_stamp_;
}
inline void SSL_RadioFrame::set_time_stamp(double value) {
  set_has_time_stamp();
  time_stamp_ = value;
}

// repeated .SSL_RobotCmd cmds = 2;
inline int SSL_RadioFrame::cmds_size() const {
  return cmds_.size();
}
inline void SSL_RadioFrame::clear_cmds() {
  cmds_.Clear();
}
inline const ::SSL_RobotCmd& SSL_RadioFrame::cmds(int index) const {
  return cmds_.Get(index);
}
inline ::SSL_RobotCmd* SSL_RadioFrame::mutable_cmds(int index) {
  return cmds_.Mutable(index);
}
inline ::SSL_RobotCmd* SSL_RadioFrame::add_cmds() {
  return cmds_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::SSL_RobotCmd >&
SSL_RadioFrame::cmds() const {
  return cmds_;
}
inline ::google::protobuf::RepeatedPtrField< ::SSL_RobotCmd >*
SSL_RadioFrame::mutable_cmds() {
  return &cmds_;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_messages_5frobocup_5fssl_5fcmd_2eproto__INCLUDED
