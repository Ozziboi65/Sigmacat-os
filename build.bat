@echo off
setlocal enabledelayedexpansion

echo [BUILD] Starting simplified GRUB/Multiboot build...


if "%CC%"=="" set "CC=i686-elf-gcc"
if "%LD%"=="" set "LD=i686-elf-ld"
if "%OBJCOPY%"=="" set "OBJCOPY=i686-elf-objcopy"

echo [BUILD] Using compiler: %CC%


set CFLAGS=-m32 -fno-exceptions -fno-rtti -fno-pie -ffreestanding -nostdlib
set CCOMPILE_FLAGS=%CFLAGS%


mkdir build 2>nul
mkdir build\iso\boot\grub 2>nul

echo building kernel entry
nasm -f elf32 src\Kernel\kernel_entry.asm -o build\kernel_entry.o || (
    echo [ERROR] Kernel entry assembly failed!
    exit /b 1
)

echo building C++ kernel files
%CC% %CCOMPILE_FLAGS% -c src\Kernel\KernelMain.cpp -o build\kernel.o || (
    echo [ERROR] C++ kernel compilation failed!
    exit /b 1
)

echo building Kernel Service
%CC% %CCOMPILE_FLAGS% -c src\Kernel\Kernel_Services.cpp -o build\Kernel_Services.o || (
    echo [ERROR] Kernel Services compilation failed!
    exit /b 1
)

echo building serial driver
%CC% %CCOMPILE_FLAGS% -c src\Kernel\Driver\Serial\serial_driver.cpp -o build\serial_driver.o || (
    echo [ERROR] Serial Driver compilation failed!
    exit /b 1
)

echo building PS2 Keyboard Driver
%CC% %CCOMPILE_FLAGS% -c src\Kernel\Driver\PS2Keyboard\Ps2KeyboardDriver.cpp -o build\ps2_driver.o || (
    echo [ERROR] PS2 Keyboard Driver compilation failed!
    exit /b 1
)

echo building PS2 Mouse Driver
%CC% %CCOMPILE_FLAGS% -c src\Kernel\Driver\PS2Mouse\Ps2MouseMain.cpp -o build\ps2_mouse.o || (
    echo [ERROR] PS2 Mouse Driver compilation failed!
    exit /b 1
)

echo building Driver Manager
%CC% %CCOMPILE_FLAGS% -c src\Kernel\Driver\DriverManager.cpp -o build\DriverManager.o || (
    echo [ERROR] Driver Manager compilation failed!
    exit /b 1
)

echo building PIT Timer Driver
%CC% %CCOMPILE_FLAGS% -c src\Kernel\Driver\Time\Pit\PitMain.cpp -o build\PitMain.o || (
    echo [ERROR] PIT Timer Driver compilation failed!
    exit /b 1
)

echo building interrupt stubs
nasm -f elf32 src\Kernel\interrupts\interrupt_stubs.asm -o build\interrupt_stubs.o || (
    echo [ERROR] Interrupt stubs assembly failed!
    exit /b 1
)

echo building interrupt system
%CC% %CCOMPILE_FLAGS% -c src\Kernel\interrupts\interruptsMain.cpp -o build\interrupts.o || (
    echo [ERROR] Interrupt system compilation failed!
    exit /b 1
)



echo building cmd line
%CC% %CCOMPILE_FLAGS% -c src\Os\cmdLine\main.cpp -o build\cmdline_main.o || (
    echo [ERROR] cmdLine compilation failed!
    exit /b 1
)

echo building graphic service
mkdir build\Graphics 2>nul
%CC% %CCOMPILE_FLAGS% -c src\Os\Services\Graphics\main.cpp -o build\Graphics\main.o || (
    echo [ERROR] Graphics service compilation failed!
    exit /b 1
)


echo building Port I/O module
%CC% %CCOMPILE_FLAGS% -c src\Kernel\IO\PortIO.cpp -o build\PortIO.o || (
    echo [ERROR] Port I/O module compilation failed!
    exit /b 1
)

echo building kernel input handler
%CC% %CCOMPILE_FLAGS% -c src\Kernel\Input\KernelInputHandler.cpp -o build\KernelInputHandler.o || (
    echo [ERROR] Kernel input handler compilation failed!
    exit /b 1
)

echo Linking kernel
%LD% -m elf_i386 -T src\Kernel\linker_grub.ld -nostdlib build\kernel_entry.o build\kernel.o build\Kernel_Services.o build\PortIO.o build\serial_driver.o build\ps2_driver.o build\ps2_mouse.o build\DriverManager.o build\PitMain.o build\interrupt_stubs.o build\interrupts.o build\Graphics\main.o build\cmdline_main.o build\KernelInputHandler.o -o build\kernel.elf || (
    echo [ERROR] Kernel linking failed!
    exit /b 1
)


copy /Y build\kernel.elf build\iso\boot\kernel.bin >nul
copy /Y grub\grub.cfg build\iso\boot\grub\grub.cfg >nul



if exist make_iso.bat (
    call make_iso.bat
)

endlocal
exit /b 0
