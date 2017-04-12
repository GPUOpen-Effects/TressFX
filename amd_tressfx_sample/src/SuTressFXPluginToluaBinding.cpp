//---------------------------------------------------------------------------------------
// Lua binding.
//-------------------------------------------------------------------------------------
//
// Copyright (c) 2017 Advanced Micro Devices, Inc. All rights reserved.
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
//

/*
** Lua binding: SuTressFXPlugin
** Generated automatically by tolua++-1.0.92 on 03/24/17 16:43:13.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_SuTressFXPlugin_open (lua_State* tolua_S);

#include "PCH.h"
// disable warnings from tolua generated code.
#ifdef WIN32
#pragma warning(disable: 4800)
#pragma warning(disable: 4018)
#endif
#include "prebuilt/include/Common/include/SuTypes.h"
#include "SuEffect.h"
#include "src/SuTressFXPlugin.h"
#include "../amd_tressfx/inc/TressFXSettings.h"
#include "src/Lights/SuLight.h"
#include "src/Lights/SuLightController.h"
#include "src/Lights/SuDirectionalLight.h"
#include "src/Lights/SuPointLight.h"
#include "src/Lights/SuSpotLight.h"
#include "src/Lights/SuVolumeLight.h"
#include "src/Lights/SuAreaLight.h"
#include "src/Lights/SuObjectLightTable.h"
#include "src/Lights/SuLightManager.h"
#include "src/Lights/SuSpotLightCameraController.h"
#include "src/Lights/SuLightVisualizer.h"
#include  "tolua++.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_SuSpotLightCameraController (lua_State* tolua_S)
{
 SuSpotLightCameraController* self = (SuSpotLightCameraController*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_SuSpotLight (lua_State* tolua_S)
{
 SuSpotLight* self = (SuSpotLight*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TressFXSimulationSettings (lua_State* tolua_S)
{
 TressFXSimulationSettings* self = (TressFXSimulationSettings*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_SuDirectionalLight (lua_State* tolua_S)
{
 SuDirectionalLight* self = (SuDirectionalLight*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_SuPointLight (lua_State* tolua_S)
{
 SuPointLight* self = (SuPointLight*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_SuLightVisualizer (lua_State* tolua_S)
{
 SuLightVisualizer* self = (SuLightVisualizer*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_SuVolumeLight (lua_State* tolua_S)
{
 SuVolumeLight* self = (SuVolumeLight*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_SuLightController (lua_State* tolua_S)
{
 SuLightController* self = (SuLightController*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_SuObjectLightTable (lua_State* tolua_S)
{
 SuObjectLightTable* self = (SuObjectLightTable*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_SuLight (lua_State* tolua_S)
{
 SuLight* self = (SuLight*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_SuAreaLight (lua_State* tolua_S)
{
 SuAreaLight* self = (SuAreaLight*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_SuLightManager (lua_State* tolua_S)
{
 SuLightManager* self = (SuLightManager*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_SuVector3 (lua_State* tolua_S)
{
 SuVector3* self = (SuVector3*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_SuMatrix4 (lua_State* tolua_S)
{
 SuMatrix4* self = (SuMatrix4*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"SuSpotLight");
 tolua_usertype(tolua_S,"TressFXSimulationSettings");
 tolua_usertype(tolua_S,"SuQuat");
 tolua_usertype(tolua_S,"SuEffectReloadListener");
 tolua_usertype(tolua_S,"SuPointLight");
 tolua_usertype(tolua_S,"SuPlugin");
 tolua_usertype(tolua_S,"SuEffectPtr");
 tolua_usertype(tolua_S,"virtualSuEffectReloadListener");
 tolua_usertype(tolua_S,"SuAreaLight");
 tolua_usertype(tolua_S,"SuLightManager");
 tolua_usertype(tolua_S,"SuPoint3");
 tolua_usertype(tolua_S,"SuObjectLightTable");
 tolua_usertype(tolua_S,"SuSpotLightCameraController");
 tolua_usertype(tolua_S,"SuLightVisualizer");
 tolua_usertype(tolua_S,"SuCameraController");
 tolua_usertype(tolua_S,"SuVolumeLight");
 tolua_usertype(tolua_S,"SuDirectionalLight");
 tolua_usertype(tolua_S,"SuBoundingBox");
 tolua_usertype(tolua_S,"SuObject");
 tolua_usertype(tolua_S,"SuAnimationCurvePtr");
 tolua_usertype(tolua_S,"SuVector4");
 tolua_usertype(tolua_S,"SuLightController");
 tolua_usertype(tolua_S,"SuFrustum");
 tolua_usertype(tolua_S,"SuStringArray");
 tolua_usertype(tolua_S,"SuLight");
 tolua_usertype(tolua_S,"SuTressFXPlugin");
 tolua_usertype(tolua_S,"SuVector3");
 tolua_usertype(tolua_S,"SuMatrix4");
}

/* method: LoadHairAsset of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_LoadHairAsset00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_LoadHairAsset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LoadHairAsset'", NULL);
#endif
  {
   self->LoadHairAsset();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LoadHairAsset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Shutdown of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_Shutdown00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_Shutdown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Shutdown'", NULL);
#endif
  {
   self->Shutdown();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Shutdown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DrawHairShadows of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_DrawHairShadows00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_DrawHairShadows00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DrawHairShadows'", NULL);
#endif
  {
   self->DrawHairShadows();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DrawHairShadows'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: EnableAnimation of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_EnableAnimation00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_EnableAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
  bool bEnable = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EnableAnimation'", NULL);
#endif
  {
   self->EnableAnimation(bEnable);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EnableAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnResize of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_OnResize00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_OnResize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
  int width = ((int)  tolua_tonumber(tolua_S,2,0));
  int height = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnResize'", NULL);
#endif
  {
   self->OnResize(width,height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnResize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Break of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_Break00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_Break00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Break'", NULL);
#endif
  {
   self->Break();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Break'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DrawHair of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_DrawHair00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_DrawHair00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DrawHair'", NULL);
#endif
  {
   self->DrawHair();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DrawHair'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Simulate of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_Simulate00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_Simulate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
   double fTime = ((  double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Simulate'", NULL);
#endif
  {
   self->Simulate(fTime);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Simulate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SignalSimulateStart of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_SignalSimulateStart00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_SignalSimulateStart00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SignalSimulateStart'", NULL);
#endif
  {
   self->SignalSimulateStart();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SignalSimulateStart'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WaitSimulateDone of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_WaitSimulateDone00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_WaitSimulateDone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WaitSimulateDone'", NULL);
#endif
  {
   self->WaitSimulateDone();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WaitSimulateDone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: EnableUpdatingCollisionMesh of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_EnableUpdatingCollisionMesh00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_EnableUpdatingCollisionMesh00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
  bool bUpdateCollMesh = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EnableUpdatingCollisionMesh'", NULL);
#endif
  {
   self->EnableUpdatingCollisionMesh(bUpdateCollMesh);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EnableUpdatingCollisionMesh'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: EnableSDFCollisionResponse of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_EnableSDFCollisionResponse00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_EnableSDFCollisionResponse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
  bool bSDFCollisionResponse = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EnableSDFCollisionResponse'", NULL);
#endif
  {
   self->EnableSDFCollisionResponse(bSDFCollisionResponse);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EnableSDFCollisionResponse'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: UpdateSimulationParameters of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_UpdateSimulationParameters00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_UpdateSimulationParameters00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const TressFXSimulationSettings",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const TressFXSimulationSettings",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
  const TressFXSimulationSettings* mohawkParameters = ((const TressFXSimulationSettings*)  tolua_tousertype(tolua_S,2,0));
  const TressFXSimulationSettings* shortHairParameters = ((const TressFXSimulationSettings*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'UpdateSimulationParameters'", NULL);
#endif
  {
   self->UpdateSimulationParameters(*mohawkParameters,*shortHairParameters);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UpdateSimulationParameters'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ToggleShortCut of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_ToggleShortCut00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_ToggleShortCut00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ToggleShortCut'", NULL);
#endif
  {
   self->ToggleShortCut();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ToggleShortCut'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DrawCollisionMesh of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_DrawCollisionMesh00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_DrawCollisionMesh00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DrawCollisionMesh'", NULL);
#endif
  {
   self->DrawCollisionMesh();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DrawCollisionMesh'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DrawSDF of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_DrawSDF00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_DrawSDF00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DrawSDF'", NULL);
#endif
  {
   self->DrawSDF();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DrawSDF'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumTressFXObjects of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_GetNumTressFXObjects00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_GetNumTressFXObjects00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumTressFXObjects'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetNumTressFXObjects();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumTressFXObjects'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTressFXObject of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuTressFXPlugin_GetTressFXObject00
static int tolua_SuTressFXPlugin_SuTressFXPlugin_GetTressFXObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuTressFXPlugin",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTressFXObject'", NULL);
#endif
  {
   SuObject* tolua_ret = (SuObject*)  self->GetTressFXObject(index);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTressFXObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __SuEffectReloadListener__ of class  SuTressFXPlugin */
