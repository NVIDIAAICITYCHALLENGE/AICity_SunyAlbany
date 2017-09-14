@set HGTRACK_BUILD=D:\CODE\HGTrack\hgtrack_build_vc12x64
@set EXEC=mt_tracker_example.exe
@REM DEBUG_RELEASE: RelWithDebInfo,Debug,Release
@set DEBUG_RELEASE=Debug
@REM COPYEXE: Yes,No,All
@set COPYEXE=Yes
@if not exist %DEBUG_RELEASE% mkdir %DEBUG_RELEASE%

PATH=OpenCV_2_4_13_Dlls_x64;%PATH%

@if %COPYEXE%==All (
  xcopy /D %HGTRACK_BUILD%\bin\RelWithDebInfo\%EXEC% RelWithDebInfo\ /y
  xcopy /D %HGTRACK_BUILD%\bin\Debug\%EXEC% Debug\ /y
  xcopy /D %HGTRACK_BUILD%\bin\Release\%EXEC% Release\ /y
  goto :END
)
@if %COPYEXE%==Yes (
  xcopy /D %HGTRACK_BUILD%\bin\%DEBUG_RELEASE%\%EXEC% %DEBUG_RELEASE%\ /y
)

%DEBUG_RELEASE%\%EXEC% GraphMultiCarTracker_Config.txt
:END
@pause