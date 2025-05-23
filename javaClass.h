/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

/**\file
   \brief Java classfile representation
*/
#ifndef CLASSDESC_JAVACLASS_H
#define CLASSDESC_JAVACLASS_H

#ifdef _CLASSDESC
#pragma omit pack classdesc::u2
#pragma omit pack classdesc::u4
#pragma omit pack classdesc::u8
#pragma omit pack classdesc::cp_info
#pragma omit pack classdesc::attribute_info
#pragma omit pack classdesc::ConstantPoolVector
#pragma omit unpack classdesc::u2
#pragma omit unpack classdesc::u4
#pragma omit unpack classdesc::u8
#pragma omit unpack classdesc::cp_info
#pragma omit unpack classdesc::attribute_info
#pragma omit unpack classdesc::ConstantPoolVector
#pragma omit dump classdesc::u2
#pragma omit dump classdesc::u4
#pragma omit dump classdesc::u8
#pragma omit dump classdesc::attribute_info
#pragma omit dump classdesc::cp_info
#pragma omit javaClass classdesc::cp_info
#endif

#include "pack_base.h"
#include "classdesc_access.h"
#include <vector>

namespace classdesc
{

  typedef unsigned char u1;

  // 2, 4 and 8 byte data types. Specialised serialisation methods provided
  // to handle JVM's byte ordering.

  struct u2
  {
    unsigned short v;
    operator short() const {return v;}
    u2(): v(0) {}
    u2(short v): v(v) {}
    short operator=(short x) {return v=x;}
    short operator|=(short x) {return v|=x;}
    short operator&=(short x) {return v&=x;}
    bool operator==(const u2& x)  const {return v==x.v;}
    u2 operator++() {v++; return *this;}
  };

  struct u4
  {
    unsigned int v;
    operator unsigned() const {return v;}
    u4(): v(0) {}
    u4(unsigned v): v(v) {}
    unsigned operator=(unsigned x) {return v=x;}
    bool operator==(const u4& x)  const {return v==x.v;}
  };

  struct u8
  {
    unsigned long long v;
    operator long long() const {return v;}
    u8(): v(0) {}
    u8(unsigned v): v(v) {}
    unsigned operator=(unsigned x) {return v=x;}
    bool operator==(const u8& x)  const {return v==x.v;}
  };

  // a variant of vector that serialises the size to a 2 byte field
  template <class T> struct JavaClassVector: public std::vector<T>
  {
    typedef u2 size_type;
    JavaClassVector(size_t sz=0): std::vector<T>(sz) {}
    size_type size() const {return std::vector<T>::size();}
  };

  template <class T> struct is_sequence<classdesc::JavaClassVector<T> >: public true_type {};

