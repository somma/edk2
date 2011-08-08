################################################################
# Boilerplate text to be included by any DSC file using the
# Standard Libraries.
#
# The including DSC file must DEFINE the EMULATE macro if
# the application is to be run in an emulation environment.
################################################################

[LibraryClasses.Common.UEFI_APPLICATION]
  #
  # C Standard Libraries
  #
  LibC|StdLib/LibC/LibC.inf
  LibCType|StdLib/LibC/Ctype/Ctype.inf
  LibLocale|StdLib/LibC/Locale/Locale.inf
  LibMath|StdLib/LibC/Math/Math.inf
  LibSignal|StdLib/LibC/Signal/Signal.inf
  LibStdio|StdLib/LibC/Stdio/Stdio.inf
  LibStdLib|StdLib/LibC/StdLib/StdLib.inf
  LibString|StdLib/LibC/String/String.inf
  LibTime|StdLib/LibC/Time/Time.inf
  LibUefi|StdLib/LibC/Uefi/Uefi.inf
  LibWchar|StdLib/LibC/Wchar/Wchar.inf

  # Common Utilities for Networking Libraries
  LibNetUtil|StdLib/LibC/NetUtil/NetUtil.inf

# Additional libraries for POSIX functionality.
  LibErr|StdLib/PosixLib/Err/LibErr.inf
  LibGen|StdLib/PosixLib/Gen/LibGen.inf
  LibGlob|StdLib/PosixLib/Glob/LibGlob.inf
  LibStringlist|StdLib/PosixLib/Stringlist/LibStringlist.inf

  # Libraries for device abstractions within the Standard C Library
  # Applications should not directly access any functions defined in these libraries.
  LibGdtoa|StdLib/LibC/gdtoa/gdtoa.inf
  DevConsole|StdLib/LibC/Uefi/Devices/daConsole.inf
  DevShell|StdLib/LibC/Uefi/Devices/daShell.inf
  DevUtility|StdLib/LibC/Uefi/Devices/daUtility.inf

###
# Select the correct TimerLib instance depending upon whether running under
# an emulation environment, or not.
!ifndef $(EMULATE)
  # Not running in an Emulation Environment
[LibraryClasses.IA32.UEFI_APPLICATION]
  TimerLib|PerformancePkg/Library/DxeTscTimerLib/DxeTscTimerLib.inf

[LibraryClasses.X64.UEFI_APPLICATION]
  TimerLib|PerformancePkg/Library/DxeTscTimerLib/DxeTscTimerLib.inf

[LibraryClasses.IPF.UEFI_APPLICATION]
  PalLib|MdePkg/Library/UefiPalLib/UefiPalLib.inf
  TimerLib|MdePkg/Library/SecPeiDxeTimerLibCpu/SecPeiDxeTimerLibCpu.inf

!else
  # Running in an Emulation Environment.
  # Adds this to the end of the current [LibraryClasses.Common.UEFI_APPLICATION] section.
[LibraryClasses.Common.UEFI_APPLICATION]
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf
!endif

[Components]
# BaseLib and BaseMemoryLib need to be built with the /GL- switch when using the Microsoft
# tool chain.  This is required so that the library functions can be resolved during
# the second pass of the linker during Link-time-code-generation.
###
  MdePkg/Library/BaseLib/BaseLib.inf {
    <BuildOptions>
      MSFT:*_*_*_CC_FLAGS    = /X /Zc:wchar_t /GL-
  }

  MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf {
    <BuildOptions>
      MSFT:*_*_*_CC_FLAGS    = /X /Zc:wchar_t /GL-
  }

##############################################################################
#
# See the additional comments below if you plan to run applications under an
# emulation environment.
#

[BuildOptions]
!ifndef $(EMULATE)
  # These Build Options are used when building the Standard Libraries to be run
  # on real hardware.
  INTEL:*_*_*_CC_FLAGS      = /Qfreestanding
   MSFT:*_*_*_CC_FLAGS      = /X /Zc:wchar_t
    GCC:*_*_*_CC_FLAGS      = -nostdinc -nostdlib

!else
  # The Build Options, below, are only used when building the Standard Libraries
  # to be run under an emulation environment.  They disable the clock() system call
  # which is currently incompatible with the most emulation environments.
  # Select the correct TimerLib instance, above.
  INTEL:*_*_IA32_CC_FLAGS     = /Od /D NT32dvm
   MSFT:*_*_IA32_CC_FLAGS     = /Od /D NT32dvm
    GCC:*_*_IA32_CC_FLAGS     = -O0 -DNT32dvm
!endif