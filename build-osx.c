#define CFLAGS "-g", "-IVulkan-Headers/include"
#define RES_PATH "boas.app/Contents/Resources"
#include "build.h"

#include <sys/stat.h>

static int link_exe() {
  RUN("clang", "-Wall",
    "-framework", "AppKit",
    "-framework", "AudioToolbox",
    "-framework", "MetalKit",
    "-o", "memory-fighter.app/Contents/MacOS/memory-fighter", 
    OBJS); //, "vulkan-osx.o");
  return 0;
}

int main(int argc, char ** argv) {
  mkdir("out", 0777);
  mkdir("memory-fighter.app", 0777);
  mkdir("memory-fighter.app/Contents", 0777);
  mkdir("memory-fighter.app/Contents/MacOS", 0777);
  mkdir("memory-fighter.app/Contents/Resources", 0777);

  RUN("cp", "libvulkan.dylib", "memory-fighter.app/Contents/MacOS/");

  //CC("vulkan-osx.m", "vulkan-osx.o", CFLAGS);
  if (compile_and_link_exe()) return 1;
  if (shaders()) return 1;

  return 0;
}
