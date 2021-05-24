#include <math.h>
#include <string.h>

#include "enc.h"
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define LIKELY(x) __builtin_expect(!!(x), 1)

#define TRAP(x) (wasm_rt_trap(WASM_RT_TRAP_##x), 0)

#define FUNC_PROLOGUE                                            \
  if (++wasm_rt_call_stack_depth > WASM_RT_MAX_CALL_STACK_DEPTH) \
    TRAP(EXHAUSTION)

#define FUNC_EPILOGUE --wasm_rt_call_stack_depth

#define UNREACHABLE TRAP(UNREACHABLE)

#define CALL_INDIRECT(table, t, ft, x, ...)          \
  (LIKELY((x) < table.size && table.data[x].func &&  \
          table.data[x].func_type == func_types[ft]) \
       ? ((t)table.data[x].func)(__VA_ARGS__)        \
       : TRAP(CALL_INDIRECT))

#define MEMCHECK(mem, a, t)  \
  if (UNLIKELY((a) + sizeof(t) > mem->size)) TRAP(OOB)

#define DEFINE_LOAD(name, t1, t2, t3)              \
  static inline t3 name(wasm_rt_memory_t* mem, u64 addr) {   \
    MEMCHECK(mem, addr, t1);                       \
    t1 result;                                     \
    memcpy(&result, &mem->data[addr], sizeof(t1)); \
    return (t3)(t2)result;                         \
  }

#define DEFINE_STORE(name, t1, t2)                           \
  static inline void name(wasm_rt_memory_t* mem, u64 addr, t2 value) { \
    MEMCHECK(mem, addr, t1);                                 \
    t1 wrapped = (t1)value;                                  \
    memcpy(&mem->data[addr], &wrapped, sizeof(t1));          \
  }

DEFINE_LOAD(i32_load, u32, u32, u32);
DEFINE_LOAD(i64_load, u64, u64, u64);
DEFINE_LOAD(f32_load, f32, f32, f32);
DEFINE_LOAD(f64_load, f64, f64, f64);
DEFINE_LOAD(i32_load8_s, s8, s32, u32);
DEFINE_LOAD(i64_load8_s, s8, s64, u64);
DEFINE_LOAD(i32_load8_u, u8, u32, u32);
DEFINE_LOAD(i64_load8_u, u8, u64, u64);
DEFINE_LOAD(i32_load16_s, s16, s32, u32);
DEFINE_LOAD(i64_load16_s, s16, s64, u64);
DEFINE_LOAD(i32_load16_u, u16, u32, u32);
DEFINE_LOAD(i64_load16_u, u16, u64, u64);
DEFINE_LOAD(i64_load32_s, s32, s64, u64);
DEFINE_LOAD(i64_load32_u, u32, u64, u64);
DEFINE_STORE(i32_store, u32, u32);
DEFINE_STORE(i64_store, u64, u64);
DEFINE_STORE(f32_store, f32, f32);
DEFINE_STORE(f64_store, f64, f64);
DEFINE_STORE(i32_store8, u8, u32);
DEFINE_STORE(i32_store16, u16, u32);
DEFINE_STORE(i64_store8, u8, u64);
DEFINE_STORE(i64_store16, u16, u64);
DEFINE_STORE(i64_store32, u32, u64);

#define I32_CLZ(x) ((x) ? __builtin_clz(x) : 32)
#define I64_CLZ(x) ((x) ? __builtin_clzll(x) : 64)
#define I32_CTZ(x) ((x) ? __builtin_ctz(x) : 32)
#define I64_CTZ(x) ((x) ? __builtin_ctzll(x) : 64)
#define I32_POPCNT(x) (__builtin_popcount(x))
#define I64_POPCNT(x) (__builtin_popcountll(x))

#define DIV_S(ut, min, x, y)                                 \
   ((UNLIKELY((y) == 0)) ?                TRAP(DIV_BY_ZERO)  \
  : (UNLIKELY((x) == min && (y) == -1)) ? TRAP(INT_OVERFLOW) \
  : (ut)((x) / (y)))

#define REM_S(ut, min, x, y)                                \
   ((UNLIKELY((y) == 0)) ?                TRAP(DIV_BY_ZERO) \
  : (UNLIKELY((x) == min && (y) == -1)) ? 0                 \
  : (ut)((x) % (y)))

#define I32_DIV_S(x, y) DIV_S(u32, INT32_MIN, (s32)x, (s32)y)
#define I64_DIV_S(x, y) DIV_S(u64, INT64_MIN, (s64)x, (s64)y)
#define I32_REM_S(x, y) REM_S(u32, INT32_MIN, (s32)x, (s32)y)
#define I64_REM_S(x, y) REM_S(u64, INT64_MIN, (s64)x, (s64)y)

#define DIVREM_U(op, x, y) \
  ((UNLIKELY((y) == 0)) ? TRAP(DIV_BY_ZERO) : ((x) op (y)))

#define DIV_U(x, y) DIVREM_U(/, x, y)
#define REM_U(x, y) DIVREM_U(%, x, y)

#define ROTL(x, y, mask) \
  (((x) << ((y) & (mask))) | ((x) >> (((mask) - (y) + 1) & (mask))))
#define ROTR(x, y, mask) \
  (((x) >> ((y) & (mask))) | ((x) << (((mask) - (y) + 1) & (mask))))

#define I32_ROTL(x, y) ROTL(x, y, 31)
#define I64_ROTL(x, y) ROTL(x, y, 63)
#define I32_ROTR(x, y) ROTR(x, y, 31)
#define I64_ROTR(x, y) ROTR(x, y, 63)

#define FMIN(x, y)                                          \
   ((UNLIKELY((x) != (x))) ? NAN                            \
  : (UNLIKELY((y) != (y))) ? NAN                            \
  : (UNLIKELY((x) == 0 && (y) == 0)) ? (signbit(x) ? x : y) \
  : (x < y) ? x : y)

