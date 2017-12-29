// [AsmJit]
// Complete x86/x64 JIT and Remote Assembler for C++.
//
// [License]
// Zlib - See LICENSE.md file in the package.

// [Export]
#define ASMJIT_EXPORTS

// [Dependencies]
#include "../core/operand.h"

ASMJIT_BEGIN_NAMESPACE

// ============================================================================
// [asmjit::Operand - Unit]
// ============================================================================

#if defined(ASMJIT_BUILD_TEST)
UNIT(core_operand) {
  INFO("Checking operand sizes");
  EXPECT(sizeof(Operand) == 16);
  EXPECT(sizeof(BaseReg) == 16);
  EXPECT(sizeof(BaseMem) == 16);
  EXPECT(sizeof(Imm)     == 16);
  EXPECT(sizeof(Label)   == 16);

  INFO("Checking basic functionality of Operand");
  Operand a, b;
  Operand dummy;

  EXPECT(a.isNone() == true);
  EXPECT(a.isReg() == false);
  EXPECT(a.isMem() == false);
  EXPECT(a.isImm() == false);
  EXPECT(a.isLabel() == false);
  EXPECT(a == b);

  EXPECT(a._p32[2] == 0);
  EXPECT(a._p32[3] == 0);

  INFO("Checking basic functionality of Label");
  Label label;
  EXPECT(label.isValid() == false);
  EXPECT(label.id() == 0);

  INFO("Checking basic functionality of BaseReg");
  EXPECT(BaseReg().isReg() == true);
  EXPECT(BaseReg().isValid() == false);

  EXPECT(BaseReg()._p32[2] == 0);
  EXPECT(BaseReg()._p32[3] == 0);
  EXPECT(dummy.as<BaseReg>().isValid() == false);

  // Create some register (not specific to any architecture).
  uint32_t rSig = Operand::kOpReg | (1 << Operand::kSignatureRegTypeShift ) |
                                    (2 << Operand::kSignatureRegGroupShift) |
                                    (8 << Operand::kSignatureSizeShift    ) ;
  BaseReg r1(rSig, 5);

  EXPECT(r1.isValid()      == true);
  EXPECT(r1.isReg()        == true);
  EXPECT(r1.isReg(1)       == true);
  EXPECT(r1.isPhysReg()    == true);
  EXPECT(r1.isVirtReg()    == false);
  EXPECT(r1.signature() == rSig);
  EXPECT(r1.type()      == 1);
  EXPECT(r1.group()     == 2);
  EXPECT(r1.size()         == 8);
  EXPECT(r1.id()           == 5);
  EXPECT(r1.isReg(1, 5)    == true); // RegType and Id.
  EXPECT(r1._p32[2] == 0);
  EXPECT(r1._p32[3] == 0);

  // The same type of register having different id.
  BaseReg r2(r1, 6);
  EXPECT(r2.isValid()      == true);
  EXPECT(r2.isReg()        == true);
  EXPECT(r2.isReg(1)       == true);
  EXPECT(r2.isPhysReg()    == true);
  EXPECT(r2.isVirtReg()    == false);
  EXPECT(r2.signature() == rSig);
  EXPECT(r2.type()      == r1.type());
  EXPECT(r2.group()     == r1.group());
  EXPECT(r2.size()         == r1.size());
  EXPECT(r2.id()           == 6);
  EXPECT(r2.isReg(1, 6)    == true);

  r1.reset();
  EXPECT(!r1.isReg());
  EXPECT(!r1.isValid());

  INFO("Checking basic functionality of BaseMem");
  BaseMem m;
  EXPECT(m.isMem());
  EXPECT(m == BaseMem());
  EXPECT(m.hasBase() == false);
  EXPECT(m.hasIndex() == false);
  EXPECT(m.hasOffset() == false);
  EXPECT(m.isOffset64Bit() == true);
  EXPECT(m.offset() == 0);

  m.setOffset(-1);
  EXPECT(m.offsetLo32() == -1);
  EXPECT(m.offset() == -1);

  int64_t x = int64_t(0xFF00FF0000000001U);
  int32_t xHi = int32_t(0xFF00FF00U);

  m.setOffset(x);
  EXPECT(m.offset() == x);
  EXPECT(m.offsetLo32() == 1);
  EXPECT(m.offsetHi32() == xHi);

  INFO("Checking basic functionality of Imm");
  EXPECT(Imm(-1).i64() == int64_t(-1));
}
#endif

ASMJIT_END_NAMESPACE
