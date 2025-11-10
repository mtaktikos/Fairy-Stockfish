@echo off

mkdir .\Windows
for /f "tokens=2 delims==" %%a in ('wmic OS Get localdatetime /value') do set "dt=%%a"
set "YY=%dt:~2,2%" & set "YYYY=%dt:~0,4%" & set "MM=%dt:~4,2%" & set "DD=%dt:~6,2%"
set "datestamp=%YYYY%%MM%%DD%"

:menu
echo    EXECUTABLES
echo    ===========
echo.
echo    1 - All
echo    2 - w32
echo    3 - x64
echo    4 - x64 modern
echo    5 - x64 avx2
echo.
echo    Q - Quit
choice /c:12345Q>nul

if errorlevel 6 goto done
if errorlevel 5 goto avx2
if errorlevel 4 goto modern
if errorlevel 3 goto x64
if errorlevel 2 goto w32
if errorlevel 1 goto all
echo CHOICE missing
goto done

:all
cd .\src
make -j profile-build ARCH=x86-64-bmi2 COMP=mingw
strip stockfish.exe
cd ..
move /-y ".\src\stockfish.exe" ".\Windows\SFFairy_x64_bmi2.exe"

cd .\src
make clean

cd .\src
make -j build ARCH=x86-64-modern COMP=mingw
strip stockfish.exe
cd ..
move /-y ".\src\stockfish.exe" ".\Windows\SFFairy_x64_modern.exe"

cd .\src
make clean

cd .\src
make -j profile-build ARCH=x86-64 COMP=mingw
strip stockfish.exe
cd ..
move /-y ".\src\stockfish.exe" ".\Windows\SFFairy_x64.exe"

cd .\src
make clean

cd .\src
make -j profile-build ARCH=x86-32 COMP=mingw
strip stockfish.exe
cd ..
move /-y ".\src\stockfish.exe" ".\Windows\SFFairy_w32.exe"

cd .\src
make clean
goto done


:avx2
cd .\src
make -j build ARCH=x86-64-avx2 COMP=mingw
strip stockfish.exe
cd ..
move /-y ".\src\stockfish.exe" ".\Windows\SFFairy_x64_avx2.exe"

cd .\src
make clean
goto done


:modern
cd .\src
make -j build ARCH=x86-64-modern COMP=mingw
strip stockfish.exe
cd ..
move /-y ".\src\stockfish.exe" ".\Windows\SFFairy_x64_modern.exe"

cd .\src
make clean
goto done


:x64
cd .\src
make -j build ARCH=x86-64 COMP=mingw
strip stockfish.exe
cd ..
move /-y ".\src\stockfish.exe" ".\Windows\SFFairy_x64.exe"

cd .\src
make clean
goto done


:w32
cd .\src
make -j profile-build ARCH=x86-32 COMP=mingw
strip stockfish.exe
cd ..
move /-y ".\src\stockfish.exe" ".\Windows\SFFairy_w32.exe"

cd .\src
make clean
goto done

:done