#define FMAX(x, y)                                          \
   ((UNLIKELY((x) != (x))) ? NAN                            \
  : (UNLIKELY((y) != (y))) ? NAN                            \
  : (UNLIKELY((x) == 0 && (y) == 0)) ? (signbit(x) ? y : x) \
  : (x > y) ? x : y)

#define TRUNC_S(ut, st, ft, min, max, maxop, x)                             \
   ((UNLIKELY((x) != (x))) ? TRAP(INVALID_CONVERSION)                       \
  : (UNLIKELY((x) < (ft)(min) || (x) maxop (ft)(max))) ? TRAP(INT_OVERFLOW) \
  : (ut)(st)(x))

#define I32_TRUNC_S_F32(x) TRUNC_S(u32, s32, f32, INT32_MIN, INT32_MAX, >=, x)
#define I64_TRUNC_S_F32(x) TRUNC_S(u64, s64, f32, INT64_MIN, INT64_MAX, >=, x)
#define I32_TRUNC_S_F64(x) TRUNC_S(u32, s32, f64, INT32_MIN, INT32_MAX, >,  x)
#define I64_TRUNC_S_F64(x) TRUNC_S(u64, s64, f64, INT64_MIN, INT64_MAX, >=, x)

#define TRUNC_U(ut, ft, max, maxop, x)                                    \
   ((UNLIKELY((x) != (x))) ? TRAP(INVALID_CONVERSION)                     \
  : (UNLIKELY((x) <= (ft)-1 || (x) maxop (ft)(max))) ? TRAP(INT_OVERFLOW) \
  : (ut)(x))

#define I32_TRUNC_U_F32(x) TRUNC_U(u32, f32, UINT32_MAX, >=, x)
#define I64_TRUNC_U_F32(x) TRUNC_U(u64, f32, UINT64_MAX, >=, x)
#define I32_TRUNC_U_F64(x) TRUNC_U(u32, f64, UINT32_MAX, >,  x)
#define I64_TRUNC_U_F64(x) TRUNC_U(u64, f64, UINT64_MAX, >=, x)

#define DEFINE_REINTERPRET(name, t1, t2)  \
  static inline t2 name(t1 x) {           \
    t2 result;                            \
    memcpy(&result, &x, sizeof(result));  \
    return result;                        \
  }

DEFINE_REINTERPRET(f32_reinterpret_i32, u32, f32)
DEFINE_REINTERPRET(i32_reinterpret_f32, f32, u32)
DEFINE_REINTERPRET(f64_reinterpret_i64, u64, f64)
DEFINE_REINTERPRET(i64_reinterpret_f64, f64, u64)


static u32 func_types[7];

static void init_func_types(void) {
  func_types[0] = wasm_rt_register_func_type(0, 1, WASM_RT_I32);
  func_types[1] = wasm_rt_register_func_type(0, 0);
  func_types[2] = wasm_rt_register_func_type(1, 0, WASM_RT_I32);
  func_types[3] = wasm_rt_register_func_type(3, 1, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32);
  func_types[4] = wasm_rt_register_func_type(2, 0, WASM_RT_I32, WASM_RT_I32);
  func_types[5] = wasm_rt_register_func_type(3, 0, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32);
  func_types[6] = wasm_rt_register_func_type(1, 1, WASM_RT_I32, WASM_RT_I32);
}

static void f0(void);
static u32 check(void);
static void f2(u32);
static void f3(u32, u32);
static void f4(u32, u32, u32);
static void _initialize(void);
static u32 __errno_location(void);
static u32 f7(u32, u32, u32);
static u32 f8(u32, u32, u32);
static u32 stackSave(void);
static void stackRestore(u32);
static u32 stackAlloc(u32);

static u32 g0;
static u32 inp;

static void init_globals(void) {
  g0 = 5245520u;
  inp = 1792u;
}

wasm_rt_memory_t memory;

static wasm_rt_table_t __indirect_function_table;

static void f0(void) {
  FUNC_PROLOGUE;
  FUNC_EPILOGUE;
}

static u32 check(void) {
  u32 l0 = 0, l1 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1, i2;
  i0 = 1744u;
  f2(i0);
  i0 = 1792u;
  i1 = 1824u;
  f3(i0, i1);
  i0 = 1800u;
  i1 = 1832u;
  f3(i0, i1);
  i0 = 0u;
  l0 = i0;
  i0 = 0u;
  i1 = 0u;
  i32_store8((&memory), (u64)(i0 + 1840), i1);
  i0 = 0u;
  i0 = i32_load8_u((&memory), (u64)(i0 + 1824));
  i1 = 0u;
  i1 = i32_load8_u((&memory), (u64)(i1 + 1760));
  i0 = i0 != i1;
  if (i0) {goto B0;}
  L2: 
    i0 = l0;
    l1 = i0;
    i1 = 1u;
    i0 += i1;
    l0 = i0;
    i1 = 16u;
    i0 = i0 == i1;
    if (i0) {goto B1;}
    i0 = l0;
    i1 = 1824u;
    i0 += i1;
    i0 = i32_load8_u((&memory), (u64)(i0));
    i1 = l0;
    i2 = 1760u;
    i1 += i2;
    i1 = i32_load8_u((&memory), (u64)(i1));
    i0 = i0 == i1;
    if (i0) {goto L2;}
  B1:;
  i0 = l1;
  i1 = 14u;
  i0 = i0 > i1;
  l0 = i0;
  B0:;
  i0 = l0;
  FUNC_EPILOGUE;
  return i0;
}

