#ifndef BUILD_H
#define BUILD_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#  include <unistd.h>
#elif _WIN32
#  define _CRT_SECURE_NO_WARNINGS
#  define _CRT_NONSTDC_NO_WARNINGS
#  include <process.h>
#endif

#include <assert.h>

static int compile_common();
static int link_exe();
static int compile_and_link_exe() {
  if (compile_common()) return 1;
  if (link_exe()) return 1;
  return 0;
}

int run(char ** args) {
  assert(args && args[0]);

#ifdef __APPLE__
  pid_t pid = fork();
  if (pid == 0) {
    execvp(args[0], args);
    abort();
  } else if (pid > 0) {
    int sl = 0;
    assert(0 <= waitpid(pid, &sl, 0));
    if (WIFEXITED(sl)) return WEXITSTATUS(sl);
  }
#elif _WIN32
  if (0 == _spawnvp(_P_WAIT, args[0], (const char * const *)args)) {
    return 0;
  }
#endif

  fprintf(stderr, "failed to run child process: %s\n", args[0]);
  return 1;
}
#define RUN(...) do { char * args[] = { __VA_ARGS__, 0 }; if (run(args)) return 1; } while (0)

#define CC(src, o, ...) RUN("clang", "-Wall", __VA_ARGS__, "-o", o, "-c", src)
#define HDR(src, o, ...) CC(src, o, "-x", "c", __VA_ARGS__)

#define SHADER(src, fld) RUN("glslang", "-V", src, "-o", fld "/" src ".spv")

#define ENGS \
  ENG("gl_main")        \
  ENG("gl_pick")        \
  ENG("lg_main")        \
  ENG("mp_friends")     \
  ENG("mp_match")       \
  ENG("mp_matches")     \
  ENG("mp_player")      \
  ENG("mp_status")      \
  ENG("sc_base")        \
  ENG("sc_curmatch")    \
  ENG("sc_game")        \
  ENG("sc_game_end")    \
  ENG("sc_game_repeat") \
  ENG("sc_load")        \
  ENG("sc_main")        \
  ENG("sc_menu")        \
  ENG("sc_newmatch")    \
  ENG("ui_main")        \

#define ENG(x) CC("eng/"x".c", "out/"x".o", CFLAGS);
static int compile_common() {
  ENGS
  //HDR("gme.h", "gme.o", CFLAGS, "-D", "GME_IMPLEMENTATION");
  //HDR("sfx.h", "sfx.o", CFLAGS, "-D", "SFX_IMPLEMENTATION");
  //HDR("snd.h", "snd.o", CFLAGS, "-D", "SND_IMPLEMENTATION");
  //HDR("snk.h", "snk.o", CFLAGS, "-D", "SNK_IMPLEMENTATION");
  //HDR("tmr.h", "tmr.o", CFLAGS, "-D", "TMR_IMPLEMENTATION");
  return 0;
}
#undef ENG

static int shaders() {
  //SHADER("boav.frag", RES_PATH);
  //SHADER("boav.vert", RES_PATH);
  return 0;
}

#define ENG(x) "out/"x".o"
#define OBJS ENGS

#endif
