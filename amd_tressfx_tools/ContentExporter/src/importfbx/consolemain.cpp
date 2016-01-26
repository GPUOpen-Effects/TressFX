//-------------------------------------------------------------------------------------
// ConsoleMain.cpp
//
// Entry point for the content exporter application.  Also contains all of the command
// line parsing code.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//  
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=226208
//-------------------------------------------------------------------------------------

#include "stdafx.h"
#include <conio.h>

#include <DirectXMesh.h>
#include <DirectXTex.h>
#include <UVAtlas.h>

#include "FBXImportMain.h"

CHAR g_strExporterName[100];

namespace ATG
{
    extern XATGExportSettings g_XATGSettings;
}

using namespace ATG;

ExportScene* g_pScene = nullptr;
FBXTransformer g_FBXTransformer;

class ConsoleOutListener : public ILogListener
{
protected:
    HANDLE  m_hOut;
    WORD    m_wDefaultConsoleTextAttributes;
    WORD    m_wBackgroundAttributes;
public:
    ConsoleOutListener()
    {
        m_hOut = GetStdHandle( STD_OUTPUT_HANDLE );
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo( m_hOut, &csbi );
        m_wDefaultConsoleTextAttributes = csbi.wAttributes;
        m_wBackgroundAttributes = m_wDefaultConsoleTextAttributes & 0x00F0;
    }
    virtual void LogMessage( const CHAR* strMessage ) override
    {
        puts( strMessage );
    }
    virtual void LogWarning( const CHAR* strMessage ) override
    {
        SetConsoleTextAttribute( m_hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | m_wBackgroundAttributes );
        LogMessage( strMessage );
        SetConsoleTextAttribute( m_hOut, m_wDefaultConsoleTextAttributes );
    }
    virtual void LogError( const CHAR* strMessage ) override
    {
        SetConsoleTextAttribute( m_hOut, FOREGROUND_RED | FOREGROUND_INTENSITY | m_wBackgroundAttributes );
        LogMessage( strMessage );
        SetConsoleTextAttribute( m_hOut, m_wDefaultConsoleTextAttributes );
    }
};

bool g_bHelpPrinted = false;
void PrintHelp();

ConsoleOutListener g_ConsoleOutListener;
DebugSpewListener g_DebugSpewListener;

ExportManifest g_Manifest;

typedef std::vector<ExportPath> FileNameVector;
FileNameVector g_InputFileNames;

ExportPath g_WorkingPath;
ExportPath g_OutputFilePath;
ExportPath g_CurrentOutputFileName;
ExportPath g_CurrentInputFileName;

enum ExportFileFormat
{
    FILEFORMAT_XATG = 0,
    FILEFORMAT_SDKMESH = 1,
};

INT g_ExportFileFormat = FILEFORMAT_SDKMESH;

typedef bool MacroCommandCallback( const CHAR* strArgument, bool& bUsedArgument );

struct MacroCommand
{
    const CHAR* strCommandLine;
    const CHAR* strAnnotation;
    const CHAR* strDescription;
    MacroCommandCallback* pCallback;
};

bool MacroDisplayHelp( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);
    UNREFERENCED_PARAMETER(bUsedArgument);

    PrintHelp();
    return true;
}

bool MacroSetOutputPath( const CHAR* strArgument, bool& bUsedArgument )
{
    if (!strArgument)
    {
        ExportLog::LogError( "Missing output path" );
        return false;
    }
    bUsedArgument = true;

    g_OutputFilePath.SetPathOnly( strArgument );
    return true;
}

bool MacroCollisionMesh( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);
    UNREFERENCED_PARAMETER(bUsedArgument);

    g_pScene->Settings().bExportAnimations = false;
    g_pScene->Settings().bCompressVertexData = false;
    g_pScene->Settings().bComputeVertexTangentSpace = false;
    g_pScene->Settings().bExportBinormal = false;
    g_pScene->Settings().bExportCameras = false;
    g_pScene->Settings().bExportLights = false;
    g_pScene->Settings().bExportMaterials = false;
    g_pScene->Settings().bExportNormals = false;
    g_pScene->Settings().bExportColors = false;
    g_pScene->Settings().bExportSkinWeights = false;
    g_pScene->Settings().bForceIndex32Format = true;
    g_pScene->Settings().iMaxUVSetCount = 0;
    g_XATGSettings.bBundleTextures = false;
    g_XATGSettings.bUseExistingBundle = false;
    strcpy_s( g_pScene->Settings().strMeshNameDecoration, "CollisionMesh" );
    return true;
}