static void f2(u32 p0) {
  u32 l1 = 0, l2 = 0, l3 = 0, l4 = 0, l5 = 0, l6 = 0, l7 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1, i2, i3;
  i0 = g0;
  i1 = 192u;
  i0 -= i1;
  l1 = i0;
  g0 = i0;
  i0 = 0u;
  l2 = i0;
  L0: 
    i0 = l2;
    i1 = 3u;
    i0 <<= (i1 & 31);
    l3 = i0;
    i0 = p0;
    i1 = l2;
    i0 += i1;
    i0 = i32_load8_u((&memory), (u64)(i0));
    l4 = i0;
    i0 = 0u;
    l5 = i0;
    L1: 
      i0 = l1;
      i1 = 128u;
      i0 += i1;
      i1 = l5;
      i2 = l3;
      i1 += i2;
      i0 += i1;
      i1 = l4;
      i2 = 7u;
      i3 = l5;
      i2 -= i3;
      i1 >>= (i2 & 31);
      i2 = 1u;
      i1 &= i2;
      i32_store8((&memory), (u64)(i0), i1);
      i0 = l5;
      i1 = 1u;
      i0 += i1;
      l5 = i0;
      i1 = 8u;
      i0 = i0 != i1;
      if (i0) {goto L1;}
    i0 = l2;
    i1 = 1u;
    i0 += i1;
    l2 = i0;
    i1 = 8u;
    i0 = i0 != i1;
    if (i0) {goto L0;}
  i0 = 0u;
  l5 = i0;
  L2: 
    i0 = l1;
    i1 = 64u;
    i0 += i1;
    i1 = l5;
    i0 += i1;
    i1 = l5;
    i2 = 1104u;
    i1 += i2;
    i1 = i32_load8_s((&memory), (u64)(i1));
    i2 = l1;
    i3 = 128u;
    i2 += i3;
    i1 += i2;
    i2 = 4294967295u;
    i1 += i2;
    i1 = i32_load8_u((&memory), (u64)(i1));
    i32_store8((&memory), (u64)(i0), i1);
    i0 = l5;
    i1 = 1u;
    i0 += i1;
    l5 = i0;
    i1 = 56u;
    i0 = i0 != i1;
    if (i0) {goto L2;}
  i0 = 0u;
  l3 = i0;
  i0 = l1;
  i1 = 64u;
  i0 += i1;
  l5 = i0;
  i0 = l1;
  l4 = i0;
  L3: 
    i0 = l4;
    i1 = l5;
    l2 = i1;
    i2 = l3;
    i3 = 1216u;
    i2 += i3;
    i2 = i32_load8_s((&memory), (u64)(i2));
    l5 = i2;
    i1 += i2;
    i2 = 28u;
    i3 = l5;
    i2 -= i3;
    p0 = i2;
    i0 = f8(i0, i1, i2);
    l4 = i0;
    i1 = 28u;
    i0 += i1;
    l6 = i0;
    i1 = l5;
    i0 -= i1;
    i1 = l2;
    i2 = l5;
    i0 = f8(i0, i1, i2);
    i0 = l6;
    i1 = l2;
    i2 = 28u;
    i1 += i2;
    l7 = i1;
    i2 = l5;
    i1 += i2;
    i2 = p0;
    i0 = f8(i0, i1, i2);
    i0 = l4;
    i1 = l5;
    i0 -= i1;
    i1 = 56u;
    i0 += i1;
    i1 = l7;
    i2 = l5;
    i0 = f8(i0, i1, i2);
    i0 = 0u;
    l5 = i0;
    L4: 
      i0 = l3;
      i1 = 48u;
      i0 *= i1;
      i1 = l5;
      i0 += i1;
      i1 = 1856u;
      i0 += i1;
      i1 = l5;
      i2 = 1168u;
      i1 += i2;
      i1 = i32_load8_s((&memory), (u64)(i1));
      i2 = l4;
      i1 += i2;
      i2 = 4294967295u;
      i1 += i2;
      i1 = i32_load8_u((&memory), (u64)(i1));
      i32_store8((&memory), (u64)(i0), i1);
      i0 = l5;
      i1 = 1u;
      i0 += i1;
      l5 = i0;
      i1 = 48u;
      i0 = i0 != i1;
      if (i0) {goto L4;}
    i0 = l4;
    l5 = i0;
    i0 = l2;
    l4 = i0;
    i0 = l3;
    i1 = 1u;
    i0 += i1;
    l3 = i0;
    i1 = 16u;
    i0 = i0 != i1;
    if (i0) {goto L3;}
  i0 = l1;
  i1 = 192u;
  i0 += i1;
  g0 = i0;
  FUNC_EPILOGUE;
}

