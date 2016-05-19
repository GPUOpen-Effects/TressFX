//--------------------------------------------------------------------------------------
// File: main.cpp
//
// Starting point for the TressFX sample.
//
//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//--------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>

#include "DXUT.h"
#include "DXUTcamera.h"
#include "DXUTgui.h"
#include "DXUTsettingsDlg.h"
#include "SDKmisc.h"
#include "SDKMesh.h"

// DDSTextureLoader is included in AMD_LIB.h below for debug builds.
// Need to include it separately for non-debug builds.
#ifndef _DEBUG
#include "DDSTextureLoader.h"
#endif

#include "resource.h"

#include "AMD_TressFX.h"
#include "AMD_LIB.h"
#include "AMD_SDK.h"
#include "ShaderCacheSampleHelper.h"
#include "SceneRender.h"
#include "TFXFileIO.h"

// prevent warnings about unreferenced formal parameters
// prevent warnings from system headers about deprecated functions
// prevent warnings about conversion from 'int' to 'LPVOID' of greater size
#pragma warning(disable: 4100 4995 4312)
#include "strsafe.h"

// Ponytail model with 4 hair sections and 16 vertices per strand.
//const wchar_t* DEFAULT_TFXPROJFILENAME = L"..\\media\\ponytail\\ponytail.tfxproj";

// Animated bear model.
const wchar_t* DEFAULT_TFXPROJFILENAME = L"..\\media\\bear\\bear.tfxproj";

// Simple test model.
//const wchar_t* DEFAULT_TFXPROJFILENAME = L"..\\media\\testhair1\\TestHair1.tfxproj";

const float MATH_PI = 3.14159265359f;

#define FIELD_OF_VIEW (MATH_PI / 4.f)

// Hair LOD ranges
#define MIN_HAIR_DENSITY 0.3f
#define MAX_HAIR_RADIUS 1.0f
#define MAX_FRAGMENTS 768
#define MIN_FRAGMENTS 8
#define MAX_ALPHA_SM 0.03f

// maximum strand copies
#define MAX_STRAND_COPIES 3

#define AVERAGE_FRAME_COUNT 200
#define MAX_STRING_SIZE 256

using namespace AMD;
using namespace DirectX;

struct DemoModel
{
    DemoModel() : m_bInitialized(false) {}

    bool                            m_bInitialized;
    XMMATRIX                        m_modelTransformForHead;            // default transform
    TressFX_Desc                    m_TressFXParams;
    std::vector<std::wstring>       m_hairSectionNames;                 // hair section names from tfxproj file
    CSceneRender                    m_SceneRender;                      // Scene rendering class
    std::wstring                    m_tfxproj_name;                     // tfx project name for GUI
    int                             m_numHairSections;
    float                           m_density;                          // high quality density for the hair
    float                           m_radius;                           // high quality hair thickness
    float                           m_alphaSM;                          // high quality shadow map alpha
    float                           m_savedDensity;                     // saved settings for restoring after disabling LODs
    float                           m_savedRadius;
    int                             m_savedMaxFragments;
    float                           m_savedAlphaSM;
    float                           m_savedAlphaThreshold;
    float                           m_SavedTransparency;
    bool                            m_bEnableLOD;                       // Adaptive LOD
    TressFX_SceneMesh               m_SceneMesh;
    ID3D11ShaderResourceView*       m_pHairTextureSRV;
};


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
// DXUT resources
CDXUTDialogResourceManager          g_DialogResourceManager;    // Manager for shared resources of dialogs
CModelViewerCamera                  g_Camera;
CD3DSettingsDlg                     g_D3DSettingsDlg;           // Device settings dialog
CDXUTTextHelper*                    g_pTxtHelper = NULL;

// default camera positions
static XMVECTOR         g_defaultEyePt = XMVectorSet(-190.0f, 70.0f, -250.0f, 1.0f);
static XMVECTOR         g_defaultLookAt = XMVectorSet(0, 40, 0, 1.0f);
static XMVECTOR         g_lightEyePt = XMVectorSet(-421.25043f, 306.7890949f, -343.22232f, 1.0f);

static float g_nearPlane = 1.0f;
static float g_farPlane = 50000.0f;

static float g_minLodDistance = 400.0f;
static float g_maxLodDistance = 1500.0f;

// This variable is overloaded in terms of its meaning.
// Current meanings include: skinned, has animation, and possibly "turn on animation"
static  bool g_animate = false;

static bool g_shortCutOn = false;

DemoModel g_DemoModel;
TFXProjectFile g_TfxProject;  // Global because we need to keep track of last load so we can have a reasonable default for saving.

int g_ScreenWidth  = 1920;
int g_ScreenHeight = 1080;


// toggle UI
bool g_bShowUI    = true;

// toggle on-screen instructions
bool g_bShowControls = false;

// toggle whether to advance time in the animation.
bool g_bAdvanceTime = true;


static COLORREF g_vCustHairColors[16] =
{
    RGB( 115,  84,  56 ),   //0 brown
    RGB( 98,  14,  4 ),     //1 natural red
    RGB(  59,  48,  36 ),   //2 darkest brown
    RGB(  78,  67,  63 ),   //3 med dark brown
    RGB(  80,  68,  68 ),   //4 chestnut brown
    RGB( 106,  78,  66 ),   //5 light chestnut brown
    RGB(  85,  72,  56 ),   //6 dark golden brown
    RGB( 167, 133, 106 ),   //7 light golden brown
    RGB( 184, 151, 120 ),   //8 dark honey blond
    RGB( 220, 208, 186 ),   //9 bleached blond
    RGB( 222, 188, 153 ),   //10 light ash blond
    RGB( 151, 121,  97 ),   //11 med ash brown
    RGB( 230, 206, 168 ),   //12 lightest blond
    RGB( 229, 200, 168 ),   //13 pale golden blond
    RGB( 165, 107,  70 ),   //14 strawberry blond
    RGB( 41, 28, 22 ),      //15 Brownish fur color
};

#define RUBY_COLOR      1
#define PONYTAIL_COLOR  14
#define ROO_COLOR       15

struct SliderRange
{
    float min;
    float max;
};

// slider ranges
static const SliderRange    g_hairDensity = {0.f, 1.f};
static const SliderRange    g_hairAlpha = {0.f, 1.f};
static const SliderRange    g_hairAlphaSM = {0.f, 0.03f};
static const SliderRange    g_fiberRadius = {0.02f, 0.2f}; // in mm

// default settings
static const float          g_hairDensityDefault = 1.0f;
static const float          g_hairAlphaDefault = 0.5f;
static const float          g_hairAlphaSMDefault = 0.004f;
static const float          g_fiberRadiusDefault = 0.15f;

// Global Simulation Settings
//float     g_wind_magnitude = 0.f;
DirectX::XMFLOAT3   g_wind_direction(1, 0, 0);
float       g_targetFrameRateForSimulation = 1.f/60.f;


typedef struct timerData_t
{
    float effect;
    float simulation;
    float generateShadow;
    float renderScene;
    float renderHair;
} timerData;

timerData g_timerResults[AVERAGE_FRAME_COUNT];

unsigned g_frameCount;

//--------------------------------------------------------------------------------------
// AMD helper classes defined here
//--------------------------------------------------------------------------------------
static AMD::ShaderCache     g_ShaderCache;
static AMD::MagnifyTool     g_MagnifyTool;
static AMD::HUD             g_SimulationHUD;
static AMD::HUD             g_RenderHUD;

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
enum TRESSFX_VIEWER_IDC
{
    IDC_TOGGLEFULLSCREEN = 1,
    IDC_CHANGEDEVICE,
    IDC_OPENTFXFILE,
    IDC_SAVETFXFILE,
    IDC_CHECKBOX_SIMULATE,
    IDC_CHECKBOX_COLLISION,
    IDC_CHECKBOX_GUIDE_FOLLOW_HAIR,
    IDC_STATIC_HAIR_DAMPING,
    IDC_SLIDER_HAIR_DAMPING,
    IDC_STATIC_HAIR_LOCAL_STIFFNESS,
    IDC_SLIDER_HAIR_LOCAL_STIFFNESS,
    IDC_STATIC_HAIR_GLOBAL_STIFFNESS,
    IDC_SLIDER_HAIR_GLOBAL_STIFFNESS,
    IDC_STATIC_GLOBAL_SHAPE_MATCHING_EFFECT_RANGE,
    IDC_SLIDER_GLOBAL_SHAPE_MATCHING_EFFECT_RANGE,
    IDC_STATIC_NUM_LENGTH_CONSTRAINT_ITERATIONS,
    IDC_SLIDER_NUM_LENGTH_CONSTRAINT_ITERATIONS,
    IDC_STATIC_NUM_LOCAL_SHAPE_MATCHING_ITERATIONS,
    IDC_SLIDER_NUM_LOCAL_SHAPE_MATCHING_ITERATIONS,
    IDC_OPACITY,
    IDC_OPACITY_STATIC,
    IDC_CHECKBOX_SHORTCUT,
    IDC_CHECKBOX_TRANSPARENCY,
    IDC_OPACITYSM,
    IDC_OPACITYSM_STATIC,
    IDC_FIBERRADIUS,
    IDC_FIBERRADIUS_STATIC,
    IDC_DENSITY,
    IDC_DENSITY_STATIC,
    IDC_CHECKBOX_SM,
    IDC_CHECKBOX_THINTIP,
    IDC_CHECKBOX_LOD,
    IDC_WIND_MAGNITUDE,
    IDC_WIND_MAGNITUDE_STATIC,
    IDC_CHECKBOX_USE_COVERAGE,
    IDC_STATIC_STRAND_COPIES,
    IDC_BUTTON_STRAND_COPIES_PLUS,
    IDC_BUTTON_STRAND_COPIES_MINUS,
    IDC_STATIC_STRAND_COPIES_VALUE,
    IDC_COMBOBOX_HAIR_SECTION,
    IDC_RENDER_MENU,
    IDC_SIMULATION_MENU,
    IDC_BUTTON_HAIR_COLOR,
    IDC_SAVETFXSIMFILE,
    IDC_CHECKBOX_ANIMATION,
    IDC_NUM_CONTROL_IDS // THIS ONE SHOULD ALWAYS BE LAST!!!!!
};

const int AMD::g_MaxApplicationControlID = IDC_NUM_CONTROL_IDS;

static const int MAX_WIND_MAG = 50;
static const int MIN_WIND_MAG = -50;

// for finding D3D memory leaks
//#define MEM_DEBUG

#ifdef MEM_DEBUG
ID3D11Device*                       g_pMemDebugDevice;
#endif


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
                                       DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext );
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext );
void CALLBACK OnD3D11DestroyDevice( void* pUserContext );
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
                                  double fTime, float fElapsedTime, void* pUserContext );

void InitApp();
void RenderText();
void DestroyDemo(bool destroyShaders);
bool CreateDemo(const TFXProjectFile& tfxproject, bool createShaders);
void InitWithFileDialog();
void SaveProjectFileDialog();
void SaveTFXSimFile();

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) || defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    static wchar_t tfxProjectFilePath[_MAX_PATH] = L"";
    wcscpy_s(tfxProjectFilePath, _MAX_PATH, lpCmdLine); // Assuming that argument = path name.
    tfxProjectFilePath[_MAX_PATH-1] = L'\0'; // making sure it is null terminated.

    // Set DXUT callbacks
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( OnKeyboard );
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );

    DXUTSetCallbackD3D11DeviceAcceptable( IsD3D11DeviceAcceptable );
    DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice, tfxProjectFilePath );
    DXUTSetCallbackD3D11SwapChainResized( OnD3D11ResizedSwapChain );
    DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11ReleasingSwapChain );
    DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );
    DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );

    // It is okay to call TressFX_GetVersion before TressFX_Initialize
    unsigned int major, minor, patch;
    AMD::TressFX_GetVersion( &major, &minor, &patch );

    WCHAR windowTitle[64];
    swprintf_s( windowTitle, 64, L"TressFX Viewer v%d.%d.%d", major, minor, patch );

    InitApp();
    DXUTInit( true, true );
    DXUTSetCursorSettings( true, true );
    DXUTCreateWindow( windowTitle );

    // Require D3D_FEATURE_LEVEL_11_0
    DXUTCreateDevice( D3D_FEATURE_LEVEL_11_0, true, g_ScreenWidth, g_ScreenHeight );

    DXUTMainLoop(); // Enter into the DXUT render loop

    // Ensure the ShaderCache aborts if in a lengthy generation process
    g_ShaderCache.Abort();

    return DXUTGetExitCode();
}

