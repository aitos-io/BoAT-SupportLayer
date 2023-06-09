/**
****************************************************************************************
* @FilePath: se05x_enums.h
* @Author: aitos
* @Date: 2022-11-21 17:27:08
* @LastEditors:
* @LastEditTime: 2022-11-21 17:27:08
* @Descripttion:
****************************************************************************************
*/
/*
 *
 * Copyright 2019,2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/** @file */

#ifndef SE05x_ENUMS_H
#define SE05x_ENUMS_H
#include "boattypes.h"

/** SE05X's key IDs */
typedef BUINT32 SE05x_KeyID_t;
/** SE05X key's max attempts */
typedef BUINT16 SE05x_MaxAttemps_t;

/* secp256k1 : SECG curve over a 256 bit prime field */
#define EC_PARAM_secp256k1_prime                        \
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,     \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
        0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFC, 0x2F
#define EC_PARAM_secp256k1_a                            \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define EC_PARAM_secp256k1_b                            \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07
#define EC_PARAM_secp256k1_x                            \
    0x79, 0xBE, 0x66, 0x7E, 0xF9, 0xDC, 0xBB, 0xAC,     \
        0x55, 0xA0, 0x62, 0x95, 0xCE, 0x87, 0x0B, 0x07, \
        0x02, 0x9B, 0xFC, 0xDB, 0x2D, 0xCE, 0x28, 0xD9, \
        0x59, 0xF2, 0x81, 0x5B, 0x16, 0xF8, 0x17, 0x98
#define EC_PARAM_secp256k1_y                            \
    0x48, 0x3A, 0xDA, 0x77, 0x26, 0xA3, 0xC4, 0x65,     \
        0x5D, 0xA4, 0xFB, 0xFC, 0x0E, 0x11, 0x08, 0xA8, \
        0xFD, 0x17, 0xB4, 0x48, 0xA6, 0x85, 0x54, 0x19, \
        0x9C, 0x47, 0xD0, 0x8F, 0xFB, 0x10, 0xD4, 0xB8
#define EC_PARAM_secp256k1_order                        \
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,     \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, \
        0xBA, 0xAE, 0xDC, 0xE6, 0xAF, 0x48, 0xA0, 0x3B, \
        0xBF, 0xD2, 0x5E, 0x8C, 0xD0, 0x36, 0x41, 0x41

/* + more or less machine Generated */

/** @addtogroup se05x_types
 *
 * @{ */

/** Reserved idendntifiers of the Applet */
typedef enum
{
    /** Invalid */
    kSE05x_AppletResID_NA = 0,
    /** An authentication object which allows the user to switch
     * LockState of the applet. The LockState defines whether the
     * applet is transport locked or not. */
    kSE05x_AppletResID_TRANSPORT = 0x7FFF0200,
    /** A device unique NIST P-256 key pair which contains SK.SE.ECKA
     * and PK.SE.ECKA in ECKey session context. */
    kSE05x_AppletResID_KP_ECKEY_USER = 0x7FFF0201,
    /** A device unique NIST P-256 key pair which contains SK.SE.ECKA
     * and PK.SE.ECKA in ECKey session context; A constant card
     * challenge (all zeroes) is applicable. */
    kSE05x_AppletResID_KP_ECKEY_IMPORT = 0x7FFF0202,
    /* Reserved Key @ location 0x7FFF0203 */
    /** An authentication object which allows the user to change the
    applet variant. */
    kSE05x_AppletResID_FEATURE = 0x7FFF0204,
    /** An authentication object which allows the user to delete all
    objects, except trust provisioned by NXP objects. */
    kSE05x_AppletResID_FACTORY_RESET = 0x7FFF0205,
    /** A BinaryFile Secure Object which holds the device unique
     *  ID. This file cannot be overwritten or deleted. */
    kSE05x_AppletResID_UNIQUE_ID = 0x7FFF0206,
    /** An authentication object which allows the user to change the
     * platform SCP requirements, i.e. make platform SCP mandatory or
     * not, using SetPlatformSCPRequest. Mandatory means full security,
     * i.e. command & response MAC and encryption. Only SCP03 will be
     * sufficient. */
    kSE05x_AppletResID_PLATFORM_SCP = 0x7FFF0207,
    /** An authentication object which grants access to the I2C master
     * feature. If the credential is not present, access to I2C master
     * is allowed in general. Otherwise, a session using this
     * credential shall be established and I2CM commands shall be sent
     * within this session. */
    kSE05x_AppletResID_I2CM_ACCESS = 0x7FFF0208,
    /** An authentication object which grants access to the
     * SetLockState command */
    kSE05x_AppletResID_RESTRICT = 0x7FFF020A,
    /** SPAKE2P_M_P256_UNCOMPRESSED KEY*/
    kSE05x_AppletResID_SPAKE2P_M_P256_UNCOMPRESSED = 0x7FFF0210,
    /** SPAKE2P_N_P256_UNCOMPRESSED KEY*/
    kSE05x_AppletResID_SPAKE2P_N_P256_UNCOMPRESSED = 0x7FFF0211,
    /** SPAKE2P_M_P384_UNCOMPRESSED KEY*/
    kSE05x_AppletResID_SPAKE2P_M_P384_UNCOMPRESSED = 0x7FFF0212,
    /** SPAKE2P_N_P384_UNCOMPRESSED KEY*/
    kSE05x_AppletResID_SPAKE2P_N_P384_UNCOMPRESSED = 0x7FFF0213,
    /** SPAKE2P_M_P521_UNCOMPRESSED KEY*/
    kSE05x_AppletResID_SPAKE2P_M_P521_UNCOMPRESSED = 0x7FFF0214,
    /** SPAKE2P_N_P521_UNCOMPRESSED KEY*/
    kSE05x_AppletResID_SPAKE2P_N_P521_UNCOMPRESSED = 0x7FFF0215,

} SE05x_AppletResID_t;

