ATG Samples Content Exporter
----------------------------

Copyright (c) Microsoft Corporation. All rights reserved.

November 20, 2015

The Samples Content Exporter was originally shipped as a sample in the legacy DirectX SDK. It makes use of the Autodesk FBX SDK
to import an FBX file and then export the data as an SDKMESH. The SDKMESH format is a runtime geometry format supported
by the DirectX Tool Kit and DXUT, and is used in the Windows 8 Store app sample Marble Maze.

The source is written for Visual Studio 2012 or 2013. It is recommended that you make use of the Windows 8.1 SDK
and Windows 7 Service Pack 1 or later.

These components are designed to work without requiring any content from the DirectX SDK. For details,
see "Where is the DirectX SDK?" <http://msdn.microsoft.com/en-us/library/ee663275.aspx>.

    ImportFBX\
        Contains the main entry point for the command-line exporter tool and code for capturing data from FBX files.

    ExportObjects\
        Contains a library of support code for the exporter.

    SDKMeshFileWriter\
        Contains a library of code for writing out an .SDKMESH file from data captured as export objects.

    XATGFileWriter
        Contains a library of code for writing out a .XATG file from data captured as export objects.

    DirectXMesh\
        This contains the DirectXMesh geometry processing library that is used instead of legacy D3DX9 for computing
        tangent frames and face adjacency.

        http://go.microsoft.com/fwlink/?LinkID=324981

    DirectXTex\
        This contains the DirectXTex texture processing library that is used instead of legacy D3DX9 for loading textures,
        generating mipmaps, and writing DDS files.

        http://go.microsoft.com/fwlink/?LinkId=248926

    UVAtlas
        This contains the UVAtlas isochart library that is used instead of the legacy D3DX9 for creating a
        isochart texture atlas.

        http://go.microsoft.com/fwlink/?LinkID=512686
    
All content and source code for this package are subject to the terms of the MIT License.
<http://opensource.org/licenses/MIT>.

For the latest version of the Samples Content Exporter, more detailed documentation, bug reports and feature requests, 
please visit the GitHub project.

http://go.microsoft.com/fwlink/?LinkId=226208


------------------
BUILD INSTRUCTIONS
------------------

Install the Autodesk FBX SDK 2011.3.1 or later (latest tested version is 2016.0) for Windows VS 2012 or VS 2013

    http://autodesk.com/fbx

Ensure the environment variable FBX_SDK is set to point to the Autodesk FBX SDK (such as "C:\Program Files\Autodesk\FBX\FbxSdk\2016.0")

Open the ContentExporter_2012.sln or ContentExporter_2013.sln file from Visual Studio.

Build the solution

The resulting command-line tool is located under ImportFBX in Debug_201x or Release_201x as ContentExporter.exe

Usage: ContentExporter [options] <filename.fbx>


----------
DISCLAIMER
----------

.SDKMESH has a long-time samples runtime geometry format for Microsoft since the retiring of the legacy .X file format. It has a number of
limitations, and we don't recommend using it as a your production solution for meshes. It is, however, very useful for samples and itself
serves as an example of such file containers. The documentation on the format can be found in the DirectXMesh project wiki.

http://go.microsoft.com/fwlink/?LinkID=324981


---------------
RELEASE HISTORY
---------------

November 20, 2015
    Added warning if using -materialcolors+ with black diffuse colors

November 5, 2015
    Add warning if attempting to convert non-multiple-of-4 images to compressed DDS
    Changed -forcetextureoverwite default to true

October 30, 2015
    Updated with VS 2015 support using the Autodesk FBX SDK 2016.1 or later
    Added new command-line option:
    -flipz (default+) to control negation of z component for LH vs. RH view coordinates
    Fixed -applyglobaltrans to transform exported normals
    Updated DirectXTex, UVAtlas, DirectXMesh libraries    
    Minor code cleanup

July 29, 2015
    Retired VS 2010 projects

July 8, 2015
    Added command-line options:
    -materialcolors (default+) to control export of material lambert/phong colors
    -lightmaps macro to set up for exporting meshes using lightmap textures
    Fixed problems with export of multiple uv sets and per-vertex colors
    Added some more warnings related to uv sets
    Updated DirectXTex, UVAtlas, DirectXMesh libraries    

June 25, 2015
    Added new command-line options:
    -useemissivetexture[+|-] Use EmissiveMapTexture as SpecularMapTexture 
    -defaultspecmap [name] Sets the default specular map texture name
    Changed defaults for -defaultdiffusemap and -defaultnormalmap to blank
    The  .SDKMESH  writer now fills in the SpecularTexture string in the material
    Change the log messages about applying the default texture names to log level 2 messages rather than warnings

May 6, 2015
    Added command-line options:
    -optimizemeshes (default-) applies a vertex cache optimization to the VB/IB controlled by -vcache/-restart
    -cleanmeshes (default+ if using optimizemeshes, otherwise -) performs back-facing and/or bowtie cleanup
    -applyglobaltrans (default-) applies the global transformation matrix on vertex data if not exporting animations
    -tangentsindex (default 0) controls which set of texture coordinates are used when computing tangents & binormals
    -gsadjacency (default-) controls how adjacency is computed for cleanup, optmization, and UV atlas operations
    -texturebgra (default-) controls how uncompressed processed textures are written (BGRA vs. RGBA)
    -exportcolors (default+) controls export of per vertex colors
    Removed support for -compressednormaltype dec3n as there is no DXGI equivalent (i.e. legacy Direct3D 9 only)
    Removed all dependencies on the legacy DirectX SDK and D3DX9
    - Makes use of DirectXMath, DirectXMesh, DirectXTex, and UVAtlas instead

April 16, 2015
    Updated with VS 2012 support using the Autodesk FBX SDK 2014.1 or later
    Updated with VS 2013 support using the Autodesk FBX SDK 2015.1 or later

April 30, 2012
    Updated support using the Autodesk FBX SDK 2011.3.1
    Updated with VS 2010 toolset support
    Last version to support VS 2008

DirectX SDK (June 2010)
    Updated with VS 2010 support (still uses VS 2008 toolset) using the Autodesk FBX SDK 2010.2

DirectX SDK (February 2010)
    Minor bug fixes
    Last version to support VS 2005

DirectX SDK (March 2009)
    Original release supporting VS 2005/2008 using the Autodesk FBX SDK 2009.1
