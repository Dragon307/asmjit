// [AsmJit]
// Complete x86/x64 JIT and Remote Assembler for C++.
//
// [License]
// Zlib - See LICENSE.md file in the package.

// [Guard]
#ifndef _ASMJIT_CORE_CALLCONV_H
#define _ASMJIT_CORE_CALLCONV_H

// [Dependencies]
#include "../core/arch.h"
#include "../core/operand.h"
#include "../core/intutils.h"

ASMJIT_BEGIN_NAMESPACE

//! \addtogroup asmjit_core_func
//! \{

// ============================================================================
// [asmjit::CallConv]
// ============================================================================

//! Function calling convention.
//!
//! Function calling convention is a scheme that defines how function parameters
//! are passed and how function returns its result. AsmJit defines a variety of
//! architecture and OS specific calling conventions and also provides a compile
//! time detection to make the code-generation easier.
struct CallConv {
  //! Calling convention id.
  enum Id : uint32_t {
    //! None or invalid (can't be used).
    kIdNone = 0,

    // ------------------------------------------------------------------------
    // [Universal]
    // ------------------------------------------------------------------------

    // TODO: To make this possible we need to know target ARCH and ABI.

    /*

    // Universal calling conventions are applicable to any target and are
    // converted to target dependent conventions at runtime. The purpose of
    // these conventions is to make using functions less target dependent.

    kIdCDecl = 1,
    kIdStdCall = 2,
    kIdFastCall = 3,

    //! AsmJit specific calling convention designed for calling functions
    //! inside a multimedia code that don't use many registers internally,
    //! but are long enough to be called and not inlined. These functions are
    //! usually used to calculate trigonometric functions, logarithms, etc...
    kIdLightCall2 = 10,
    kIdLightCall3 = 11,
    kIdLightCall4 = 12,
    */

    // ------------------------------------------------------------------------
    // [X86]
    // ------------------------------------------------------------------------

    //! X86 `__cdecl` calling convention (used by C runtime and libraries).
    kIdX86CDecl = 16,
    //! X86 `__stdcall` calling convention (used mostly by WinAPI).
    kIdX86StdCall = 17,
    //! X86 `__thiscall` calling convention (MSVC/Intel).
    kIdX86MsThisCall = 18,
    //! X86 `__fastcall` convention (MSVC/Intel).
    kIdX86MsFastCall = 19,
    //! X86 `__fastcall` convention (GCC and Clang).
    kIdX86GccFastCall = 20,
    //! X86 `regparm(1)` convention (GCC and Clang).
    kIdX86GccRegParm1 = 21,
    //! X86 `regparm(2)` convention (GCC and Clang).
    kIdX86GccRegParm2 = 22,
    //! X86 `regparm(3)` convention (GCC and Clang).
    kIdX86GccRegParm3 = 23,

    kIdX86LightCall2 = 29,
    kIdX86LightCall3 = 30,
    kIdX86LightCall4 = 31,

    //! X64 calling convention - WIN64-ABI.
    kIdX86Win64 = 32,
    //! X64 calling convention - SystemV / AMD64-ABI.
    kIdX86SysV64 = 33,

    kIdX64LightCall2 = 45,
    kIdX64LightCall3 = 46,
    kIdX64LightCall4 = 47,

    // ------------------------------------------------------------------------
    // [ARM]
    // ------------------------------------------------------------------------

    //! Legacy calling convention, floating point arguments are passed via GP registers.
    kIdArm32SoftFP = 48,
    //! Modern calling convention, uses VFP registers to pass floating point arguments.
    kIdArm32HardFP = 49,

    // ------------------------------------------------------------------------
    // [Internal]
    // ------------------------------------------------------------------------

    _kIdX86Start = 16,  //!< \internal
    _kIdX86End = 31,    //!< \internal

    _kIdX64Start = 32,  //!< \internal
    _kIdX64End = 47,    //!< \internal

