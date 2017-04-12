
--=================================================================================================
--
-- Author: Josh Barczak
--         3D Application Research Group  
--         ATI Research, Inc.
--
-- tolua++ parser hook which is used to work around things that the tolua parser screws up on.
--  
-- 
--=================================================================================================
-- $Id: //depot/3darg/Demos/Effects/TressFXRelease/amd_tressfx_sample/src/SushiToLuaParseHook.lua#1 $ 
--
-- Last check-in:  $DateTime: 2017/03/28 18:02:41 $ 
-- Last edited by: $Author: khillesl $
--=================================================================================================
-- (C) ATI Research, Inc. All rights reserved. 
--=================================================================================================

-- called right after processing the $[ichl]file directives,
-- right before processing anything else
-- takes the package object as the parameter
function preprocess_hook(p)
   -- p.code has all the input code from the pkg
   -- tolua screws up on our SU_DLL symbol (it thinks its a type), so strip it out
   p.code = string.gsub( p.code, "SU_DLL", "" )


end


-- called for every $ifile directive
-- takes a table with a string called 'code' inside, the filename, and any extra arguments
-- passed to $ifile. no return value
function include_file_hook(t, filename, ...)

end

-- called after processing anything that's not code (like '$renaming', comments, etc)
-- and right before parsing the actual code.
-- takes the Package object with all the code on the 'code' key. no return value
function preparse_hook(package)

_basic['SuString'] = 'sushistring'
_basic_ctype.sushistring = 'const char*'

end


-- called after writing all the output.
-- takes the Package object
function post_output_hook(package)

end

-- called at the beginning of the main parser function, with the code being parsed as a parameter
-- it can return nil if nothing was foind, or the contents of 'code', modified by the function
-- Usually a parser hook will search the beginning of the string for a token, and if it finds
-- anything, return the string without that token (after doing whatever it has to do with the token).
function parser_hook(code)
   return nil
end