  // constants imported from jdk: classfile_constants

enum {
    JVM_ACC_PUBLIC        = 0x0001,
    JVM_ACC_PRIVATE       = 0x0002,
    JVM_ACC_PROTECTED     = 0x0004,
    JVM_ACC_STATIC        = 0x0008,
    JVM_ACC_FINAL         = 0x0010,
    JVM_ACC_SYNCHRONIZED  = 0x0020,
    JVM_ACC_SUPER         = 0x0020,
    JVM_ACC_VOLATILE      = 0x0040,
    JVM_ACC_BRIDGE        = 0x0040,
    JVM_ACC_TRANSIENT     = 0x0080,
    JVM_ACC_VARARGS       = 0x0080,
    JVM_ACC_NATIVE        = 0x0100,
    JVM_ACC_INTERFACE     = 0x0200,
    JVM_ACC_ABSTRACT      = 0x0400,
    JVM_ACC_STRICT        = 0x0800,
    JVM_ACC_SYNTHETIC     = 0x1000,
    JVM_ACC_ANNOTATION    = 0x2000,
    JVM_ACC_ENUM          = 0x4000
};

/* Used in newarray instruction. */

enum {
    JVM_T_BOOLEAN = 4,
    JVM_T_CHAR    = 5,
    JVM_T_FLOAT   = 6,
    JVM_T_DOUBLE  = 7,
    JVM_T_BYTE    = 8,
    JVM_T_SHORT   = 9,
    JVM_T_INT     = 10,
    JVM_T_LONG    = 11
};

/* Constant Pool Entries */

enum {
    JVM_CONSTANT_Utf8                   = 1,
    JVM_CONSTANT_Unicode                = 2, /* unused */
    JVM_CONSTANT_Integer                = 3,
    JVM_CONSTANT_Float                  = 4,
    JVM_CONSTANT_Long                   = 5,      
    JVM_CONSTANT_Double                 = 6,
    JVM_CONSTANT_Class                  = 7,
    JVM_CONSTANT_String                 = 8,
    JVM_CONSTANT_Fieldref               = 9,
    JVM_CONSTANT_Methodref              = 10,
    JVM_CONSTANT_InterfaceMethodref     = 11,
    JVM_CONSTANT_NameAndType            = 12
};

/* StackMapTable type item numbers */

enum {
    JVM_ITEM_Top                = 0,
    JVM_ITEM_Integer            = 1,
    JVM_ITEM_Float              = 2,
    JVM_ITEM_Double             = 3,
    JVM_ITEM_Long               = 4,
    JVM_ITEM_Null               = 5,
    JVM_ITEM_UninitializedThis  = 6,
    JVM_ITEM_Object             = 7,
    JVM_ITEM_Uninitialized      = 8
};

/* Type signatures */

enum {
    JVM_SIGNATURE_ARRAY         = '[',
    JVM_SIGNATURE_BYTE          = 'B',
    JVM_SIGNATURE_CHAR          = 'C',
    JVM_SIGNATURE_CLASS         = 'L',
    JVM_SIGNATURE_ENDCLASS      = ';',
    JVM_SIGNATURE_ENUM          = 'E',
    JVM_SIGNATURE_FLOAT         = 'F',
    JVM_SIGNATURE_DOUBLE        = 'D',
    JVM_SIGNATURE_FUNC          = '(',
    JVM_SIGNATURE_ENDFUNC       = ')',
    JVM_SIGNATURE_INT           = 'I',
    JVM_SIGNATURE_LONG          = 'J',
    JVM_SIGNATURE_SHORT         = 'S',
    JVM_SIGNATURE_VOID          = 'V',
    JVM_SIGNATURE_BOOLEAN       = 'Z'
};

/* Opcodes */

enum {
    JVM_OPC_nop                 = 0,
    JVM_OPC_aconst_null         = 1,
    JVM_OPC_iconst_m1           = 2,
    JVM_OPC_iconst_0            = 3,
    JVM_OPC_iconst_1            = 4,
    JVM_OPC_iconst_2            = 5,
    JVM_OPC_iconst_3            = 6,
    JVM_OPC_iconst_4            = 7,
    JVM_OPC_iconst_5            = 8,
    JVM_OPC_lconst_0            = 9,
    JVM_OPC_lconst_1            = 10,
    JVM_OPC_fconst_0            = 11,
    JVM_OPC_fconst_1            = 12,
    JVM_OPC_fconst_2            = 13,
    JVM_OPC_dconst_0            = 14,
    JVM_OPC_dconst_1            = 15,
    JVM_OPC_bipush              = 16,
    JVM_OPC_sipush              = 17,
    JVM_OPC_ldc                 = 18,
    JVM_OPC_ldc_w               = 19,
    JVM_OPC_ldc2_w              = 20,
    JVM_OPC_iload               = 21,
    JVM_OPC_lload               = 22,
    JVM_OPC_fload               = 23,
    JVM_OPC_dload               = 24,
    JVM_OPC_aload               = 25,
    JVM_OPC_iload_0             = 26,
    JVM_OPC_iload_1             = 27,
    JVM_OPC_iload_2             = 28,
    JVM_OPC_iload_3             = 29,
    JVM_OPC_lload_0             = 30,
    JVM_OPC_lload_1             = 31,
    JVM_OPC_lload_2             = 32,
    JVM_OPC_lload_3             = 33,
    JVM_OPC_fload_0             = 34,
    JVM_OPC_fload_1             = 35,
    JVM_OPC_fload_2             = 36,
    JVM_OPC_fload_3             = 37,
    JVM_OPC_dload_0             = 38,
    JVM_OPC_dload_1             = 39,
    JVM_OPC_dload_2             = 40,
    JVM_OPC_dload_3             = 41,
    JVM_OPC_aload_0             = 42,
    JVM_OPC_aload_1             = 43,
    JVM_OPC_aload_2             = 44,
    JVM_OPC_aload_3             = 45,
    JVM_OPC_iaload              = 46,
    JVM_OPC_laload              = 47,
    JVM_OPC_faload              = 48,
    JVM_OPC_daload              = 49,
    JVM_OPC_aaload              = 50,
    JVM_OPC_baload              = 51,
    JVM_OPC_caload              = 52,
    JVM_OPC_saload              = 53,
    JVM_OPC_istore              = 54,
    JVM_OPC_lstore              = 55,
    JVM_OPC_fstore              = 56,
    JVM_OPC_dstore              = 57,
    JVM_OPC_astore              = 58,
    JVM_OPC_istore_0            = 59,
    JVM_OPC_istore_1            = 60,
    JVM_OPC_istore_2            = 61,
    JVM_OPC_istore_3            = 62,
    JVM_OPC_lstore_0            = 63,
    JVM_OPC_lstore_1            = 64,
    JVM_OPC_lstore_2            = 65,
    JVM_OPC_lstore_3            = 66,
    JVM_OPC_fstore_0            = 67,
    JVM_OPC_fstore_1            = 68,
    JVM_OPC_fstore_2            = 69,
    JVM_OPC_fstore_3            = 70,
    JVM_OPC_dstore_0            = 71,
    JVM_OPC_dstore_1            = 72,
    JVM_OPC_dstore_2            = 73,
    JVM_OPC_dstore_3            = 74,
    JVM_OPC_astore_0            = 75,
    JVM_OPC_astore_1            = 76,
    JVM_OPC_astore_2            = 77,
    JVM_OPC_astore_3            = 78,
    JVM_OPC_iastore             = 79,
    JVM_OPC_lastore             = 80,
    JVM_OPC_fastore             = 81,
    JVM_OPC_dastore             = 82,
    JVM_OPC_aastore             = 83,
    JVM_OPC_bastore             = 84,
    JVM_OPC_castore             = 85,
    JVM_OPC_sastore             = 86,
    JVM_OPC_pop                 = 87,
    JVM_OPC_pop2                = 88,
    JVM_OPC_dup                 = 89,
    JVM_OPC_dup_x1              = 90,
    JVM_OPC_dup_x2              = 91,
    JVM_OPC_dup2                = 92,
    JVM_OPC_dup2_x1             = 93,
    JVM_OPC_dup2_x2             = 94,
    JVM_OPC_swap                = 95,
    JVM_OPC_iadd                = 96,
    JVM_OPC_ladd                = 97,
    JVM_OPC_fadd                = 98,
    JVM_OPC_dadd                = 99,
    JVM_OPC_isub                = 100,
    JVM_OPC_lsub                = 101,
    JVM_OPC_fsub                = 102,
    JVM_OPC_dsub                = 103,
    JVM_OPC_imul                = 104,
    JVM_OPC_lmul                = 105,
    JVM_OPC_fmul                = 106,
    JVM_OPC_dmul                = 107,
    JVM_OPC_idiv                = 108,
    JVM_OPC_ldiv                = 109,
    JVM_OPC_fdiv                = 110,
    JVM_OPC_ddiv                = 111,
    JVM_OPC_irem                = 112,
    JVM_OPC_lrem                = 113,
    JVM_OPC_frem                = 114,
    JVM_OPC_drem                = 115,
    JVM_OPC_ineg                = 116,
    JVM_OPC_lneg                = 117,
    JVM_OPC_fneg                = 118,
    JVM_OPC_dneg                = 119,
    JVM_OPC_ishl                = 120,
    JVM_OPC_lshl                = 121,
    JVM_OPC_ishr                = 122,
    JVM_OPC_lshr                = 123,
    JVM_OPC_iushr               = 124,
    JVM_OPC_lushr               = 125,
    JVM_OPC_iand                = 126,
    JVM_OPC_land                = 127,
    JVM_OPC_ior                 = 128,
    JVM_OPC_lor                 = 129,
    JVM_OPC_ixor                = 130,
    JVM_OPC_lxor                = 131,
    JVM_OPC_iinc                = 132,
    JVM_OPC_i2l                 = 133,
    JVM_OPC_i2f                 = 134,
    JVM_OPC_i2d                 = 135,
    JVM_OPC_l2i                 = 136,
    JVM_OPC_l2f                 = 137,
    JVM_OPC_l2d                 = 138,
    JVM_OPC_f2i                 = 139,
    JVM_OPC_f2l                 = 140,
    JVM_OPC_f2d                 = 141,
    JVM_OPC_d2i                 = 142,
    JVM_OPC_d2l                 = 143,
    JVM_OPC_d2f                 = 144,
    JVM_OPC_i2b                 = 145,
    JVM_OPC_i2c                 = 146,
    JVM_OPC_i2s                 = 147,
    JVM_OPC_lcmp                = 148,
    JVM_OPC_fcmpl               = 149,
    JVM_OPC_fcmpg               = 150,
    JVM_OPC_dcmpl               = 151,
    JVM_OPC_dcmpg               = 152,
    JVM_OPC_ifeq                = 153,
    JVM_OPC_ifne                = 154,
    JVM_OPC_iflt                = 155,
    JVM_OPC_ifge                = 156,
    JVM_OPC_ifgt                = 157,
    JVM_OPC_ifle                = 158,
    JVM_OPC_if_icmpeq           = 159,
    JVM_OPC_if_icmpne           = 160,
    JVM_OPC_if_icmplt           = 161,
    JVM_OPC_if_icmpge           = 162,
    JVM_OPC_if_icmpgt           = 163,
    JVM_OPC_if_icmple           = 164,
    JVM_OPC_if_acmpeq           = 165,
    JVM_OPC_if_acmpne           = 166,
    JVM_OPC_goto                = 167,
    JVM_OPC_jsr                 = 168,
    JVM_OPC_ret                 = 169,
    JVM_OPC_tableswitch         = 170,
    JVM_OPC_lookupswitch        = 171,
    JVM_OPC_ireturn             = 172,
    JVM_OPC_lreturn             = 173,
    JVM_OPC_freturn             = 174,
    JVM_OPC_dreturn             = 175,
    JVM_OPC_areturn             = 176,
    JVM_OPC_return              = 177,
    JVM_OPC_getstatic           = 178,
    JVM_OPC_putstatic           = 179,
    JVM_OPC_getfield            = 180,
    JVM_OPC_putfield            = 181,
    JVM_OPC_invokevirtual       = 182,
    JVM_OPC_invokespecial       = 183,
    JVM_OPC_invokestatic        = 184,
    JVM_OPC_invokeinterface     = 185,
    JVM_OPC_xxxunusedxxx        = 186,
    JVM_OPC_new                 = 187,
    JVM_OPC_newarray            = 188,
    JVM_OPC_anewarray           = 189,
    JVM_OPC_arraylength         = 190,
    JVM_OPC_athrow              = 191,
    JVM_OPC_checkcast           = 192,
    JVM_OPC_instanceof          = 193,
    JVM_OPC_monitorenter        = 194,
    JVM_OPC_monitorexit         = 195,
    JVM_OPC_wide                = 196,
    JVM_OPC_multianewarray      = 197,
    JVM_OPC_ifnull              = 198,
    JVM_OPC_ifnonnull           = 199,
    JVM_OPC_goto_w              = 200,
    JVM_OPC_jsr_w               = 201,
    JVM_OPC_MAX                 = 201
};

/* Opcode length initializer, use with something like:
 *   unsigned char opcode_length[JVM_OPC_MAX+1] = JVM_OPCODE_LENGTH_INITIALIZER;
 */
#define JVM_OPCODE_LENGTH_INITIALIZER { \
   1,   /* nop */                       \
   1,   /* aconst_null */               \
   1,   /* iconst_m1 */                 \
   1,   /* iconst_0 */                  \
   1,   /* iconst_1 */                  \
   1,   /* iconst_2 */                  \
   1,   /* iconst_3 */                  \
   1,   /* iconst_4 */                  \
   1,   /* iconst_5 */                  \
   1,   /* lconst_0 */                  \
   1,   /* lconst_1 */                  \
   1,   /* fconst_0 */                  \
   1,   /* fconst_1 */                  \
   1,   /* fconst_2 */                  \
   1,   /* dconst_0 */                  \
   1,   /* dconst_1 */                  \
   2,   /* bipush */                    \
   3,   /* sipush */                    \
   2,   /* ldc */                       \
   3,   /* ldc_w */                     \
   3,   /* ldc2_w */                    \
   2,   /* iload */                     \
   2,   /* lload */                     \
   2,   /* fload */                     \
   2,   /* dload */                     \
   2,   /* aload */                     \
   1,   /* iload_0 */                   \
   1,   /* iload_1 */                   \
   1,   /* iload_2 */                   \
   1,   /* iload_3 */                   \
   1,   /* lload_0 */                   \
   1,   /* lload_1 */                   \
   1,   /* lload_2 */                   \
   1,   /* lload_3 */                   \
   1,   /* fload_0 */                   \
   1,   /* fload_1 */                   \
   1,   /* fload_2 */                   \
   1,   /* fload_3 */                   \
   1,   /* dload_0 */                   \
   1,   /* dload_1 */                   \
   1,   /* dload_2 */                   \
   1,   /* dload_3 */                   \
   1,   /* aload_0 */                   \
   1,   /* aload_1 */                   \
   1,   /* aload_2 */                   \
   1,   /* aload_3 */                   \
   1,   /* iaload */                    \
   1,   /* laload */                    \
   1,   /* faload */                    \
   1,   /* daload */                    \
   1,   /* aaload */                    \
   1,   /* baload */                    \
   1,   /* caload */                    \
   1,   /* saload */                    \
   2,   /* istore */                    \
   2,   /* lstore */                    \
   2,   /* fstore */                    \
   2,   /* dstore */                    \
   2,   /* astore */                    \
   1,   /* istore_0 */                  \
   1,   /* istore_1 */                  \
   1,   /* istore_2 */                  \
   1,   /* istore_3 */                  \
   1,   /* lstore_0 */                  \
   1,   /* lstore_1 */                  \
   1,   /* lstore_2 */                  \
   1,   /* lstore_3 */                  \
   1,   /* fstore_0 */                  \
   1,   /* fstore_1 */                  \
   1,   /* fstore_2 */                  \
   1,   /* fstore_3 */                  \
   1,   /* dstore_0 */                  \
   1,   /* dstore_1 */                  \
   1,   /* dstore_2 */                  \
   1,   /* dstore_3 */                  \
   1,   /* astore_0 */                  \
   1,   /* astore_1 */                  \
   1,   /* astore_2 */                  \
   1,   /* astore_3 */                  \
   1,   /* iastore */                   \
   1,   /* lastore */                   \
   1,   /* fastore */                   \
   1,   /* dastore */                   \
   1,   /* aastore */                   \
   1,   /* bastore */                   \
   1,   /* castore */                   \
   1,   /* sastore */                   \
   1,   /* pop */                       \
   1,   /* pop2 */                      \
   1,   /* dup */                       \
   1,   /* dup_x1 */                    \
   1,   /* dup_x2 */                    \
   1,   /* dup2 */                      \
   1,   /* dup2_x1 */                   \
   1,   /* dup2_x2 */                   \
   1,   /* swap */                      \
   1,   /* iadd */                      \
   1,   /* ladd */                      \
   1,   /* fadd */                      \
   1,   /* dadd */                      \
   1,   /* isub */                      \
   1,   /* lsub */                      \
   1,   /* fsub */                      \
   1,   /* dsub */                      \
   1,   /* imul */                      \
   1,   /* lmul */                      \
   1,   /* fmul */                      \
   1,   /* dmul */                      \
   1,   /* idiv */                      \
   1,   /* ldiv */                      \
   1,   /* fdiv */                      \
   1,   /* ddiv */                      \
   1,   /* irem */                      \
   1,   /* lrem */                      \
   1,   /* frem */                      \
   1,   /* drem */                      \
   1,   /* ineg */                      \
   1,   /* lneg */                      \
   1,   /* fneg */                      \
   1,   /* dneg */                      \
   1,   /* ishl */                      \
   1,   /* lshl */                      \
   1,   /* ishr */                      \
   1,   /* lshr */                      \
   1,   /* iushr */                     \
   1,   /* lushr */                     \
   1,   /* iand */                      \
   1,   /* land */                      \
   1,   /* ior */                       \
   1,   /* lor */                       \
   1,   /* ixor */                      \
   1,   /* lxor */                      \
   3,   /* iinc */                      \
   1,   /* i2l */                       \
   1,   /* i2f */                       \
   1,   /* i2d */                       \
   1,   /* l2i */                       \
   1,   /* l2f */                       \
   1,   /* l2d */                       \
   1,   /* f2i */                       \
   1,   /* f2l */                       \
   1,   /* f2d */                       \
   1,   /* d2i */                       \
   1,   /* d2l */                       \
   1,   /* d2f */                       \
   1,   /* i2b */                       \
   1,   /* i2c */                       \
   1,   /* i2s */                       \
   1,   /* lcmp */                      \
   1,   /* fcmpl */                     \
   1,   /* fcmpg */                     \
   1,   /* dcmpl */                     \
   1,   /* dcmpg */                     \
   3,   /* ifeq */                      \
   3,   /* ifne */                      \
   3,   /* iflt */                      \
   3,   /* ifge */                      \
   3,   /* ifgt */                      \
   3,   /* ifle */                      \
   3,   /* if_icmpeq */                 \
   3,   /* if_icmpne */                 \
   3,   /* if_icmplt */                 \
   3,   /* if_icmpge */                 \
   3,   /* if_icmpgt */                 \
   3,   /* if_icmple */                 \
   3,   /* if_acmpeq */                 \
   3,   /* if_acmpne */                 \
   3,   /* goto */                      \
   3,   /* jsr */                       \
   2,   /* ret */                       \
   99,  /* tableswitch */               \
   99,  /* lookupswitch */              \
   1,   /* ireturn */                   \
   1,   /* lreturn */                   \
   1,   /* freturn */                   \
   1,   /* dreturn */                   \
   1,   /* areturn */                   \
   1,   /* return */                    \
   3,   /* getstatic */                 \
   3,   /* putstatic */                 \
   3,   /* getfield */                  \
   3,   /* putfield */                  \
   3,   /* invokevirtual */             \
   3,   /* invokespecial */             \
   3,   /* invokestatic */              \
   5,   /* invokeinterface */           \
   0,   /* xxxunusedxxx */              \
   3,   /* new */                       \
   2,   /* newarray */                  \
   3,   /* anewarray */                 \
   1,   /* arraylength */               \
   1,   /* athrow */                    \
   3,   /* checkcast */                 \
   3,   /* instanceof */                \
   1,   /* monitorenter */              \
   1,   /* monitorexit */               \
   0,   /* wide */                      \
   4,   /* multianewarray */            \
   3,   /* ifnull */                    \
   3,   /* ifnonnull */                 \
   5,   /* goto_w */                    \
   5    /* jsr_w */                     \
}
  struct Ref
  {
    u2 class_index;
    u2 name_and_type_index;
    Ref() {}
    Ref(u2 class_index, u2 name_and_type_index):
      class_index(class_index), name_and_type_index(name_and_type_index) {}
    bool operator==(const Ref& x)  const {
      return class_index==x.class_index && 
        name_and_type_index==x.name_and_type_index;}
  };

  struct NameAndTypeInfo
  {
    u2 name_index;
    u2 descriptor_index;
    NameAndTypeInfo() {}
    NameAndTypeInfo(u2 name_index, u2 descriptor_index): 
      name_index(name_index), descriptor_index(descriptor_index) {}
    bool operator==(const NameAndTypeInfo& x)  const {
      return name_index==x.name_index && 
        descriptor_index==x.descriptor_index;}    
  };

  class cp_info
  {
    u1 _tag;
    shared_ptr<void> info;
    CLASSDESC_ACCESS(cp_info);
  public:
    cp_info(): _tag(0) {}
    template <class T>
    cp_info(u1 t, const T& v) {set(t,v);}

    template <class T>
    void set(u1 t, const T& v) {
      _tag=t;
      info.reset(new T(v));
    }
    template <class T> void unpack(pack_t& t, u1 tag);

    //Basic C types need to be unpacked in Java byte order
    template <class T>
    void unpack_basic(pack_t& t, u1 tag)
    {
      switch (sizeof(T))
        {
        case 1: unpack<u1>(t,tag); break;
        case 2: unpack<u2>(t,tag); break;
        case 4: unpack<u4>(t,tag); break;
        case 8: unpack<u8>(t,tag); break;
        }
    }

    template <class T>
    static u1 Tag();

    template <class T>
    const T& get() const {return *static_cast<const T*>(info.get());}

    u1 tag() const {return _tag;}
    bool operator==(const cp_info& x) const; //definition in javaClass_serialisation.h
  };

  // specialised serialisers provided for these 2 cases
  struct ConstantPoolVector: public std::vector<cp_info>
  {
    ConstantPoolVector(size_t sz=0): std::vector<cp_info>(sz) {}
  };
  
  struct InfoVector: public std::vector<u1> {};

  struct attribute_info 
  {
    u2 attribute_name_index;
    InfoVector info; //[attribute_length];
    bool operator==(const attribute_info& x)  const {
      return attribute_name_index==x.attribute_name_index && info == x.info;
    }
  };

  struct method_info 
  {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    JavaClassVector<attribute_info> attributes; //[attributes_count];
    bool operator==(const method_info& x) const {
      return access_flags==x.access_flags && name_index==x.name_index &&
        descriptor_index == x.descriptor_index && attributes==x.attributes;
    }
};

  struct field_info 
  {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    JavaClassVector<attribute_info> attributes; //[attributes_count];
    bool operator==(const field_info& x) const {
      return access_flags==x.access_flags && name_index==x.name_index &&
        descriptor_index==x.descriptor_index && attributes==x.attributes;
    }
  };

  struct ClassFile 
  {
    u4 magic;
    u2 minor_version;
    u2 major_version;
    ConstantPoolVector constant_pool; //[constant_pool_count-1];
    u2 access_flags;
    u2 this_class;
    u2 super_class;
    JavaClassVector<u2> interfaces; //[interfaces_count];
    JavaClassVector<field_info> fields; //[fields_count];
    JavaClassVector<method_info> methods; //[methods_count];
    JavaClassVector<attribute_info> attributes; //[attributes_count];
    ClassFile(): constant_pool(1) {} //add a zeroth (ignorable) element
    bool operator==(const ClassFile& x) const {
      return magic==x.magic && minor_version==x.minor_version &&
        major_version==x.major_version && constant_pool==x.constant_pool &&
        access_flags==x.access_flags && this_class == x.this_class &&
        super_class == x.super_class && interfaces == x.interfaces &&
        fields == x.fields && methods == x.methods && attributes==x.attributes;
    }

    /// add a method 
    void addMethod(const std::string& method_name, const std::string& descriptor)
    {
      method_info mi;
      mi.name_index=constant_pool.size();
      constant_pool.push_back(cp_info(JVM_CONSTANT_Utf8, method_name));
      mi.descriptor_index=constant_pool.size();
      constant_pool.push_back(cp_info(JVM_CONSTANT_Utf8, descriptor));
      //constant_pool.push_back(cp_info(JVM_CONSTANT_Utf8, std::string("([Ljava/lang/Object;)Ljava/lang/Object;")));
      methods.push_back(mi);
    }
    /// make this a concrete class with native methods
    void makeNative()
    {
      for (std::vector<method_info>::iterator i=methods.begin(); i!=methods.end(); ++i)
        i->access_flags = JVM_ACC_NATIVE|JVM_ACC_PUBLIC;
      access_flags = JVM_ACC_PUBLIC;
    }
    /// make this an interface with abstract methods
    void makeInterface()
    {
      for (std::vector<method_info>::iterator i=methods.begin(); i!=methods.end(); ++i)
        i->access_flags = JVM_ACC_ABSTRACT|JVM_ACC_PUBLIC;
      access_flags = JVM_ACC_INTERFACE|JVM_ACC_ABSTRACT|JVM_ACC_PUBLIC;
    }
  };


}

#endif