/** Mapping of 2 byte return code */
typedef enum
{
    /** Invalid */
    kSE05x_SW12_NA = 0,
    /** No Error */
    kSE05x_SW12_NO_ERROR = 0x9000,
    /** Conditions not satisfied */
    kSE05x_SW12_CONDITIONS_NOT_SATISFIED = 0x6985,
    /** Security status not satisfied. */
    kSE05x_SW12_SECURITY_STATUS = 0x6982,
    /** Wrong data provided. */
    kSE05x_SW12_WRONG_DATA = 0x6A80,
    /** Data invalid - policy set invalid for the given object */
    kSE05x_SW12_DATA_INVALID = 0x6984,
    /** Command not allowed - access denied based on object policy */
    kSE05x_SW12_COMMAND_NOT_ALLOWED = 0x6986,
} SE05x_SW12_t;

/** Values for INS in ISO7816 APDU */
typedef enum
{
    /** Invalid */
    kSE05x_INS_NA = 0,
    /** 3 MSBit for instruction characteristics. */
    kSE05x_INS_MASK_INS_CHAR = 0xE0,
    /** 5 LSBit for instruction */
    kSE05x_INS_MASK_INSTRUCTION = 0x1F,

    /** Mask for transient object creation, can only be combined with INS_WRITE. */
    kSE05x_INS_TRANSIENT = 0x80,
    /** Mask for authentication object creation, can only be combined with INS_WRITE */
    kSE05x_INS_AUTH_OBJECT = 0x40,
    /** Mask for getting attestation data. */
    kSE05x_INS_ATTEST = 0x20,

    /** Write or create a persistent object. */
    kSE05x_INS_WRITE = 0x01,
    /** Read the object */
    kSE05x_INS_READ = 0x02,
    /** Perform Security Operation */
    kSE05x_INS_CRYPTO = 0x03,
    /** General operation */
    kSE05x_INS_MGMT = 0x04,
    /** Process session command */
    kSE05x_INS_PROCESS = 0x05,
} SE05x_INS_t;

/** When we want to read with attestation */
#define kSE05x_INS_READ_With_Attestation (kSE05x_INS_READ | kSE05x_INS_ATTEST)

/** Values for P1 in ISO7816 APDU */
typedef enum
{
    /** Invalid */
    kSE05x_P1_NA = 0,
    /** Highest bit not used */
    kSE05x_P1_UNUSED = 0x80,
    /** 2 MSBit for key type */
    kSE05x_P1_MASK_KEY_TYPE = 0x60,
    /** 5 LSBit for credential type */
    kSE05x_P1_MASK_CRED_TYPE = 0x1F,

    /** Key pair (private key + public key) */
    kSE05x_P1_KEY_PAIR = 0x60,
    /** Private key */
    kSE05x_P1_PRIVATE = 0x40,
    /** Public key */
    kSE05x_P1_PUBLIC = 0x20,

    kSE05x_P1_DEFAULT = 0x00,
    kSE05x_P1_EC = 0x01,
    kSE05x_P1_RSA = 0x02,
    kSE05x_P1_AES = 0x03,
    kSE05x_P1_DES = 0x04,
    kSE05x_P1_HMAC = 0x05,
    kSE05x_P1_BINARY = 0x06,
    kSE05x_P1_UserID = 0x07,
    kSE05x_P1_COUNTER = 0x08,
    kSE05x_P1_PCR = 0x09,
    kSE05x_P1_CURVE = 0x0B,
    kSE05x_P1_SIGNATURE = 0x0C,
    kSE05x_P1_MAC = 0x0D,
    kSE05x_P1_CIPHER = 0x0E,
    kSE05x_P1_TLS = 0x0F,
    kSE05x_P1_CRYPTO_OBJ = 0x10,
#if SSS_HAVE_SE05X_VER_GTE_06_00
    /** Applet >= 4.4 */
    kSE05x_P1_AEAD = 0x11,
    /** Applet >= 4.4 */
    kSE05x_P1_AEAD_SP800_38D = 0x12,
#endif /* SSS_HAVE_SE05X_VER_GTE_06_00 */
    kSE05x_P1_PAKE = 0x12,
} SE05x_P1_t;