#ifndef TOLUA_DISABLE_tolua_get_SuTressFXPlugin___SuEffectReloadListener__
static int tolua_get_SuTressFXPlugin___SuEffectReloadListener__(lua_State* tolua_S)
{
  SuTressFXPlugin* self = (SuTressFXPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__SuEffectReloadListener__'",NULL);
#endif
#ifdef __cplusplus
   tolua_pushusertype(tolua_S,(void*)static_cast<SuEffectReloadListener*>(self), "SuEffectReloadListener");
#else
   tolua_pushusertype(tolua_S,(void*)((SuEffectReloadListener*)self), "SuEffectReloadListener");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_TressFXSimulationSettings_new00
static int tolua_SuTressFXPlugin_TressFXSimulationSettings_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TressFXSimulationSettings",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   TressFXSimulationSettings* tolua_ret = (TressFXSimulationSettings*)  Mtolua_new((TressFXSimulationSettings)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TressFXSimulationSettings");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_TressFXSimulationSettings_new00_local
static int tolua_SuTressFXPlugin_TressFXSimulationSettings_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TressFXSimulationSettings",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   TressFXSimulationSettings* tolua_ret = (TressFXSimulationSettings*)  Mtolua_new((TressFXSimulationSettings)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TressFXSimulationSettings");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: vspCoeff of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_get_TressFXSimulationSettings_vspCoeff
static int tolua_get_TressFXSimulationSettings_vspCoeff(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vspCoeff'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_vspCoeff);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: vspCoeff of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_set_TressFXSimulationSettings_vspCoeff
static int tolua_set_TressFXSimulationSettings_vspCoeff(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vspCoeff'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_vspCoeff = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: vspAccelThreshold of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_get_TressFXSimulationSettings_vspAccelThreshold
static int tolua_get_TressFXSimulationSettings_vspAccelThreshold(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vspAccelThreshold'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_vspAccelThreshold);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: vspAccelThreshold of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_set_TressFXSimulationSettings_vspAccelThreshold
static int tolua_set_TressFXSimulationSettings_vspAccelThreshold(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vspAccelThreshold'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_vspAccelThreshold = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: localConstraintStiffness of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_get_TressFXSimulationSettings_localConstraintStiffness
static int tolua_get_TressFXSimulationSettings_localConstraintStiffness(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'localConstraintStiffness'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_localConstraintStiffness);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: localConstraintStiffness of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_set_TressFXSimulationSettings_localConstraintStiffness
static int tolua_set_TressFXSimulationSettings_localConstraintStiffness(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'localConstraintStiffness'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_localConstraintStiffness = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: localConstraintsIterations of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_get_TressFXSimulationSettings_localConstraintsIterations
static int tolua_get_TressFXSimulationSettings_localConstraintsIterations(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'localConstraintsIterations'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_localConstraintsIterations);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: localConstraintsIterations of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_set_TressFXSimulationSettings_localConstraintsIterations
static int tolua_set_TressFXSimulationSettings_localConstraintsIterations(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'localConstraintsIterations'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_localConstraintsIterations = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: globalConstraintStiffness of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_get_TressFXSimulationSettings_globalConstraintStiffness
static int tolua_get_TressFXSimulationSettings_globalConstraintStiffness(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'globalConstraintStiffness'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_globalConstraintStiffness);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: globalConstraintStiffness of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_set_TressFXSimulationSettings_globalConstraintStiffness
static int tolua_set_TressFXSimulationSettings_globalConstraintStiffness(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'globalConstraintStiffness'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_globalConstraintStiffness = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: globalConstraintsRange of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_get_TressFXSimulationSettings_globalConstraintsRange
static int tolua_get_TressFXSimulationSettings_globalConstraintsRange(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'globalConstraintsRange'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_globalConstraintsRange);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: globalConstraintsRange of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_set_TressFXSimulationSettings_globalConstraintsRange
static int tolua_set_TressFXSimulationSettings_globalConstraintsRange(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'globalConstraintsRange'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_globalConstraintsRange = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lengthConstraintsIterations of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_get_TressFXSimulationSettings_lengthConstraintsIterations
static int tolua_get_TressFXSimulationSettings_lengthConstraintsIterations(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'lengthConstraintsIterations'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_lengthConstraintsIterations);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lengthConstraintsIterations of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_set_TressFXSimulationSettings_lengthConstraintsIterations
static int tolua_set_TressFXSimulationSettings_lengthConstraintsIterations(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'lengthConstraintsIterations'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_lengthConstraintsIterations = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: damping of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_get_TressFXSimulationSettings_damping
static int tolua_get_TressFXSimulationSettings_damping(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'damping'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_damping);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: damping of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_set_TressFXSimulationSettings_damping
static int tolua_set_TressFXSimulationSettings_damping(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'damping'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_damping = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gravityMagnitude of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_get_TressFXSimulationSettings_gravityMagnitude
static int tolua_get_TressFXSimulationSettings_gravityMagnitude(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gravityMagnitude'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_gravityMagnitude);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gravityMagnitude of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_set_TressFXSimulationSettings_gravityMagnitude
static int tolua_set_TressFXSimulationSettings_gravityMagnitude(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gravityMagnitude'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_gravityMagnitude = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: tipSeparation of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_get_TressFXSimulationSettings_tipSeparation
static int tolua_get_TressFXSimulationSettings_tipSeparation(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tipSeparation'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_tipSeparation);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: tipSeparation of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_set_TressFXSimulationSettings_tipSeparation
static int tolua_set_TressFXSimulationSettings_tipSeparation(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tipSeparation'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_tipSeparation = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: windMagnitude of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_get_TressFXSimulationSettings_windMagnitude
static int tolua_get_TressFXSimulationSettings_windMagnitude(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'windMagnitude'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_windMagnitude);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: windMagnitude of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_set_TressFXSimulationSettings_windMagnitude
static int tolua_set_TressFXSimulationSettings_windMagnitude(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'windMagnitude'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_windMagnitude = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: windDirection of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_get_SuTressFXPlugin_TressFXSimulationSettings_windDirection
static int tolua_get_SuTressFXPlugin_TressFXSimulationSettings_windDirection(lua_State* tolua_S)
{
 int tolua_index;
  TressFXSimulationSettings* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (TressFXSimulationSettings*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=3)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_windDirection[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: windDirection of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_set_SuTressFXPlugin_TressFXSimulationSettings_windDirection
static int tolua_set_SuTressFXPlugin_TressFXSimulationSettings_windDirection(lua_State* tolua_S)
{
 int tolua_index;
  TressFXSimulationSettings* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (TressFXSimulationSettings*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=3)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->m_windDirection[tolua_index] = ((float)  tolua_tonumber(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: windAngleRadians of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_get_TressFXSimulationSettings_windAngleRadians
static int tolua_get_TressFXSimulationSettings_windAngleRadians(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'windAngleRadians'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_windAngleRadians);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: windAngleRadians of class  TressFXSimulationSettings */
#ifndef TOLUA_DISABLE_tolua_set_TressFXSimulationSettings_windAngleRadians
static int tolua_set_TressFXSimulationSettings_windAngleRadians(lua_State* tolua_S)
{
  TressFXSimulationSettings* self = (TressFXSimulationSettings*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'windAngleRadians'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_windAngleRadians = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_delete00
static int tolua_SuTressFXPlugin_SuLight_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLight* self = (SuLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetType of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_GetType00
static int tolua_SuTressFXPlugin_SuLight_GetType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetType'", NULL);
#endif
  {
   SuLight::LightType tolua_ret = (SuLight::LightType)  self->GetType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsVisible of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_IsVisible00
static int tolua_SuTressFXPlugin_SuLight_IsVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsVisible'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsVisible();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetVisible of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_SetVisible00
static int tolua_SuTressFXPlugin_SuLight_SetVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLight",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLight* self = (SuLight*)  tolua_tousertype(tolua_S,1,0);
  bool bVisible = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetVisible'", NULL);
#endif
  {
   self->SetVisible(bVisible);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetIntensity of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_SetIntensity00
static int tolua_SuTressFXPlugin_SuLight_SetIntensity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLight",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLight* self = (SuLight*)  tolua_tousertype(tolua_S,1,0);
   float fIntensity = ((  float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetIntensity'", NULL);
#endif
  {
   self->SetIntensity(fIntensity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetIntensity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetIntensity of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_GetIntensity00
static int tolua_SuTressFXPlugin_SuLight_GetIntensity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetIntensity'", NULL);
#endif
  {
    float tolua_ret = (  float)  self->GetIntensity();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetIntensity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetColor of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_SetColor00
static int tolua_SuTressFXPlugin_SuLight_SetColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuVector4",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLight* self = (SuLight*)  tolua_tousertype(tolua_S,1,0);
  const SuVector4* rColor = ((const SuVector4*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetColor'", NULL);
#endif
  {
   self->SetColor(*rColor);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetColor of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_GetColor00
static int tolua_SuTressFXPlugin_SuLight_GetColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetColor'", NULL);
#endif
  {
   const SuVector4& tolua_ret = (const SuVector4&)  self->GetColor();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuVector4");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDiffuse of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_SetDiffuse00
static int tolua_SuTressFXPlugin_SuLight_SetDiffuse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLight",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLight* self = (SuLight*)  tolua_tousertype(tolua_S,1,0);
  bool bEnable = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDiffuse'", NULL);
#endif
  {
   self->SetDiffuse(bEnable);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDiffuse'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsDiffuse of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_IsDiffuse00
static int tolua_SuTressFXPlugin_SuLight_IsDiffuse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsDiffuse'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsDiffuse();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsDiffuse'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSpecular of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_SetSpecular00
static int tolua_SuTressFXPlugin_SuLight_SetSpecular00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLight",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLight* self = (SuLight*)  tolua_tousertype(tolua_S,1,0);
  bool bEnable = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSpecular'", NULL);
#endif
  {
   self->SetSpecular(bEnable);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSpecular'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsSpecular of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_IsSpecular00
static int tolua_SuTressFXPlugin_SuLight_IsSpecular00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSpecular'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsSpecular();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSpecular'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFalloff of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_SetFalloff00
static int tolua_SuTressFXPlugin_SuLight_SetFalloff00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLight",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLight* self = (SuLight*)  tolua_tousertype(tolua_S,1,0);
  SuLight::FalloffType type = ((SuLight::FalloffType) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFalloff'", NULL);
#endif
  {
   self->SetFalloff(type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFalloff'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFalloff of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_GetFalloff00
static int tolua_SuTressFXPlugin_SuLight_GetFalloff00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFalloff'", NULL);
#endif
  {
   SuLight::FalloffType tolua_ret = (SuLight::FalloffType)  self->GetFalloff();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFalloff'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ComputeDistanceAttenuation of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_ComputeDistanceAttenuation00
static int tolua_SuTressFXPlugin_SuLight_ComputeDistanceAttenuation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
   float fDist = ((  float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ComputeDistanceAttenuation'", NULL);
#endif
  {
    float tolua_ret = (  float)  self->ComputeDistanceAttenuation(fDist);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ComputeDistanceAttenuation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IlluminatesBoundingBox of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_IlluminatesBoundingBox00
static int tolua_SuTressFXPlugin_SuLight_IlluminatesBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuBoundingBox",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
  const SuBoundingBox* rBB = ((const SuBoundingBox*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IlluminatesBoundingBox'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IlluminatesBoundingBox(*rBB);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IlluminatesBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetController of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_GetController00
static int tolua_SuTressFXPlugin_SuLight_GetController00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetController'", NULL);
#endif
  {
   SuLightController* tolua_ret = (SuLightController*)  self->GetController();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuLightController");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetController'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Update of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_Update00
static int tolua_SuTressFXPlugin_SuLight_Update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLight",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLight* self = (SuLight*)  tolua_tousertype(tolua_S,1,0);
   double fTime = ((  double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Update'", NULL);
#endif
  {
   self->Update(fTime);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetName of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_GetName00
static int tolua_SuTressFXPlugin_SuLight_GetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetName'", NULL);
#endif
  {
   const SuString tolua_ret = (const SuString)  self->GetName();
   tolua_pushsushistring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddProperty of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_AddProperty00
static int tolua_SuTressFXPlugin_SuLight_AddProperty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLight",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLight* self = (SuLight*)  tolua_tousertype(tolua_S,1,0);
  const SuString rStrProperty = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddProperty'", NULL);
#endif
  {
   self->AddProperty(rStrProperty);
   tolua_pushsushistring(tolua_S,(const char*)rStrProperty);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddProperty'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HasProperty of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_HasProperty00
static int tolua_SuTressFXPlugin_SuLight_HasProperty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
  const SuString rStrProperty = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HasProperty'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->HasProperty(rStrProperty);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushsushistring(tolua_S,(const char*)rStrProperty);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HasProperty'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumProperties of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_GetNumProperties00
static int tolua_SuTressFXPlugin_SuLight_GetNumProperties00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumProperties'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->GetNumProperties();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumProperties'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetProperty of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_GetProperty00
static int tolua_SuTressFXPlugin_SuLight_GetProperty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLight",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLight* self = (SuLight*)  tolua_tousertype(tolua_S,1,0);
  unsigned int i = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetProperty'", NULL);
#endif
  {
   const SuString tolua_ret = (const SuString)  self->GetProperty(i);
   tolua_pushsushistring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetProperty'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetProperties of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_GetProperties00
static int tolua_SuTressFXPlugin_SuLight_GetProperties00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetProperties'", NULL);
#endif
  {
   const SuStringArray& tolua_ret = (const SuStringArray&)  self->GetProperties();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuStringArray");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetProperties'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetIndex of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_SetIndex00
static int tolua_SuTressFXPlugin_SuLight_SetIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLight",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLight* self = (SuLight*)  tolua_tousertype(tolua_S,1,0);
   int nIndex = ((  int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetIndex'", NULL);
#endif
  {
   self->SetIndex(nIndex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetIndex of class  SuLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLight_GetIndex00
static int tolua_SuTressFXPlugin_SuLight_GetIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLight* self = (const SuLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetIndex'", NULL);
#endif
  {
    int tolua_ret = (  int)  self->GetIndex();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  SuLightController */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightController_new00
static int tolua_SuTressFXPlugin_SuLightController_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuLightController",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,7,&tolua_err) || !tolua_isusertype(tolua_S,7,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,8,&tolua_err) || !tolua_isusertype(tolua_S,8,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,9,&tolua_err) || !tolua_isusertype(tolua_S,9,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,10,&tolua_err) || !tolua_isusertype(tolua_S,10,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,11,&tolua_err) || !tolua_isusertype(tolua_S,11,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,12,&tolua_err) || !tolua_isusertype(tolua_S,12,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,13,&tolua_err) || !tolua_isusertype(tolua_S,13,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,14,&tolua_err) || !tolua_isusertype(tolua_S,14,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,15,&tolua_err) || !tolua_isusertype(tolua_S,15,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,16,&tolua_err) || !tolua_isusertype(tolua_S,16,"SuAnimationCurvePtr",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,17,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuAnimationCurvePtr pColorCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,2,0));
  SuAnimationCurvePtr pIntensityCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,3,0));
  SuAnimationCurvePtr pDiffuseCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,4,0));
  SuAnimationCurvePtr pSpecularCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,5,0));
  SuAnimationCurvePtr pFalloffCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,6,0));
  SuAnimationCurvePtr pPosCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,7,0));
  SuAnimationCurvePtr pRadiusCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,8,0));
  SuAnimationCurvePtr pOrientationCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,9,0));
  SuAnimationCurvePtr pScaleCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,10,0));
  SuAnimationCurvePtr pConeAngleCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,11,0));
  SuAnimationCurvePtr pPenumbraAngleCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,12,0));
  SuAnimationCurvePtr pDropOffCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,13,0));
  SuAnimationCurvePtr pVisibilityCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,14,0));
  SuAnimationCurvePtr pLightShapeCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,15,0));
  SuAnimationCurvePtr pSushiLightIndirectCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,16,0));
  {
   SuLightController* tolua_ret = (SuLightController*)  Mtolua_new((SuLightController)(pColorCurve,pIntensityCurve,pDiffuseCurve,pSpecularCurve,pFalloffCurve,pPosCurve,pRadiusCurve,pOrientationCurve,pScaleCurve,pConeAngleCurve,pPenumbraAngleCurve,pDropOffCurve,pVisibilityCurve,pLightShapeCurve,pSushiLightIndirectCurve));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuLightController");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SuLightController */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightController_new00_local
static int tolua_SuTressFXPlugin_SuLightController_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuLightController",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,7,&tolua_err) || !tolua_isusertype(tolua_S,7,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,8,&tolua_err) || !tolua_isusertype(tolua_S,8,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,9,&tolua_err) || !tolua_isusertype(tolua_S,9,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,10,&tolua_err) || !tolua_isusertype(tolua_S,10,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,11,&tolua_err) || !tolua_isusertype(tolua_S,11,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,12,&tolua_err) || !tolua_isusertype(tolua_S,12,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,13,&tolua_err) || !tolua_isusertype(tolua_S,13,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,14,&tolua_err) || !tolua_isusertype(tolua_S,14,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,15,&tolua_err) || !tolua_isusertype(tolua_S,15,"SuAnimationCurvePtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,16,&tolua_err) || !tolua_isusertype(tolua_S,16,"SuAnimationCurvePtr",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,17,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuAnimationCurvePtr pColorCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,2,0));
  SuAnimationCurvePtr pIntensityCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,3,0));
  SuAnimationCurvePtr pDiffuseCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,4,0));
  SuAnimationCurvePtr pSpecularCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,5,0));
  SuAnimationCurvePtr pFalloffCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,6,0));
  SuAnimationCurvePtr pPosCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,7,0));
  SuAnimationCurvePtr pRadiusCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,8,0));
  SuAnimationCurvePtr pOrientationCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,9,0));
  SuAnimationCurvePtr pScaleCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,10,0));
  SuAnimationCurvePtr pConeAngleCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,11,0));
  SuAnimationCurvePtr pPenumbraAngleCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,12,0));
  SuAnimationCurvePtr pDropOffCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,13,0));
  SuAnimationCurvePtr pVisibilityCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,14,0));
  SuAnimationCurvePtr pLightShapeCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,15,0));
  SuAnimationCurvePtr pSushiLightIndirectCurve = *((SuAnimationCurvePtr*)  tolua_tousertype(tolua_S,16,0));
  {
   SuLightController* tolua_ret = (SuLightController*)  Mtolua_new((SuLightController)(pColorCurve,pIntensityCurve,pDiffuseCurve,pSpecularCurve,pFalloffCurve,pPosCurve,pRadiusCurve,pOrientationCurve,pScaleCurve,pConeAngleCurve,pPenumbraAngleCurve,pDropOffCurve,pVisibilityCurve,pLightShapeCurve,pSushiLightIndirectCurve));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuLightController");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  SuLightController */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightController_delete00
static int tolua_SuTressFXPlugin_SuLightController_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLightController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLightController* self = (SuLightController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Update of class  SuLightController */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightController_Update00
static int tolua_SuTressFXPlugin_SuLightController_Update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLightController",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLightController* self = (SuLightController*)  tolua_tousertype(tolua_S,1,0);
   double fTime = ((  double)  tolua_tonumber(tolua_S,2,0));
  SuLight* pLight = ((SuLight*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Update'", NULL);
#endif
  {
   self->Update(fTime,pLight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HasChanged of class  SuLightController */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightController_HasChanged00
static int tolua_SuTressFXPlugin_SuLightController_HasChanged00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLightController",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLightController* self = (const SuLightController*)  tolua_tousertype(tolua_S,1,0);
  SuLightController::CurveMask curveMask = ((SuLightController::CurveMask) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HasChanged'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->HasChanged(curveMask);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HasChanged'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsDynamic of class  SuLightController */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightController_IsDynamic00
static int tolua_SuTressFXPlugin_SuLightController_IsDynamic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLightController",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLightController* self = (const SuLightController*)  tolua_tousertype(tolua_S,1,0);
  SuLightController::CurveMask curveMask = ((SuLightController::CurveMask) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsDynamic'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsDynamic(curveMask);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsDynamic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  SuDirectionalLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuDirectionalLight_new00
static int tolua_SuTressFXPlugin_SuDirectionalLight_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuDirectionalLight",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuString rName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
  {
   SuDirectionalLight* tolua_ret = (SuDirectionalLight*)  Mtolua_new((SuDirectionalLight)(rName));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuDirectionalLight");
   tolua_pushsushistring(tolua_S,(const char*)rName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SuDirectionalLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuDirectionalLight_new00_local
static int tolua_SuTressFXPlugin_SuDirectionalLight_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuDirectionalLight",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuString rName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
  {
   SuDirectionalLight* tolua_ret = (SuDirectionalLight*)  Mtolua_new((SuDirectionalLight)(rName));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuDirectionalLight");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushsushistring(tolua_S,(const char*)rName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  SuDirectionalLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuDirectionalLight_delete00
static int tolua_SuTressFXPlugin_SuDirectionalLight_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuDirectionalLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuDirectionalLight* self = (SuDirectionalLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDirection of class  SuDirectionalLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuDirectionalLight_SetDirection00
static int tolua_SuTressFXPlugin_SuDirectionalLight_SetDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuDirectionalLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuVector3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuDirectionalLight* self = (SuDirectionalLight*)  tolua_tousertype(tolua_S,1,0);
  const SuVector3* rDir = ((const SuVector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDirection'", NULL);
#endif
  {
   self->SetDirection(*rDir);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetDirection of class  SuDirectionalLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuDirectionalLight_GetDirection00
static int tolua_SuTressFXPlugin_SuDirectionalLight_GetDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuDirectionalLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuDirectionalLight* self = (const SuDirectionalLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDirection'", NULL);
#endif
  {
   const SuVector3& tolua_ret = (const SuVector3&)  self->GetDirection();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuVector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IlluminatesBoundingBox of class  SuDirectionalLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuDirectionalLight_IlluminatesBoundingBox00
static int tolua_SuTressFXPlugin_SuDirectionalLight_IlluminatesBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuDirectionalLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuBoundingBox",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuDirectionalLight* self = (const SuDirectionalLight*)  tolua_tousertype(tolua_S,1,0);
  const SuBoundingBox* rBV = ((const SuBoundingBox*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IlluminatesBoundingBox'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IlluminatesBoundingBox(*rBV);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IlluminatesBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  SuPointLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuPointLight_new00
static int tolua_SuTressFXPlugin_SuPointLight_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuPointLight",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuString rName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
  {
   SuPointLight* tolua_ret = (SuPointLight*)  Mtolua_new((SuPointLight)(rName));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuPointLight");
   tolua_pushsushistring(tolua_S,(const char*)rName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SuPointLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuPointLight_new00_local
static int tolua_SuTressFXPlugin_SuPointLight_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuPointLight",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuString rName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
  {
   SuPointLight* tolua_ret = (SuPointLight*)  Mtolua_new((SuPointLight)(rName));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuPointLight");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushsushistring(tolua_S,(const char*)rName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  SuPointLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuPointLight_delete00
static int tolua_SuTressFXPlugin_SuPointLight_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuPointLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuPointLight* self = (SuPointLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPosition of class  SuPointLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuPointLight_SetPosition00
static int tolua_SuTressFXPlugin_SuPointLight_SetPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuPointLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuPoint3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuPointLight* self = (SuPointLight*)  tolua_tousertype(tolua_S,1,0);
  const SuPoint3* rPos = ((const SuPoint3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosition'", NULL);
#endif
  {
   self->SetPosition(*rPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPosition of class  SuPointLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuPointLight_GetPosition00
static int tolua_SuTressFXPlugin_SuPointLight_GetPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuPointLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuPointLight* self = (const SuPointLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosition'", NULL);
#endif
  {
   const SuPoint3& tolua_ret = (const SuPoint3&)  self->GetPosition();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuPoint3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRadius of class  SuPointLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuPointLight_SetRadius00
static int tolua_SuTressFXPlugin_SuPointLight_SetRadius00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuPointLight",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuPointLight* self = (SuPointLight*)  tolua_tousertype(tolua_S,1,0);
  const float fRadius = ((const float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRadius'", NULL);
#endif
  {
   self->SetRadius(fRadius);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRadius'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRadius of class  SuPointLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuPointLight_GetRadius00
static int tolua_SuTressFXPlugin_SuPointLight_GetRadius00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuPointLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuPointLight* self = (const SuPointLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRadius'", NULL);
#endif
  {
    float tolua_ret = (  float)  self->GetRadius();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRadius'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IlluminatesBoundingBox of class  SuPointLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuPointLight_IlluminatesBoundingBox00
static int tolua_SuTressFXPlugin_SuPointLight_IlluminatesBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuPointLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuBoundingBox",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuPointLight* self = (const SuPointLight*)  tolua_tousertype(tolua_S,1,0);
  const SuBoundingBox* rBV = ((const SuBoundingBox*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IlluminatesBoundingBox'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IlluminatesBoundingBox(*rBV);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IlluminatesBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_new00
static int tolua_SuTressFXPlugin_SuSpotLight_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuSpotLight",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuString rName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
  {
   SuSpotLight* tolua_ret = (SuSpotLight*)  Mtolua_new((SuSpotLight)(rName));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuSpotLight");
   tolua_pushsushistring(tolua_S,(const char*)rName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_new00_local
static int tolua_SuTressFXPlugin_SuSpotLight_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuSpotLight",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuString rName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
  {
   SuSpotLight* tolua_ret = (SuSpotLight*)  Mtolua_new((SuSpotLight)(rName));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuSpotLight");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushsushistring(tolua_S,(const char*)rName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_delete00
static int tolua_SuTressFXPlugin_SuSpotLight_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLight* self = (SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetDropOff of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_GetDropOff00
static int tolua_SuTressFXPlugin_SuSpotLight_GetDropOff00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDropOff'", NULL);
#endif
  {
    float tolua_ret = (  float)  self->GetDropOff();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDropOff'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDropOff of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_SetDropOff00
static int tolua_SuTressFXPlugin_SuSpotLight_SetDropOff00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuSpotLight",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLight* self = (SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
   float fDropOff = ((  float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDropOff'", NULL);
#endif
  {
   self->SetDropOff(fDropOff);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDropOff'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPosition of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_SetPosition00
static int tolua_SuTressFXPlugin_SuSpotLight_SetPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuSpotLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuPoint3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLight* self = (SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
  const SuPoint3* rPos = ((const SuPoint3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosition'", NULL);
#endif
  {
   self->SetPosition(*rPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPosition of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_GetPosition00
static int tolua_SuTressFXPlugin_SuSpotLight_GetPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosition'", NULL);
#endif
  {
   const SuPoint3& tolua_ret = (const SuPoint3&)  self->GetPosition();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuPoint3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDirection of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_SetDirection00
static int tolua_SuTressFXPlugin_SuSpotLight_SetDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuSpotLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuVector3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLight* self = (SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
  const SuVector3* rDirection = ((const SuVector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDirection'", NULL);
#endif
  {
   self->SetDirection(*rDirection);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetOrientation of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_SetOrientation00
static int tolua_SuTressFXPlugin_SuSpotLight_SetOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuSpotLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuVector3",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const SuVector3",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const SuVector3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLight* self = (SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
  const SuVector3* rDirection = ((const SuVector3*)  tolua_tousertype(tolua_S,2,0));
  const SuVector3* rUp = ((const SuVector3*)  tolua_tousertype(tolua_S,3,0));
  const SuVector3* rRight = ((const SuVector3*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetOrientation'", NULL);
#endif
  {
   self->SetOrientation(*rDirection,*rUp,*rRight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetLookAt of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_SetLookAt00
static int tolua_SuTressFXPlugin_SuSpotLight_SetLookAt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuSpotLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuPoint3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLight* self = (SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
  const SuPoint3* rLookAtPt = ((const SuPoint3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetLookAt'", NULL);
#endif
  {
   self->SetLookAt(*rLookAtPt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetLookAt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetDirection of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_GetDirection00
static int tolua_SuTressFXPlugin_SuSpotLight_GetDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDirection'", NULL);
#endif
  {
   const SuVector3& tolua_ret = (const SuVector3&)  self->GetDirection();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuVector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetUpVector of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_GetUpVector00
static int tolua_SuTressFXPlugin_SuSpotLight_GetUpVector00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetUpVector'", NULL);
#endif
  {
   const SuVector3& tolua_ret = (const SuVector3&)  self->GetUpVector();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuVector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetUpVector'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRightVector of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_GetRightVector00
static int tolua_SuTressFXPlugin_SuSpotLight_GetRightVector00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRightVector'", NULL);
#endif
  {
   const SuVector3& tolua_ret = (const SuVector3&)  self->GetRightVector();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuVector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRightVector'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetConeAngle of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_SetConeAngle00
static int tolua_SuTressFXPlugin_SuSpotLight_SetConeAngle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuSpotLight",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLight* self = (SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
   float fAngle = ((  float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetConeAngle'", NULL);
#endif
  {
   self->SetConeAngle(fAngle);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetConeAngle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetConeAngle of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_GetConeAngle00
static int tolua_SuTressFXPlugin_SuSpotLight_GetConeAngle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetConeAngle'", NULL);
#endif
  {
    float tolua_ret = (  float)  self->GetConeAngle();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetConeAngle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHotSpotAngle of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_SetHotSpotAngle00
static int tolua_SuTressFXPlugin_SuSpotLight_SetHotSpotAngle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuSpotLight",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLight* self = (SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
   float fAngle = ((  float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHotSpotAngle'", NULL);
#endif
  {
   self->SetHotSpotAngle(fAngle);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHotSpotAngle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHotSpotAngle of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_GetHotSpotAngle00
static int tolua_SuTressFXPlugin_SuSpotLight_GetHotSpotAngle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHotSpotAngle'", NULL);
#endif
  {
    float tolua_ret = (  float)  self->GetHotSpotAngle();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHotSpotAngle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNearPlaneDistance of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_SetNearPlaneDistance00
static int tolua_SuTressFXPlugin_SuSpotLight_SetNearPlaneDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuSpotLight",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLight* self = (SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
   float fDist = ((  float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNearPlaneDistance'", NULL);
#endif
  {
   self->SetNearPlaneDistance(fDist);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNearPlaneDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNearPlaneDistance of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_GetNearPlaneDistance00
static int tolua_SuTressFXPlugin_SuSpotLight_GetNearPlaneDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNearPlaneDistance'", NULL);
#endif
  {
    float tolua_ret = (  float)  self->GetNearPlaneDistance();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNearPlaneDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFarPlaneDistance of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_SetFarPlaneDistance00
static int tolua_SuTressFXPlugin_SuSpotLight_SetFarPlaneDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuSpotLight",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLight* self = (SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
   float fDist = ((  float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFarPlaneDistance'", NULL);
#endif
  {
   self->SetFarPlaneDistance(fDist);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFarPlaneDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFarPlaneDistance of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_GetFarPlaneDistance00
static int tolua_SuTressFXPlugin_SuSpotLight_GetFarPlaneDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFarPlaneDistance'", NULL);
#endif
  {
    float tolua_ret = (  float)  self->GetFarPlaneDistance();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFarPlaneDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetViewMatrix of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_GetViewMatrix00
static int tolua_SuTressFXPlugin_SuSpotLight_GetViewMatrix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetViewMatrix'", NULL);
#endif
  {
   const SuMatrix4& tolua_ret = (const SuMatrix4&)  self->GetViewMatrix();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuMatrix4");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetViewMatrix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetProjectionMatrix of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_GetProjectionMatrix00
static int tolua_SuTressFXPlugin_SuSpotLight_GetProjectionMatrix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetProjectionMatrix'", NULL);
#endif
  {
   const SuMatrix4& tolua_ret = (const SuMatrix4&)  self->GetProjectionMatrix();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuMatrix4");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetProjectionMatrix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetViewProjectionMatrix of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_GetViewProjectionMatrix00
static int tolua_SuTressFXPlugin_SuSpotLight_GetViewProjectionMatrix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetViewProjectionMatrix'", NULL);
#endif
  {
   const SuMatrix4 tolua_ret = (const SuMatrix4)  self->GetViewProjectionMatrix();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((SuMatrix4)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"const SuMatrix4");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(const SuMatrix4));
     tolua_pushusertype(tolua_S,tolua_obj,"const SuMatrix4");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetViewProjectionMatrix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFrustum of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_GetFrustum00
static int tolua_SuTressFXPlugin_SuSpotLight_GetFrustum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFrustum'", NULL);
#endif
  {
   const SuFrustum& tolua_ret = (const SuFrustum&)  self->GetFrustum();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuFrustum");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFrustum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IlluminatesBoundingBox of class  SuSpotLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLight_IlluminatesBoundingBox00
static int tolua_SuTressFXPlugin_SuSpotLight_IlluminatesBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuBoundingBox",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLight* self = (const SuSpotLight*)  tolua_tousertype(tolua_S,1,0);
  const SuBoundingBox* rBV = ((const SuBoundingBox*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IlluminatesBoundingBox'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IlluminatesBoundingBox(*rBV);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IlluminatesBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_new00
static int tolua_SuTressFXPlugin_SuVolumeLight_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuVolumeLight",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuString rName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
  {
   SuVolumeLight* tolua_ret = (SuVolumeLight*)  Mtolua_new((SuVolumeLight)(rName));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuVolumeLight");
   tolua_pushsushistring(tolua_S,(const char*)rName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_new00_local
static int tolua_SuTressFXPlugin_SuVolumeLight_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuVolumeLight",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuString rName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
  {
   SuVolumeLight* tolua_ret = (SuVolumeLight*)  Mtolua_new((SuVolumeLight)(rName));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuVolumeLight");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushsushistring(tolua_S,(const char*)rName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_delete00
static int tolua_SuTressFXPlugin_SuVolumeLight_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuVolumeLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuVolumeLight* self = (SuVolumeLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPosition of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_SetPosition00
static int tolua_SuTressFXPlugin_SuVolumeLight_SetPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuVolumeLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuPoint3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuVolumeLight* self = (SuVolumeLight*)  tolua_tousertype(tolua_S,1,0);
  const SuPoint3* rPos = ((const SuPoint3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosition'", NULL);
#endif
  {
   self->SetPosition(*rPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPosition of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_GetPosition00
static int tolua_SuTressFXPlugin_SuVolumeLight_GetPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuVolumeLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuVolumeLight* self = (const SuVolumeLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosition'", NULL);
#endif
  {
   const SuPoint3& tolua_ret = (const SuPoint3&)  self->GetPosition();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuPoint3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetOrientation of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_SetOrientation00
static int tolua_SuTressFXPlugin_SuVolumeLight_SetOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuVolumeLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuQuat",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuVolumeLight* self = (SuVolumeLight*)  tolua_tousertype(tolua_S,1,0);
  const SuQuat* rOrientation = ((const SuQuat*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetOrientation'", NULL);
#endif
  {
   self->SetOrientation(*rOrientation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetOrientation of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_GetOrientation00
static int tolua_SuTressFXPlugin_SuVolumeLight_GetOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuVolumeLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuVolumeLight* self = (const SuVolumeLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetOrientation'", NULL);
#endif
  {
   const SuQuat& tolua_ret = (const SuQuat&)  self->GetOrientation();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuQuat");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetScale of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_SetScale00
static int tolua_SuTressFXPlugin_SuVolumeLight_SetScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuVolumeLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuVector3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuVolumeLight* self = (SuVolumeLight*)  tolua_tousertype(tolua_S,1,0);
  const SuVector3* rScale = ((const SuVector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetScale'", NULL);
#endif
  {
   self->SetScale(*rScale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetScale of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_GetScale00
static int tolua_SuTressFXPlugin_SuVolumeLight_GetScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuVolumeLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuVolumeLight* self = (const SuVolumeLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetScale'", NULL);
#endif
  {
   const SuVector3 tolua_ret = (const SuVector3)  self->GetScale();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((SuVector3)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"const SuVector3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(const SuVector3));
     tolua_pushusertype(tolua_S,tolua_obj,"const SuVector3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetLightShape of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_SetLightShape00
static int tolua_SuTressFXPlugin_SuVolumeLight_SetLightShape00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuVolumeLight",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuVolumeLight* self = (SuVolumeLight*)  tolua_tousertype(tolua_S,1,0);
  SuLight::LightShapeType rLightShape = ((SuLight::LightShapeType) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetLightShape'", NULL);
#endif
  {
   self->SetLightShape(rLightShape);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetLightShape'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLightShape of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_GetLightShape00
static int tolua_SuTressFXPlugin_SuVolumeLight_GetLightShape00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuVolumeLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuVolumeLight* self = (const SuVolumeLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLightShape'", NULL);
#endif
  {
   SuLight::LightShapeType tolua_ret = (SuLight::LightShapeType)  self->GetLightShape();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLightShape'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSushiLightIndirect of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_SetSushiLightIndirect00
static int tolua_SuTressFXPlugin_SuVolumeLight_SetSushiLightIndirect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuVolumeLight",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuVolumeLight* self = (SuVolumeLight*)  tolua_tousertype(tolua_S,1,0);
  bool bSushiLightIndirect = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSushiLightIndirect'", NULL);
#endif
  {
   self->SetSushiLightIndirect(bSushiLightIndirect);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSushiLightIndirect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSushiLightIndirect of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_GetSushiLightIndirect00
static int tolua_SuTressFXPlugin_SuVolumeLight_GetSushiLightIndirect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuVolumeLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuVolumeLight* self = (const SuVolumeLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSushiLightIndirect'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetSushiLightIndirect();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSushiLightIndirect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IlluminatesBoundingBox of class  SuVolumeLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuVolumeLight_IlluminatesBoundingBox00
static int tolua_SuTressFXPlugin_SuVolumeLight_IlluminatesBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuVolumeLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuBoundingBox",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuVolumeLight* self = (const SuVolumeLight*)  tolua_tousertype(tolua_S,1,0);
  const SuBoundingBox* rBV = ((const SuBoundingBox*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IlluminatesBoundingBox'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IlluminatesBoundingBox(*rBV);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IlluminatesBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  SuAreaLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuAreaLight_new00
static int tolua_SuTressFXPlugin_SuAreaLight_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuAreaLight",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuString rName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
  {
   SuAreaLight* tolua_ret = (SuAreaLight*)  Mtolua_new((SuAreaLight)(rName));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuAreaLight");
   tolua_pushsushistring(tolua_S,(const char*)rName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SuAreaLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuAreaLight_new00_local
static int tolua_SuTressFXPlugin_SuAreaLight_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuAreaLight",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuString rName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
  {
   SuAreaLight* tolua_ret = (SuAreaLight*)  Mtolua_new((SuAreaLight)(rName));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuAreaLight");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushsushistring(tolua_S,(const char*)rName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  SuAreaLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuAreaLight_delete00
static int tolua_SuTressFXPlugin_SuAreaLight_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuAreaLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuAreaLight* self = (SuAreaLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPosition of class  SuAreaLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuAreaLight_SetPosition00
static int tolua_SuTressFXPlugin_SuAreaLight_SetPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuAreaLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuPoint3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuAreaLight* self = (SuAreaLight*)  tolua_tousertype(tolua_S,1,0);
  const SuPoint3* rPos = ((const SuPoint3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosition'", NULL);
#endif
  {
   self->SetPosition(*rPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPosition of class  SuAreaLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuAreaLight_GetPosition00
static int tolua_SuTressFXPlugin_SuAreaLight_GetPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuAreaLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuAreaLight* self = (const SuAreaLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosition'", NULL);
#endif
  {
   const SuPoint3& tolua_ret = (const SuPoint3&)  self->GetPosition();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuPoint3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetOrientation of class  SuAreaLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuAreaLight_SetOrientation00
static int tolua_SuTressFXPlugin_SuAreaLight_SetOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuAreaLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuQuat",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuAreaLight* self = (SuAreaLight*)  tolua_tousertype(tolua_S,1,0);
  const SuQuat* rOrientation = ((const SuQuat*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetOrientation'", NULL);
#endif
  {
   self->SetOrientation(*rOrientation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetOrientation of class  SuAreaLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuAreaLight_GetOrientation00
static int tolua_SuTressFXPlugin_SuAreaLight_GetOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuAreaLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuAreaLight* self = (const SuAreaLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetOrientation'", NULL);
#endif
  {
   const SuQuat& tolua_ret = (const SuQuat&)  self->GetOrientation();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const SuQuat");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetScale of class  SuAreaLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuAreaLight_SetScale00
static int tolua_SuTressFXPlugin_SuAreaLight_SetScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuAreaLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuVector3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuAreaLight* self = (SuAreaLight*)  tolua_tousertype(tolua_S,1,0);
  const SuVector3* rScale = ((const SuVector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetScale'", NULL);
#endif
  {
   self->SetScale(*rScale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetScale of class  SuAreaLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuAreaLight_GetScale00
static int tolua_SuTressFXPlugin_SuAreaLight_GetScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuAreaLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuAreaLight* self = (const SuAreaLight*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetScale'", NULL);
#endif
  {
   const SuVector3 tolua_ret = (const SuVector3)  self->GetScale();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((SuVector3)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"const SuVector3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(const SuVector3));
     tolua_pushusertype(tolua_S,tolua_obj,"const SuVector3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IlluminatesBoundingBox of class  SuAreaLight */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuAreaLight_IlluminatesBoundingBox00
static int tolua_SuTressFXPlugin_SuAreaLight_IlluminatesBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuAreaLight",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const SuBoundingBox",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuAreaLight* self = (const SuAreaLight*)  tolua_tousertype(tolua_S,1,0);
  const SuBoundingBox* rBV = ((const SuBoundingBox*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IlluminatesBoundingBox'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IlluminatesBoundingBox(*rBV);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IlluminatesBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  SuObjectLightTable */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuObjectLightTable_new00
static int tolua_SuTressFXPlugin_SuObjectLightTable_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuObjectLightTable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SuObjectLightTable* tolua_ret = (SuObjectLightTable*)  Mtolua_new((SuObjectLightTable)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuObjectLightTable");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SuObjectLightTable */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuObjectLightTable_new00_local
static int tolua_SuTressFXPlugin_SuObjectLightTable_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuObjectLightTable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SuObjectLightTable* tolua_ret = (SuObjectLightTable*)  Mtolua_new((SuObjectLightTable)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuObjectLightTable");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  SuObjectLightTable */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuObjectLightTable_delete00
static int tolua_SuTressFXPlugin_SuObjectLightTable_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuObjectLightTable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuObjectLightTable* self = (SuObjectLightTable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  SuObjectLightTable */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuObjectLightTable_Clear00
static int tolua_SuTressFXPlugin_SuObjectLightTable_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuObjectLightTable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuObjectLightTable* self = (SuObjectLightTable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'", NULL);
#endif
  {
   self->Clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddActiveLightToObject of class  SuObjectLightTable */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuObjectLightTable_AddActiveLightToObject00
static int tolua_SuTressFXPlugin_SuObjectLightTable_AddActiveLightToObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuObjectLightTable",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const SuObject",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuObjectLightTable* self = (SuObjectLightTable*)  tolua_tousertype(tolua_S,1,0);
  const SuObject* pObj = ((const SuObject*)  tolua_tousertype(tolua_S,2,0));
  const SuLight* pLight = ((const SuLight*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddActiveLightToObject'", NULL);
#endif
  {
   self->AddActiveLightToObject(pObj,pLight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddActiveLightToObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_delete00
static int tolua_SuTressFXPlugin_SuLightManager_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLightManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLightManager* self = (SuLightManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPtr of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_GetPtr00
static int tolua_SuTressFXPlugin_SuLightManager_GetPtr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuLightManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SuLightManager* tolua_ret = (SuLightManager*)  SuLightManager::GetPtr();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuLightManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPtr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRef of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_GetRef00
static int tolua_SuTressFXPlugin_SuLightManager_GetRef00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuLightManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SuLightManager& tolua_ret = (SuLightManager&)  SuLightManager::GetRef();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"SuLightManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRef'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ShutDown of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_ShutDown00
static int tolua_SuTressFXPlugin_SuLightManager_ShutDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuLightManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SuLightManager::ShutDown();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShutDown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CreatePointLight of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_CreatePointLight00
static int tolua_SuTressFXPlugin_SuLightManager_CreatePointLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLightManager",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLightManager* self = (SuLightManager*)  tolua_tousertype(tolua_S,1,0);
  const SuString rLightName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CreatePointLight'", NULL);
#endif
  {
   SuPointLight* tolua_ret = (SuPointLight*)  self->CreatePointLight(rLightName);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuPointLight");
   tolua_pushsushistring(tolua_S,(const char*)rLightName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreatePointLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CreateDirectionalLight of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_CreateDirectionalLight00
static int tolua_SuTressFXPlugin_SuLightManager_CreateDirectionalLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLightManager",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLightManager* self = (SuLightManager*)  tolua_tousertype(tolua_S,1,0);
  const SuString rLightName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CreateDirectionalLight'", NULL);
#endif
  {
   SuDirectionalLight* tolua_ret = (SuDirectionalLight*)  self->CreateDirectionalLight(rLightName);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuDirectionalLight");
   tolua_pushsushistring(tolua_S,(const char*)rLightName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateDirectionalLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CreateSpotLight of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_CreateSpotLight00
static int tolua_SuTressFXPlugin_SuLightManager_CreateSpotLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLightManager",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLightManager* self = (SuLightManager*)  tolua_tousertype(tolua_S,1,0);
  const SuString rLightName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CreateSpotLight'", NULL);
#endif
  {
   SuSpotLight* tolua_ret = (SuSpotLight*)  self->CreateSpotLight(rLightName);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuSpotLight");
   tolua_pushsushistring(tolua_S,(const char*)rLightName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateSpotLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CreateVolumeLight of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_CreateVolumeLight00
static int tolua_SuTressFXPlugin_SuLightManager_CreateVolumeLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLightManager",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLightManager* self = (SuLightManager*)  tolua_tousertype(tolua_S,1,0);
  const SuString rLightName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CreateVolumeLight'", NULL);
#endif
  {
   SuVolumeLight* tolua_ret = (SuVolumeLight*)  self->CreateVolumeLight(rLightName);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuVolumeLight");
   tolua_pushsushistring(tolua_S,(const char*)rLightName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateVolumeLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CreateAreaLight of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_CreateAreaLight00
static int tolua_SuTressFXPlugin_SuLightManager_CreateAreaLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLightManager",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLightManager* self = (SuLightManager*)  tolua_tousertype(tolua_S,1,0);
  const SuString rLightName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CreateAreaLight'", NULL);
#endif
  {
   SuAreaLight* tolua_ret = (SuAreaLight*)  self->CreateAreaLight(rLightName);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuAreaLight");
   tolua_pushsushistring(tolua_S,(const char*)rLightName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateAreaLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLightByName of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_GetLightByName00
static int tolua_SuTressFXPlugin_SuLightManager_GetLightByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLightManager",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLightManager* self = (const SuLightManager*)  tolua_tousertype(tolua_S,1,0);
  const SuString rLightName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLightByName'", NULL);
#endif
  {
   SuLight* tolua_ret = (SuLight*)  self->GetLightByName(rLightName);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuLight");
   tolua_pushsushistring(tolua_S,(const char*)rLightName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLightByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLightByIndex of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_GetLightByIndex00
static int tolua_SuTressFXPlugin_SuLightManager_GetLightByIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLightManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLightManager* self = (const SuLightManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int nIndex = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLightByIndex'", NULL);
#endif
  {
   SuLight* tolua_ret = (SuLight*)  self->GetLightByIndex(nIndex);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuLight");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLightByIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumLights of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_GetNumLights00
static int tolua_SuTressFXPlugin_SuLightManager_GetNumLights00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLightManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLightManager* self = (const SuLightManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumLights'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->GetNumLights();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumLights'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LoadLightSet of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_LoadLightSet00
static int tolua_SuTressFXPlugin_SuLightManager_LoadLightSet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLightManager",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLightManager* self = (SuLightManager*)  tolua_tousertype(tolua_S,1,0);
  const SuString rFileName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LoadLightSet'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->LoadLightSet(rFileName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushsushistring(tolua_S,(const char*)rFileName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LoadLightSet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LoadLightSet of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_LoadLightSet01
static int tolua_SuTressFXPlugin_SuLightManager_LoadLightSet01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLightManager",0,&tolua_err) ||
     !tolua_issushistring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_istable(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  SuLightManager* self = (SuLightManager*)  tolua_tousertype(tolua_S,1,0);
  const SuString rFileName = ((const SuString)  tolua_tosushistring(tolua_S,2,0));
  unsigned int nProperties = (( unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifdef __cplusplus
  SuString* pProperties = Mtolua_new_dim(SuString, nProperties);
#else
  SuString* pProperties = (SuString*) malloc((nProperties)*sizeof(SuString));
#endif
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LoadLightSet'", NULL);
#endif
  {
#ifndef TOLUA_RELEASE
   if (!tolua_issushistringarray(tolua_S,4,nProperties,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<nProperties;i++)
    pProperties[i] = ((SuString)  tolua_tofieldsushistring(tolua_S,4,i+1,0));
   }
  }
  {
   bool tolua_ret = (bool)  self->LoadLightSet(rFileName,nProperties,pProperties);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushsushistring(tolua_S,(const char*)rFileName);
  }
#ifdef __cplusplus
  Mtolua_delete_dim(pProperties);
#else
  free(pProperties);
#endif
 }
 return 2;
tolua_lerror:
 return tolua_SuTressFXPlugin_SuLightManager_LoadLightSet00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReloadLightSets of class  SuLightManager */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightManager_ReloadLightSets00
static int tolua_SuTressFXPlugin_SuLightManager_ReloadLightSets00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLightManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLightManager* self = (SuLightManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReloadLightSets'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ReloadLightSets();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReloadLightSets'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  SuSpotLightCameraController */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLightCameraController_new00
static int tolua_SuTressFXPlugin_SuSpotLightCameraController_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuSpotLightCameraController",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLight* pSpotLight = ((SuSpotLight*)  tolua_tousertype(tolua_S,2,0));
  {
   SuSpotLightCameraController* tolua_ret = (SuSpotLightCameraController*)  Mtolua_new((SuSpotLightCameraController)(pSpotLight));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuSpotLightCameraController");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SuSpotLightCameraController */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLightCameraController_new00_local
static int tolua_SuTressFXPlugin_SuSpotLightCameraController_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuSpotLightCameraController",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"SuSpotLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLight* pSpotLight = ((SuSpotLight*)  tolua_tousertype(tolua_S,2,0));
  {
   SuSpotLightCameraController* tolua_ret = (SuSpotLightCameraController*)  Mtolua_new((SuSpotLightCameraController)(pSpotLight));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuSpotLightCameraController");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  SuSpotLightCameraController */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLightCameraController_delete00
static int tolua_SuTressFXPlugin_SuSpotLightCameraController_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuSpotLightCameraController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLightCameraController* self = (SuSpotLightCameraController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSpotLight of class  SuSpotLightCameraController */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLightCameraController_GetSpotLight00
static int tolua_SuTressFXPlugin_SuSpotLightCameraController_GetSpotLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuSpotLightCameraController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuSpotLightCameraController* self = (const SuSpotLightCameraController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSpotLight'", NULL);
#endif
  {
   SuSpotLight* tolua_ret = (SuSpotLight*)  self->GetSpotLight();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuSpotLight");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSpotLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Update of class  SuSpotLightCameraController */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuSpotLightCameraController_Update00
static int tolua_SuTressFXPlugin_SuSpotLightCameraController_Update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuSpotLightCameraController",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuSpotLightCameraController* self = (SuSpotLightCameraController*)  tolua_tousertype(tolua_S,1,0);
   double t = ((  double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Update'", NULL);
#endif
  {
   self->Update(t);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  SuLightVisualizer */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightVisualizer_new00
static int tolua_SuTressFXPlugin_SuLightVisualizer_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuLightVisualizer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SuLightVisualizer* tolua_ret = (SuLightVisualizer*)  Mtolua_new((SuLightVisualizer)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuLightVisualizer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SuLightVisualizer */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightVisualizer_new00_local
static int tolua_SuTressFXPlugin_SuLightVisualizer_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SuLightVisualizer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SuLightVisualizer* tolua_ret = (SuLightVisualizer*)  Mtolua_new((SuLightVisualizer)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SuLightVisualizer");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  SuLightVisualizer */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightVisualizer_delete00
static int tolua_SuTressFXPlugin_SuLightVisualizer_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLightVisualizer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLightVisualizer* self = (SuLightVisualizer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DrawLight of class  SuLightVisualizer */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightVisualizer_DrawLight00
static int tolua_SuTressFXPlugin_SuLightVisualizer_DrawLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const SuLightVisualizer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const SuLight",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const SuLightVisualizer* self = (const SuLightVisualizer*)  tolua_tousertype(tolua_S,1,0);
  const SuLight* pLight = ((const SuLight*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DrawLight'", NULL);
#endif
  {
   self->DrawLight(pLight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DrawLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnEffectReload of class  SuLightVisualizer */
#ifndef TOLUA_DISABLE_tolua_SuTressFXPlugin_SuLightVisualizer_OnEffectReload00
static int tolua_SuTressFXPlugin_SuLightVisualizer_OnEffectReload00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SuLightVisualizer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"SuEffectPtr",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"SuEffectPtr",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SuLightVisualizer* self = (SuLightVisualizer*)  tolua_tousertype(tolua_S,1,0);
  SuEffectPtr pOld = *((SuEffectPtr*)  tolua_tousertype(tolua_S,2,0));
  SuEffectPtr pNew = *((SuEffectPtr*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnEffectReload'", NULL);
#endif
  {
   self->OnEffectReload(pOld,pNew);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnEffectReload'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_SuTressFXPlugin_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"SuTressFXPlugin","SuTressFXPlugin","SuPlugin",NULL);
  tolua_beginmodule(tolua_S,"SuTressFXPlugin");
   tolua_function(tolua_S,"LoadHairAsset",tolua_SuTressFXPlugin_SuTressFXPlugin_LoadHairAsset00);
   tolua_function(tolua_S,"Shutdown",tolua_SuTressFXPlugin_SuTressFXPlugin_Shutdown00);
   tolua_function(tolua_S,"DrawHairShadows",tolua_SuTressFXPlugin_SuTressFXPlugin_DrawHairShadows00);
   tolua_function(tolua_S,"EnableAnimation",tolua_SuTressFXPlugin_SuTressFXPlugin_EnableAnimation00);
   tolua_function(tolua_S,"OnResize",tolua_SuTressFXPlugin_SuTressFXPlugin_OnResize00);
   tolua_function(tolua_S,"Break",tolua_SuTressFXPlugin_SuTressFXPlugin_Break00);
   tolua_function(tolua_S,"DrawHair",tolua_SuTressFXPlugin_SuTressFXPlugin_DrawHair00);
   tolua_function(tolua_S,"Simulate",tolua_SuTressFXPlugin_SuTressFXPlugin_Simulate00);
   tolua_function(tolua_S,"SignalSimulateStart",tolua_SuTressFXPlugin_SuTressFXPlugin_SignalSimulateStart00);
   tolua_function(tolua_S,"WaitSimulateDone",tolua_SuTressFXPlugin_SuTressFXPlugin_WaitSimulateDone00);
   tolua_function(tolua_S,"EnableUpdatingCollisionMesh",tolua_SuTressFXPlugin_SuTressFXPlugin_EnableUpdatingCollisionMesh00);
   tolua_function(tolua_S,"EnableSDFCollisionResponse",tolua_SuTressFXPlugin_SuTressFXPlugin_EnableSDFCollisionResponse00);
   tolua_function(tolua_S,"UpdateSimulationParameters",tolua_SuTressFXPlugin_SuTressFXPlugin_UpdateSimulationParameters00);
   tolua_function(tolua_S,"ToggleShortCut",tolua_SuTressFXPlugin_SuTressFXPlugin_ToggleShortCut00);
   tolua_function(tolua_S,"DrawCollisionMesh",tolua_SuTressFXPlugin_SuTressFXPlugin_DrawCollisionMesh00);
   tolua_function(tolua_S,"DrawSDF",tolua_SuTressFXPlugin_SuTressFXPlugin_DrawSDF00);
   tolua_function(tolua_S,"GetNumTressFXObjects",tolua_SuTressFXPlugin_SuTressFXPlugin_GetNumTressFXObjects00);
   tolua_function(tolua_S,"GetTressFXObject",tolua_SuTressFXPlugin_SuTressFXPlugin_GetTressFXObject00);
   tolua_variable(tolua_S,"__SuEffectReloadListener__",tolua_get_SuTressFXPlugin___SuEffectReloadListener__,NULL);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"TressFXSimulationSettings","TressFXSimulationSettings","",tolua_collect_TressFXSimulationSettings);
  #else
  tolua_cclass(tolua_S,"TressFXSimulationSettings","TressFXSimulationSettings","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"TressFXSimulationSettings");
   tolua_function(tolua_S,"new",tolua_SuTressFXPlugin_TressFXSimulationSettings_new00);
   tolua_function(tolua_S,"new_local",tolua_SuTressFXPlugin_TressFXSimulationSettings_new00_local);
   tolua_function(tolua_S,".call",tolua_SuTressFXPlugin_TressFXSimulationSettings_new00_local);
   tolua_variable(tolua_S,"vspCoeff",tolua_get_TressFXSimulationSettings_vspCoeff,tolua_set_TressFXSimulationSettings_vspCoeff);
   tolua_variable(tolua_S,"vspAccelThreshold",tolua_get_TressFXSimulationSettings_vspAccelThreshold,tolua_set_TressFXSimulationSettings_vspAccelThreshold);
   tolua_variable(tolua_S,"localConstraintStiffness",tolua_get_TressFXSimulationSettings_localConstraintStiffness,tolua_set_TressFXSimulationSettings_localConstraintStiffness);
   tolua_variable(tolua_S,"localConstraintsIterations",tolua_get_TressFXSimulationSettings_localConstraintsIterations,tolua_set_TressFXSimulationSettings_localConstraintsIterations);
   tolua_variable(tolua_S,"globalConstraintStiffness",tolua_get_TressFXSimulationSettings_globalConstraintStiffness,tolua_set_TressFXSimulationSettings_globalConstraintStiffness);
   tolua_variable(tolua_S,"globalConstraintsRange",tolua_get_TressFXSimulationSettings_globalConstraintsRange,tolua_set_TressFXSimulationSettings_globalConstraintsRange);
   tolua_variable(tolua_S,"lengthConstraintsIterations",tolua_get_TressFXSimulationSettings_lengthConstraintsIterations,tolua_set_TressFXSimulationSettings_lengthConstraintsIterations);
   tolua_variable(tolua_S,"damping",tolua_get_TressFXSimulationSettings_damping,tolua_set_TressFXSimulationSettings_damping);
   tolua_variable(tolua_S,"gravityMagnitude",tolua_get_TressFXSimulationSettings_gravityMagnitude,tolua_set_TressFXSimulationSettings_gravityMagnitude);
   tolua_variable(tolua_S,"tipSeparation",tolua_get_TressFXSimulationSettings_tipSeparation,tolua_set_TressFXSimulationSettings_tipSeparation);
   tolua_variable(tolua_S,"windMagnitude",tolua_get_TressFXSimulationSettings_windMagnitude,tolua_set_TressFXSimulationSettings_windMagnitude);
   tolua_array(tolua_S,"windDirection",tolua_get_SuTressFXPlugin_TressFXSimulationSettings_windDirection,tolua_set_SuTressFXPlugin_TressFXSimulationSettings_windDirection);
   tolua_variable(tolua_S,"windAngleRadians",tolua_get_TressFXSimulationSettings_windAngleRadians,tolua_set_TressFXSimulationSettings_windAngleRadians);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"SuLight","SuLight","",tolua_collect_SuLight);
  #else
  tolua_cclass(tolua_S,"SuLight","SuLight","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"SuLight");
   tolua_constant(tolua_S,"POINT_LIGHT",SuLight::POINT_LIGHT);
   tolua_constant(tolua_S,"DIRECTIONAL_LIGHT",SuLight::DIRECTIONAL_LIGHT);
   tolua_constant(tolua_S,"SPOT_LIGHT",SuLight::SPOT_LIGHT);
   tolua_constant(tolua_S,"VOLUME_LIGHT",SuLight::VOLUME_LIGHT);
   tolua_constant(tolua_S,"AREA_LIGHT",SuLight::AREA_LIGHT);
   tolua_constant(tolua_S,"FALLOFF_NONE",SuLight::FALLOFF_NONE);
   tolua_constant(tolua_S,"FALLOFF_LINEAR",SuLight::FALLOFF_LINEAR);
   tolua_constant(tolua_S,"FALLOFF_QUADRATIC",SuLight::FALLOFF_QUADRATIC);
   tolua_constant(tolua_S,"VOLUMELIGHT_BOX",SuLight::VOLUMELIGHT_BOX);
   tolua_constant(tolua_S,"VOLUMELIGHT_SPHERE",SuLight::VOLUMELIGHT_SPHERE);
   tolua_constant(tolua_S,"VOLUMELIGHT_CYLINDER",SuLight::VOLUMELIGHT_CYLINDER);
   tolua_constant(tolua_S,"VOLUMELIGHT_CONE",SuLight::VOLUMELIGHT_CONE);
   tolua_function(tolua_S,"delete",tolua_SuTressFXPlugin_SuLight_delete00);
   tolua_function(tolua_S,"GetType",tolua_SuTressFXPlugin_SuLight_GetType00);
   tolua_function(tolua_S,"IsVisible",tolua_SuTressFXPlugin_SuLight_IsVisible00);
   tolua_function(tolua_S,"SetVisible",tolua_SuTressFXPlugin_SuLight_SetVisible00);
   tolua_function(tolua_S,"SetIntensity",tolua_SuTressFXPlugin_SuLight_SetIntensity00);
   tolua_function(tolua_S,"GetIntensity",tolua_SuTressFXPlugin_SuLight_GetIntensity00);
   tolua_function(tolua_S,"SetColor",tolua_SuTressFXPlugin_SuLight_SetColor00);
   tolua_function(tolua_S,"GetColor",tolua_SuTressFXPlugin_SuLight_GetColor00);
   tolua_function(tolua_S,"SetDiffuse",tolua_SuTressFXPlugin_SuLight_SetDiffuse00);
   tolua_function(tolua_S,"IsDiffuse",tolua_SuTressFXPlugin_SuLight_IsDiffuse00);
   tolua_function(tolua_S,"SetSpecular",tolua_SuTressFXPlugin_SuLight_SetSpecular00);
   tolua_function(tolua_S,"IsSpecular",tolua_SuTressFXPlugin_SuLight_IsSpecular00);
   tolua_function(tolua_S,"SetFalloff",tolua_SuTressFXPlugin_SuLight_SetFalloff00);
   tolua_function(tolua_S,"GetFalloff",tolua_SuTressFXPlugin_SuLight_GetFalloff00);
   tolua_function(tolua_S,"ComputeDistanceAttenuation",tolua_SuTressFXPlugin_SuLight_ComputeDistanceAttenuation00);
   tolua_function(tolua_S,"IlluminatesBoundingBox",tolua_SuTressFXPlugin_SuLight_IlluminatesBoundingBox00);
   tolua_function(tolua_S,"GetController",tolua_SuTressFXPlugin_SuLight_GetController00);
   tolua_function(tolua_S,"Update",tolua_SuTressFXPlugin_SuLight_Update00);
   tolua_function(tolua_S,"GetName",tolua_SuTressFXPlugin_SuLight_GetName00);
   tolua_function(tolua_S,"AddProperty",tolua_SuTressFXPlugin_SuLight_AddProperty00);
   tolua_function(tolua_S,"HasProperty",tolua_SuTressFXPlugin_SuLight_HasProperty00);
   tolua_function(tolua_S,"GetNumProperties",tolua_SuTressFXPlugin_SuLight_GetNumProperties00);
   tolua_function(tolua_S,"GetProperty",tolua_SuTressFXPlugin_SuLight_GetProperty00);
   tolua_function(tolua_S,"GetProperties",tolua_SuTressFXPlugin_SuLight_GetProperties00);
   tolua_function(tolua_S,"SetIndex",tolua_SuTressFXPlugin_SuLight_SetIndex00);
   tolua_function(tolua_S,"GetIndex",tolua_SuTressFXPlugin_SuLight_GetIndex00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"SuLightController","SuLightController","",tolua_collect_SuLightController);
  #else
  tolua_cclass(tolua_S,"SuLightController","SuLightController","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"SuLightController");
   tolua_constant(tolua_S,"COLOR_INDEX",SuLightController::COLOR_INDEX);
   tolua_constant(tolua_S,"INTENSITY_INDEX",SuLightController::INTENSITY_INDEX);
   tolua_constant(tolua_S,"DIFFUSE_INDEX",SuLightController::DIFFUSE_INDEX);
   tolua_constant(tolua_S,"SPECULAR_INDEX",SuLightController::SPECULAR_INDEX);
   tolua_constant(tolua_S,"FALLOFF_INDEX",SuLightController::FALLOFF_INDEX);
   tolua_constant(tolua_S,"POS_INDEX",SuLightController::POS_INDEX);
   tolua_constant(tolua_S,"RADIUS_INDEX",SuLightController::RADIUS_INDEX);
   tolua_constant(tolua_S,"ORIENTATION_INDEX",SuLightController::ORIENTATION_INDEX);
   tolua_constant(tolua_S,"SCALE_INDEX",SuLightController::SCALE_INDEX);
   tolua_constant(tolua_S,"CONE_ANGLE_INDEX",SuLightController::CONE_ANGLE_INDEX);
   tolua_constant(tolua_S,"PENUMBRA_ANGLE_INDEX",SuLightController::PENUMBRA_ANGLE_INDEX);
   tolua_constant(tolua_S,"DROP_OFF_INDEX",SuLightController::DROP_OFF_INDEX);
   tolua_constant(tolua_S,"VISIBILITY_INDEX",SuLightController::VISIBILITY_INDEX);
   tolua_constant(tolua_S,"SHAPE_INDEX",SuLightController::SHAPE_INDEX);
   tolua_constant(tolua_S,"INDIRECT_INDEX",SuLightController::INDIRECT_INDEX);
   tolua_constant(tolua_S,"NUM_CURVE_TYPES",SuLightController::NUM_CURVE_TYPES);
   tolua_constant(tolua_S,"COLOR_MASK",SuLightController::COLOR_MASK);
   tolua_constant(tolua_S,"INTENSITY_MASK",SuLightController::INTENSITY_MASK);
   tolua_constant(tolua_S,"DIFFUSE_MASK",SuLightController::DIFFUSE_MASK);
   tolua_constant(tolua_S,"SPECULAR_MASK",SuLightController::SPECULAR_MASK);
   tolua_constant(tolua_S,"FALLOFF_MASK",SuLightController::FALLOFF_MASK);
   tolua_constant(tolua_S,"POS_MASK",SuLightController::POS_MASK);
   tolua_constant(tolua_S,"RADIUS_MASK",SuLightController::RADIUS_MASK);
   tolua_constant(tolua_S,"ORIENTATION_MASK",SuLightController::ORIENTATION_MASK);
   tolua_constant(tolua_S,"SCALE_MASK",SuLightController::SCALE_MASK);
   tolua_constant(tolua_S,"CONE_ANGLE_MASK",SuLightController::CONE_ANGLE_MASK);
   tolua_constant(tolua_S,"PENUMBRA_ANGLE_MASK",SuLightController::PENUMBRA_ANGLE_MASK);
   tolua_constant(tolua_S,"DROP_OFF_MASK",SuLightController::DROP_OFF_MASK);
   tolua_constant(tolua_S,"VISIBILITY_MASK",SuLightController::VISIBILITY_MASK);
   tolua_constant(tolua_S,"SHAPE_MASK",SuLightController::SHAPE_MASK);
   tolua_constant(tolua_S,"INDIRECT_MASK",SuLightController::INDIRECT_MASK);
   tolua_constant(tolua_S,"ALL_MASK",SuLightController::ALL_MASK);
   tolua_function(tolua_S,"new",tolua_SuTressFXPlugin_SuLightController_new00);
   tolua_function(tolua_S,"new_local",tolua_SuTressFXPlugin_SuLightController_new00_local);
   tolua_function(tolua_S,".call",tolua_SuTressFXPlugin_SuLightController_new00_local);
   tolua_function(tolua_S,"delete",tolua_SuTressFXPlugin_SuLightController_delete00);
   tolua_function(tolua_S,"Update",tolua_SuTressFXPlugin_SuLightController_Update00);
   tolua_function(tolua_S,"HasChanged",tolua_SuTressFXPlugin_SuLightController_HasChanged00);
   tolua_function(tolua_S,"IsDynamic",tolua_SuTressFXPlugin_SuLightController_IsDynamic00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"SuDirectionalLight","SuDirectionalLight","SuLight",tolua_collect_SuDirectionalLight);
  #else
  tolua_cclass(tolua_S,"SuDirectionalLight","SuDirectionalLight","SuLight",NULL);
  #endif
  tolua_beginmodule(tolua_S,"SuDirectionalLight");
   tolua_function(tolua_S,"new",tolua_SuTressFXPlugin_SuDirectionalLight_new00);
   tolua_function(tolua_S,"new_local",tolua_SuTressFXPlugin_SuDirectionalLight_new00_local);
   tolua_function(tolua_S,".call",tolua_SuTressFXPlugin_SuDirectionalLight_new00_local);
   tolua_function(tolua_S,"delete",tolua_SuTressFXPlugin_SuDirectionalLight_delete00);
   tolua_function(tolua_S,"SetDirection",tolua_SuTressFXPlugin_SuDirectionalLight_SetDirection00);
   tolua_function(tolua_S,"GetDirection",tolua_SuTressFXPlugin_SuDirectionalLight_GetDirection00);
   tolua_function(tolua_S,"IlluminatesBoundingBox",tolua_SuTressFXPlugin_SuDirectionalLight_IlluminatesBoundingBox00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"SuPointLight","SuPointLight","SuLight",tolua_collect_SuPointLight);
  #else
  tolua_cclass(tolua_S,"SuPointLight","SuPointLight","SuLight",NULL);
  #endif
  tolua_beginmodule(tolua_S,"SuPointLight");
   tolua_function(tolua_S,"new",tolua_SuTressFXPlugin_SuPointLight_new00);
   tolua_function(tolua_S,"new_local",tolua_SuTressFXPlugin_SuPointLight_new00_local);
   tolua_function(tolua_S,".call",tolua_SuTressFXPlugin_SuPointLight_new00_local);
   tolua_function(tolua_S,"delete",tolua_SuTressFXPlugin_SuPointLight_delete00);
   tolua_function(tolua_S,"SetPosition",tolua_SuTressFXPlugin_SuPointLight_SetPosition00);
   tolua_function(tolua_S,"GetPosition",tolua_SuTressFXPlugin_SuPointLight_GetPosition00);
   tolua_function(tolua_S,"SetRadius",tolua_SuTressFXPlugin_SuPointLight_SetRadius00);
   tolua_function(tolua_S,"GetRadius",tolua_SuTressFXPlugin_SuPointLight_GetRadius00);
   tolua_function(tolua_S,"IlluminatesBoundingBox",tolua_SuTressFXPlugin_SuPointLight_IlluminatesBoundingBox00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"SuSpotLight","SuSpotLight","SuLight",tolua_collect_SuSpotLight);
  #else
  tolua_cclass(tolua_S,"SuSpotLight","SuSpotLight","SuLight",NULL);
  #endif
  tolua_beginmodule(tolua_S,"SuSpotLight");
   tolua_function(tolua_S,"new",tolua_SuTressFXPlugin_SuSpotLight_new00);
   tolua_function(tolua_S,"new_local",tolua_SuTressFXPlugin_SuSpotLight_new00_local);
   tolua_function(tolua_S,".call",tolua_SuTressFXPlugin_SuSpotLight_new00_local);
   tolua_function(tolua_S,"delete",tolua_SuTressFXPlugin_SuSpotLight_delete00);
   tolua_function(tolua_S,"GetDropOff",tolua_SuTressFXPlugin_SuSpotLight_GetDropOff00);
   tolua_function(tolua_S,"SetDropOff",tolua_SuTressFXPlugin_SuSpotLight_SetDropOff00);
   tolua_function(tolua_S,"SetPosition",tolua_SuTressFXPlugin_SuSpotLight_SetPosition00);
   tolua_function(tolua_S,"GetPosition",tolua_SuTressFXPlugin_SuSpotLight_GetPosition00);
   tolua_function(tolua_S,"SetDirection",tolua_SuTressFXPlugin_SuSpotLight_SetDirection00);
   tolua_function(tolua_S,"SetOrientation",tolua_SuTressFXPlugin_SuSpotLight_SetOrientation00);
   tolua_function(tolua_S,"SetLookAt",tolua_SuTressFXPlugin_SuSpotLight_SetLookAt00);
   tolua_function(tolua_S,"GetDirection",tolua_SuTressFXPlugin_SuSpotLight_GetDirection00);
   tolua_function(tolua_S,"GetUpVector",tolua_SuTressFXPlugin_SuSpotLight_GetUpVector00);
   tolua_function(tolua_S,"GetRightVector",tolua_SuTressFXPlugin_SuSpotLight_GetRightVector00);
   tolua_function(tolua_S,"SetConeAngle",tolua_SuTressFXPlugin_SuSpotLight_SetConeAngle00);
   tolua_function(tolua_S,"GetConeAngle",tolua_SuTressFXPlugin_SuSpotLight_GetConeAngle00);
   tolua_function(tolua_S,"SetHotSpotAngle",tolua_SuTressFXPlugin_SuSpotLight_SetHotSpotAngle00);
   tolua_function(tolua_S,"GetHotSpotAngle",tolua_SuTressFXPlugin_SuSpotLight_GetHotSpotAngle00);
   tolua_function(tolua_S,"SetNearPlaneDistance",tolua_SuTressFXPlugin_SuSpotLight_SetNearPlaneDistance00);
   tolua_function(tolua_S,"GetNearPlaneDistance",tolua_SuTressFXPlugin_SuSpotLight_GetNearPlaneDistance00);
   tolua_function(tolua_S,"SetFarPlaneDistance",tolua_SuTressFXPlugin_SuSpotLight_SetFarPlaneDistance00);
   tolua_function(tolua_S,"GetFarPlaneDistance",tolua_SuTressFXPlugin_SuSpotLight_GetFarPlaneDistance00);
   tolua_function(tolua_S,"GetViewMatrix",tolua_SuTressFXPlugin_SuSpotLight_GetViewMatrix00);
   tolua_function(tolua_S,"GetProjectionMatrix",tolua_SuTressFXPlugin_SuSpotLight_GetProjectionMatrix00);
   tolua_function(tolua_S,"GetViewProjectionMatrix",tolua_SuTressFXPlugin_SuSpotLight_GetViewProjectionMatrix00);
   tolua_function(tolua_S,"GetFrustum",tolua_SuTressFXPlugin_SuSpotLight_GetFrustum00);
   tolua_function(tolua_S,"IlluminatesBoundingBox",tolua_SuTressFXPlugin_SuSpotLight_IlluminatesBoundingBox00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"SuVolumeLight","SuVolumeLight","SuLight",tolua_collect_SuVolumeLight);
  #else
  tolua_cclass(tolua_S,"SuVolumeLight","SuVolumeLight","SuLight",NULL);
  #endif
  tolua_beginmodule(tolua_S,"SuVolumeLight");
   tolua_function(tolua_S,"new",tolua_SuTressFXPlugin_SuVolumeLight_new00);
   tolua_function(tolua_S,"new_local",tolua_SuTressFXPlugin_SuVolumeLight_new00_local);
   tolua_function(tolua_S,".call",tolua_SuTressFXPlugin_SuVolumeLight_new00_local);
   tolua_function(tolua_S,"delete",tolua_SuTressFXPlugin_SuVolumeLight_delete00);
   tolua_function(tolua_S,"SetPosition",tolua_SuTressFXPlugin_SuVolumeLight_SetPosition00);
   tolua_function(tolua_S,"GetPosition",tolua_SuTressFXPlugin_SuVolumeLight_GetPosition00);
   tolua_function(tolua_S,"SetOrientation",tolua_SuTressFXPlugin_SuVolumeLight_SetOrientation00);
   tolua_function(tolua_S,"GetOrientation",tolua_SuTressFXPlugin_SuVolumeLight_GetOrientation00);
   tolua_function(tolua_S,"SetScale",tolua_SuTressFXPlugin_SuVolumeLight_SetScale00);
   tolua_function(tolua_S,"GetScale",tolua_SuTressFXPlugin_SuVolumeLight_GetScale00);
   tolua_function(tolua_S,"SetLightShape",tolua_SuTressFXPlugin_SuVolumeLight_SetLightShape00);
   tolua_function(tolua_S,"GetLightShape",tolua_SuTressFXPlugin_SuVolumeLight_GetLightShape00);
   tolua_function(tolua_S,"SetSushiLightIndirect",tolua_SuTressFXPlugin_SuVolumeLight_SetSushiLightIndirect00);
   tolua_function(tolua_S,"GetSushiLightIndirect",tolua_SuTressFXPlugin_SuVolumeLight_GetSushiLightIndirect00);
   tolua_function(tolua_S,"IlluminatesBoundingBox",tolua_SuTressFXPlugin_SuVolumeLight_IlluminatesBoundingBox00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"SuAreaLight","SuAreaLight","SuLight",tolua_collect_SuAreaLight);
  #else
  tolua_cclass(tolua_S,"SuAreaLight","SuAreaLight","SuLight",NULL);
  #endif
  tolua_beginmodule(tolua_S,"SuAreaLight");
   tolua_function(tolua_S,"new",tolua_SuTressFXPlugin_SuAreaLight_new00);
   tolua_function(tolua_S,"new_local",tolua_SuTressFXPlugin_SuAreaLight_new00_local);
   tolua_function(tolua_S,".call",tolua_SuTressFXPlugin_SuAreaLight_new00_local);
   tolua_function(tolua_S,"delete",tolua_SuTressFXPlugin_SuAreaLight_delete00);
   tolua_function(tolua_S,"SetPosition",tolua_SuTressFXPlugin_SuAreaLight_SetPosition00);
   tolua_function(tolua_S,"GetPosition",tolua_SuTressFXPlugin_SuAreaLight_GetPosition00);
   tolua_function(tolua_S,"SetOrientation",tolua_SuTressFXPlugin_SuAreaLight_SetOrientation00);
   tolua_function(tolua_S,"GetOrientation",tolua_SuTressFXPlugin_SuAreaLight_GetOrientation00);
   tolua_function(tolua_S,"SetScale",tolua_SuTressFXPlugin_SuAreaLight_SetScale00);
   tolua_function(tolua_S,"GetScale",tolua_SuTressFXPlugin_SuAreaLight_GetScale00);
   tolua_function(tolua_S,"IlluminatesBoundingBox",tolua_SuTressFXPlugin_SuAreaLight_IlluminatesBoundingBox00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"SuObjectLightTable","SuObjectLightTable","",tolua_collect_SuObjectLightTable);
  #else
  tolua_cclass(tolua_S,"SuObjectLightTable","SuObjectLightTable","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"SuObjectLightTable");
   tolua_function(tolua_S,"new",tolua_SuTressFXPlugin_SuObjectLightTable_new00);
   tolua_function(tolua_S,"new_local",tolua_SuTressFXPlugin_SuObjectLightTable_new00_local);
   tolua_function(tolua_S,".call",tolua_SuTressFXPlugin_SuObjectLightTable_new00_local);
   tolua_function(tolua_S,"delete",tolua_SuTressFXPlugin_SuObjectLightTable_delete00);
   tolua_function(tolua_S,"Clear",tolua_SuTressFXPlugin_SuObjectLightTable_Clear00);
   tolua_function(tolua_S,"AddActiveLightToObject",tolua_SuTressFXPlugin_SuObjectLightTable_AddActiveLightToObject00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"SuLightManager","SuLightManager","",tolua_collect_SuLightManager);
  #else
  tolua_cclass(tolua_S,"SuLightManager","SuLightManager","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"SuLightManager");
   tolua_function(tolua_S,"delete",tolua_SuTressFXPlugin_SuLightManager_delete00);
   tolua_function(tolua_S,"GetPtr",tolua_SuTressFXPlugin_SuLightManager_GetPtr00);
   tolua_function(tolua_S,"GetRef",tolua_SuTressFXPlugin_SuLightManager_GetRef00);
   tolua_function(tolua_S,"ShutDown",tolua_SuTressFXPlugin_SuLightManager_ShutDown00);
   tolua_function(tolua_S,"CreatePointLight",tolua_SuTressFXPlugin_SuLightManager_CreatePointLight00);
   tolua_function(tolua_S,"CreateDirectionalLight",tolua_SuTressFXPlugin_SuLightManager_CreateDirectionalLight00);
   tolua_function(tolua_S,"CreateSpotLight",tolua_SuTressFXPlugin_SuLightManager_CreateSpotLight00);
   tolua_function(tolua_S,"CreateVolumeLight",tolua_SuTressFXPlugin_SuLightManager_CreateVolumeLight00);
   tolua_function(tolua_S,"CreateAreaLight",tolua_SuTressFXPlugin_SuLightManager_CreateAreaLight00);
   tolua_function(tolua_S,"GetLightByName",tolua_SuTressFXPlugin_SuLightManager_GetLightByName00);
   tolua_function(tolua_S,"GetLightByIndex",tolua_SuTressFXPlugin_SuLightManager_GetLightByIndex00);
   tolua_function(tolua_S,"GetNumLights",tolua_SuTressFXPlugin_SuLightManager_GetNumLights00);
   tolua_function(tolua_S,"LoadLightSet",tolua_SuTressFXPlugin_SuLightManager_LoadLightSet00);
   tolua_function(tolua_S,"LoadLightSet",tolua_SuTressFXPlugin_SuLightManager_LoadLightSet01);
   tolua_function(tolua_S,"ReloadLightSets",tolua_SuTressFXPlugin_SuLightManager_ReloadLightSets00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"SuSpotLightCameraController","SuSpotLightCameraController","SuCameraController",tolua_collect_SuSpotLightCameraController);
  #else
  tolua_cclass(tolua_S,"SuSpotLightCameraController","SuSpotLightCameraController","SuCameraController",NULL);
  #endif
  tolua_beginmodule(tolua_S,"SuSpotLightCameraController");
   tolua_function(tolua_S,"new",tolua_SuTressFXPlugin_SuSpotLightCameraController_new00);
   tolua_function(tolua_S,"new_local",tolua_SuTressFXPlugin_SuSpotLightCameraController_new00_local);
   tolua_function(tolua_S,".call",tolua_SuTressFXPlugin_SuSpotLightCameraController_new00_local);
   tolua_function(tolua_S,"delete",tolua_SuTressFXPlugin_SuSpotLightCameraController_delete00);
   tolua_function(tolua_S,"GetSpotLight",tolua_SuTressFXPlugin_SuSpotLightCameraController_GetSpotLight00);
   tolua_function(tolua_S,"Update",tolua_SuTressFXPlugin_SuSpotLightCameraController_Update00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"SuLightVisualizer","SuLightVisualizer","virtualSuEffectReloadListener",tolua_collect_SuLightVisualizer);
  #else
  tolua_cclass(tolua_S,"SuLightVisualizer","SuLightVisualizer","virtualSuEffectReloadListener",NULL);
  #endif
  tolua_beginmodule(tolua_S,"SuLightVisualizer");
   tolua_function(tolua_S,"new",tolua_SuTressFXPlugin_SuLightVisualizer_new00);
   tolua_function(tolua_S,"new_local",tolua_SuTressFXPlugin_SuLightVisualizer_new00_local);
   tolua_function(tolua_S,".call",tolua_SuTressFXPlugin_SuLightVisualizer_new00_local);
   tolua_function(tolua_S,"delete",tolua_SuTressFXPlugin_SuLightVisualizer_delete00);
   tolua_function(tolua_S,"DrawLight",tolua_SuTressFXPlugin_SuLightVisualizer_DrawLight00);
   tolua_function(tolua_S,"OnEffectReload",tolua_SuTressFXPlugin_SuLightVisualizer_OnEffectReload00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_SuTressFXPlugin (lua_State* tolua_S) {
 return tolua_SuTressFXPlugin_open(tolua_S);
};
#endif