//--------------------------------------------------------------------------------------
// Initialize the app
//--------------------------------------------------------------------------------------
void InitApp()
{
    D3DCOLOR DlgColor = 0x88888888; // Semi-transparent background for the dialog
    WCHAR szMessage[100];
    szMessage[0] = 0;

    // Initialize dialogs
    g_D3DSettingsDlg.Init( &g_DialogResourceManager );

    g_RenderHUD.m_GUI.Init( &g_DialogResourceManager );
    g_RenderHUD.m_GUI.SetBackgroundColors( DlgColor );
    g_RenderHUD.m_GUI.SetCallback( ::OnGUIEvent );
    g_RenderHUD.m_GUI.SetVisible(true);

    g_SimulationHUD.m_GUI.Init( &g_DialogResourceManager );
    g_SimulationHUD.m_GUI.SetBackgroundColors( DlgColor );
    g_SimulationHUD.m_GUI.SetCallback( ::OnGUIEvent );
    g_SimulationHUD.m_GUI.SetVisible(false);

    int iX = AMD::HUD::iElementOffset / 2;
    int iY = AMD::HUD::iElementDelta / 2;

    g_RenderHUD.m_GUI.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", AMD::HUD::iElementOffset,
        iY, AMD::HUD::iElementWidth, AMD::HUD::iElementHeight );
    g_RenderHUD.m_GUI.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", AMD::HUD::iElementOffset,
        iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, VK_F2 );
    g_RenderHUD.m_GUI.AddButton( IDC_OPENTFXFILE, L"(O)pen project", AMD::HUD::iElementOffset,
        iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, 79 ); // 79 is 'O' (that's o for "open", not zero)
    g_RenderHUD.m_GUI.AddButton( IDC_SAVETFXFILE, L"(S)ave project", AMD::HUD::iElementOffset,
        iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, 83 ); // 83 is 'S'

    iY += AMD::HUD::iElementDelta/2;

    g_RenderHUD.m_GUI.AddCheckBox(IDC_CHECKBOX_ANIMATION, L"Animation", iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, true);

    g_RenderHUD.m_GUI.AddCheckBox(IDC_CHECKBOX_SHORTCUT, L"ShortCut", iX, iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth,
        AMD::HUD::iElementHeight, g_shortCutOn);

    g_RenderHUD.m_GUI.AddCheckBox(IDC_CHECKBOX_USE_COVERAGE, L"Strand Antialiasing", iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, true);

    g_RenderHUD.m_GUI.AddCheckBox(IDC_CHECKBOX_TRANSPARENCY, NULL, iX, iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth,
        AMD::HUD::iElementHeight, true);
    swprintf_s(szMessage, L" Transparency:%.4f", g_hairAlphaDefault);
    g_RenderHUD.m_GUI.AddStatic( IDC_OPACITY_STATIC, szMessage, iX + AMD::HUD::iElementDelta, iY, AMD::HUD::iElementWidth,
        AMD::HUD::iElementHeight );
    g_RenderHUD.m_GUI.AddSlider( IDC_OPACITY, iX, iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth, AMD::HUD::iElementHeight,
        0, 100, (int)(100 * g_hairAlphaDefault) );

    g_RenderHUD.m_GUI.AddCheckBox(IDC_CHECKBOX_SM, NULL, iX, iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth,
        AMD::HUD::iElementHeight, true);
    swprintf_s(szMessage, L" Self-shadowing:%.4f", g_hairAlphaSMDefault);
    g_RenderHUD.m_GUI.AddStatic( IDC_OPACITYSM_STATIC, szMessage, iX + AMD::HUD::iElementDelta, iY,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight );
    g_RenderHUD.m_GUI.AddSlider( IDC_OPACITYSM, iX, iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth, AMD::HUD::iElementHeight,
        0, 100, (int)(100 * g_hairAlphaSMDefault) );

    iY += AMD::HUD::iElementDelta/2;

    swprintf_s(szMessage, L"Density:%.4f", g_hairDensityDefault);
    g_RenderHUD.m_GUI.AddStatic( IDC_DENSITY_STATIC, szMessage, iX, iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth,
        AMD::HUD::iElementHeight );
    g_RenderHUD.m_GUI.AddSlider( IDC_DENSITY, iX, iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth,
        AMD::HUD::iElementHeight, 0, 100, (int)(100 * g_hairDensityDefault) );

    swprintf_s(szMessage, L"Hair thickness:%.4f", g_fiberRadiusDefault);
    g_RenderHUD.m_GUI.AddStatic( IDC_FIBERRADIUS_STATIC, szMessage, iX, iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth,
        AMD::HUD::iElementHeight );
    g_RenderHUD.m_GUI.AddSlider( IDC_FIBERRADIUS, iX, iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth, AMD::HUD::iElementHeight,
        0, 200, (int)(100 * g_fiberRadiusDefault) );

    g_RenderHUD.m_GUI.AddCheckBox(IDC_CHECKBOX_LOD, L"Distance adaptive LOD", iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, true);

    iY += AMD::HUD::iElementDelta/2;

    int iSavedX = iX;
    swprintf_s(szMessage, L"Strand Copies: ");
    g_RenderHUD.m_GUI.AddStatic( IDC_STATIC_STRAND_COPIES, szMessage, iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight);
    g_RenderHUD.m_GUI.AddButton( IDC_BUTTON_STRAND_COPIES_MINUS, L"-", iX += 120, iY, AMD::HUD::iElementDelta, AMD::HUD::iElementHeight);
    swprintf_s(szMessage, L" %d", 1);
    g_RenderHUD.m_GUI.AddStatic( IDC_STATIC_STRAND_COPIES_VALUE, szMessage, iX += AMD::HUD::iElementDelta, iY,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight);
    g_RenderHUD.m_GUI.AddButton( IDC_BUTTON_STRAND_COPIES_PLUS, L"+", iX += AMD::HUD::iElementDelta, iY,
        AMD::HUD::iElementDelta, AMD::HUD::iElementHeight);
    iX = iSavedX;

    g_RenderHUD.m_GUI.AddCheckBox(IDC_CHECKBOX_THINTIP, L"Thin Tip", iX, iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth,
        AMD::HUD::iElementHeight, true);

    iY += AMD::HUD::iElementDelta/2;

    g_RenderHUD.m_GUI.AddButton( IDC_BUTTON_HAIR_COLOR, L"Change Hair Color", iX + 20, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight );

    iY += AMD::HUD::iElementDelta/2;

    // Add the magnify tool UI to our HUD
    g_MagnifyTool.InitApp( &g_RenderHUD.m_GUI, iY += AMD::HUD::iElementDelta );

    iY += AMD::HUD::iElementDelta / 2;

    g_RenderHUD.m_GUI.AddButton( IDC_SIMULATION_MENU, L"Simulation Menu", iX + 20, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight );
    unsigned int iSimButtonY = iY;

   //------------
    // Hair Simulation Parameters
    //------------
    iX = AMD::HUD::iElementOffset / 2;
    iY = AMD::HUD::iElementDelta / 2;

    g_SimulationHUD.m_GUI.AddButton( IDC_SAVETFXSIMFILE, L"Save tfxsim File", AMD::HUD::iElementOffset,
        iY, AMD::HUD::iElementWidth, AMD::HUD::iElementHeight );

    iY += AMD::HUD::iElementDelta;     // space for button
    iY += AMD::HUD::iElementDelta/2;   // extra space

    g_SimulationHUD.m_GUI.AddCheckBox( IDC_CHECKBOX_SIMULATE, L"(S)imulate", iX, iY, AMD::HUD::iElementWidth,
        AMD::HUD::iElementHeight, true, 'S');
    g_SimulationHUD.m_GUI.AddCheckBox( IDC_CHECKBOX_COLLISION, L"(C)ollision with head", iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, true, 'C');
    g_SimulationHUD.m_GUI.AddCheckBox( IDC_CHECKBOX_GUIDE_FOLLOW_HAIR, L"(G)uide and follow hair", iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, true, 'G');

    iY += AMD::HUD::iGroupDelta;

    CDXUTComboBox* pComboHairSection;
    g_SimulationHUD.m_GUI.AddComboBox( IDC_COMBOBOX_HAIR_SECTION, iX, iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth+55,
        AMD::HUD::iElementHeight, L'L', false, &pComboHairSection );

    if ( pComboHairSection )
    {
        pComboHairSection->SetDropHeight( 45 );
        pComboHairSection->SetScrollBarWidth(20);
    }

    swprintf_s(szMessage, L"Damping:%.2f", 0.f);
    g_SimulationHUD.m_GUI.AddStatic( IDC_STATIC_HAIR_DAMPING, szMessage, iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight );
    g_SimulationHUD.m_GUI.AddSlider( IDC_SLIDER_HAIR_DAMPING, iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, 0, (int)(100.0f), 50, true );

    swprintf_s(szMessage, L"Local stiffness:%.2f", 0.f);
    g_SimulationHUD.m_GUI.AddStatic( IDC_STATIC_HAIR_LOCAL_STIFFNESS, szMessage, iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight );
    g_SimulationHUD.m_GUI.AddSlider( IDC_SLIDER_HAIR_LOCAL_STIFFNESS, iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, 0, (int)(100.0f), 50, true );

    swprintf_s(szMessage, L"Global shape stiffness:%.2f", 0.f);
    g_SimulationHUD.m_GUI.AddStatic( IDC_STATIC_HAIR_GLOBAL_STIFFNESS, szMessage, iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight );
    g_SimulationHUD.m_GUI.AddSlider( IDC_SLIDER_HAIR_GLOBAL_STIFFNESS, iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, 0, (int)(100.0f), 50, true );

    swprintf_s(szMessage, L"Global shape range:%.2f", 0.f);
    g_SimulationHUD.m_GUI.AddStatic( IDC_STATIC_GLOBAL_SHAPE_MATCHING_EFFECT_RANGE, szMessage,
        iX, iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth, AMD::HUD::iElementHeight );
    g_SimulationHUD.m_GUI.AddSlider( IDC_SLIDER_GLOBAL_SHAPE_MATCHING_EFFECT_RANGE, iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, 0, (int)(100.0f), (int)(50), true );

    iY += AMD::HUD::iGroupDelta;

    swprintf_s(szMessage, L"Length constraint\n iterations:%d", 1);
    g_SimulationHUD.m_GUI.AddStatic( IDC_STATIC_NUM_LENGTH_CONSTRAINT_ITERATIONS, szMessage, iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight );
    g_SimulationHUD.m_GUI.AddSlider( IDC_SLIDER_NUM_LENGTH_CONSTRAINT_ITERATIONS, iX, iY += AMD::HUD::iElementDelta * 2,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, 0, (int)(10.0f), 1, true );

    swprintf_s(szMessage, L"Local shape constraint\n iterations:%d", 1);
    g_SimulationHUD.m_GUI.AddStatic( IDC_STATIC_NUM_LOCAL_SHAPE_MATCHING_ITERATIONS, szMessage, iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight );
    g_SimulationHUD.m_GUI.AddSlider( IDC_SLIDER_NUM_LOCAL_SHAPE_MATCHING_ITERATIONS, iX, iY += AMD::HUD::iElementDelta * 2,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight, 0, (int)(20.0f), 1, true );

    swprintf_s(szMessage, L"Wind Magnitude: %.2f", 0.0);
    g_SimulationHUD.m_GUI.AddStatic( IDC_WIND_MAGNITUDE_STATIC, szMessage, iX, iY += AMD::HUD::iElementDelta,
        AMD::HUD::iElementWidth, AMD::HUD::iElementHeight);
    g_SimulationHUD.m_GUI.AddSlider( IDC_WIND_MAGNITUDE, iX, iY += AMD::HUD::iElementDelta, AMD::HUD::iElementWidth,
        AMD::HUD::iElementHeight, MIN_WIND_MAG, MAX_WIND_MAG, 0 );

    iY += AMD::HUD::iGroupDelta;


    g_SimulationHUD.m_GUI.AddButton( IDC_RENDER_MENU, L"Render Menu", iX+20, iSimButtonY, AMD::HUD::iElementWidth, AMD::HUD::iElementHeight );


    // Setup the camera's view parameters
    g_Camera.SetEnablePositionMovement( true );
    g_Camera.SetScalers(0.001f, 20.f);
    g_Camera.SetViewParams(g_defaultEyePt, g_defaultLookAt);

    for (int i = 0; i < AVERAGE_FRAME_COUNT; i++)
    {
        g_timerResults[i].effect = 0;
        g_timerResults[i].simulation = 0;
        g_timerResults[i].generateShadow = 0;
        g_timerResults[i].renderScene = 0;
        g_timerResults[i].renderHair = 0;
        g_frameCount = 0;
    }

}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D10 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if ( s_bFirstTime )
    {
        s_bFirstTime = false;
        if ( pDeviceSettings->d3d11.DriverType == D3D_DRIVER_TYPE_REFERENCE )
        {
            DXUTDisplaySwitchingToREFWarning();
        }

        // Start with vsync disabled
        pDeviceSettings->d3d11.SyncInterval = 0;
    }

    // Disable MSAA
    pDeviceSettings->d3d11.sd.SampleDesc.Count = 1;
    pDeviceSettings->d3d11.sd.SampleDesc.Quality = 0;

    return true;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
    // Update the camera's position based on user input
    g_Camera.FrameMove( fElapsedTime );


    if ( !g_DemoModel.m_bInitialized )
    {
        return;
    }

    static double fTimeFromLastFrame = 0.0f;
    const bool bSimulationEnabled = g_DemoModel.m_TressFXParams.hairParams.bSimulation;
    const double fTimeForAnimationUpdate = bSimulationEnabled && g_bAdvanceTime ? fTime : fTimeFromLastFrame;
    if (g_bAdvanceTime)
    {
        fTimeFromLastFrame = fTime;
    }

    g_DemoModel.m_SceneRender.OnFrameMove(fTimeForAnimationUpdate);
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text
//--------------------------------------------------------------------------------------
void RenderText()
{
    WCHAR wcbuf[256];

    g_pTxtHelper->Begin();
    g_pTxtHelper->SetInsertionPos( 5, 5 );
    g_pTxtHelper->SetForegroundColor( XMVectorSet( 1.0f, 1.0f, 0.0f, 1.0f ) );
    g_pTxtHelper->DrawTextLine( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
    g_pTxtHelper->DrawTextLine( DXUTGetDeviceStats() );

    int numStrands = g_DemoModel.m_TressFXParams.numTotalHairStrands;
    float density = g_DemoModel.m_TressFXParams.hairParams.density;
    int strandCopies = g_DemoModel.m_TressFXParams.hairParams.strandCopies;
    int count = g_DemoModel.m_TressFXParams.numTotalHairVertices * 2; // triangle verts = number of line verts * 2

    swprintf_s( wcbuf, 256, L"Number of hair strands = %d verts = %d",
        int(numStrands * density) * strandCopies,
        int(density * count) * strandCopies );
    g_pTxtHelper->DrawTextLine( wcbuf );

    g_timerResults[g_frameCount].effect = (float)TIMER_GetTime( Gpu, L"Effect"          ) * 1000.0f;
    g_timerResults[g_frameCount].simulation = (float)TIMER_GetTime( Gpu, L"Effect|Simulation"      ) * 1000.0f;
    g_timerResults[g_frameCount].generateShadow = (float)TIMER_GetTime( Gpu, L"Effect|GenerateShadow" ) * 1000.0f;
    g_timerResults[g_frameCount].renderScene = (float)TIMER_GetTime( Gpu, L"Effect|RenderScene"     ) * 1000.0f;
    g_timerResults[g_frameCount].renderHair = (float)TIMER_GetTime( Gpu, L"Effect|RenderHair"      ) * 1000.0f;

    timerData movingAverage = {0};
    unsigned simZeroCount = 0;
    int simCount;

    for (int i=0; i < AVERAGE_FRAME_COUNT; i++)
    {
        movingAverage.effect += g_timerResults[i].effect;
        // only count the non-zero simulation times, since the simulation may skip frames
        if (g_timerResults[i].simulation == 0.0f)
        {
            simZeroCount++;
        }
        movingAverage.simulation += g_timerResults[i].simulation;
        movingAverage.generateShadow += g_timerResults[i].generateShadow;
        movingAverage.renderScene += g_timerResults[i].renderScene;
        movingAverage.renderHair += g_timerResults[i].renderHair;
    }
    movingAverage.effect /= AVERAGE_FRAME_COUNT;
    simCount = AVERAGE_FRAME_COUNT - simZeroCount;
    if (simCount <= 0)
    {
        simCount = AVERAGE_FRAME_COUNT;
    }
    movingAverage.simulation /= simCount;
    movingAverage.generateShadow /= AVERAGE_FRAME_COUNT;
    movingAverage.renderScene /= AVERAGE_FRAME_COUNT;
    movingAverage.renderHair /= AVERAGE_FRAME_COUNT;

    swprintf_s( wcbuf, 256, L"Effect cost in milliseconds( Total = %.3f )", movingAverage.effect );
    g_pTxtHelper->DrawTextLine( wcbuf );
    swprintf_s( wcbuf, 256, L"  Simulation      (ms): %.3f", movingAverage.simulation );
    g_pTxtHelper->DrawTextLine( wcbuf );
    swprintf_s( wcbuf, 256, L"  GenerateShadows (ms): %.3f", movingAverage.generateShadow );
    g_pTxtHelper->DrawTextLine( wcbuf );
    swprintf_s( wcbuf, 256, L"  RenderScene     (ms): %.3f", movingAverage.renderScene );
    g_pTxtHelper->DrawTextLine( wcbuf );
    swprintf_s( wcbuf, 256, L"  RenderHair      (ms): %.3f", movingAverage.renderHair );
    g_pTxtHelper->DrawTextLine( wcbuf );

    if (++g_frameCount >= AVERAGE_FRAME_COUNT)
    {
        g_frameCount = 0;
    }

    if (g_bShowControls)
    {
        g_pTxtHelper->SetForegroundColor( XMVectorSet( 0.0f, 1.0f, 0.0f, 1.0f ) );
        g_pTxtHelper->SetInsertionPos( 5, DXUTGetDXGIBackBufferSurfaceDesc()->Height/2 );
        g_pTxtHelper->DrawTextLine( L"Rotate Camera   : Left Mouse Button" );
        g_pTxtHelper->DrawTextLine( L"Translate Model : Middle Mouse Button" );
        g_pTxtHelper->DrawTextLine( L"Rotate Model    : Ctrl + Middle Mouse Button" );
        g_pTxtHelper->SetForegroundColor( XMVectorSet( 1.0f, 1.0f, 0.0f, 1.0f ) );
    }

    g_pTxtHelper->SetInsertionPos( 5, DXUTGetDXGIBackBufferSurfaceDesc()->Height - (AMD::HUD::iElementDelta * 2) );
    g_pTxtHelper->DrawTextLine( L"Toggle GUI    : F1" );

    g_pTxtHelper->SetInsertionPos( 5, DXUTGetDXGIBackBufferSurfaceDesc()->Height - AMD::HUD::iElementDelta );
    if (g_bShowControls)
    {
        g_pTxtHelper->DrawTextLine(L"Hide Controls : F4");
    }
    else
    {
        g_pTxtHelper->DrawTextLine(L"Show Controls : F4");
    }

    g_pTxtHelper->End();
}

//--------------------------------------------------------------------------------------
// Calculate the screen coordinates from 3D coordinates
//--------------------------------------------------------------------------------------
void CalcScreenCoordinates(const XMVECTOR position, float& x, float& y)
{
    XMMATRIX viewProjection = g_Camera.GetViewMatrix() * g_Camera.GetProjMatrix();

    XMVECTOR screenCoord = XMVector3TransformCoord(position, viewProjection);
    x = 0.5f * ( XMVectorGetX(screenCoord) + 1) * g_ScreenWidth;
    y = 0.5f * (-XMVectorGetY(screenCoord) + 1) * g_ScreenHeight;
}

//--------------------------------------------------------------------------------------
// Scale and normalize the coordinates
//--------------------------------------------------------------------------------------
XMFLOAT3 ScaleAndNormalize(float x, float y)
{
    float scale = 4;
    x = - x / (scale * g_ScreenWidth / 2);
    y =   y / (scale * g_ScreenHeight / 2);
    float z = 0;
    float mag = x * x + y * y;

    if ( mag > 1 )
    {
        scale = 1.0f / sqrtf(mag);
        x *= scale;
        y *= scale;
    }
    else
    {
        z = sqrtf(1.0f - mag);
    }

    return XMFLOAT3(x, y, z);
}

//--------------------------------------------------------------------------------------
// Move Model
//--------------------------------------------------------------------------------------
void MoveModel(int msg, WPARAM wParam, LPARAM lParam)
{
    DemoModel *pModel = &g_DemoModel;

    static LPARAM g_LastMouse;
    static XMFLOAT3 g_LastPoint;
    static bool g_ModelIsMoved;
    static XMMATRIX mtxTfm, mtxInvTfm;
    static float g_ScreenCenterX, g_ScreenCenterY;

    int mouseX = (short)LOWORD(lParam);
    int mouseY = (short)HIWORD(lParam);
    switch (msg)
    {
        case WM_MBUTTONDOWN:
            {
                XMMATRIX mtxTranslation = XMMatrixTranslationFromVector(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
                XMFLOAT4X4 f4x4Rotation;
                XMStoreFloat4x4(&f4x4Rotation, g_Camera.GetViewMatrix());
                for (int i = 0; i < 3; i++)
                {
                    f4x4Rotation.m[3][i] = 0;
                }
                XMMATRIX mtxRotation = XMLoadFloat4x4(&f4x4Rotation);
                mtxTfm = mtxTranslation * mtxRotation;
                mtxInvTfm = XMMatrixInverse(0, mtxTfm);
            }

            CalcScreenCoordinates(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), g_ScreenCenterX, g_ScreenCenterY);
            g_LastPoint = ScaleAndNormalize(mouseX - g_ScreenCenterX, mouseY - g_ScreenCenterY);
            g_LastMouse = lParam;
            g_ModelIsMoved = true;
            break;

        case WM_MOUSEMOVE:
            if (g_ModelIsMoved)
            {
                XMMATRIX transform = XMMatrixIdentity();

                if (wParam & MK_MBUTTON) // Translate
                {
                    transform = XMMatrixTranslation((float)-(42 * ((short)LOWORD(g_LastMouse) - mouseX)) / g_ScreenWidth,
                        (float)(42 * ((short)HIWORD(g_LastMouse) - mouseY)) / g_ScreenHeight, 0);
                    g_LastMouse = lParam;
                }

                XMMATRIX modelTransform = mtxTfm * transform * mtxInvTfm;

                bool bSimulate = g_SimulationHUD.m_GUI.GetCheckBox(IDC_CHECKBOX_SIMULATE)->GetChecked();

                if ( !bSimulate )
                {
                    pModel->m_TressFXParams.bWarp = true;
                }

                pModel->m_modelTransformForHead = pModel->m_modelTransformForHead * modelTransform;
            }
            break;
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            {
                g_ModelIsMoved = false;
                break;
            }
        default:
    break;
    }
}

//--------------------------------------------------------------------------------------
// Rotate Model
//--------------------------------------------------------------------------------------
void RotateModel(int msg, WPARAM wParam, LPARAM lParam)
{
    DemoModel *pModel = &g_DemoModel;

    static LPARAM g_LastMouse;
    static XMFLOAT3 g_LastPoint;
    static bool g_ModelIsMoved;
    static XMMATRIX mtxTfm, mtxInvTfm;
    static float g_ScreenCenterX, g_ScreenCenterY;

    int mouseX = (short)LOWORD(lParam);
    int mouseY = (short)HIWORD(lParam);
    switch (msg)
    {
        case WM_MBUTTONDOWN:
            {
                XMMATRIX mtxTranslation = XMMatrixTranslationFromVector(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
                XMFLOAT4X4 f4x4Rotation;
                XMStoreFloat4x4(&f4x4Rotation, g_Camera.GetViewMatrix());
                for (int i = 0; i<3; i++)
                {
                    f4x4Rotation.m[3][i] = 0;
                }
                XMMATRIX mtxRotation = XMLoadFloat4x4(&f4x4Rotation);
                mtxTfm = mtxTranslation * mtxRotation;
                mtxInvTfm = XMMatrixInverse(0, mtxTfm);
            }

            CalcScreenCoordinates(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), g_ScreenCenterX, g_ScreenCenterY);
            g_LastPoint = ScaleAndNormalize(mouseX - g_ScreenCenterX, mouseY - g_ScreenCenterY);
            g_LastMouse = lParam;
            g_ModelIsMoved = true;
            break;

        case WM_MOUSEMOVE:
            if (g_ModelIsMoved)
            {
                XMMATRIX transformX = XMMatrixIdentity();
                XMMATRIX transformY = XMMatrixIdentity();

                if (wParam & MK_MBUTTON) // Translate
                {
                    transformX = XMMatrixRotationY( (float)(42 * ((short)LOWORD(g_LastMouse) - mouseX)) / (g_ScreenWidth * 10) );
                    transformY = XMMatrixRotationX( (float)(42 * ((short)HIWORD(g_LastMouse) - mouseY)) / (g_ScreenHeight* 10) );
                    g_LastMouse = lParam;
                }

                XMMATRIX modelTransform = mtxTfm * transformX * transformY * mtxInvTfm;

                bool bSimulate = g_SimulationHUD.m_GUI.GetCheckBox(IDC_CHECKBOX_SIMULATE)->GetChecked();

                if (!bSimulate)
                {
                    pModel->m_TressFXParams.bWarp = true;
                }

                pModel->m_modelTransformForHead = modelTransform * pModel->m_modelTransformForHead;
            }
            break;
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            {
                g_ModelIsMoved = false;
                break;
            }
        default:
    break;
    }
}
//--------------------------------------------------------------------------------------
// Open a DialogBox to select the hair color
//--------------------------------------------------------------------------------------
void ChooseHairColor()
{
    CHOOSECOLOR cc;
    DirectX::XMFLOAT3 *pColor = &g_DemoModel.m_TressFXParams.hairParams.color;
    DWORD rgbCurrent = RGB((int)(pColor->x * 255), (int)(pColor->y * 255), (int)(pColor->z * 255));

    // the color picker only works in windowed mode
    DXUTDeviceSettings deviceSettings = DXUTGetDeviceSettings();
    bool fullscreen = !deviceSettings.d3d11.sd.Windowed;
    if (fullscreen)
    {
        DXUTToggleFullScreen();
    }

    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = DXUTGetHWND();
    cc.lpCustColors = (LPDWORD) g_vCustHairColors;
    cc.rgbResult = rgbCurrent;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    if (ChooseColor(&cc)==TRUE)
    {
        pColor->x = ((cc.rgbResult>> 0) & 0xFF)/255.f;
        pColor->y = ((cc.rgbResult>> 8) & 0xFF)/255.f;
        pColor->z = ((cc.rgbResult>>16) & 0xFF)/255.f;
    }

    // restore full screen mode
    if (fullscreen)
    {
        DXUTToggleFullScreen();
    }
}

//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext )
{
    // Pass messages to dialog resource manager calls so GUI state is updated correctly
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if (*pbNoFurtherProcessing)
    {
        return 0;
    }

    // Pass messages to settings dialog if its active
    if (g_D3DSettingsDlg.IsActive())
    {
        g_D3DSettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_RenderHUD.m_GUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if (*pbNoFurtherProcessing)
    {
        return 0;
    }
    *pbNoFurtherProcessing = g_SimulationHUD.m_GUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if (*pbNoFurtherProcessing)
    {
        return 0;
    }

    if (DXUTIsKeyDown(VK_CONTROL))
    {
        RotateModel(uMsg, wParam, lParam);
    }
    else
    {
        MoveModel(uMsg, wParam, lParam);
    }

    g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
    if ( bKeyDown )
    {
        switch ( nChar )
        {
        case VK_F1:
            g_bShowUI = !g_bShowUI;
            break;
        case VK_F4:
            g_bShowControls = !g_bShowControls;
            break;
        }
    }
}

//--------------------------------------------------------------------------------------
//
// UpdateGUIForCurrentModel
//
// Update the GUI controls for the current demo model.
// This should get called every time the model changes.
//
//--------------------------------------------------------------------------------------
void UpdateGUIForCurrentModel()
{
    DemoModel *pModel = &g_DemoModel;
    WCHAR szMessage[100];
    szMessage[0] = 0;


    // Update the Render Menu
    CDXUTComboBox* pComboSection;

    TressFX_HairParams *pHairParams = &pModel->m_TressFXParams.hairParams;
    g_RenderHUD.m_GUI.GetCheckBox(IDC_CHECKBOX_USE_COVERAGE)->SetChecked(pHairParams->bAntialias);

    swprintf_s(szMessage, L" Transparency:%.4f", pHairParams->alpha);
    g_RenderHUD.m_GUI.GetStatic(IDC_OPACITY_STATIC)->SetText(szMessage);
    g_RenderHUD.m_GUI.GetSlider(IDC_OPACITY)->SetValue((int)(100 * (pHairParams->alpha - g_hairAlpha.min) / (g_hairAlpha.max - g_hairAlpha.min)));

    g_RenderHUD.m_GUI.GetCheckBox(IDC_CHECKBOX_SM)->SetChecked(pHairParams->shadowTechnique != NONE_SHADOW_INDEX);

    swprintf_s(szMessage, L" Self-shadowing:%.4f", pHairParams->shadowMapAlpha);
    g_RenderHUD.m_GUI.GetStatic(IDC_OPACITYSM_STATIC)->SetText(szMessage);
    g_RenderHUD.m_GUI.GetSlider(IDC_OPACITYSM)->SetValue((int)(100 * (pHairParams->shadowMapAlpha - g_hairAlphaSM.min)
        / (g_hairAlphaSM.max - g_hairAlphaSM.min)));

    swprintf_s(szMessage, L" Density:%.4f", pHairParams->density);
    g_RenderHUD.m_GUI.GetStatic(IDC_DENSITY_STATIC)->SetText(szMessage);
    g_RenderHUD.m_GUI.GetSlider(IDC_DENSITY)->SetValue((int)(100 * (pHairParams->density - g_hairDensity.min)
        / (g_hairDensity.max - g_hairDensity.min)));

    swprintf_s(szMessage, L" Hair thickness:%.4f", pHairParams->thickness);
    g_RenderHUD.m_GUI.GetStatic(IDC_FIBERRADIUS_STATIC)->SetText(szMessage);
    g_RenderHUD.m_GUI.GetSlider(IDC_FIBERRADIUS)->SetValue((int)(100 * (pHairParams->thickness - g_fiberRadius.min)
        / (g_fiberRadius.max - g_fiberRadius.min)));

    swprintf_s(szMessage, L" %d", pHairParams->strandCopies);
    g_RenderHUD.m_GUI.GetStatic(IDC_STATIC_STRAND_COPIES_VALUE)->SetText(szMessage);
    g_RenderHUD.m_GUI.GetCheckBox(IDC_CHECKBOX_THINTIP)->SetChecked(pHairParams->bThinTip);
    g_RenderHUD.m_GUI.GetCheckBox(IDC_CHECKBOX_LOD)->SetChecked(pModel->m_bEnableLOD);

    // Update the Simulation Menu
    TressFX_SimulationParams *pSimulationParams = &pModel->m_TressFXParams.simulationParams;
    g_SimulationHUD.m_GUI.GetCheckBox(IDC_CHECKBOX_SIMULATE)->SetChecked(pModel->m_TressFXParams.hairParams.bSimulation);
    g_SimulationHUD.m_GUI.GetCheckBox(IDC_CHECKBOX_COLLISION)->SetChecked(pSimulationParams->bCollision);
    g_SimulationHUD.m_GUI.GetCheckBox(IDC_CHECKBOX_GUIDE_FOLLOW_HAIR)->SetChecked(pSimulationParams->bGuideFollowSimulation);

    pComboSection = g_SimulationHUD.m_GUI.GetComboBox(IDC_COMBOBOX_HAIR_SECTION);
    pComboSection->RemoveAllItems();

    if ( pModel->m_hairSectionNames.size() > 0 )
    {
        for (int i = 0; i < pModel->m_hairSectionNames.size(); i++)
        {
            pComboSection->AddItem(pModel->m_hairSectionNames[i].c_str(), ( LPVOID )i);
        }

        int selectedGroup = 0;

        pComboSection->SetSelectedByData( ( LPVOID )selectedGroup );

        swprintf_s(szMessage, L"Damping:%.2f", pSimulationParams->perSectionShapeParams[selectedGroup].damping);
        g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_HAIR_DAMPING)->SetText(szMessage);
        g_SimulationHUD.m_GUI.GetSlider(IDC_SLIDER_HAIR_DAMPING)->SetValue((int)(100.0f * pSimulationParams->perSectionShapeParams[selectedGroup].damping));

        swprintf_s(szMessage, L"Local stiffness:%.2f", pSimulationParams->perSectionShapeParams[selectedGroup].stiffnessForLocalShapeMatching);
        g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_HAIR_LOCAL_STIFFNESS)->SetText(szMessage);
        g_SimulationHUD.m_GUI.GetSlider(IDC_SLIDER_HAIR_LOCAL_STIFFNESS)->SetValue((int)(100.0f * pSimulationParams->perSectionShapeParams[selectedGroup].stiffnessForLocalShapeMatching));

        swprintf_s(szMessage, L"Global shape stiffness:%.2f", pSimulationParams->perSectionShapeParams[selectedGroup].stiffnessForGlobalShapeMatching);
        g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_HAIR_GLOBAL_STIFFNESS)->SetText(szMessage);
        g_SimulationHUD.m_GUI.GetSlider(IDC_SLIDER_HAIR_GLOBAL_STIFFNESS)->SetValue((int)(100.0f * pSimulationParams->perSectionShapeParams[selectedGroup].stiffnessForGlobalShapeMatching));

        swprintf_s(szMessage, L"Global shape range:%.2f", pSimulationParams->perSectionShapeParams[selectedGroup].globalShapeMatchingEffectiveRange);
        g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_GLOBAL_SHAPE_MATCHING_EFFECT_RANGE)->SetText(szMessage);
        g_SimulationHUD.m_GUI.GetSlider(IDC_SLIDER_GLOBAL_SHAPE_MATCHING_EFFECT_RANGE)->SetValue(
            (int)(100.0f * pSimulationParams->perSectionShapeParams[selectedGroup].globalShapeMatchingEffectiveRange));

        swprintf_s(szMessage, L"Length constraint\n iterations:%d", pSimulationParams->numLengthConstraintIterations);
        g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_NUM_LENGTH_CONSTRAINT_ITERATIONS)->SetText(szMessage);
        g_SimulationHUD.m_GUI.GetSlider(IDC_SLIDER_NUM_LENGTH_CONSTRAINT_ITERATIONS)->SetValue(pSimulationParams->numLengthConstraintIterations);

        swprintf_s(szMessage, L"Local shape matching\n iterations:%d", pSimulationParams->numLocalShapeMatchingIterations);
        g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_NUM_LOCAL_SHAPE_MATCHING_ITERATIONS)->SetText(szMessage);
        g_SimulationHUD.m_GUI.GetSlider(IDC_SLIDER_NUM_LOCAL_SHAPE_MATCHING_ITERATIONS)->SetValue(pSimulationParams->numLocalShapeMatchingIterations);
    }

    swprintf_s(szMessage, L" Wind magnitude:%.4f", g_TfxProject.windMag);
    g_SimulationHUD.m_GUI.GetStatic(IDC_WIND_MAGNITUDE_STATIC)->SetText(szMessage);
    g_SimulationHUD.m_GUI.GetSlider(IDC_WIND_MAGNITUDE)->SetValue((int)(g_TfxProject.windMag));

    g_RenderHUD.m_GUI.GetCheckBox( IDC_CHECKBOX_LOD )->SetChecked(pModel->m_bEnableLOD);
    g_RenderHUD.m_GUI.GetCheckBox( IDC_CHECKBOX_LOD )->SetEnabled(true);
    g_RenderHUD.m_GUI.GetSlider( IDC_DENSITY )->SetEnabled(!pModel->m_bEnableLOD);
    g_RenderHUD.m_GUI.GetSlider( IDC_FIBERRADIUS )->SetEnabled(!pModel->m_bEnableLOD);
    g_RenderHUD.m_GUI.GetSlider( IDC_OPACITYSM )->SetEnabled(!pModel->m_bEnableLOD);
    g_RenderHUD.m_GUI.GetCheckBox(IDC_CHECKBOX_SM)->SetEnabled(!pModel->m_bEnableLOD);
}