/** Values for P2 in ISO7816 APDU */
typedef enum
{
    /** Invalid */
    kSE05x_P2_DEFAULT = 0x00,
    kSE05x_P2_GENERATE = 0x03,
    kSE05x_P2_CREATE = 0x04,
    kSE05x_P2_SIZE = 0x07,
    kSE05x_P2_SIGN = 0x09,
    kSE05x_P2_VERIFY = 0x0A,
    kSE05x_P2_INIT = 0x0B,
    kSE05x_P2_UPDATE = 0x0C,
    kSE05x_P2_FINAL = 0x0D,
    kSE05x_P2_ONESHOT = 0x0E,
    kSE05x_P2_DH = 0x0F,
    kSE05x_P2_DIVERSIFY = 0x10,
    // kSE05x_P2_AUTH_PART1 = 0x11,
    kSE05x_P2_AUTH_FIRST_PART2 = 0x12,
    kSE05x_P2_AUTH_NONFIRST_PART2 = 0x13,
    kSE05x_P2_DUMP_KEY = 0x14,
    kSE05x_P2_CHANGE_KEY_PART1 = 0x15,
    kSE05x_P2_CHANGE_KEY_PART2 = 0x16,
    kSE05x_P2_KILL_AUTH = 0x17,
    kSE05x_P2_IMPORT = 0x18,
    kSE05x_P2_EXPORT = 0x19,
    kSE05x_P2_SESSION_CREATE = 0x1B,
    kSE05x_P2_SESSION_CLOSE = 0x1C,
    kSE05x_P2_SESSION_REFRESH = 0x1E,
    kSE05x_P2_SESSION_POLICY = 0x1F,
    kSE05x_P2_VERSION = 0x20,
    kSE05x_P2_VERSION_EXT = 0x21,
    kSE05x_P2_MEMORY = 0x22,
    kSE05x_P2_LIST = 0x25,
    kSE05x_P2_TYPE = 0x26,
    kSE05x_P2_EXIST = 0x27,
    kSE05x_P2_DELETE_OBJECT = 0x28,
    kSE05x_P2_DELETE_ALL = 0x2A,
    kSE05x_P2_SESSION_UserID = 0x2C,
    kSE05x_P2_HKDF = 0x2D,
    kSE05x_P2_PBKDF = 0x2E,
    /* Applet >= 4.4 */
    kSE05x_P2_HKDF_EXPAND_ONLY = 0x2F,
    kSE05x_P2_I2CM = 0x30,
    kSE05x_P2_I2CM_ATTESTED = 0x31,
    kSE05x_P2_MAC = 0x32,
    kSE05x_P2_UNLOCK_CHALLENGE = 0x33,
    kSE05x_P2_CURVE_LIST = 0x34,
    kSE05x_P2_SIGN_ECDAA = 0x35,
    kSE05x_P2_ID = 0x36,
    kSE05x_P2_ENCRYPT_ONESHOT = 0x37,
    kSE05x_P2_DECRYPT_ONESHOT = 0x38,
    kSE05x_P2_ATTEST = 0x3A,
    kSE05x_P2_ATTRIBUTES = 0x3B,
    kSE05x_P2_CPLC = 0x3C,
    kSE05x_P2_TIME = 0x3D,
    kSE05x_P2_TRANSPORT = 0x3E,
    kSE05x_P2_VARIANT = 0x3F,
    kSE05x_P2_PARAM = 0x40,
    kSE05x_P2_DELETE_CURVE = 0x41,
    kSE05x_P2_ENCRYPT = 0x42,
    kSE05x_P2_DECRYPT = 0x43,
    kSE05x_P2_VALIDATE = 0x44,
    kSE05x_P2_GENERATE_ONESHOT = 0x45,
    kSE05x_P2_VALIDATE_ONESHOT = 0x46,
    kSE05x_P2_CRYPTO_LIST = 0x47,
    kSE05x_P2_RANDOM = 0x49,
    kSE05x_P2_TLS_PMS = 0x4A,
    kSE05x_P2_TLS_PRF_CLI_HELLO = 0x4B,
    kSE05x_P2_TLS_PRF_SRV_HELLO = 0x4C,
    kSE05x_P2_TLS_PRF_CLI_RND = 0x4D,
    kSE05x_P2_TLS_PRF_SRV_RND = 0x4E,
    kSE05x_P2_TLS_PRF_BOTH = 0x5A,
    kSE05x_P2_RAW = 0x4F,
    kSE05x_P2_IMPORT_EXT = 0x51,
    kSE05x_P2_SCP = 0x52,
    kSE05x_P2_AUTH_FIRST_PART1 = 0x53,
    kSE05x_P2_AUTH_NONFIRST_PART1 = 0x54,
#if SSS_HAVE_SE05X_VER_GTE_06_00
    kSE05x_P2_CM_COMMAND = 0x55,
    kSE05x_P2_MODE_OF_OPERATION = 0x56,
    kSE05x_P2_RESTRICT = 0x57,
    kSE05x_P2_SANITY = 0x58,
    kSE05x_P2_DH_REVERSE = 0x59,
    kSE05x_P2_READ_STATE = 0x5B,
#endif
#if SSS_HAVE_SE05X_VER_GTE_07_02
    kSE05x_P2_ECPM = 0x62
#endif
} SE05x_P2_t;

