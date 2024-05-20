#include "stdarg.h"
#include "ctype.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "unistd.h"
#include "errno.h"
#include "SDL2/SDL.h"
#include "math.h"

/* Embed: /home/runner/work/og-wasm-template/og-wasm-template/ocen/std/og/interface.c */
/**
 * This is a simple interface to register a main loop function.
 * It's written at the C level since we want some compile-time
 * conditional logic dependong on the target platform, which
 * we cannot do natively in Ocen right now.
*/

// NOTE: Emscripten expects a callback that doesn't return anything,
//       but we want to be able to return a boolean to indicate when
//       the main loop should stop. We'll use a wrapper function to
//       handle this, and store the actual callback in a global variable.


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

_Bool (*callback)(void) = NULL;
// EMS no-return callback wrapper
void ems_callback() {
    if (callback) callback();
}
#endif

void c_set_main_loop(_Bool (*func)(void)) {
#ifdef __EMSCRIPTEN__
    callback = func;
    emscripten_set_main_loop(ems_callback, 0, 1);
#else
    while (func()) {}
#endif
}


/* Embed: /home/runner/work/og-wasm-template/og-wasm-template/ocen/std/./prelude.h */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>

#ifndef OC_NO_BACKTRACE
#include <execinfo.h>
#endif

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

void dump_backtrace() {
#ifndef OC_NO_BACKTRACE
  void *array[40];
  size_t size = backtrace(array, 40);
  char **strings = backtrace_symbols(array, size);
  printf("\nBacktrace:\n");
  for (size_t i = 1; i < size; i++) {
    printf("%s\n", strings[i]);
  }
  free(strings);
#endif
}

#ifdef __APPLE__
  #define oc_trap __builtin_debugtrap
#else
  #define oc_trap __builtin_trap
#endif

void ae_assert_fail(char *dbg_msg, char *msg) {
  printf("--------------------------------------------------------------------------------\n");
  printf("%s\n", dbg_msg);
  if (msg) {
    printf("  Message: %s\n", msg);
  }
  printf("--------------------------------------------------------------------------------\n");
  fflush(stdout);
  dump_backtrace();
  oc_trap();
}


/* Enums */
typedef enum std_Endian {
  std_Endian_Little,
  std_Endian_Big,
} std_Endian;

char *std_Endian_dbg(std_Endian this) {
  switch (this) {
    case std_Endian_Little: return "Little";
    case std_Endian_Big: return "Big";
    default: return "<unknown>";
  }
}

typedef enum std_og_state_MouseWheel {
  std_og_state_MouseWheel_Down,
  std_og_state_MouseWheel_Right,
  std_og_state_MouseWheel_Up,
  std_og_state_MouseWheel_Left,
} std_og_state_MouseWheel;

char *std_og_state_MouseWheel_dbg(std_og_state_MouseWheel this) {
  switch (this) {
    case std_og_state_MouseWheel_Down: return "Down";
    case std_og_state_MouseWheel_Right: return "Right";
    case std_og_state_MouseWheel_Up: return "Up";
    case std_og_state_MouseWheel_Left: return "Left";
    default: return "<unknown>";
  }
}

char *std_sdl_Key_dbg(SDL_Scancode this) {
  switch (this) {
    case SDL_SCANCODE_A: return "A";
    case SDL_SCANCODE_B: return "B";
    case SDL_SCANCODE_C: return "C";
    case SDL_SCANCODE_D: return "D";
    case SDL_SCANCODE_E: return "E";
    case SDL_SCANCODE_F: return "F";
    case SDL_SCANCODE_G: return "G";
    case SDL_SCANCODE_H: return "H";
    case SDL_SCANCODE_I: return "I";
    case SDL_SCANCODE_J: return "J";
    case SDL_SCANCODE_K: return "K";
    case SDL_SCANCODE_L: return "L";
    case SDL_SCANCODE_M: return "M";
    case SDL_SCANCODE_N: return "N";
    case SDL_SCANCODE_O: return "O";
    case SDL_SCANCODE_P: return "P";
    case SDL_SCANCODE_Q: return "Q";
    case SDL_SCANCODE_R: return "R";
    case SDL_SCANCODE_S: return "S";
    case SDL_SCANCODE_T: return "T";
    case SDL_SCANCODE_U: return "U";
    case SDL_SCANCODE_V: return "V";
    case SDL_SCANCODE_W: return "W";
    case SDL_SCANCODE_X: return "X";
    case SDL_SCANCODE_Y: return "Y";
    case SDL_SCANCODE_Z: return "Z";
    case SDL_SCANCODE_UP: return "Up";
    case SDL_SCANCODE_DOWN: return "Down";
    case SDL_SCANCODE_LEFT: return "Left";
    case SDL_SCANCODE_RIGHT: return "Right";
    case SDL_SCANCODE_SPACE: return "Space";
    case SDL_SCANCODE_ESCAPE: return "Escape";
    case SDL_SCANCODE_COMMA: return "Comma";
    case SDL_SCANCODE_PERIOD: return "Dot";
    default: return "<unknown>";
  }
}

char *std_sdl_MouseButton_dbg(u8 this) {
  switch (this) {
    case SDL_BUTTON_LEFT: return "Left";
    case SDL_BUTTON_MIDDLE: return "Middle";
    case SDL_BUTTON_RIGHT: return "Right";
    case SDL_BUTTON_X1: return "Forward";
    case SDL_BUTTON_X2: return "Backward";
    default: return "<unknown>";
  }
}

char *std_sdl_EventType_dbg(SDL_EventType this) {
  switch (this) {
    case SDL_QUIT: return "Quit";
    case SDL_KEYDOWN: return "KeyDown";
    case SDL_KEYUP: return "KeyUp";
    case SDL_MOUSEBUTTONDOWN: return "MouseDown";
    case SDL_MOUSEBUTTONUP: return "MouseUp";
    case SDL_MOUSEWHEEL: return "Wheel";
    case SDL_WINDOWEVENT: return "Window";
    case SDL_USEREVENT: return "User";
    default: return "<unknown>";
  }
}

char *std_sdl_BlendMode_dbg(SDL_BlendMode this) {
  switch (this) {
    case SDL_BLENDMODE_NONE: return "None";
    case SDL_BLENDMODE_BLEND: return "Blend";
    case SDL_BLENDMODE_ADD: return "Add";
    case SDL_BLENDMODE_MOD: return "Mod";
    default: return "<unknown>";
  }
}

char *std_sdl_WindowMode_dbg(int this) {
  switch (this) {
    case 0: return "Windowed";
    case SDL_WINDOW_FULLSCREEN: return "Fullscreen";
    case SDL_WINDOW_FULLSCREEN_DESKTOP: return "Borderless";
    default: return "<unknown>";
  }
}

char *std_sdl_CursorType_dbg(SDL_SystemCursor this) {
  switch (this) {
    case SDL_SYSTEM_CURSOR_ARROW: return "Arrow";
    case SDL_SYSTEM_CURSOR_IBEAM: return "IBeam";
    case SDL_SYSTEM_CURSOR_WAIT: return "Wait";
    case SDL_SYSTEM_CURSOR_CROSSHAIR: return "Crosshair";
    case SDL_SYSTEM_CURSOR_WAITARROW: return "WaitArrow";
    case SDL_SYSTEM_CURSOR_SIZENWSE: return "SizeNWSE";
    case SDL_SYSTEM_CURSOR_SIZENESW: return "SizeNESW";
    case SDL_SYSTEM_CURSOR_SIZEWE: return "SizeWE";
    case SDL_SYSTEM_CURSOR_SIZENS: return "SizeNS";
    case SDL_SYSTEM_CURSOR_SIZEALL: return "SizeAll";
    case SDL_SYSTEM_CURSOR_NO: return "No";
    case SDL_SYSTEM_CURSOR_HAND: return "Hand";
    default: return "<unknown>";
  }
}

/* Constants */
#define std_math_TAU ((M_PI * 2.0f))
#define std_og_state_FRAMETIMES_COUNT (200)
/* Struct typedefs */
typedef struct std_CharIterator std_CharIterator;
typedef struct std_vec_Vec2__0 std_vec_Vec2__0;
typedef struct std_vec_Vec2__1 std_vec_Vec2__1;
typedef struct std_vec_Vec2__2 std_vec_Vec2__2;
typedef struct std_vec_Vec2__3 std_vec_Vec2__3;
typedef struct std_vec_Vec2__4 std_vec_Vec2__4;
typedef struct std_vec_Vec2__5 std_vec_Vec2__5;
typedef struct std_vec_Vec3__0 std_vec_Vec3__0;
typedef struct std_vec_Vec3__1 std_vec_Vec3__1;
typedef struct std_vec_Vec3__2 std_vec_Vec3__2;
typedef struct std_vec_Vec3__3 std_vec_Vec3__3;
typedef struct std_vec_Vec3__4 std_vec_Vec3__4;
typedef struct std_vec_Vec3__5 std_vec_Vec3__5;
typedef struct std_og_Rect std_og_Rect;
typedef struct std_og_Texture std_og_Texture;
typedef struct std_og_state_KeyState std_og_state_KeyState;
typedef struct std_og_state_MouseState std_og_state_MouseState;

/* Struct definitions */
struct std_CharIterator {
  char *data;
  u32 len;
  u32 pos;
};

struct std_vec_Vec2__0 {
  u64 x;
  u64 y;
};

struct std_vec_Vec2__1 {
  f32 x;
  f32 y;
};

struct std_vec_Vec2__2 {
  f64 x;
  f64 y;
};

struct std_vec_Vec2__3 {
  i32 x;
  i32 y;
};

struct std_vec_Vec2__4 {
  i64 x;
  i64 y;
};

struct std_vec_Vec2__5 {
  u32 x;
  u32 y;
};

struct std_vec_Vec3__0 {
  f32 x;
  f32 y;
  f32 z;
};

struct std_vec_Vec3__1 {
  f64 x;
  f64 y;
  f64 z;
};

struct std_vec_Vec3__2 {
  i32 x;
  i32 y;
  i32 z;
};

struct std_vec_Vec3__3 {
  i64 x;
  i64 y;
  i64 z;
};

struct std_vec_Vec3__4 {
  u32 x;
  u32 y;
  u32 z;
};

struct std_vec_Vec3__5 {
  u64 x;
  u64 y;
  u64 z;
};

struct std_og_Rect {
  i32 x;
  i32 y;
  i32 w;
  i32 h;
};

struct std_og_Texture {
  SDL_Texture *tx;
  i32 w;
  i32 h;
  u8 *pixels;
  i32 pitch;
};

struct std_og_state_KeyState {
  bool keys[SDL_NUM_SCANCODES];
};

struct std_og_state_MouseState {
  std_vec_Vec2__3 vec;
  std_vec_Vec2__3 pos;
  bool buttons[16];
  bool wheel_dirs[8];
};