bool MacroAnimation( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);
    UNREFERENCED_PARAMETER(bUsedArgument);

    g_pScene->Settings().bExportAnimations = true;
    g_pScene->Settings().bCompressVertexData = false;
    g_pScene->Settings().bComputeVertexTangentSpace = false;
    g_pScene->Settings().bExportBinormal = false;
    g_pScene->Settings().bExportCameras = false;
    g_pScene->Settings().bExportLights = false;
    g_pScene->Settings().bExportMaterials = false;
    g_pScene->Settings().bExportNormals = false;
    g_pScene->Settings().bExportColors = false;
    g_pScene->Settings().bExportSkinWeights = false;
    g_pScene->Settings().bForceIndex32Format = false;
    g_pScene->Settings().bExportMeshes = false;
    g_pScene->Settings().bExportScene = false;
    g_pScene->Settings().iMaxUVSetCount = 0;
    g_XATGSettings.bBinaryBlobExport = false;
    g_XATGSettings.bBundleTextures = false;
    g_XATGSettings.bUseExistingBundle = false;
    g_pScene->Settings().bRenameAnimationsToFileName = true;
    return true;
}

bool MacroCharacter( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);
    UNREFERENCED_PARAMETER(bUsedArgument);

    g_pScene->Settings().bExportAnimations = false;
    g_pScene->Settings().bExportCameras = false;
    g_pScene->Settings().bExportLights = false;
    g_pScene->Settings().bExportMaterials = true;
    g_pScene->Settings().bExportMeshes = true;
    g_pScene->Settings().bExportScene = true;
    return true;
}

bool MacroLightmaps( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);
    UNREFERENCED_PARAMETER(bUsedArgument);

    g_pScene->Settings().bExportAnimations = false;
    g_pScene->Settings().bExportMeshes = true;
    g_pScene->Settings().bExportMaterials = true;
    g_pScene->Settings().bExportScene = true;
    g_pScene->Settings().bComputeVertexTangentSpace = false;
    g_pScene->Settings().bExportBinormal = false;
    g_pScene->Settings().bExportNormals = false;
    g_pScene->Settings().iMaxUVSetCount = 2;
    g_pScene->Settings().bMaterialColors = false;
    g_pScene->Settings().bUseEmissiveTexture = true;
    return true;
}

bool MacroWindowsD3D9( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);
    UNREFERENCED_PARAMETER(bUsedArgument);

    g_pScene->Settings().bLittleEndian = true;
    g_XATGSettings.bBundleTextures = false;
    g_XATGSettings.bUseExistingBundle = false;
    g_pScene->Settings().bCompressVertexData = false;
    g_pScene->Settings().dwNormalCompressedType = D3DDECLTYPE_FLOAT16_4;
    g_pScene->Settings().bBGRvsRGB = false;
    g_ExportFileFormat = FILEFORMAT_SDKMESH;
    return true;
}

bool MacroWindowsD3D10( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);
    UNREFERENCED_PARAMETER(bUsedArgument);

    g_pScene->Settings().bLittleEndian = true;
    g_XATGSettings.bBundleTextures = false;
    g_XATGSettings.bUseExistingBundle = false;
    g_pScene->Settings().bCompressVertexData = false;
    g_pScene->Settings().dwNormalCompressedType = D3DDECLTYPE_FLOAT16_4;
    g_pScene->Settings().bBGRvsRGB = false;
    g_ExportFileFormat = FILEFORMAT_SDKMESH;
    return true;
}

bool MacroWindowsD3D11( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);
    UNREFERENCED_PARAMETER(bUsedArgument);

    g_pScene->Settings().bLittleEndian = true;
    g_XATGSettings.bBundleTextures = false;
    g_XATGSettings.bUseExistingBundle = false;
    g_pScene->Settings().bCompressVertexData = false;
    g_pScene->Settings().dwNormalCompressedType = D3DDECLTYPE_FLOAT16_4;
    g_pScene->Settings().bBGRvsRGB = true;
    g_ExportFileFormat = FILEFORMAT_SDKMESH;
    return true;
}

