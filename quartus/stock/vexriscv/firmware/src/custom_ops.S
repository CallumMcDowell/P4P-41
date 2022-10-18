// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

//-------------------------------------------------------------------------------------------
// Preprocessing Directives
//-------------------------------------------------------------------------------------------

/*
* Each register is 32 bit (of some representation, depending on the instruction).
*
* For an absolute representation of the bits, the symbolic register names are concataneted 
* with the definitons below to resolve/dereference to the correct bit sequence in signals.
*/ 

#define regnum_q0   0
#define regnum_q1   1
#define regnum_q2   2
#define regnum_q3   3

#define regnum_x0   0
#define regnum_x1   1
#define regnum_x2   2
#define regnum_x3   3
#define regnum_x4   4
#define regnum_x5   5
#define regnum_x6   6
#define regnum_x7   7
#define regnum_x8   8
#define regnum_x9   9
#define regnum_x10 10
#define regnum_x11 11
#define regnum_x12 12
#define regnum_x13 13
#define regnum_x14 14
#define regnum_x15 15
#define regnum_x16 16
#define regnum_x17 17
#define regnum_x18 18
#define regnum_x19 19
#define regnum_x20 20
#define regnum_x21 21
#define regnum_x22 22
#define regnum_x23 23
#define regnum_x24 24
#define regnum_x25 25
#define regnum_x26 26
#define regnum_x27 27
#define regnum_x28 28
#define regnum_x29 29
#define regnum_x30 30
#define regnum_x31 31

#define regnum_zero 0
#define regnum_ra   1
#define regnum_sp   2
#define regnum_gp   3
#define regnum_tp   4
#define regnum_t0   5
#define regnum_t1   6
#define regnum_t2   7
#define regnum_s0   8
#define regnum_s1   9
#define regnum_a0  10
#define regnum_a1  11
#define regnum_a2  12
#define regnum_a3  13
#define regnum_a4  14
#define regnum_a5  15
#define regnum_a6  16
#define regnum_a7  17
#define regnum_s2  18
#define regnum_s3  19
#define regnum_s4  20
#define regnum_s5  21
#define regnum_s6  22
#define regnum_s7  23
#define regnum_s8  24
#define regnum_s9  25
#define regnum_s10 26
#define regnum_s11 27
#define regnum_t3  28
#define regnum_t4  29
#define regnum_t5  30
#define regnum_t6  31

// x8 is s0 and also fp
#define regnum_fp   8

// Shorthands for constructing the bit sequence
//-------------------------------------------------------------------------------------------
#define r_type_insn(_f7, _rs2, _rs1, _f3, _rd, _opc) \
.word (((_f7) << 25) | ((_rs2) << 20) | ((_rs1) << 15) | ((_f3) << 12) | ((_rd) << 7) | ((_opc) << 0))

// Stock SIMD_ADD
// Instruction encoding :
// 0000011----------000-----0110011
//        |RS2||RS1|   |RD |

#define SIMD_ADD(_rs2, _rs1, _rd) \
r_type_insn(0b0000011, regnum_ ## _rs2, regnum_ ## _rs1, 0b000, regnum_ ## _rd, 0b0110011)

//-------------------------------------------------------------------------------------------
// Macro Definitions
//-------------------------------------------------------------------------------------------
.text


.global _simd_add // extern void _simd_add(uint32_t r2, uint32_t r1, uint32_t rd);

_simd_add:
    SIMD_ADD(a0, a1, a2)
    mv a0, a2
    ret