# Unreal Engine 4 Shooter Game Demo

The Unreal Engine Shooter Game demo with quality-of-life improvements. Improvements are licensed under [the MIT license](https://opensource.org/licenses/MIT)

# Setup

## Linux
1. [Build Unreal Engine 4 from source](https://www.ue4community.wiki/Legacy/Building_On_Linux#Building):
   ```
   $ git clone https://github.com/EpicGames/UnrealEngine.git
   $ # or if you are using ssh authentication:
   $ # git clone git@github.com:EpicGames/UnrealEngine.git
   $ cd UnrealEngine
   $ ./Setup.sh
   $ ./GenerateProjectFiles.sh
   $ make
   ```
2. Clone Shooter Game source
3. [Generate project files](https://www.ue4community.wiki/Legacy/Building_On_Linux#Generating_project_files_for_your_project):
   ```
   $ ./GenerateProjectFiles.sh -project="/path/to/ShooterGame/clone/ShooterGame.uproject" -game -engine
   ```
3. [Open the project in UEd](https://www.ue4community.wiki/Legacy/Building_On_Linux#Opening_your_project):
   ```
   $ ./UE4Editor "/path/to/ShooterGame/clone/ShooterGame.uproject"
   ```

## Windows
1. [Setup Visual Studio](https://docs.unrealengine.com/en-US/Programming/Development/VisualStudioSetup/index.html) (the Community edition works)
2. Download and launch Unreal Engine 4
3. Open the ShooterGame.uproject project
4. Click File->Generate Visual Studio Solution

# Command Line Options

`-vulkan`: Render the game using the Vulkan graphics API (supported on Linux and Windows)

`-dx11`: Render the game using the DX11 graphics API (supported on Windows only)

`-dx12`: Render the game using the DX12 graphics API (supported on Windows only)

`-benchmark`: Launch the Shooter game in benchmarking mode. In this mode, the game will launch directly into a Free-for-All match with no bots

`-timeout`: Exit benchmark after the specified number of seconds. Default: 60