bool MacroXbox360( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);
    UNREFERENCED_PARAMETER(bUsedArgument);

    g_pScene->Settings().bLittleEndian = false;
    g_XATGSettings.bBinaryBlobExport = true;
    g_XATGSettings.bBundleTextures = true;
    g_XATGSettings.bUseExistingBundle = true;
    g_pScene->Settings().bCompressVertexData = true;
    g_pScene->Settings().dwNormalCompressedType = D3DDECLTYPE_FLOAT16_4;
    g_pScene->Settings().bBGRvsRGB = true;
    g_ExportFileFormat = FILEFORMAT_XATG;
    return true;
}

bool MacroSDKMesh( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);
    UNREFERENCED_PARAMETER(bUsedArgument);

    g_ExportFileFormat = FILEFORMAT_SDKMESH;
    return true;
}

bool MacroXATG( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);
    UNREFERENCED_PARAMETER(bUsedArgument);

    g_ExportFileFormat = FILEFORMAT_XATG;
    return true;
}

bool MacroSubD11( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);

    MacroWindowsD3D11( nullptr, bUsedArgument );
    MacroSDKMesh( nullptr, bUsedArgument );
    g_pScene->Settings().bCompressVertexData = false;
    g_pScene->Settings().bConvertMeshesToSubD = true;
    g_pScene->Settings().bExportBinormal = false;
    g_pScene->Settings().bForceExportSkinWeights = true;
    g_pScene->Settings().iMaxUVSetCount = 1;
    g_pScene->Settings().bOptimizeAnimations = false;
    return true;
}

bool MacroSubDXbox( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);

    MacroXbox360( nullptr, bUsedArgument );
    g_pScene->Settings().bCompressVertexData = true;
    g_pScene->Settings().bConvertMeshesToSubD = true;
    g_pScene->Settings().bExportBinormal = false;
    g_pScene->Settings().bForceExportSkinWeights = true;
    g_pScene->Settings().iMaxUVSetCount = 1;
    return true;
}

bool MacroSetLogLevel( const CHAR* strArgument, bool& bUsedArgument )
{
    if (!strArgument)
    {
        ExportLog::LogError( "Missing log level to set" );
        return false;
    }
    bUsedArgument = true;

    INT iValue = atoi( strArgument );
    iValue = std::min( 10, std::max( 0, iValue ) );
    ExportLog::SetLogLevel( static_cast<UINT>( iValue ) );
    return true;
}

bool MacroSetVerbose( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);
    UNREFERENCED_PARAMETER(bUsedArgument);

    ExportLog::SetLogLevel( 4 );
    return true;
}

bool MacroAttach( const CHAR* strArgument, bool& bUsedArgument )
{
    UNREFERENCED_PARAMETER(strArgument);
    UNREFERENCED_PARAMETER(bUsedArgument);

#ifdef _DEBUG
    ExportLog::LogMsg( 0, "!!! Attach debugger NOW and then press any key..." );
    (void)_getch();
#endif
    return true;
}

bool MacroSaveSettings( const CHAR* strArgument, bool& bUsedArgument )
{
    if (!strArgument)
    {
        ExportLog::LogError( "Missing settings file to save" );
        return false;
    }
    bUsedArgument = true;

    bool bResult = g_SettingsManager.SaveSettings( strArgument );
    if( !bResult )
    {
        ExportLog::LogError( "Could not save settings to file \"%s\".", strArgument );
        return false;
    }

    ExportLog::LogMsg( 1, "Saved settings to file \"%s\".", strArgument );
    return true;
}

bool MacroLoadSettings( const CHAR* strArgument, bool& bUsedArgument )
{
    if (!strArgument)
    {
        ExportLog::LogError( "Missing settings file to load" );
        return false;
    }
    bUsedArgument = true;

    bool bResult = g_SettingsManager.LoadSettings( strArgument );
    if( !bResult )
    {
        ExportLog::LogError( "Could not load settings from file \"%s\".", strArgument );
        return false;
    }

    ExportLog::LogMsg( 1, "Loaded settings from file \"%s\".", strArgument );
    return true;
}