    _kIdArmStart = 48,  //!< \internal
    _kIdArmEnd = 49,    //!< \internal

    // ------------------------------------------------------------------------
    // [Host]
    // ------------------------------------------------------------------------

    #if defined(ASMJIT_DOCGEN)

    //! Default calling convention based on the current C++ compiler's settings.
    //!
    //! NOTE: This should be always the same as `kIdHostCDecl`, but some
    //! compilers allow to override the default calling convention. Overriding
    //! is not detected at the moment.
    kIdHost          = DETECTED_AT_COMPILE_TIME,
    //! Default CDECL calling convention based on the current C++ compiler's settings.
    kIdHostCDecl     = DETECTED_AT_COMPILE_TIME,
    //! Default STDCALL calling convention based on the current C++ compiler's settings.
    //!
    //! NOTE: If not defined by the host then it's the same as `kIdHostCDecl`.
    kIdHostStdCall   = DETECTED_AT_COMPILE_TIME,
    //! Compatibility for `__fastcall` calling convention.
    //!
    //! NOTE: If not defined by the host then it's the same as `kIdHostCDecl`.
    kIdHostFastCall  = DETECTED_AT_COMPILE_TIME

    #elif ASMJIT_ARCH_X86 == 32

    kIdHost          = kIdX86CDecl,
    kIdHostCDecl     = kIdX86CDecl,
    kIdHostStdCall   = kIdX86StdCall,
    kIdHostFastCall  = ASMJIT_CXX_MSC ? kIdX86MsFastCall  :
                       ASMJIT_CXX_GNU ? kIdX86GccFastCall : kIdNone,
    kIdHostLightCall2 = kIdX86LightCall2,
    kIdHostLightCall3 = kIdX86LightCall3,
    kIdHostLightCall4 = kIdX86LightCall4

    #elif ASMJIT_ARCH_X86 == 64

    kIdHost          = ASMJIT_OS_WINDOWS ? kIdX86Win64 : kIdX86SysV64,
    kIdHostCDecl     = kIdHost, // Doesn't exist, redirected to host.
    kIdHostStdCall   = kIdHost, // Doesn't exist, redirected to host.
    kIdHostFastCall  = kIdHost, // Doesn't exist, redirected to host.
    kIdHostLightCall2 = kIdX64LightCall2,
    kIdHostLightCall3 = kIdX64LightCall3,
    kIdHostLightCall4 = kIdX64LightCall4

    #elif ASMJIT_ARCH_ARM == 32

    #if defined(__SOFTFP__)
    kIdHost          = kIdArm32SoftFP,
    #else
    kIdHost          = kIdArm32HardFP,
    #endif
    // These don't exist on ARM.
    kIdHostCDecl     = kIdHost, // Doesn't exist, redirected to host.
    kIdHostStdCall   = kIdHost, // Doesn't exist, redirected to host.
    kIdHostFastCall  = kIdHost  // Doesn't exist, redirected to host.

    #else

    kIdHost          = kIdNone,
    kIdHostCDecl     = kIdHost,
    kIdHostStdCall   = kIdHost,
    kIdHostFastCall  = kIdHost

    #endif
  };

  //! Strategy used to assign registers to function arguments.
  //!
  //! This is AsmJit specific. It basically describes how should AsmJit convert
  //! the function arguments defined by `FuncSignature` into register IDs or
  //! stack offsets. The default strategy assigns registers and then stack.
  //! The Win64 strategy does register shadowing as defined by `WIN64` calling
  //! convention - it applies to 64-bit calling conventions only.
  enum Strategy : uint32_t {
    kStrategyDefault     = 0,            //!< Default register assignment strategy.
    kStrategyWin64       = 1             //!< WIN64 specific register assignment strategy.
  };

  //! Calling convention flags.
  enum Flags : uint32_t {
    kFlagCalleePopsStack = 0x01,         //!< Callee is responsible for cleaning up the stack.
    kFlagPassFloatsByVec = 0x02,         //!< Pass F32 and F64 arguments by VEC128 register.
    kFlagVectorCall      = 0x04,         //!< This is a '__vectorcall' calling convention.
    kFlagIndirectVecArgs = 0x08          //!< Pass vector arguments indirectly (as a pointer).
  };

