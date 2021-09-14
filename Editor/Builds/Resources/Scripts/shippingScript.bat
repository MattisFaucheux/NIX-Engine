call "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
devenv "..\Engine.sln" /Build Game_Shipping_Release
ren ..\x64\Game_Shipping_Release\Editor.exe Build.exe 
del Builds
move ..\x64\Game_Shipping_Release Builds
