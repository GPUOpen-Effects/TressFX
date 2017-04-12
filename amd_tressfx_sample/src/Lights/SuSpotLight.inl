//=================================================================================================================================
//
// Author: Thorsten Scheuermann
//         3D Application Research Group
//         ATI Research, Inc.
//
// SuSpotLight inline functions
//=================================================================================================================================
// $Id: //depot/3darg/Demos/Sushi4/Engine/SuSpotLight.inl#1 $ 
// 
// Last check-in:  $DateTime: 2012/01/09 12:48:30 $ 
// Last edited by: $Author: khillesl $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

//=================================================================================================================================
/// Sets the spot light orientation. This function is only important if the spot light is going to be used for projective
/// texturing.
///
/// \sa SetDirection
///
/// \param rDirection The direction the spot light is facing
/// \param rUp The spot light's up vector
/// \param rRight The spot light's right vector
//=================================================================================================================================
inline void SuSpotLight::SetOrientation( const SuVector3& rDirection,
                                         const SuVector3& rUp,
                                         const SuVector3& rRight )
{
   m_camera.SetOrientation( rDirection, rUp, rRight );
}

//=================================================================================================================================
/// Sets the spot light orientation using a lookat point
///
/// \param rLookAtPt The point the spot light is facing
//=================================================================================================================================
void SuSpotLight::SetLookAt( const SuPoint3& rLookAtPt )
{
   m_camera.SetLookAt( rLookAtPt, SuVector3::UNIT_Y );
}