static TressFX_SimulationParams* GetCurrentSimulationParams()
{
    DemoModel *pModel = &g_DemoModel;
    return &pModel->m_TressFXParams.simulationParams;
}

static int GetSelectedSectionIndex()
{
    int selectedIndex = g_SimulationHUD.m_GUI.GetComboBox(IDC_COMBOBOX_HAIR_SECTION)->GetSelectedIndex();

    if ( selectedIndex < 0 )
    {
        selectedIndex = 0;
    }
    else if ( selectedIndex >= GetCurrentSimulationParams()->numHairSections )
    {
        selectedIndex = GetCurrentSimulationParams()->numHairSections-1;
    }

    return selectedIndex;
}

static TressFX_HairParams* GetCurrentHairParams()
{
    DemoModel *pModel = &g_DemoModel;
    return &pModel->m_TressFXParams.hairParams;
}

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
    float value = 0;
    WCHAR szMessage[100];
    szMessage[0] = 0;

    DemoModel *pModel = &g_DemoModel;
    TressFX_HairParams *pHairParams = &pModel->m_TressFXParams.hairParams;
    TressFX_SimulationParams *pSimulationParams = GetCurrentSimulationParams();
    int selectedIndex = GetSelectedSectionIndex();

    switch ( nControlID )
    {
        // Standard DXUT controls
        case IDC_TOGGLEFULLSCREEN:
            DXUTToggleFullScreen(); break;
        case IDC_CHANGEDEVICE:
            g_D3DSettingsDlg.SetActive( !g_D3DSettingsDlg.IsActive() ); break;
        case IDC_OPENTFXFILE:
            InitWithFileDialog(); break;
        case IDC_SAVETFXFILE:
            SaveProjectFileDialog(); break;

        case IDC_RENDER_MENU:
            g_SimulationHUD.m_GUI.SetVisible(false);
            g_RenderHUD.m_GUI.SetVisible(true);
            break;

        case IDC_SIMULATION_MENU:
            g_RenderHUD.m_GUI.SetVisible(false);
            g_SimulationHUD.m_GUI.SetVisible(true);
            break;

        case IDC_WIND_MAGNITUDE:
            value = (float)(((CDXUTSlider*)pControl)->GetValue());
            g_TfxProject.windMag = value;
            swprintf_s(szMessage, L"Wind Magnitude: %.2f", g_TfxProject.windMag);
            g_SimulationHUD.m_GUI.GetStatic( IDC_WIND_MAGNITUDE_STATIC )->SetText( szMessage );
            break;

        case IDC_OPACITY:
            value = ((CDXUTSlider*)pControl)->GetValue()/100.0f;
            pHairParams->alpha = g_hairAlpha.min * (1-value) + g_hairAlpha.max * value;
            swprintf_s(szMessage, L" Transparency:%.4f", pHairParams->alpha);
            g_RenderHUD.m_GUI.GetStatic( IDC_OPACITY_STATIC )->SetText( szMessage );
            break;

        case IDC_CHECKBOX_TRANSPARENCY:
            pHairParams->bTransparency = (((CDXUTCheckBox*)pControl)->GetChecked());
            if ( pHairParams->bTransparency == false)
            {
                pModel->m_SavedTransparency = pHairParams->alpha;
                pHairParams->alpha = 1.0;
                g_RenderHUD.m_GUI.GetSlider( IDC_OPACITY )->SetEnabled(false);
            }
            else
            {
                pHairParams->alpha = pModel->m_SavedTransparency;
                g_RenderHUD.m_GUI.GetSlider( IDC_OPACITY )->SetEnabled(true);
            }
            swprintf_s(szMessage, L" Transparency:%.4f",  pHairParams->alpha);
            g_RenderHUD.m_GUI.GetStatic( IDC_OPACITY_STATIC )->SetText( szMessage );
            g_RenderHUD.m_GUI.GetSlider( IDC_OPACITY )->SetValue((int)(100 * ( pHairParams->alpha - g_hairAlpha.min)
                / (g_hairAlpha.max - g_hairAlpha.min)));
           break;

        case IDC_FIBERRADIUS:
            value = ((CDXUTSlider*)pControl)->GetValue()/100.0f;
            pHairParams->thickness = g_fiberRadius.min * (1-value) + g_fiberRadius.max*value;
            swprintf_s(szMessage, L"Hair thickness:%.4f",  pHairParams->thickness);
            g_RenderHUD.m_GUI.GetStatic( IDC_FIBERRADIUS_STATIC )->SetText( szMessage );
            break;

        case IDC_DENSITY:
            value = ((CDXUTSlider*)pControl)->GetValue()/100.0f;
            pHairParams->density = g_hairDensity.min * (1-value) + g_hairDensity.max*value;
            swprintf_s(szMessage, L"Density:%.4f",  pHairParams->density);
            g_RenderHUD.m_GUI.GetStatic( IDC_DENSITY_STATIC )->SetText( szMessage );
            break;

         case IDC_OPACITYSM:
            value = ((CDXUTSlider*)pControl)->GetValue()/100.0f;
            pHairParams->shadowMapAlpha = g_hairAlphaSM.min * (1-value) + g_hairAlphaSM.max*value;
            swprintf_s(szMessage, L" Self-shadowing:%.4f",  pHairParams->shadowMapAlpha);
            g_RenderHUD.m_GUI.GetStatic( IDC_OPACITYSM_STATIC )->SetText( szMessage );
            break;

        case IDC_SLIDER_NUM_LENGTH_CONSTRAINT_ITERATIONS:
            pSimulationParams->numLengthConstraintIterations = ((CDXUTSlider*)pControl)->GetValue();
            swprintf_s(szMessage, L"Length constraint\n iterations:%d", pSimulationParams->numLengthConstraintIterations);

            g_SimulationHUD.m_GUI.GetStatic( IDC_STATIC_NUM_LENGTH_CONSTRAINT_ITERATIONS )->SetText( szMessage );
            break;

        case IDC_SLIDER_NUM_LOCAL_SHAPE_MATCHING_ITERATIONS:
            pSimulationParams->numLocalShapeMatchingIterations = ((CDXUTSlider*)pControl)->GetValue();
            swprintf_s(szMessage, L"Local shape matching\n iterations:%d", pSimulationParams->numLocalShapeMatchingIterations);

            g_SimulationHUD.m_GUI.GetStatic( IDC_STATIC_NUM_LOCAL_SHAPE_MATCHING_ITERATIONS )->SetText( szMessage );
            break;

        case IDC_BUTTON_STRAND_COPIES_PLUS:
            if ( pHairParams->strandCopies < MAX_STRAND_COPIES)
            {
                 pHairParams->strandCopies++;
            }
            swprintf_s(szMessage, L" %d",  pHairParams->strandCopies);
            g_RenderHUD.m_GUI.GetStatic( IDC_STATIC_STRAND_COPIES_VALUE )->SetText( szMessage );
            break;

        case IDC_BUTTON_STRAND_COPIES_MINUS:
            if ( pHairParams->strandCopies > 1)
            {
                 pHairParams->strandCopies--;
            }
            swprintf_s(szMessage, L" %d",  pHairParams->strandCopies);
            g_RenderHUD.m_GUI.GetStatic( IDC_STATIC_STRAND_COPIES_VALUE )->SetText( szMessage );
            break;

        case IDC_SLIDER_HAIR_DAMPING:
            value = (float)(((CDXUTSlider*)pControl)->GetValue()) / 100.0f;
            pSimulationParams->perSectionShapeParams[selectedIndex].damping = value;

            swprintf_s(szMessage, L"Damping:%.2f", pSimulationParams->perSectionShapeParams[selectedIndex].damping);
            g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_HAIR_DAMPING)->SetText(szMessage);
            break;

        case IDC_SLIDER_HAIR_LOCAL_STIFFNESS:
            value = (float)(((CDXUTSlider*)pControl)->GetValue()) / 100.0f;
            pSimulationParams->perSectionShapeParams[selectedIndex].stiffnessForLocalShapeMatching = value;

            swprintf_s(szMessage, L"Local stiffness:%.2f", pSimulationParams->perSectionShapeParams[selectedIndex].stiffnessForLocalShapeMatching);
            g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_HAIR_LOCAL_STIFFNESS)->SetText(szMessage);
            break;

        case IDC_SLIDER_HAIR_GLOBAL_STIFFNESS:
            value = (float)(((CDXUTSlider*)pControl)->GetValue()) / 100.0f;
            pSimulationParams->perSectionShapeParams[selectedIndex].stiffnessForGlobalShapeMatching = value;

            swprintf_s(szMessage, L"Global shape stiffness:%.2f", pSimulationParams->perSectionShapeParams[selectedIndex].stiffnessForGlobalShapeMatching);
            g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_HAIR_GLOBAL_STIFFNESS)->SetText(szMessage);
            break;

        case IDC_SLIDER_GLOBAL_SHAPE_MATCHING_EFFECT_RANGE:
            value = (float)(((CDXUTSlider*)pControl)->GetValue()) / 100.0f;
            pSimulationParams->perSectionShapeParams[selectedIndex].globalShapeMatchingEffectiveRange = value;

            swprintf_s(szMessage, L"Global shape range:%.2f", pSimulationParams->perSectionShapeParams[selectedIndex].globalShapeMatchingEffectiveRange);
            g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_GLOBAL_SHAPE_MATCHING_EFFECT_RANGE)->SetText(szMessage);
            break;

        case IDC_CHECKBOX_SM:
            {
                bool bSelfShadow = (((CDXUTCheckBox*)pControl)->GetChecked());
                if (bSelfShadow)
                {
                     pHairParams->shadowTechnique = SDSM_SHADOW_INDEX;
                    g_RenderHUD.m_GUI.GetSlider( IDC_OPACITYSM )->SetEnabled(true);
                }
                else
                {
                     pHairParams->shadowTechnique = NONE_SHADOW_INDEX;
                    g_RenderHUD.m_GUI.GetSlider( IDC_OPACITYSM )->SetEnabled(false);
                }
            }
            break;

        case IDC_CHECKBOX_THINTIP:
             pHairParams->bThinTip = (((CDXUTCheckBox*)pControl)->GetChecked());
            break;

        case IDC_CHECKBOX_ANIMATION:
             g_bAdvanceTime = (((CDXUTCheckBox*)pControl)->GetChecked());
            break;

        case IDC_CHECKBOX_SHORTCUT:
            g_shortCutOn = (((CDXUTCheckBox*)pControl)->GetChecked());
            DestroyDemo(false);
            CreateDemo(g_TfxProject, false);
            break;

        case IDC_CHECKBOX_LOD:
            pModel->m_bEnableLOD = (((CDXUTCheckBox*)pControl)->GetChecked());
            if (pModel->m_bEnableLOD)
            {
                pModel->m_savedDensity =  pHairParams->density;
                pModel->m_savedRadius =  pHairParams->thickness;
                pModel->m_savedMaxFragments =  pHairParams->maxFragments;
                pModel->m_savedAlphaSM =  pHairParams->shadowMapAlpha;
                pModel->m_savedAlphaThreshold = pHairParams->alphaThreshold;
                g_RenderHUD.m_GUI.GetSlider( IDC_DENSITY )->SetEnabled(false);
                g_RenderHUD.m_GUI.GetSlider( IDC_FIBERRADIUS )->SetEnabled(false);
                g_RenderHUD.m_GUI.GetSlider( IDC_OPACITYSM )->SetEnabled(false);
                g_RenderHUD.m_GUI.GetCheckBox(IDC_CHECKBOX_SM)->SetEnabled(false);
            }
            else
            {
                pHairParams->alphaThreshold = pModel->m_savedAlphaThreshold;
                pHairParams->shadowMapAlpha = pModel->m_savedAlphaSM;
                pHairParams->maxFragments = pModel->m_savedMaxFragments;
                pHairParams->density = pModel->m_savedDensity;
                pHairParams->thickness = pModel->m_savedRadius;
                g_RenderHUD.m_GUI.GetSlider( IDC_DENSITY )->SetEnabled(true);
                g_RenderHUD.m_GUI.GetSlider( IDC_FIBERRADIUS )->SetEnabled(true);
                g_RenderHUD.m_GUI.GetSlider( IDC_OPACITYSM )->SetEnabled(true);
                g_RenderHUD.m_GUI.GetCheckBox(IDC_CHECKBOX_SM)->SetEnabled(true);

                swprintf_s(szMessage, L"Density:%.4f",  pHairParams->density);
                g_RenderHUD.m_GUI.GetStatic( IDC_DENSITY_STATIC )->SetText( szMessage );
                g_RenderHUD.m_GUI.GetSlider( IDC_DENSITY )->SetValue((int)(100 * ( pHairParams->density - g_hairDensity.min)
                    / (g_hairDensity.max - g_hairDensity.min)));
                swprintf_s(szMessage, L"Hair thickness:%.4f",  pHairParams->thickness);
                g_RenderHUD.m_GUI.GetStatic( IDC_FIBERRADIUS_STATIC )->SetText( szMessage );
                g_RenderHUD.m_GUI.GetSlider( IDC_FIBERRADIUS )->SetValue((int)(100 * ( pHairParams->thickness - g_fiberRadius.min)
                    / (g_fiberRadius.max - g_fiberRadius.min)));
                swprintf_s(szMessage, L" Self-shadowing:%.4f",  pHairParams->shadowMapAlpha);
                g_RenderHUD.m_GUI.GetStatic( IDC_OPACITYSM_STATIC )->SetText( szMessage );
                g_RenderHUD.m_GUI.GetSlider( IDC_OPACITYSM )->SetValue((int)(100 * ( pHairParams->shadowMapAlpha - g_hairAlphaSM.min)
                    / (g_hairAlphaSM.max - g_hairAlphaSM.min)));
            }
            break;

        case IDC_CHECKBOX_USE_COVERAGE:
             pHairParams->bAntialias = (((CDXUTCheckBox*)pControl)->GetChecked());
            break;

        case IDC_CHECKBOX_SIMULATE:
             pHairParams->bSimulation = (((CDXUTCheckBox*)pControl)->GetChecked());
            break;

        case IDC_CHECKBOX_COLLISION:
            pSimulationParams->bCollision = (((CDXUTCheckBox*)pControl)->GetChecked());
            break;

        case IDC_CHECKBOX_GUIDE_FOLLOW_HAIR:
            pSimulationParams->bGuideFollowSimulation = (((CDXUTCheckBox*)pControl)->GetChecked());
            break;

        case IDC_BUTTON_HAIR_COLOR:
            ChooseHairColor();
            break;

        case IDC_COMBOBOX_HAIR_SECTION:
            g_SimulationHUD.m_GUI.GetSlider(IDC_SLIDER_HAIR_DAMPING)->SetValue(
                (int)(pSimulationParams->perSectionShapeParams[selectedIndex].damping * 100));
            g_SimulationHUD.m_GUI.GetSlider(IDC_SLIDER_HAIR_LOCAL_STIFFNESS)->SetValue(
                (int)(pSimulationParams->perSectionShapeParams[selectedIndex].stiffnessForLocalShapeMatching * 100));
            g_SimulationHUD.m_GUI.GetSlider(IDC_SLIDER_HAIR_GLOBAL_STIFFNESS)->SetValue(
                (int)(pSimulationParams->perSectionShapeParams[selectedIndex].stiffnessForGlobalShapeMatching * 100));
            g_SimulationHUD.m_GUI.GetSlider(IDC_SLIDER_GLOBAL_SHAPE_MATCHING_EFFECT_RANGE)->SetValue(
                (int)(pSimulationParams->perSectionShapeParams[selectedIndex].globalShapeMatchingEffectiveRange * 100));

            swprintf_s(szMessage, L"Damping:%.2f", pSimulationParams->perSectionShapeParams[selectedIndex].damping);
            g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_HAIR_DAMPING)->SetText(szMessage);

            swprintf_s(szMessage, L"Local stiffness:%.2f", pSimulationParams->perSectionShapeParams[selectedIndex].stiffnessForLocalShapeMatching);
            g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_HAIR_LOCAL_STIFFNESS)->SetText(szMessage);

            swprintf_s(szMessage, L"Global shape stiffness:%.2f", pSimulationParams->perSectionShapeParams[selectedIndex].stiffnessForGlobalShapeMatching);
            g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_HAIR_GLOBAL_STIFFNESS)->SetText(szMessage);

            swprintf_s(szMessage, L"Global shape range:%.2f", pSimulationParams->perSectionShapeParams[selectedIndex].globalShapeMatchingEffectiveRange);
            g_SimulationHUD.m_GUI.GetStatic(IDC_STATIC_GLOBAL_SHAPE_MATCHING_EFFECT_RANGE)->SetText(szMessage);

            break;

        case IDC_SAVETFXSIMFILE:
            SaveTFXSimFile(); break;

        default:
            break;
    }

    // Call the MagnifyTool GUI event handler
    g_MagnifyTool.OnGUIEvent( nEvent, nControlID, pControl, pUserContext );

}