/* function declarations */
i32 main(void);
void og_wasm_template_main_main(void);
void std_panic(char *msg) __attribute__((noreturn));
u32 str_to_u32(char *this);
bool str_eq(char *this, char *other);
char *std_format(char *fmt, ...);
char *str_substring(char *this, u32 start, u32 len);
bool str_ends_with(char *this, char *suffix);
bool str_starts_with(char *this, char *prefix);
void str_strip_trailing_whitespace(char *this);
void str_remove_last_n(char *this, u32 n);
void str_replace_with(char **this, char *other);
void str_free(char **this);
std_CharIterator str_chars(char *this, u32 start);
bool std_CharIterator_has_value(std_CharIterator *this);
u32 std_CharIterator_next(std_CharIterator *this);
char std_CharIterator_cur(std_CharIterator *this);
bool char_is_digit(char this);
bool char_is_hex_digit(char this);
u32 char_get_digit(char this);
u8 char_get_hex_digit(char this);
bool char_is_alpha(char this);
bool char_is_alnum(char this);
bool char_is_print(char this);
bool char_is_space(char this);
bool char_is_upper(char this);
bool char_is_lower(char this);
i32 i32_min(i32 this, i32 b);
i32 i32_max(i32 this, i32 b);
i8 i8_min(i8 this, i8 b);
i8 i8_max(i8 this, i8 b);
u64 u64_min(u64 this, u64 other);
u64 u64_max(u64 this, u64 other);
u32 u32_min(u32 this, u32 other);
u32 u32_max(u32 this, u32 other);
u8 u8_min(u8 this, u8 other);
u8 u8_max(u8 this, u8 other);
char *std_shift_args(i32 *argc, char ***argv, char *where);
void std_mem_set_allocator(void *allocator, void *(*alloc_fn)(void *, u32), void (*free_fn)(void *, void *), void *(*realloc_fn)(void *, void *, u32, u32));
void std_mem_reset_to_default_allocator(void);
char *std_mem_alloc__0(u32 count);
void std_mem_free(void *ptr);
void *std_mem_impl_my_calloc(void *state, u32 size);
void *std_mem_impl_my_realloc(void *state, void *ptr, u32 old_size, u32 size);
void std_mem_impl_my_free(void *state, void *ptr);
char *std_libc_get_err(void);
u32 std_traits_hash_hash_bytes(u8 *data, u32 len);
u32 std_traits_hash_pair_hash(u32 a, u32 b);
u32 std_traits_hash_ptr_hash(void *ptr);
u32 str_hash(char *this);
u32 u8_hash(u8 this);
u32 i8_hash(i8 this);
u32 u16_hash(u16 this);
u32 i16_hash(i16 this);
u32 u32_hash(u32 this);
u32 i32_hash(i32 this);
u32 u64_hash(u64 this);
u32 i64_hash(i64 this);
u32 char_hash(char this);
u32 f32_hash(f32 this);
u32 f64_hash(f64 this);
f32 f32_inf(void);
f64 f64_inf(void);
f32 f32_min(f32 this, f32 b);
f32 f32_max(f32 this, f32 b);
f64 f64_min(f64 this, f64 b);
f64 f64_max(f64 this, f64 b);
f32 f32_clamp(f32 this, f32 min, f32 max);
f32 f32_clamp01(f32 this);
f64 f64_clamp(f64 this, f64 min, f64 max);
f64 f64_clamp01(f64 this);
bool f32_is_close(f32 this, f32 other, f32 thresh);
bool f64_is_close(f64 this, f64 other, f64 thresh);
i8 i8_abs(i8 this);
i16 i16_abs(i16 this);
i32 i32_abs(i32 this);
i64 i64_abs(i64 this);
f32 std_math_degrees(f32 radians);
f32 std_math_radians(f32 degrees);
std_vec_Vec2__0 std_vec_Vec2__0_subrs(u64 val, std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_sub(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
std_vec_Vec2__1 std_vec_Vec2__0_normalized(std_vec_Vec2__0 this);
std_vec_Vec2__1 std_vec_Vec2__1_subrs(f32 val, std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_sub(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
std_vec_Vec2__1 std_vec_Vec2__1_normalized(std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_multrs(f32 val, std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_add(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
f32 std_vec_Vec2__1_length_sq(std_vec_Vec2__1 this);
std_vec_Vec2__2 std_vec_Vec2__1_to_f64(std_vec_Vec2__1 this);
std_vec_Vec2__2 std_vec_Vec2__2_subrs(f64 val, std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_sub(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
std_vec_Vec2__1 std_vec_Vec2__2_normalized(std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_multrs(f64 val, std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_add(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
f64 std_vec_Vec2__2_length_sq(std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_to_f64(std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_mult(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
std_vec_Vec2__2 std_vec_Vec2__2_addrs(f64 val, std_vec_Vec2__2 this);
std_vec_Vec2__1 std_vec_Vec2__2_to_f32(std_vec_Vec2__2 this);
std_vec_Vec2__3 std_vec_Vec2__2_to_i32(std_vec_Vec2__2 this);
std_vec_Vec2__3 std_vec_Vec2__3_subrs(i32 val, std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_sub(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
std_vec_Vec2__1 std_vec_Vec2__3_normalized(std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_multrs(i32 val, std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_add(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
i32 std_vec_Vec2__3_length_sq(std_vec_Vec2__3 this);
std_vec_Vec2__2 std_vec_Vec2__3_to_f64(std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_mult(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
std_vec_Vec2__3 std_vec_Vec2__3_addrs(i32 val, std_vec_Vec2__3 this);
std_vec_Vec2__1 std_vec_Vec2__3_to_f32(std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_to_i32(std_vec_Vec2__3 this);
std_vec_Vec2__4 std_vec_Vec2__3_to_i64(std_vec_Vec2__3 this);
std_vec_Vec2__4 std_vec_Vec2__4_subrs(i64 val, std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_sub(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
std_vec_Vec2__1 std_vec_Vec2__4_normalized(std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_multrs(i64 val, std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_add(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
i64 std_vec_Vec2__4_length_sq(std_vec_Vec2__4 this);
std_vec_Vec2__2 std_vec_Vec2__4_to_f64(std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_mult(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
std_vec_Vec2__4 std_vec_Vec2__4_addrs(i64 val, std_vec_Vec2__4 this);
std_vec_Vec2__1 std_vec_Vec2__4_to_f32(std_vec_Vec2__4 this);
std_vec_Vec2__3 std_vec_Vec2__4_to_i32(std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_to_i64(std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_reflect(std_vec_Vec2__4 this, std_vec_Vec2__4 normal);
std_vec_Vec2__4 std_vec_Vec2__4_divs(std_vec_Vec2__4 this, i64 val);
bool std_vec_Vec2__4_eq(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
std_vec_Vec2__4 std_vec_Vec2__4_subs(std_vec_Vec2__4 this, i64 val);
std_vec_Vec2__5 std_vec_Vec2__4_to_u32(std_vec_Vec2__4 this);
std_vec_Vec2__5 std_vec_Vec2__5_subrs(u32 val, std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_sub(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
std_vec_Vec2__1 std_vec_Vec2__5_normalized(std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_multrs(u32 val, std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_add(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
u32 std_vec_Vec2__5_length_sq(std_vec_Vec2__5 this);
std_vec_Vec2__2 std_vec_Vec2__5_to_f64(std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_mult(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
std_vec_Vec2__5 std_vec_Vec2__5_addrs(u32 val, std_vec_Vec2__5 this);
std_vec_Vec2__1 std_vec_Vec2__5_to_f32(std_vec_Vec2__5 this);
std_vec_Vec2__3 std_vec_Vec2__5_to_i32(std_vec_Vec2__5 this);
std_vec_Vec2__4 std_vec_Vec2__5_to_i64(std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_reflect(std_vec_Vec2__5 this, std_vec_Vec2__5 normal);
std_vec_Vec2__5 std_vec_Vec2__5_divs(std_vec_Vec2__5 this, u32 val);
bool std_vec_Vec2__5_eq(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
std_vec_Vec2__5 std_vec_Vec2__5_subs(std_vec_Vec2__5 this, u32 val);
std_vec_Vec2__5 std_vec_Vec2__5_to_u32(std_vec_Vec2__5 this);
std_vec_Vec2__0 std_vec_Vec2__5_to_u64(std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_mults(std_vec_Vec2__5 this, u32 val);
u32 std_vec_Vec2__5_hash(std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_adds(std_vec_Vec2__5 this, u32 val);
std_vec_Vec2__5 std_vec_Vec2__5_negate(std_vec_Vec2__5 this);
f32 std_vec_Vec2__5_length(std_vec_Vec2__5 this);
u32 std_vec_Vec2__5_dot(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
u32 std_vec_Vec2__5_cross(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
std_vec_Vec2__1 std_vec_Vec2__5_rotate(std_vec_Vec2__5 *this, f32 angle);
std_vec_Vec2__5 std_vec_Vec2__5_div(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
std_vec_Vec2__5 std_vec_Vec2__5_divrs(u32 val, std_vec_Vec2__5 this);
std_vec_Vec2__0 std_vec_Vec2__4_to_u64(std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_mults(std_vec_Vec2__4 this, i64 val);
u32 std_vec_Vec2__4_hash(std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_adds(std_vec_Vec2__4 this, i64 val);
std_vec_Vec2__4 std_vec_Vec2__4_negate(std_vec_Vec2__4 this);
f32 std_vec_Vec2__4_length(std_vec_Vec2__4 this);
i64 std_vec_Vec2__4_dot(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
i64 std_vec_Vec2__4_cross(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
std_vec_Vec2__1 std_vec_Vec2__4_rotate(std_vec_Vec2__4 *this, f32 angle);
std_vec_Vec2__4 std_vec_Vec2__4_div(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
std_vec_Vec2__4 std_vec_Vec2__4_divrs(i64 val, std_vec_Vec2__4 this);
std_vec_Vec2__3 std_vec_Vec2__3_reflect(std_vec_Vec2__3 this, std_vec_Vec2__3 normal);
std_vec_Vec2__3 std_vec_Vec2__3_divs(std_vec_Vec2__3 this, i32 val);
bool std_vec_Vec2__3_eq(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
std_vec_Vec2__3 std_vec_Vec2__3_subs(std_vec_Vec2__3 this, i32 val);
std_vec_Vec2__5 std_vec_Vec2__3_to_u32(std_vec_Vec2__3 this);
std_vec_Vec2__0 std_vec_Vec2__3_to_u64(std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_mults(std_vec_Vec2__3 this, i32 val);
u32 std_vec_Vec2__3_hash(std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_adds(std_vec_Vec2__3 this, i32 val);
std_vec_Vec2__3 std_vec_Vec2__3_negate(std_vec_Vec2__3 this);
f32 std_vec_Vec2__3_length(std_vec_Vec2__3 this);
i32 std_vec_Vec2__3_dot(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
i32 std_vec_Vec2__3_cross(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
std_vec_Vec2__1 std_vec_Vec2__3_rotate(std_vec_Vec2__3 *this, f32 angle);
std_vec_Vec2__3 std_vec_Vec2__3_div(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
std_vec_Vec2__3 std_vec_Vec2__3_divrs(i32 val, std_vec_Vec2__3 this);
std_vec_Vec2__4 std_vec_Vec2__2_to_i64(std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_reflect(std_vec_Vec2__2 this, std_vec_Vec2__2 normal);
std_vec_Vec2__2 std_vec_Vec2__2_divs(std_vec_Vec2__2 this, f64 val);
bool std_vec_Vec2__2_eq(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
std_vec_Vec2__2 std_vec_Vec2__2_subs(std_vec_Vec2__2 this, f64 val);
std_vec_Vec2__5 std_vec_Vec2__2_to_u32(std_vec_Vec2__2 this);
std_vec_Vec2__0 std_vec_Vec2__2_to_u64(std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_mults(std_vec_Vec2__2 this, f64 val);
u32 std_vec_Vec2__2_hash(std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_adds(std_vec_Vec2__2 this, f64 val);
std_vec_Vec2__2 std_vec_Vec2__2_negate(std_vec_Vec2__2 this);
f32 std_vec_Vec2__2_length(std_vec_Vec2__2 this);
f64 std_vec_Vec2__2_dot(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
f64 std_vec_Vec2__2_cross(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
std_vec_Vec2__1 std_vec_Vec2__2_rotate(std_vec_Vec2__2 *this, f32 angle);
std_vec_Vec2__2 std_vec_Vec2__2_div(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
std_vec_Vec2__2 std_vec_Vec2__2_divrs(f64 val, std_vec_Vec2__2 this);
std_vec_Vec2__1 std_vec_Vec2__1_mult(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
std_vec_Vec2__1 std_vec_Vec2__1_addrs(f32 val, std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_to_f32(std_vec_Vec2__1 this);
std_vec_Vec2__3 std_vec_Vec2__1_to_i32(std_vec_Vec2__1 this);
std_vec_Vec2__4 std_vec_Vec2__1_to_i64(std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_reflect(std_vec_Vec2__1 this, std_vec_Vec2__1 normal);
std_vec_Vec2__1 std_vec_Vec2__1_divs(std_vec_Vec2__1 this, f32 val);
bool std_vec_Vec2__1_eq(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
std_vec_Vec2__1 std_vec_Vec2__1_subs(std_vec_Vec2__1 this, f32 val);
std_vec_Vec2__5 std_vec_Vec2__1_to_u32(std_vec_Vec2__1 this);
std_vec_Vec2__0 std_vec_Vec2__1_to_u64(std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_mults(std_vec_Vec2__1 this, f32 val);
u32 std_vec_Vec2__1_hash(std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_adds(std_vec_Vec2__1 this, f32 val);
std_vec_Vec2__1 std_vec_Vec2__1_negate(std_vec_Vec2__1 this);
f32 std_vec_Vec2__1_length(std_vec_Vec2__1 this);
f32 std_vec_Vec2__1_dot(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
f32 std_vec_Vec2__1_cross(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
std_vec_Vec2__1 std_vec_Vec2__1_rotate(std_vec_Vec2__1 *this, f32 angle);
std_vec_Vec2__1 std_vec_Vec2__1_div(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
std_vec_Vec2__1 std_vec_Vec2__1_divrs(f32 val, std_vec_Vec2__1 this);
std_vec_Vec2__0 std_vec_Vec2__0_multrs(u64 val, std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_add(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
u64 std_vec_Vec2__0_length_sq(std_vec_Vec2__0 this);
std_vec_Vec2__2 std_vec_Vec2__0_to_f64(std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_mult(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
std_vec_Vec2__0 std_vec_Vec2__0_addrs(u64 val, std_vec_Vec2__0 this);
std_vec_Vec2__1 std_vec_Vec2__0_to_f32(std_vec_Vec2__0 this);
std_vec_Vec2__3 std_vec_Vec2__0_to_i32(std_vec_Vec2__0 this);
std_vec_Vec2__4 std_vec_Vec2__0_to_i64(std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_reflect(std_vec_Vec2__0 this, std_vec_Vec2__0 normal);
std_vec_Vec2__0 std_vec_Vec2__0_divs(std_vec_Vec2__0 this, u64 val);
bool std_vec_Vec2__0_eq(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
std_vec_Vec2__0 std_vec_Vec2__0_subs(std_vec_Vec2__0 this, u64 val);
std_vec_Vec2__5 std_vec_Vec2__0_to_u32(std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_to_u64(std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_mults(std_vec_Vec2__0 this, u64 val);
u32 std_vec_Vec2__0_hash(std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_adds(std_vec_Vec2__0 this, u64 val);
std_vec_Vec2__0 std_vec_Vec2__0_negate(std_vec_Vec2__0 this);
f32 std_vec_Vec2__0_length(std_vec_Vec2__0 this);
u64 std_vec_Vec2__0_dot(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
u64 std_vec_Vec2__0_cross(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
std_vec_Vec2__1 std_vec_Vec2__0_rotate(std_vec_Vec2__0 *this, f32 angle);
std_vec_Vec2__0 std_vec_Vec2__0_div(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
std_vec_Vec2__0 std_vec_Vec2__0_divrs(u64 val, std_vec_Vec2__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_subrs(f32 val, std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_sub(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
std_vec_Vec3__0 std_vec_Vec3__0_rotateX(std_vec_Vec3__0 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__0_normalized(std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_multrs(f32 val, std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_add(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
f32 std_vec_Vec3__0_length_sq(std_vec_Vec3__0 this);
std_vec_Vec3__1 std_vec_Vec3__0_to_f64(std_vec_Vec3__0 this);
std_vec_Vec3__1 std_vec_Vec3__1_subrs(f64 val, std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_sub(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
std_vec_Vec3__0 std_vec_Vec3__1_rotateX(std_vec_Vec3__1 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__1_normalized(std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_multrs(f64 val, std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_add(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
f64 std_vec_Vec3__1_length_sq(std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_to_f64(std_vec_Vec3__1 this);
std_vec_Vec3__0 std_vec_Vec3__1_rotateY(std_vec_Vec3__1 *this, f32 angle);
std_vec_Vec3__1 std_vec_Vec3__1_mult(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
std_vec_Vec3__1 std_vec_Vec3__1_addrs(f64 val, std_vec_Vec3__1 this);
std_vec_Vec3__0 std_vec_Vec3__1_to_f32(std_vec_Vec3__1 this);
std_vec_Vec3__0 std_vec_Vec3__1_rotateZ(std_vec_Vec3__1 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__1_to_i32(std_vec_Vec3__1 this);
std_vec_Vec3__2 std_vec_Vec3__2_subrs(i32 val, std_vec_Vec3__2 this);
std_vec_Vec3__2 std_vec_Vec3__2_sub(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
std_vec_Vec3__0 std_vec_Vec3__2_rotateX(std_vec_Vec3__2 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__2_normalized(std_vec_Vec3__2 this);
std_vec_Vec3__2 std_vec_Vec3__2_multrs(i32 val, std_vec_Vec3__2 this);
std_vec_Vec3__2 std_vec_Vec3__2_add(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
i32 std_vec_Vec3__2_length_sq(std_vec_Vec3__2 this);
std_vec_Vec3__1 std_vec_Vec3__2_to_f64(std_vec_Vec3__2 this);
std_vec_Vec3__0 std_vec_Vec3__2_rotateY(std_vec_Vec3__2 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__2_mult(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
std_vec_Vec3__2 std_vec_Vec3__2_addrs(i32 val, std_vec_Vec3__2 this);
std_vec_Vec3__0 std_vec_Vec3__2_to_f32(std_vec_Vec3__2 this);
std_vec_Vec3__0 std_vec_Vec3__2_rotateZ(std_vec_Vec3__2 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__2_to_i32(std_vec_Vec3__2 this);
std_vec_Vec3__3 std_vec_Vec3__2_to_i64(std_vec_Vec3__2 this);
std_vec_Vec3__3 std_vec_Vec3__3_subrs(i64 val, std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_sub(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
std_vec_Vec3__0 std_vec_Vec3__3_rotateX(std_vec_Vec3__3 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__3_normalized(std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_multrs(i64 val, std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_add(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
i64 std_vec_Vec3__3_length_sq(std_vec_Vec3__3 this);
std_vec_Vec3__1 std_vec_Vec3__3_to_f64(std_vec_Vec3__3 this);
std_vec_Vec3__0 std_vec_Vec3__3_rotateY(std_vec_Vec3__3 *this, f32 angle);
std_vec_Vec3__3 std_vec_Vec3__3_mult(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
std_vec_Vec3__3 std_vec_Vec3__3_addrs(i64 val, std_vec_Vec3__3 this);
std_vec_Vec3__0 std_vec_Vec3__3_to_f32(std_vec_Vec3__3 this);
std_vec_Vec3__0 std_vec_Vec3__3_rotateZ(std_vec_Vec3__3 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__3_to_i32(std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_to_i64(std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_reflect(std_vec_Vec3__3 this, std_vec_Vec3__3 normal);
std_vec_Vec3__3 std_vec_Vec3__3_divs(std_vec_Vec3__3 this, i64 val);
bool std_vec_Vec3__3_eq(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
std_vec_Vec3__3 std_vec_Vec3__3_subs(std_vec_Vec3__3 this, i64 val);
std_vec_Vec3__4 std_vec_Vec3__3_to_u32(std_vec_Vec3__3 this);
std_vec_Vec3__4 std_vec_Vec3__4_subrs(u32 val, std_vec_Vec3__4 this);
std_vec_Vec3__4 std_vec_Vec3__4_sub(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
std_vec_Vec3__0 std_vec_Vec3__4_rotateX(std_vec_Vec3__4 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__4_normalized(std_vec_Vec3__4 this);
std_vec_Vec3__4 std_vec_Vec3__4_multrs(u32 val, std_vec_Vec3__4 this);
std_vec_Vec3__4 std_vec_Vec3__4_add(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
u32 std_vec_Vec3__4_length_sq(std_vec_Vec3__4 this);
std_vec_Vec3__1 std_vec_Vec3__4_to_f64(std_vec_Vec3__4 this);
std_vec_Vec3__0 std_vec_Vec3__4_rotateY(std_vec_Vec3__4 *this, f32 angle);
std_vec_Vec3__4 std_vec_Vec3__4_mult(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
std_vec_Vec3__4 std_vec_Vec3__4_addrs(u32 val, std_vec_Vec3__4 this);
std_vec_Vec3__0 std_vec_Vec3__4_to_f32(std_vec_Vec3__4 this);
std_vec_Vec3__0 std_vec_Vec3__4_rotateZ(std_vec_Vec3__4 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__4_to_i32(std_vec_Vec3__4 this);
std_vec_Vec3__3 std_vec_Vec3__4_to_i64(std_vec_Vec3__4 this);
std_vec_Vec3__4 std_vec_Vec3__4_reflect(std_vec_Vec3__4 this, std_vec_Vec3__4 normal);
std_vec_Vec3__4 std_vec_Vec3__4_divs(std_vec_Vec3__4 this, u32 val);
bool std_vec_Vec3__4_eq(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
std_vec_Vec3__4 std_vec_Vec3__4_subs(std_vec_Vec3__4 this, u32 val);
std_vec_Vec3__4 std_vec_Vec3__4_to_u32(std_vec_Vec3__4 this);
std_vec_Vec3__5 std_vec_Vec3__4_to_u64(std_vec_Vec3__4 this);
std_vec_Vec3__5 std_vec_Vec3__5_subrs(u64 val, std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_sub(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
std_vec_Vec3__0 std_vec_Vec3__5_rotateX(std_vec_Vec3__5 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__5_normalized(std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_multrs(u64 val, std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_add(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
u64 std_vec_Vec3__5_length_sq(std_vec_Vec3__5 this);
std_vec_Vec3__1 std_vec_Vec3__5_to_f64(std_vec_Vec3__5 this);
std_vec_Vec3__0 std_vec_Vec3__5_rotateY(std_vec_Vec3__5 *this, f32 angle);
std_vec_Vec3__5 std_vec_Vec3__5_mult(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
std_vec_Vec3__5 std_vec_Vec3__5_addrs(u64 val, std_vec_Vec3__5 this);
std_vec_Vec3__0 std_vec_Vec3__5_to_f32(std_vec_Vec3__5 this);
std_vec_Vec3__0 std_vec_Vec3__5_rotateZ(std_vec_Vec3__5 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__5_to_i32(std_vec_Vec3__5 this);
std_vec_Vec3__3 std_vec_Vec3__5_to_i64(std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_reflect(std_vec_Vec3__5 this, std_vec_Vec3__5 normal);
std_vec_Vec3__5 std_vec_Vec3__5_divs(std_vec_Vec3__5 this, u64 val);
bool std_vec_Vec3__5_eq(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
std_vec_Vec3__5 std_vec_Vec3__5_subs(std_vec_Vec3__5 this, u64 val);
std_vec_Vec3__4 std_vec_Vec3__5_to_u32(std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_to_u64(std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_mults(std_vec_Vec3__5 this, u64 val);
u32 std_vec_Vec3__5_hash(std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_adds(std_vec_Vec3__5 this, u64 val);
std_vec_Vec3__5 std_vec_Vec3__5_negate(std_vec_Vec3__5 this);
f32 std_vec_Vec3__5_length(std_vec_Vec3__5 this);
u64 std_vec_Vec3__5_dot(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
u64 std_vec_Vec3__5_index(std_vec_Vec3__5 this, u32 idx);
std_vec_Vec3__5 std_vec_Vec3__5_cross(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
std_vec_Vec3__5 std_vec_Vec3__5_div(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
std_vec_Vec3__5 std_vec_Vec3__5_divrs(u64 val, std_vec_Vec3__5 this);
std_vec_Vec3__4 std_vec_Vec3__4_mults(std_vec_Vec3__4 this, u32 val);
u32 std_vec_Vec3__4_hash(std_vec_Vec3__4 this);
std_vec_Vec3__4 std_vec_Vec3__4_adds(std_vec_Vec3__4 this, u32 val);
std_vec_Vec3__4 std_vec_Vec3__4_negate(std_vec_Vec3__4 this);
f32 std_vec_Vec3__4_length(std_vec_Vec3__4 this);
u32 std_vec_Vec3__4_dot(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
u32 std_vec_Vec3__4_index(std_vec_Vec3__4 this, u32 idx);
std_vec_Vec3__4 std_vec_Vec3__4_cross(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
std_vec_Vec3__4 std_vec_Vec3__4_div(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
std_vec_Vec3__4 std_vec_Vec3__4_divrs(u32 val, std_vec_Vec3__4 this);
std_vec_Vec3__5 std_vec_Vec3__3_to_u64(std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_mults(std_vec_Vec3__3 this, i64 val);
u32 std_vec_Vec3__3_hash(std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_adds(std_vec_Vec3__3 this, i64 val);
std_vec_Vec3__3 std_vec_Vec3__3_negate(std_vec_Vec3__3 this);
f32 std_vec_Vec3__3_length(std_vec_Vec3__3 this);
i64 std_vec_Vec3__3_dot(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
i64 std_vec_Vec3__3_index(std_vec_Vec3__3 this, u32 idx);
std_vec_Vec3__3 std_vec_Vec3__3_cross(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
std_vec_Vec3__3 std_vec_Vec3__3_div(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
std_vec_Vec3__3 std_vec_Vec3__3_divrs(i64 val, std_vec_Vec3__3 this);
std_vec_Vec3__2 std_vec_Vec3__2_reflect(std_vec_Vec3__2 this, std_vec_Vec3__2 normal);
std_vec_Vec3__2 std_vec_Vec3__2_divs(std_vec_Vec3__2 this, i32 val);
bool std_vec_Vec3__2_eq(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
std_vec_Vec3__2 std_vec_Vec3__2_subs(std_vec_Vec3__2 this, i32 val);
std_vec_Vec3__4 std_vec_Vec3__2_to_u32(std_vec_Vec3__2 this);
std_vec_Vec3__5 std_vec_Vec3__2_to_u64(std_vec_Vec3__2 this);
std_vec_Vec3__2 std_vec_Vec3__2_mults(std_vec_Vec3__2 this, i32 val);
u32 std_vec_Vec3__2_hash(std_vec_Vec3__2 this);
std_vec_Vec3__2 std_vec_Vec3__2_adds(std_vec_Vec3__2 this, i32 val);
std_vec_Vec3__2 std_vec_Vec3__2_negate(std_vec_Vec3__2 this);
f32 std_vec_Vec3__2_length(std_vec_Vec3__2 this);
i32 std_vec_Vec3__2_dot(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
i32 std_vec_Vec3__2_index(std_vec_Vec3__2 this, u32 idx);
std_vec_Vec3__2 std_vec_Vec3__2_cross(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
std_vec_Vec3__2 std_vec_Vec3__2_div(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
std_vec_Vec3__2 std_vec_Vec3__2_divrs(i32 val, std_vec_Vec3__2 this);
std_vec_Vec3__3 std_vec_Vec3__1_to_i64(std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_reflect(std_vec_Vec3__1 this, std_vec_Vec3__1 normal);
std_vec_Vec3__1 std_vec_Vec3__1_divs(std_vec_Vec3__1 this, f64 val);
bool std_vec_Vec3__1_eq(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
std_vec_Vec3__1 std_vec_Vec3__1_subs(std_vec_Vec3__1 this, f64 val);
std_vec_Vec3__4 std_vec_Vec3__1_to_u32(std_vec_Vec3__1 this);
std_vec_Vec3__5 std_vec_Vec3__1_to_u64(std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_mults(std_vec_Vec3__1 this, f64 val);
u32 std_vec_Vec3__1_hash(std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_adds(std_vec_Vec3__1 this, f64 val);
std_vec_Vec3__1 std_vec_Vec3__1_negate(std_vec_Vec3__1 this);
f32 std_vec_Vec3__1_length(std_vec_Vec3__1 this);
f64 std_vec_Vec3__1_dot(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
f64 std_vec_Vec3__1_index(std_vec_Vec3__1 this, u32 idx);
std_vec_Vec3__1 std_vec_Vec3__1_cross(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
std_vec_Vec3__1 std_vec_Vec3__1_div(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
std_vec_Vec3__1 std_vec_Vec3__1_divrs(f64 val, std_vec_Vec3__1 this);
std_vec_Vec3__0 std_vec_Vec3__0_rotateY(std_vec_Vec3__0 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__0_mult(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
std_vec_Vec3__0 std_vec_Vec3__0_addrs(f32 val, std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_to_f32(std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_rotateZ(std_vec_Vec3__0 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__0_to_i32(std_vec_Vec3__0 this);
std_vec_Vec3__3 std_vec_Vec3__0_to_i64(std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_reflect(std_vec_Vec3__0 this, std_vec_Vec3__0 normal);
std_vec_Vec3__0 std_vec_Vec3__0_divs(std_vec_Vec3__0 this, f32 val);
bool std_vec_Vec3__0_eq(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
std_vec_Vec3__0 std_vec_Vec3__0_subs(std_vec_Vec3__0 this, f32 val);
std_vec_Vec3__4 std_vec_Vec3__0_to_u32(std_vec_Vec3__0 this);
std_vec_Vec3__5 std_vec_Vec3__0_to_u64(std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_mults(std_vec_Vec3__0 this, f32 val);
u32 std_vec_Vec3__0_hash(std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_adds(std_vec_Vec3__0 this, f32 val);
std_vec_Vec3__0 std_vec_Vec3__0_negate(std_vec_Vec3__0 this);
f32 std_vec_Vec3__0_length(std_vec_Vec3__0 this);
f32 std_vec_Vec3__0_dot(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
f32 std_vec_Vec3__0_index(std_vec_Vec3__0 this, u32 idx);
std_vec_Vec3__0 std_vec_Vec3__0_cross(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
std_vec_Vec3__0 std_vec_Vec3__0_div(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
std_vec_Vec3__0 std_vec_Vec3__0_divrs(f32 val, std_vec_Vec3__0 this);
std_og_Rect std_og_Rect_from_v(std_vec_Vec2__3 pos, std_vec_Vec2__3 size);
bool std_og_Rect_contains(std_og_Rect this, std_vec_Vec2__3 pos);
u32 std_og_get_ticks(void);
void std_og_init(u32 width, u32 height, char *title, bool fullscreen);
void std_og_toggle_fullscreen(void);
bool std_og_is_running(void);
void std_og_clear(SDL_Color col);
void std_og_draw_rect(i32 x, i32 y, i32 w, i32 h, SDL_Color col);
void std_og_draw_rect_v(std_vec_Vec2__3 pos, std_vec_Vec2__3 size, SDL_Color col);
void std_og_draw_line(i32 x1, i32 y1, i32 x2, i32 y2, SDL_Color col);
void std_og_draw_line_v(std_vec_Vec2__3 p1, std_vec_Vec2__3 p2, SDL_Color col);
void std_og_draw_pixel(i32 x, i32 y, SDL_Color col);
void std_og_draw_pixel_v(std_vec_Vec2__3 pos, SDL_Color col);
f32 std_og_get_frame_time(void);
void std_og_quit(void);
bool std_og_is_key_pressed(SDL_Scancode k);
bool std_og_is_key_released(SDL_Scancode k);
bool std_og_is_key_down(SDL_Scancode k);
bool std_og_is_key_up(SDL_Scancode k);
bool std_og_is_mouse_pressed(u8 b);
bool std_og_is_mouse_released(u8 b);
bool std_og_is_mouse_down(u8 b);
bool std_og_is_mouse_up(u8 b);
std_vec_Vec2__3 std_og_get_window_size(void);
bool std_og_is_scroll(std_og_state_MouseWheel w);
std_vec_Vec2__3 std_og_get_mouse_pos(void);
std_vec_Vec2__3 std_og_get_mouse_delta(void);
void std_og_capture_mouse(bool capture);
void std_og_grab_input(bool grab);
void std_og_show_cursor(bool show);
void std_og_set_cursor(SDL_SystemCursor c);
void std_og_Texture_draw_pixel(std_og_Texture this, i32 x, i32 y, SDL_Color col);
void std_og_Texture_draw_rect(std_og_Texture this, i32 x, i32 y, i32 w, i32 h, SDL_Color col);
void std_og_Texture_draw_rect_v(std_og_Texture this, std_vec_Vec2__3 pos, std_vec_Vec2__3 size, SDL_Color col);
void std_og_Texture_draw_pixel_v(std_og_Texture this, std_vec_Vec2__3 pos, SDL_Color col);
std_og_Texture *std_og_get_buffer(void);
void std_og_draw_buffer(void);
void std_og_set_main_loop(bool (*callback)(void));
bool std_og_state_KeyState_get(std_og_state_KeyState *this, SDL_Scancode k);
bool std_og_state_MouseState_get_button(std_og_state_MouseState *this, u8 b);
bool std_og_state_MouseState_get_wheel(std_og_state_MouseState *this, std_og_state_MouseWheel w);
void std_og_state_update_window_size(void);
void std_og_state_add_frametime(f32 frametime);
f32 std_og_state_get_avg_frametime(void);
void std_og_utils_handle_sdl_events(void);
bool std_og_internal_og_main_fn(void);
void *std_mem_state_allocator = NULL;
void *(*std_mem_state_alloc_fn)(void *, u32) = std_mem_impl_my_calloc;
void *(*std_mem_state_realloc_fn)(void *, void *, u32, u32) = std_mem_impl_my_realloc;
void (*std_mem_state_free_fn)(void *, void *) = std_mem_impl_my_free;
SDL_Window *std_og_state_window = NULL;
SDL_Renderer *std_og_state_renderer = NULL;
u32 std_og_state_prev_time = 0;
f32 std_og_state_frame_time = 0.0f;
bool std_og_state_running = true;
std_vec_Vec2__3 std_og_state_window_size = {0};
char *std_og_state_title = "Window";
SDL_SystemCursor std_og_state_cursor_type = SDL_SYSTEM_CURSOR_ARROW;
SDL_Cursor *std_og_state_cursor = NULL;
std_og_state_KeyState std_og_state_prev_keys = {0};
std_og_state_KeyState std_og_state_keys = {0};
std_og_state_MouseState std_og_state_prev_mouse = {0};
std_og_state_MouseState std_og_state_mouse = {0};
f32 std_og_state_frametimes[std_og_state_FRAMETIMES_COUNT] = {0};
u32 std_og_state_frametime_index;
f32 std_og_state_frametime_total = 0.0f;
char std_og_state_window_title_fps_buf[256] = {0};
u64 std_og_state_frame_num = ((u64)0);
std_og_Texture std_og_state_buffer = (std_og_Texture){.tx=NULL, .w=0, .h=0, .pixels=NULL, .pitch=0};
bool (*std_og_internal_user_main_fn)(void) = NULL;
SDL_Color std_og_colors_RED = (SDL_Color){.r=255, .g=0, .b=0, .a=255};
SDL_Color std_og_colors_GREEN = (SDL_Color){.r=0, .g=255, .b=0, .a=255};
SDL_Color std_og_colors_BLUE = (SDL_Color){.r=0, .g=0, .b=255, .a=255};
SDL_Color std_og_colors_BLACK = (SDL_Color){.r=0, .g=0, .b=0, .a=255};
SDL_Color std_og_colors_WHITE = (SDL_Color){.r=255, .g=255, .b=255, .a=255};
SDL_Color std_og_colors_YELLOW = (SDL_Color){.r=255, .g=255, .b=0, .a=255};
SDL_Color std_og_colors_CYAN = (SDL_Color){.r=0, .g=255, .b=255, .a=255};
/* function implementations */
i32 main(void) {
  i32 rows = 20;
  i32 cols = 20;
  i32 cell_size = 20;
  std_og_init(((u32)(cols * cell_size)), ((u32)(rows * cell_size)), "Demo", false);
  i32 off = 0;
  u32 frames = 0;
  while (std_og_is_running()) {
    if (std_og_is_key_pressed(SDL_SCANCODE_ESCAPE)) {
      break;
    }
    if (std_og_is_key_pressed(SDL_SCANCODE_UP)) {
      cell_size+=1;
    }
    if (std_og_is_key_pressed(SDL_SCANCODE_DOWN)) {
      cell_size-=1;
    }
    for (i32 i = -1; i < (rows + 1); i++) {
      for (i32 j = -1; j < (cols + 1); j++) {
        std_og_draw_rect(((i32)((j * cell_size) + off)), ((i32)((i * cell_size) + off)), ((i32)cell_size), ((i32)cell_size), (((i + j) % 2)==0 ? std_og_colors_RED : std_og_colors_WHITE));
      }
    }
    frames++;
    if ((frames % 20)==0) {
      if (off==cell_size) {
        off=0;
      }
      off++;
    }
  }
  std_og_quit();
}

void og_wasm_template_main_main(void) {
  i32 rows = 20;
  i32 cols = 20;
  i32 cell_size = 20;
  std_og_init(((u32)(cols * cell_size)), ((u32)(rows * cell_size)), "Demo", false);
  i32 off = 0;
  u32 frames = 0;
  while (std_og_is_running()) {
    if (std_og_is_key_pressed(SDL_SCANCODE_ESCAPE)) {
      break;
    }
    if (std_og_is_key_pressed(SDL_SCANCODE_UP)) {
      cell_size+=1;
    }
    if (std_og_is_key_pressed(SDL_SCANCODE_DOWN)) {
      cell_size-=1;
    }
    for (i32 i = -1; i < (rows + 1); i++) {
      for (i32 j = -1; j < (cols + 1); j++) {
        std_og_draw_rect(((i32)((j * cell_size) + off)), ((i32)((i * cell_size) + off)), ((i32)cell_size), ((i32)cell_size), (((i + j) % 2)==0 ? std_og_colors_BLACK : std_og_colors_WHITE));
      }
    }
    frames++;
    if ((frames % 20)==0) {
      if (off==cell_size) {
        off=0;
      }
      off++;
    }
  }
  std_og_quit();
}

void std_panic(char *msg) {
  printf("%s""\n", msg);
  dump_backtrace();
  oc_trap();
  exit(1);
}

u32 str_to_u32(char *this) {
  return ((u32)atoi(this));
}

bool str_eq(char *this, char *other) {
  void *a = ((void *)this);
  void *b = ((void *)other);
  if (a==b) {
    return true;
  }
  if (!(((bool)a) && ((bool)b))) {
    return false;
  }
  return strcmp(this, other)==0;
}

char *std_format(char *fmt, ...) {
  va_list args = {0};
  va_start(args, fmt);
  u32 size = vsnprintf(NULL, 0, fmt, args);
  va_end(args);
  va_start(args, fmt);
  char *s = std_mem_alloc__0(((u32)(size + 1)));
  vsnprintf(s, (size + 1), fmt, args);
  s[size]='\0';
  va_end(args);
  return s;
}

char *str_substring(char *this, u32 start, u32 len) {
  char *new_str = std_mem_alloc__0(((u32)(len + 1)));
  memcpy(new_str, &this[start], len);
  return new_str;
}

bool str_ends_with(char *this, char *suffix) {
  u32 suffix_len = strlen(suffix);
  u32 len = strlen(this);
  if (suffix_len > len) {
    return false;
  }
  for (u32 i = 0; i < suffix_len; i+=1) {
    if (this[((len - suffix_len) + i)] != suffix[i]) {
      return false;
    }
  }
  return true;
}

bool str_starts_with(char *this, char *prefix) {
  u32 prefix_len = strlen(prefix);
  u32 len = strlen(this);
  if (prefix_len > len) {
    return false;
  }
  for (u32 i = 0; i < prefix_len; i+=1) {
    if (this[i] != prefix[i]) {
      return false;
    }
  }
  return true;
}

void str_strip_trailing_whitespace(char *this) {
  for (u32 i = (strlen(this) - 1); i >= 0; i-=1) {
    if (this[i] != ' ') {
      break;
    }
    this[i]='\0';
  }
}

void str_remove_last_n(char *this, u32 n) {
  this[(strlen(this) - n)]='\0';
}

void str_replace_with(char **this, char *other) {
  std_mem_free((*this));
  (*this)=other;
}

void str_free(char **this) {
  std_mem_free((*this));
  (*this)=NULL;
}

std_CharIterator str_chars(char *this, u32 start) {
  return (std_CharIterator){.data=this, .len=strlen(this), .pos=start};
}

bool std_CharIterator_has_value(std_CharIterator *this) {
  return this->pos < this->len;
}

u32 std_CharIterator_next(std_CharIterator *this) {
  return this->pos++;
}

char std_CharIterator_cur(std_CharIterator *this) {
  return this->data[this->pos];
}

bool char_is_digit(char this) {
  return isdigit(this);
}

bool char_is_hex_digit(char this) {
  return isxdigit(this);
}

u32 char_get_digit(char this) {
  return ((u32)this) - ((u32)'0');
}

u8 char_get_hex_digit(char this) {
  if (('0' <= this) && (this <= '9')) {
    return ((u8)this) - ((u8)'0');
  }
  if (('a' <= this) && (this <= 'f')) {
    return (((u8)this) - ((u8)'a')) + ((u8)10);
  }
  if (('A' <= this) && (this <= 'F')) {
    return (((u8)this) - ((u8)'A')) + ((u8)10);
  }
  std_panic(std_format("[-] Invalid hex digit: '%c' (0x%02x)", this, this));
}

bool char_is_alpha(char this) {
  return isalpha(this);
}

bool char_is_alnum(char this) {
  return isalnum(this);
}

bool char_is_print(char this) {
  return isprint(this);
}

bool char_is_space(char this) {
  return isspace(this);
}

bool char_is_upper(char this) {
  return isupper(this);
}

bool char_is_lower(char this) {
  return islower(this);
}

i32 i32_min(i32 this, i32 b) {
  return ((this < b) ? this : b);
}

i32 i32_max(i32 this, i32 b) {
  return ((this > b) ? this : b);
}

i8 i8_min(i8 this, i8 b) {
  return ((this < b) ? this : b);
}

i8 i8_max(i8 this, i8 b) {
  return ((this > b) ? this : b);
}

u64 u64_min(u64 this, u64 other) {
  return ((this < other) ? this : other);
}

u64 u64_max(u64 this, u64 other) {
  return ((this > other) ? this : other);
}

u32 u32_min(u32 this, u32 other) {
  return ((this < other) ? this : other);
}

u32 u32_max(u32 this, u32 other) {
  return ((this > other) ? this : other);
}

u8 u8_min(u8 this, u8 other) {
  return ((this < other) ? this : other);
}

u8 u8_max(u8 this, u8 other) {
  return ((this > other) ? this : other);
}

char *std_shift_args(i32 *argc, char ***argv, char *where) {
  if ((*argc)==0) {
    printf("Expected argument but got none\n");
    exit(1);
  }
  char *arg = (*argv)[0];
  (*argv)=((*argv) + 1);
  (*argc)=((*argc) - 1);
  return arg;
}

void std_mem_set_allocator(void *allocator, void *(*alloc_fn)(void *, u32), void (*free_fn)(void *, void *), void *(*realloc_fn)(void *, void *, u32, u32)) {
  std_mem_state_allocator=allocator;
  std_mem_state_alloc_fn=alloc_fn;
  std_mem_state_realloc_fn=realloc_fn;
  std_mem_state_free_fn=free_fn;
}

void std_mem_reset_to_default_allocator(void) {
  std_mem_set_allocator(NULL, std_mem_impl_my_calloc, std_mem_impl_my_free, std_mem_impl_my_realloc);
}

char *std_mem_alloc__0(u32 count) {
  return ((char *)std_mem_state_alloc_fn(std_mem_state_allocator, (count * ((u32)sizeof(char)))));
}

void std_mem_free(void *ptr) {
  if (std_mem_state_free_fn != NULL) {
    std_mem_state_free_fn(std_mem_state_allocator, ptr);
    return;
  }
}

void *std_mem_impl_my_calloc(void *state, u32 size) {
  return calloc(size, 1);
}

void *std_mem_impl_my_realloc(void *state, void *ptr, u32 old_size, u32 size) {
  return realloc(ptr, size);
}

void std_mem_impl_my_free(void *state, void *ptr) {
  free(ptr);
}

char *std_libc_get_err(void) {
  return ((char *)strerror(errno));
}

u32 std_traits_hash_hash_bytes(u8 *data, u32 len) {
  u32 hash = 5381;
  for (u32 i = 0; i < len; i+=1) {
    hash=((hash * 33) ^ ((u32)data[i]));
  }
  return hash;
}

u32 std_traits_hash_pair_hash(u32 a, u32 b) {
  return (a * 33) ^ b;
}

u32 std_traits_hash_ptr_hash(void *ptr) {
  return u64_hash(((u64)ptr));
}

u32 str_hash(char *this) {
  return std_traits_hash_hash_bytes(((u8 *)this), strlen(this));
}

u32 u8_hash(u8 this) {
  return ((u32)this) * 7817;
}

u32 i8_hash(i8 this) {
  return ((u32)this) * 7817;
}

u32 u16_hash(u16 this) {
  return ((u32)this) * 7817;
}

u32 i16_hash(i16 this) {
  return ((u32)this) * 7817;
}

u32 u32_hash(u32 this) {
  return ((u32)this) * 7817;
}

u32 i32_hash(i32 this) {
  return ((u32)this) * 7817;
}

u32 u64_hash(u64 this) {
  return std_traits_hash_pair_hash(u32_hash(((u32)this)), u32_hash(((u32)(this >> ((u64)32)))));
}

u32 i64_hash(i64 this) {
  return std_traits_hash_pair_hash(u32_hash(((u32)this)), u32_hash(((u32)(this >> ((i64)32)))));
}

u32 char_hash(char this) {
  return ((u32)this) * 7817;
}

u32 f32_hash(f32 this) {
  return ((u32)this) * 7817;
}

u32 f64_hash(f64 this) {
  return u64_hash(((u64)this));
}

f32 f32_inf(void) {
  return ((f32)INFINITY);
}

f64 f64_inf(void) {
  return INFINITY;
}

f32 f32_min(f32 this, f32 b) {
  return ((this < b) ? this : b);
}

f32 f32_max(f32 this, f32 b) {
  return ((this > b) ? this : b);
}

f64 f64_min(f64 this, f64 b) {
  return ((this < b) ? this : b);
}

f64 f64_max(f64 this, f64 b) {
  return ((this > b) ? this : b);
}

f32 f32_clamp(f32 this, f32 min, f32 max) {
  return f32_max(f32_min(this, max), min);
}

f32 f32_clamp01(f32 this) {
  return f32_clamp(this, 0.0f, 1.0f);
}

f64 f64_clamp(f64 this, f64 min, f64 max) {
  return f64_max(f64_min(this, max), min);
}

f64 f64_clamp01(f64 this) {
  return f64_clamp(this, 0.0, 1.0);
}

bool f32_is_close(f32 this, f32 other, f32 thresh) {
  return fabsf((this - other)) < thresh;
}

bool f64_is_close(f64 this, f64 other, f64 thresh) {
  return fabs((this - other)) < thresh;
}

i8 i8_abs(i8 this) {
  return ((this < ((i8)0)) ? -this : this);
}

i16 i16_abs(i16 this) {
  return ((this < ((i16)0)) ? -this : this);
}

i32 i32_abs(i32 this) {
  return ((this < 0) ? -this : this);
}

i64 i64_abs(i64 this) {
  return ((this < ((i64)0)) ? -this : this);
}

f32 std_math_degrees(f32 radians) {
  return (radians * 180.0f) / M_PI;
}

f32 std_math_radians(f32 degrees) {
  return (degrees * M_PI) / 180.0f;
}

std_vec_Vec2__0 std_vec_Vec2__0_subrs(u64 val, std_vec_Vec2__0 this) {
  return (std_vec_Vec2__0){.x=(val - this.x), .y=(val - this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_sub(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return (std_vec_Vec2__0){.x=(this.x - other.x), .y=(this.y - other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__0_normalized(std_vec_Vec2__0 this) {
  f32 len = std_vec_Vec2__0_length(this);
  return (std_vec_Vec2__1){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len)};
}

std_vec_Vec2__1 std_vec_Vec2__1_subrs(f32 val, std_vec_Vec2__1 this) {
  return (std_vec_Vec2__1){.x=(val - this.x), .y=(val - this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_sub(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return (std_vec_Vec2__1){.x=(this.x - other.x), .y=(this.y - other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_normalized(std_vec_Vec2__1 this) {
  f32 len = std_vec_Vec2__1_length(this);
  return (std_vec_Vec2__1){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len)};
}

std_vec_Vec2__1 std_vec_Vec2__1_multrs(f32 val, std_vec_Vec2__1 this) {
  return (std_vec_Vec2__1){.x=(val * this.x), .y=(val * this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_add(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return (std_vec_Vec2__1){.x=(this.x + other.x), .y=(this.y + other.y)};
}

f32 std_vec_Vec2__1_length_sq(std_vec_Vec2__1 this) {
  return (this.x * this.x) + (this.y * this.y);
}

std_vec_Vec2__2 std_vec_Vec2__1_to_f64(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__2){.x=((f64)this.x), .y=((f64)this.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_subrs(f64 val, std_vec_Vec2__2 this) {
  return (std_vec_Vec2__2){.x=(val - this.x), .y=(val - this.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_sub(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return (std_vec_Vec2__2){.x=(this.x - other.x), .y=(this.y - other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__2_normalized(std_vec_Vec2__2 this) {
  f32 len = std_vec_Vec2__2_length(this);
  return (std_vec_Vec2__1){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len)};
}

std_vec_Vec2__2 std_vec_Vec2__2_multrs(f64 val, std_vec_Vec2__2 this) {
  return (std_vec_Vec2__2){.x=(val * this.x), .y=(val * this.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_add(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return (std_vec_Vec2__2){.x=(this.x + other.x), .y=(this.y + other.y)};
}

f64 std_vec_Vec2__2_length_sq(std_vec_Vec2__2 this) {
  return (this.x * this.x) + (this.y * this.y);
}

std_vec_Vec2__2 std_vec_Vec2__2_to_f64(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__2){.x=((f64)this.x), .y=((f64)this.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_mult(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return (std_vec_Vec2__2){.x=(this.x * other.x), .y=(this.y * other.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_addrs(f64 val, std_vec_Vec2__2 this) {
  return (std_vec_Vec2__2){.x=(val + this.x), .y=(val + this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__2_to_f32(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__1){.x=((f32)this.x), .y=((f32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__2_to_i32(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__3){.x=((i32)this.x), .y=((i32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_subrs(i32 val, std_vec_Vec2__3 this) {
  return (std_vec_Vec2__3){.x=(val - this.x), .y=(val - this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_sub(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return (std_vec_Vec2__3){.x=(this.x - other.x), .y=(this.y - other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__3_normalized(std_vec_Vec2__3 this) {
  f32 len = std_vec_Vec2__3_length(this);
  return (std_vec_Vec2__1){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len)};
}

std_vec_Vec2__3 std_vec_Vec2__3_multrs(i32 val, std_vec_Vec2__3 this) {
  return (std_vec_Vec2__3){.x=(val * this.x), .y=(val * this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_add(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return (std_vec_Vec2__3){.x=(this.x + other.x), .y=(this.y + other.y)};
}

i32 std_vec_Vec2__3_length_sq(std_vec_Vec2__3 this) {
  return (this.x * this.x) + (this.y * this.y);
}

std_vec_Vec2__2 std_vec_Vec2__3_to_f64(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__2){.x=((f64)this.x), .y=((f64)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_mult(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return (std_vec_Vec2__3){.x=(this.x * other.x), .y=(this.y * other.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_addrs(i32 val, std_vec_Vec2__3 this) {
  return (std_vec_Vec2__3){.x=(val + this.x), .y=(val + this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__3_to_f32(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__1){.x=((f32)this.x), .y=((f32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_to_i32(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__3){.x=((i32)this.x), .y=((i32)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__3_to_i64(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__4){.x=((i64)this.x), .y=((i64)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_subrs(i64 val, std_vec_Vec2__4 this) {
  return (std_vec_Vec2__4){.x=(val - this.x), .y=(val - this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_sub(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return (std_vec_Vec2__4){.x=(this.x - other.x), .y=(this.y - other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__4_normalized(std_vec_Vec2__4 this) {
  f32 len = std_vec_Vec2__4_length(this);
  return (std_vec_Vec2__1){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len)};
}

std_vec_Vec2__4 std_vec_Vec2__4_multrs(i64 val, std_vec_Vec2__4 this) {
  return (std_vec_Vec2__4){.x=(val * this.x), .y=(val * this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_add(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return (std_vec_Vec2__4){.x=(this.x + other.x), .y=(this.y + other.y)};
}

i64 std_vec_Vec2__4_length_sq(std_vec_Vec2__4 this) {
  return (this.x * this.x) + (this.y * this.y);
}

std_vec_Vec2__2 std_vec_Vec2__4_to_f64(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__2){.x=((f64)this.x), .y=((f64)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_mult(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return (std_vec_Vec2__4){.x=(this.x * other.x), .y=(this.y * other.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_addrs(i64 val, std_vec_Vec2__4 this) {
  return (std_vec_Vec2__4){.x=(val + this.x), .y=(val + this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__4_to_f32(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__1){.x=((f32)this.x), .y=((f32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__4_to_i32(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__3){.x=((i32)this.x), .y=((i32)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_to_i64(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__4){.x=((i64)this.x), .y=((i64)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_reflect(std_vec_Vec2__4 this, std_vec_Vec2__4 normal) {
  return std_vec_Vec2__4_sub(this, std_vec_Vec2__4_mults(normal, (((i64)2) * std_vec_Vec2__4_dot(this, normal))));
}

std_vec_Vec2__4 std_vec_Vec2__4_divs(std_vec_Vec2__4 this, i64 val) {
  return (std_vec_Vec2__4){.x=(this.x / val), .y=(this.y / val)};
}

bool std_vec_Vec2__4_eq(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return this.x==other.x && this.y==other.y;
}

std_vec_Vec2__4 std_vec_Vec2__4_subs(std_vec_Vec2__4 this, i64 val) {
  return (std_vec_Vec2__4){.x=(this.x - val), .y=(this.y - val)};
}

std_vec_Vec2__5 std_vec_Vec2__4_to_u32(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__5){.x=((u32)this.x), .y=((u32)this.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_subrs(u32 val, std_vec_Vec2__5 this) {
  return (std_vec_Vec2__5){.x=(val - this.x), .y=(val - this.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_sub(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return (std_vec_Vec2__5){.x=(this.x - other.x), .y=(this.y - other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__5_normalized(std_vec_Vec2__5 this) {
  f32 len = std_vec_Vec2__5_length(this);
  return (std_vec_Vec2__1){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len)};
}

std_vec_Vec2__5 std_vec_Vec2__5_multrs(u32 val, std_vec_Vec2__5 this) {
  return (std_vec_Vec2__5){.x=(val * this.x), .y=(val * this.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_add(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return (std_vec_Vec2__5){.x=(this.x + other.x), .y=(this.y + other.y)};
}

u32 std_vec_Vec2__5_length_sq(std_vec_Vec2__5 this) {
  return (this.x * this.x) + (this.y * this.y);
}

std_vec_Vec2__2 std_vec_Vec2__5_to_f64(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__2){.x=((f64)this.x), .y=((f64)this.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_mult(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return (std_vec_Vec2__5){.x=(this.x * other.x), .y=(this.y * other.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_addrs(u32 val, std_vec_Vec2__5 this) {
  return (std_vec_Vec2__5){.x=(val + this.x), .y=(val + this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__5_to_f32(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__1){.x=((f32)this.x), .y=((f32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__5_to_i32(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__3){.x=((i32)this.x), .y=((i32)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__5_to_i64(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__4){.x=((i64)this.x), .y=((i64)this.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_reflect(std_vec_Vec2__5 this, std_vec_Vec2__5 normal) {
  return std_vec_Vec2__5_sub(this, std_vec_Vec2__5_mults(normal, (((u32)2) * std_vec_Vec2__5_dot(this, normal))));
}

std_vec_Vec2__5 std_vec_Vec2__5_divs(std_vec_Vec2__5 this, u32 val) {
  return (std_vec_Vec2__5){.x=(this.x / val), .y=(this.y / val)};
}

bool std_vec_Vec2__5_eq(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return this.x==other.x && this.y==other.y;
}

std_vec_Vec2__5 std_vec_Vec2__5_subs(std_vec_Vec2__5 this, u32 val) {
  return (std_vec_Vec2__5){.x=(this.x - val), .y=(this.y - val)};
}

std_vec_Vec2__5 std_vec_Vec2__5_to_u32(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__5){.x=((u32)this.x), .y=((u32)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__5_to_u64(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__0){.x=((u64)this.x), .y=((u64)this.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_mults(std_vec_Vec2__5 this, u32 val) {
  return (std_vec_Vec2__5){.x=(this.x * val), .y=(this.y * val)};
}

u32 std_vec_Vec2__5_hash(std_vec_Vec2__5 this) {
  return std_traits_hash_pair_hash(u32_hash(this.x), u32_hash(this.y));
}

std_vec_Vec2__5 std_vec_Vec2__5_adds(std_vec_Vec2__5 this, u32 val) {
  return (std_vec_Vec2__5){.x=(this.x + val), .y=(this.y + val)};
}

std_vec_Vec2__5 std_vec_Vec2__5_negate(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__5){.x=-this.x, .y=-this.y};
}

f32 std_vec_Vec2__5_length(std_vec_Vec2__5 this) {
  return sqrtf(((f32)((this.x * this.x) + (this.y * this.y))));
}

u32 std_vec_Vec2__5_dot(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return (this.x * other.x) + (this.y * other.y);
}

u32 std_vec_Vec2__5_cross(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return (this.x * other.y) - (this.y * other.x);
}

std_vec_Vec2__1 std_vec_Vec2__5_rotate(std_vec_Vec2__5 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  return (std_vec_Vec2__1){.x=((((f32)this->x) * c) - (((f32)this->y) * s)), .y=((((f32)this->x) * s) + (((f32)this->y) * c))};
}

std_vec_Vec2__5 std_vec_Vec2__5_div(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return (std_vec_Vec2__5){.x=(this.x / other.x), .y=(this.y / other.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_divrs(u32 val, std_vec_Vec2__5 this) {
  return (std_vec_Vec2__5){.x=(val / this.x), .y=(val / this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__4_to_u64(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__0){.x=((u64)this.x), .y=((u64)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_mults(std_vec_Vec2__4 this, i64 val) {
  return (std_vec_Vec2__4){.x=(this.x * val), .y=(this.y * val)};
}

u32 std_vec_Vec2__4_hash(std_vec_Vec2__4 this) {
  return std_traits_hash_pair_hash(i64_hash(this.x), i64_hash(this.y));
}

std_vec_Vec2__4 std_vec_Vec2__4_adds(std_vec_Vec2__4 this, i64 val) {
  return (std_vec_Vec2__4){.x=(this.x + val), .y=(this.y + val)};
}

std_vec_Vec2__4 std_vec_Vec2__4_negate(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__4){.x=-this.x, .y=-this.y};
}

f32 std_vec_Vec2__4_length(std_vec_Vec2__4 this) {
  return sqrtf(((f32)((this.x * this.x) + (this.y * this.y))));
}

i64 std_vec_Vec2__4_dot(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return (this.x * other.x) + (this.y * other.y);
}

i64 std_vec_Vec2__4_cross(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return (this.x * other.y) - (this.y * other.x);
}

std_vec_Vec2__1 std_vec_Vec2__4_rotate(std_vec_Vec2__4 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  return (std_vec_Vec2__1){.x=((((f32)this->x) * c) - (((f32)this->y) * s)), .y=((((f32)this->x) * s) + (((f32)this->y) * c))};
}

std_vec_Vec2__4 std_vec_Vec2__4_div(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return (std_vec_Vec2__4){.x=(this.x / other.x), .y=(this.y / other.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_divrs(i64 val, std_vec_Vec2__4 this) {
  return (std_vec_Vec2__4){.x=(val / this.x), .y=(val / this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_reflect(std_vec_Vec2__3 this, std_vec_Vec2__3 normal) {
  return std_vec_Vec2__3_sub(this, std_vec_Vec2__3_mults(normal, (((i32)2) * std_vec_Vec2__3_dot(this, normal))));
}

std_vec_Vec2__3 std_vec_Vec2__3_divs(std_vec_Vec2__3 this, i32 val) {
  return (std_vec_Vec2__3){.x=(this.x / val), .y=(this.y / val)};
}

bool std_vec_Vec2__3_eq(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return this.x==other.x && this.y==other.y;
}

std_vec_Vec2__3 std_vec_Vec2__3_subs(std_vec_Vec2__3 this, i32 val) {
  return (std_vec_Vec2__3){.x=(this.x - val), .y=(this.y - val)};
}

std_vec_Vec2__5 std_vec_Vec2__3_to_u32(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__5){.x=((u32)this.x), .y=((u32)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__3_to_u64(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__0){.x=((u64)this.x), .y=((u64)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_mults(std_vec_Vec2__3 this, i32 val) {
  return (std_vec_Vec2__3){.x=(this.x * val), .y=(this.y * val)};
}

u32 std_vec_Vec2__3_hash(std_vec_Vec2__3 this) {
  return std_traits_hash_pair_hash(i32_hash(this.x), i32_hash(this.y));
}

std_vec_Vec2__3 std_vec_Vec2__3_adds(std_vec_Vec2__3 this, i32 val) {
  return (std_vec_Vec2__3){.x=(this.x + val), .y=(this.y + val)};
}

std_vec_Vec2__3 std_vec_Vec2__3_negate(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__3){.x=-this.x, .y=-this.y};
}

f32 std_vec_Vec2__3_length(std_vec_Vec2__3 this) {
  return sqrtf(((f32)((this.x * this.x) + (this.y * this.y))));
}

i32 std_vec_Vec2__3_dot(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return (this.x * other.x) + (this.y * other.y);
}

i32 std_vec_Vec2__3_cross(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return (this.x * other.y) - (this.y * other.x);
}

std_vec_Vec2__1 std_vec_Vec2__3_rotate(std_vec_Vec2__3 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  return (std_vec_Vec2__1){.x=((((f32)this->x) * c) - (((f32)this->y) * s)), .y=((((f32)this->x) * s) + (((f32)this->y) * c))};
}

std_vec_Vec2__3 std_vec_Vec2__3_div(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return (std_vec_Vec2__3){.x=(this.x / other.x), .y=(this.y / other.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_divrs(i32 val, std_vec_Vec2__3 this) {
  return (std_vec_Vec2__3){.x=(val / this.x), .y=(val / this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__2_to_i64(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__4){.x=((i64)this.x), .y=((i64)this.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_reflect(std_vec_Vec2__2 this, std_vec_Vec2__2 normal) {
  return std_vec_Vec2__2_sub(this, std_vec_Vec2__2_mults(normal, (((f64)2) * std_vec_Vec2__2_dot(this, normal))));
}

std_vec_Vec2__2 std_vec_Vec2__2_divs(std_vec_Vec2__2 this, f64 val) {
  return (std_vec_Vec2__2){.x=(this.x / val), .y=(this.y / val)};
}

bool std_vec_Vec2__2_eq(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return this.x==other.x && this.y==other.y;
}

std_vec_Vec2__2 std_vec_Vec2__2_subs(std_vec_Vec2__2 this, f64 val) {
  return (std_vec_Vec2__2){.x=(this.x - val), .y=(this.y - val)};
}

std_vec_Vec2__5 std_vec_Vec2__2_to_u32(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__5){.x=((u32)this.x), .y=((u32)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__2_to_u64(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__0){.x=((u64)this.x), .y=((u64)this.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_mults(std_vec_Vec2__2 this, f64 val) {
  return (std_vec_Vec2__2){.x=(this.x * val), .y=(this.y * val)};
}

u32 std_vec_Vec2__2_hash(std_vec_Vec2__2 this) {
  return std_traits_hash_pair_hash(f64_hash(this.x), f64_hash(this.y));
}

std_vec_Vec2__2 std_vec_Vec2__2_adds(std_vec_Vec2__2 this, f64 val) {
  return (std_vec_Vec2__2){.x=(this.x + val), .y=(this.y + val)};
}

std_vec_Vec2__2 std_vec_Vec2__2_negate(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__2){.x=-this.x, .y=-this.y};
}

f32 std_vec_Vec2__2_length(std_vec_Vec2__2 this) {
  return sqrtf(((f32)((this.x * this.x) + (this.y * this.y))));
}

f64 std_vec_Vec2__2_dot(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return (this.x * other.x) + (this.y * other.y);
}

f64 std_vec_Vec2__2_cross(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return (this.x * other.y) - (this.y * other.x);
}

std_vec_Vec2__1 std_vec_Vec2__2_rotate(std_vec_Vec2__2 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  return (std_vec_Vec2__1){.x=((((f32)this->x) * c) - (((f32)this->y) * s)), .y=((((f32)this->x) * s) + (((f32)this->y) * c))};
}

std_vec_Vec2__2 std_vec_Vec2__2_div(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return (std_vec_Vec2__2){.x=(this.x / other.x), .y=(this.y / other.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_divrs(f64 val, std_vec_Vec2__2 this) {
  return (std_vec_Vec2__2){.x=(val / this.x), .y=(val / this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_mult(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return (std_vec_Vec2__1){.x=(this.x * other.x), .y=(this.y * other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_addrs(f32 val, std_vec_Vec2__1 this) {
  return (std_vec_Vec2__1){.x=(val + this.x), .y=(val + this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_to_f32(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__1){.x=((f32)this.x), .y=((f32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__1_to_i32(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__3){.x=((i32)this.x), .y=((i32)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__1_to_i64(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__4){.x=((i64)this.x), .y=((i64)this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_reflect(std_vec_Vec2__1 this, std_vec_Vec2__1 normal) {
  return std_vec_Vec2__1_sub(this, std_vec_Vec2__1_mults(normal, (((f32)2) * std_vec_Vec2__1_dot(this, normal))));
}

std_vec_Vec2__1 std_vec_Vec2__1_divs(std_vec_Vec2__1 this, f32 val) {
  return (std_vec_Vec2__1){.x=(this.x / val), .y=(this.y / val)};
}

bool std_vec_Vec2__1_eq(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return this.x==other.x && this.y==other.y;
}

std_vec_Vec2__1 std_vec_Vec2__1_subs(std_vec_Vec2__1 this, f32 val) {
  return (std_vec_Vec2__1){.x=(this.x - val), .y=(this.y - val)};
}

std_vec_Vec2__5 std_vec_Vec2__1_to_u32(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__5){.x=((u32)this.x), .y=((u32)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__1_to_u64(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__0){.x=((u64)this.x), .y=((u64)this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_mults(std_vec_Vec2__1 this, f32 val) {
  return (std_vec_Vec2__1){.x=(this.x * val), .y=(this.y * val)};
}

u32 std_vec_Vec2__1_hash(std_vec_Vec2__1 this) {
  return std_traits_hash_pair_hash(f32_hash(this.x), f32_hash(this.y));
}

std_vec_Vec2__1 std_vec_Vec2__1_adds(std_vec_Vec2__1 this, f32 val) {
  return (std_vec_Vec2__1){.x=(this.x + val), .y=(this.y + val)};
}

std_vec_Vec2__1 std_vec_Vec2__1_negate(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__1){.x=-this.x, .y=-this.y};
}

f32 std_vec_Vec2__1_length(std_vec_Vec2__1 this) {
  return sqrtf(((f32)((this.x * this.x) + (this.y * this.y))));
}

f32 std_vec_Vec2__1_dot(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return (this.x * other.x) + (this.y * other.y);
}

f32 std_vec_Vec2__1_cross(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return (this.x * other.y) - (this.y * other.x);
}

std_vec_Vec2__1 std_vec_Vec2__1_rotate(std_vec_Vec2__1 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  return (std_vec_Vec2__1){.x=((((f32)this->x) * c) - (((f32)this->y) * s)), .y=((((f32)this->x) * s) + (((f32)this->y) * c))};
}

std_vec_Vec2__1 std_vec_Vec2__1_div(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return (std_vec_Vec2__1){.x=(this.x / other.x), .y=(this.y / other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_divrs(f32 val, std_vec_Vec2__1 this) {
  return (std_vec_Vec2__1){.x=(val / this.x), .y=(val / this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_multrs(u64 val, std_vec_Vec2__0 this) {
  return (std_vec_Vec2__0){.x=(val * this.x), .y=(val * this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_add(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return (std_vec_Vec2__0){.x=(this.x + other.x), .y=(this.y + other.y)};
}

u64 std_vec_Vec2__0_length_sq(std_vec_Vec2__0 this) {
  return (this.x * this.x) + (this.y * this.y);
}

std_vec_Vec2__2 std_vec_Vec2__0_to_f64(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__2){.x=((f64)this.x), .y=((f64)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_mult(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return (std_vec_Vec2__0){.x=(this.x * other.x), .y=(this.y * other.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_addrs(u64 val, std_vec_Vec2__0 this) {
  return (std_vec_Vec2__0){.x=(val + this.x), .y=(val + this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__0_to_f32(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__1){.x=((f32)this.x), .y=((f32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__0_to_i32(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__3){.x=((i32)this.x), .y=((i32)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__0_to_i64(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__4){.x=((i64)this.x), .y=((i64)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_reflect(std_vec_Vec2__0 this, std_vec_Vec2__0 normal) {
  return std_vec_Vec2__0_sub(this, std_vec_Vec2__0_mults(normal, (((u64)2) * std_vec_Vec2__0_dot(this, normal))));
}

std_vec_Vec2__0 std_vec_Vec2__0_divs(std_vec_Vec2__0 this, u64 val) {
  return (std_vec_Vec2__0){.x=(this.x / val), .y=(this.y / val)};
}

bool std_vec_Vec2__0_eq(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return this.x==other.x && this.y==other.y;
}

std_vec_Vec2__0 std_vec_Vec2__0_subs(std_vec_Vec2__0 this, u64 val) {
  return (std_vec_Vec2__0){.x=(this.x - val), .y=(this.y - val)};
}

std_vec_Vec2__5 std_vec_Vec2__0_to_u32(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__5){.x=((u32)this.x), .y=((u32)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_to_u64(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__0){.x=((u64)this.x), .y=((u64)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_mults(std_vec_Vec2__0 this, u64 val) {
  return (std_vec_Vec2__0){.x=(this.x * val), .y=(this.y * val)};
}

u32 std_vec_Vec2__0_hash(std_vec_Vec2__0 this) {
  return std_traits_hash_pair_hash(u64_hash(this.x), u64_hash(this.y));
}

std_vec_Vec2__0 std_vec_Vec2__0_adds(std_vec_Vec2__0 this, u64 val) {
  return (std_vec_Vec2__0){.x=(this.x + val), .y=(this.y + val)};
}

std_vec_Vec2__0 std_vec_Vec2__0_negate(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__0){.x=-this.x, .y=-this.y};
}

f32 std_vec_Vec2__0_length(std_vec_Vec2__0 this) {
  return sqrtf(((f32)((this.x * this.x) + (this.y * this.y))));
}

u64 std_vec_Vec2__0_dot(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return (this.x * other.x) + (this.y * other.y);
}

u64 std_vec_Vec2__0_cross(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return (this.x * other.y) - (this.y * other.x);
}

std_vec_Vec2__1 std_vec_Vec2__0_rotate(std_vec_Vec2__0 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  return (std_vec_Vec2__1){.x=((((f32)this->x) * c) - (((f32)this->y) * s)), .y=((((f32)this->x) * s) + (((f32)this->y) * c))};
}

std_vec_Vec2__0 std_vec_Vec2__0_div(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return (std_vec_Vec2__0){.x=(this.x / other.x), .y=(this.y / other.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_divrs(u64 val, std_vec_Vec2__0 this) {
  return (std_vec_Vec2__0){.x=(val / this.x), .y=(val / this.y)};
}

std_vec_Vec3__0 std_vec_Vec3__0_subrs(f32 val, std_vec_Vec3__0 this) {
  return (std_vec_Vec3__0){.x=(val - this.x), .y=(val - this.y), .z=(val - this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_sub(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return (std_vec_Vec3__0){.x=(this.x - other.x), .y=(this.y - other.y), .z=(this.z - other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_rotateX(std_vec_Vec3__0 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 y = ((((f32)this->y) * c) - (((f32)this->z) * s));
  f32 z = ((((f32)this->y) * s) + (((f32)this->z) * c));
  return (std_vec_Vec3__0){.x=((f32)this->x), .y=y, .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__0_normalized(std_vec_Vec3__0 this) {
  f32 len = std_vec_Vec3__0_length(this);
  return (std_vec_Vec3__0){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len), .z=(((f32)this.z) / len)};
}

std_vec_Vec3__0 std_vec_Vec3__0_multrs(f32 val, std_vec_Vec3__0 this) {
  return (std_vec_Vec3__0){.x=(val * this.x), .y=(val * this.y), .z=(val * this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_add(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return (std_vec_Vec3__0){.x=(this.x + other.x), .y=(this.y + other.y), .z=(this.z + other.z)};
}

f32 std_vec_Vec3__0_length_sq(std_vec_Vec3__0 this) {
  return ((this.x * this.x) + (this.y * this.y)) + (this.z * this.z);
}

std_vec_Vec3__1 std_vec_Vec3__0_to_f64(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__1){.x=((f64)this.x), .y=((f64)this.y), .z=((f64)this.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_subrs(f64 val, std_vec_Vec3__1 this) {
  return (std_vec_Vec3__1){.x=(val - this.x), .y=(val - this.y), .z=(val - this.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_sub(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return (std_vec_Vec3__1){.x=(this.x - other.x), .y=(this.y - other.y), .z=(this.z - other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__1_rotateX(std_vec_Vec3__1 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 y = ((((f32)this->y) * c) - (((f32)this->z) * s));
  f32 z = ((((f32)this->y) * s) + (((f32)this->z) * c));
  return (std_vec_Vec3__0){.x=((f32)this->x), .y=y, .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__1_normalized(std_vec_Vec3__1 this) {
  f32 len = std_vec_Vec3__1_length(this);
  return (std_vec_Vec3__0){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len), .z=(((f32)this.z) / len)};
}

std_vec_Vec3__1 std_vec_Vec3__1_multrs(f64 val, std_vec_Vec3__1 this) {
  return (std_vec_Vec3__1){.x=(val * this.x), .y=(val * this.y), .z=(val * this.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_add(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return (std_vec_Vec3__1){.x=(this.x + other.x), .y=(this.y + other.y), .z=(this.z + other.z)};
}

f64 std_vec_Vec3__1_length_sq(std_vec_Vec3__1 this) {
  return ((this.x * this.x) + (this.y * this.y)) + (this.z * this.z);
}

std_vec_Vec3__1 std_vec_Vec3__1_to_f64(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__1){.x=((f64)this.x), .y=((f64)this.y), .z=((f64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__1_rotateY(std_vec_Vec3__1 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 z = ((((f32)this->z) * c) - (((f32)this->x) * s));
  f32 x = ((((f32)this->z) * s) + (((f32)this->x) * c));
  return (std_vec_Vec3__0){.x=x, .y=((f32)this->y), .z=z};
}

std_vec_Vec3__1 std_vec_Vec3__1_mult(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return (std_vec_Vec3__1){.x=(this.x * other.x), .y=(this.y * other.y), .z=(this.z * other.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_addrs(f64 val, std_vec_Vec3__1 this) {
  return (std_vec_Vec3__1){.x=(val + this.x), .y=(val + this.y), .z=(val + this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__1_to_f32(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__0){.x=((f32)this.x), .y=((f32)this.y), .z=((f32)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__1_rotateZ(std_vec_Vec3__1 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 x = ((((f32)this->x) * c) - (((f32)this->y) * s));
  f32 y = ((((f32)this->x) * s) + (((f32)this->y) * c));
  return (std_vec_Vec3__0){.x=x, .y=y, .z=((f32)this->z)};
}

std_vec_Vec3__2 std_vec_Vec3__1_to_i32(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__2){.x=((i32)this.x), .y=((i32)this.y), .z=((i32)this.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_subrs(i32 val, std_vec_Vec3__2 this) {
  return (std_vec_Vec3__2){.x=(val - this.x), .y=(val - this.y), .z=(val - this.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_sub(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return (std_vec_Vec3__2){.x=(this.x - other.x), .y=(this.y - other.y), .z=(this.z - other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__2_rotateX(std_vec_Vec3__2 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 y = ((((f32)this->y) * c) - (((f32)this->z) * s));
  f32 z = ((((f32)this->y) * s) + (((f32)this->z) * c));
  return (std_vec_Vec3__0){.x=((f32)this->x), .y=y, .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__2_normalized(std_vec_Vec3__2 this) {
  f32 len = std_vec_Vec3__2_length(this);
  return (std_vec_Vec3__0){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len), .z=(((f32)this.z) / len)};
}

std_vec_Vec3__2 std_vec_Vec3__2_multrs(i32 val, std_vec_Vec3__2 this) {
  return (std_vec_Vec3__2){.x=(val * this.x), .y=(val * this.y), .z=(val * this.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_add(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return (std_vec_Vec3__2){.x=(this.x + other.x), .y=(this.y + other.y), .z=(this.z + other.z)};
}

i32 std_vec_Vec3__2_length_sq(std_vec_Vec3__2 this) {
  return ((this.x * this.x) + (this.y * this.y)) + (this.z * this.z);
}

std_vec_Vec3__1 std_vec_Vec3__2_to_f64(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__1){.x=((f64)this.x), .y=((f64)this.y), .z=((f64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__2_rotateY(std_vec_Vec3__2 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 z = ((((f32)this->z) * c) - (((f32)this->x) * s));
  f32 x = ((((f32)this->z) * s) + (((f32)this->x) * c));
  return (std_vec_Vec3__0){.x=x, .y=((f32)this->y), .z=z};
}

std_vec_Vec3__2 std_vec_Vec3__2_mult(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return (std_vec_Vec3__2){.x=(this.x * other.x), .y=(this.y * other.y), .z=(this.z * other.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_addrs(i32 val, std_vec_Vec3__2 this) {
  return (std_vec_Vec3__2){.x=(val + this.x), .y=(val + this.y), .z=(val + this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__2_to_f32(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__0){.x=((f32)this.x), .y=((f32)this.y), .z=((f32)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__2_rotateZ(std_vec_Vec3__2 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 x = ((((f32)this->x) * c) - (((f32)this->y) * s));
  f32 y = ((((f32)this->x) * s) + (((f32)this->y) * c));
  return (std_vec_Vec3__0){.x=x, .y=y, .z=((f32)this->z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_to_i32(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__2){.x=((i32)this.x), .y=((i32)this.y), .z=((i32)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__2_to_i64(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__3){.x=((i64)this.x), .y=((i64)this.y), .z=((i64)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_subrs(i64 val, std_vec_Vec3__3 this) {
  return (std_vec_Vec3__3){.x=(val - this.x), .y=(val - this.y), .z=(val - this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_sub(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return (std_vec_Vec3__3){.x=(this.x - other.x), .y=(this.y - other.y), .z=(this.z - other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__3_rotateX(std_vec_Vec3__3 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 y = ((((f32)this->y) * c) - (((f32)this->z) * s));
  f32 z = ((((f32)this->y) * s) + (((f32)this->z) * c));
  return (std_vec_Vec3__0){.x=((f32)this->x), .y=y, .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__3_normalized(std_vec_Vec3__3 this) {
  f32 len = std_vec_Vec3__3_length(this);
  return (std_vec_Vec3__0){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len), .z=(((f32)this.z) / len)};
}

std_vec_Vec3__3 std_vec_Vec3__3_multrs(i64 val, std_vec_Vec3__3 this) {
  return (std_vec_Vec3__3){.x=(val * this.x), .y=(val * this.y), .z=(val * this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_add(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return (std_vec_Vec3__3){.x=(this.x + other.x), .y=(this.y + other.y), .z=(this.z + other.z)};
}

i64 std_vec_Vec3__3_length_sq(std_vec_Vec3__3 this) {
  return ((this.x * this.x) + (this.y * this.y)) + (this.z * this.z);
}

std_vec_Vec3__1 std_vec_Vec3__3_to_f64(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__1){.x=((f64)this.x), .y=((f64)this.y), .z=((f64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__3_rotateY(std_vec_Vec3__3 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 z = ((((f32)this->z) * c) - (((f32)this->x) * s));
  f32 x = ((((f32)this->z) * s) + (((f32)this->x) * c));
  return (std_vec_Vec3__0){.x=x, .y=((f32)this->y), .z=z};
}

std_vec_Vec3__3 std_vec_Vec3__3_mult(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return (std_vec_Vec3__3){.x=(this.x * other.x), .y=(this.y * other.y), .z=(this.z * other.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_addrs(i64 val, std_vec_Vec3__3 this) {
  return (std_vec_Vec3__3){.x=(val + this.x), .y=(val + this.y), .z=(val + this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__3_to_f32(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__0){.x=((f32)this.x), .y=((f32)this.y), .z=((f32)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__3_rotateZ(std_vec_Vec3__3 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 x = ((((f32)this->x) * c) - (((f32)this->y) * s));
  f32 y = ((((f32)this->x) * s) + (((f32)this->y) * c));
  return (std_vec_Vec3__0){.x=x, .y=y, .z=((f32)this->z)};
}

std_vec_Vec3__2 std_vec_Vec3__3_to_i32(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__2){.x=((i32)this.x), .y=((i32)this.y), .z=((i32)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_to_i64(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__3){.x=((i64)this.x), .y=((i64)this.y), .z=((i64)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_reflect(std_vec_Vec3__3 this, std_vec_Vec3__3 normal) {
  return std_vec_Vec3__3_sub(this, std_vec_Vec3__3_mults(normal, (((i64)2) * std_vec_Vec3__3_dot(this, normal))));
}

std_vec_Vec3__3 std_vec_Vec3__3_divs(std_vec_Vec3__3 this, i64 val) {
  return (std_vec_Vec3__3){.x=(this.x / val), .y=(this.y / val), .z=(this.z / val)};
}

bool std_vec_Vec3__3_eq(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return (this.x==other.x && this.y==other.y) && this.z==other.z;
}

std_vec_Vec3__3 std_vec_Vec3__3_subs(std_vec_Vec3__3 this, i64 val) {
  return (std_vec_Vec3__3){.x=(this.x - val), .y=(this.y - val), .z=(this.z - val)};
}

std_vec_Vec3__4 std_vec_Vec3__3_to_u32(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__4){.x=((u32)this.x), .y=((u32)this.y), .z=((u32)this.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_subrs(u32 val, std_vec_Vec3__4 this) {
  return (std_vec_Vec3__4){.x=(val - this.x), .y=(val - this.y), .z=(val - this.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_sub(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return (std_vec_Vec3__4){.x=(this.x - other.x), .y=(this.y - other.y), .z=(this.z - other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__4_rotateX(std_vec_Vec3__4 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 y = ((((f32)this->y) * c) - (((f32)this->z) * s));
  f32 z = ((((f32)this->y) * s) + (((f32)this->z) * c));
  return (std_vec_Vec3__0){.x=((f32)this->x), .y=y, .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__4_normalized(std_vec_Vec3__4 this) {
  f32 len = std_vec_Vec3__4_length(this);
  return (std_vec_Vec3__0){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len), .z=(((f32)this.z) / len)};
}

std_vec_Vec3__4 std_vec_Vec3__4_multrs(u32 val, std_vec_Vec3__4 this) {
  return (std_vec_Vec3__4){.x=(val * this.x), .y=(val * this.y), .z=(val * this.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_add(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return (std_vec_Vec3__4){.x=(this.x + other.x), .y=(this.y + other.y), .z=(this.z + other.z)};
}

u32 std_vec_Vec3__4_length_sq(std_vec_Vec3__4 this) {
  return ((this.x * this.x) + (this.y * this.y)) + (this.z * this.z);
}

std_vec_Vec3__1 std_vec_Vec3__4_to_f64(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__1){.x=((f64)this.x), .y=((f64)this.y), .z=((f64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__4_rotateY(std_vec_Vec3__4 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 z = ((((f32)this->z) * c) - (((f32)this->x) * s));
  f32 x = ((((f32)this->z) * s) + (((f32)this->x) * c));
  return (std_vec_Vec3__0){.x=x, .y=((f32)this->y), .z=z};
}

std_vec_Vec3__4 std_vec_Vec3__4_mult(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return (std_vec_Vec3__4){.x=(this.x * other.x), .y=(this.y * other.y), .z=(this.z * other.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_addrs(u32 val, std_vec_Vec3__4 this) {
  return (std_vec_Vec3__4){.x=(val + this.x), .y=(val + this.y), .z=(val + this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__4_to_f32(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__0){.x=((f32)this.x), .y=((f32)this.y), .z=((f32)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__4_rotateZ(std_vec_Vec3__4 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 x = ((((f32)this->x) * c) - (((f32)this->y) * s));
  f32 y = ((((f32)this->x) * s) + (((f32)this->y) * c));
  return (std_vec_Vec3__0){.x=x, .y=y, .z=((f32)this->z)};
}

std_vec_Vec3__2 std_vec_Vec3__4_to_i32(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__2){.x=((i32)this.x), .y=((i32)this.y), .z=((i32)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__4_to_i64(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__3){.x=((i64)this.x), .y=((i64)this.y), .z=((i64)this.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_reflect(std_vec_Vec3__4 this, std_vec_Vec3__4 normal) {
  return std_vec_Vec3__4_sub(this, std_vec_Vec3__4_mults(normal, (((u32)2) * std_vec_Vec3__4_dot(this, normal))));
}

std_vec_Vec3__4 std_vec_Vec3__4_divs(std_vec_Vec3__4 this, u32 val) {
  return (std_vec_Vec3__4){.x=(this.x / val), .y=(this.y / val), .z=(this.z / val)};
}

bool std_vec_Vec3__4_eq(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return (this.x==other.x && this.y==other.y) && this.z==other.z;
}

std_vec_Vec3__4 std_vec_Vec3__4_subs(std_vec_Vec3__4 this, u32 val) {
  return (std_vec_Vec3__4){.x=(this.x - val), .y=(this.y - val), .z=(this.z - val)};
}

std_vec_Vec3__4 std_vec_Vec3__4_to_u32(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__4){.x=((u32)this.x), .y=((u32)this.y), .z=((u32)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__4_to_u64(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__5){.x=((u64)this.x), .y=((u64)this.y), .z=((u64)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_subrs(u64 val, std_vec_Vec3__5 this) {
  return (std_vec_Vec3__5){.x=(val - this.x), .y=(val - this.y), .z=(val - this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_sub(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return (std_vec_Vec3__5){.x=(this.x - other.x), .y=(this.y - other.y), .z=(this.z - other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__5_rotateX(std_vec_Vec3__5 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 y = ((((f32)this->y) * c) - (((f32)this->z) * s));
  f32 z = ((((f32)this->y) * s) + (((f32)this->z) * c));
  return (std_vec_Vec3__0){.x=((f32)this->x), .y=y, .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__5_normalized(std_vec_Vec3__5 this) {
  f32 len = std_vec_Vec3__5_length(this);
  return (std_vec_Vec3__0){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len), .z=(((f32)this.z) / len)};
}

std_vec_Vec3__5 std_vec_Vec3__5_multrs(u64 val, std_vec_Vec3__5 this) {
  return (std_vec_Vec3__5){.x=(val * this.x), .y=(val * this.y), .z=(val * this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_add(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return (std_vec_Vec3__5){.x=(this.x + other.x), .y=(this.y + other.y), .z=(this.z + other.z)};
}

u64 std_vec_Vec3__5_length_sq(std_vec_Vec3__5 this) {
  return ((this.x * this.x) + (this.y * this.y)) + (this.z * this.z);
}

std_vec_Vec3__1 std_vec_Vec3__5_to_f64(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__1){.x=((f64)this.x), .y=((f64)this.y), .z=((f64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__5_rotateY(std_vec_Vec3__5 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 z = ((((f32)this->z) * c) - (((f32)this->x) * s));
  f32 x = ((((f32)this->z) * s) + (((f32)this->x) * c));
  return (std_vec_Vec3__0){.x=x, .y=((f32)this->y), .z=z};
}

std_vec_Vec3__5 std_vec_Vec3__5_mult(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return (std_vec_Vec3__5){.x=(this.x * other.x), .y=(this.y * other.y), .z=(this.z * other.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_addrs(u64 val, std_vec_Vec3__5 this) {
  return (std_vec_Vec3__5){.x=(val + this.x), .y=(val + this.y), .z=(val + this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__5_to_f32(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__0){.x=((f32)this.x), .y=((f32)this.y), .z=((f32)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__5_rotateZ(std_vec_Vec3__5 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 x = ((((f32)this->x) * c) - (((f32)this->y) * s));
  f32 y = ((((f32)this->x) * s) + (((f32)this->y) * c));
  return (std_vec_Vec3__0){.x=x, .y=y, .z=((f32)this->z)};
}

std_vec_Vec3__2 std_vec_Vec3__5_to_i32(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__2){.x=((i32)this.x), .y=((i32)this.y), .z=((i32)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__5_to_i64(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__3){.x=((i64)this.x), .y=((i64)this.y), .z=((i64)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_reflect(std_vec_Vec3__5 this, std_vec_Vec3__5 normal) {
  return std_vec_Vec3__5_sub(this, std_vec_Vec3__5_mults(normal, (((u64)2) * std_vec_Vec3__5_dot(this, normal))));
}

std_vec_Vec3__5 std_vec_Vec3__5_divs(std_vec_Vec3__5 this, u64 val) {
  return (std_vec_Vec3__5){.x=(this.x / val), .y=(this.y / val), .z=(this.z / val)};
}

bool std_vec_Vec3__5_eq(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return (this.x==other.x && this.y==other.y) && this.z==other.z;
}

std_vec_Vec3__5 std_vec_Vec3__5_subs(std_vec_Vec3__5 this, u64 val) {
  return (std_vec_Vec3__5){.x=(this.x - val), .y=(this.y - val), .z=(this.z - val)};
}

std_vec_Vec3__4 std_vec_Vec3__5_to_u32(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__4){.x=((u32)this.x), .y=((u32)this.y), .z=((u32)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_to_u64(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__5){.x=((u64)this.x), .y=((u64)this.y), .z=((u64)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_mults(std_vec_Vec3__5 this, u64 val) {
  return (std_vec_Vec3__5){.x=(this.x * val), .y=(this.y * val), .z=(this.z * val)};
}

u32 std_vec_Vec3__5_hash(std_vec_Vec3__5 this) {
  return std_traits_hash_pair_hash(u64_hash(this.x), std_traits_hash_pair_hash(u64_hash(this.y), u64_hash(this.z)));
}

std_vec_Vec3__5 std_vec_Vec3__5_adds(std_vec_Vec3__5 this, u64 val) {
  return (std_vec_Vec3__5){.x=(this.x + val), .y=(this.y + val), .z=(this.z + val)};
}

std_vec_Vec3__5 std_vec_Vec3__5_negate(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__5){.x=-this.x, .y=-this.y, .z=-this.z};
}

f32 std_vec_Vec3__5_length(std_vec_Vec3__5 this) {
  return sqrtf(((f32)(((this.x * this.x) + (this.y * this.y)) + (this.z * this.z))));
}

u64 std_vec_Vec3__5_dot(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return ((this.x * other.x) + (this.y * other.y)) + (this.z * other.z);
}

u64 std_vec_Vec3__5_index(std_vec_Vec3__5 this, u32 idx) {
  return ((u64 *)&this)[idx];
}

std_vec_Vec3__5 std_vec_Vec3__5_cross(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return (std_vec_Vec3__5){.x=((this.y * other.z) - (this.z * other.y)), .y=((this.z * other.x) - (this.x * other.z)), .z=((this.x * other.y) - (this.y * other.x))};
}

std_vec_Vec3__5 std_vec_Vec3__5_div(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return (std_vec_Vec3__5){.x=(this.x / other.x), .y=(this.y / other.y), .z=(this.z / other.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_divrs(u64 val, std_vec_Vec3__5 this) {
  return (std_vec_Vec3__5){.x=(val / this.x), .y=(val / this.y), .z=(val / this.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_mults(std_vec_Vec3__4 this, u32 val) {
  return (std_vec_Vec3__4){.x=(this.x * val), .y=(this.y * val), .z=(this.z * val)};
}

u32 std_vec_Vec3__4_hash(std_vec_Vec3__4 this) {
  return std_traits_hash_pair_hash(u32_hash(this.x), std_traits_hash_pair_hash(u32_hash(this.y), u32_hash(this.z)));
}

std_vec_Vec3__4 std_vec_Vec3__4_adds(std_vec_Vec3__4 this, u32 val) {
  return (std_vec_Vec3__4){.x=(this.x + val), .y=(this.y + val), .z=(this.z + val)};
}

std_vec_Vec3__4 std_vec_Vec3__4_negate(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__4){.x=-this.x, .y=-this.y, .z=-this.z};
}

f32 std_vec_Vec3__4_length(std_vec_Vec3__4 this) {
  return sqrtf(((f32)(((this.x * this.x) + (this.y * this.y)) + (this.z * this.z))));
}

u32 std_vec_Vec3__4_dot(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return ((this.x * other.x) + (this.y * other.y)) + (this.z * other.z);
}

u32 std_vec_Vec3__4_index(std_vec_Vec3__4 this, u32 idx) {
  return ((u32 *)&this)[idx];
}

std_vec_Vec3__4 std_vec_Vec3__4_cross(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return (std_vec_Vec3__4){.x=((this.y * other.z) - (this.z * other.y)), .y=((this.z * other.x) - (this.x * other.z)), .z=((this.x * other.y) - (this.y * other.x))};
}

std_vec_Vec3__4 std_vec_Vec3__4_div(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return (std_vec_Vec3__4){.x=(this.x / other.x), .y=(this.y / other.y), .z=(this.z / other.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_divrs(u32 val, std_vec_Vec3__4 this) {
  return (std_vec_Vec3__4){.x=(val / this.x), .y=(val / this.y), .z=(val / this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__3_to_u64(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__5){.x=((u64)this.x), .y=((u64)this.y), .z=((u64)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_mults(std_vec_Vec3__3 this, i64 val) {
  return (std_vec_Vec3__3){.x=(this.x * val), .y=(this.y * val), .z=(this.z * val)};
}

u32 std_vec_Vec3__3_hash(std_vec_Vec3__3 this) {
  return std_traits_hash_pair_hash(i64_hash(this.x), std_traits_hash_pair_hash(i64_hash(this.y), i64_hash(this.z)));
}

std_vec_Vec3__3 std_vec_Vec3__3_adds(std_vec_Vec3__3 this, i64 val) {
  return (std_vec_Vec3__3){.x=(this.x + val), .y=(this.y + val), .z=(this.z + val)};
}

std_vec_Vec3__3 std_vec_Vec3__3_negate(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__3){.x=-this.x, .y=-this.y, .z=-this.z};
}

f32 std_vec_Vec3__3_length(std_vec_Vec3__3 this) {
  return sqrtf(((f32)(((this.x * this.x) + (this.y * this.y)) + (this.z * this.z))));
}

i64 std_vec_Vec3__3_dot(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return ((this.x * other.x) + (this.y * other.y)) + (this.z * other.z);
}

i64 std_vec_Vec3__3_index(std_vec_Vec3__3 this, u32 idx) {
  return ((i64 *)&this)[idx];
}

std_vec_Vec3__3 std_vec_Vec3__3_cross(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return (std_vec_Vec3__3){.x=((this.y * other.z) - (this.z * other.y)), .y=((this.z * other.x) - (this.x * other.z)), .z=((this.x * other.y) - (this.y * other.x))};
}

std_vec_Vec3__3 std_vec_Vec3__3_div(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return (std_vec_Vec3__3){.x=(this.x / other.x), .y=(this.y / other.y), .z=(this.z / other.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_divrs(i64 val, std_vec_Vec3__3 this) {
  return (std_vec_Vec3__3){.x=(val / this.x), .y=(val / this.y), .z=(val / this.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_reflect(std_vec_Vec3__2 this, std_vec_Vec3__2 normal) {
  return std_vec_Vec3__2_sub(this, std_vec_Vec3__2_mults(normal, (((i32)2) * std_vec_Vec3__2_dot(this, normal))));
}

std_vec_Vec3__2 std_vec_Vec3__2_divs(std_vec_Vec3__2 this, i32 val) {
  return (std_vec_Vec3__2){.x=(this.x / val), .y=(this.y / val), .z=(this.z / val)};
}

bool std_vec_Vec3__2_eq(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return (this.x==other.x && this.y==other.y) && this.z==other.z;
}

std_vec_Vec3__2 std_vec_Vec3__2_subs(std_vec_Vec3__2 this, i32 val) {
  return (std_vec_Vec3__2){.x=(this.x - val), .y=(this.y - val), .z=(this.z - val)};
}

std_vec_Vec3__4 std_vec_Vec3__2_to_u32(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__4){.x=((u32)this.x), .y=((u32)this.y), .z=((u32)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__2_to_u64(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__5){.x=((u64)this.x), .y=((u64)this.y), .z=((u64)this.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_mults(std_vec_Vec3__2 this, i32 val) {
  return (std_vec_Vec3__2){.x=(this.x * val), .y=(this.y * val), .z=(this.z * val)};
}

u32 std_vec_Vec3__2_hash(std_vec_Vec3__2 this) {
  return std_traits_hash_pair_hash(i32_hash(this.x), std_traits_hash_pair_hash(i32_hash(this.y), i32_hash(this.z)));
}

std_vec_Vec3__2 std_vec_Vec3__2_adds(std_vec_Vec3__2 this, i32 val) {
  return (std_vec_Vec3__2){.x=(this.x + val), .y=(this.y + val), .z=(this.z + val)};
}

std_vec_Vec3__2 std_vec_Vec3__2_negate(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__2){.x=-this.x, .y=-this.y, .z=-this.z};
}

f32 std_vec_Vec3__2_length(std_vec_Vec3__2 this) {
  return sqrtf(((f32)(((this.x * this.x) + (this.y * this.y)) + (this.z * this.z))));
}

i32 std_vec_Vec3__2_dot(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return ((this.x * other.x) + (this.y * other.y)) + (this.z * other.z);
}

i32 std_vec_Vec3__2_index(std_vec_Vec3__2 this, u32 idx) {
  return ((i32 *)&this)[idx];
}

std_vec_Vec3__2 std_vec_Vec3__2_cross(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return (std_vec_Vec3__2){.x=((this.y * other.z) - (this.z * other.y)), .y=((this.z * other.x) - (this.x * other.z)), .z=((this.x * other.y) - (this.y * other.x))};
}

std_vec_Vec3__2 std_vec_Vec3__2_div(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return (std_vec_Vec3__2){.x=(this.x / other.x), .y=(this.y / other.y), .z=(this.z / other.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_divrs(i32 val, std_vec_Vec3__2 this) {
  return (std_vec_Vec3__2){.x=(val / this.x), .y=(val / this.y), .z=(val / this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__1_to_i64(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__3){.x=((i64)this.x), .y=((i64)this.y), .z=((i64)this.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_reflect(std_vec_Vec3__1 this, std_vec_Vec3__1 normal) {
  return std_vec_Vec3__1_sub(this, std_vec_Vec3__1_mults(normal, (((f64)2) * std_vec_Vec3__1_dot(this, normal))));
}

std_vec_Vec3__1 std_vec_Vec3__1_divs(std_vec_Vec3__1 this, f64 val) {
  return (std_vec_Vec3__1){.x=(this.x / val), .y=(this.y / val), .z=(this.z / val)};
}

bool std_vec_Vec3__1_eq(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return (this.x==other.x && this.y==other.y) && this.z==other.z;
}

std_vec_Vec3__1 std_vec_Vec3__1_subs(std_vec_Vec3__1 this, f64 val) {
  return (std_vec_Vec3__1){.x=(this.x - val), .y=(this.y - val), .z=(this.z - val)};
}

std_vec_Vec3__4 std_vec_Vec3__1_to_u32(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__4){.x=((u32)this.x), .y=((u32)this.y), .z=((u32)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__1_to_u64(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__5){.x=((u64)this.x), .y=((u64)this.y), .z=((u64)this.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_mults(std_vec_Vec3__1 this, f64 val) {
  return (std_vec_Vec3__1){.x=(this.x * val), .y=(this.y * val), .z=(this.z * val)};
}

u32 std_vec_Vec3__1_hash(std_vec_Vec3__1 this) {
  return std_traits_hash_pair_hash(f64_hash(this.x), std_traits_hash_pair_hash(f64_hash(this.y), f64_hash(this.z)));
}

std_vec_Vec3__1 std_vec_Vec3__1_adds(std_vec_Vec3__1 this, f64 val) {
  return (std_vec_Vec3__1){.x=(this.x + val), .y=(this.y + val), .z=(this.z + val)};
}

std_vec_Vec3__1 std_vec_Vec3__1_negate(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__1){.x=-this.x, .y=-this.y, .z=-this.z};
}

f32 std_vec_Vec3__1_length(std_vec_Vec3__1 this) {
  return sqrtf(((f32)(((this.x * this.x) + (this.y * this.y)) + (this.z * this.z))));
}

f64 std_vec_Vec3__1_dot(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return ((this.x * other.x) + (this.y * other.y)) + (this.z * other.z);
}

f64 std_vec_Vec3__1_index(std_vec_Vec3__1 this, u32 idx) {
  return ((f64 *)&this)[idx];
}

std_vec_Vec3__1 std_vec_Vec3__1_cross(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return (std_vec_Vec3__1){.x=((this.y * other.z) - (this.z * other.y)), .y=((this.z * other.x) - (this.x * other.z)), .z=((this.x * other.y) - (this.y * other.x))};
}

std_vec_Vec3__1 std_vec_Vec3__1_div(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return (std_vec_Vec3__1){.x=(this.x / other.x), .y=(this.y / other.y), .z=(this.z / other.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_divrs(f64 val, std_vec_Vec3__1 this) {
  return (std_vec_Vec3__1){.x=(val / this.x), .y=(val / this.y), .z=(val / this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_rotateY(std_vec_Vec3__0 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 z = ((((f32)this->z) * c) - (((f32)this->x) * s));
  f32 x = ((((f32)this->z) * s) + (((f32)this->x) * c));
  return (std_vec_Vec3__0){.x=x, .y=((f32)this->y), .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__0_mult(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return (std_vec_Vec3__0){.x=(this.x * other.x), .y=(this.y * other.y), .z=(this.z * other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_addrs(f32 val, std_vec_Vec3__0 this) {
  return (std_vec_Vec3__0){.x=(val + this.x), .y=(val + this.y), .z=(val + this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_to_f32(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__0){.x=((f32)this.x), .y=((f32)this.y), .z=((f32)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_rotateZ(std_vec_Vec3__0 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 x = ((((f32)this->x) * c) - (((f32)this->y) * s));
  f32 y = ((((f32)this->x) * s) + (((f32)this->y) * c));
  return (std_vec_Vec3__0){.x=x, .y=y, .z=((f32)this->z)};
}

std_vec_Vec3__2 std_vec_Vec3__0_to_i32(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__2){.x=((i32)this.x), .y=((i32)this.y), .z=((i32)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__0_to_i64(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__3){.x=((i64)this.x), .y=((i64)this.y), .z=((i64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_reflect(std_vec_Vec3__0 this, std_vec_Vec3__0 normal) {
  return std_vec_Vec3__0_sub(this, std_vec_Vec3__0_mults(normal, (((f32)2) * std_vec_Vec3__0_dot(this, normal))));
}

std_vec_Vec3__0 std_vec_Vec3__0_divs(std_vec_Vec3__0 this, f32 val) {
  return (std_vec_Vec3__0){.x=(this.x / val), .y=(this.y / val), .z=(this.z / val)};
}

bool std_vec_Vec3__0_eq(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return (this.x==other.x && this.y==other.y) && this.z==other.z;
}

std_vec_Vec3__0 std_vec_Vec3__0_subs(std_vec_Vec3__0 this, f32 val) {
  return (std_vec_Vec3__0){.x=(this.x - val), .y=(this.y - val), .z=(this.z - val)};
}

std_vec_Vec3__4 std_vec_Vec3__0_to_u32(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__4){.x=((u32)this.x), .y=((u32)this.y), .z=((u32)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__0_to_u64(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__5){.x=((u64)this.x), .y=((u64)this.y), .z=((u64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_mults(std_vec_Vec3__0 this, f32 val) {
  return (std_vec_Vec3__0){.x=(this.x * val), .y=(this.y * val), .z=(this.z * val)};
}

u32 std_vec_Vec3__0_hash(std_vec_Vec3__0 this) {
  return std_traits_hash_pair_hash(f32_hash(this.x), std_traits_hash_pair_hash(f32_hash(this.y), f32_hash(this.z)));
}

std_vec_Vec3__0 std_vec_Vec3__0_adds(std_vec_Vec3__0 this, f32 val) {
  return (std_vec_Vec3__0){.x=(this.x + val), .y=(this.y + val), .z=(this.z + val)};
}

std_vec_Vec3__0 std_vec_Vec3__0_negate(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__0){.x=-this.x, .y=-this.y, .z=-this.z};
}

f32 std_vec_Vec3__0_length(std_vec_Vec3__0 this) {
  return sqrtf(((f32)(((this.x * this.x) + (this.y * this.y)) + (this.z * this.z))));
}

f32 std_vec_Vec3__0_dot(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return ((this.x * other.x) + (this.y * other.y)) + (this.z * other.z);
}

f32 std_vec_Vec3__0_index(std_vec_Vec3__0 this, u32 idx) {
  return ((f32 *)&this)[idx];
}

std_vec_Vec3__0 std_vec_Vec3__0_cross(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return (std_vec_Vec3__0){.x=((this.y * other.z) - (this.z * other.y)), .y=((this.z * other.x) - (this.x * other.z)), .z=((this.x * other.y) - (this.y * other.x))};
}

std_vec_Vec3__0 std_vec_Vec3__0_div(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return (std_vec_Vec3__0){.x=(this.x / other.x), .y=(this.y / other.y), .z=(this.z / other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_divrs(f32 val, std_vec_Vec3__0 this) {
  return (std_vec_Vec3__0){.x=(val / this.x), .y=(val / this.y), .z=(val / this.z)};
}

std_og_Rect std_og_Rect_from_v(std_vec_Vec2__3 pos, std_vec_Vec2__3 size) {
  return (std_og_Rect){.x=pos.x, .y=pos.y, .w=size.x, .h=size.y};
}

bool std_og_Rect_contains(std_og_Rect this, std_vec_Vec2__3 pos) {
  if (!((this.x <= pos.x) && (pos.x <= (this.x + this.w)))) {
    return false;
  }
  if (!((this.y <= pos.y) && (pos.y <= (this.y + this.h)))) {
    return false;
  }
  return true;
}

u32 std_og_get_ticks(void) {
  return ((u32)SDL_GetTicks());
}

void std_og_init(u32 width, u32 height, char *title, bool fullscreen) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(((i32)width), ((i32)height), SDL_WINDOW_RESIZABLE, &std_og_state_window, &std_og_state_renderer);
  if (fullscreen) {
    SDL_SetWindowFullscreen(std_og_state_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  std_og_state_title=title;
  SDL_SetWindowTitle(std_og_state_window, title);
  std_og_state_prev_time=std_og_get_ticks();
  std_og_state_update_window_size();
}

void std_og_toggle_fullscreen(void) {
  i32 flags = SDL_GetWindowFlags(std_og_state_window);
  if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) {
    SDL_SetWindowFullscreen(std_og_state_window, 0);
  } else {
    SDL_SetWindowFullscreen(std_og_state_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
}

bool std_og_is_running(void) {
  SDL_RenderPresent(std_og_state_renderer);
  std_og_state_frame_num+=((u64)1);
  u32 cur_time = ((u32)std_og_get_ticks());
  std_og_state_frame_time=(((f32)(cur_time - std_og_state_prev_time)) / 1000.0f);
  std_og_state_prev_time=cur_time;
  std_og_state_update_window_size();
  std_og_state_add_frametime(std_og_state_frame_time);
  if ((std_og_state_frame_num % ((u64)30))==((u64)0)) {
    f32 avg = std_og_state_get_avg_frametime();
    sprintf(std_og_state_window_title_fps_buf, "%s | FPS: %.0f", std_og_state_title, (1.0f / avg));
    SDL_SetWindowTitle(std_og_state_window, std_og_state_window_title_fps_buf);
  }
  std_og_utils_handle_sdl_events();
  return std_og_state_running;
}

void std_og_clear(SDL_Color col) {
  SDL_SetRenderDrawColor(std_og_state_renderer, col.r, col.g, col.b, col.a);
  SDL_RenderClear(std_og_state_renderer);
}

void std_og_draw_rect(i32 x, i32 y, i32 w, i32 h, SDL_Color col) {
  SDL_SetRenderDrawColor(std_og_state_renderer, col.r, col.g, col.b, col.a);
  SDL_Rect rect = (SDL_Rect){.x=x, .y=y, .w=w, .h=h};
  SDL_RenderFillRect(std_og_state_renderer, &rect);
}

void std_og_draw_rect_v(std_vec_Vec2__3 pos, std_vec_Vec2__3 size, SDL_Color col) {
  std_og_draw_rect(pos.x, pos.y, size.x, size.y, col);
}

void std_og_draw_line(i32 x1, i32 y1, i32 x2, i32 y2, SDL_Color col) {
  SDL_SetRenderDrawColor(std_og_state_renderer, col.r, col.g, col.b, col.a);
  SDL_RenderDrawLine(std_og_state_renderer, x1, y1, x2, y2);
}

void std_og_draw_line_v(std_vec_Vec2__3 p1, std_vec_Vec2__3 p2, SDL_Color col) {
  std_og_draw_line(p1.x, p1.y, p2.x, p2.y, col);
}

void std_og_draw_pixel(i32 x, i32 y, SDL_Color col) {
  SDL_SetRenderDrawColor(std_og_state_renderer, col.r, col.g, col.b, col.a);
  SDL_RenderDrawPoint(std_og_state_renderer, x, y);
}

void std_og_draw_pixel_v(std_vec_Vec2__3 pos, SDL_Color col) {
  std_og_draw_pixel(pos.x, pos.y, col);
}

f32 std_og_get_frame_time(void) {
  return std_og_state_frame_time;
}

void std_og_quit(void) {
  SDL_Quit();
}

bool std_og_is_key_pressed(SDL_Scancode k) {
  return std_og_state_KeyState_get(&std_og_state_prev_keys, k)==false && std_og_state_KeyState_get(&std_og_state_keys, k)==true;
}

bool std_og_is_key_released(SDL_Scancode k) {
  return std_og_state_KeyState_get(&std_og_state_prev_keys, k)==true && std_og_state_KeyState_get(&std_og_state_keys, k)==false;
}

bool std_og_is_key_down(SDL_Scancode k) {
  return std_og_state_KeyState_get(&std_og_state_keys, k)==true;
}

bool std_og_is_key_up(SDL_Scancode k) {
  return std_og_state_KeyState_get(&std_og_state_keys, k)==false;
}

bool std_og_is_mouse_pressed(u8 b) {
  return std_og_state_MouseState_get_button(&std_og_state_prev_mouse, b)==false && std_og_state_MouseState_get_button(&std_og_state_mouse, b)==true;
}

bool std_og_is_mouse_released(u8 b) {
  return std_og_state_MouseState_get_button(&std_og_state_prev_mouse, b)==true && std_og_state_MouseState_get_button(&std_og_state_mouse, b)==false;
}

bool std_og_is_mouse_down(u8 b) {
  return std_og_state_MouseState_get_button(&std_og_state_mouse, b)==true;
}

bool std_og_is_mouse_up(u8 b) {
  return std_og_state_MouseState_get_button(&std_og_state_mouse, b)==false;
}

std_vec_Vec2__3 std_og_get_window_size(void) {
  return std_og_state_window_size;
}

bool std_og_is_scroll(std_og_state_MouseWheel w) {
  return std_og_state_MouseState_get_wheel(&std_og_state_mouse, w);
}

std_vec_Vec2__3 std_og_get_mouse_pos(void) {
  return std_og_state_mouse.pos;
}

std_vec_Vec2__3 std_og_get_mouse_delta(void) {
  return std_og_state_mouse.vec;
}

void std_og_capture_mouse(bool capture) {
  SDL_CaptureMouse(capture);
}

void std_og_grab_input(bool grab) {
  SDL_SetRelativeMouseMode(grab);
}

void std_og_show_cursor(bool show) {
  SDL_ShowCursor(show);
}

void std_og_set_cursor(SDL_SystemCursor c) {
  if (c==std_og_state_cursor_type) {
    return;
  }
  if (((bool)std_og_state_cursor)) {
    SDL_FreeCursor(std_og_state_cursor);
  }
  std_og_state_cursor=SDL_CreateSystemCursor(c);
  std_og_state_cursor_type=c;
  SDL_SetCursor(std_og_state_cursor);
}

void std_og_Texture_draw_pixel(std_og_Texture this, i32 x, i32 y, SDL_Color col) {
  if (((0 <= x) && (x < this.w)) && ((0 <= y) && (y < this.h))) {
    u32 offset = ((u32)((y * this.pitch) + (x * 4)));
    this.pixels[(offset + 0)]=((u8)col.r);
    this.pixels[(offset + 1)]=((u8)col.g);
    this.pixels[(offset + 2)]=((u8)col.b);
    this.pixels[(offset + 3)]=((u8)col.a);
  }
}

void std_og_Texture_draw_rect(std_og_Texture this, i32 x, i32 y, i32 w, i32 h, SDL_Color col) {
  x=i32_max(x, 0);
  y=i32_max(y, 0);
  w=i32_min(w, (this.w - x));
  h=i32_min(h, (this.h - y));
  for (i32 i = 0; i < h; i+=1) {
    i32 py = (y + i);
    u8 *view = (this.pixels + ((u32)((py * this.pitch) + (x * 4))));
    for (i32 j = 0; j < w; j+=1) {
      (*(view + 0))=((u8)col.r);
      (*(view + 1))=((u8)col.g);
      (*(view + 2))=((u8)col.b);
      (*(view + 3))=((u8)col.a);
      view=(view + 4);
    }
  }
}

void std_og_Texture_draw_rect_v(std_og_Texture this, std_vec_Vec2__3 pos, std_vec_Vec2__3 size, SDL_Color col) {
  std_og_Texture_draw_rect(this, pos.x, pos.y, size.x, size.y, col);
}

void std_og_Texture_draw_pixel_v(std_og_Texture this, std_vec_Vec2__3 pos, SDL_Color col) {
  std_og_Texture_draw_pixel(this, pos.x, pos.y, col);
}

std_og_Texture *std_og_get_buffer(void) {
  bool matches = true;
  if (!((bool)std_og_state_buffer.tx)) {
    matches=false;
  } else if ((std_og_state_buffer.w != std_og_state_window_size.x) || (std_og_state_buffer.h != std_og_state_window_size.y)) {
    SDL_DestroyTexture(std_og_state_buffer.tx);
    matches=false;
  }
  if (!matches) {
    std_og_Texture buf = {0};
    buf.tx=SDL_CreateTexture(std_og_state_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, std_og_state_window_size.x, std_og_state_window_size.y);
    buf.w=std_og_state_window_size.x;
    buf.h=std_og_state_window_size.y;
    std_og_state_buffer=buf;
  }
  SDL_LockTexture(std_og_state_buffer.tx, NULL, ((void **)&std_og_state_buffer.pixels), &std_og_state_buffer.pitch);
  return &std_og_state_buffer;
}

void std_og_draw_buffer(void) {
  SDL_UnlockTexture(std_og_state_buffer.tx);
  SDL_RenderCopy(std_og_state_renderer, std_og_state_buffer.tx, NULL, NULL);
}

void std_og_set_main_loop(bool (*callback)(void)) {
  std_og_internal_user_main_fn=callback;
  c_set_main_loop(std_og_internal_og_main_fn);
}

bool std_og_state_KeyState_get(std_og_state_KeyState *this, SDL_Scancode k) {
  return this->keys[((u32)k)];
}

bool std_og_state_MouseState_get_button(std_og_state_MouseState *this, u8 b) {
  return this->buttons[((u32)b)];
}

bool std_og_state_MouseState_get_wheel(std_og_state_MouseState *this, std_og_state_MouseWheel w) {
  return this->wheel_dirs[((u32)w)];
}

void std_og_state_update_window_size(void) {
  SDL_GetWindowSize(std_og_state_window, &std_og_state_window_size.x, &std_og_state_window_size.y);
}

void std_og_state_add_frametime(f32 frametime) {
  std_og_state_frametime_total+=(frametime - std_og_state_frametimes[std_og_state_frametime_index]);
  std_og_state_frametimes[std_og_state_frametime_index]=frametime;
  std_og_state_frametime_index=((std_og_state_frametime_index + 1) % std_og_state_FRAMETIMES_COUNT);
}

f32 std_og_state_get_avg_frametime(void) {
  return std_og_state_frametime_total / ((f32)std_og_state_FRAMETIMES_COUNT);
}

void std_og_utils_handle_sdl_events(void) {
  std_og_state_prev_mouse=std_og_state_mouse;
  std_og_state_prev_keys=std_og_state_keys;
  SDL_Event event = {0};
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT: {
        std_og_state_running=false;
      } break;
      case SDL_KEYDOWN: {
        SDL_Scancode k = event.key.keysym.scancode;
        if (((i32)k) < 1024) {
          std_og_state_keys.keys[((i32)k)]=true;
        }
      } break;
      case SDL_KEYUP: {
        SDL_Scancode k = event.key.keysym.scancode;
        if (((i32)k) < 1024) {
          std_og_state_keys.keys[((i32)k)]=false;
        }
      } break;
      case SDL_MOUSEBUTTONDOWN: {
        u8 b = event.button.button;
        std_og_state_mouse.buttons[((i32)b)]=true;
      } break;
      case SDL_MOUSEBUTTONUP: {
        u8 b = event.button.button;
        std_og_state_mouse.buttons[((i32)b)]=false;
      } break;
      case SDL_MOUSEWHEEL: {
        std_og_state_mouse.wheel_dirs[((i32)std_og_state_MouseWheel_Right)]=(event.wheel.x > 0);
        std_og_state_mouse.wheel_dirs[((i32)std_og_state_MouseWheel_Left)]=(event.wheel.x < 0);
        std_og_state_mouse.wheel_dirs[((i32)std_og_state_MouseWheel_Down)]=(event.wheel.y > 0);
        std_og_state_mouse.wheel_dirs[((i32)std_og_state_MouseWheel_Up)]=(event.wheel.y < 0);
      } break;
      default: {
      } break;
    }
  }
  SDL_GetMouseState(&std_og_state_mouse.pos.x, &std_og_state_mouse.pos.y);
  SDL_GetRelativeMouseState(&std_og_state_mouse.vec.x, &std_og_state_mouse.vec.y);
}

bool std_og_internal_og_main_fn(void) {
  if (!std_og_is_running()) {
    return false;
  }
  if (std_og_internal_user_main_fn != NULL) {
    return std_og_internal_user_main_fn();
  }
  return true;
}

