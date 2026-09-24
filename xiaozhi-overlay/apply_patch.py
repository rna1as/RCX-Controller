#!/usr/bin/env python3
from pathlib import Path
import sys

root = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()
kconfig_path = root / "main" / "Kconfig.projbuild"
cmake_path = root / "main" / "CMakeLists.txt"

kconfig = kconfig_path.read_text(encoding="utf-8")
if "BOARD_TYPE_RCX_JARVIS_N16R8" not in kconfig:
    marker = '    config BOARD_TYPE_BREAD_COMPACT_WIFI\n'
    insert = (
        '    config BOARD_TYPE_RCX_JARVIS_N16R8\n'
        '        bool "RCX Jarvis ESP32-S3 N16R8"\n'
        '        depends on IDF_TARGET_ESP32S3\n'
    )
    if marker not in kconfig:
        raise SystemExit("Kconfig marker not found; upstream layout changed")
    kconfig = kconfig.replace(marker, insert + marker, 1)
    kconfig_path.write_text(kconfig, encoding="utf-8")

cmake = cmake_path.read_text(encoding="utf-8")
if "CONFIG_BOARD_TYPE_RCX_JARVIS_N16R8" not in cmake:
    marker = 'if(CONFIG_BOARD_TYPE_BREAD_COMPACT_WIFI)\n'
    insert = (
        'if(CONFIG_BOARD_TYPE_RCX_JARVIS_N16R8)\n'
        '    set(BOARD_DIR "rcx-jarvis-n16r8")\n'
        'elseif(CONFIG_BOARD_TYPE_BREAD_COMPACT_WIFI)\n'
    )
    if marker not in cmake:
        raise SystemExit("CMake marker not found; upstream layout changed")
    cmake = cmake.replace(marker, insert, 1)
    cmake_path.write_text(cmake, encoding="utf-8")

print("RCX Jarvis N16R8 board patch applied")