//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
                                       DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    return true;
}

bool CreateDemo(const TFXProjectFile& tfxproject, bool createShaders)
{
    ID3D11Device* pd3dDevice = DXUTGetD3D11Device();
    assert(pd3dDevice);
    ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();
    assert(pd3dImmediateContext);

    DemoModel *pModel;
    TressFX_HairParams *pHairParams;
    TressFX_SimulationParams *pSimulationParams;
    TressFX_HairBlob hairBlob;
    CSceneRender *pSceneRender;
    const WCHAR *bodyFile;
    const WCHAR *animationFile;

    // Model to view
    pModel = &g_DemoModel;
    pModel->m_TressFXParams.pd3dDevice = pd3dDevice;
    pModel->m_TressFXParams.pd3dDeviceContext = pd3dImmediateContext;
    pModel->m_TressFXParams.backBufferWidth = g_ScreenWidth;
    pModel->m_TressFXParams.backBufferHeight = g_ScreenHeight;
    pModel->m_TressFXParams.bWarp = true;
    pHairParams = &pModel->m_TressFXParams.hairParams;
    pSimulationParams = GetCurrentSimulationParams();

    // enable guide/follow hair simulation
    pSimulationParams->bGuideFollowSimulation = true;

    pSceneRender = &pModel->m_SceneRender;

    pModel->m_modelTransformForHead = XMMatrixIdentity();
    pModel->m_tfxproj_name = tfxproject.mProjectName;

    g_animate = tfxproject.HasAnimation();
    pModel->m_TressFXParams.bSingleHeadTransform = !g_animate;
    pModel->m_TressFXParams.bShortCutOn = g_shortCutOn;

     // load the scene mesh
    bodyFile = tfxproject.mMeshFile.c_str();

    if (g_animate)
    {
        animationFile = tfxproject.mAnimFile.c_str();
    }
    else
    {
        animationFile = NULL;
    }

    pSceneRender->OnCreateDevice(pd3dDevice, bodyFile, animationFile, &g_ShaderCache, createShaders);
    pModel->m_SceneMesh.numMeshes = pSceneRender->m_numMeshes;
    pModel->m_SceneMesh.meshOffsets = new unsigned[pSceneRender->m_numMeshes];

    // Set eye point with respect to bounding box extent of scene mesh
    float boundBoxDiagLength = sqrt((pSceneRender->m_MeshBBoxExtent.x * pSceneRender->m_MeshBBoxExtent.x) +
                                    (pSceneRender->m_MeshBBoxExtent.y * pSceneRender->m_MeshBBoxExtent.y) +
                                    (pSceneRender->m_MeshBBoxExtent.z * pSceneRender->m_MeshBBoxExtent.z));
    g_defaultEyePt = XMVectorSet(pSceneRender->m_MeshBBoxCenter.x-boundBoxDiagLength*1.5f,
                                 pSceneRender->m_MeshBBoxCenter.y,
                                 pSceneRender->m_MeshBBoxCenter.z-boundBoxDiagLength*1.5f, 1.0f);
    g_defaultLookAt = XMVectorSet(pSceneRender->m_MeshBBoxCenter.x, pSceneRender->m_MeshBBoxCenter.y, pSceneRender->m_MeshBBoxCenter.z, 1.0f);
    g_Camera.SetViewParams(g_defaultEyePt, g_defaultLookAt);

    if (tfxproject.customLightPos)
    {
        g_lightEyePt = XMVectorSet(tfxproject.lightPos[0], tfxproject.lightPos[1], tfxproject.lightPos[2], 1.0f);
    }
    else
    {
        g_lightEyePt = XMVectorSet(pSceneRender->m_MeshBBoxCenter.x + boundBoxDiagLength*1.0f,
                                   pSceneRender->m_MeshBBoxCenter.y + boundBoxDiagLength*3.0f,
                                   pSceneRender->m_MeshBBoxCenter.z - boundBoxDiagLength*3.0f, 1.0f);
    }

    // Set near and far plane for this geometry
    g_nearPlane = 0.1f * boundBoxDiagLength;
    g_nearPlane = (g_nearPlane > 1.0f) ? 1.0f : g_nearPlane;
    g_farPlane = 20.0f * boundBoxDiagLength;

    const XMVECTOR modelCenter = XMLoadFloat3(&pSceneRender->m_MeshBBoxCenter);
    XMVECTOR vCameraToHair = modelCenter - g_Camera.GetEyePt();
    g_minLodDistance = XMVectorGetX(XMVector3Length(vCameraToHair));
    g_maxLodDistance = g_farPlane;

    unsigned totalIndexCount = 0;
    for (unsigned i = 0; i < pSceneRender->m_numMeshes; i++)
    {
       pModel->m_SceneMesh.meshOffsets[i] = totalIndexCount;
       totalIndexCount += pSceneRender->m_pMeshIndexCount[i];
    }

    // collision capsules
    TressFX_CollisionCapsule& collisionCapsules = pModel->m_TressFXParams.collisionCapsule;

    pSimulationParams->bCollision = false;

    if ( tfxproject.mCollisionCapsule.size() >= 1 )
    {
        collisionCapsules.cc0_center1AndRadius.x = tfxproject.mCollisionCapsule[0].x0;
        collisionCapsules.cc0_center1AndRadius.y = tfxproject.mCollisionCapsule[0].y0;
        collisionCapsules.cc0_center1AndRadius.z = tfxproject.mCollisionCapsule[0].z0;
        collisionCapsules.cc0_center1AndRadius.w = tfxproject.mCollisionCapsule[0].r;
        collisionCapsules.cc0_center2AndRadiusSquared.x = tfxproject.mCollisionCapsule[0].x1;
        collisionCapsules.cc0_center2AndRadiusSquared.y = tfxproject.mCollisionCapsule[0].y1;
        collisionCapsules.cc0_center2AndRadiusSquared.z = tfxproject.mCollisionCapsule[0].z1;
        collisionCapsules.cc0_center2AndRadiusSquared.w = tfxproject.mCollisionCapsule[0].r * tfxproject.mCollisionCapsule[0].r;
        pSimulationParams->bCollision = true;
    }

    if ( tfxproject.mCollisionCapsule.size() >= 2 )
    {
        collisionCapsules.cc1_center1AndRadius.x = tfxproject.mCollisionCapsule[1].x0;
        collisionCapsules.cc1_center1AndRadius.y = tfxproject.mCollisionCapsule[1].y0;
        collisionCapsules.cc1_center1AndRadius.z = tfxproject.mCollisionCapsule[1].z0;
        collisionCapsules.cc1_center1AndRadius.w = tfxproject.mCollisionCapsule[1].r;
        collisionCapsules.cc1_center2AndRadiusSquared.x = tfxproject.mCollisionCapsule[1].x1;
        collisionCapsules.cc1_center2AndRadiusSquared.y = tfxproject.mCollisionCapsule[1].y1;
        collisionCapsules.cc1_center2AndRadiusSquared.z = tfxproject.mCollisionCapsule[1].z1;
        collisionCapsules.cc1_center2AndRadiusSquared.w = tfxproject.mCollisionCapsule[1].r * tfxproject.mCollisionCapsule[1].r;
    }

    if ( tfxproject.mCollisionCapsule.size() >= 3 )
    {
        collisionCapsules.cc2_center1AndRadius.x = tfxproject.mCollisionCapsule[2].x0;
        collisionCapsules.cc2_center1AndRadius.y = tfxproject.mCollisionCapsule[2].y0;
        collisionCapsules.cc2_center1AndRadius.z = tfxproject.mCollisionCapsule[2].z0;
        collisionCapsules.cc2_center1AndRadius.w = tfxproject.mCollisionCapsule[2].r;
        collisionCapsules.cc2_center2AndRadiusSquared.x = tfxproject.mCollisionCapsule[2].x1;
        collisionCapsules.cc2_center2AndRadiusSquared.y = tfxproject.mCollisionCapsule[2].y1;
        collisionCapsules.cc2_center2AndRadiusSquared.z = tfxproject.mCollisionCapsule[2].z1;
        collisionCapsules.cc2_center2AndRadiusSquared.w = tfxproject.mCollisionCapsule[2].r * tfxproject.mCollisionCapsule[2].r;
    }

    // load the tfx files
    std::ifstream tfxFile;
    std::filebuf* pbuf;
    std::size_t size;
    int numHairSectionsCounter = 0;

    TressFX_Initialize(pModel->m_TressFXParams);

    pModel->m_hairSectionNames.clear();
    pSimulationParams->numHairSections = 0;
    const int numHairSectionsTotal = std::min(tfxproject.CountTFXFiles(), TressFX_SimulationParams::MAX_NUM_HAIR_SECTIONS);

    for (int i = 0; i < numHairSectionsTotal; ++i)
    {
        // set hair section name for GUI
        pModel->m_hairSectionNames.push_back(tfxproject.mTFXFile[i].sectionNameForGUI);

        tfxFile.clear();
        tfxFile.open(tfxproject.mTFXFile[i].tfxFileName, std::ios::binary | std::ifstream::in);

        if (tfxFile.fail())
        {
            return false;
        }

        pbuf = tfxFile.rdbuf();
        size = pbuf->pubseekoff (0, tfxFile.end, tfxFile.in);
        pbuf->pubseekpos (0, tfxFile.in);
        hairBlob.pHair = (void *) new char[size];
        tfxFile.read((char *)hairBlob.pHair, size);
        tfxFile.close();
        hairBlob.size = (unsigned)size;

        TressFX_GuideFollowParams guideFollowParams;
        guideFollowParams.numFollowHairsPerGuideHair = tfxproject.mTFXFile[i].numFollowHairsPerGuideHair;
        guideFollowParams.radiusForFollowHair = tfxproject.mTFXFile[i].radiusForFollowHair;
        guideFollowParams.tipSeparationFactor = tfxproject.mTFXFile[i].tipSeparationFactor;

        if (g_animate)
        {
            std::ifstream skinFile;
            skinFile.open(tfxproject.mTFXSkinFile[i], std::ios::binary | std::ifstream::in);
            pbuf = skinFile.rdbuf();
            size = pbuf->pubseekoff (0, skinFile.end, skinFile.in);
            pbuf->pubseekpos (0, skinFile.in);
            hairBlob.pAnimation = (void *) new char[size];
            skinFile.read((char *)hairBlob.pAnimation, size);
            skinFile.close();
            hairBlob.animationSize = (unsigned)size;
        }
        else
        {
            hairBlob.animationSize = 0;
            hairBlob.pAnimation = NULL;
        }

        TressFX_LoadRawAsset(pModel->m_TressFXParams, guideFollowParams, &hairBlob);

        SAFE_DELETE_ARRAY(hairBlob.pAnimation);
        SAFE_DELETE_ARRAY(hairBlob.pHair);
        hairBlob.size = 0;

        TressFX_ShapeParams shapeParam;
        shapeParam.damping = tfxproject.mTFXSimFile[i].damping;
        shapeParam.globalShapeMatchingEffectiveRange = tfxproject.mTFXSimFile[i].globalShapeMatchingEffectiveRange;
        shapeParam.stiffnessForGlobalShapeMatching = tfxproject.mTFXSimFile[i].stiffnessForGlobalShapeMatching;
        shapeParam.stiffnessForLocalShapeMatching = tfxproject.mTFXSimFile[i].stiffnessForLocalShapeMatching;
        pSimulationParams->perSectionShapeParams[numHairSectionsCounter] = shapeParam;

        numHairSectionsCounter++;
    }

    assert( numHairSectionsCounter == numHairSectionsTotal );
    pModel->m_numHairSections = numHairSectionsTotal;
    pSimulationParams->numHairSections = numHairSectionsTotal;

    pSimulationParams->numLengthConstraintIterations = tfxproject.lengthConstraintIterations;
    pSimulationParams->numLocalShapeMatchingIterations = tfxproject.localShapeMatchingIterations;
    pSimulationParams->windMag = tfxproject.windMag;
    pSimulationParams->windDir = g_wind_direction;


    // Only supports one hair texture file.
    DirectX::CreateDDSTextureFromFile(pd3dDevice, tfxproject.mTFXTexFile.c_str(), NULL, &pModel->m_pHairTextureSRV);

    TressFX_CreateProcessedAsset(pModel->m_TressFXParams, NULL, &pModel->m_SceneMesh, pModel->m_pHairTextureSRV);


    pHairParams->color.x = tfxproject.hairColor[0];
    pHairParams->color.y = tfxproject.hairColor[1];
    pHairParams->color.z = tfxproject.hairColor[2];
    pHairParams->Ka = tfxproject.Ka >= 0 ? tfxproject.Ka : pSceneRender->m_MeshAmbient;
    pHairParams->Kd = tfxproject.Kd;
    pHairParams->Ks1 = tfxproject.Ks1;
    pHairParams->Ex1 = tfxproject.Ex1;
    pHairParams->Ks2 = tfxproject.Ks2;
    pHairParams->Ex2 = tfxproject.Ex2;
    pHairParams->alpha = tfxproject.alpha;
    pHairParams->shadowMapAlpha = tfxproject.shadowMapAlpha;
    pHairParams->thickness = tfxproject.thickness;
    pHairParams->density = tfxproject.density;
    pHairParams->strandCopies = 1;
    pHairParams->duplicateStrandSpacing = 0.3f;
    pHairParams->bAntialias = true;
    pHairParams->bThinTip = true;
    pHairParams->bTransparency = true;
    pHairParams->shadowTechnique = SDSM_SHADOW_INDEX;
    pHairParams->bSimulation = true;
    pHairParams->maxFragments = MAX_FRAGMENTS;
    pHairParams->ambientLightColor = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
    pHairParams->pointLightColor = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
    pHairParams->alphaThreshold = ALPHA_THRESHOLD;

    pModel->m_bEnableLOD = false;
    pModel->m_density = pHairParams->density;
    pModel->m_radius = pHairParams->thickness;
    pModel->m_alphaSM = pHairParams->shadowMapAlpha;
    pModel->m_SceneMesh.pMeshVertices = NULL;
    pModel->m_SceneMesh.pTransformedVerts = NULL;
    pModel->m_SceneMesh.pMeshVertices = 0;
    pModel->m_bInitialized = true;

    // Update the UI
    UpdateGUIForCurrentModel();

    return true;
}