static void f3(u32 p0, u32 p1) {
  u32 l2 = 0, l3 = 0, l4 = 0, l5 = 0, l6 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1, i2, i3;
  i0 = g0;
  i1 = 96u;
  i0 -= i1;
  l2 = i0;
  g0 = i0;
  i0 = 0u;
  l3 = i0;
  L0: 
    i0 = l3;
    i1 = 3u;
    i0 <<= (i1 & 31);
    l4 = i0;
    i0 = p0;
    i1 = l3;
    i0 += i1;
    i0 = i32_load8_u((&memory), (u64)(i0));
    l5 = i0;
    i0 = 0u;
    l6 = i0;
    L1: 
      i0 = l2;
      i1 = 32u;
      i0 += i1;
      i1 = l6;
      i2 = l4;
      i1 += i2;
      i0 += i1;
      i1 = l5;
      i2 = 7u;
      i3 = l6;
      i2 -= i3;
      i1 >>= (i2 & 31);
      i2 = 1u;
      i1 &= i2;
      i32_store8((&memory), (u64)(i0), i1);
      i0 = l6;
      i1 = 1u;
      i0 += i1;
      l6 = i0;
      i1 = 8u;
      i0 = i0 != i1;
      if (i0) {goto L1;}
    i0 = l3;
    i1 = 1u;
    i0 += i1;
    l3 = i0;
    i1 = 8u;
    i0 = i0 != i1;
    if (i0) {goto L0;}
  i0 = l2;
  i1 = 32u;
  i0 += i1;
  i1 = 32u;
  i0 += i1;
  l3 = i0;
  i0 = 0u;
  p0 = i0;
  i0 = l2;
  i1 = 32u;
  i0 += i1;
  l6 = i0;
  L2: 
    i0 = l6;
    l5 = i0;
    i0 = l3;
    i1 = l2;
    i2 = p0;
    i3 = 48u;
    i2 *= i3;
    i3 = 1856u;
    i2 += i3;
    f4(i0, i1, i2);
    i0 = 0u;
    l6 = i0;
    L3: 
      i0 = l5;
      i1 = l6;
      i0 += i1;
      l4 = i0;
      i1 = l2;
      i2 = l6;
      i1 += i2;
      i1 = i32_load8_u((&memory), (u64)(i1));
      i2 = l4;
      i2 = i32_load8_u((&memory), (u64)(i2));
      i1 += i2;
      i2 = 1u;
      i1 &= i2;
      i32_store8((&memory), (u64)(i0), i1);
      i0 = l6;
      i1 = 1u;
      i0 += i1;
      l6 = i0;
      i1 = 32u;
      i0 = i0 != i1;
      if (i0) {goto L3;}
    i0 = l3;
    l6 = i0;
    i0 = l5;
    l3 = i0;
    i0 = p0;
    i1 = 1u;
    i0 += i1;
    p0 = i0;
    i1 = 16u;
    i0 = i0 != i1;
    if (i0) {goto L2;}
  i0 = 0u;
  l3 = i0;
  L4: 
    i0 = l2;
    i1 = 32u;
    i0 += i1;
    i1 = l3;
    i2 = 3u;
    i1 <<= (i2 & 31);
    l5 = i1;
    i0 += i1;
    i0 = i32_load8_u((&memory), (u64)(i0));
    i1 = 1u;
    i0 &= i1;
    l4 = i0;
    i0 = 1u;
    l6 = i0;
    L5: 
      i0 = l2;
      i1 = 32u;
      i0 += i1;
      i1 = l6;
      i2 = l5;
      i1 += i2;
      i0 += i1;
      i0 = i32_load8_u((&memory), (u64)(i0));
      i1 = 1u;
      i0 &= i1;
      i1 = l4;
      i2 = 1u;
      i1 <<= (i2 & 31);
      i0 |= i1;
      l4 = i0;
      i0 = l6;
      i1 = 1u;
      i0 += i1;
      l6 = i0;
      i1 = 8u;
      i0 = i0 != i1;
      if (i0) {goto L5;}
    i0 = p1;
    i1 = l3;
    i0 += i1;
    i1 = l4;
    i32_store8((&memory), (u64)(i0), i1);
    i0 = l3;
    i1 = 1u;
    i0 += i1;
    l3 = i0;
    i1 = 8u;
    i0 = i0 != i1;
    if (i0) {goto L4;}
  i0 = l2;
  i1 = 96u;
  i0 += i1;
  g0 = i0;
  FUNC_EPILOGUE;
}

static void f4(u32 p0, u32 p1, u32 p2) {
  u32 l3 = 0, l4 = 0, l5 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1, i2, i3, i4, i5;
  u64 j1;
  i0 = g0;
  i1 = 80u;
  i0 -= i1;
  l3 = i0;
  i0 = 0u;
  l4 = i0;
  i0 = 0u;
  l5 = i0;
  L0: 
    i0 = l3;
    i1 = 32u;
    i0 += i1;
    i1 = l5;
    i0 += i1;
    i1 = l5;
    i2 = 1024u;
    i1 += i2;
    i1 = i32_load8_s((&memory), (u64)(i1));
    i2 = p0;
    i1 += i2;
    i2 = 4294967295u;
    i1 += i2;
    i1 = i32_load8_u((&memory), (u64)(i1));
    i32_store8((&memory), (u64)(i0), i1);
    i0 = l5;
    i1 = 1u;
    i0 += i1;
    l5 = i0;
    i1 = 48u;
    i0 = i0 != i1;
    if (i0) {goto L0;}
  L1: 
    i0 = l3;
    i1 = 32u;
    i0 += i1;
    i1 = l4;
    i0 += i1;
    l5 = i0;
    i1 = p2;
    i2 = l4;
    i1 += i2;
    i1 = i32_load8_u((&memory), (u64)(i1));
    i2 = l5;
    i2 = i32_load8_u((&memory), (u64)(i2));
    i1 += i2;
    i2 = 1u;
    i1 &= i2;
    i32_store8((&memory), (u64)(i0), i1);
    i0 = l4;
    i1 = 1u;
    i0 += i1;
    l4 = i0;
    i1 = 48u;
    i0 = i0 != i1;
    if (i0) {goto L1;}
  i0 = 0u;
  p0 = i0;
  i0 = 0u;
  l4 = i0;
  L2: 
    i0 = l3;
    i1 = l4;
    i0 += i1;
    i1 = p0;
    i2 = 6u;
    i1 <<= (i2 & 31);
    i2 = l3;
    i3 = 32u;
    i2 += i3;
    i3 = p0;
    i4 = 6u;
    i3 *= i4;
    p2 = i3;
    i2 += i3;
    l5 = i2;
    i2 = i32_load8_u((&memory), (u64)(i2));
    i3 = 1u;
    i2 <<= (i3 & 31);
    i3 = l5;
    i4 = 5u;
    i3 += i4;
    i3 = i32_load8_u((&memory), (u64)(i3));
    i2 |= i3;
    i3 = 3u;
    i2 &= i3;
    i3 = 4u;
    i2 <<= (i3 & 31);
    i1 |= i2;
    i2 = l5;
    i3 = 2u;
    i2 += i3;
    i2 = i32_load8_u((&memory), (u64)(i2));
    i3 = 2u;
    i2 <<= (i3 & 31);
    i3 = l3;
    i4 = 32u;
    i3 += i4;
    i4 = p2;
    i5 = 1u;
    i4 |= i5;
    i3 += i4;
    i3 = i32_load8_u((&memory), (u64)(i3));
    i4 = 3u;
    i3 <<= (i4 & 31);
    i2 |= i3;
    i3 = l5;
    i4 = 3u;
    i3 += i4;
    i3 = i32_load8_u((&memory), (u64)(i3));
    i4 = 1u;
    i3 <<= (i4 & 31);
    i2 |= i3;
    i3 = l5;
    i4 = 4u;
    i3 += i4;
    i3 = i32_load8_u((&memory), (u64)(i3));
    i2 |= i3;
    i3 = 15u;
    i2 &= i3;
    i1 |= i2;
    i2 = 1232u;
    i1 += i2;
    i1 = i32_load8_u((&memory), (u64)(i1));
    l5 = i1;
    i2 = 3u;
    i1 >>= (i2 & 31);
    i2 = 1u;
    i1 &= i2;
    i32_store8((&memory), (u64)(i0), i1);
    i0 = l3;
    i1 = l4;
    i2 = 1u;
    i1 |= i2;
    i0 += i1;
    i1 = l5;
    i2 = 2u;
    i1 >>= (i2 & 31);
    i2 = 1u;
    i1 &= i2;
    i32_store8((&memory), (u64)(i0), i1);
    i0 = l3;
    i1 = l4;
    i2 = 2u;
    i1 |= i2;
    i0 += i1;
    i1 = l5;
    i2 = 1u;
    i1 >>= (i2 & 31);
    i2 = 1u;
    i1 &= i2;
    i32_store8((&memory), (u64)(i0), i1);
    i0 = l3;
    i1 = l4;
    i2 = 3u;
    i1 |= i2;
    i0 += i1;
    i1 = l5;
    i2 = 1u;
    i1 &= i2;
    i32_store8((&memory), (u64)(i0), i1);
    i0 = l4;
    i1 = 4u;
    i0 += i1;
    l4 = i0;
    i0 = p0;
    i1 = 1u;
    i0 += i1;
    p0 = i0;
    i1 = 8u;
    i0 = i0 != i1;
    if (i0) {goto L2;}
  i0 = 0u;
  l4 = i0;
  L3: 
    i0 = l3;
    i1 = 32u;
    i0 += i1;
    i1 = l4;
    i0 += i1;
    i1 = l4;
    i2 = 1072u;
    i1 += i2;
    i1 = i32_load8_s((&memory), (u64)(i1));
    i2 = l3;
    i1 += i2;
    i2 = 4294967295u;
    i1 += i2;
    i1 = i32_load8_u((&memory), (u64)(i1));
    i32_store8((&memory), (u64)(i0), i1);
    i0 = l4;
    i1 = 1u;
    i0 += i1;
    l4 = i0;
    i1 = 32u;
    i0 = i0 != i1;
    if (i0) {goto L3;}
  i0 = p1;
  i1 = l3;
  j1 = i64_load((&memory), (u64)(i1 + 32));
  i64_store((&memory), (u64)(i0), j1);
  i0 = p1;
  i1 = 8u;
  i0 += i1;
  i1 = l3;
  j1 = i64_load((&memory), (u64)(i1 + 40));
  i64_store((&memory), (u64)(i0), j1);
  i0 = p1;
  i1 = 24u;
  i0 += i1;
  i1 = l3;
  i2 = 32u;
  i1 += i2;
  i2 = 24u;
  i1 += i2;
  j1 = i64_load((&memory), (u64)(i1));
  i64_store((&memory), (u64)(i0), j1);
  i0 = p1;
  i1 = 16u;
  i0 += i1;
  i1 = l3;
  i2 = 32u;
  i1 += i2;
  i2 = 16u;
  i1 += i2;
  j1 = i64_load((&memory), (u64)(i1));
  i64_store((&memory), (u64)(i0), j1);
  FUNC_EPILOGUE;
}

