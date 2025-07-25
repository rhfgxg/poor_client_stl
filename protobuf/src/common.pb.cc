// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: common.proto

#include "common.pb.h"

#include <algorithm>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
#include "google/protobuf/generated_message_tctable_impl.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::google::protobuf;
namespace _pbi = ::google::protobuf::internal;
namespace _fl = ::google::protobuf::internal::field_layout;
namespace rpc_server {
}  // namespace rpc_server
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_common_2eproto[3];
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_common_2eproto = nullptr;
const ::uint32_t TableStruct_common_2eproto::offsets[1] = {};
static constexpr ::_pbi::MigrationSchema* schemas = nullptr;
static constexpr ::_pb::Message* const* file_default_instances = nullptr;
const char descriptor_table_protodef_common_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
    "\n\014common.proto\022\nrpc_server*\261\003\n\013ServiceTy"
    "pe\022\020\n\014REQ_REGISTER\020\000\022\020\n\014RES_REGISTER\020\001\022\r"
    "\n\tREQ_LOGIN\020\002\022\r\n\tRES_LOGIN\020\003\022\016\n\nREQ_LOGO"
    "UT\020\004\022\016\n\nRES_LOGOUT\020\005\022\027\n\023REQ_CHANGE_PASSW"
    "ORD\020\006\022\027\n\023RES_CHANGE_PASSWORD\020\007\022\021\n\rREQ_HE"
    "ARTBEAT\020\010\022\021\n\rRES_HEARTBEAT\020\t\022\037\n\033REQ_FILE"
    "_TRANSMISSION_READY\020\n\022\037\n\033RES_FILE_TRANSM"
    "ISSION_READY\020\013\022\023\n\017REQ_FILE_UPLOAD\020\014\022\023\n\017R"
    "ES_FILE_UPLOAD\020\r\022\025\n\021REQ_FILE_DOWNLOAD\020\016\022"
    "\025\n\021RES_FILE_DOWNLOAD\020\017\022\023\n\017REQ_FILE_DELET"
    "E\020\020\022\023\n\017RES_FILE_DELETE\020\021\022\021\n\rREQ_FILE_LIS"
    "T\020\022\022\021\n\rRES_FILE_LIST\020\023*S\n\010LogLevel\022\t\n\005DE"
    "BUG\020\000\022\010\n\004INFO\020\001\022\010\n\004WARN\020\002\022\017\n\013ERROR_LEVEL"
    "\020\003\022\014\n\010CRITICAL\020\004\022\t\n\005TRACE\020\005*\332\001\n\013LogCateg"
    "ory\022\024\n\020STARTUP_SHUTDOWN\020\000\022\030\n\024APPLICATION"
    "_ACTIVITY\020\001\022\023\n\017CONNECTION_POOL\020\002\022\025\n\021SYST"
    "EM_MONITORING\020\003\022\r\n\tHEARTBEAT\020\004\022\014\n\010SECURI"
    "TY\020\005\022\031\n\025CONFIGURATION_CHANGES\020\006\022\027\n\023DATAB"
    "ASE_OPERATIONS\020\007\022\021\n\rUSER_ACTIVITY\020\010\022\013\n\007N"
    "ETWORK\020\tb\006proto3"
};
static ::absl::once_flag descriptor_table_common_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_common_2eproto = {
    false,
    false,
    776,
    descriptor_table_protodef_common_2eproto,
    "common.proto",
    &descriptor_table_common_2eproto_once,
    nullptr,
    0,
    0,
    schemas,
    file_default_instances,
    TableStruct_common_2eproto::offsets,
    nullptr,
    file_level_enum_descriptors_common_2eproto,
    file_level_service_descriptors_common_2eproto,
};

// This function exists to be marked as weak.
// It can significantly speed up compilation by breaking up LLVM's SCC
// in the .pb.cc translation units. Large translation units see a
// reduction of more than 35% of walltime for optimized builds. Without
// the weak attribute all the messages in the file, including all the
// vtables and everything they use become part of the same SCC through
// a cycle like:
// GetMetadata -> descriptor table -> default instances ->
//   vtables -> GetMetadata
// By adding a weak function here we break the connection from the
// individual vtables back into the descriptor table.
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_common_2eproto_getter() {
  return &descriptor_table_common_2eproto;
}
// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2
static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_common_2eproto(&descriptor_table_common_2eproto);
namespace rpc_server {
const ::google::protobuf::EnumDescriptor* ServiceType_descriptor() {
  ::google::protobuf::internal::AssignDescriptors(&descriptor_table_common_2eproto);
  return file_level_enum_descriptors_common_2eproto[0];
}
PROTOBUF_CONSTINIT const uint32_t ServiceType_internal_data_[] = {
    1310720u, 0u, };
bool ServiceType_IsValid(int value) {
  return 0 <= value && value <= 19;
}
const ::google::protobuf::EnumDescriptor* LogLevel_descriptor() {
  ::google::protobuf::internal::AssignDescriptors(&descriptor_table_common_2eproto);
  return file_level_enum_descriptors_common_2eproto[1];
}
PROTOBUF_CONSTINIT const uint32_t LogLevel_internal_data_[] = {
    393216u, 0u, };
bool LogLevel_IsValid(int value) {
  return 0 <= value && value <= 5;
}
const ::google::protobuf::EnumDescriptor* LogCategory_descriptor() {
  ::google::protobuf::internal::AssignDescriptors(&descriptor_table_common_2eproto);
  return file_level_enum_descriptors_common_2eproto[2];
}
PROTOBUF_CONSTINIT const uint32_t LogCategory_internal_data_[] = {
    655360u, 0u, };
bool LogCategory_IsValid(int value) {
  return 0 <= value && value <= 9;
}
// @@protoc_insertion_point(namespace_scope)
}  // namespace rpc_server
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
#include "google/protobuf/port_undef.inc"