void DestroyDemo(bool destroyShaders)
{
    DemoModel *pModel = &g_DemoModel;

    if ( !pModel->m_bInitialized )
    {
        return;
    }

    TressFX_Release(pModel->m_TressFXParams);
    pModel->m_hairSectionNames.clear();
    pModel->m_SceneRender.OnDestroy(destroyShaders);
    pModel->m_bInitialized = false;

    SAFE_DELETE_ARRAY(pModel->m_SceneMesh.meshOffsets);
}

void InitWithFileDialog()
{
    TCHAR lpstrFile[MAX_PATH] = L"";  // MAX_PATH = 260

    TCHAR curDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, curDir);

    OPENFILENAME OFN =
    {
        sizeof(OPENFILENAME),
        NULL, NULL,
        L"tfxproj Files (*.tfxproj)\0*.tfxproj\0All(*.*)\0*.*\0\0",
        NULL, 0, 1,
        lpstrFile, MAX_PATH,
        NULL, 0,
        NULL, L"Load TressFX Project File Dialog",
        OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, 0,
        0, L"tfxproj", 0,
        NULL, NULL
    };

    if (GetOpenFileName(&OFN))
    {
        std::wstring tfxproj_filePath = lpstrFile;

        g_TfxProject.Read(tfxproj_filePath.c_str());

        SetCurrentDirectory(curDir);

        DestroyDemo(false);

        CreateDemo(g_TfxProject, false);
    }
}