/** Different TAG Values to talk to SE05X IoT Applet */
typedef enum
{
    /** Invalid */
    kSE05x_TAG_NA = 0,
    kSE05x_TAG_SESSION_ID = 0x10,
    kSE05x_TAG_POLICY = 0x11,
    kSE05x_TAG_MAX_ATTEMPTS = 0x12,
    kSE05x_TAG_IMPORT_AUTH_DATA = 0x13,
    kSE05x_TAG_IMPORT_AUTH_KEY_ID = 0x14,
    kSE05x_TAG_POLICY_CHECK = 0x15,
    kSE05x_TAG_1 = 0x41,
    kSE05x_TAG_2 = 0x42,
    kSE05x_TAG_3 = 0x43,
    kSE05x_TAG_4 = 0x44,
    kSE05x_TAG_5 = 0x45,
    kSE05x_TAG_6 = 0x46,
    kSE05x_TAG_7 = 0x47,
    kSE05x_TAG_8 = 0x48,
    kSE05x_TAG_9 = 0x49,
    kSE05x_TAG_10 = 0x4A,
    kSE05x_TAG_11 = 0x4B,
#if SSS_HAVE_SE05X_VER_GTE_07_02
    kSE05x_TAG_TIMESTAMP = 0x4F,
    kSE05x_TAG_SIGNATURE = 0x52,
#endif
    kSE05x_GP_TAG_CONTRL_REF_PARM = 0xA6,
    kSE05x_GP_TAG_AID = 0x4F,
    kSE05x_GP_TAG_KEY_TYPE = 0x80,
    kSE05x_GP_TAG_KEY_LEN = 0x81,
    kSE05x_GP_TAG_GET_DATA = 0x83,
    kSE05x_GP_TAG_DR_SE = 0x85,
    kSE05x_GP_TAG_RECEIPT = 0x86,
    kSE05x_GP_TAG_SCP_PARMS = 0x90,

#if SSS_HAVE_APPLET_SE051_UWB
    /** FiRaLite applet specific Tags */
    kSE05x_FIRALITE_OID_TAG = 0x06,
    kSE05x_FIRALITE_OPTSA_TAG = 0x80,
    kSE05x_FIRALITE_SESSION_ID_TAG = 0x80,
    kSE05x_FIRALITE_DISPATCH_TAG = 0x81,
    kSE05x_FIRALITE_TUNNEL_TAG = 0x81,
    kSE05x_FIRALITE_PROPRIETARY_CMD_TAG = 0x70,
    kSE05x_FIRALITE_TAG_FCI_TEMPLATE = 0x6F,
    kSE05x_FIRALITE_TAG_PROP_RSP_TEMPLATE = 0x71,
    kSE05x_FIRALITE_TAG_STATUS = 0x80,
    kSE05x_FIRALITE_TAG_NOTIFICATION_FORMAT = 0x80,
    kSE05x_FIRALITE_TAG_COMMAND_OR_RESPONSE = 0x81,
    kSE05x_FIRALITE_TAG_EVENT_ID = 0x81,
    kSE05x_FIRALITE_TAG_EVENT_DATA = 0x82,
    kSE05x_FIRALITE_TAG_AID = 0x84,
    kSE05x_FIRALITE_TAG_PROPRIETARY = 0x85,
    kSE05x_FIRALITE_TAG_NOTIFICATION = 0xE1,

    /* Wrapp Data specific Tags*/
    kSE05x_SUS_TAG_RANGING_SESSION_KEY = 0xC0,
    kSE05x_SUS_TAG_RESPONDER_RANGING_KEY,
    kSE05x_SUS_TAG_PROXIMITY_DISTANCE,
    kSE05x_SUS_TAG_ANGLE_OF_ARRIVAL,
    kSE05x_SUS_TAG_CLIENT_DATA,
    kSE05x_SUS_TAG_TRANSACTION_IDENTIFIER,
    kSE05x_SUS_TAG_KEY_IDENTIFIER,
    kSE05x_SUS_TAG_ARIBTARY_DATA,
    kSE05x_SUS_TAG_FINALIZATION_APPLET_AID = 0XCE,
    kSE05x_SUS_TAG_SESSION_ID,
    kSE05x_SUS_TAG_RANDOM_NUM = 0xD0,
    kSE05x_SUS_TAG_WRDS,
#endif
} SE05x_TAG_t;