bool MacroLoadFileList( const CHAR* strArgument, bool& bUsedArgument )
{
    if (!strArgument)
    {
        ExportLog::LogError( "Missing file list name to load" );
        return false;
    }
    bUsedArgument = true;

    FILE* fp = nullptr;
    fopen_s( &fp, strArgument, "r" );
    if( !fp )
    {
        ExportLog::LogError( "Could not load file list from file \"%s\".", strArgument );
        return false;
    }

    size_t dwCount = 0;
    while( !feof( fp ) )
    {
        CHAR strFileName[MAX_PATH];
        fgets( strFileName, ARRAYSIZE( strFileName ), fp );
        CHAR* strNewline = strchr( strFileName, '\n' );
        if( strNewline )
        {
            *strNewline = '\0';
        }

        g_InputFileNames.push_back( strFileName );
        ++dwCount;
    }

    fclose( fp );

    ExportLog::LogMsg( 1, "Loaded %Iu input filenames from file \"%s\".", dwCount, strArgument );

    return true;
}

MacroCommand g_MacroCommands[] = {
#ifdef _DEBUG
    { "attach", "", "Wait for debugger attach", MacroAttach },
#endif
    { "help", "", "Display help", MacroDisplayHelp },
    { "?", "", "Display help", MacroDisplayHelp },
    { "outputpath", " <path>", "Sets the output root path; files will appear in scenes/ and textures/ subdirectories", MacroSetOutputPath },
    { "verbose", "", "Displays more detailed output, equivalent to -loglevel 4", MacroSetVerbose },
    { "xatg", "", "Use the XATG output file format, equivalent to -fileformat xatg", MacroXATG },
    { "sdkmesh", "", "Use the SDKMESH output file format, equivalent to -fileformat sdkmesh", MacroSDKMesh },
    { "xbox360", "", "Sets export options for an Xbox 360 target", MacroXbox360 },
    { "windowsd3d9", "", "Sets export options for a Windows Direct3D 9 target", MacroWindowsD3D9 },
    { "windowsd3d10", "", "Sets export options for a Windows Direct3D 10 target", MacroWindowsD3D10 },
    { "windowsd3d11", "", "Sets export options for a Windows Direct3D 11 target", MacroWindowsD3D11 },
    { "collisionmesh", "", "Sets export options for collision mesh export", MacroCollisionMesh },
    { "animation", "", "Sets export options for animation track export", MacroAnimation },
    { "character", "", "Sets export options for character (mesh & skeleton) export", MacroCharacter },
    { "lightmaps", "", "Sets export options for light-mapped mesh export", MacroLightmaps },
    { "subd11", "", "Sets export options for subdivision surface processing for SubD11 sample", MacroSubD11 },
    { "subdxbox", "", "Sets export options for subdivision surface processing for Xbox SubD sample", MacroSubDXbox },
    { "savesettings", " <filename>", "Saves all settings to the specified filename", MacroSaveSettings },
    { "loadsettings", " <filename>", "Loads settings from the specified filename", MacroLoadSettings },
    { "filelist", " <filename>", "Loads a list of input filenames from the specified filename", MacroLoadFileList },
    { "loglevel", " <ranged value 1 - 10>", "Sets the message logging level, higher values show more messages", MacroSetLogLevel },
};

ExportSettingsEntry* FindCommandHelper( ExportSettingsEntry* pRoot, const CHAR* strCommand )
{
    if( !pRoot )
        return nullptr;

    if( pRoot->m_Type == ExportSettingsEntry::CT_CATEGORY )
    {
        ExportSettingsEntry* pResult = FindCommandHelper( pRoot->m_pFirstChild, strCommand );
        if( pResult )
            return pResult;
        return FindCommandHelper( pRoot->m_pSibling, strCommand );
    }

    const CHAR* strMatch = pRoot->m_CommandLineOptionName.SafeString();
    if( strstr( strCommand, strMatch ) == strCommand )
    {
        return pRoot;
    }
    return FindCommandHelper( pRoot->m_pSibling, strCommand );
}