static void _initialize(void) {
  FUNC_PROLOGUE;
  u32 i0;
  i0 = 1u;
  i0 = !(i0);
  if (i0) {goto B0;}
  f0();
  B0:;
  FUNC_EPILOGUE;
}

static u32 __errno_location(void) {
  FUNC_PROLOGUE;
  u32 i0;
  i0 = 2624u;
  FUNC_EPILOGUE;
  return i0;
}

static u32 f7(u32 p0, u32 p1, u32 p2) {
  u32 l3 = 0, l4 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1, i2, i3, i4, i5;
  i0 = p2;
  i0 = !(i0);
  if (i0) {goto B0;}
  i0 = p0;
  l3 = i0;
  L1: 
    i0 = l3;
    i1 = p1;
    i2 = p2;
    i3 = 508u;
    i4 = p2;
    i5 = 508u;
    i4 = i4 < i5;
    i2 = i4 ? i2 : i3;
    l4 = i2;
    i0 = f8(i0, i1, i2);
    l3 = i0;
    i0 = p1;
    i1 = 508u;
    i0 += i1;
    p1 = i0;
    i0 = l3;
    i1 = 508u;
    i0 += i1;
    l3 = i0;
    i0 = p2;
    i1 = l4;
    i0 -= i1;
    p2 = i0;
    if (i0) {goto L1;}
  B0:;
  i0 = p0;
  FUNC_EPILOGUE;
  return i0;
}