  //! Internal limits of AsmJit's CallConv.
  enum Limits : uint32_t {
    kMaxRegArgsPerGroup  = 16
  };

  //! Passed registers' order.
  union RegOrder {
    uint8_t id[kMaxRegArgsPerGroup];     //!< Passed registers, ordered.
    uint32_t packed[(kMaxRegArgsPerGroup + 3) / 4];
  };

  // --------------------------------------------------------------------------
  // [Utilities]
  // --------------------------------------------------------------------------

  static inline bool isX86Family(uint32_t ccId) noexcept { return ccId >= _kIdX86Start && ccId <= _kIdX64End; }
  static inline bool isArmFamily(uint32_t ccId) noexcept { return ccId >= _kIdArmStart && ccId <= _kIdArmEnd; }

  // --------------------------------------------------------------------------
  // [Init / Reset]
  // --------------------------------------------------------------------------

  ASMJIT_API Error init(uint32_t ccId) noexcept;

  inline void reset() noexcept {
    std::memset(this, 0, sizeof(*this));
    std::memset(_passedOrder, 0xFF, sizeof(_passedOrder));
  }

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  //! Get calling convention id, see `Id`.
  inline uint32_t id() const noexcept { return _id; }
  //! Set calling convention id, see `Id`.
  inline void setId(uint32_t id) noexcept { _id = uint8_t(id); }

  //! Get architecture type.
  inline uint32_t archId() const noexcept { return _archId; }
  //! Set architecture type.
  inline void setArchType(uint32_t archId) noexcept { _archId = uint8_t(archId); }

  //! Get a strategy used to assign registers to arguments, see `Strategy`.
  inline uint32_t strategy() const noexcept { return _strategy; }
  //! Set a strategy used to assign registers to arguments, see `Strategy`.
  inline void setStrategy(uint32_t strategy) noexcept { _strategy = uint8_t(strategy); }

  //! Get whether the calling convention has the given `flag` set.
  inline bool hasFlag(uint32_t flag) const noexcept { return (uint32_t(_flags) & flag) != 0; }
  //! Get calling convention flags, see `Flags`.
  inline uint32_t flags() const noexcept { return _flags; }
  //! Add calling convention flags, see `Flags`.
  inline void setFlags(uint32_t flag) noexcept { _flags = uint8_t(flag); };
  //! Add calling convention flags, see `Flags`.
  inline void addFlags(uint32_t flags) noexcept { _flags = uint8_t(_flags | flags); };

  //! Get whether this calling convention specifies 'RedZone'.
  inline bool hasRedZone() const noexcept { return _redZoneSize != 0; }
  //! Get whether this calling convention specifies 'SpillZone'.
  inline bool hasSpillZone() const noexcept { return _spillZoneSize != 0; }

  //! Get size of 'RedZone'.
  inline uint32_t redZoneSize() const noexcept { return _redZoneSize; }
  //! Get size of 'SpillZone'.
  inline uint32_t spillZoneSize() const noexcept { return _spillZoneSize; }

  //! Set size of 'RedZone'.
  inline void setRedZoneSize(uint32_t size) noexcept { _redZoneSize = uint8_t(size); }
  //! Set size of 'SpillZone'.
  inline void setSpillZoneSize(uint32_t size) noexcept { _spillZoneSize = uint8_t(size); }

  //! Get a natural stack alignment.
  inline uint32_t naturalStackAlignment() const noexcept { return _naturalStackAlignment; }
  //! Set a natural stack alignment.
  //!
  //! This function can be used to override the default stack alignment in case
  //! that you know that it's alignment is different. For example it allows to
  //! implement custom calling conventions that guarantee higher stack alignment.
  inline void setNaturalStackAlignment(uint32_t value) noexcept { _naturalStackAlignment = uint8_t(value); }