ExportSettingsEntry* FindCommand( const CHAR* strCommand )
{
    ExportSettingsEntry* pResult = nullptr;
    size_t dwCount = g_SettingsManager.GetRootCategoryCount();
    for( size_t i = 0; i < dwCount; ++i )
    {
        auto pEntry = g_SettingsManager.GetRootCategory( i );
        pResult = FindCommandHelper( pEntry, strCommand );
        if( pResult )
            return pResult;
    }
    return nullptr;
}

bool ParseMacroCommand( const CHAR* strCommand, const CHAR* strArgument, bool& pMacroFound, bool& bUsedArgument )
{
    pMacroFound = false;
    size_t dwCount = _countof( g_MacroCommands );
    for( size_t i = 0; i < dwCount; ++i )
    {
        MacroCommand& Command = g_MacroCommands[i];
        if( _stricmp( strCommand, Command.strCommandLine ) == 0 )
        {
            pMacroFound = true;
            return (Command.pCallback)( strArgument, bUsedArgument );
        }
    }
    return true;
}

bool ParseEnum( ExportSettingsEntry* pEntry, const CHAR* strArgument )
{
    DWORD dwEnumCount = pEntry->m_dwEnumValueCount;
    for( DWORD i = 0; i < dwEnumCount; ++i )
    {
        if( _stricmp( strArgument, pEntry->m_pEnumValues[i].strCommandLine ) == 0 )
        {
            pEntry->SetValue( pEntry->m_pEnumValues[i].iValue );
            return true;
        }
    }
    return false;
}

bool ParseCommand( const CHAR* strOriginalCommand, const CHAR* strArgument, bool& bUsedArgument )
{
    CHAR strCommand[128];
    strcpy_s( strCommand, strOriginalCommand );
    _strlwr_s( strCommand );

    bUsedArgument = false;

    ExportSettingsEntry* pEntry = FindCommand( strCommand );
    if( !pEntry )
    {
        bool bMacro = false;
        if (!ParseMacroCommand(strCommand, strArgument, bMacro, bUsedArgument))
            return false;

        if( !bMacro )
        {
            ExportLog::LogError( "Unknown command line option \"%s\"", strCommand );
            return false;
        }
        return true;
    }

    const CHAR* strCommandName = pEntry->m_CommandLineOptionName.SafeString();
    switch( pEntry->m_Type )
    {
    case ExportSettingsEntry::CT_CHECKBOX:
        {
            size_t dwLength = strlen( strCommandName );
            CHAR Argument = strCommand[dwLength];
            if( Argument == '+' )
            {
                pEntry->SetValue( true );
            }
            else if( Argument == '-' )
            {
                pEntry->SetValue( false );
            }
            else
            {
                ExportLog::LogError( "Missing + or - for option \"%s\".", strCommandName );
                return false;
            }
            break;
        }
    case ExportSettingsEntry::CT_BOUNDEDINTSLIDER:
        {
            if( !strArgument )
            {
                ExportLog::LogError( "No parameter value passed for option \"%s\".", strCommandName );
                return false;
            }
            INT iValue = atoi( strArgument );
            pEntry->SetValue( iValue );
            bUsedArgument = true;
            break;
        }
    case ExportSettingsEntry::CT_BOUNDEDFLOATSLIDER:
        {
            if( !strArgument )
            {
                ExportLog::LogError( "No parameter value passed for option \"%s\".", strCommandName );
                return false;
            }
            float fValue = (float)atof( strArgument );
            pEntry->SetValue( fValue );
            bUsedArgument = true;
            break;
        }
    case ExportSettingsEntry::CT_ENUM:
        {
            if( !strArgument )
            {
                ExportLog::LogError( "No parameter value passed for option \"%s\".", strCommandName );
                return false;
            }
            bool bValid = ParseEnum( pEntry, strArgument );
            if( !bValid )
            {
                ExportLog::LogError( "Invalid parameter value (\"%s\") given for option \"%s\".", strArgument, strCommand );
                return false;
            }
            bUsedArgument = true;
            break;
        }
    case ExportSettingsEntry::CT_STRING:
        {
            if( !strArgument )
            {
                ExportLog::LogError( "No parameter value passed for option \"%s\".", strCommandName );
                return false;
            }
            pEntry->SetValue( strArgument );
            bUsedArgument = true;
            break;
        }
    default:
        ExportLog::LogError( "Unknown type for option \"%s\".", strCommandName );
        return false;
    }

    return true;
}

