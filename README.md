# Unreal Engine 4 Shooter Game Demo

The Unreal Engine Shooter Game demo with quality-of-life improvements. Improvements are licensed under [the MIT license](https://opensource.org/licenses/MIT)

# Setup

## Linux
1. [Build Unreal Engine 4 from source](https://wiki.unrealengine.com/Building_On_Linux#Building)
2. [Generate project files](https://wiki.unrealengine.com/Building_On_Linux#Generating_project_files_for_your_project)
3. [Open project in UEd](https://wiki.unrealengine.com/Building_On_Linux#Opening_your_project)

## Windows
1. [Setup Visual Studio](https://docs.unrealengine.com/en-US/Programming/Development/VisualStudioSetup/index.html) (the Community edition works)
2. Download and launch Unreal Engine 4
3. Open the ShooterGame.uproject project
4. Click File->Generate Visual Studio Solution

# Command Line Options

_-vulkan_: Render the game using the Vulkan graphics API (supported on Linux and Windows)

_-dx11:_ Render the game using the DX11 graphics API (supported on Windows only)

_-benchmark:_ Launch the Shooter game in benchmarking mode. In this mode, the game will launch directly into a Free-for-All match with no bots, then automatically after 60 seconds.