  inline const uint8_t* passedOrder(uint32_t group) const noexcept {
    ASMJIT_ASSERT(group < BaseReg::kGroupVirt);
    return _passedOrder[group].id;
  }

  inline uint32_t passedRegs(uint32_t group) const noexcept {
    ASMJIT_ASSERT(group < BaseReg::kGroupVirt);
    return _passedRegs[group];
  }

  inline void _setPassedPacked(uint32_t group, uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3) noexcept {
    ASMJIT_ASSERT(group < BaseReg::kGroupVirt);

    _passedOrder[group].packed[0] = p0;
    _passedOrder[group].packed[1] = p1;
    _passedOrder[group].packed[2] = p2;
    _passedOrder[group].packed[3] = p3;
  }

  inline void setPassedToNone(uint32_t group) noexcept {
    ASMJIT_ASSERT(group < BaseReg::kGroupVirt);

    _setPassedPacked(group, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU);
    _passedRegs[group] = 0U;
  }

  inline void setPassedOrder(uint32_t group, uint32_t a0, uint32_t a1 = 0xFF, uint32_t a2 = 0xFF, uint32_t a3 = 0xFF, uint32_t a4 = 0xFF, uint32_t a5 = 0xFF, uint32_t a6 = 0xFF, uint32_t a7 = 0xFF) noexcept {
    ASMJIT_ASSERT(group < BaseReg::kGroupVirt);

    // NOTE: This should always be called with all arguments known at compile time,
    // so even if it looks scary it should be translated into few instructions.
    _setPassedPacked(group, IntUtils::bytepack32_4x8(a0, a1, a2, a3),
                            IntUtils::bytepack32_4x8(a4, a5, a6, a7),
                            0xFFFFFFFFU,
                            0xFFFFFFFFU);

    _passedRegs[group] = (a0 != 0xFF ? 1U << a0 : 0U) |
                         (a1 != 0xFF ? 1U << a1 : 0U) |
                         (a2 != 0xFF ? 1U << a2 : 0U) |
                         (a3 != 0xFF ? 1U << a3 : 0U) |
                         (a4 != 0xFF ? 1U << a4 : 0U) |
                         (a5 != 0xFF ? 1U << a5 : 0U) |
                         (a6 != 0xFF ? 1U << a6 : 0U) |
                         (a7 != 0xFF ? 1U << a7 : 0U) ;
  }

  inline uint32_t preservedRegs(uint32_t group) const noexcept {
    ASMJIT_ASSERT(group < BaseReg::kGroupVirt);
    return _preservedRegs[group];
  }

  inline void setPreservedRegs(uint32_t group, uint32_t regs) noexcept {
    ASMJIT_ASSERT(group < BaseReg::kGroupVirt);
    _preservedRegs[group] = regs;
  }

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  uint8_t _id;                           //!< Calling convention id, see `Id`.
  uint8_t _archId;                       //!< Architecture id (see `ArchInfo::Id`).
  uint8_t _strategy;                     //!< Register assignment strategy.
  uint8_t _flags;                        //!< Flags.

  uint8_t _redZoneSize;                  //!< Red zone size (AMD64 == 128 bytes).
  uint8_t _spillZoneSize;                //!< Spill zone size (WIN64 == 32 bytes).
  uint8_t _naturalStackAlignment;        //!< Natural stack alignment as defined by OS/ABI.
  uint8_t _reserved[1];

  uint32_t _passedRegs[BaseReg::kGroupVirt];    //!< Mask of all passed registers, per group.
  uint32_t _preservedRegs[BaseReg::kGroupVirt]; //!< Mask of all preserved registers, per group.
  RegOrder _passedOrder[BaseReg::kGroupVirt];   //!< Passed registers' order, per group.
};

//! \}

ASMJIT_END_NAMESPACE

// [Guard]
#endif // _ASMJIT_CORE_CALLCONV_H