void PrintEntryHelp( ExportSettingsEntry* pEntry )
{
    if( !pEntry )
        return;

    static CHAR strAnnotation[256];
    switch( pEntry->m_Type )
    {
    case ExportSettingsEntry::CT_CATEGORY:
        ExportLog::LogMsg( 0, "\n%s Options:", pEntry->m_DisplayName.SafeString() );
        PrintEntryHelp( pEntry->m_pFirstChild );
        PrintEntryHelp( pEntry->m_pSibling );
        return;
    case ExportSettingsEntry::CT_CHECKBOX:
        sprintf_s( strAnnotation, "[+|-] (default %s)", pEntry->m_DefaultValue.m_bValue ? "+" : "-" );
        break;
    case ExportSettingsEntry::CT_BOUNDEDFLOATSLIDER:
        sprintf_s( strAnnotation, " <ranged value %0.3f - %0.3f, default %0.3f>", pEntry->m_MinValue.m_fValue, pEntry->m_MaxValue.m_fValue, pEntry->m_DefaultValue.m_fValue );
        break;
    case ExportSettingsEntry::CT_BOUNDEDINTSLIDER:
        sprintf_s( strAnnotation, " <ranged value %d - %d, default %d>", pEntry->m_MinValue.m_iValue, pEntry->m_MaxValue.m_iValue, pEntry->m_DefaultValue.m_iValue );
        break;
    case ExportSettingsEntry::CT_ENUM:
        {
            strcpy_s( strAnnotation, " [ " );
            DWORD dwCount = pEntry->m_dwEnumValueCount;
            for( DWORD i = 0; i < dwCount; ++i )
            {
                if( i > 0 )
                {
                    strcat_s( strAnnotation, " | " );
                }
                strcat_s( strAnnotation, pEntry->m_pEnumValues[i].strCommandLine );
            }
            strcat_s( strAnnotation, " ]" );
            break;
        }
    case ExportSettingsEntry::CT_STRING:
        sprintf_s( strAnnotation, " <string default: \"%s\">", (const CHAR*)pEntry->m_DefaultValue.m_strValue );
        break;
    }

    if( pEntry->m_CommandLineOptionName )
    {
        ExportLog::LogMsg( 0, "    -%s%s : %s", pEntry->m_CommandLineOptionName.SafeString(), strAnnotation, pEntry->m_DisplayName.SafeString() );
    }
    PrintEntryHelp( pEntry->m_pSibling );
}

void PrintHelp()
{
    if( g_bHelpPrinted )
    {
        return;
    }
    g_bHelpPrinted = true;

    ExportLog::LogMsg( 0, "\nUsage: ContentExporter [options] \"filename1\" \"filename2\" ... \"filenameN\"" );
    ExportLog::LogMsg( 0, "\nThe following command line options can be used to modify scene parse and file output behavior." );

    ExportLog::LogMsg( 0, "\nGeneral & Macro Options:" );
    size_t dwMacroCount = ARRAYSIZE( g_MacroCommands );
    for( size_t i = 0; i < dwMacroCount; ++i )
    {
        ExportLog::LogMsg( 0, "    -%s%s : %s", g_MacroCommands[i].strCommandLine, g_MacroCommands[i].strAnnotation, g_MacroCommands[i].strDescription );
    }

    size_t dwRootCategoryCount = g_SettingsManager.GetRootCategoryCount();
    for( size_t i = 0; i < dwRootCategoryCount; ++i )
    {
        auto pCategory = g_SettingsManager.GetRootCategory( i );
        PrintEntryHelp( pCategory );
    }
    ExportLog::LogMsg( 0, "" );
}

void ParseInputFileName( const CHAR* strFileName )
{
    ExportPath InputFileName( strFileName );
    if( !InputFileName.IsAbsolutePath() )
    {
        InputFileName = g_WorkingPath;
        InputFileName.Append( strFileName );
    }

    g_InputFileNames.push_back( InputFileName );
}

std::vector<CHAR*> g_CommandStrings;

