/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: peer/signed_cc_dep_spec.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "peer/signed_cc_dep_spec.pb-c.h"
void   protos__signed_chaincode_deployment_spec__init
                     (Protos__SignedChaincodeDeploymentSpec         *message)
{
  static Protos__SignedChaincodeDeploymentSpec init_value = PROTOS__SIGNED_CHAINCODE_DEPLOYMENT_SPEC__INIT;
  *message = init_value;
}
size_t protos__signed_chaincode_deployment_spec__get_packed_size
                     (const Protos__SignedChaincodeDeploymentSpec *message)
{
  assert(message->base.descriptor == &protos__signed_chaincode_deployment_spec__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t protos__signed_chaincode_deployment_spec__pack
                     (const Protos__SignedChaincodeDeploymentSpec *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &protos__signed_chaincode_deployment_spec__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t protos__signed_chaincode_deployment_spec__pack_to_buffer
                     (const Protos__SignedChaincodeDeploymentSpec *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &protos__signed_chaincode_deployment_spec__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Protos__SignedChaincodeDeploymentSpec *
       protos__signed_chaincode_deployment_spec__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Protos__SignedChaincodeDeploymentSpec *)
     protobuf_c_message_unpack (&protos__signed_chaincode_deployment_spec__descriptor,
                                allocator, len, data);
}
void   protos__signed_chaincode_deployment_spec__free_unpacked
                     (Protos__SignedChaincodeDeploymentSpec *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &protos__signed_chaincode_deployment_spec__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor protos__signed_chaincode_deployment_spec__field_descriptors[3] =
{
  {
    "chaincode_deployment_spec",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Protos__SignedChaincodeDeploymentSpec, has_chaincode_deployment_spec),
    offsetof(Protos__SignedChaincodeDeploymentSpec, chaincode_deployment_spec),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "instantiation_policy",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Protos__SignedChaincodeDeploymentSpec, has_instantiation_policy),
    offsetof(Protos__SignedChaincodeDeploymentSpec, instantiation_policy),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "owner_endorsements",
    3,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Protos__SignedChaincodeDeploymentSpec, n_owner_endorsements),
    offsetof(Protos__SignedChaincodeDeploymentSpec, owner_endorsements),
    &protos__endorsement__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned protos__signed_chaincode_deployment_spec__field_indices_by_name[] = {
  0,   /* field[0] = chaincode_deployment_spec */
  1,   /* field[1] = instantiation_policy */
  2,   /* field[2] = owner_endorsements */
};
static const ProtobufCIntRange protos__signed_chaincode_deployment_spec__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor protos__signed_chaincode_deployment_spec__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "protos.SignedChaincodeDeploymentSpec",
  "SignedChaincodeDeploymentSpec",
  "Protos__SignedChaincodeDeploymentSpec",
  "protos",
  sizeof(Protos__SignedChaincodeDeploymentSpec),
  3,
  protos__signed_chaincode_deployment_spec__field_descriptors,
  protos__signed_chaincode_deployment_spec__field_indices_by_name,
  1,  protos__signed_chaincode_deployment_spec__number_ranges,
  (ProtobufCMessageInit) protos__signed_chaincode_deployment_spec__init,
  NULL,NULL,NULL    /* reserved[123] */
};