/** Type of Object */
typedef enum
{
    /**  */
    kSE05x_SecObjTyp_NA = 0x00,
    /**  */
    kSE05x_SecObjTyp_EC_KEY_PAIR = 0x01,
    /**  */
    kSE05x_SecObjTyp_EC_PRIV_KEY = 0x02,
    /**  */
    kSE05x_SecObjTyp_EC_PUB_KEY = 0x03,
    /**  */
    kSE05x_SecObjTyp_RSA_KEY_PAIR = 0x04,
    /**  */
    kSE05x_SecObjTyp_RSA_KEY_PAIR_CRT = 0x05,
    /**  */
    kSE05x_SecObjTyp_RSA_PRIV_KEY = 0x06,
    /**  */
    kSE05x_SecObjTyp_RSA_PRIV_KEY_CRT = 0x07,
    /**  */
    kSE05x_SecObjTyp_RSA_PUB_KEY = 0x08,
    /**  */
    kSE05x_SecObjTyp_AES_KEY = 0x09,
    /**  */
    kSE05x_SecObjTyp_DES_KEY = 0x0A,
    /**  */
    kSE05x_SecObjTyp_BINARY_FILE = 0x0B,
    /**  */
    kSE05x_SecObjTyp_UserID = 0x0C,
    /**  */
    kSE05x_SecObjTyp_COUNTER = 0x0D,
    /**  */
    kSE05x_SecObjTyp_PCR = 0x0F,
    /**  */
    kSE05x_SecObjTyp_CURVE = 0x10,
    /**  */
    kSE05x_SecObjTyp_HMAC_KEY = 0x11,
} SE05x_SecObjTyp_t;

/** @copydoc SE05x_SecObjTyp_t */
typedef SE05x_SecObjTyp_t SE05x_SecureObjectType_t;

/** ECC Curve Identifiers */
typedef enum
{
    /** Invalid */
    kSE05x_ECCurve_NA = 0x00,
    kSE05x_ECCurve_NIST_P192 = 0x01,
    kSE05x_ECCurve_NIST_P224 = 0x02,
    kSE05x_ECCurve_NIST_P256 = 0x03,
    kSE05x_ECCurve_NIST_P384 = 0x04,
    kSE05x_ECCurve_NIST_P521 = 0x05,
    kSE05x_ECCurve_Brainpool160 = 0x06,
    kSE05x_ECCurve_Brainpool192 = 0x07,
    kSE05x_ECCurve_Brainpool224 = 0x08,
    kSE05x_ECCurve_Brainpool256 = 0x09,
    kSE05x_ECCurve_Brainpool320 = 0x0A,
    kSE05x_ECCurve_Brainpool384 = 0x0B,
    kSE05x_ECCurve_Brainpool512 = 0x0C,
    kSE05x_ECCurve_Secp160k1 = 0x0D,
    kSE05x_ECCurve_Secp192k1 = 0x0E,
    kSE05x_ECCurve_Secp224k1 = 0x0F,
    kSE05x_ECCurve_Secp256k1 = 0x10,
    kSE05x_ECCurve_TPM_ECC_BN_P256 = 0x11,
    /** Not Weierstrass */
    kSE05x_ECCurve_ECC_ED_25519 = 0x40,
    kSE05x_ECCurve_ECC_MONT_DH_25519 = 0x41,
    /** Not Weierstrass */
    kSE05x_ECCurve_ECC_MONT_DH_448 = 0x43,
} SE05x_ECCurve_t;

/** @brief I2C Master micro operation */
typedef enum
{
    kSE05x_TAG_I2CM_Config = 0x01,
    kSE05x_TAG_I2CM_Write = 0x03,
    kSE05x_TAG_I2CM_Read = 0x04,
} SE05x_I2CM_TAG_t;

/** Result of operations */
typedef enum
{
    /** Invalid */
    kSE05x_Result_NA = 0,
    kSE05x_Result_SUCCESS = 0x01,
    kSE05x_Result_FAILURE = 0x02,
} SE05x_Result_t;

