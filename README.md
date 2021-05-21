# Unreal Engine 4 Shooter Game Demo

The Unreal Engine Shooter Game demo with quality-of-life improvements. Improvements are licensed under the [MIT license](https://opensource.org/licenses/MIT).

## Command Line Options

`-vulkan`: Render the game using the Vulkan graphics API (supported on Linux and Windows)

`-dx11`: Render the game using the DX11 graphics API (supported on Windows only)

`-dx12`: Render the game using the DX12 graphics API (supported on Windows only)

`-benchmark`: Launch the Shooter game in benchmarking mode. In this mode, the game will launch directly into a Free-for-All match with no bots

`-timeout`: Exit benchmark after the specified number of seconds. Default: 60

## Releases

Prebuilt binary packages for Linux and Windows are available on the [GitHub Releases page](https://github.com/cqcallaw/shootergame/releases).

## Linux Build Setup

1. Clone Unreal Engine 4 from source:

   ```bash
   mkdir -p ~/src && cd src
   git clone https://github.com/EpicGames/UnrealEngine.git
   # or if you are using ssh authentication:
   # git clone git@github.com:EpicGames/UnrealEngine.git
   cd UnrealEngine
   # checkout target engine version
   git checkout 4.26.0-release
   ```

2. Run initial engine setup:

   ```bash
   ./Setup.sh
   ./GenerateProjectFiles.sh
   make
   ```

3. Clone Shooter Game source:

   ```bash
   cd ~/src
   # make sure git-lfs is initialized; see https://www.atlassian.com/git/tutorials/git-lfs#installing-git-lfs
   git lfs install
   git clone https://github.com/cqcallaw/shootergame.git
   ```

4. [Generate project files](https://www.ue4community.wiki/Legacy/Building_On_Linux#Generating_project_files_for_your_project):

   ```bash
   ~/src/UnrealEngine/GenerateProjectFiles.sh -project ~/src/shootergame/ShooterGame.uproject -game -engine
   ```

5. Run initial build engine (without which, the Unreal Editor complains about changing engine files):

   ```bash
   ~/src/UnrealEngine/Engine/Build/BatchFiles/Linux/RunMono.sh ~/src/UnrealEngine/Engine/Binaries/DotNET/UnrealBuildTool.exe Development Linux -Project ~/src/shootergame/ShooterGame.uproject -TargetType=Editor -Progress
   ```

6. [Open the project in UEd](https://www.ue4community.wiki/Legacy/Building_On_Linux#Opening_your_project):

   ```bash
   ~/src/UnrealEngine/Engine/Binaries/Linux/UE4Editor ~/src/shootergame/ShooterGame.uproject
   ```

## Windows Build Setup

1. [Setup Visual Studio](https://docs.unrealengine.com/en-US/Programming/Development/VisualStudioSetup/index.html) (the Community edition works)
2. Download and launch Unreal Engine 4
3. Clone the ShooterGame project (Git LFS is required; Github Desktop works well)
4. Open the ShooterGame.uproject project
5. Click File->Generate Visual Studio Solution

## Build FAQ

* Why does time run too fast in-game?
  * Non-Shipping and builds that without complete lighting can exhibit this behavior. Try a lighting rebuild and building a Shipping binary.
* Why is the player start position random?
  * Verify that benchmark mode has been enabled with the `-benchmark` command-line argument. Without this flag, the game behaves as normal Shooter Game demo.