bool ParseCommandLine( INT argc, CHAR* argv[] )
{
    assert( argc >= 1 );

    for( INT i = 1; i < argc; ++i )
    {
        CHAR* strToken = argv[i];
        g_CommandStrings.push_back( strToken );
    }

    size_t dwCommandCount = g_CommandStrings.size();
    for( size_t i = 0; i < dwCommandCount; ++i )
    {
        const CHAR* strCommand = g_CommandStrings[i];
        if( strCommand[0] == '-' || strCommand[0] == '/' )
        {
            const CHAR* strArgument = nullptr;
            if( i < ( dwCommandCount - 1 ) )
            {
                strArgument = g_CommandStrings[i + 1];
            }

            bool bCommandWithParameter;
            if (!ParseCommand(strCommand + 1, strArgument, bCommandWithParameter))
                return false;

            if( bCommandWithParameter )
            {
                ++i;
            }
        }
        else
        {
            ParseInputFileName( strCommand );
        }
    }

    return true;
}

void BuildOutputFileName( const ExportPath& InputFileName )
{
    if( !g_OutputFilePath.IsEmpty() )
    {
        g_CurrentOutputFileName = g_OutputFilePath;
    }
    else
    {
        g_CurrentOutputFileName = InputFileName;
        g_CurrentOutputFileName.TrimOffFileName();
    }

    g_CurrentOutputFileName.ChangeFileName( InputFileName );

    if( g_ExportFileFormat == FILEFORMAT_SDKMESH )
    {
        g_CurrentOutputFileName.ChangeExtension( CONTENT_EXPORTER_BINARYFILE_EXTENSION );
    }
    else
    {
        g_CurrentOutputFileName.ChangeExtension( CONTENT_EXPORTER_FILE_EXTENSION );
    }
}