/** Part of the asymmetric key */
typedef enum
{
    kSE05x_KeyPart_NA = kSE05x_P1_DEFAULT,
    /** Key pair (private key + public key) */
    kSE05x_KeyPart_Pair = kSE05x_P1_KEY_PAIR,
    /** Private key */
    kSE05x_KeyPart_Private = kSE05x_P1_PRIVATE,
    /** Public key */
    kSE05x_KeyPart_Public = kSE05x_P1_PUBLIC,
} SE05x_KeyPart_t;

/** In case the read is attested */
typedef enum
{
    kSE05x_AttestationType_None = 0,
    kSE05x_AttestationType_AUTH = kSE05x_INS_AUTH_OBJECT,
} SE05x_AttestationType_t;
/** Symmetric keys */
typedef enum
{
    kSE05x_SymmKeyType_NA = 0,
    kSE05x_SymmKeyType_AES = kSE05x_P1_AES,
    kSE05x_SymmKeyType_DES = kSE05x_P1_DES,
    kSE05x_SymmKeyType_HMAC = kSE05x_P1_HMAC,
    kSE05x_SymmKeyType_CMAC = kSE05x_P1_AES,
} SE05x_SymmKeyType_t;
/** Parameters while setting the curve */
typedef enum
{ /** Invalid */
  kSE05x_ECCurveParam_NA = 0,
  kSE05x_ECCurveParam_PARAM_A = 0x01,
  kSE05x_ECCurveParam_PARAM_B = 0x02,
  kSE05x_ECCurveParam_PARAM_G = 0x04,
  kSE05x_ECCurveParam_PARAM_N = 0x08,
  kSE05x_ECCurveParam_PARAM_PRIME = 0x10,
} SE05x_ECCurveParam_t;

/** Crypto object identifiers */
typedef enum
{
    /** Invalid */
    kSE05x_CryptoObject_NA = 0,
    kSE05x_CryptoObject_DIGEST_SHA,
    kSE05x_CryptoObject_DIGEST_SHA224,
    kSE05x_CryptoObject_DIGEST_SHA256,
    kSE05x_CryptoObject_DIGEST_SHA384,
    kSE05x_CryptoObject_DIGEST_SHA512,
    kSE05x_CryptoObject_DES_CBC_NOPAD,
    kSE05x_CryptoObject_DES_CBC_ISO9797_M1,
    kSE05x_CryptoObject_DES_CBC_ISO9797_M2,
    kSE05x_CryptoObject_DES_CBC_PKCS5,
    kSE05x_CryptoObject_DES_ECB_NOPAD,
    kSE05x_CryptoObject_DES_ECB_ISO9797_M1,
    kSE05x_CryptoObject_DES_ECB_ISO9797_M2,
    kSE05x_CryptoObject_DES_ECB_PKCS5,
    kSE05x_CryptoObject_AES_ECB_NOPAD,
    kSE05x_CryptoObject_AES_CBC_NOPAD,
    kSE05x_CryptoObject_AES_CBC_ISO9797_M1,
    kSE05x_CryptoObject_AES_CBC_ISO9797_M2,
    kSE05x_CryptoObject_AES_CBC_PKCS5,
    kSE05x_CryptoObject_AES_CTR,
    kSE05x_CryptoObject_AES_CTR_INT_IV,
    kSE05x_CryptoObject_HMAC_SHA1,
    kSE05x_CryptoObject_HMAC_SHA256,
    kSE05x_CryptoObject_HMAC_SHA384,
    kSE05x_CryptoObject_HMAC_SHA512,
    kSE05x_CryptoObject_CMAC_128,
    kSE05x_CryptoObject_AES_GCM,
    kSE05x_CryptoObject_AES_GCM_INT_IV,
    kSE05x_CryptoObject_AES_CCM,
    kSE05x_CryptoObject_AES_CCM_INT_IV,
    kSE05x_CryptoObject_PAKE_TYPE_A,
    kSE05x_CryptoObject_PAKE_TYPE_B,
    kSE05x_CryptoObject_End,
} SE05x_CryptoObject_t;

/** @copydoc SE05x_CryptoObject_t */
#define SE05x_CryptoObjectID_t SE05x_CryptoObject_t

/** Cryptographic context for operation */
typedef enum
{
    /** Invalid */
    kSE05x_CryptoContext_NA = 0,
    /** For DigestInit/DigestUpdate/DigestFinal */
    kSE05x_CryptoContext_DIGEST = 0x01,
    /** For CipherInit/CipherUpdate/CipherFinal */
    kSE05x_CryptoContext_CIPHER = 0x02,
    /** For MACInit/MACUpdate/MACFinal */
    kSE05x_CryptoContext_SIGNATURE = 0x03,
    /** For AEADInit/AEADUpdate/AEADFinal */
    kSE05x_CryptoContext_AEAD = 0x04,
    /** For PAKE */
    kSE05x_CryptoContext_PAKE = 0x05,
} SE05x_CryptoContext_t;

