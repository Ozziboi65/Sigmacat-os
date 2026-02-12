@echo off
setlocal enabledelayedexpansion
REM ========================================
REM Automated ISO Build Script
REM Detects and uses available GRUB tools
REM ========================================

echo [ISO-BUILD] Creating bootable ISO with GRUB...
echo.

REM First check if kernel is built
if not exist "build\iso\boot\kernel.bin" (
    echo kernel not found
    exit /b 1
)

if not exist "build\iso\boot\grub\grub.cfg" (
    echo no grub config
    exit /b 1
)





REM Method 1: Try WSL
wsl --list >nul 2>nul
if !errorlevel! equ 0 (
    echo [ISO-BUILD] Found WSL - Checking for GRUB tools...
    wsl which grub-mkrescue >nul 2>nul
    if !errorlevel! equ 0 (
        echo [ISO-BUILD] Using WSL with GRUB tools
        wsl bash -c "cd /mnt/c/projects/code/OS && grub-mkrescue -o build/os.iso build/iso 2>&1"
        if !errorlevel! equ 0 (
            echo.
            echo [ISO-BUILD] Success! ISO created
            exit /b 0
        ) else (
            echo [WARNING] WSL ISO creation failed
        )
    ) else (
        echo [INFO] WSL found but GRUB tools not installed
        echo [INFO] To install: wsl sudo apt-get install -y grub-pc-bin grub-common xorriso
    )
)

REM Method 2: Try MSYS2
if exist "C:\msys64\usr\bin\bash.exe" (
    echo [ISO-BUILD] Found MSYS2 - Checking for GRUB tools...
    C:\msys64\usr\bin\bash.exe -c "which grub-mkrescue" >nul 2>nul
    if !errorlevel! equ 0 (
        echo [ISO-BUILD] Using MSYS2 with GRUB tools
        C:\msys64\usr\bin\bash.exe -c "cd /c/projects/code/OS && grub-mkrescue -o build/os.iso build/iso 2>&1"
        if !errorlevel! equ 0 (
            echo.
            echo [ISO-BUILD] Success! ISO created at build\os.iso
            echo [ISO-BUILD] To test: qemu-system-i386 -cdrom build\os.iso
            exit /b 0
        ) else (
            echo [WARNING] MSYS2 ISO creation failed
        )
    ) else (
        echo [INFO] MSYS2 found but GRUB tools not installed
        echo [INFO] To install: pacman -S grub xorriso mtools
    )
)

REM Method 3: Try Cygwin
if exist "C:\cygwin64\bin\bash.exe" (
    echo [ISO-BUILD] Found Cygwin - Checking for GRUB tools...
    C:\cygwin64\bin\bash.exe -c "which grub-mkrescue" >nul 2>nul
    if !errorlevel! equ 0 (
        echo [ISO-BUILD] Using Cygwin with GRUB tools
        C:\cygwin64\bin\bash.exe -c "cd /cygdrive/c/projects/code/OS && grub-mkrescue -o build/os.iso build/iso 2>&1"
        if !errorlevel! equ 0 (
            echo.
            echo [ISO-BUILD] Success! ISO created at build\os.iso
            echo [ISO-BUILD] To test: qemu-system-i386 -cdrom build\os.iso
            exit /b 0
        ) else (
            echo [WARNING] Cygwin ISO creation failed
        )
    ) else (
        echo [INFO] Cygwin found but GRUB tools not installed
        echo [INFO] Install grub and xorriso packages via Cygwin setup
    )
)





endlocal
exit /b 1