static u32 f8(u32 p0, u32 p1, u32 p2) {
  u32 l3 = 0, l4 = 0, l5 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1, i2;
  i0 = p2;
  i1 = 512u;
  i0 = i0 < i1;
  if (i0) {goto B0;}
  i0 = p0;
  i1 = p1;
  i2 = p2;
  i0 = f7(i0, i1, i2);
  i0 = p0;
  goto Bfunc;
  B0:;
  i0 = p0;
  i1 = p2;
  i0 += i1;
  l3 = i0;
  i0 = p1;
  i1 = p0;
  i0 ^= i1;
  i1 = 3u;
  i0 &= i1;
  if (i0) {goto B2;}
  i0 = p0;
  i1 = 3u;
  i0 &= i1;
  if (i0) {goto B4;}
  i0 = p0;
  p2 = i0;
  goto B3;
  B4:;
  i0 = p2;
  i1 = 1u;
  i0 = (u32)((s32)i0 >= (s32)i1);
  if (i0) {goto B5;}
  i0 = p0;
  p2 = i0;
  goto B3;
  B5:;
  i0 = p0;
  p2 = i0;
  L6: 
    i0 = p2;
    i1 = p1;
    i1 = i32_load8_u((&memory), (u64)(i1));
    i32_store8((&memory), (u64)(i0), i1);
    i0 = p1;
    i1 = 1u;
    i0 += i1;
    p1 = i0;
    i0 = p2;
    i1 = 1u;
    i0 += i1;
    p2 = i0;
    i1 = 3u;
    i0 &= i1;
    i0 = !(i0);
    if (i0) {goto B3;}
    i0 = p2;
    i1 = l3;
    i0 = i0 < i1;
    if (i0) {goto L6;}
  B3:;
  i0 = l3;
  i1 = 4294967292u;
  i0 &= i1;
  l4 = i0;
  i1 = 64u;
  i0 = i0 < i1;
  if (i0) {goto B7;}
  i0 = p2;
  i1 = l4;
  i2 = 4294967232u;
  i1 += i2;
  l5 = i1;
  i0 = i0 > i1;
  if (i0) {goto B7;}
  L8: 
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1));
    i32_store((&memory), (u64)(i0), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 4));
    i32_store((&memory), (u64)(i0 + 4), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 8));
    i32_store((&memory), (u64)(i0 + 8), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 12));
    i32_store((&memory), (u64)(i0 + 12), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 16));
    i32_store((&memory), (u64)(i0 + 16), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 20));
    i32_store((&memory), (u64)(i0 + 20), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 24));
    i32_store((&memory), (u64)(i0 + 24), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 28));
    i32_store((&memory), (u64)(i0 + 28), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 32));
    i32_store((&memory), (u64)(i0 + 32), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 36));
    i32_store((&memory), (u64)(i0 + 36), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 40));
    i32_store((&memory), (u64)(i0 + 40), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 44));
    i32_store((&memory), (u64)(i0 + 44), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 48));
    i32_store((&memory), (u64)(i0 + 48), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 52));
    i32_store((&memory), (u64)(i0 + 52), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 56));
    i32_store((&memory), (u64)(i0 + 56), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1 + 60));
    i32_store((&memory), (u64)(i0 + 60), i1);
    i0 = p1;
    i1 = 64u;
    i0 += i1;
    p1 = i0;
    i0 = p2;
    i1 = 64u;
    i0 += i1;
    p2 = i0;
    i1 = l5;
    i0 = i0 <= i1;
    if (i0) {goto L8;}
  B7:;
  i0 = p2;
  i1 = l4;
  i0 = i0 >= i1;
  if (i0) {goto B1;}
  L9: 
    i0 = p2;
    i1 = p1;
    i1 = i32_load((&memory), (u64)(i1));
    i32_store((&memory), (u64)(i0), i1);
    i0 = p1;
    i1 = 4u;
    i0 += i1;
    p1 = i0;
    i0 = p2;
    i1 = 4u;
    i0 += i1;
    p2 = i0;
    i1 = l4;
    i0 = i0 < i1;
    if (i0) {goto L9;}
    goto B1;
  UNREACHABLE;
  B2:;
  i0 = l3;
  i1 = 4u;
  i0 = i0 >= i1;
  if (i0) {goto B10;}
  i0 = p0;
  p2 = i0;
  goto B1;
  B10:;
  i0 = l3;
  i1 = 4294967292u;
  i0 += i1;
  l4 = i0;
  i1 = p0;
  i0 = i0 >= i1;
  if (i0) {goto B11;}
  i0 = p0;
  p2 = i0;
  goto B1;
  B11:;
  i0 = p0;
  p2 = i0;
  L12: 
    i0 = p2;
    i1 = p1;
    i1 = i32_load8_u((&memory), (u64)(i1));
    i32_store8((&memory), (u64)(i0), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load8_u((&memory), (u64)(i1 + 1));
    i32_store8((&memory), (u64)(i0 + 1), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load8_u((&memory), (u64)(i1 + 2));
    i32_store8((&memory), (u64)(i0 + 2), i1);
    i0 = p2;
    i1 = p1;
    i1 = i32_load8_u((&memory), (u64)(i1 + 3));
    i32_store8((&memory), (u64)(i0 + 3), i1);
    i0 = p1;
    i1 = 4u;
    i0 += i1;
    p1 = i0;
    i0 = p2;
    i1 = 4u;
    i0 += i1;
    p2 = i0;
    i1 = l4;
    i0 = i0 <= i1;
    if (i0) {goto L12;}
  B1:;
  i0 = p2;
  i1 = l3;
  i0 = i0 >= i1;
  if (i0) {goto B13;}
  L14: 
    i0 = p2;
    i1 = p1;
    i1 = i32_load8_u((&memory), (u64)(i1));
    i32_store8((&memory), (u64)(i0), i1);
    i0 = p1;
    i1 = 1u;
    i0 += i1;
    p1 = i0;
    i0 = p2;
    i1 = 1u;
    i0 += i1;
    p2 = i0;
    i1 = l3;
    i0 = i0 != i1;
    if (i0) {goto L14;}
  B13:;
  i0 = p0;
  Bfunc:;
  FUNC_EPILOGUE;
  return i0;
}

static u32 stackSave(void) {
  FUNC_PROLOGUE;
  u32 i0;
  i0 = g0;
  FUNC_EPILOGUE;
  return i0;
}

static void stackRestore(u32 p0) {
  FUNC_PROLOGUE;
  u32 i0;
  i0 = p0;
  g0 = i0;
  FUNC_EPILOGUE;
}

static u32 stackAlloc(u32 p0) {
  u32 l1 = 0, l2 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1;
  i0 = g0;
  i1 = p0;
  i0 -= i1;
  i1 = 4294967280u;
  i0 &= i1;
  l1 = i0;
  g0 = i0;
  i0 = l1;
  FUNC_EPILOGUE;
  return i0;
}

static const u8 data_segment_data_0[] = {
  0x20, 0x01, 0x02, 0x03, 0x04, 0x05, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 
  0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x01, 
  0x10, 0x07, 0x14, 0x15, 0x1d, 0x0c, 0x1c, 0x11, 0x01, 0x0f, 0x17, 0x1a, 
  0x05, 0x12, 0x1f, 0x0a, 0x02, 0x08, 0x18, 0x0e, 0x20, 0x1b, 0x03, 0x09, 
  0x13, 0x0d, 0x1e, 0x06, 0x16, 0x0b, 0x04, 0x19, 0x39, 0x31, 0x29, 0x21, 
  0x19, 0x11, 0x09, 0x01, 0x3a, 0x32, 0x2a, 0x22, 0x1a, 0x12, 0x0a, 0x02, 
  0x3b, 0x33, 0x2b, 0x23, 0x1b, 0x13, 0x0b, 0x03, 0x3c, 0x34, 0x2c, 0x24, 
  0x3f, 0x37, 0x2f, 0x27, 0x1f, 0x17, 0x0f, 0x07, 0x3e, 0x36, 0x2e, 0x26, 
  0x1e, 0x16, 0x0e, 0x06, 0x3d, 0x35, 0x2d, 0x25, 0x1d, 0x15, 0x0d, 0x05, 
  0x1c, 0x14, 0x0c, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x0e, 0x11, 0x0b, 0x18, 0x01, 0x05, 0x03, 0x1c, 0x0f, 0x06, 0x15, 0x0a, 
  0x17, 0x13, 0x0c, 0x04, 0x1a, 0x08, 0x10, 0x07, 0x1b, 0x14, 0x0d, 0x02, 
  0x29, 0x34, 0x1f, 0x25, 0x2f, 0x37, 0x1e, 0x28, 0x33, 0x2d, 0x21, 0x30, 
  0x2c, 0x31, 0x27, 0x38, 0x22, 0x35, 0x2e, 0x2a, 0x32, 0x24, 0x1d, 0x20, 
  0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x02, 0x02, 0x02, 
  0x02, 0x02, 0x02, 0x01, 0x0e, 0x04, 0x0d, 0x01, 0x02, 0x0f, 0x0b, 0x08, 
  0x03, 0x0a, 0x06, 0x0c, 0x05, 0x09, 0x00, 0x07, 0x00, 0x0f, 0x07, 0x04, 
  0x0e, 0x02, 0x0d, 0x01, 0x0a, 0x06, 0x0c, 0x0b, 0x09, 0x05, 0x03, 0x08, 
  0x04, 0x01, 0x0e, 0x08, 0x0d, 0x06, 0x02, 0x0b, 0x0f, 0x0c, 0x09, 0x07, 
  0x03, 0x0a, 0x05, 0x00, 0x0f, 0x0c, 0x08, 0x02, 0x04, 0x09, 0x01, 0x07, 
  0x05, 0x0b, 0x03, 0x0e, 0x0a, 0x00, 0x06, 0x0d, 0x0f, 0x01, 0x08, 0x0e, 
  0x06, 0x0b, 0x03, 0x04, 0x09, 0x07, 0x02, 0x0d, 0x0c, 0x00, 0x05, 0x0a, 
  0x03, 0x0d, 0x04, 0x07, 0x0f, 0x02, 0x08, 0x0e, 0x0c, 0x00, 0x01, 0x0a, 
  0x06, 0x09, 0x0b, 0x05, 0x00, 0x0e, 0x07, 0x0b, 0x0a, 0x04, 0x0d, 0x01, 
  0x05, 0x08, 0x0c, 0x06, 0x09, 0x03, 0x02, 0x0f, 0x0d, 0x08, 0x0a, 0x01, 
  0x03, 0x0f, 0x04, 0x02, 0x0b, 0x06, 0x07, 0x0c, 0x00, 0x05, 0x0e, 0x09, 
  0x0a, 0x00, 0x09, 0x0e, 0x06, 0x03, 0x0f, 0x05, 0x01, 0x0d, 0x0c, 0x07, 
  0x0b, 0x04, 0x02, 0x08, 0x0d, 0x07, 0x00, 0x09, 0x03, 0x04, 0x06, 0x0a, 
  0x02, 0x08, 0x05, 0x0e, 0x0c, 0x0b, 0x0f, 0x01, 0x0d, 0x06, 0x04, 0x09, 
  0x08, 0x0f, 0x03, 0x00, 0x0b, 0x01, 0x02, 0x0c, 0x05, 0x0a, 0x0e, 0x07, 
  0x01, 0x0a, 0x0d, 0x00, 0x06, 0x09, 0x08, 0x07, 0x04, 0x0f, 0x0e, 0x03, 
  0x0b, 0x05, 0x02, 0x0c, 0x07, 0x0d, 0x0e, 0x03, 0x00, 0x06, 0x09, 0x0a, 
  0x01, 0x02, 0x08, 0x05, 0x0b, 0x0c, 0x04, 0x0f, 0x0d, 0x08, 0x0b, 0x05, 
  0x06, 0x0f, 0x00, 0x03, 0x04, 0x07, 0x02, 0x0c, 0x01, 0x0a, 0x0e, 0x09, 
  0x0a, 0x06, 0x09, 0x00, 0x0c, 0x0b, 0x07, 0x0d, 0x0f, 0x01, 0x03, 0x0e, 
  0x05, 0x02, 0x08, 0x04, 0x03, 0x0f, 0x00, 0x06, 0x0a, 0x01, 0x0d, 0x08, 
  0x09, 0x04, 0x05, 0x0b, 0x0c, 0x07, 0x02, 0x0e, 0x02, 0x0c, 0x04, 0x01, 
  0x07, 0x0a, 0x0b, 0x06, 0x08, 0x05, 0x03, 0x0f, 0x0d, 0x00, 0x0e, 0x09, 
  0x0e, 0x0b, 0x02, 0x0c, 0x04, 0x07, 0x0d, 0x01, 0x05, 0x00, 0x0f, 0x0a, 
  0x03, 0x09, 0x08, 0x06, 0x04, 0x02, 0x01, 0x0b, 0x0a, 0x0d, 0x07, 0x08, 
  0x0f, 0x09, 0x0c, 0x05, 0x06, 0x03, 0x00, 0x0e, 0x0b, 0x08, 0x0c, 0x07, 
  0x01, 0x0e, 0x02, 0x0d, 0x06, 0x0f, 0x00, 0x09, 0x0a, 0x04, 0x05, 0x03, 
  0x0c, 0x01, 0x0a, 0x0f, 0x09, 0x02, 0x06, 0x08, 0x00, 0x0d, 0x03, 0x04, 
  0x0e, 0x07, 0x05, 0x0b, 0x0a, 0x0f, 0x04, 0x02, 0x07, 0x0c, 0x09, 0x05, 
  0x06, 0x01, 0x0d, 0x0e, 0x00, 0x0b, 0x03, 0x08, 0x09, 0x0e, 0x0f, 0x05, 
  0x02, 0x08, 0x0c, 0x03, 0x07, 0x00, 0x04, 0x0a, 0x01, 0x0d, 0x0b, 0x06, 
  0x04, 0x03, 0x02, 0x0c, 0x09, 0x05, 0x0f, 0x0a, 0x0b, 0x0e, 0x01, 0x07, 
  0x06, 0x00, 0x08, 0x0d, 0x04, 0x0b, 0x02, 0x0e, 0x0f, 0x00, 0x08, 0x0d, 
  0x03, 0x0c, 0x09, 0x07, 0x05, 0x0a, 0x06, 0x01, 0x0d, 0x00, 0x0b, 0x07, 
  0x04, 0x09, 0x01, 0x0a, 0x0e, 0x03, 0x05, 0x0c, 0x02, 0x0f, 0x08, 0x06, 
  0x01, 0x04, 0x0b, 0x0d, 0x0c, 0x03, 0x07, 0x0e, 0x0a, 0x0f, 0x06, 0x08, 
  0x00, 0x05, 0x09, 0x02, 0x06, 0x0b, 0x0d, 0x08, 0x01, 0x04, 0x0a, 0x07, 
  0x09, 0x05, 0x00, 0x0f, 0x0e, 0x02, 0x03, 0x0c, 0x0d, 0x02, 0x08, 0x04, 
  0x06, 0x0f, 0x0b, 0x01, 0x0a, 0x09, 0x03, 0x0e, 0x05, 0x00, 0x0c, 0x07, 
  0x01, 0x0f, 0x0d, 0x08, 0x0a, 0x03, 0x07, 0x04, 0x0c, 0x05, 0x06, 0x0b, 
  0x00, 0x0e, 0x09, 0x02, 0x07, 0x0b, 0x04, 0x01, 0x09, 0x0c, 0x0e, 0x02, 
  0x00, 0x06, 0x0a, 0x0d, 0x0f, 0x03, 0x05, 0x08, 0x02, 0x01, 0x0e, 0x07, 
  0x04, 0x0a, 0x08, 0x0d, 0x0f, 0x0c, 0x09, 0x00, 0x03, 0x05, 0x06, 0x0b, 
  
};

static const u8 data_segment_data_1[] = {
  0x43, 0x31, 0x6e, 0x45, 0x5f, 0x4b, 0x6c, 0x42, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x82, 0xb0, 0xab, 0xe4, 0x2d, 0x54, 0x5b, 0x2b, 
  0xd7, 0xf2, 0x54, 0xb8, 0x1f, 0x1b, 0xc7, 0x92, 0x00, 0x00, 0x00, 0x00, 
  
};

static void init_memory(void) {
  wasm_rt_allocate_memory((&memory), 256, 256);
  memcpy(&(memory.data[1024u]), data_segment_data_0, 720);
  memcpy(&(memory.data[1744u]), data_segment_data_1, 36);
}

static void init_table(void) {
  uint32_t offset;
  wasm_rt_allocate_table((&__indirect_function_table), 2, 2);
  offset = 1u;
  __indirect_function_table.data[offset + 0] = (wasm_rt_elem_t){func_types[1], (wasm_rt_anyfunc_t)(&f0)};
}

/* export: 'memory' */
wasm_rt_memory_t (*WASM_RT_ADD_PREFIX(Z_memory));
/* export: 'check' */
u32 (*WASM_RT_ADD_PREFIX(Z_checkZ_iv))(void);
/* export: 'inp' */
u32 (*WASM_RT_ADD_PREFIX(Z_inpZ_i));
/* export: '_initialize' */
void (*WASM_RT_ADD_PREFIX(Z__initializeZ_vv))(void);
/* export: '__indirect_function_table' */
wasm_rt_table_t (*WASM_RT_ADD_PREFIX(Z___indirect_function_table));
/* export: '__errno_location' */
u32 (*WASM_RT_ADD_PREFIX(Z___errno_locationZ_iv))(void);
/* export: 'stackSave' */
u32 (*WASM_RT_ADD_PREFIX(Z_stackSaveZ_iv))(void);
/* export: 'stackRestore' */
void (*WASM_RT_ADD_PREFIX(Z_stackRestoreZ_vi))(u32);
/* export: 'stackAlloc' */
u32 (*WASM_RT_ADD_PREFIX(Z_stackAllocZ_ii))(u32);

static void init_exports(void) {
  /* export: 'memory' */
  WASM_RT_ADD_PREFIX(Z_memory) = (&memory);
  /* export: 'check' */
  WASM_RT_ADD_PREFIX(Z_checkZ_iv) = (&check);
  /* export: 'inp' */
  WASM_RT_ADD_PREFIX(Z_inpZ_i) = (&inp);
  /* export: '_initialize' */
  WASM_RT_ADD_PREFIX(Z__initializeZ_vv) = (&_initialize);
  /* export: '__indirect_function_table' */
  WASM_RT_ADD_PREFIX(Z___indirect_function_table) = (&__indirect_function_table);
  /* export: '__errno_location' */
  WASM_RT_ADD_PREFIX(Z___errno_locationZ_iv) = (&__errno_location);
  /* export: 'stackSave' */
  WASM_RT_ADD_PREFIX(Z_stackSaveZ_iv) = (&stackSave);
  /* export: 'stackRestore' */
  WASM_RT_ADD_PREFIX(Z_stackRestoreZ_vi) = (&stackRestore);
  /* export: 'stackAlloc' */
  WASM_RT_ADD_PREFIX(Z_stackAllocZ_ii) = (&stackAlloc);
}

void WASM_RT_ADD_PREFIX(init)(void) {
  init_func_types();
  init_globals();
  init_memory();
  init_table();
  init_exports();
}
