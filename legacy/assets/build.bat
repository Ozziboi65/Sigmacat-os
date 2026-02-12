@echo off
REM ========================================
REM OS Build Script for Windows
REM Supports: Assembly OR C kernel
REM Set KERNEL_TYPE=asm or c
REM ========================================

REM Configuration: Change to 'c' to build C kernel, 'asm' for assembly kernel
set KERNEL_TYPE=c
REM Enable linking with a standard C library if your toolchain provides one (0 or 1)
set USE_STD_LIB=1


fastfetch

REM Toolchain: prefer i686-elf cross compiler, otherwise fall back
if exist "C:\elf\bin\i686-elf-gcc.exe" (
    set "CC=i686-elf-gcc"
    set "LD=i686-elf-ld"
    set "OBJCOPY=i686-elf-objcopy"
    set "LINKER_MODE=elf"
) else if exist "C:\i686-elf-tools\bin\i686-elf-gcc.exe" (
    set "CC=i686-elf-gcc"
    set "LD=i686-elf-ld"
    set "OBJCOPY=i686-elf-objcopy"
    set "LINKER_MODE=elf"
) else (
    if "%CC%"=="" set "CC=gcc"
    if "%LD%"=="" set "LD=ld"
    if "%OBJCOPY%"=="" set "OBJCOPY=objcopy"
    set "LINKER_MODE=pe"
)

REM Common compile flags (can be extended). When USE_STD_LIB==1 we allow
REM access to standard C library headers (Newlib); otherwise keep bare-metal.
set CFLAGS=-m32 -fno-exceptions -fno-rtti -fno-pie -ffreestanding -nostdlib
if "%USE_STD_LIB%"=="1" (
    set CCOMPILE_FLAGS=%CFLAGS% -isystem C:\elf\i686-elf\include
    set LINK_LIBS=-lc -lm
) else (
    set CCOMPILE_FLAGS=%CFLAGS%
    set LINK_LIBS=
)

REM FORCE bare-metal mode - disable all C library linking
set USE_STD_LIB=0
set CCOMPILE_FLAGS=%CFLAGS%
set LINK_LIBS=

echo [BUILD] Starting OS build process (Kernel Type: %KERNEL_TYPE%)...
echo [BUILD] Using compiler: %CC%
echo [BUILD] Linker mode: %LINKER_MODE%



REM Create build directory
if not exist "build" mkdir build

REM ========================================
REM Step 1: Assemble the bootloader
REM ========================================
echo [BUILD] Assembling bootloader...
nasm -f bin src\Bootloader\main_bootloader.asm -o build\bootloader.bin
if errorlevel 1 (
    echo [ERROR] Bootloader assembly failed!
    exit /b 1
)

REM ========================================
REM Build kernel based on type
REM ========================================


if "%KERNEL_TYPE%"=="c" goto build_c
echo [ERROR] Invalid KERNEL_TYPE. Use 'asm' or 'c'
exit /b 1



:build_c
echo [BUILD] Assembling kernel entry point...

if "%LINKER_MODE%"=="elf" (
    nasm -f elf32 src\Kernel\kernel_entry.asm -o build\kernel_entry.o
    if errorlevel 1 (
        echo [ERROR] Kernel entry assembly failed!
        exit /b 1
    )
    set KERNELOBJ=build\kernel_entry.o
) else (
    nasm -f win32 src\Kernel\kernel_entry.asm -o build\kernel_entry.obj
    if errorlevel 1 (
        echo [ERROR] Kernel entry assembly failed!
        exit /b 1
    )
    set KERNELOBJ=build\kernel_entry.obj
)

echo [BUILD] Compiling C++ kernel files...
%CC% %CCOMPILE_FLAGS% -c src\Kernel\KernelMain.cpp -o build\kernel.o
if errorlevel 1 (
    echo [ERROR] C++ kernel compilation failed!
    exit /b 1
)

echo [BUILD] Compiling Kernel Services...
%CC% %CCOMPILE_FLAGS% -c src\Kernel\Kernel_Services.cpp -o build\Kernel_Services.o
if errorlevel 1 (
    echo [ERROR] Kernel Services compilation failed!
    exit /b 1
)

echo [BUILD] Compiling Serial Driver...
%CC% %CCOMPILE_FLAGS% -c src\Kernel\Driver\Serial\serial_driver.cpp -o build\serial_driver.o
if errorlevel 1 (
    echo [ERROR] Serial Driver compilation failed!
    exit /b 1
)

echo [BUILD] Compiling PS2 Keyboard Driver...
%CC% %CCOMPILE_FLAGS% -c src\Kernel\Driver\PS2Keyboard\Ps2KeyboardDriver.cpp -o build\ps2_driver.o
if errorlevel 1 (
    echo [ERROR] PS2 Keyboard Driver compilation failed!
    exit /b 1
)

echo [BUILD] Compiling Driver Manager...
%CC% %CCOMPILE_FLAGS% -c src\Kernel\Driver\DriverManager.cpp -o build\DriverManager.o
if errorlevel 1 (
    echo [ERROR] Driver Manager compilation failed!
    exit /b 1
)




echo [BUILD] Compiling cmdLine...
%CC% %CCOMPILE_FLAGS% -c src\Os\cmdLine\main.cpp -o build\cmdline_main.o
if errorlevel 1 (
    echo [ERROR] cmdLine compilation failed!
    exit /b 1
)

echo [BUILD] Compiling kernel input handler...
%CC% %CCOMPILE_FLAGS% -c src\Kernel\KernelInputHandler.cpp -o build\KernelInputHandler.o
if errorlevel 1 (
    echo [ERROR] Driver Manager compilation failed!
    exit /b 1
)

echo [BUILD] Linking kernel with linker script and converting to flat binary...
%LD% -m elf_i386 -T src\Kernel\linker.ld -nostdlib %KERNELOBJ% build\kernel.o build\Kernel_Services.o build\serial_driver.o build\ps2_driver.o build\DriverManager.o build\cmdline_main.o build\KernelInputHandler.o -o build\kernel.bin
if errorlevel 1 (
    echo [ERROR] Kernel linking failed!
    exit /b 1
)
goto build_done

:build_done

REM ========================================
REM Step 3: Create the OS image
REM ========================================
echo [BUILD] Creating OS image...
copy /b build\bootloader.bin + build\kernel.bin build\os-image.bin > nul
if errorlevel 1 (
    echo [ERROR] Image creation failed!
    exit /b 1
)

REM ========================================
REM Step 4: Create 1.44MB floppy image for VMware
REM ========================================
echo [BUILD] Creating floppy...
REM Delete old floppy image first, then create fresh 1.44MB file
if exist build\os-floppy.flp del build\os-floppy.flp
fsutil file createnew build\os-floppy.flp 1474560 > nul
REM Use PowerShell to write os-image.bin at start of floppy without truncating
powershell -Command "$floppy = [System.IO.File]::Open('build\os-floppy.flp', 'Open', 'Write'); $data = [System.IO.File]::ReadAllBytes('build\os-image.bin'); $floppy.Write($data, 0, $data.Length); $floppy.Close()"

echo.
echo [BUILD] Build completed successfully!
echo [BUILD] Output: build\os-image.bin
echo [BUILD] FLOPPY: build\os-floppy.flp


exit /b 0
