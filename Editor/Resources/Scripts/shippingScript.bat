call "D:\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
devenv "..\NIX_Engine.sln" /Build Game_Shipping_Release
ren "..\x64\Game_Shipping_Release\Editor.exe" "DemoNIX.exe"
move "..\x64\Game_Shipping_Release" "F:\Projet-Git Ecole\EnginPasReel\Build"
