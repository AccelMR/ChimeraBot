# ChimeraBot

ChimeraBot is a Discord bot, written in C++20 and using the Discord++ (DPP) library as its primary dependency.

## Installation

Here are the steps to get the ChimeraBot up and running:

<br />

### Prerequisites

The ChimeraBot requires the [vcpkg](https://github.com/Microsoft/vcpkg) package manager to manage its dependencies.

Please follow the [official vcpkg installation instructions](https://github.com/microsoft/vcpkg#quick-start) to install vcpkg on your machine.

<br />

### Environment Setup

After you have installed vcpkg, you need to set the `VCPKG_ROOT` environment variable. This step is crucial because it helps the system know where to link to the DPP library.

You can set `VCPKG_ROOT` to the location where you installed vcpkg.

For example, in Windows, you can use:

```powershell
[System.Environment]::SetEnvironmentVariable('VCPKG_ROOT','C:\path\to\vcpkg', [System.EnvironmentVariableTarget]::User)
```

In Linux, add the following line to your ~/.bashrc or ~/.zshrc file:
```bash
export VCPKG_ROOT=/path/to/vcpkg
```
<br />
<br />

## Linking to DPP
For information on how to link to the DPP library, see the [official DPP documentation](https://dpp.dev/install-vcpkg.html).

<br />
<br />

## Running ChimeraBot
To be completed with instructions on how to run the bot after installation is complete.

<br />
<br />

# Contributing
