# AMD TressFX

![AMD TressFX](http://gpuopen-effects.github.io/media/effects/tressfx4_screenshot_medium.jpg)

The TressFX library implements AMD's TressFX hair/fur rendering and simulation technology. The TressFX technology uses the GPU to simulate and render high-quality, realistic hair and fur. TressFX makes use of the processing power of high-performance GPUs to do realistic rendering and utilizes DirectCompute to physically simulate each individual strand of hair.

<div>
  <a href="https://github.com/GPUOpen-Effects/TressFX/releases/latest/"><img src="http://gpuopen-effects.github.io/media/latest-release-button.svg" alt="Latest release" title="Latest release"></a>
</div>

Highlights include the following:
* GCN-optimized rendering and simulation
* Hair and fur (skinning) support with high quality anti-aliasing
* Two options for order-independent transparency
* Maya plugin provided for authoring
* Full source code provided

### Integrating TressFX

TressFX is our largest GPUOpen effect to date. Integration with a game engine for use in production is a substantial undertaking.  This release moves us a lot closer to what we've done in production for specific engine integrations.  You'll see some items (such as integration with a lighting system) will look more complicated than in previous releases. This release also tries to introduce some reusability across engines and APIs.  

We're aware that there are still plenty of improvements we can make for the sake of usability in production.  Some of the approaches to make this release more portable are experimental, and will potentially change in future releases.

### New in TressFX 4
* Hair is skinned directly, rather than through triangle stream-out.
* Signed distance field (SDF) collision, including compute shaders to generate the SDF from a dynamic mesh.
* New system for handling fast motion.
* Refactored to be more engine / API agnostic.
* Example code includes compute-based skinning and marching cubes generation.
* DirectX 12 support

### DirectX 12
Although DirectX 12 support is included, it hasn't been properly optimized.


### Prerequisites
* AMD Radeon&trade; GCN-based GPU (HD 7000 series or newer)
  * Or other DirectX&reg; 11 or DirectX 12 compatible discrete GPU with Shader Model 5 support
* 64-bit Windows&reg; 7 (SP1 with the [Platform Update](https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687.aspx)), Windows 8.1, or Windows 10
* Visual Studio&reg; 2013, Visual Studio 2015 or Visual Studio 2017
* Visual Studio 2015 or 2017, and Windows 10 required for DirectX 12

### Getting started

#### Running the demo
* Visual studio solutions for VS2015, VS2017, DX11 and DX12, can be found in the `amd_tressfx_sample\build` directory.
* There are also solutions for just the core library in the `amd_tressfx\build` directory.
* Change settings such as fullscreen/windowed and enable/disable D3D debug layer in `amd_tressfx_sample\bin\sushi.ini`
* Run `SushiD3D11.exe` or `SushiD3D12.exe` in `amd_tressfx_sample\bin`.

#### Where to find things
* Documentation is in the `doc` subdirectory.
* TressFX source code is in the `amd_tressfx` subdirectory.  
* The Maya plugin, written in python, is in `tools\Maya`.
* Example code and content is in the `amd_tressfx_sample` directory. `amd_tressfx_sample\src` shows how to use TressFX.


### Premake
The Visual Studio solutions and projects in this repo were generated with Premake. If you need to regenerate the Visual Studio files, double-click on `gpuopen_tressfx_update_vs_files.bat` in the `premake` directory.

This version of Premake has been modified from the stock version to use the property sheet technique for the Windows SDK from this [Visual C++ Team blog post](http://blogs.msdn.com/b/vcblog/archive/2012/11/23/using-the-windows-8-sdk-with-visual-studio-2010-configuring-multiple-projects.aspx). The technique was originally described for using the Windows 8.0 SDK with Visual Studio 2010, but it applies more generally to using newer versions of the Windows SDK with older versions of Visual Studio.

By default, Visual Studio 2013 projects will compile against the Windows 8.1 SDK. However, the VS2013 projects generated with this version of Premake will use the next higher SDK (i.e. the Windows 10 SDK), if the newer SDK exists on the user's machine.

For Visual Studio 2015 and 2017, the `systemversion` Premake function is used to add the `WindowsTargetPlatformVersion` element to the project file, to specify which version of the Windows SDK will be used. To change `WindowsTargetPlatformVersion` for Visual Studio 2015 and 2017, change the value for `WINDOWS_TARGET_SDK` in `amd_tressfx_sample\premake` and regenerate the Visual Studio files.

### Third-Party Software
* TressFX is distributed as source code, licensed under MIT.  It contains no third-party code.
* Premake is used to generate project files.  It is distributed under the terms of the BSD License. See `premake\LICENSE.txt`.
* The sample makes use of some third-party code.  See `amd_tressfx_sample\prebuilt\LICENSE.txt` for further details.

### Previous releases
TressFX 4 is a big change from the prior 3.1.1 release. A separate
branch for 3.1.1 has been created for the convenience of users that have
been working with TressFX 3.
* [TressFX 3.1.1 branch](https://github.com/GPUOpen-Effects/TressFX/tree/3.1.1)

### Attribution
* AMD, the AMD Arrow logo, Radeon, and combinations thereof are either registered trademarks or trademarks of Advanced Micro Devices, Inc. in the United States and/or other countries.
* Microsoft, DirectX, Visual Studio, and Windows are either registered trademarks or trademarks of Microsoft Corporation in the United States and/or other countries.