/** Hashing/Digest algorithms */
typedef enum
{
    /** Invalid */
    kSE05x_DigestMode_NA = 0,
    kSE05x_DigestMode_NO_HASH = 0x00,
    kSE05x_DigestMode_SHA = 0x01,
    /** Not supported */
    kSE05x_DigestMode_SHA224 = 0x07,
    kSE05x_DigestMode_SHA256 = 0x04,
    kSE05x_DigestMode_SHA384 = 0x05,
    kSE05x_DigestMode_SHA512 = 0x06,
} SE05x_DigestMode_t;

/** Symmetric cipher modes */
typedef enum
{
    /** Invalid */
    kSE05x_CipherMode_NA = 0,
    /** Typically using DESKey identifiers */
    kSE05x_CipherMode_DES_CBC_NOPAD = 0x01,
    /** Typically using DESKey identifiers */
    kSE05x_CipherMode_DES_CBC_ISO9797_M1 = 0x02,
    /** Typically using DESKey identifiers */
    kSE05x_CipherMode_DES_CBC_ISO9797_M2 = 0x03,
    /** NOT SUPPORTED */
    kSE05x_CipherMode_DES_CBC_PKCS5 = 0x04,
    /** Typically using DESKey identifiers */
    kSE05x_CipherMode_DES_ECB_NOPAD = 0x05,
    /** NOT SUPPORTED */
    kSE05x_CipherMode_DES_ECB_ISO9797_M1 = 0x06,
    /** NOT SUPPORTED */
    kSE05x_CipherMode_DES_ECB_ISO9797_M2 = 0x07,
    /** NOT SUPPORTED */
    kSE05x_CipherMode_DES_ECB_PKCS5 = 0x08,
    /** Typically using AESKey identifiers */
    kSE05x_CipherMode_AES_ECB_NOPAD = 0x0E,
    /** Typically using AESKey identifiers */
    kSE05x_CipherMode_AES_CBC_NOPAD = 0x0D,
    /** Typically using AESKey identifiers */
    kSE05x_CipherMode_AES_CBC_ISO9797_M1 = 0x16,
    /** Typically using AESKey identifiers */
    kSE05x_CipherMode_AES_CBC_ISO9797_M2 = 0x17,
    /** NOT SUPPORTED */
    kSE05x_CipherMode_AES_CBC_PKCS5 = 0x18,
    /** Typically using AEAD GCM mode */
    kSE05x_CipherMode_AES_GCM = 0xB0,
    /** Typically using AESKey identifiers */
    kSE05x_CipherMode_AES_CTR = 0xF0,
    /** Typically using AESKey CTR mode with internal IV Gen */
    /** Only used by MW. Change to kSE05x_CipherMode_AES_CTR when sending to SE */
    kSE05x_CipherMode_AES_CTR_INT_IV = 0xF1,
    /** Typically using AEAD GCM with internal IV Gen */
    kSE05x_CipherMode_AES_GCM_INT_IV = 0xF3,
    /** Typically using AEAD CCM mode */
    kSE05x_CipherMode_AES_CCM = 0xF4,
    /** Typically using AEAD CCM with internal IV Gen */
    kSE05x_CipherMode_AES_CCM_INT_IV = 0xF5,
} SE05x_CipherMode_t;

/** HMAC/CMAC Algorithms  */
typedef enum
{
    /** Invalid */
    kSE05x_MACAlgo_NA = 0,
    kSE05x_MACAlgo_HMAC_SHA1 = 0x18,
    kSE05x_MACAlgo_HMAC_SHA256 = 0x19,
    kSE05x_MACAlgo_HMAC_SHA384 = 0x1A,
    kSE05x_MACAlgo_HMAC_SHA512 = 0x1B,
    kSE05x_MACAlgo_CMAC_128 = 0x31,
    kSE05x_MACAlgo_DES_CMAC8 = 0x7A,
} SE05x_MACAlgo_t;

/** AEAD Algorithms */
typedef enum
{
    /** Invalid */
    kSE05x_AeadAlgo_NA = 0,
    kSE05x_AeadGCMAlgo = 0xB0,
    kSE05x_AeadGCM_IVAlgo = 0xF3,
    kSE05x_AeadCCMAlgo = 0xF4,
} SE05x_AeadAlgo_t;