void SaveProjectFileDialog()
{
    TCHAR curDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, curDir);

    wchar_t pFilePath[MAX_PATH];

    wcscpy_s(pFilePath, MAX_PATH, g_TfxProject.mProjectFile.c_str());


    OPENFILENAME OFN =
    {
        sizeof(OPENFILENAME),
        NULL, NULL,
        L"tfxproj Files (*.tfxproj)\0*.tfxproj\0All(*.*)\0*.*\0\0",
        NULL, 0, 1,
        pFilePath, MAX_PATH,
        NULL, 0, // File title
        g_TfxProject.mProjectDir.c_str(),  // initial dir
        L"Save TressFX Project File Dialog",
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 0,
        0, L"tfxproj", 0,
        NULL, NULL
    };

    if (GetSaveFileName(&OFN))
    {
        g_TfxProject.SetHairParams(GetCurrentHairParams());
        g_TfxProject.SetHairSimulationParams(GetCurrentSimulationParams());
        g_TfxProject.Write(pFilePath);

        SetCurrentDirectory(curDir);
    }
}


//void SaveTFXSimFile(const TressFX_ShapeParams& params, const WCHAR* projectFilepath, const WCHAR* filename)
void SaveTFXSimFile(const TressFX_ShapeParams& params, const wchar_t* simFilePath)
{
    // The TressFX_ShapeParams and the TressFXSimFileObject are somewhat redundant.
    // For consistency, I translate to the file object before calling into the file-oriented code.
    TressFXSimFileObject simFileObject;
    simFileObject.damping = params.damping;
    simFileObject.stiffnessForLocalShapeMatching = params.stiffnessForLocalShapeMatching;
    simFileObject.stiffnessForGlobalShapeMatching = params.stiffnessForGlobalShapeMatching;
    simFileObject.globalShapeMatchingEffectiveRange = params.globalShapeMatchingEffectiveRange;

    WriteTFXSimFile(simFilePath, simFileObject);
}


