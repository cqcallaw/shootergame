@echo off
if [%1] == [] goto :Usage

set Platform=%1
set DeployDir=ShooterServerBuild
set ExeFile=ShooterServer
set UE4Dir=UE4

set PakListFile=../../../ShooterGame/Build/ShooterGameServer_PakList.txt
set PakFileName=ShooterServer.pak
set PakCreatePath=../../../ShooterGame/Content/Paks/%PakFileName%
set PakCopyPath=..\%UE4Dir%\ShooterGame\Content\Paks\%PakFileName%

call Deploy.cmd
exit

:Usage
echo Usage: %~nx0 ^<Platform^>
exit