int __cdecl main(_In_ int argc, _In_z_count_(argc) char* argv[])
{
    g_WorkingPath = ExportPath::GetCurrentPath();

    ExportLog::AddListener( &g_ConsoleOutListener );
#if _MSC_VER >= 1500
    if( IsDebuggerPresent() )
    {
        ExportLog::AddListener( &g_DebugSpewListener );
    }
#endif

#ifdef _DEBUG
    sprintf_s( g_strExporterName, "%s version %d.%d.%d (DEBUG)", CONTENT_EXPORTER_TITLE, CONTENT_EXPORTER_MAJOR_VERSION, CONTENT_EXPORTER_MINOR_VERSION, CONTENT_EXPORTER_REVISION );
#else
    sprintf_s( g_strExporterName, "%s version %d.%d.%d", CONTENT_EXPORTER_TITLE, CONTENT_EXPORTER_MAJOR_VERSION, CONTENT_EXPORTER_MINOR_VERSION, CONTENT_EXPORTER_REVISION );
#endif

    ExportLog::SetLogLevel( 1 );
    ExportLog::EnableLogging( true );

    ExportLog::LogMsg( 0, "----------------------------------------------------------" );
    ExportLog::LogMsg( 0, g_strExporterName );
    ExportLog::LogMsg( 0, CONTENT_EXPORTER_VENDOR );
    ExportLog::LogMsg( 0, CONTENT_EXPORTER_COPYRIGHT );
    ExportLog::LogMsg( 0, "----------------------------------------------------------" );

    g_pScene = new ExportScene();
    g_pScene->SetDCCTransformer( &g_FBXTransformer );

    static const ExportEnumValue FileFormatEnums[] = {
        { CONTENT_EXPORTER_FILE_FILTER_DESCRIPTION, CONTENT_EXPORTER_FILE_EXTENSION, FILEFORMAT_XATG },
        { CONTENT_EXPORTER_BINARYFILE_FILTER_DESCRIPTION, CONTENT_EXPORTER_BINARYFILE_EXTENSION, FILEFORMAT_SDKMESH },
    };
    g_SettingsManager.AddEnum( g_SettingsManager.GetRootCategory( 0 ), "Output File Format", "fileformat", FILEFORMAT_SDKMESH, FileFormatEnums, ARRAYSIZE( FileFormatEnums ), &g_ExportFileFormat );

    XATGInitializeSettings();
    if ( !ParseCommandLine(argc, argv) )
    {
        return 1;
    }

    ExportLog::LogMsg( 9, "Microsoft C++ compiler version %d", _MSC_VER );
    ExportLog::LogMsg( 9, "FBX SDK version %s", FBXSDK_VERSION_STRING );

    ExportLog::LogMsg( 9, "DirectXMath version %d", DIRECTX_MATH_VERSION );
    ExportLog::LogMsg( 9, "DirectXMesh version %d", DIRECTX_MESH_VERSION );
    ExportLog::LogMsg( 9, "DirectXTex version %d", DIRECTX_TEX_VERSION );
    ExportLog::LogMsg( 9, "UVAtlas version %d", UVATLAS_VERSION );

    if( g_InputFileNames.empty() )
    {
        ExportLog::LogError( "No input filename(s) provided." );
        PrintHelp();
        return 1;
    }

    ExportCoreSettings InitialSettings = g_pScene->Settings();

    if ( InitialSettings.bForceIndex32Format && (InitialSettings.dwFeatureLevel <= D3D_FEATURE_LEVEL_9_1) )
    {
        ExportLog::LogWarning("32-bit index buffers not supported on Feature Level 9.1");
    }

    HRESULT hr = FBXImport::Initialize();
    if (FAILED(hr))
    {
        ExportLog::LogError( "Failed to initialize FBX (%08X)\n", hr );
        return 1;
    }
    ExportLog::LogMsg( 4, "FBX has been initialized." );

    // Initialize COM (needed for WIC)
    if( FAILED( hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED) ) )
    {
        ExportLog::LogError( "Failed to initialize COM (%08X)\n", hr );
        return 1;
    }

    ExportLog::LogMsg( 4, "COM has been initialized." );

    size_t dwInputFileCount = g_InputFileNames.size();

    bool bFoundErrors = false;

    for( size_t i = 0; i < dwInputFileCount; ++i )
    {
        ExportPath InputFileName = g_InputFileNames[i];
        g_CurrentInputFileName = InputFileName;

        BuildOutputFileName( InputFileName );
        if( g_CurrentOutputFileName.IsEmpty() )
        {
            ExportLog::LogError( "Output filename is invalid." );
            return 1;
        }
        g_pScene->Statistics().StartExport();
        g_pScene->Statistics().StartSceneParse();

        hr = FBXImport::ImportFile( InputFileName );
        if( FAILED(hr) )
        {
            ExportLog::LogError( "Could not load file \"%s\".", (const CHAR*)InputFileName );
            return 1;
        }

        g_pScene->Statistics().StartSave();

        bool bExportMaterials = g_pScene->Settings().bExportMaterials;

        if( SUCCEEDED(hr) )
        {
            if( g_ExportFileFormat == FILEFORMAT_SDKMESH )
            {
                ExportTextureConverter::ProcessScene( g_pScene, &g_Manifest, "", true );
                WriteSDKMeshFile( g_CurrentOutputFileName, &g_Manifest );
                if (bExportMaterials)
                {
                    ExportTextureConverter::PerformTextureFileOperations(&g_Manifest);
                }
            }
            else
            {
                if( g_XATGSettings.bBundleTextures && bExportMaterials )
                {
                    ExportTextureConverter::ProcessScene( g_pScene, &g_Manifest, "textures\\", false );
                    WriteXATGFile( g_CurrentOutputFileName, &g_Manifest );
                    ExportTextureConverter::PerformTextureFileOperations( &g_Manifest );
                    BundleTextures();
                }
                else
                {
                    ExportTextureConverter::ProcessScene( g_pScene, &g_Manifest, "textures\\", true );
                    WriteXATGFile( g_CurrentOutputFileName, &g_Manifest );
                    if (bExportMaterials)
                    {
                        ExportTextureConverter::PerformTextureFileOperations(&g_Manifest);
                    }
                }
            }
        }

        g_pScene->Statistics().EndExport();
        g_pScene->Statistics().FinalReport();
        if ( ExportLog::GenerateLogReport() )
            bFoundErrors = true;

        if( ( i + 1 ) < dwInputFileCount )
        {
            FBXImport::ClearScene();
            delete g_pScene;
            g_pScene = new ExportScene();
            g_pScene->SetDCCTransformer( &g_FBXTransformer );
            g_pScene->Settings() = InitialSettings;
            g_Manifest.Clear();
            ExportLog::ResetCounters();
        }
    }

    return (bFoundErrors) ? 1 : 0;
}