void SaveTFXSimFile()
{
    int sectionIndex = GetSelectedSectionIndex();
    TressFX_SimulationParams* pSimParams = GetCurrentSimulationParams();


    //TCHAR curDir[MAX_PATH];
    //GetCurrentDirectory(MAX_PATH, curDir);

    wchar_t pSimFilePath[MAX_PATH];

    wcscpy_s(pSimFilePath, MAX_PATH, g_TfxProject.mTFXSimFilename[sectionIndex].c_str());


    OPENFILENAME OFN =
    {
        sizeof(OPENFILENAME),
        NULL, NULL,
        L"tfxsim Files (*.tfxsim)\0*.tfxsim\0All(*.*)\0*.*\0\0",
        NULL, 0, 1,
        pSimFilePath, MAX_PATH,
        NULL, 0, // File title
        g_TfxProject.mProjectDir.c_str(),  // initial dir
        L"Save TressFX Sim File Dialog",
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 0,
        0, L"tfxsim", 0,
        NULL, NULL
    };

    if (GetSaveFileName(&OFN))
    {
        SaveTFXSimFile(pSimParams->perSectionShapeParams[sectionIndex], pSimFilePath);
        g_TfxProject.mTFXSimFilename[sectionIndex] = pSimFilePath;
    }
}

