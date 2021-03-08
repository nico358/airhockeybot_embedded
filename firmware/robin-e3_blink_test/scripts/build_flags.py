# taken from
# https://github.com/makerbase-mks/MKS-Robin-E3-E3D/tree/master/firmware/marlin2.0%20for%20Ender3/Marlin/src/HAL/STM32F1

from __future__ import print_function
import sys

#dynamic build flags for generic compile options
if __name__ == "__main__":
  args = " ".join([ "-std=gnu11",
                    "-Os",
                    "-mcpu=cortex-m3",
                    "-mthumb",

                    "-fsigned-char",
                    "-fno-move-loop-invariants",
                    "-fno-strict-aliasing",

                    "--specs=nano.specs",
                    "--specs=nosys.specs",
                    "-Iinclude",

                    "-MMD",
                    "-MP",
                    "-DTARGET_STM32F1"
                  ])

  for i in range(1, len(sys.argv)):
    args += " " + sys.argv[i]

  print(args)

# extra script for linker options
else:
  from SCons.Script import DefaultEnvironment
  env = DefaultEnvironment()
  env.Append(
      ARFLAGS=["rcs"],

      ASFLAGS=["-x", "assembler-with-cpp"],

      CXXFLAGS=[
          "-fabi-version=0",
          "-fno-use-cxa-atexit",
          "-fno-threadsafe-statics"
      ],
      LINKFLAGS=[
          "-Os",
          "-mcpu=cortex-m3",
          "-ffreestanding",
          "-mthumb",
          "--specs=nano.specs",
          "--specs=nosys.specs",
          "-u_printf_float",
      ],
  )