/** PAKE Mode */
typedef enum
{
    /** Invalid */
    kSE05x_SPAKE2PLUS_NA = 0,
    kSE05x_SPAKE2PLUS_P256_SHA256_HKDF_HMAC = 0x01,
    kSE05x_SPAKE2PLUS_P256_SHA512_HKDF_HMAC = 0x02,
    kSE05x_SPAKE2PLUS_P384_SHA256_HKDF_HMAC = 0x03,
    kSE05x_SPAKE2PLUS_P384_SHA512_HKDF_HMAC = 0x04,
    kSE05x_SPAKE2PLUS_P521_SHA512_HKDF_HMAC = 0x05,
    // kSE05x_SPAKE2PLUS_ED25519_SHA256_HKDF_HMAC = 0x06, //Not supported
    // kSE05x_SPAKE2PLUS_ED448_SHA512_HKDF_HMAC = 0x07, //Not supported
    kSE05x_SPAKE2PLUS_P256_SHA256_HKDF_CMAC = 0x08,
    kSE05x_SPAKE2PLUS_P256_SHA512_HKDF_CMAC = 0x09,
} SE05x_PAKEMode_t;

/** Cyrpto module subtype */
typedef union {
    /** In case it's digest */
    SE05x_DigestMode_t digest;
    /** In case it's cipher */
    SE05x_CipherMode_t cipher;
    /** In case it's mac */
    SE05x_MACAlgo_t mac;
    /** In case it's aead */
    SE05x_AeadAlgo_t aead;
    /** In case it's pake */
    SE05x_PAKEMode_t pakeMode;
    /** Accessing 8 bit value for APDUs */
    BUINT8 union_8bit;
} SE05x_CryptoModeSubType_t;

/** Different signature algorithms for EC */
typedef enum
{
    /** Invalid */
    kSE05x_ECSignatureAlgo_NA = 0,
    /** NOT SUPPORTED */
    kSE05x_ECSignatureAlgo_PLAIN = 0x09,
    kSE05x_ECSignatureAlgo_SHA = 0x11,
    kSE05x_ECSignatureAlgo_SHA_224 = 0x25,
    kSE05x_ECSignatureAlgo_SHA_256 = 0x21,
    kSE05x_ECSignatureAlgo_SHA_384 = 0x22,
    kSE05x_ECSignatureAlgo_SHA_512 = 0x26,
} SE05x_ECSignatureAlgo_t;

/** Different signature algorithms for ED */
typedef enum
{
    /** Invalid */
    kSE05x_EDSignatureAlgo_NA = 0,
    /** Message input must be plain Data. Pure EDDSA algorithm */
    kSE05x_EDSignatureAlgo_ED25519PURE_SHA_512 = 0xA3,
} SE05x_EDSignatureAlgo_t;

/*Encrypt or decrypt */
typedef enum
{
    kSE05x_Cipher_Oper_NA = 0,
    kSE05x_Cipher_Oper_Encrypt = kSE05x_P2_ENCRYPT,
    kSE05x_Cipher_Oper_Decrypt = kSE05x_P2_DECRYPT,
} SE05x_Cipher_Oper_t;

/** MAC operations */
typedef enum
{
    kSE05x_Mac_Oper_NA = 0,
    kSE05x_Mac_Oper_Generate = kSE05x_P2_GENERATE,
    kSE05x_Mac_Oper_Validate = kSE05x_P2_VALIDATE,
} SE05x_Mac_Oper_t;

/** Data for available memory */
typedef enum
{
    /** Invalid */
    kSE05x_MemoryType_NA = 0,
    /** Persistent memory */
    kSE05x_MemoryType_PERSISTENT = 0x01,
    /** Transient memory, clear on reset */
    kSE05x_MemoryType_TRANSIENT_RESET = 0x02,
    /** Transient memory, clear on deselect */
    kSE05x_MemoryType_TRANSIENT_DESELECT = 0x03,
} SE05x_MemoryType_t;

/** Attestation */
typedef enum
{
    kSE05x_AttestationAlgo_NA = 0,
    kSE05x_AttestationAlgo_EC_PLAIN = kSE05x_ECSignatureAlgo_PLAIN,
    kSE05x_AttestationAlgo_EC_SHA = kSE05x_ECSignatureAlgo_SHA,
    kSE05x_AttestationAlgo_EC_SHA_224 = kSE05x_ECSignatureAlgo_SHA_224,
    kSE05x_AttestationAlgo_EC_SHA_256 = kSE05x_ECSignatureAlgo_SHA_256,
    kSE05x_AttestationAlgo_EC_SHA_384 = kSE05x_ECSignatureAlgo_SHA_384,
    kSE05x_AttestationAlgo_EC_SHA_512 = kSE05x_ECSignatureAlgo_SHA_512,
    kSE05x_AttestationAlgo_ED25519PURE_SHA_512 = kSE05x_EDSignatureAlgo_ED25519PURE_SHA_512,

} SE05x_AttestationAlgo_t;
/*!
 *@}
 */
/* end of se05x_types */

#endif /* SE05x_ENUMS_H */