//--------------------------------------------------------------------------------------
// Look for ini file with project filename, or return default.
//--------------------------------------------------------------------------------------
static std::wstring GetTFXProjFilename()
{
    std::wifstream inFile("TressFX.ini");
    if ( inFile )
    {
        std::wstring sLine;
        while (!inFile.eof())
        {
            getline(inFile, sLine);

            std::wistringstream iss(sLine);
            std::wstring candidate;
            while ( iss >> candidate )
            {
                if (candidate.length() > 0 && candidate[0] != L'#')
                {
                    return candidate;
                }
            }
        }
    }
    return std::wstring(DEFAULT_TFXPROJFILENAME);
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependent upon the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    TIMER_Init(pd3dDevice);

    HRESULT hr;

    // Get device context
    ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();

    // GUI creation
    V_RETURN( g_DialogResourceManager.OnD3D11CreateDevice( pd3dDevice, pd3dImmediateContext ) );
    V_RETURN( g_D3DSettingsDlg.OnD3D11CreateDevice( pd3dDevice ) );
    g_pTxtHelper = new CDXUTTextHelper( pd3dDevice, pd3dImmediateContext, &g_DialogResourceManager, 15 );

    if (g_TfxProject.mProjectFile.empty())
    {
        // create a demo
        bool bFoundProjectFile = false;
        if (pUserContext && ((WCHAR*)pUserContext)[0] != L'\0')
        {
            bFoundProjectFile = g_TfxProject.Read((WCHAR*)pUserContext);
        }

        if (!bFoundProjectFile)
        {
            g_TfxProject.Read(GetTFXProjFilename().c_str());
        }

        // Zero clear some members of g_DemoModel. Some members of g_DemoModel[i], m_SceneMesh.meshOffsets for instance,
        // might be dynamically allocated and we'll decide whether to delete the array with respect to whether the pointer
        // is NULL or not in Destroy Demo.
        g_DemoModel.m_SceneMesh.meshOffsets = NULL;

        CreateDemo(g_TfxProject, true);
        // Generate shaders ( this is an async operation - call AMD::ShaderCache::ShadersReady() to find out if they are complete )
        static bool bFirstPass = true;
        if ( bFirstPass )
        {
            g_ShaderCache.GenerateShaders( AMD::ShaderCache::CREATE_TYPE_COMPILE_CHANGES );    // Only compile shaders that have changed (development mode)
            bFirstPass = false;
        }
    }
    else
    {
        DestroyDemo(true);
        CreateDemo(g_TfxProject, true);
    }

    // Create AMD_SDK resources here
    g_RenderHUD.OnCreateDevice( pd3dDevice );
    g_SimulationHUD.OnCreateDevice( pd3dDevice );
    g_MagnifyTool.OnCreateDevice( pd3dDevice );

#ifdef MEM_DEBUG
    g_pMemDebugDevice = pd3dDevice;
#endif

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                         const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnD3D11ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );
    V_RETURN( g_D3DSettingsDlg.OnD3D11ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );

    g_ScreenWidth = pBackBufferSurfaceDesc->Width;
    g_ScreenHeight = pBackBufferSurfaceDesc->Height;

    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams(FIELD_OF_VIEW, fAspectRatio, g_nearPlane, g_farPlane);
    g_Camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
    g_Camera.SetButtonMasks(NULL, MOUSE_WHEEL, MOUSE_LEFT_BUTTON );

    // Set GUI size and locations
    g_RenderHUD.m_GUI.SetLocation( pBackBufferSurfaceDesc->Width - AMD::HUD::iDialogWidth, 0 );
    g_RenderHUD.m_GUI.SetSize( AMD::HUD::iDialogWidth, pBackBufferSurfaceDesc->Height  );
    g_RenderHUD.OnResizedSwapChain( pBackBufferSurfaceDesc );

    g_SimulationHUD.m_GUI.SetLocation( pBackBufferSurfaceDesc->Width - AMD::HUD::iDialogWidth, 0 );
    g_SimulationHUD.m_GUI.SetSize( AMD::HUD::iDialogWidth, pBackBufferSurfaceDesc->Height  );
    g_SimulationHUD.OnResizedSwapChain( pBackBufferSurfaceDesc );


    // Resize the buffers used for hair rendering
    g_DemoModel.m_TressFXParams.backBufferWidth = pBackBufferSurfaceDesc->Width;
    g_DemoModel.m_TressFXParams.backBufferHeight = pBackBufferSurfaceDesc->Height;

    if ( g_DemoModel.m_bInitialized )
    {
        TressFX_Resize(g_DemoModel.m_TressFXParams);
    }

    // Magnify tool will capture from the color buffer
    g_MagnifyTool.OnResizedSwapChain( pd3dDevice, pSwapChain, pBackBufferSurfaceDesc, pUserContext,
        pBackBufferSurfaceDesc->Width - AMD::HUD::iDialogWidth, 0 );
    D3D11_RENDER_TARGET_VIEW_DESC RTDesc;
    ID3D11Resource* pTempRTResource;
    DXUTGetD3D11RenderTargetView()->GetResource( &pTempRTResource );
    DXUTGetD3D11RenderTargetView()->GetDesc( &RTDesc );
    g_MagnifyTool.SetSourceResources( pTempRTResource, RTDesc.Format,
                DXUTGetDXGIBackBufferSurfaceDesc()->Width, DXUTGetDXGIBackBufferSurfaceDesc()->Height,
                DXUTGetDXGIBackBufferSurfaceDesc()->SampleDesc.Count );
    g_MagnifyTool.SetPixelRegion( 128 );
    g_MagnifyTool.SetScale( 5 );
    SAFE_RELEASE( pTempRTResource );

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
                                  double fTime, float fElapsedTime, void* pUserContext )
{
    TIMER_Reset();

    //---------------------------------
    // Settings dialog if it is active.
    //---------------------------------
    if ( g_D3DSettingsDlg.IsActive() )
    {
        g_D3DSettingsDlg.OnRender( fElapsedTime );
        return;
    }

    //-------------------------------------------
    // Clear the render target and depth stencil
    //-------------------------------------------
    float ClearColor[4] = { 0.25f, 0.25f, 0.35f, 0.0f };
    ID3D11RenderTargetView* pPrimaryRTV = DXUTGetD3D11RenderTargetView();
    ID3D11DepthStencilView* pPrimaryDSV = DXUTGetD3D11DepthStencilView();
    pd3dImmediateContext->ClearRenderTargetView( pPrimaryRTV, ClearColor );
    pd3dImmediateContext->ClearDepthStencilView( pPrimaryDSV, D3D11_CLEAR_DEPTH, 1.0f, 0 );

    DemoModel *pModel = &g_DemoModel;

    if ( g_ShaderCache.ShadersReady() && pModel->m_bInitialized )
    {
        TIMER_Begin( 0, L"Effect" );
        {
            TressFX_HairParams *pHairParams = &pModel->m_TressFXParams.hairParams;
            CSceneRender *pSceneRender = &pModel->m_SceneRender;

            pModel->m_TressFXParams.modelTransformForHead = pModel->m_modelTransformForHead;
            pModel->m_TressFXParams.eyePoint = g_Camera.GetEyePt();
            pModel->m_TressFXParams.lightPosition = g_lightEyePt;
            XMMATRIX mView = g_Camera.GetViewMatrix();
            XMMATRIX mProj = g_Camera.GetProjMatrix();
            pModel->m_TressFXParams.mViewProj = mView * mProj;
            TressFX_Begin(pModel->m_TressFXParams);

            pSceneRender->UpdateSceneCB(pModel->m_TressFXParams);

            // only need to save the mesh sizes and untransformed vertices once
            if (pModel->m_SceneMesh.pMeshVertices == NULL)
            {
                pSceneRender->StreamOutVertices(pd3dImmediateContext, false);
                pModel->m_SceneMesh.pMeshVertices = pSceneRender->m_pMeshVerticesSRV;
            }

            // scene mesh animation
            if (g_animate)
            {
                pSceneRender->StreamOutVertices(pd3dImmediateContext, true);
            }

            pModel->m_SceneMesh.pTransformedVerts = pSceneRender->m_pTransformedSRV;
            TressFX_GenerateTransforms(pModel->m_TressFXParams, pModel->m_SceneMesh);

            if (pHairParams->bSimulation)
            {
                pModel->m_TressFXParams.targetFrameRate = g_targetFrameRateForSimulation;
                pModel->m_TressFXParams.simulationParams.windMag = g_TfxProject.windMag;

                TIMER_Begin(0, L"Simulation");
                TressFX_Simulate(pModel->m_TressFXParams, fElapsedTime);
                TIMER_End(); // Simulation

                pModel->m_TressFXParams.bWarp = false; // only needed for 1 frame
            }
            else
            {
                TressFX_ApplyRigidTransforms(pModel->m_TressFXParams);
            }


            TIMER_Begin( 0, L"GenerateShadow" );
            if ( pHairParams->shadowTechnique != NONE_SHADOW_INDEX)
            {
                TressFX_RenderShadowMap(pModel->m_TressFXParams);
                pSceneRender->GenerateShadowMap(pModel->m_TressFXParams);
            }
            TIMER_End(); // GenerateShadow

            TIMER_Begin( 0, L"RenderScene" );
            pSceneRender->RenderScene(pModel->m_TressFXParams,  pHairParams->shadowTechnique != NONE_SHADOW_INDEX);
            TIMER_End(); // RenderScene

            if (pModel->m_bEnableLOD)
            {
                // Adjust the density and the thickness based on distance from camera to hair
                const XMVECTOR modelCenter = XMVector3TransformCoord(XMLoadFloat3(&pSceneRender->m_MeshBBoxCenter), pModel->m_modelTransformForHead);
                XMVECTOR vCameraToHair = modelCenter - g_Camera.GetEyePt();
                float distance = XMVectorGetX(XMVector3Length(vCameraToHair));
                if (distance >  g_maxLodDistance) { distance = g_maxLodDistance; }
                if (distance <= g_minLodDistance) { distance = 0; }
                else                              { distance -= g_minLodDistance; }
                float scaledDistance = distance / (g_maxLodDistance - g_minLodDistance);
                 pHairParams->density = (1 - scaledDistance) * pModel->m_density + scaledDistance * MIN_HAIR_DENSITY;
                 pHairParams->thickness = (1 - scaledDistance) * pModel->m_radius + scaledDistance * MAX_HAIR_RADIUS;
                // adjust the max overlapping fragment count
                 pHairParams->maxFragments = (int)((1 - scaledDistance) * MAX_FRAGMENTS + scaledDistance * MIN_FRAGMENTS);
                // adjust the shadow map alpha
                 pHairParams->shadowMapAlpha = (1 - scaledDistance) * pModel->m_alphaSM + scaledDistance * MAX_ALPHA_SM;
                 pHairParams->alphaThreshold = ALPHA_THRESHOLD / (pModel->m_density * pModel->m_density * pModel->m_density);

                // update the UI
                WCHAR szMessage[100];
                swprintf_s(szMessage, L"Density:%.4f",  pHairParams->density);
                g_RenderHUD.m_GUI.GetStatic( IDC_DENSITY_STATIC )->SetText( szMessage );
                g_RenderHUD.m_GUI.GetSlider( IDC_DENSITY )->SetValue((int)(100 * ( pHairParams->density - g_hairDensity.min) / (g_hairDensity.max - g_hairDensity.min)));
                swprintf_s(szMessage, L"Hair thickness:%.4f",  pHairParams->thickness);
                g_RenderHUD.m_GUI.GetStatic( IDC_FIBERRADIUS_STATIC )->SetText( szMessage );
                g_RenderHUD.m_GUI.GetSlider( IDC_FIBERRADIUS )->SetValue((int)(100 * ( pHairParams->thickness - g_fiberRadius.min) / (g_fiberRadius.max - g_fiberRadius.min)));
                swprintf_s(szMessage, L" Self-shadowing:%.4f",  pHairParams->shadowMapAlpha);
                g_RenderHUD.m_GUI.GetStatic( IDC_OPACITYSM_STATIC )->SetText( szMessage );
                g_RenderHUD.m_GUI.GetSlider( IDC_OPACITYSM )->SetValue((int)(100 * ( pHairParams->shadowMapAlpha - g_hairAlphaSM.min) / (g_hairAlphaSM.max - g_hairAlphaSM.min)));
            }

            TIMER_Begin( 0, L"RenderHair" );
            TressFX_Render(pModel->m_TressFXParams);
            TIMER_End(); // RenderHair

            TressFX_End(pModel->m_TressFXParams);
        }

        TIMER_End(); // Effect
    }

    DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" );

    if ( g_ShaderCache.ShadersReady() )
    {
        ID3D11BlendState *pBlendState;
        FLOAT blendFactor[4];
        UINT sampleMask;
        // save the current blend state
        pd3dImmediateContext->OMGetBlendState(&pBlendState, blendFactor, &sampleMask);

        if ( g_bShowUI )
        {
            g_MagnifyTool.Render();
            g_RenderHUD.OnRender( fElapsedTime );
            g_SimulationHUD.OnRender( fElapsedTime );

            RenderText();
        }

        // restore the saved blend state
        pd3dImmediateContext->OMSetBlendState(pBlendState, blendFactor, sampleMask);
    }
    else
    {
        // Render shader cache progress if still processing
        g_ShaderCache.RenderProgress( g_pTxtHelper, 15, XMVectorSet( 1.0f, 1.0f, 0.0f, 1.0f ) );
    }

    DXUT_EndPerfEvent(/*HUD / Stats*/);
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
{
    g_DialogResourceManager.OnD3D11ReleasingSwapChain();
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
{
    g_DialogResourceManager.OnD3D11DestroyDevice();
    g_D3DSettingsDlg.OnD3D11DestroyDevice();
    DXUTGetGlobalResourceCache().OnDestroyDevice();
    SAFE_DELETE( g_pTxtHelper );

    DestroyDemo(true);

    // Destroy AMD_SDK resources here
    g_ShaderCache.OnDestroyDevice();
    g_RenderHUD.OnDestroyDevice();
    g_SimulationHUD.OnDestroyDevice();
    g_MagnifyTool.OnDestroyDevice();
    TIMER_Destroy();

#ifdef MEM_DEBUG
    ID3D11Debug *pd3dDebug;
    g_pMemDebugDevice->QueryInterface(__uuidof(ID3D11Debug), (LPVOID *) &pd3dDebug);
    pd3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
}
