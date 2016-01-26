//Maya ASCII 2015 scene
//Name: grizzly_walkCycle.ma
//Last modified: Fri, Jul 24, 2015 09:45:43 PM
//Codeset: 1252
file -rdi 1 -ns "Grizzly_weighted3" -rfn "Grizzly_weighted3RN" -op "VERS|2015|UVER|undef|MADE|undef|CHNG|Fri, Oct 24, 2014 09:09:21 PM|ICON|undef|INFO|undef|OBJN|3260|INCL|undef(|LUNI|cm|TUNI|ntsc|AUNI|deg|"
		 "S:/gaming/isv_desktop/AMD_EFFECTS/TressFX_Assets/Grizzly/Art/Mesh/t_Grizzly_weighted4.ma";
file -r -ns "Grizzly_weighted3" -dr 1 -rfn "Grizzly_weighted3RN" -op "VERS|2015|UVER|undef|MADE|undef|CHNG|Fri, Oct 24, 2014 09:09:21 PM|ICON|undef|INFO|undef|OBJN|3260|INCL|undef(|LUNI|cm|TUNI|ntsc|AUNI|deg|"
		 "S:/gaming/isv_desktop/AMD_EFFECTS/TressFX_Assets/Grizzly/Art/Mesh/t_Grizzly_weighted4.ma";
requires maya "2015";
requires -nodeType "mentalrayFramebuffer" -nodeType "mentalrayOptions" -nodeType "mentalrayGlobals"
		 -nodeType "mentalrayItemsList" -dataType "byteArray" "Mayatomr" "2015.0 - 3.12.1.18 ";
requires -nodeType "ilrOptionsNode" -nodeType "ilrUIOptionsNode" -nodeType "ilrBakeLayerManager"
		 -nodeType "ilrBakeLayer" "Turtle" "2015.0.0";
requires "maxwell" "2.7.11";
currentUnit -l centimeter -a degree -t ntsc;
fileInfo "application" "maya";
fileInfo "product" "Maya 2015";
fileInfo "version" "2015";
fileInfo "cutIdentifier" "201503261530-955654";
fileInfo "osv" "Microsoft Windows 7 Business Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -139.46334165059801 141.98956542588826 368.84704076520825 ;
	setAttr ".r" -type "double3" -18.871881152716519 -2544.9999999989377 0 ;
createNode camera -s -n "perspShape" -p "persp";
	setAttr -k off ".v" no;
	setAttr ".pze" yes;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 302.02117405234577;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -29.62505171669838 159.45996904440958 93.44305961194425 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 338.76620878878111;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
createNode transform -s -n "front";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -10.286191501415718 54.473577662860748 1078.3071822142024 ;
createNode camera -s -n "frontShape" -p "front";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 229.85174040038677;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
createNode transform -s -n "side";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 202.27995265552252 44.291074550095104 150.26516699008573 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 324.47002058524265;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
createNode transform -n "pPlane1";
	setAttr ".s" -type "double3" 485.21010933693316 485.21010933693316 1169.5053490251755 ;
createNode mesh -n "pPlaneShape1" -p "pPlane1";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "group1";
createNode transform -n "persp1" -p "group1";
	setAttr ".t" -type "double3" 7.5479556243500632 288.66643149208778 336.69570771282451 ;
	setAttr ".r" -type "double3" -42.271881495185667 -2158.199999999084 -5.9664841274928313e-016 ;
createNode camera -n "perspShape2" -p "persp1";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 334.75320806126427;
	setAttr ".imn" -type "string" "persp1";
	setAttr ".den" -type "string" "persp1_depth";
	setAttr ".man" -type "string" "persp1_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode pointConstraint -n "group1_pointConstraint1" -p "group1";
	addAttr -dcb 0 -ci true -k true -sn "w0" -ln "keyboardMonster_root_JNT_CTRLW0" 
		-dv 1 -min 0 -at "double";
	setAttr -k on ".nds";
	setAttr -k off ".v";
	setAttr -k off ".tx";
	setAttr -k off ".ty";
	setAttr -k off ".tz";
	setAttr -k off ".rx";
	setAttr -k off ".ry";
	setAttr -k off ".rz";
	setAttr -k off ".sx";
	setAttr -k off ".sy";
	setAttr -k off ".sz";
	setAttr ".erp" yes;
	setAttr ".rst" -type "double3" 0 61.054523724452309 -3.3393722050787629 ;
	setAttr -k on ".w0";
createNode transform -n "locator1" -p "group1";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -19.741915139295742 6.4262946554091229 110.89708498384459 ;
	setAttr ".s" -type "double3" 47.796557795219421 47.796557795219421 47.796557795219421 ;
createNode locator -n "locatorShape1" -p "locator1";
	setAttr -k off ".v";
createNode transform -n "locator2" -p "group1";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 25.664707781571131 7.9637574726338016 53.001073444700701 ;
	setAttr ".s" -type "double3" 47.796557795219421 47.796557795219421 47.796557795219421 ;
createNode locator -n "locatorShape2" -p "locator2";
	setAttr -k off ".v";
createNode mentalrayItemsList -s -n "mentalrayItemsList";
	setAttr -s 2 ".opt";
createNode mentalrayGlobals -s -n "mentalrayGlobals";
createNode mentalrayOptions -s -n "miDefaultOptions";
	addAttr -ci true -m -sn "stringOptions" -ln "stringOptions" -at "compound" -nc 
		3;
	addAttr -ci true -sn "name" -ln "name" -dt "string" -p "stringOptions";
	addAttr -ci true -sn "value" -ln "value" -dt "string" -p "stringOptions";
	addAttr -ci true -sn "type" -ln "type" -dt "string" -p "stringOptions";
	setAttr -s 48 ".stringOptions";
	setAttr ".stringOptions[0].name" -type "string" "rast motion factor";
	setAttr ".stringOptions[0].value" -type "string" "1.0";
	setAttr ".stringOptions[0].type" -type "string" "scalar";
	setAttr ".stringOptions[1].name" -type "string" "rast transparency depth";
	setAttr ".stringOptions[1].value" -type "string" "8";
	setAttr ".stringOptions[1].type" -type "string" "integer";
	setAttr ".stringOptions[2].name" -type "string" "rast useopacity";
	setAttr ".stringOptions[2].value" -type "string" "true";
	setAttr ".stringOptions[2].type" -type "string" "boolean";
	setAttr ".stringOptions[3].name" -type "string" "importon";
	setAttr ".stringOptions[3].value" -type "string" "false";
	setAttr ".stringOptions[3].type" -type "string" "boolean";
	setAttr ".stringOptions[4].name" -type "string" "importon density";
	setAttr ".stringOptions[4].value" -type "string" "1.0";
	setAttr ".stringOptions[4].type" -type "string" "scalar";
	setAttr ".stringOptions[5].name" -type "string" "importon merge";
	setAttr ".stringOptions[5].value" -type "string" "0.0";
	setAttr ".stringOptions[5].type" -type "string" "scalar";
	setAttr ".stringOptions[6].name" -type "string" "importon trace depth";
	setAttr ".stringOptions[6].value" -type "string" "0";
	setAttr ".stringOptions[6].type" -type "string" "integer";
	setAttr ".stringOptions[7].name" -type "string" "importon traverse";
	setAttr ".stringOptions[7].value" -type "string" "true";
	setAttr ".stringOptions[7].type" -type "string" "boolean";
	setAttr ".stringOptions[8].name" -type "string" "shadowmap pixel samples";
	setAttr ".stringOptions[8].value" -type "string" "3";
	setAttr ".stringOptions[8].type" -type "string" "integer";
	setAttr ".stringOptions[9].name" -type "string" "ambient occlusion";
	setAttr ".stringOptions[9].value" -type "string" "false";
	setAttr ".stringOptions[9].type" -type "string" "boolean";
	setAttr ".stringOptions[10].name" -type "string" "ambient occlusion rays";
	setAttr ".stringOptions[10].value" -type "string" "64";
	setAttr ".stringOptions[10].type" -type "string" "integer";
	setAttr ".stringOptions[11].name" -type "string" "ambient occlusion cache";
	setAttr ".stringOptions[11].value" -type "string" "false";
	setAttr ".stringOptions[11].type" -type "string" "boolean";
	setAttr ".stringOptions[12].name" -type "string" "ambient occlusion cache density";
	setAttr ".stringOptions[12].value" -type "string" "1.0";
	setAttr ".stringOptions[12].type" -type "string" "scalar";
	setAttr ".stringOptions[13].name" -type "string" "ambient occlusion cache points";
	setAttr ".stringOptions[13].value" -type "string" "64";
	setAttr ".stringOptions[13].type" -type "string" "integer";
	setAttr ".stringOptions[14].name" -type "string" "irradiance particles";
	setAttr ".stringOptions[14].value" -type "string" "false";
	setAttr ".stringOptions[14].type" -type "string" "boolean";
	setAttr ".stringOptions[15].name" -type "string" "irradiance particles rays";
	setAttr ".stringOptions[15].value" -type "string" "256";
	setAttr ".stringOptions[15].type" -type "string" "integer";
	setAttr ".stringOptions[16].name" -type "string" "irradiance particles interpolate";
	setAttr ".stringOptions[16].value" -type "string" "1";
	setAttr ".stringOptions[16].type" -type "string" "integer";
	setAttr ".stringOptions[17].name" -type "string" "irradiance particles interppoints";
	setAttr ".stringOptions[17].value" -type "string" "64";
	setAttr ".stringOptions[17].type" -type "string" "integer";
	setAttr ".stringOptions[18].name" -type "string" "irradiance particles indirect passes";
	setAttr ".stringOptions[18].value" -type "string" "0";
	setAttr ".stringOptions[18].type" -type "string" "integer";
	setAttr ".stringOptions[19].name" -type "string" "irradiance particles scale";
	setAttr ".stringOptions[19].value" -type "string" "1.0";
	setAttr ".stringOptions[19].type" -type "string" "scalar";
	setAttr ".stringOptions[20].name" -type "string" "irradiance particles env";
	setAttr ".stringOptions[20].value" -type "string" "true";
	setAttr ".stringOptions[20].type" -type "string" "boolean";
	setAttr ".stringOptions[21].name" -type "string" "irradiance particles env rays";
	setAttr ".stringOptions[21].value" -type "string" "256";
	setAttr ".stringOptions[21].type" -type "string" "integer";
	setAttr ".stringOptions[22].name" -type "string" "irradiance particles env scale";
	setAttr ".stringOptions[22].value" -type "string" "1";
	setAttr ".stringOptions[22].type" -type "string" "integer";
	setAttr ".stringOptions[23].name" -type "string" "irradiance particles rebuild";
	setAttr ".stringOptions[23].value" -type "string" "true";
	setAttr ".stringOptions[23].type" -type "string" "boolean";
	setAttr ".stringOptions[24].name" -type "string" "irradiance particles file";
	setAttr ".stringOptions[24].value" -type "string" "";
	setAttr ".stringOptions[24].type" -type "string" "string";
	setAttr ".stringOptions[25].name" -type "string" "geom displace motion factor";
	setAttr ".stringOptions[25].value" -type "string" "1.0";
	setAttr ".stringOptions[25].type" -type "string" "scalar";
	setAttr ".stringOptions[26].name" -type "string" "contrast all buffers";
	setAttr ".stringOptions[26].value" -type "string" "true";
	setAttr ".stringOptions[26].type" -type "string" "boolean";
	setAttr ".stringOptions[27].name" -type "string" "finalgather normal tolerance";
	setAttr ".stringOptions[27].value" -type "string" "25.842";
	setAttr ".stringOptions[27].type" -type "string" "scalar";
	setAttr ".stringOptions[28].name" -type "string" "trace camera clip";
	setAttr ".stringOptions[28].value" -type "string" "false";
	setAttr ".stringOptions[28].type" -type "string" "boolean";
	setAttr ".stringOptions[29].name" -type "string" "unified sampling";
	setAttr ".stringOptions[29].value" -type "string" "true";
	setAttr ".stringOptions[29].type" -type "string" "boolean";
	setAttr ".stringOptions[30].name" -type "string" "samples quality";
	setAttr ".stringOptions[30].value" -type "string" "0.25 0.25 0.25 0.25";
	setAttr ".stringOptions[30].type" -type "string" "color";
	setAttr ".stringOptions[31].name" -type "string" "samples min";
	setAttr ".stringOptions[31].value" -type "string" "1.0";
	setAttr ".stringOptions[31].type" -type "string" "scalar";
	setAttr ".stringOptions[32].name" -type "string" "samples max";
	setAttr ".stringOptions[32].value" -type "string" "100.0";
	setAttr ".stringOptions[32].type" -type "string" "scalar";
	setAttr ".stringOptions[33].name" -type "string" "samples error cutoff";
	setAttr ".stringOptions[33].value" -type "string" "0.0 0.0 0.0 0.0";
	setAttr ".stringOptions[33].type" -type "string" "color";
	setAttr ".stringOptions[34].name" -type "string" "samples per object";
	setAttr ".stringOptions[34].value" -type "string" "false";
	setAttr ".stringOptions[34].type" -type "string" "boolean";
	setAttr ".stringOptions[35].name" -type "string" "progressive";
	setAttr ".stringOptions[35].value" -type "string" "false";
	setAttr ".stringOptions[35].type" -type "string" "boolean";
	setAttr ".stringOptions[36].name" -type "string" "progressive max time";
	setAttr ".stringOptions[36].value" -type "string" "0";
	setAttr ".stringOptions[36].type" -type "string" "integer";
	setAttr ".stringOptions[37].name" -type "string" "progressive subsampling size";
	setAttr ".stringOptions[37].value" -type "string" "4";
	setAttr ".stringOptions[37].type" -type "string" "integer";
	setAttr ".stringOptions[38].name" -type "string" "iray";
	setAttr ".stringOptions[38].value" -type "string" "false";
	setAttr ".stringOptions[38].type" -type "string" "boolean";
	setAttr ".stringOptions[39].name" -type "string" "light relative scale";
	setAttr ".stringOptions[39].value" -type "string" "0.31831";
	setAttr ".stringOptions[39].type" -type "string" "scalar";
	setAttr ".stringOptions[40].name" -type "string" "trace camera motion vectors";
	setAttr ".stringOptions[40].value" -type "string" "false";
	setAttr ".stringOptions[40].type" -type "string" "boolean";
	setAttr ".stringOptions[41].name" -type "string" "ray differentials";
	setAttr ".stringOptions[41].value" -type "string" "true";
	setAttr ".stringOptions[41].type" -type "string" "boolean";
	setAttr ".stringOptions[42].name" -type "string" "environment lighting mode";
	setAttr ".stringOptions[42].value" -type "string" "off";
	setAttr ".stringOptions[42].type" -type "string" "string";
	setAttr ".stringOptions[43].name" -type "string" "environment lighting quality";
	setAttr ".stringOptions[43].value" -type "string" "0.2";
	setAttr ".stringOptions[43].type" -type "string" "scalar";
	setAttr ".stringOptions[44].name" -type "string" "environment lighting shadow";
	setAttr ".stringOptions[44].value" -type "string" "transparent";
	setAttr ".stringOptions[44].type" -type "string" "string";
	setAttr ".stringOptions[45].name" -type "string" "environment lighting resolution";
	setAttr ".stringOptions[45].value" -type "string" "512";
	setAttr ".stringOptions[45].type" -type "string" "integer";
	setAttr ".stringOptions[46].name" -type "string" "environment lighting shader samples";
	setAttr ".stringOptions[46].value" -type "string" "2";
	setAttr ".stringOptions[46].type" -type "string" "integer";
	setAttr ".stringOptions[47].name" -type "string" "environment lighting scale";
	setAttr ".stringOptions[47].value" -type "string" "1.0 1.0 1.0";
	setAttr ".stringOptions[47].type" -type "string" "color";
createNode mentalrayFramebuffer -s -n "miDefaultFramebuffer";
createNode lightLinker -s -n "lightLinker1";
	setAttr -s 7 ".lnk";
	setAttr -s 7 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode reference -n "Grizzly_weighted3RN";
	setAttr -s 2 ".fn";
	setAttr ".fn[0]" -type "string" "S:/gaming/isv_desktop/AMD_EFFECTS/TressFX_Assets/Grizzly/Art/Mesh/t_Grizzly_weighted3.ma";
	setAttr ".fn[1]" -type "string" "S:/gaming/isv_desktop/AMD_EFFECTS/TressFX_Assets/Grizzly/Art/Mesh/Grizzly_weighted3.mb";
	setAttr -s 189 ".phl";
	setAttr ".phl[212]" 0;
	setAttr ".phl[213]" 0;
	setAttr ".phl[214]" 0;
	setAttr ".phl[215]" 0;
	setAttr ".phl[216]" 0;
	setAttr ".phl[217]" 0;
	setAttr ".phl[218]" 0;
	setAttr ".phl[219]" 0;
	setAttr ".phl[220]" 0;
	setAttr ".phl[221]" 0;
	setAttr ".phl[222]" 0;
	setAttr ".phl[223]" 0;
	setAttr ".phl[224]" 0;
	setAttr ".phl[225]" 0;
	setAttr ".phl[226]" 0;
	setAttr ".phl[227]" 0;
	setAttr ".phl[228]" 0;
	setAttr ".phl[229]" 0;
	setAttr ".phl[230]" 0;
	setAttr ".phl[231]" 0;
	setAttr ".phl[232]" 0;
	setAttr ".phl[233]" 0;
	setAttr ".phl[234]" 0;
	setAttr ".phl[235]" 0;
	setAttr ".phl[236]" 0;
	setAttr ".phl[237]" 0;
	setAttr ".phl[238]" 0;
	setAttr ".phl[239]" 0;
	setAttr ".phl[240]" 0;
	setAttr ".phl[241]" 0;
	setAttr ".phl[242]" 0;
	setAttr ".phl[243]" 0;
	setAttr ".phl[244]" 0;
	setAttr ".phl[245]" 0;
	setAttr ".phl[246]" 0;
	setAttr ".phl[247]" 0;
	setAttr ".phl[248]" 0;
	setAttr ".phl[249]" 0;
	setAttr ".phl[250]" 0;
	setAttr ".phl[251]" 0;
	setAttr ".phl[252]" 0;
	setAttr ".phl[253]" 0;
	setAttr ".phl[254]" 0;
	setAttr ".phl[255]" 0;
	setAttr ".phl[256]" 0;
	setAttr ".phl[257]" 0;
	setAttr ".phl[258]" 0;
	setAttr ".phl[259]" 0;
	setAttr ".phl[260]" 0;
	setAttr ".phl[261]" 0;
	setAttr ".phl[262]" 0;
	setAttr ".phl[263]" 0;
	setAttr ".phl[264]" 0;
	setAttr ".phl[265]" 0;
	setAttr ".phl[266]" 0;
	setAttr ".phl[267]" 0;
	setAttr ".phl[268]" 0;
	setAttr ".phl[269]" 0;
	setAttr ".phl[270]" 0;
	setAttr ".phl[271]" 0;
	setAttr ".phl[272]" 0;
	setAttr ".phl[273]" 0;
	setAttr ".phl[274]" 0;
	setAttr ".phl[275]" 0;
	setAttr ".phl[276]" 0;
	setAttr ".phl[277]" 0;
	setAttr ".phl[278]" 0;
	setAttr ".phl[279]" 0;
	setAttr ".phl[280]" 0;
	setAttr ".phl[281]" 0;
	setAttr ".phl[282]" 0;
	setAttr ".phl[283]" 0;
	setAttr ".phl[284]" 0;
	setAttr ".phl[285]" 0;
	setAttr ".phl[286]" 0;
	setAttr ".phl[287]" 0;
	setAttr ".phl[288]" 0;
	setAttr ".phl[289]" 0;
	setAttr ".phl[290]" 0;
	setAttr ".phl[291]" 0;
	setAttr ".phl[292]" 0;
	setAttr ".phl[293]" 0;
	setAttr ".phl[294]" 0;
	setAttr ".phl[295]" 0;
	setAttr ".phl[296]" 0;
	setAttr ".phl[297]" 0;
	setAttr ".phl[298]" 0;
	setAttr ".phl[299]" 0;
	setAttr ".phl[300]" 0;
	setAttr ".phl[301]" 0;
	setAttr ".phl[302]" 0;
	setAttr ".phl[303]" 0;
	setAttr ".phl[304]" 0;
	setAttr ".phl[305]" 0;
	setAttr ".phl[306]" 0;
	setAttr ".phl[307]" 0;
	setAttr ".phl[308]" 0;
	setAttr ".phl[309]" 0;
	setAttr ".phl[310]" 0;
	setAttr ".phl[311]" 0;
	setAttr ".phl[312]" 0;
	setAttr ".phl[313]" 0;
	setAttr ".phl[314]" 0;
	setAttr ".phl[315]" 0;
	setAttr ".phl[316]" 0;
	setAttr ".phl[317]" 0;
	setAttr ".phl[318]" 0;
	setAttr ".phl[319]" 0;
	setAttr ".phl[320]" 0;
	setAttr ".phl[321]" 0;
	setAttr ".phl[322]" 0;
	setAttr ".phl[323]" 0;
	setAttr ".phl[324]" 0;
	setAttr ".phl[325]" 0;
	setAttr ".phl[326]" 0;
	setAttr ".phl[327]" 0;
	setAttr ".phl[328]" 0;
	setAttr ".phl[329]" 0;
	setAttr ".phl[330]" 0;
	setAttr ".phl[331]" 0;
	setAttr ".phl[332]" 0;
	setAttr ".phl[333]" 0;
	setAttr ".phl[334]" 0;
	setAttr ".phl[335]" 0;
	setAttr ".phl[336]" 0;
	setAttr ".phl[337]" 0;
	setAttr ".phl[338]" 0;
	setAttr ".phl[339]" 0;
	setAttr ".phl[340]" 0;
	setAttr ".phl[341]" 0;
	setAttr ".phl[342]" 0;
	setAttr ".phl[343]" 0;
	setAttr ".phl[344]" 0;
	setAttr ".phl[345]" 0;
	setAttr ".phl[346]" 0;
	setAttr ".phl[347]" 0;
	setAttr ".phl[348]" 0;
	setAttr ".phl[349]" 0;
	setAttr ".phl[350]" 0;
	setAttr ".phl[351]" 0;
	setAttr ".phl[352]" 0;
	setAttr ".phl[353]" 0;
	setAttr ".phl[354]" 0;
	setAttr ".phl[355]" 0;
	setAttr ".phl[356]" 0;
	setAttr ".phl[357]" 0;
	setAttr ".phl[358]" 0;
	setAttr ".phl[359]" 0;
	setAttr ".phl[360]" 0;
	setAttr ".phl[361]" 0;
	setAttr ".phl[362]" 0;
	setAttr ".phl[363]" 0;
	setAttr ".phl[364]" 0;
	setAttr ".phl[365]" 0;
	setAttr ".phl[366]" 0;
	setAttr ".phl[367]" 0;
	setAttr ".phl[368]" 0;
	setAttr ".phl[369]" 0;
	setAttr ".phl[370]" 0;
	setAttr ".phl[371]" 0;
	setAttr ".phl[372]" 0;
	setAttr ".phl[373]" 0;
	setAttr ".phl[374]" 0;
	setAttr ".phl[375]" 0;
	setAttr ".phl[376]" 0;
	setAttr ".phl[377]" 0;
	setAttr ".phl[378]" 0;
	setAttr ".phl[379]" 0;
	setAttr ".phl[380]" 0;
	setAttr ".phl[381]" 0;
	setAttr ".phl[382]" 0;
	setAttr ".phl[383]" 0;
	setAttr ".phl[384]" 0;
	setAttr ".phl[385]" 0;
	setAttr ".phl[386]" 0;
	setAttr ".phl[387]" 0;
	setAttr ".phl[388]" 0;
	setAttr ".phl[389]" 0;
	setAttr ".phl[390]" 0;
	setAttr ".phl[391]" 0;
	setAttr ".phl[392]" 0;
	setAttr ".phl[393]" 0;
	setAttr ".phl[394]" 0;
	setAttr ".phl[395]" 0;
	setAttr ".phl[396]" 0;
	setAttr ".phl[397]" 0;
	setAttr ".phl[398]" 0;
	setAttr ".phl[399]" 0;
	setAttr ".phl[400]" 0;
	setAttr ".ed" -type "dataReferenceEdits" 
		"Grizzly_weighted3RN"
		"Grizzly_weighted3RN" 8
		2 "Grizzly_weighted3:xgmr:xgen_geo1" "data" " -type \"string\" -debug 1 -warning 1 -stats 1  -shutter 0.0 -file ${XGEN_ROOT}/../Mesh/Grizzly_weighted3__Grizzly.xgen -palette Grizzly_weighted3:Grizzly -geom ${XGEN_ROOT}/../Mesh/Grizzly_weighted3__Grizzly.abc -patch ${PATCH}  -description Grizzly_weighted3:fur2 -fps 30.0"
		
		2 "|Grizzly_weighted3:Grizzly" "xgFileName" " -type \"string\" \"grizzly_walkCycle__Grizzly_weighted3__Grizzly.xgen\""
		
		2 "|Grizzly_weighted3:Grizzly|Grizzly_weighted3:fur2" "inheritsTransform" 
		" 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"Heel_Roll" " -av -k 1 -0.030217073831205359"
		5 3 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.parentMatrix" 
		"Grizzly_weighted3RN.placeHolderList[205]" ""
		5 3 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.translate" 
		"Grizzly_weighted3RN.placeHolderList[206]" ""
		5 3 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.rotatePivot" 
		"Grizzly_weighted3RN.placeHolderList[210]" ""
		5 3 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.rotatePivotTranslate" 
		"Grizzly_weighted3RN.placeHolderList[211]" ""
		"Grizzly_weighted3RN" 505
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:Grizzly_Mesh|Grizzly_weighted3:grizzlyBear_body|Grizzly_weighted3:grizzlyBear_bodyShape" 
		"visibility" " -k 0 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:Grizzly_Mesh|Grizzly_weighted3:grizzlyBear_body|Grizzly_weighted3:grizzlyBear_bodyShapeOrig" 
		"uvSet[0].uvSetName" " -type \"string\" \"map1\""
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:Grizzly_Mesh|Grizzly_weighted3:grizzlyBear_bttmTeeth|Grizzly_weighted3:grizzlyBear_bttmTeethShape" 
		"visibility" " -k 0 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:Grizzly_Mesh|Grizzly_weighted3:grizzlyBear_R_eye|Grizzly_weighted3:grizzlyBear_R_eyeShape" 
		"visibility" " -k 0 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:Grizzly_Mesh|Grizzly_weighted3:grizzlyBear_tongue|Grizzly_weighted3:grizzlyBear_tongueShape" 
		"visibility" " -k 0 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:Grizzly_Mesh|Grizzly_weighted3:grizzlyBear_Upperteeth|Grizzly_weighted3:grizzlyBear_UpperteethShape" 
		"visibility" " -k 0 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:Grizzly_Mesh|Grizzly_weighted3:grizzlyBear_L_eye|Grizzly_weighted3:grizzlyBear_L_eyeShape" 
		"visibility" " -k 0 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL" 
		"translate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL" 
		"translate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL" 
		"translate" " -type \"double3\" 0 -17.567613987177481 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL" 
		"rotate" " -type \"double3\" 95.666000000000039 -0.13661442131937671 2.3294854673275363"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL" 
		"translate" " -type \"double3\" 0.11717570630582859 -0.16310795381399681 -2.8760775554822517"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL" 
		"rotate" " -type \"double3\" -70.745408418461523 -4.2302328086837848 -12.528685898421321"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL" 
		"translate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL" 
		"rotate" " -type \"double3\" 9.5791961947696134 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL" 
		"translate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL" 
		"translate" " -type \"double3\" -2.710915864 23.846766760000001 -0.9762420077127647"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL" 
		"rotate" " -type \"double3\" -0.010765621931439194 -1.7456464977679853 -1.3622786177726536"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL" 
		"translate" " -type \"double3\" 0.21389392489872083 5.4525847049999996 -0.22196066999000127"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL" 
		"rotate" " -type \"double3\" -0.11296244186577918 1.397359877684019 1.2596079093875123"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL" 
		"translate" " -type \"double3\" -0.017621931905311625 -1.1688129833795022 -1.0325895557346829"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL" 
		"rotate" " -type \"double3\" 0.39166481301587219 -3.7421958202833694 -6.6547708241350296"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL" 
		"translate" " -type \"double3\" -1.3266461306733217 -2.9668430438250541 0.059718976794088413"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL" 
		"translate" " -type \"double3\" 10.31121986 10.359652069999999 -0.16830073703868029"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL" 
		"rotate" " -type \"double3\" 0.12136475761273754 0.28959674121712259 96.4971228911661"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL" 
		"ShowFacial" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL" 
		"PrimaryFacial" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL" 
		"SecondaryFacial" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL" 
		"Nose" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL" 
		"Tongue" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL" 
		"Ear" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL" 
		"Teeth" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL" 
		"translate" " -type \"double3\" -1.3952529428422837 0.090923413625001365 -0.065359637216377581"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL" 
		"rotate" " -type \"double3\" 2.3648857654688786 -1.995266817533379 0.64940021684394866"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL" 
		"translate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL" 
		"rotate" " -type \"double3\" -16.707688589390397 2.913071823554342 5.8336274983903786"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_CTRL" 
		"rotate" " -type \"double3\" 0 -12.633300116220061 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_CTRL" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_knee_GRP|Grizzly_weighted3:keyboardMonster_R_knee_CTRL" 
		"translate" " -type \"double3\" 0 -17.375031369999999 171.98055167117985"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_knee_GRP|Grizzly_weighted3:keyboardMonster_R_knee_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_knee_GRP|Grizzly_weighted3:keyboardMonster_R_knee_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_knee_GRP|Grizzly_weighted3:keyboardMonster_R_knee_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_knee_GRP|Grizzly_weighted3:keyboardMonster_R_knee_CTRL" 
		"rotatePivot" " -type \"double3\" -23.641611099243164 46.849479675292969 36.216175079345703"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_knee_GRP|Grizzly_weighted3:keyboardMonster_R_knee_CTRL" 
		"scalePivot" " -type \"double3\" -23.641611099243164 46.849479675292969 36.216175079345703"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_knee_GRP|Grizzly_weighted3:keyboardMonster_R_knee_CTRL" 
		"ToWorld" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_knee_GRP|Grizzly_weighted3:keyboardMonster_L_knee_CTRL" 
		"translate" " -type \"double3\" -2.6468260670000001 -17.375031369999999 141.87534199117985"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_knee_GRP|Grizzly_weighted3:keyboardMonster_L_knee_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_knee_GRP|Grizzly_weighted3:keyboardMonster_L_knee_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_knee_GRP|Grizzly_weighted3:keyboardMonster_L_knee_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_knee_GRP|Grizzly_weighted3:keyboardMonster_L_knee_CTRL" 
		"rotatePivot" " -type \"double3\" 23.641611099243164 46.849479675292969 36.216175079345703"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_knee_GRP|Grizzly_weighted3:keyboardMonster_L_knee_CTRL" 
		"scalePivot" " -type \"double3\" 23.641611099243164 46.849479675292969 36.216175079345703"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_knee_GRP|Grizzly_weighted3:keyboardMonster_L_knee_CTRL" 
		"ToWorld" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_Foot_IKFK_SWITCH_CTRL" 
		"FK_IK" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_Foot_IKFK_SWITCH_CTRL" 
		"FK_IK" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_CTRL" 
		"translate" " -type \"double3\" -31.254878990000002 -104.7733163 63.242705389999998"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_CTRL" 
		"rotatePivot" " -type \"double3\" 66.564323425292969 136.88821411132812 -60.847511291503906"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_CTRL" 
		"scalePivot" " -type \"double3\" 66.564323425292969 136.88821411132812 -60.847511291503906"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_CTRL" 
		"ToWorld" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK" 
		"translate" " -type \"double3\" -19.741915139295727 6.4262946554090661 107.55771277876582"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK" 
		"translate" " -type \"double3\" 25.664707781571146 7.9637574726337732 49.661701239621941"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IKFK_SWITCH_CTRL" 
		"FK_IK" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IKFK_SWITCH_CTRL" 
		"FK_IK" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL" 
		"rotate" " -type \"double3\" -11.784296055817421 19.795358651207625 -6.0830658774984805"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index02_JNT_CTRL" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index02_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index02_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index02_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL" 
		"rotate" " -type \"double3\" -15.062881361920734 18.379592483594848 3.8089154427228529"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_CTRL" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL" 
		"rotate" " -type \"double3\" -8.7411809654318819 11.961180754712906 11.189935813111259"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_CTRL" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL" 
		"rotate" " -type \"double3\" -19.137273839203608 25.034596726446846 -2.9663727224068022"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_CTRL" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL" 
		"rotate" " -type \"double3\" -14.424156285472328 14.319006074157757 -7.0312708761945633"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_CTRL" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL" 
		"rotate" " -type \"double3\" -25.197710974416569 12.516625718085205 -27.156017552018465"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index02_JNT_CTRL" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index02_JNT_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index02_JNT_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index02_JNT_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_twistForearm_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_twistForearm_JNT_CTRL" 
		"rotateX" " -av 69.35685658955471"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_twistForearm_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_twistForearm_JNT_CTRL" 
		"rotateX" " -av 47.166126251570667"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP" 
		"translate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_CTRL" 
		"translate" " -type \"double3\" 38.93502831 -104.6730787 76.770317259999999"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_CTRL" 
		"rotatePivot" " -type \"double3\" -66.564323425292955 136.88821411132812 -60.847511291503913"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_CTRL" 
		"scalePivot" " -type \"double3\" -66.564323425292955 136.88821411132812 -60.847511291503913"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_CTRL" 
		"ToWorld" " -av -k 1 1"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"translate" " -type \"double3\" 1.696204224435802 -1.4493927001774836 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"rotatePivot" " -type \"double3\" -23.641610461600049 18.695805184209945 -10.584382204460708"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"scalePivot" " -type \"double3\" -23.641610461600049 18.695805184209945 -10.584382204460708"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"Heel_Roll" " -av -k 1 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"Ball_Roll" " -av -k 1 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"ToeTip_Roll" " -av -k 1 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"Toe_Wiggle" " -av -k 1 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL" 
		"Knee_Twist" " -av -k 1 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL|Grizzly_weighted3:keyboardMonster_R_Heel_GRP" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL|Grizzly_weighted3:keyboardMonster_R_Heel_GRP" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL|Grizzly_weighted3:keyboardMonster_R_Heel_GRP|Grizzly_weighted3:keyboardMonster_R_ToeRoll_GRP" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL|Grizzly_weighted3:keyboardMonster_R_Heel_GRP|Grizzly_weighted3:keyboardMonster_R_ToeRoll_GRP" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL|Grizzly_weighted3:keyboardMonster_R_Heel_GRP|Grizzly_weighted3:keyboardMonster_R_ToeRoll_GRP|Grizzly_weighted3:keyboardMonster_R_Ball_GRP" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL|Grizzly_weighted3:keyboardMonster_R_Heel_GRP|Grizzly_weighted3:keyboardMonster_R_ToeRoll_GRP|Grizzly_weighted3:keyboardMonster_R_Ball_GRP" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"translate" " -type \"double3\" -1.69 -4.6329418360057701 -45.733644301519895"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"rotate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"rotatePivot" " -type \"double3\" 23.641610461600045 18.695805184209945 -10.584382204460702"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"scalePivot" " -type \"double3\" 23.641610461600045 18.695805184209945 -10.584382204460702"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"Heel_Roll" " -av -k 1 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"Ball_Roll" " -av -k 1 1.8"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"ToeTip_Roll" " -av -k 1 5.3000000000000007"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"Toe_Wiggle" " -av -k 1 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL" 
		"Knee_Twist" " -av -k 1 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL|Grizzly_weighted3:keyboardMonster_L_Heel_GRP|Grizzly_weighted3:keyboardMonster_L_ToeRoll_GRP" 
		"rotate" " -type \"double3\" 32.696761710433961 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL|Grizzly_weighted3:keyboardMonster_L_Heel_GRP|Grizzly_weighted3:keyboardMonster_L_ToeRoll_GRP" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL|Grizzly_weighted3:keyboardMonster_L_Heel_GRP|Grizzly_weighted3:keyboardMonster_L_ToeRoll_GRP|Grizzly_weighted3:keyboardMonster_L_Ball_GRP" 
		"rotate" " -type \"double3\" 5.1321597466278108 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL|Grizzly_weighted3:keyboardMonster_L_Heel_GRP|Grizzly_weighted3:keyboardMonster_L_ToeRoll_GRP|Grizzly_weighted3:keyboardMonster_L_Ball_GRP" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP" 
		"translate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP" 
		"rotate" " -type \"double3\" -179.24534919304955 4.6467320619744719 56.134452044800604"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL" 
		"translate" " -type \"double3\" -62.59794828947534 114.79610307610631 -115.3911454768735"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL" 
		"rotate" " -type \"double3\" -43.322835867790467 -116.10660649682038 99.120760525626423"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL" 
		"rotatePivot" " -type \"double3\" -86.688860673967326 115.38464187807739 -11.008361849016072"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL" 
		"scalePivot" " -type \"double3\" -86.688860673967326 115.38464187807739 -11.008361849016072"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL" 
		"ToSpine" " -av -k 1 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP" 
		"translate" " -type \"double3\" 0 0 0"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP" 
		"rotate" " -type \"double3\" 0.75465080695091524 -4.6467320619743928 -56.134452044800639"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL" 
		"translate" " -type \"double3\" 59.924854051467008 -109.78407051892552 57.450438598386008"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL" 
		"translateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL" 
		"translateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL" 
		"translateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL" 
		"rotate" " -type \"double3\" -55.791411972469959 237.00144515954025 -248.64687201755169"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL" 
		"rotateX" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL" 
		"rotateY" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL" 
		"rotateZ" " -av"
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL" 
		"rotatePivot" " -type \"double3\" 86.688920294774647 115.38475244143896 -11.008377835621118"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL" 
		"scalePivot" " -type \"double3\" 86.688920294774647 115.38475244143896 -11.008377835621118"
		
		2 "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL" 
		"ToSpine" " -av -k 1 0"
		2 "Grizzly_weighted3:xgmr:xgen_geo1" "patches" " -type \"string\" \"Grizzly_weighted3:grizzlyBear_body\""
		
		2 "Grizzly_weighted3:xgmr:xgen_geo1" "approx_degree" " 1"
		2 "Grizzly_weighted3:xgmr:xgen_geo1" "approx_mode" " 0"
		2 "Grizzly_weighted3:xgmr:xgen_geo1" "approx_parametric_subdivisions" " 3"
		
		2 "Grizzly_weighted3:xgmr:xgen_geo1" "approx_fine_sub_pixel_size" " 0.10000000149011612"
		
		2 "Grizzly_weighted3:xgmr:xgen_geo1" "motion_blur" " 0"
		2 "Grizzly_weighted3:xgmr:xgen_geo1" "motion_blur_mode" " 1"
		2 "Grizzly_weighted3:xgmr:xgen_geo1" "motion_blur_steps" " 5"
		2 "Grizzly_weighted3:xgmr:xgen_geo1" "motion_blur_factor" " 1"
		2 "Grizzly_weighted3:xgmr:xgen_geo1" "motion_blur_multiplier" " 1"
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[212]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[213]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[214]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[215]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[216]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[217]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[218]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[219]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[220]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[221]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[222]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[223]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[224]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[225]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[226]" ""
		5 3 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.parentMatrix" 
		"Grizzly_weighted3RN.placeHolderList[227]" ""
		5 3 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.translate" 
		"Grizzly_weighted3RN.placeHolderList[228]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[229]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[230]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[231]" ""
		5 3 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.rotatePivot" 
		"Grizzly_weighted3RN.placeHolderList[232]" ""
		5 3 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL.rotatePivotTranslate" 
		"Grizzly_weighted3RN.placeHolderList[233]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[234]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[235]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[236]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[237]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[238]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[239]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[240]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[241]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[242]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[243]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[244]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_legadjust_JNT_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[245]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[246]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[247]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[248]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[249]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[250]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_pelvis_JNT_GRP|Grizzly_weighted3:keyboardMonster_pelvis_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_legadjust_JNT_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[251]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[252]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[253]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[254]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[255]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[256]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[257]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[258]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[259]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[260]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[261]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[262]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[263]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[264]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[265]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[266]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[267]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[268]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[269]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[270]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[271]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[272]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[273]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[274]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[275]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[276]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[277]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[278]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[279]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[280]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[281]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL.ShowFacial" 
		"Grizzly_weighted3RN.placeHolderList[282]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL.PrimaryFacial" 
		"Grizzly_weighted3RN.placeHolderList[283]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL.SecondaryFacial" 
		"Grizzly_weighted3RN.placeHolderList[284]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL.Nose" 
		"Grizzly_weighted3RN.placeHolderList[285]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL.Tongue" 
		"Grizzly_weighted3RN.placeHolderList[286]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL.Ear" 
		"Grizzly_weighted3RN.placeHolderList[287]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_neck_JNT_GRP|Grizzly_weighted3:keyboardMonster_neck_JNT_CTRL|Grizzly_weighted3:keyboardMonster_head_JNT_GRP|Grizzly_weighted3:keyboardMonster_head_JNT_CTRL|Grizzly_weighted3:keyboardMonster_Facial_CTRL.Teeth" 
		"Grizzly_weighted3RN.placeHolderList[288]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[289]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[290]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[291]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[292]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[293]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_Clavicle_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[294]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[295]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[296]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[297]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[298]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[299]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_Clavicle_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[300]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[301]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[302]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pectoral_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[303]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[304]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[305]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_root_JNT_GRP|Grizzly_weighted3:keyboardMonster_root_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine01_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine02_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine02_JNT_CTRL|Grizzly_weighted3:keyboardMonster_spine03_JNT_GRP|Grizzly_weighted3:keyboardMonster_spine03_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pectoral_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[306]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_knee_GRP|Grizzly_weighted3:keyboardMonster_R_knee_CTRL.ToWorld" 
		"Grizzly_weighted3RN.placeHolderList[307]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_knee_GRP|Grizzly_weighted3:keyboardMonster_R_knee_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[308]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_knee_GRP|Grizzly_weighted3:keyboardMonster_R_knee_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[309]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_knee_GRP|Grizzly_weighted3:keyboardMonster_R_knee_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[310]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_knee_GRP|Grizzly_weighted3:keyboardMonster_L_knee_CTRL.ToWorld" 
		"Grizzly_weighted3RN.placeHolderList[311]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_knee_GRP|Grizzly_weighted3:keyboardMonster_L_knee_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[312]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_knee_GRP|Grizzly_weighted3:keyboardMonster_L_knee_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[313]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_knee_GRP|Grizzly_weighted3:keyboardMonster_L_knee_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[314]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_Foot_IKFK_SWITCH_CTRL.FK_IK" 
		"Grizzly_weighted3RN.placeHolderList[315]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_Foot_IKFK_SWITCH_CTRL.FK_IK" 
		"Grizzly_weighted3RN.placeHolderList[316]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_CTRL.ToWorld" 
		"Grizzly_weighted3RN.placeHolderList[317]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[318]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[319]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_elbow2_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_GRP|Grizzly_weighted3:keyboardMonster_L_elbow_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[320]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IKFK_SWITCH_CTRL.FK_IK" 
		"Grizzly_weighted3RN.placeHolderList[321]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IKFK_SWITCH_CTRL.FK_IK" 
		"Grizzly_weighted3RN.placeHolderList[322]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[323]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[324]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[325]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index02_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[326]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index02_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[327]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_index02_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[328]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[329]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[330]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[331]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[332]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[333]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_middle02_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[334]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[335]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[336]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[337]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[338]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[339]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_fingers_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_pinky02_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[340]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[341]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[342]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[343]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[344]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[345]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_pinky02_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[346]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[347]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[348]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[349]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[350]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[351]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_middle02_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[352]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[353]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[354]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[355]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index02_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[356]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index02_JNT_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[357]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_fingers_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index01_JNT_CTRL|Grizzly_weighted3:keyboardMonster_R_index02_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_index02_JNT_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[358]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_L_twistForearm_JNT_GRP|Grizzly_weighted3:keyboardMonster_L_twistForearm_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[359]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_twistForearm_JNT_GRP|Grizzly_weighted3:keyboardMonster_R_twistForearm_JNT_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[360]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_CTRL.ToWorld" 
		"Grizzly_weighted3RN.placeHolderList[361]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[362]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[363]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_Others_GRP|Grizzly_weighted3:keyboardMonster_R_elbow2_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_GRP|Grizzly_weighted3:keyboardMonster_R_elbow_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[364]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL.Heel_Roll" 
		"Grizzly_weighted3RN.placeHolderList[365]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL.Ball_Roll" 
		"Grizzly_weighted3RN.placeHolderList[366]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL.ToeTip_Roll" 
		"Grizzly_weighted3RN.placeHolderList[367]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL.Toe_Wiggle" 
		"Grizzly_weighted3RN.placeHolderList[368]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL.Knee_Twist" 
		"Grizzly_weighted3RN.placeHolderList[369]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[370]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[371]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[372]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[373]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[374]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Foot_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[375]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL.Heel_Roll" 
		"Grizzly_weighted3RN.placeHolderList[376]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL.Ball_Roll" 
		"Grizzly_weighted3RN.placeHolderList[377]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL.ToeTip_Roll" 
		"Grizzly_weighted3RN.placeHolderList[378]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL.Toe_Wiggle" 
		"Grizzly_weighted3RN.placeHolderList[379]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL.Knee_Twist" 
		"Grizzly_weighted3RN.placeHolderList[380]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[381]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[382]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[383]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[384]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[385]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Foot_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[386]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL.ToSpine" 
		"Grizzly_weighted3RN.placeHolderList[387]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[388]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[389]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[390]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[391]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[392]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_R_Wrist_IK_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[393]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL.ToSpine" 
		"Grizzly_weighted3RN.placeHolderList[394]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL.translateX" 
		"Grizzly_weighted3RN.placeHolderList[395]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL.translateY" 
		"Grizzly_weighted3RN.placeHolderList[396]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL.translateZ" 
		"Grizzly_weighted3RN.placeHolderList[397]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL.rotateX" 
		"Grizzly_weighted3RN.placeHolderList[398]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL.rotateY" 
		"Grizzly_weighted3RN.placeHolderList[399]" ""
		5 4 "Grizzly_weighted3RN" "|Grizzly_weighted3:keyboardMonster|Grizzly_weighted3:keyboardMonster_RIG|Grizzly_weighted3:keyboardMonster_Global_CTRL|Grizzly_weighted3:keyboardMonster_Main_CTRL|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_GRP|Grizzly_weighted3:keyboardMonster_L_Wrist_IK_CTRL.rotateZ" 
		"Grizzly_weighted3RN.placeHolderList[400]" "";
	setAttr ".ptag" -type "string" "";
lockNode -l 1 ;
createNode ilrOptionsNode -s -n "TurtleRenderOptions";
lockNode -l 1 ;
createNode ilrUIOptionsNode -s -n "TurtleUIOptions";
lockNode -l 1 ;
createNode ilrBakeLayerManager -s -n "TurtleBakeLayerManager";
lockNode -l 1 ;
createNode ilrBakeLayer -s -n "TurtleDefaultBakeLayer";
lockNode -l 1 ;
createNode mentalrayOptions -s -n "miContourPreset";
createNode expression -n "xgmRefreshPreview";
	setAttr -k on ".nds";
	setAttr ".ixp" -type "string" "xgmPreview -r";
	setAttr ".uno" 1;
createNode script -n "uiConfigurationScriptNode";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n"
		+ "                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n"
		+ "                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n"
		+ "            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n"
		+ "            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp1\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n"
		+ "                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n"
		+ "                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n"
		+ "            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp1\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n"
		+ "            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n"
		+ "            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n"
		+ "                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n"
		+ "                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n"
		+ "                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n"
		+ "            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n"
		+ "            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n"
		+ "            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Perspective\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Perspective\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n"
		+ "                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 1\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n"
		+ "                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 0\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 0\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n"
		+ "                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Perspective\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n"
		+ "            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 1\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n"
		+ "            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 0\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 0\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 0\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n"
		+ "            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -docTag \"isolOutln_fromSeln\" \n                -showShapes 0\n"
		+ "                -showReferenceNodes 1\n                -showReferenceMembers 1\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 1\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n"
		+ "                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                -ignoreHiddenAttribute 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n"
		+ "            -showShapes 0\n            -showReferenceNodes 1\n            -showReferenceMembers 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 1\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n"
		+ "            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\tif ($useSceneConfig) {\n\t\toutlinerPanel -e -to $panelName;\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n"
		+ "                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n"
		+ "                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 1\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1.25\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -clipTime \"on\" \n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 1\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n"
		+ "            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n"
		+ "                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n"
		+ "                -displayActiveKeyTangents 1\n                -displayInfinities 1\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1.25\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -clipTime \"on\" \n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 1\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\tif ($useSceneConfig) {\n\t\tscriptedPanel -e -to $panelName;\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n"
		+ "                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n"
		+ "                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n"
		+ "                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n"
		+ "                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n"
		+ "                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n"
		+ "                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n"
		+ "                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n"
		+ "                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\tif ($useSceneConfig) {\n\t\tscriptedPanel -e -to $panelName;\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"nodeEditorPanel\" -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n"
		+ "            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -ignoreAssets 1\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -keyReleaseCommand \"nodeEdKeyReleaseCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                $editorName;\n\t\t\tif (`objExists nodeEditorPanel1Info`) nodeEditor -e -restoreInfo nodeEditorPanel1Info $editorName;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -ignoreAssets 1\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -keyReleaseCommand \"nodeEdKeyReleaseCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n"
		+ "                -syncedSelection 1\n                -extendToShapes 1\n                $editorName;\n\t\t\tif (`objExists nodeEditorPanel1Info`) nodeEditor -e -restoreInfo nodeEditorPanel1Info $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"createNodePanel\" -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Texture Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n"
		+ "                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n"
		+ "                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 0\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n"
		+ "                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n"
		+ "            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 4 4 \n            -bumpResolution 4 4 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n"
		+ "            -maximumNumHardwareLights 0\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 0\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n"
		+ "            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Perspective\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Perspective\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -camera \\\"persp\\\" \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 1\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 0\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 0\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 0\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Perspective\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -camera \\\"persp\\\" \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 1\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 0\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 0\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 0\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 43 -ast 1 -aet 43 ";
	setAttr ".st" 6;
createNode polyPlane -n "polyPlane1";
	setAttr ".cuv" 2;
createNode script -n "xgenGlobals";
	setAttr ".a" -type "string" "import maya.cmds as cmds\nif cmds.about(batch=True):\n\txgg.Playblast=False\nelse:\n\txgui.createDescriptionEditor(False).setGlobals( previewSel=0, previewMode=0, clearSel=0, clearMode=0, playblast=1, clearCache=0, autoCreateMR=1 )";
	setAttr ".stp" 1;
	setAttr ".ire" 1;
createNode reference -n "sharedReferenceNode";
	setAttr ".ed" -type "dataReferenceEdits" 
		"sharedReferenceNode";
createNode animCurveTL -n "keyboardMonster_Global_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTL -n "keyboardMonster_Global_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTL -n "keyboardMonster_Global_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTL -n "keyboardMonster_Main_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTL -n "keyboardMonster_Main_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTL -n "keyboardMonster_Main_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTL -n "keyboardMonster_L_elbow_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 -31.254878990000002 11 -31.25487899204704
		 16 -33.328660480924427 22 -23.47170373322443 33 -31.25487899204704 44 -31.254878990000002;
createNode animCurveTL -n "keyboardMonster_L_elbow_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 -104.7733163 11 -104.77331627101329 22 -104.77331627101329
		 33 -104.77331627101329 44 -104.7733163;
createNode animCurveTL -n "keyboardMonster_L_elbow_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 63.242705389999998 11 63.242705393271223
		 22 63.242705393271223 33 63.242705393271223 44 63.242705389999998;
createNode animCurveTL -n "keyboardMonster_L_knee_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 -2.6468260670000001 11 -2.646826067370192
		 22 -2.646826067370192 33 -2.646826067370192 44 -2.6468260670000001;
createNode animCurveTL -n "keyboardMonster_L_knee_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 -17.375031369999999 11 -17.375031373091851
		 22 -17.375031373091851 33 -17.375031373091851 44 -17.375031369999999;
createNode animCurveTL -n "keyboardMonster_L_knee_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 141.87534199117985 11 45.557451344151531
		 22 96.469457811350878 33 109.95884656290633 44 141.87534199117985;
createNode animCurveTL -n "keyboardMonster_R_elbow_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 38.93502831 11 38.935028310950806 22 38.935028310950806
		 33 38.935028310950806 44 38.93502831;
createNode animCurveTL -n "keyboardMonster_R_elbow_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 -104.6730787 11 -104.67307874413413 22 -104.67307874413413
		 33 -104.67307874413413 44 -104.6730787;
createNode animCurveTL -n "keyboardMonster_R_elbow_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 76.770317259999999 11 76.770317256314343
		 22 76.770317256314343 33 76.770317256314343 44 76.770317259999999;
createNode animCurveTL -n "keyboardMonster_R_knee_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTL -n "keyboardMonster_R_knee_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 -17.375031369999999 11 -17.375031373090238
		 22 -17.375031373090238 33 -17.375031373090238 44 -17.375031369999999;
createNode animCurveTL -n "keyboardMonster_R_knee_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 171.98055167117985 11 52.729634327544275
		 22 64.693192977707341 33 114.28900581400424 44 171.98055167117985;
createNode animCurveTL -n "keyboardMonster_pelvis_JNT_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 8 ".ktv[0:7]"  1 0.11717570630582859 6 0.17460056875554386
		 11 0.11717570630582859 22 0.11717570630582859 27 0.13626636642885459 33 0.11717570630582859
		 38 0.08393897298675991 44 0.11717570630582859;
createNode animCurveTL -n "keyboardMonster_pelvis_JNT_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 8 ".ktv[0:7]"  1 -0.16310795381399681 6 -0.28785990992293459
		 11 -0.16310795381399681 22 -0.16310795381399681 27 -0.20992528652132084 33 -0.16310795381399681
		 38 -0.092012098958038357 44 -0.16310795381399681;
createNode animCurveTL -n "keyboardMonster_pelvis_JNT_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 8 ".ktv[0:7]"  1 -2.8760775554822517 6 -4.4232157627990549
		 11 -2.8760775554822517 22 -2.8760775554822517 27 -3.4389364053933789 33 -2.8760775554822517
		 38 -1.9690130305587239 44 -2.8760775550000002;
createNode animCurveTL -n "keyboardMonster_L_legadjust_JNT_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTL -n "keyboardMonster_L_legadjust_JNT_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTL -n "keyboardMonster_L_legadjust_JNT_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTL -n "keyboardMonster_R_legadjust_JNT_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTL -n "keyboardMonster_R_legadjust_JNT_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTL -n "keyboardMonster_R_legadjust_JNT_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTL -n "keyboardMonster_spine01_JNT_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 -2.710915864 11 -2.7109158635846229 22 -2.7109158635846229
		 27 -2.7109158635846229 33 -2.7109158635846229 44 -2.710915864;
createNode animCurveTL -n "keyboardMonster_spine01_JNT_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 23.846766760000001 11 23.846766759290283
		 22 23.846766759290283 27 23.846766759290283 33 23.846766759290283 44 23.846766760000001;
createNode animCurveTL -n "keyboardMonster_spine01_JNT_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 -0.9762420077127647 11 -0.9762420077127647
		 22 -0.9762420077127647 27 -0.9762420077127647 33 -0.9762420077127647 44 -0.9762420077127647;
createNode animCurveTL -n "keyboardMonster_spine02_JNT_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0.21389392489872083 11 0.21389392489872083
		 22 0.21389392489872083 33 0.21389392489872083 44 0.21389392489872083;
createNode animCurveTL -n "keyboardMonster_spine02_JNT_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 5.4525847049999996 11 5.4525847051815557
		 22 5.4525847051815557 33 5.4525847051815557 44 5.4525847049999996;
createNode animCurveTL -n "keyboardMonster_spine02_JNT_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 -0.22196066999000127 11 -0.22196066999000127
		 22 -0.22196066999000127 33 -0.22196066999000127 44 -0.22196066999000127;
createNode animCurveTL -n "keyboardMonster_spine03_JNT_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 -0.017621931905311625 5 -0.055314412878606153
		 11 -0.032951462454250249 16 -0.050218345574080031 22 0.21565009289709777 27 0.047623047396714607
		 32 -0.32136691302270903 38 -0.55200969400740119 44 -0.017621931905311625;
	setAttr -s 9 ".kit[6:8]"  1 9 9;
	setAttr -s 9 ".kot[6:8]"  1 9 9;
	setAttr -s 9 ".kix[6:8]"  0.44668486714363098 0.79640674591064453 
		0.35051587224006653;
	setAttr -s 9 ".kiy[6:8]"  -0.89469146728515625 0.60476142168045044 
		0.93655675649642944;
	setAttr -s 9 ".kox[6:8]"  0.44668486714363098 0.79640674591064453 
		0.35051587224006653;
	setAttr -s 9 ".koy[6:8]"  -0.8946913480758667 0.60476142168045044 
		0.93655675649642944;
	setAttr ".pre" 3;
	setAttr ".pst" 3;
createNode animCurveTL -n "keyboardMonster_spine03_JNT_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 -1.1688129833795022 5 -1.8186142751747894
		 11 -1.2869928664291237 16 -0.26128275499088793 22 0.25220710023364473 27 -1.6904509194589468
		 32 -1.5032596497810331 38 0.36739806566697847 44 -1.1688129833795022;
	setAttr -s 9 ".kit[0:8]"  1 1 1 1 9 9 9 9 
		9;
	setAttr -s 9 ".kot[0:8]"  1 1 1 1 9 9 9 9 
		9;
	setAttr -s 9 ".kix[0:8]"  0.17361651360988617 0.48132026195526123 
		0.14236980676651001 0.15991403162479401 0.24851097166538239 0.18654973804950714 0.17541678249835968 
		0.76717007160186768 0.1291009783744812;
	setAttr -s 9 ".kiy[0:8]"  -0.98481333255767822 -0.87654483318328857 
		0.98981356620788574 0.98713093996047974 -0.96862918138504028 -0.98244547843933105 
		0.98449426889419556 0.64144372940063477 -0.99163144826889038;
	setAttr -s 9 ".kox[0:8]"  0.17361649870872498 0.48132017254829407 
		0.14236980676651001 0.15991400182247162 0.24851097166538239 0.18654973804950714 0.17541678249835968 
		0.76717007160186768 0.1291009783744812;
	setAttr -s 9 ".koy[0:8]"  -0.98481333255767822 -0.87654483318328857 
		0.98981356620788574 0.98713093996047974 -0.96862918138504028 -0.98244547843933105 
		0.98449426889419556 0.64144372940063477 -0.99163144826889038;
	setAttr ".pre" 3;
	setAttr ".pst" 3;
createNode animCurveTL -n "keyboardMonster_spine03_JNT_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 -1.0325895557346829 5 -0.69706365261406811
		 11 -0.91086889680033634 16 0.054007610377906398 22 2.5644266212019642 27 1.4207241410845963
		 32 -0.4899328769283155 38 -2.0896970250630789 44 -1.0325895557346829;
	setAttr -s 9 ".kit[8]"  1;
	setAttr -s 9 ".kot[8]"  1;
	setAttr -s 9 ".kix[8]"  0.11934085935354233;
	setAttr -s 9 ".kiy[8]"  0.99285334348678589;
	setAttr -s 9 ".kox[8]"  0.11934086680412292;
	setAttr -s 9 ".koy[8]"  0.99285340309143066;
	setAttr ".pre" 3;
	setAttr ".pst" 3;
createNode animCurveTL -n "keyboardMonster_L_Clavicle_JNT_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 -1.3952529428422837 6 -1.0845814987926319
		 11 0 17 -7.5657391137975658 22 -3.6998818862060388 26 -3.8964295973878986 33 -6.5862517604036679
		 38 -4.3983364747138767 44 -1.3952529428422837;
createNode animCurveTL -n "keyboardMonster_L_Clavicle_JNT_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 0.090923413625001365 6 0.087513401024861145
		 11 0 17 0.46526385116936186 22 0.23505355852746729 26 0.23958685702977309 33 0.24046306188941829
		 38 0.15270060419206696 44 0.090923413625001365;
createNode animCurveTL -n "keyboardMonster_L_Clavicle_JNT_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 -0.065359637216377581 6 -0.062804384324474605
		 11 0 17 -0.37936017313326342 22 -0.27846672271257888 26 -0.25518820274597909 33 0.11209881251096025
		 38 0.098992288833168796 44 -0.065359637216377581;
createNode animCurveTL -n "keyboardMonster_R_Clavicle_JNT_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 7 ".ktv[0:6]"  1 0 11 2.2489288284885287 22 0 28 0.86952769961208221
		 33 0 40 0.71910377645940837 44 0;
createNode animCurveTL -n "keyboardMonster_R_Clavicle_JNT_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 7 ".ktv[0:6]"  1 0 11 -0.18668722008315655 22 0 28 -0.040997154314390175
		 33 0 40 -0.030735011886864602 44 0;
createNode animCurveTL -n "keyboardMonster_R_Clavicle_JNT_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 7 ".ktv[0:6]"  1 0 11 0.0693877065573556 22 0 28 0.018523350930262965
		 33 0 40 0.0047653208810331518 44 0;
createNode animCurveTL -n "keyboardMonster_neck_JNT_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 -1.3266461306733217 11 -1.3266461306733217
		 22 -1.3266461306733217 33 -1.3266461306733217 44 -1.326646131;
createNode animCurveTL -n "keyboardMonster_neck_JNT_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 -2.9668430438250541 11 -2.9668430438250541
		 22 -2.9668430438250541 33 -2.9668430438250541 44 -2.966843044;
createNode animCurveTL -n "keyboardMonster_neck_JNT_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0.059718976794088413 11 0.059718976794088413
		 22 0.059718976794088413 33 0.059718976794088413 44 0.059718976794088413;
createNode animCurveTL -n "keyboardMonster_head_JNT_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 10.31121986 6 10.307764899982265 12 10.311219862151093
		 20 10.311219863483908 25 10.305446716217579 44 10.31121986;
createNode animCurveTL -n "keyboardMonster_head_JNT_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 10.359652069999999 6 10.30353962714258
		 12 10.359652068656603 20 10.359652067824236 25 10.421082618385434 44 10.359652069999999;
createNode animCurveTL -n "keyboardMonster_head_JNT_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 -0.16830073703868029 6 -0.6575739242433104
		 12 -0.16830073703868029 20 -0.16830073703868029 25 0.54197392173979053 44 -0.16830073703868029;
createNode animCurveTA -n "keyboardMonster_Global_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_Global_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_Global_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_Main_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_Main_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_Main_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_pelvis_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 8 ".ktv[0:7]"  1 -70.745408418461523 6 -72.645985532378276
		 11 -73.235719736104215 22 -64.310180364592938 27 -63.376244669400883 33 -64.194163583540231
		 38 -66.930879366966593 44 -70.745408420000004;
createNode animCurveTA -n "keyboardMonster_pelvis_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 8 ".ktv[0:7]"  1 -4.2302328086837848 6 -0.39664818456439455
		 11 -1.1274648605616293 22 -4.3726679411765952 27 -3.6941383186657477 33 -6.7366749523434724
		 38 -5.9832578344658414 44 -4.2302328090000003;
createNode animCurveTA -n "keyboardMonster_pelvis_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 8 ".ktv[0:7]"  1 -12.528685898421321 6 -0.37881682779538717
		 11 9.7309441423667469 22 13.110437384693267 27 4.3462716804720447 33 -9.956554094942927
		 38 -12.254519532596898 44 -12.528685900000001;
createNode animCurveTA -n "keyboardMonster_L_legadjust_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 9.5791961947696134 11 0 22 0 33 0 44 9.5791961947696134;
createNode animCurveTA -n "keyboardMonster_L_legadjust_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 -1.9400155663660832 44 0;
createNode animCurveTA -n "keyboardMonster_L_legadjust_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_legadjust_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_legadjust_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 3.9313530896903601 33 3.9313530896903601
		 44 0;
createNode animCurveTA -n "keyboardMonster_R_legadjust_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_spine01_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 -0.010765621931439194 11 -0.010797506986962592
		 22 -0.010760631061098933 27 -0.010794817703918356 33 -0.010774400168809544 44 -0.010765621931439194;
createNode animCurveTA -n "keyboardMonster_spine01_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 -1.7456464977679853 11 -4.7369918040875616
		 22 -0.057213545467286291 27 1.3166150653085749 33 0.40772377079610816 44 -1.7456464977679853;
createNode animCurveTA -n "keyboardMonster_spine01_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 -1.3622786177726536 11 -1.36171488705607
		 22 -1.362595821055806 27 -1.3634524868058924 33 -1.3631512070648499 44 -1.3622786177726536;
createNode animCurveTA -n "keyboardMonster_spine02_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 -0.11296244186577918 11 0.013546758946229609
		 22 -0.79385072841856097 33 0.013533713949427125 44 -0.11296244186577918;
createNode animCurveTA -n "keyboardMonster_spine02_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1.397359877684019 11 3.1486333159017743
		 22 -0.024470026128454732 33 -1.8954569471718903 44 1.397359877684019;
createNode animCurveTA -n "keyboardMonster_spine02_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1.2596079093875123 11 0.9040408500492586
		 22 1.4771542477205095 33 0.90284913564649172 44 1.2596079093875123;
createNode animCurveTA -n "keyboardMonster_spine03_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 0.39166481301587219 5 1.8109548348235562
		 11 0.076550633747340752 16 0.5194296099486837 22 1.5376175522371511 27 1.2148761826894034
		 32 -1.8473029780206527 38 -1.5010621529608139 44 0.39166481301587219;
	setAttr -s 9 ".kit[0:8]"  1 9 9 9 9 9 9 9 
		9;
	setAttr -s 9 ".kot[0:8]"  1 9 9 9 9 9 9 9 
		9;
	setAttr -s 9 ".kix[0:8]"  0.96939557790756226 0.99986386299133301 
		0.9981156587600708 0.99759042263031006 0.99945253133773804 0.98465466499328613 0.99174678325653076 
		0.99526190757751465 0.98663210868835449;
	setAttr -s 9 ".kiy[0:8]"  0.2455039918422699 -0.016497092321515083 
		-0.061360608786344528 0.069379039108753204 0.033085059374570847 -0.1745142936706543 
		-0.12821140885353088 0.097230531275272369 0.16296355426311493;
	setAttr -s 9 ".kox[0:8]"  0.96939557790756226 0.99986386299133301 
		0.9981156587600708 0.99759042263031006 0.99945253133773804 0.98465466499328613 0.99174678325653076 
		0.99526190757751465 0.98663210868835449;
	setAttr -s 9 ".koy[0:8]"  0.24550400674343109 -0.016497092321515083 
		-0.061360608786344528 0.069379039108753204 0.033085059374570847 -0.1745142936706543 
		-0.12821140885353088 0.097230531275272369 0.16296355426311493;
	setAttr ".pre" 3;
	setAttr ".pst" 3;
createNode animCurveTA -n "keyboardMonster_spine03_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 -3.7421958202833694 5 -4.6404187944585598
		 11 -2.3244811020734741 16 -2.8130573470374665 22 -3.9018425774849907 27 -3.4874665424605036
		 32 -0.96587708612193179 38 -0.89711105918286294 44 -3.7421958202833694;
	setAttr -s 9 ".kit[0:8]"  1 1 9 1 9 9 9 9 
		9;
	setAttr -s 9 ".kot[0:8]"  1 1 9 1 9 9 9 9 
		9;
	setAttr -s 9 ".kix[0:8]"  0.99507218599319458 0.99982786178588867 
		0.9962383508682251 0.99380069971084595 0.99948513507843018 0.98838937282562256 0.99248415231704712 
		0.99274230003356934 0.97053396701812744;
	setAttr -s 9 ".kiy[0:8]"  -0.099153123795986176 -0.01855502650141716 
		0.086655005812644958 -0.11117652803659439 -0.032085280865430832 0.15194191038608551 
		0.12237391620874405 -0.1202605664730072 -0.24096457660198212;
	setAttr -s 9 ".kox[0:8]"  0.99507218599319458 0.99982786178588867 
		0.9962383508682251 0.99380069971084595 0.99948513507843018 0.98838937282562256 0.99248415231704712 
		0.99274230003356934 0.97053396701812744;
	setAttr -s 9 ".koy[0:8]"  -0.099153123795986176 -0.018555022776126862 
		0.086655005812644958 -0.11117654293775558 -0.032085280865430832 0.15194191038608551 
		0.12237391620874405 -0.1202605664730072 -0.24096457660198212;
	setAttr ".pre" 3;
	setAttr ".pst" 3;
createNode animCurveTA -n "keyboardMonster_spine03_JNT_CTRL_rotateZ";
	setAttr ".tan" 1;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 -6.6547708241350296 5 -7.9339498182182266
		 11 -7.2504239369097263 16 -6.1558088799169104 22 -4.7149902141375586 27 -5.4415153337688329
		 32 -4.8288409506107559 38 -2.0965783534005853 44 -6.6547708241350296;
	setAttr -s 9 ".kit[2:8]"  9 1 1 1 1 1 9;
	setAttr -s 9 ".kot[2:8]"  9 1 1 1 1 1 9;
	setAttr -s 9 ".kix[0:8]"  0.97682785987854004 0.99998897314071655 
		0.99643731117248535 0.99303251504898071 0.9999154806137085 0.99977594614028931 0.98533022403717041 
		0.99924385547637939 0.92918723821640015;
	setAttr -s 9 ".kiy[0:8]"  -0.21402661502361298 0.0047014812007546425 
		0.084337763488292694 0.11784103512763977 -0.013004869222640991 -0.021168159320950508 
		0.17065908014774323 -0.038880746811628342 -0.369609534740448;
	setAttr -s 9 ".kox[0:8]"  0.97682780027389526 0.99998897314071655 
		0.99643731117248535 0.99303251504898071 0.9999154806137085 0.99977594614028931 0.98533016443252563 
		0.99924385547637939 0.92918723821640015;
	setAttr -s 9 ".koy[0:8]"  -0.21402671933174133 0.0047014751471579075 
		0.084337763488292694 0.11784102022647858 -0.01300487108528614 -0.02116808295249939 
		0.17065905034542084 -0.038880743086338043 -0.369609534740448;
	setAttr ".pre" 3;
	setAttr ".pst" 3;
createNode animCurveTA -n "keyboardMonster_neck_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_neck_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_neck_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_head_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 8 ".ktv[0:7]"  1 0.12136475761273754 6 -3.3257775976538353
		 12 -1.4027915729266671 20 -0.97371524775509 25 1.5037572969945923 31 4.359337591278277
		 37 2.9236300185546238 44 0.12136475761273754;
createNode animCurveTA -n "keyboardMonster_head_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 8 ".ktv[0:7]"  1 0.28959674121712259 6 1.884037791113012
		 12 1.007273249615473 20 2.7320797483070884 25 1.7782291985240779 31 4.5463623259328934
		 37 2.5769070566424293 44 0.28959674121712259;
createNode animCurveTA -n "keyboardMonster_head_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 8 ".ktv[0:7]"  1 96.4971228911661 6 91.997372626402608
		 12 91.915645356930327 20 101.57773157164617 25 97.486495453020112 31 92.108609389527956
		 37 92.331756993536729 44 96.4971228911661;
	setAttr -s 8 ".kit[0:7]"  3 9 9 9 9 9 9 3;
	setAttr -s 8 ".kot[0:7]"  3 9 9 9 9 9 9 3;
createNode animCurveTU -n "keyboardMonster_Facial_CTRL_ShowFacial";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 11 1 22 1 33 1 44 1;
	setAttr -s 5 ".kot[0:4]"  5 5 5 5 5;
createNode animCurveTU -n "keyboardMonster_Facial_CTRL_PrimaryFacial";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 11 1 22 1 33 1 44 1;
	setAttr -s 5 ".kot[0:4]"  5 5 5 5 5;
createNode animCurveTU -n "keyboardMonster_Facial_CTRL_SecondaryFacial";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 11 1 22 1 33 1 44 1;
	setAttr -s 5 ".kot[0:4]"  5 5 5 5 5;
createNode animCurveTU -n "keyboardMonster_Facial_CTRL_Nose";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 11 1 22 1 33 1 44 1;
	setAttr -s 5 ".kot[0:4]"  5 5 5 5 5;
createNode animCurveTU -n "keyboardMonster_Facial_CTRL_Tongue";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 11 1 22 1 33 1 44 1;
	setAttr -s 5 ".kot[0:4]"  5 5 5 5 5;
createNode animCurveTU -n "keyboardMonster_Facial_CTRL_Ear";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 11 1 22 1 33 1 44 1;
	setAttr -s 5 ".kot[0:4]"  5 5 5 5 5;
createNode animCurveTU -n "keyboardMonster_Facial_CTRL_Teeth";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 11 1 22 1 33 1 44 1;
	setAttr -s 5 ".kot[0:4]"  5 5 5 5 5;
createNode animCurveTA -n "keyboardMonster_L_Clavicle_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 2.3648857654688786 6 1.5036962764115349
		 11 -6.1855403988320834 17 -6.4197742384146794 22 -6.1861074495173121 26 -0.032090914721827915
		 33 0 38 8.6643203887872193 44 2.3648857654688786;
createNode animCurveTA -n "keyboardMonster_L_Clavicle_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 -1.995266817533379 6 11.061308219509284
		 11 13.402269298950916 17 19.270535484007063 22 9.1555963591834963 26 -0.84100266286446779
		 33 18.99845301827785 38 -1.1836147650918025 44 -1.995266817533379;
createNode animCurveTA -n "keyboardMonster_L_Clavicle_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 0.64940021684394866 6 -23.662781843853391
		 11 -44.379575163671156 17 1.0367369465684004 22 -5.6721684657894746 26 0.44426031590653803
		 33 0 38 1.020045164972551 44 0.64940021684394866;
createNode animCurveTA -n "keyboardMonster_R_Clavicle_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 -16.707688589390397 4 -17.406417100384544
		 11 -5.3836427507112212 16 1.6778244608799333 22 7.8228566904569092 28 7.7876014845296435
		 33 -0.028256201002700515 40 -13.378646183196402 44 -16.707688589390397;
createNode animCurveTA -n "keyboardMonster_R_Clavicle_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 2.913071823554342 4 1.9045332385000338
		 11 -14.657258603384834 16 -3.4518576997941142 22 9.0705754199855786 28 4.8126910781678749
		 33 -3.317004779685508 40 -3.0305457413287029 44 2.913071823554342;
createNode animCurveTA -n "keyboardMonster_R_Clavicle_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 5.8336274983903786 4 4.3044819440310862
		 11 10.418618122835998 16 8.5022159088021212 22 5.5540478975210528 28 7.0155715010887958
		 33 1.1300103499127943 40 5.9306249448681347 44 5.8336274983903786;
createNode animCurveTA -n "keyboardMonster_L_pectoral_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 0 11 -4.3830271865146431 22 2.1933827761554512
		 28 0.84507738915066366 33 -1.2043127681889301 44 0;
createNode animCurveTA -n "keyboardMonster_L_pectoral_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 -12.633300116220061 11 -14.823540143581329
		 22 -2.0238463790415939 28 -8.96241059994985 33 -8.7293849533757246 44 -12.633300116220061;
createNode animCurveTA -n "keyboardMonster_L_pectoral_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 0 11 0.071487136830882966 22 -0.011305559337888529
		 28 -0.1303414128641141 33 -0.50753321192306811 44 0;
createNode animCurveTA -n "keyboardMonster_R_pectoral_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 8 ".ktv[0:7]"  1 0 5 0.0024688973498039521 8 -0.66226770941535207
		 11 -1.1228517828661666 16 -0.03856253034692235 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_pectoral_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 8 ".ktv[0:7]"  1 0 5 -1.5892584567593568 8 1.2214756633491088
		 11 -1.4201327664517254 16 5.9402762851365489 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_pectoral_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 8 ".ktv[0:7]"  1 0 5 0.42030970706322096 8 0.72921678814567525
		 11 0.27637926370663934 16 -6.5681533486627828 22 0 33 0 44 0;
createNode animCurveTU -n "keyboardMonster_R_knee_CTRL_ToWorld";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 11 1 22 1 33 1 44 1;
createNode animCurveTU -n "keyboardMonster_L_knee_CTRL_ToWorld";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 11 1 22 1 33 1 44 1;
createNode animCurveTU -n "keyboardMonster_L_Foot_IKFK_SWITCH_CTRL_FK_IK";
	setAttr ".tan" 3;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 12 1 23 1 33 1 44 1;
	setAttr -s 5 ".kit[0:4]"  9 3 3 3 9;
	setAttr -s 5 ".kot[0:4]"  9 3 3 3 9;
createNode animCurveTU -n "keyboardMonster_R_Foot_IKFK_SWITCH_CTRL_FK_IK";
	setAttr ".tan" 3;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 11 1 22 1 36 1 44 1;
	setAttr -s 5 ".kit[2:4]"  9 3 3;
	setAttr -s 5 ".kot[2:4]"  9 3 3;
createNode animCurveTU -n "keyboardMonster_L_elbow_CTRL_ToWorld";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 11 1 22 1 33 1 44 1;
createNode animCurveTU -n "keyboardMonster_L_Wrist_IKFK_SWITCH_CTRL_FK_IK";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 8 1 22 1 33 1 44 1;
	setAttr -s 5 ".kit[2:4]"  3 3 9;
	setAttr -s 5 ".kot[2:4]"  3 3 9;
	setAttr ".pre" 3;
	setAttr ".pst" 3;
createNode animCurveTU -n "keyboardMonster_R_Wrist_IKFK_SWITCH_CTRL_FK_IK";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 11 1 22 1 30 1 44 1;
	setAttr -s 5 ".kit[1:4]"  3 3 9 9;
	setAttr -s 5 ".kot[1:4]"  3 3 9 9;
createNode animCurveTA -n "keyboardMonster_L_index01_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 -11.784296055817421 11 -11.784296055817421
		 22 -11.784296055817421 33 -11.784296055817421 44 -11.784296055817421;
createNode animCurveTA -n "keyboardMonster_L_index01_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 19.795358651207625 11 19.795358651207625
		 22 19.795358651207625 33 19.795358651207625 44 19.795358651207625;
createNode animCurveTA -n "keyboardMonster_L_index01_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 -6.0830658774984805 11 -6.0830658774984805
		 22 -6.0830658774984805 33 -6.0830658774984805 44 -6.0830658774984805;
createNode animCurveTA -n "keyboardMonster_L_index02_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_L_index02_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_L_index02_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_L_middle01_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 -15.062881361920734 11 -15.062881361920734
		 22 -15.062881361920734 33 -15.062881361920734 44 -15.062881361920734;
createNode animCurveTA -n "keyboardMonster_L_middle01_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 18.379592483594848 11 18.379592483594848
		 22 18.379592483594848 33 18.379592483594848 44 18.379592483594848;
createNode animCurveTA -n "keyboardMonster_L_middle01_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 3.8089154427228529 11 3.8089154427228529
		 22 3.8089154427228529 33 3.8089154427228529 44 3.8089154427228529;
createNode animCurveTA -n "keyboardMonster_L_middle02_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_L_middle02_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_L_middle02_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_L_pinky01_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 -8.7411809654318819 11 -8.7411809654318819
		 22 -8.7411809654318819 33 -8.7411809654318819 44 -8.7411809654318819;
createNode animCurveTA -n "keyboardMonster_L_pinky01_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 11.961180754712906 11 11.961180754712906
		 22 11.961180754712906 33 11.961180754712906 44 11.961180754712906;
createNode animCurveTA -n "keyboardMonster_L_pinky01_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 11.189935813111259 11 11.189935813111259
		 22 11.189935813111259 33 11.189935813111259 44 11.189935813111259;
createNode animCurveTA -n "keyboardMonster_L_pinky02_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_L_pinky02_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_L_pinky02_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_pinky01_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 7 ".ktv[0:6]"  1 -19.137273839203608 11 -19.137273839203608
		 22 -19.137273839203608 25 -2.5034208653315524 28 -5.8369257290649985 33 -19.137273839203608
		 44 -19.137273839203608;
createNode animCurveTA -n "keyboardMonster_R_pinky01_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 7 ".ktv[0:6]"  1 25.034596726446846 11 25.034596726446846
		 22 25.034596726446846 25 31.041067329798995 28 14.497172634027422 33 25.034596726446846
		 44 25.034596726446846;
createNode animCurveTA -n "keyboardMonster_R_pinky01_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 7 ".ktv[0:6]"  1 -2.9663727224068022 11 -2.9663727224068022
		 22 -2.9663727224068022 25 31.540159771589718 28 59.181410961771796 33 -2.9663727224068022
		 44 -2.9663727224068022;
createNode animCurveTA -n "keyboardMonster_R_pinky02_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_pinky02_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_pinky02_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_middle01_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 7 ".ktv[0:6]"  1 -14.424156285472328 11 -14.424156285472328
		 22 -14.424156285472328 25 -4.8730383584505903 28 -4.5516766501303119 33 -14.424156285472328
		 44 -14.424156285472328;
createNode animCurveTA -n "keyboardMonster_R_middle01_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 7 ".ktv[0:6]"  1 14.319006074157757 11 14.319006074157757
		 22 14.319006074157757 25 19.64469898468457 28 -10.086026372025067 33 14.319006074157757
		 44 14.319006074157757;
createNode animCurveTA -n "keyboardMonster_R_middle01_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 7 ".ktv[0:6]"  1 -7.0312708761945633 11 -7.0312708761945633
		 22 -7.0312708761945633 25 24.861095491695394 28 64.654772849091017 33 -7.0312708761945633
		 44 -7.0312708761945633;
createNode animCurveTA -n "keyboardMonster_R_middle02_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_middle02_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_middle02_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_index01_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 7 ".ktv[0:6]"  1 -25.197710974416569 11 -25.197710974416569
		 22 -25.197710974416569 25 -15.515602191618163 28 -1.9652665664156777 33 -25.197710974416569
		 44 -25.197710974416569;
createNode animCurveTA -n "keyboardMonster_R_index01_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 7 ".ktv[0:6]"  1 12.516625718085205 11 12.516625718085205
		 22 12.516625718085205 25 23.544915321600541 28 11.664396374475308 33 12.516625718085205
		 44 12.516625718085205;
createNode animCurveTA -n "keyboardMonster_R_index01_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 7 ".ktv[0:6]"  1 -27.156017552018465 11 -27.156017552018465
		 22 -27.156017552018465 25 3.3147292489506959 28 38.795265917671294 33 -27.156017552018465
		 44 -27.156017552018465;
createNode animCurveTA -n "keyboardMonster_R_index02_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_index02_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_index02_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 0 11 0 22 0 33 0 44 0;
createNode animCurveTA -n "keyboardMonster_L_twistForearm_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 69.35685658955471 11 69.35685658955471
		 22 69.35685658955471 33 69.35685658955471 44 69.35685658955471;
createNode animCurveTA -n "keyboardMonster_R_twistForearm_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 47.166126251570667 11 47.166126251570667
		 22 47.166126251570667 33 47.166126251570667 44 47.166126251570667;
createNode animCurveTU -n "keyboardMonster_R_elbow_CTRL_ToWorld";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 5 ".ktv[0:4]"  1 1 11 1 22 1 33 1 44 1;
createNode animCurveTU -n "keyboardMonster_R_Foot_CTRL_Heel_Roll";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 11 ".ktv[0:10]"  1 0 7 0 11 0 15 0 19 0 22 0 26 0.95183684713752692
		 32 1.4000000000000001 36 0 38 0 44 0;
	setAttr -s 11 ".kit[0:10]"  3 9 3 9 9 9 9 9 
		3 9 3;
	setAttr -s 11 ".kot[0:10]"  3 9 3 9 9 9 9 9 
		3 9 3;
createNode animCurveTU -n "keyboardMonster_R_Foot_CTRL_Ball_Roll";
	setAttr ".tan" 3;
	setAttr ".wgt" no;
	setAttr -s 11 ".ktv[0:10]"  1 0 5 0 11 2.9045557553052812 15 3.1165473598200268
		 19 1.2433510297202794 22 0 26 0 32 0 36 0 38 0 44 0;
	setAttr -s 11 ".kit[2:10]"  1 1 1 3 9 9 3 9 
		3;
	setAttr -s 11 ".kot[2:10]"  1 1 1 3 9 9 3 9 
		3;
	setAttr -s 11 ".kix[2:10]"  0.16874255239963531 0.086235277354717255 
		0.067084692418575287 1 1 1 1 1 1;
	setAttr -s 11 ".kiy[2:10]"  0.9856601357460022 -0.99627476930618286 
		-0.99774730205535889 0 0 0 0 0 0;
	setAttr -s 11 ".kox[2:10]"  0.16874255239963531 0.086235314607620239 
		0.067084677517414093 1 1 1 1 1 1;
	setAttr -s 11 ".koy[2:10]"  0.9856601357460022 -0.99627476930618286 
		-0.99774730205535889 0 0 0 0 0 0;
createNode animCurveTU -n "keyboardMonster_R_Foot_CTRL_ToeTip_Roll";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 11 ".ktv[0:10]"  1 0 7 0 11 0 15 3.7852636253224521 19 6.0865263166781212
		 22 4.8999999999999995 26 0 32 0 36 0 38 0 44 0;
	setAttr -s 11 ".kit[0:10]"  3 9 3 1 9 9 3 9 
		3 9 3;
	setAttr -s 11 ".kot[0:10]"  3 9 3 1 9 9 3 9 
		3 9 3;
	setAttr -s 11 ".kix[3:10]"  0.036635339260101318 0.20487697422504425 
		0.038307905197143555 1 1 1 1 1;
	setAttr -s 11 ".kiy[3:10]"  0.99932873249053955 0.97878772020339966 
		-0.99926596879959106 0 0 0 0 0;
	setAttr -s 11 ".kox[3:10]"  0.036635309457778931 0.20487697422504425 
		0.038307905197143555 1 1 1 1 1;
	setAttr -s 11 ".koy[3:10]"  0.99932873249053955 0.97878772020339966 
		-0.99926596879959106 0 0 0 0 0;
createNode animCurveTU -n "keyboardMonster_R_Foot_CTRL_Toe_Wiggle";
	setAttr ".tan" 3;
	setAttr ".wgt" no;
	setAttr -s 11 ".ktv[0:10]"  1 0 7 0 11 0 15 0 19 0 22 0 26 0 32 3.8000000000000003
		 36 1.9000000000000001 38 0 44 0;
	setAttr -s 11 ".kit[1:10]"  9 3 9 9 3 3 9 1 
		3 3;
	setAttr -s 11 ".kot[1:10]"  9 3 9 9 3 3 9 1 
		3 3;
	setAttr -s 11 ".kix[8:10]"  0.026693757623434067 1 1;
	setAttr -s 11 ".kiy[8:10]"  -0.99964374303817749 0 0;
	setAttr -s 11 ".kox[8:10]"  0.026693835854530334 1 1;
	setAttr -s 11 ".koy[8:10]"  -0.99964368343353271 0 0;
createNode animCurveTU -n "keyboardMonster_R_Foot_CTRL_Knee_Twist";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 11 ".ktv[0:10]"  1 0 7 0 11 0 15 0 19 0 22 0 26 0 32 0 36 0
		 38 0 44 0;
	setAttr -s 11 ".kit[0:10]"  3 9 3 9 9 9 9 9 
		3 9 3;
	setAttr -s 11 ".kot[0:10]"  3 9 3 9 9 9 9 9 
		3 9 3;
createNode animCurveTU -n "keyboardMonster_L_Foot_CTRL_Heel_Roll";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 0 3 0 12 0 14 0 17 0 19 0 23 0 33 0 44 0;
	setAttr -s 9 ".kit[2:8]"  3 9 3 9 3 3 9;
	setAttr -s 9 ".kot[2:8]"  3 9 3 9 3 3 9;
createNode animCurveTU -n "keyboardMonster_L_Foot_CTRL_Ball_Roll";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 10 ".ktv[0:9]"  1 1.8 3 1.3188440049363996 12 0 14 0 17 0
		 19 0 23 0 33 2.3000000000000003 38 3.7125001147320256 44 1.8;
	setAttr -s 10 ".kit[0:9]"  1 9 3 9 3 9 3 1 
		9 1;
	setAttr -s 10 ".kot[0:9]"  1 9 3 9 3 9 3 1 
		9 1;
	setAttr -s 10 ".kix[0:9]"  0.13633649051189423 0.19960448145866394 
		1 1 1 1 1 0.093297712504863739 0.59136366844177246 0.12565216422080994;
	setAttr -s 10 ".kiy[0:9]"  -0.99066257476806641 -0.97987651824951172 
		0 0 0 0 0 0.99563825130462646 -0.80640494823455811 -0.99207437038421631;
	setAttr -s 10 ".kox[0:9]"  0.13633646070957184 0.19960448145866394 
		1 1 1 1 1 0.093297652900218964 0.59136366844177246 0.12565216422080994;
	setAttr -s 10 ".koy[0:9]"  -0.99066257476806641 -0.97987651824951172 
		0 0 0 0 0 0.99563825130462646 -0.80640494823455811 -0.99207437038421631;
	setAttr ".pre" 3;
	setAttr ".pst" 3;
createNode animCurveTU -n "keyboardMonster_L_Foot_CTRL_ToeTip_Roll";
	setAttr ".tan" 3;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 5.3000000000000007 3 5.4124262742740266
		 12 0 14 0 17 0 19 0 23 0 33 0 44 5.3000000000000007;
	setAttr -s 9 ".kit[0:8]"  1 1 3 9 3 9 3 3 
		1;
	setAttr -s 9 ".kot[0:8]"  1 1 3 9 3 9 3 3 
		1;
	setAttr -s 9 ".kix[0:8]"  0.24133995175361633 0.18782016634941101 
		1 1 1 1 1 1 0.1456432044506073;
	setAttr -s 9 ".kiy[0:8]"  0.97044068574905396 -0.98220348358154297 
		0 0 0 0 0 0 0.98933720588684082;
	setAttr -s 9 ".kox[0:8]"  0.24133996665477753 0.18782016634941101 
		1 1 1 1 1 1 0.14564338326454163;
	setAttr -s 9 ".koy[0:8]"  0.97044062614440918 -0.98220348358154297 
		0 0 0 0 0 0 0.98933720588684082;
	setAttr ".pre" 3;
	setAttr ".pst" 3;
createNode animCurveTU -n "keyboardMonster_L_Foot_CTRL_Toe_Wiggle";
	setAttr ".tan" 3;
	setAttr ".wgt" no;
	setAttr -s 10 ".ktv[0:9]"  1 0 3 0 12 0 14 1.9035137725017457 15 2.6
		 17 1.4642750070646353 19 0 23 0 33 0 44 0;
	setAttr -s 10 ".kit[0:9]"  9 9 3 9 1 1 3 3 
		3 9;
	setAttr -s 10 ".kot[0:9]"  9 9 3 9 1 1 3 3 
		3 9;
	setAttr -s 10 ".kix[4:9]"  0.5514330267906189 0.031083550304174423 
		1 1 1 1;
	setAttr -s 10 ".kiy[4:9]"  0.83421915769577026 -0.99951678514480591 
		0 0 0 0;
	setAttr -s 10 ".kox[4:9]"  0.5514330267906189 0.031083518639206886 
		1 1 1 1;
	setAttr -s 10 ".koy[4:9]"  0.83421915769577026 -0.99951678514480591 
		0 0 0 0;
createNode animCurveTU -n "keyboardMonster_L_Foot_CTRL_Knee_Twist";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 9 ".ktv[0:8]"  1 0 3 0 12 0 14 0 17 0 19 0 23 0 33 0 44 0;
	setAttr -s 9 ".kit[2:8]"  3 9 3 9 3 3 9;
	setAttr -s 9 ".kot[2:8]"  3 9 3 9 3 3 9;
createNode animCurveTU -n "keyboardMonster_R_Wrist_IK_CTRL_ToSpine";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 0 11 0 22 0 26 0 30 0 44 0;
	setAttr -s 6 ".kit[1:5]"  3 3 9 9 9;
	setAttr -s 6 ".kot[1:5]"  3 3 9 9 9;
createNode animCurveTU -n "keyboardMonster_L_Wrist_IK_CTRL_ToSpine";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 6 ".ktv[0:5]"  1 0 8 0 22 0 24 0 33 0 44 0;
	setAttr -s 6 ".kit[2:5]"  3 9 3 9;
	setAttr -s 6 ".kot[2:5]"  3 9 3 9;
	setAttr ".pre" 3;
	setAttr ".pst" 3;
createNode hyperGraphInfo -n "nodeEditorPanel1Info";
createNode hyperView -n "hyperView1";
	setAttr ".dag" no;
createNode hyperLayout -n "hyperLayout1";
	setAttr ".ihi" 0;
	setAttr -s 185 ".hyp";
	setAttr ".hyp[0].x" 1.4285714626312256;
	setAttr ".hyp[0].y" -907.14288330078125;
	setAttr ".hyp[0].nvs" 1920;
	setAttr ".hyp[1].x" 212.85714721679687;
	setAttr ".hyp[1].y" -907.14288330078125;
	setAttr ".hyp[1].nvs" 1920;
	setAttr ".hyp[2].x" 424.28570556640625;
	setAttr ".hyp[2].y" -907.14288330078125;
	setAttr ".hyp[2].nvs" 1920;
	setAttr ".hyp[3].x" 1.4285714626312256;
	setAttr ".hyp[3].y" -802.85711669921875;
	setAttr ".hyp[3].nvs" 1920;
	setAttr ".hyp[4].x" 212.85714721679687;
	setAttr ".hyp[4].y" -802.85711669921875;
	setAttr ".hyp[4].nvs" 1920;
	setAttr ".hyp[5].x" 424.28570556640625;
	setAttr ".hyp[5].y" -802.85711669921875;
	setAttr ".hyp[5].nvs" 1920;
	setAttr ".hyp[6].nvs" 1920;
	setAttr ".hyp[7].nvs" 1920;
	setAttr ".hyp[8].nvs" 1920;
	setAttr ".hyp[9].nvs" 1920;
	setAttr ".hyp[10].nvs" 1920;
	setAttr ".hyp[11].nvs" 1920;
	setAttr ".hyp[12].x" 1.4285714626312256;
	setAttr ".hyp[12].y" -698.5714111328125;
	setAttr ".hyp[12].nvs" 1920;
	setAttr ".hyp[13].x" 212.85714721679687;
	setAttr ".hyp[13].y" -698.5714111328125;
	setAttr ".hyp[13].nvs" 1920;
	setAttr ".hyp[14].x" 424.28570556640625;
	setAttr ".hyp[14].y" -698.5714111328125;
	setAttr ".hyp[14].nvs" 1920;
	setAttr ".hyp[15].x" 635.71429443359375;
	setAttr ".hyp[15].y" -698.5714111328125;
	setAttr ".hyp[15].nvs" 1920;
	setAttr ".hyp[16].x" 847.14288330078125;
	setAttr ".hyp[16].y" -698.5714111328125;
	setAttr ".hyp[16].nvs" 1920;
	setAttr ".hyp[17].x" 1058.5714111328125;
	setAttr ".hyp[17].y" -698.5714111328125;
	setAttr ".hyp[17].nvs" 1920;
	setAttr ".hyp[18].x" 1270;
	setAttr ".hyp[18].y" -907.14288330078125;
	setAttr ".hyp[18].nvs" 1920;
	setAttr ".hyp[19].x" 1270;
	setAttr ".hyp[19].y" -802.85711669921875;
	setAttr ".hyp[19].nvs" 1920;
	setAttr ".hyp[20].x" 1270;
	setAttr ".hyp[20].y" -698.5714111328125;
	setAttr ".hyp[20].nvs" 1920;
	setAttr ".hyp[21].x" 1481.4285888671875;
	setAttr ".hyp[21].y" -907.14288330078125;
	setAttr ".hyp[21].nvs" 1920;
	setAttr ".hyp[22].x" 1481.4285888671875;
	setAttr ".hyp[22].y" -802.85711669921875;
	setAttr ".hyp[22].nvs" 1920;
	setAttr ".hyp[23].x" 1481.4285888671875;
	setAttr ".hyp[23].y" -698.5714111328125;
	setAttr ".hyp[23].nvs" 1920;
	setAttr ".hyp[24].nvs" 1920;
	setAttr ".hyp[25].nvs" 1920;
	setAttr ".hyp[26].nvs" 1920;
	setAttr ".hyp[27].nvs" 1920;
	setAttr ".hyp[28].nvs" 1920;
	setAttr ".hyp[29].nvs" 1920;
	setAttr ".hyp[30].nvs" 1920;
	setAttr ".hyp[31].nvs" 1920;
	setAttr ".hyp[32].nvs" 1920;
	setAttr ".hyp[33].x" 1270;
	setAttr ".hyp[33].y" -594.28570556640625;
	setAttr ".hyp[33].nvs" 1920;
	setAttr ".hyp[34].x" 1481.4285888671875;
	setAttr ".hyp[34].y" -594.28570556640625;
	setAttr ".hyp[34].nvs" 1920;
	setAttr ".hyp[35].x" 1692.857177734375;
	setAttr ".hyp[35].y" -594.28570556640625;
	setAttr ".hyp[35].nvs" 1920;
	setAttr ".hyp[36].x" 1904.2857666015625;
	setAttr ".hyp[36].y" -907.14288330078125;
	setAttr ".hyp[36].nvs" 1920;
	setAttr ".hyp[37].x" 1904.2857666015625;
	setAttr ".hyp[37].y" -802.85711669921875;
	setAttr ".hyp[37].nvs" 1920;
	setAttr ".hyp[38].x" 1904.2857666015625;
	setAttr ".hyp[38].y" -698.5714111328125;
	setAttr ".hyp[38].nvs" 1920;
	setAttr ".hyp[39].x" 1904.2857666015625;
	setAttr ".hyp[39].y" -594.28570556640625;
	setAttr ".hyp[39].nvs" 1920;
	setAttr ".hyp[40].x" 2115.71435546875;
	setAttr ".hyp[40].y" -907.14288330078125;
	setAttr ".hyp[40].nvs" 1920;
	setAttr ".hyp[41].x" 2115.71435546875;
	setAttr ".hyp[41].y" -802.85711669921875;
	setAttr ".hyp[41].nvs" 1920;
	setAttr ".hyp[42].x" 2115.71435546875;
	setAttr ".hyp[42].y" -698.5714111328125;
	setAttr ".hyp[42].nvs" 1920;
	setAttr ".hyp[43].x" 2115.71435546875;
	setAttr ".hyp[43].y" -594.28570556640625;
	setAttr ".hyp[43].nvs" 1920;
	setAttr ".hyp[44].x" 2327.142822265625;
	setAttr ".hyp[44].y" -907.14288330078125;
	setAttr ".hyp[44].nvs" 1920;
	setAttr ".hyp[45].x" 1.4285714626312256;
	setAttr ".hyp[45].y" -490;
	setAttr ".hyp[45].nvs" 1920;
	setAttr ".hyp[46].x" 212.85714721679687;
	setAttr ".hyp[46].y" -490;
	setAttr ".hyp[46].nvs" 1920;
	setAttr ".hyp[47].x" 424.28570556640625;
	setAttr ".hyp[47].y" -490;
	setAttr ".hyp[47].nvs" 1920;
	setAttr ".hyp[48].x" 635.71429443359375;
	setAttr ".hyp[48].y" -490;
	setAttr ".hyp[48].nvs" 1920;
	setAttr ".hyp[49].x" 847.14288330078125;
	setAttr ".hyp[49].y" -490;
	setAttr ".hyp[49].nvs" 1920;
	setAttr ".hyp[50].x" 1058.5714111328125;
	setAttr ".hyp[50].y" -490;
	setAttr ".hyp[50].nvs" 1920;
	setAttr ".hyp[51].x" 1270;
	setAttr ".hyp[51].y" -490;
	setAttr ".hyp[51].nvs" 1920;
	setAttr ".hyp[52].x" 1481.4285888671875;
	setAttr ".hyp[52].y" -490;
	setAttr ".hyp[52].nvs" 1920;
	setAttr ".hyp[53].x" 1692.857177734375;
	setAttr ".hyp[53].y" -490;
	setAttr ".hyp[53].nvs" 1920;
	setAttr ".hyp[54].x" 1904.2857666015625;
	setAttr ".hyp[54].y" -490;
	setAttr ".hyp[54].nvs" 1920;
	setAttr ".hyp[55].x" 2115.71435546875;
	setAttr ".hyp[55].y" -490;
	setAttr ".hyp[55].nvs" 1920;
	setAttr ".hyp[56].x" 2327.142822265625;
	setAttr ".hyp[56].y" -802.85711669921875;
	setAttr ".hyp[56].nvs" 1920;
	setAttr ".hyp[57].x" 2327.142822265625;
	setAttr ".hyp[57].y" -698.5714111328125;
	setAttr ".hyp[57].nvs" 1920;
	setAttr ".hyp[58].x" 2327.142822265625;
	setAttr ".hyp[58].y" -594.28570556640625;
	setAttr ".hyp[58].nvs" 1920;
	setAttr ".hyp[59].x" 2327.142822265625;
	setAttr ".hyp[59].y" -490;
	setAttr ".hyp[59].nvs" 1920;
	setAttr ".hyp[60].x" 2538.571533203125;
	setAttr ".hyp[60].y" -907.14288330078125;
	setAttr ".hyp[60].nvs" 1920;
	setAttr ".hyp[61].x" 2538.571533203125;
	setAttr ".hyp[61].y" -802.85711669921875;
	setAttr ".hyp[61].nvs" 1920;
	setAttr ".hyp[62].x" 2538.571533203125;
	setAttr ".hyp[62].y" -698.5714111328125;
	setAttr ".hyp[62].nvs" 1920;
	setAttr ".hyp[63].x" 2538.571533203125;
	setAttr ".hyp[63].y" -594.28570556640625;
	setAttr ".hyp[63].nvs" 1920;
	setAttr ".hyp[64].x" 2538.571533203125;
	setAttr ".hyp[64].y" -490;
	setAttr ".hyp[64].nvs" 1920;
	setAttr ".hyp[65].x" 2750;
	setAttr ".hyp[65].y" -907.14288330078125;
	setAttr ".hyp[65].nvs" 1920;
	setAttr ".hyp[66].x" 2750;
	setAttr ".hyp[66].y" -802.85711669921875;
	setAttr ".hyp[66].nvs" 1920;
	setAttr ".hyp[67].x" 2750;
	setAttr ".hyp[67].y" -698.5714111328125;
	setAttr ".hyp[67].nvs" 1920;
	setAttr ".hyp[68].x" 2750;
	setAttr ".hyp[68].y" -594.28570556640625;
	setAttr ".hyp[68].nvs" 1920;
	setAttr ".hyp[69].nvs" 1920;
	setAttr ".hyp[70].nvs" 1920;
	setAttr ".hyp[71].nvs" 1920;
	setAttr ".hyp[72].x" 424.28570556640625;
	setAttr ".hyp[72].y" -385.71429443359375;
	setAttr ".hyp[72].nvs" 1920;
	setAttr ".hyp[73].x" 635.71429443359375;
	setAttr ".hyp[73].y" -385.71429443359375;
	setAttr ".hyp[73].nvs" 1920;
	setAttr ".hyp[74].x" 847.14288330078125;
	setAttr ".hyp[74].y" -385.71429443359375;
	setAttr ".hyp[74].nvs" 1920;
	setAttr ".hyp[75].x" 1058.5714111328125;
	setAttr ".hyp[75].y" -385.71429443359375;
	setAttr ".hyp[75].nvs" 1920;
	setAttr ".hyp[76].x" 1270;
	setAttr ".hyp[76].y" -385.71429443359375;
	setAttr ".hyp[76].nvs" 1920;
	setAttr ".hyp[77].x" 1481.4285888671875;
	setAttr ".hyp[77].y" -385.71429443359375;
	setAttr ".hyp[77].nvs" 1920;
	setAttr ".hyp[78].x" 1692.857177734375;
	setAttr ".hyp[78].y" -385.71429443359375;
	setAttr ".hyp[78].nvs" 1920;
	setAttr ".hyp[79].x" 1904.2857666015625;
	setAttr ".hyp[79].y" -385.71429443359375;
	setAttr ".hyp[79].nvs" 1920;
	setAttr ".hyp[80].x" 2115.71435546875;
	setAttr ".hyp[80].y" -385.71429443359375;
	setAttr ".hyp[80].nvs" 1920;
	setAttr ".hyp[81].x" 2327.142822265625;
	setAttr ".hyp[81].y" -385.71429443359375;
	setAttr ".hyp[81].nvs" 1920;
	setAttr ".hyp[82].x" 2538.571533203125;
	setAttr ".hyp[82].y" -385.71429443359375;
	setAttr ".hyp[82].nvs" 1920;
	setAttr ".hyp[83].x" 2750;
	setAttr ".hyp[83].y" -385.71429443359375;
	setAttr ".hyp[83].nvs" 1920;
	setAttr ".hyp[84].x" 2961.428466796875;
	setAttr ".hyp[84].y" -907.14288330078125;
	setAttr ".hyp[84].nvs" 1920;
	setAttr ".hyp[85].x" 2961.428466796875;
	setAttr ".hyp[85].y" -802.85711669921875;
	setAttr ".hyp[85].nvs" 1920;
	setAttr ".hyp[86].x" 2961.428466796875;
	setAttr ".hyp[86].y" -698.5714111328125;
	setAttr ".hyp[86].nvs" 1920;
	setAttr ".hyp[87].x" 2961.428466796875;
	setAttr ".hyp[87].y" -594.28570556640625;
	setAttr ".hyp[87].nvs" 1920;
	setAttr ".hyp[88].x" 2961.428466796875;
	setAttr ".hyp[88].y" -490;
	setAttr ".hyp[88].nvs" 1920;
	setAttr ".hyp[89].x" 2961.428466796875;
	setAttr ".hyp[89].y" -385.71429443359375;
	setAttr ".hyp[89].nvs" 1920;
	setAttr ".hyp[90].x" 3172.857177734375;
	setAttr ".hyp[90].y" -907.14288330078125;
	setAttr ".hyp[90].nvs" 1920;
	setAttr ".hyp[91].x" 3172.857177734375;
	setAttr ".hyp[91].y" -802.85711669921875;
	setAttr ".hyp[91].nvs" 1920;
	setAttr ".hyp[92].x" 3172.857177734375;
	setAttr ".hyp[92].y" -698.5714111328125;
	setAttr ".hyp[92].nvs" 1920;
	setAttr ".hyp[93].x" 3172.857177734375;
	setAttr ".hyp[93].y" -594.28570556640625;
	setAttr ".hyp[93].nvs" 1920;
	setAttr ".hyp[94].x" 3172.857177734375;
	setAttr ".hyp[94].y" -490;
	setAttr ".hyp[94].nvs" 1920;
	setAttr ".hyp[95].x" 3172.857177734375;
	setAttr ".hyp[95].y" -385.71429443359375;
	setAttr ".hyp[95].nvs" 1920;
	setAttr ".hyp[96].x" 3384.28564453125;
	setAttr ".hyp[96].y" -907.14288330078125;
	setAttr ".hyp[96].nvs" 1920;
	setAttr ".hyp[97].x" 3384.28564453125;
	setAttr ".hyp[97].y" -802.85711669921875;
	setAttr ".hyp[97].nvs" 1920;
	setAttr ".hyp[98].x" 3384.28564453125;
	setAttr ".hyp[98].y" -698.5714111328125;
	setAttr ".hyp[98].nvs" 1920;
	setAttr ".hyp[99].x" 3384.28564453125;
	setAttr ".hyp[99].y" -594.28570556640625;
	setAttr ".hyp[99].nvs" 1920;
	setAttr ".hyp[100].x" 3384.28564453125;
	setAttr ".hyp[100].y" -490;
	setAttr ".hyp[100].nvs" 1920;
	setAttr ".hyp[101].x" 3384.28564453125;
	setAttr ".hyp[101].y" -385.71429443359375;
	setAttr ".hyp[101].nvs" 1920;
	setAttr ".hyp[102].x" 1.4285714626312256;
	setAttr ".hyp[102].y" -281.42855834960937;
	setAttr ".hyp[102].nvs" 1920;
	setAttr ".hyp[103].x" 212.85714721679687;
	setAttr ".hyp[103].y" -281.42855834960937;
	setAttr ".hyp[103].nvs" 1920;
	setAttr ".hyp[104].x" 424.28570556640625;
	setAttr ".hyp[104].y" -281.42855834960937;
	setAttr ".hyp[104].nvs" 1920;
	setAttr ".hyp[105].x" 635.71429443359375;
	setAttr ".hyp[105].y" -281.42855834960937;
	setAttr ".hyp[105].nvs" 1920;
	setAttr ".hyp[106].x" 847.14288330078125;
	setAttr ".hyp[106].y" -281.42855834960937;
	setAttr ".hyp[106].nvs" 1920;
	setAttr ".hyp[107].x" 1058.5714111328125;
	setAttr ".hyp[107].y" -281.42855834960937;
	setAttr ".hyp[107].nvs" 1920;
	setAttr ".hyp[108].x" 1270;
	setAttr ".hyp[108].y" -281.42855834960937;
	setAttr ".hyp[108].nvs" 1920;
	setAttr ".hyp[109].x" 1481.4285888671875;
	setAttr ".hyp[109].y" -281.42855834960937;
	setAttr ".hyp[109].nvs" 1920;
	setAttr ".hyp[110].x" 1692.857177734375;
	setAttr ".hyp[110].y" -281.42855834960937;
	setAttr ".hyp[110].nvs" 1920;
	setAttr ".hyp[111].x" 1904.2857666015625;
	setAttr ".hyp[111].y" -281.42855834960937;
	setAttr ".hyp[111].nvs" 1920;
	setAttr ".hyp[112].x" 2115.71435546875;
	setAttr ".hyp[112].y" -281.42855834960937;
	setAttr ".hyp[112].nvs" 1920;
	setAttr ".hyp[113].x" 2327.142822265625;
	setAttr ".hyp[113].y" -281.42855834960937;
	setAttr ".hyp[113].nvs" 1920;
	setAttr ".hyp[114].x" 2538.571533203125;
	setAttr ".hyp[114].y" -281.42855834960937;
	setAttr ".hyp[114].nvs" 1920;
	setAttr ".hyp[115].x" 2750;
	setAttr ".hyp[115].y" -281.42855834960937;
	setAttr ".hyp[115].nvs" 1920;
	setAttr ".hyp[116].x" 2961.428466796875;
	setAttr ".hyp[116].y" -281.42855834960937;
	setAttr ".hyp[116].nvs" 1920;
	setAttr ".hyp[117].x" 3172.857177734375;
	setAttr ".hyp[117].y" -281.42855834960937;
	setAttr ".hyp[117].nvs" 1920;
	setAttr ".hyp[118].x" 3384.28564453125;
	setAttr ".hyp[118].y" -281.42855834960937;
	setAttr ".hyp[118].nvs" 1920;
	setAttr ".hyp[119].x" 3595.71435546875;
	setAttr ".hyp[119].y" -907.14288330078125;
	setAttr ".hyp[119].nvs" 1920;
	setAttr ".hyp[120].x" 3595.71435546875;
	setAttr ".hyp[120].y" -802.85711669921875;
	setAttr ".hyp[120].nvs" 1920;
	setAttr ".hyp[121].x" 3595.71435546875;
	setAttr ".hyp[121].y" -698.5714111328125;
	setAttr ".hyp[121].nvs" 1920;
	setAttr ".hyp[122].x" 3595.71435546875;
	setAttr ".hyp[122].y" -594.28570556640625;
	setAttr ".hyp[122].nvs" 1920;
	setAttr ".hyp[123].x" 3595.71435546875;
	setAttr ".hyp[123].y" -490;
	setAttr ".hyp[123].nvs" 1920;
	setAttr ".hyp[124].x" 3595.71435546875;
	setAttr ".hyp[124].y" -385.71429443359375;
	setAttr ".hyp[124].nvs" 1920;
	setAttr ".hyp[125].x" 3595.71435546875;
	setAttr ".hyp[125].y" -281.42855834960937;
	setAttr ".hyp[125].nvs" 1920;
	setAttr ".hyp[126].x" 3807.142822265625;
	setAttr ".hyp[126].y" -907.14288330078125;
	setAttr ".hyp[126].nvs" 1920;
	setAttr ".hyp[127].x" 3807.142822265625;
	setAttr ".hyp[127].y" -802.85711669921875;
	setAttr ".hyp[127].nvs" 1920;
	setAttr ".hyp[128].x" 3807.142822265625;
	setAttr ".hyp[128].y" -698.5714111328125;
	setAttr ".hyp[128].nvs" 1920;
	setAttr ".hyp[129].x" 3807.142822265625;
	setAttr ".hyp[129].y" -594.28570556640625;
	setAttr ".hyp[129].nvs" 1920;
	setAttr ".hyp[130].x" 3807.142822265625;
	setAttr ".hyp[130].y" -490;
	setAttr ".hyp[130].nvs" 1920;
	setAttr ".hyp[131].x" 3807.142822265625;
	setAttr ".hyp[131].y" -385.71429443359375;
	setAttr ".hyp[131].nvs" 1920;
	setAttr ".hyp[132].x" 3807.142822265625;
	setAttr ".hyp[132].y" -281.42855834960937;
	setAttr ".hyp[132].nvs" 1920;
	setAttr ".hyp[133].x" 4018.571533203125;
	setAttr ".hyp[133].y" -907.14288330078125;
	setAttr ".hyp[133].nvs" 1920;
	setAttr ".hyp[134].x" 1.4285714626312256;
	setAttr ".hyp[134].y" -177.14285278320312;
	setAttr ".hyp[134].nvs" 1920;
	setAttr ".hyp[135].x" 212.85714721679687;
	setAttr ".hyp[135].y" -177.14285278320312;
	setAttr ".hyp[135].nvs" 1920;
	setAttr ".hyp[136].x" 424.28570556640625;
	setAttr ".hyp[136].y" -177.14285278320312;
	setAttr ".hyp[136].nvs" 1920;
	setAttr ".hyp[137].x" 635.71429443359375;
	setAttr ".hyp[137].y" -177.14285278320312;
	setAttr ".hyp[137].nvs" 1920;
	setAttr ".hyp[138].x" 847.14288330078125;
	setAttr ".hyp[138].y" -177.14285278320312;
	setAttr ".hyp[138].nvs" 1920;
	setAttr ".hyp[139].x" 1058.5714111328125;
	setAttr ".hyp[139].y" -177.14285278320312;
	setAttr ".hyp[139].nvs" 1920;
	setAttr ".hyp[140].x" 1270;
	setAttr ".hyp[140].y" -177.14285278320312;
	setAttr ".hyp[140].nvs" 1920;
	setAttr ".hyp[141].x" 1481.4285888671875;
	setAttr ".hyp[141].y" -177.14285278320312;
	setAttr ".hyp[141].nvs" 1920;
	setAttr ".hyp[142].x" 1692.857177734375;
	setAttr ".hyp[142].y" -177.14285278320312;
	setAttr ".hyp[142].nvs" 1920;
	setAttr ".hyp[143].x" 1904.2857666015625;
	setAttr ".hyp[143].y" -177.14285278320312;
	setAttr ".hyp[143].nvs" 1920;
	setAttr ".hyp[144].x" 2115.71435546875;
	setAttr ".hyp[144].y" -177.14285278320312;
	setAttr ".hyp[144].nvs" 1920;
	setAttr ".hyp[145].x" 2327.142822265625;
	setAttr ".hyp[145].y" -177.14285278320312;
	setAttr ".hyp[145].nvs" 1920;
	setAttr ".hyp[146].x" 2538.571533203125;
	setAttr ".hyp[146].y" -177.14285278320312;
	setAttr ".hyp[146].nvs" 1920;
	setAttr ".hyp[147].x" 2750;
	setAttr ".hyp[147].y" -177.14285278320312;
	setAttr ".hyp[147].nvs" 1920;
	setAttr ".hyp[148].x" 2961.428466796875;
	setAttr ".hyp[148].y" -177.14285278320312;
	setAttr ".hyp[148].nvs" 1920;
	setAttr ".hyp[149].x" 3172.857177734375;
	setAttr ".hyp[149].y" -177.14285278320312;
	setAttr ".hyp[149].nvs" 1920;
	setAttr ".hyp[150].x" 3384.28564453125;
	setAttr ".hyp[150].y" -177.14285278320312;
	setAttr ".hyp[150].nvs" 1920;
	setAttr ".hyp[151].x" 3595.71435546875;
	setAttr ".hyp[151].y" -177.14285278320312;
	setAttr ".hyp[151].nvs" 1920;
	setAttr ".hyp[152].x" 3807.142822265625;
	setAttr ".hyp[152].y" -177.14285278320312;
	setAttr ".hyp[152].nvs" 1920;
	setAttr ".hyp[153].x" 4018.571533203125;
	setAttr ".hyp[153].y" -802.85711669921875;
	setAttr ".hyp[153].nvs" 1920;
	setAttr ".hyp[154].x" 4018.571533203125;
	setAttr ".hyp[154].y" -698.5714111328125;
	setAttr ".hyp[154].nvs" 1920;
	setAttr ".hyp[155].x" 4018.571533203125;
	setAttr ".hyp[155].y" -594.28570556640625;
	setAttr ".hyp[155].nvs" 1920;
	setAttr ".hyp[156].x" 4018.571533203125;
	setAttr ".hyp[156].y" -490;
	setAttr ".hyp[156].nvs" 1920;
	setAttr ".hyp[157].x" 4018.571533203125;
	setAttr ".hyp[157].y" -385.71429443359375;
	setAttr ".hyp[157].nvs" 1920;
	setAttr ".hyp[158].x" 4018.571533203125;
	setAttr ".hyp[158].y" -281.42855834960937;
	setAttr ".hyp[158].nvs" 1920;
	setAttr ".hyp[159].x" 4018.571533203125;
	setAttr ".hyp[159].y" -177.14285278320312;
	setAttr ".hyp[159].nvs" 1920;
	setAttr ".hyp[160].x" 4230;
	setAttr ".hyp[160].y" -907.14288330078125;
	setAttr ".hyp[160].nvs" 1920;
	setAttr ".hyp[161].nvs" 1920;
	setAttr ".hyp[162].nvs" 1920;
	setAttr ".hyp[163].nvs" 1920;
	setAttr ".hyp[164].x" 4230;
	setAttr ".hyp[164].y" -490;
	setAttr ".hyp[164].nvs" 1920;
	setAttr ".hyp[165].x" 4230;
	setAttr ".hyp[165].y" -385.71429443359375;
	setAttr ".hyp[165].nvs" 1920;
	setAttr ".hyp[166].x" 4230;
	setAttr ".hyp[166].y" -281.42855834960937;
	setAttr ".hyp[166].nvs" 1920;
	setAttr ".hyp[167].x" 4230;
	setAttr ".hyp[167].y" -177.14285278320312;
	setAttr ".hyp[167].nvs" 1920;
	setAttr ".hyp[168].x" 4441.4287109375;
	setAttr ".hyp[168].y" -907.14288330078125;
	setAttr ".hyp[168].nvs" 1920;
	setAttr ".hyp[169].nvs" 1920;
	setAttr ".hyp[170].nvs" 1920;
	setAttr ".hyp[171].nvs" 1920;
	setAttr ".hyp[172].x" 4441.4287109375;
	setAttr ".hyp[172].y" -490;
	setAttr ".hyp[172].nvs" 1920;
	setAttr ".hyp[173].x" 4441.4287109375;
	setAttr ".hyp[173].y" -385.71429443359375;
	setAttr ".hyp[173].nvs" 1920;
	setAttr ".hyp[174].x" 4441.4287109375;
	setAttr ".hyp[174].y" -281.42855834960937;
	setAttr ".hyp[174].nvs" 1920;
	setAttr ".hyp[175].x" 4441.4287109375;
	setAttr ".hyp[175].y" -177.14285278320312;
	setAttr ".hyp[175].nvs" 1920;
	setAttr ".hyp[176].x" 1.4285714626312256;
	setAttr ".hyp[176].y" -72.857139587402344;
	setAttr ".hyp[176].nvs" 1920;
	setAttr ".hyp[177].nvs" 1920;
	setAttr ".hyp[178].nvs" 1920;
	setAttr ".hyp[179].nvs" 1920;
	setAttr ".hyp[180].x" 847.14288330078125;
	setAttr ".hyp[180].y" -72.857139587402344;
	setAttr ".hyp[180].nvs" 1920;
	setAttr ".hyp[181].nvs" 1920;
	setAttr ".hyp[182].nvs" 1920;
	setAttr ".hyp[183].nvs" 1920;
	setAttr ".hyp[184].x" 1692.857177734375;
	setAttr ".hyp[184].y" -72.857139587402344;
	setAttr ".hyp[184].nvs" 1920;
	setAttr ".anf" yes;
createNode animLayer -n "BaseAnimation";
	setAttr ".ovrd" yes;
createNode animCurveTL -n "keyboardMonster_root_JNT_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 0 2 0 3 0 4 0 5 0 6 0 7 0 8 0 9 0 10 0
		 11 0 12 0 13 0 14 0 15 0 16 0 17 0 18 0 19 0 20 0 21 0 22 0 23 0 24 0 25 0 26 0 27 0
		 28 0 29 0 30 0 31 0 32 0 33 0 34 0 35 0 36 0 37 0 38 0 39 0 40 0 41 0 42 0 43 0 44 0;
createNode animCurveTL -n "keyboardMonster_root_JNT_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -17.567613987177481 2 -17.652911197321856
		 3 -17.884449890021529 4 -18.225700805770231 5 -18.640134439891348 6 -19.091221512438501
		 7 -19.542432678090243 8 -19.957238763377319 9 -20.299110038504651 10 -20.531517354442343
		 11 -20.617931365868955 12 -20.548519653454477 13 -20.356021072042324 14 -20.06776318757116
		 15 -19.711073565979682 16 -19.313279407320422 17 -18.901709375190478 18 -18.503689596214997
		 19 -18.146549027184136 20 -17.857613989378152 21 -17.664213049470092 22 -17.59367311431933
		 23 -17.664208143776705 24 -17.857597207706121 25 -18.146514878075024 26 -18.503637386567192
		 27 -18.901638719594139 28 -19.313195498953597 29 -19.710981516385104 30 -20.067673392916376
		 31 -20.355945116498972 32 -20.548473158883581 33 -20.617931365868955 34 -20.545908363952854
		 35 -20.350503145417719 36 -20.05915669649827 37 -19.699313349854151 38 -19.298414385758136
		 39 -18.883901971218954 40 -18.483216897729008 41 -18.123804279996669 42 -17.833104810988303
		 43 -17.638560657712659 44 -17.567613987177481;
createNode animCurveTL -n "keyboardMonster_root_JNT_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -1.4210854715202004e-014 2 -6.6442873425387461e-009
		 3 -2.4678740828676382e-008 4 -5.1255836908126184e-008 5 -8.3528015615286222e-008
		 6 -1.1864774496217478e-007 7 -1.5376747075634967e-007 8 -1.8603966456254284e-007
		 9 -2.1261675176020844e-007 10 -2.3065120746679213e-007 11 -2.3729546771988907e-007
		 12 -2.3285749239931874e-007 13 -2.2068887961523842e-007 14 -2.0250751919803633e-007
		 15 -1.800313542688059e-007 16 -1.5497828531607638e-007 17 -1.2906629809350534e-007
		 18 -1.0401322203534846e-007 19 -8.15370784224001e-008 20 -6.3355703794343299e-008
		 21 -5.1187091010262975e-008 22 -4.6749119242406323e-008 23 -3.920039848281931e-008
		 24 -1.8502224641281373e-008 25 1.2423217121693142e-008 26 5.0653969019265332e-008
		 27 9.3267774836647277e-008 28 1.3734271231413686e-007 29 1.7995651813151881e-007
		 30 2.1818725581823628e-007 31 2.4911273310834758e-007 32 2.6981092826616759e-007
		 33 2.7735966323660932e-007 34 2.7738322483372713e-007 35 2.7744782737926243e-007
		 36 2.7754434750448809e-007 37 2.7766366184067692e-007 38 2.7779667544081121e-007
		 39 2.7793422230359965e-007 40 2.7806723590373394e-007 41 2.7818655023992278e-007
		 42 2.7828307036514843e-007 43 2.7834767291068374e-007 44 2.7837123450780155e-007;
createNode animCurveTA -n "keyboardMonster_root_JNT_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 95.666000000000039 2 95.606898978697146
		 3 95.446481944127655 4 95.210077838393346 5 94.92301577344054 6 94.610624705525026
		 7 94.298233636194041 8 94.011171448104221 9 93.774767408892245 10 93.614350384230264
		 11 93.555249375665767 12 93.604410351669301 13 93.739206577293004 14 93.94060799895604
		 15 94.189584563077844 16 94.467106471290691 17 94.754142904375044 18 95.03166481258792
		 19 95.280641172539362 20 95.482042798372731 21 95.616838938925483 22 95.666 23 95.616839109067357
		 24 95.482042798372731 25 95.280641580880044 26 95.031664812587877 27 94.754143414801007
		 28 94.467106471290677 29 94.189585039475418 30 93.940608203126416 31 93.739206883548604
		 32 93.604410266598379 33 93.555249375665767 34 93.604410266598393 35 93.73920627103756
		 36 93.940608203126445 37 94.189584563077801 38 94.467105960864771 39 94.754142393949095
		 40 95.03166481258792 41 95.280641172539333 42 95.482042492117131 43 95.616838768783211
		 44 95.666000000000039;
createNode animCurveTA -n "keyboardMonster_root_JNT_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -0.13661442131937671 2 -0.10980516924991932
		 3 -0.079659974012600526 4 -0.047608516480073339 5 -0.015080502146488511 6 0.016494385092701676
		 7 0.045686454337764852 8 0.071066024969313579 9 0.091203382863921115 10 0.10466884942608629
		 11 0.11003273375607597 12 0.10538863357040186 13 0.091187817136361871 14 0.069537099380408696
		 15 0.042543295228991848 16 0.012313191761921234 17 -0.019046312554425196 18 -0.049428512432510271
		 19 -0.076726486802279018 20 -0.098833515851016324 21 -0.11364270810296034 22 -0.1190472990819872
		 23 -0.11364586928509973 24 -0.098844828804570184 25 -0.076749157498107706 26 -0.049463708289010637
		 27 -0.019093505682676593 28 0.012256626989656266 29 0.042481650686158931 30 0.069476741556393176
		 31 0.091136876133362035 32 0.10535721745766799 33 0.11003273375607597 34 0.10548131690600585
		 35 0.094138915564790454 36 0.077077719271502018 37 0.055370112754747389 38 0.030088304088012843
		 39 0.0023045527734548302 40 -0.02690898744049924 41 -0.056479844905131318 42 -0.085335862676421917
		 43 -0.11240478125221562 44 -0.13661442131937712;
createNode animCurveTA -n "keyboardMonster_root_JNT_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 2.3294854673275363 2 2.2904693447786157
		 3 2.2487208562860888 4 2.2054110035642362 5 2.1617108218383141 6 2.1187913172048116
		 7 2.0778235053098513 8 2.0399783857539528 9 2.0064270084556384 10 1.9783403713577239
		 11 1.9568894901063316 12 1.9422261305461301 13 1.9331348471468426 14 1.9287358384921787
		 15 1.9281493031658461 16 1.9304954428979688 17 1.9348944468330094 18 1.9404665282379501
		 19 1.9463318668180039 20 1.9516106810841807 21 1.9554231517904987 22 1.9568894901063305
		 23 1.95548980845517 24 1.9518506275524206 25 1.9468117697538874 26 1.9412130283825508
		 27 1.9358942328021473 28 1.9316951753345157 29 1.9294556803377378 30 1.9300155511461048
		 31 1.9342145950984557 32 1.9428926585777566 33 1.9568894901063316 34 1.9767649848233109
		 35 2.0019592711043774 36 2.0316326538101794 37 2.0649451099305889 38 2.1010569298098494
		 39 2.1391283051806775 40 2.1783195689354384 41 2.2177906299869949 42 2.2567018187248826
		 43 2.2942133268818168 44 2.3294854673275354;
createNode animCurveTL -n "keyboardMonster_R_Wrist_IK_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -62.59794828947534 2 -62.315897782422837
		 3 -62.025110693261666 4 -61.729331187480739 5 -61.432303660774863 6 -61.137772310224278
		 7 -60.849481398602649 8 -60.57117506706075 9 -60.306597828592203 10 -60.059493819047361
		 11 -59.833607301199919 12 -59.634316689711483 13 -59.460671551765614 14 -59.306922878918925
		 15 -59.167321662727943 16 -59.036118779485889 17 -58.907565566539432 18 -58.77591254884922
		 19 -58.635411195652637 20 -58.480311989180478 21 -58.30486648252014 22 -58.103325032932219
		 23 -57.012415235062491 24 -54.702333343930121 25 -51.967551494558705 26 -49.602525674282646
		 27 -47.588526095335993 28 -45.58798232693286 29 -43.917130442308633 30 -42.892194138714672
		 31 -43.181964411560244 32 -44.441015002131707 33 -45.464238642623855 34 -45.595488269028891
		 35 -45.314390418044184 36 -45.119516103100693 37 -45.509439165733824 38 -46.982729419496565
		 39 -51.477143846648886 40 -58.035285015438234 41 -62.812900060870589 42 -63.956142457493115
		 43 -63.314307425377223 44 -62.597945507992506;
createNode animCurveTL -n "keyboardMonster_R_Wrist_IK_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 114.79610307610631 2 114.84180759131925
		 3 114.88892781485362 4 114.93685702818107 5 114.98498847546995 6 115.03271543307267
		 7 115.07943116669662 8 115.12452896175731 9 115.16740204341545 10 115.20744369794551
		 11 115.24404719105476 12 115.27634098438901 13 115.30447908962356 14 115.32939309751004
		 15 115.35201459880008 16 115.37327520292303 17 115.39410644459738 18 115.4154399901837
		 19 115.43820735302847 20 115.46334020641632 21 115.49177005010637 22 115.5244285776339
		 23 114.99960018845974 24 113.66633435880317 25 111.99171974425411 26 110.44283535699833
		 27 108.98879898889925 28 107.43080315205549 29 105.98406702138089 30 104.86379953913996
		 31 104.34174137411648 32 104.27118474924728 33 104.23967948805873 34 103.90712433551104
		 35 103.39810677137041 36 102.99720832401758 37 102.98901513853559 38 103.65810784776221
		 39 106.17762806337413 40 110.05513241998847 41 113.07677106559635 42 114.21962436801269
		 43 114.50736787109099 44 114.79610001092942;
createNode animCurveTL -n "keyboardMonster_R_Wrist_IK_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -115.3911454768735 2 -111.92129641272847
		 3 -108.34396794154273 4 -104.70522163311611 5 -101.05112188929338 6 -97.427730668518166
		 7 -93.881110737409273 8 -90.457323366353279 9 -87.202434400231994 10 -84.162505044225057
		 11 -81.383598064951244 12 -78.931879934774969 13 -76.795658214448196 14 -74.904207303171034
		 15 -73.186801600143596 16 -71.572714086570102 17 -69.991223415638359 18 -68.371598246380231
		 19 -66.643118854533427 20 -64.735053373456637 21 -62.576683110429698 22 -60.097274661400249
		 23 -57.025670765366712 24 -53.831012382478619 25 -51.553061542456348 26 -51.231563182407882
		 27 -53.361689347908609 28 -57.430525020158058 29 -62.965668311292447 30 -69.494750553699618
		 31 -78.90417756113645 32 -90.463564488326938 33 -99.77513054575239 34 -106.13886155822971
		 35 -111.77026671354753 36 -116.64502227769853 37 -120.73875352586103 38 -124.02713947360061
		 39 -124.97627576074561 40 -123.45820376312929 41 -121.55753040355282 42 -119.81968875254964
		 43 -117.64294876184331 44 -115.39114446206368;
createNode animCurveTA -n "keyboardMonster_R_Wrist_IK_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -43.322835867790467 2 -43.322835867790467
		 3 -43.322835867790467 4 -43.322835867790467 5 -43.322835867790467 6 -43.322835867790467
		 7 -43.322835867790467 8 -43.322835867790467 9 -43.322835867790467 10 -43.322835867790467
		 11 -43.322835867790467 12 -43.322835867790467 13 -43.322835867790467 14 -43.322835867790467
		 15 -43.322835867790467 16 -43.322835867790467 17 -43.322835867790467 18 -43.322835867790467
		 19 -43.322835867790467 20 -43.322835867790467 21 -43.322835867790467 22 -43.322835867790467
		 23 -45.312942568584816 24 -50.187491263965285 25 -56.302789488116694 26 -62.015180159153665
		 27 -67.221895371819187 28 -72.687727008242732 29 -78.070256491824722 30 -83.027103226386402
		 31 -88.227983872998749 32 -92.874999459103719 33 -94.424226926692313 34 -93.738252132718728
		 35 -92.656322561085815 36 -90.099564035108941 37 -84.989130517097564 38 -76.246155033887135
		 39 -54.679010803614986 40 -25.019253855001335 41 -187.07321386015465 42 -11.347327948836099
		 43 -27.335046471953842 44 -43.322835867790467;
createNode animCurveTA -n "keyboardMonster_R_Wrist_IK_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -116.10660649682038 2 -116.10660649682038
		 3 -116.10660649682038 4 -116.10660649682038 5 -116.10660649682038 6 -116.10660649682038
		 7 -116.10660649682038 8 -116.10660649682038 9 -116.10660649682038 10 -116.10660649682038
		 11 -116.10660649682038 12 -116.10660649682038 13 -116.10660649682038 14 -116.10660649682038
		 15 -116.10660649682038 16 -116.10660649682038 17 -116.10660649682038 18 -116.10660649682038
		 19 -116.10660649682038 20 -116.10660649682038 21 -116.10660649682038 22 -116.10660649682038
		 23 -119.08200584662976 24 -126.63986346416731 25 -136.72761403193121 26 -147.29275044440524
		 27 -160.06380171620091 28 -175.37686728697477 29 -189.09057597147586 30 -197.06365051860755
		 31 -194.19765694561335 32 -183.58013802769003 33 -173.34882907912169 34 -166.33056943406893
		 35 -159.68923458061343 36 -153.06794635938422 37 -146.1098969755453 38 -138.45820883605487
		 39 -127.88778592156385 40 -116.05138653287959 41 -71.590903880253947 42 -108.17888364622945
		 43 -112.14273549292287 44 -116.10660649682038;
createNode animCurveTA -n "keyboardMonster_R_Wrist_IK_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 99.120760525626423 2 99.120760525626423
		 3 99.120760525626423 4 99.120760525626423 5 99.120760525626423 6 99.120760525626423
		 7 99.120760525626423 8 99.120760525626423 9 99.120760525626423 10 99.120760525626423
		 11 99.120760525626423 12 99.120760525626423 13 99.120760525626423 14 99.120760525626423
		 15 99.120760525626423 16 99.120760525626423 17 99.120760525626423 18 99.120760525626423
		 19 99.120760525626423 20 99.120760525626423 21 99.120760525626423 22 99.120760525626423
		 23 103.14839589384262 24 112.39619436625848 25 122.61142708177249 26 129.54142464528411
		 27 132.42641246073958 28 133.4097424315554 29 132.59333973690832 30 130.07913039596474
		 31 123.21174430640046 32 113.26560464184671 33 106.2374879891847 34 104.72206940683957
		 35 105.78669702763395 36 107.13702018440073 37 106.4786691091285 38 101.51729621557365
		 39 82.989768766652702 40 56.758112568941051 41 223.21608548084416 42 53.391259851416095
		 43 76.255961961928435 44 99.120760525626423;
createNode animCurveTL -n "keyboardMonster_L_Wrist_IK_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 59.924854051467008 2 57.825907519365614
		 3 55.497446358622966 4 53.108797728737926 5 50.829290632015613 6 48.828252437926295
		 7 47.275011049426325 8 46.338894114165271 9 45.828613911351994 10 45.452447237379005
		 11 45.229070974573688 12 45.178796157151368 13 45.315604971849382 14 45.648681029778004
		 15 46.187207942047479 16 46.940370091833017 17 47.917348774050225 18 49.127331104708503
		 19 50.579496356740009 20 54.964416508042376 21 61.415212203757164 22 64.604920571667876
		 23 64.387782141536903 24 64.14738934371664 25 63.888661976878979 26 63.616518066430444
		 27 63.335877524593663 28 63.051658298407446 29 62.768780343279055 30 62.492161598245701
		 31 62.226721991533232 32 61.977379096008946 33 61.74905364139827 34 61.540416377946229
		 35 61.345423447469692 36 61.161919845525773 37 60.987752512048502 38 60.82076640535599
		 39 60.658807159286162 40 60.499719841306103 41 60.341351216711395 42 60.1815463465486
		 43 60.018150864655695 44 59.924854051467008;
	setAttr -s 44 ".kit[21:43]"  1 9 9 9 9 9 9 9 
		9 9 9 9 9 9 9 9 9 9 9 9 9 9 9;
	setAttr -s 44 ".kot[21:43]"  1 9 9 9 9 9 9 9 
		9 9 9 9 9 9 9 9 9 9 9 9 9 9 9;
	setAttr -s 44 ".kix[21:43]"  0.44635143876075745 0.14418694376945496 
		0.13239261507987976 0.12460105866193771 0.11973394453525543 0.11720988154411316 0.11675374209880829 
		0.11831773072481155 0.12206824868917465 0.12843212485313416 0.1382271945476532 0.15082290768623352 
		0.16295969486236572 0.17346540093421936 0.18323543667793274 0.19178768992424011 0.19862948358058929 
		0.20331709086894989 0.20552012324333191 0.20507578551769257 0.20201730728149414 0.25137504935264587 
		0.33645394444465637;
	setAttr -s 44 ".kiy[21:43]"  0.89485770463943481 -0.98955047130584717 
		-0.99119740724563599 -0.99220693111419678 -0.99280595779418945 -0.99310719966888428 
		-0.99316084384918213 -0.99297583103179932 -0.99252170324325562 -0.99171829223632813 
		-0.99040055274963379 -0.98856079578399658 -0.98663276433944702 -0.98483997583389282 
		-0.98306900262832642 -0.98143643140792847 -0.98007464408874512 -0.97911298274993896 
		-0.9786529541015625 -0.97874611616134644 -0.9793819785118103 -0.96788978576660156 
		-0.94169992208480835;
	setAttr -s 44 ".kox[21:43]"  0.446351557970047 0.14418694376945496 0.13239261507987976 
		0.12460105866193771 0.11973394453525543 0.11720988154411316 0.11675374209880829 0.11831773072481155 
		0.12206824868917465 0.12843212485313416 0.1382271945476532 0.15082290768623352 0.16295969486236572 
		0.17346540093421936 0.18323543667793274 0.19178768992424011 0.19862948358058929 0.20331709086894989 
		0.20552012324333191 0.20507578551769257 0.20201730728149414 0.25137504935264587 0.33645394444465637;
	setAttr -s 44 ".koy[21:43]"  0.89485770463943481 -0.98955047130584717 
		-0.99119740724563599 -0.99220693111419678 -0.99280595779418945 -0.99310719966888428 
		-0.99316084384918213 -0.99297583103179932 -0.99252170324325562 -0.99171829223632813 
		-0.99040055274963379 -0.98856079578399658 -0.98663276433944702 -0.98483997583389282 
		-0.98306900262832642 -0.98143643140792847 -0.98007464408874512 -0.97911298274993896 
		-0.9786529541015625 -0.97874611616134644 -0.9793819785118103 -0.96788978576660156 
		-0.94169992208480835;
createNode animCurveTL -n "keyboardMonster_L_Wrist_IK_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -109.78407051892552 2 -108.37010809392505
		 3 -106.77352719533795 4 -105.1317463052988 5 -103.58218516994987 6 -102.26226240886957
		 7 -101.30939700867081 8 -100.86100788875713 9 -100.82454372689526 10 -100.99548205779081
		 11 -101.34328631675541 12 -101.837155135039 13 -102.44731269392113 14 -103.14476075375769
		 15 -103.90050107490455 16 -104.68553612404628 17 -105.47086554255254 18 -106.22749386477011
		 19 -106.92642017971198 20 -108.05398346242035 21 -109.46285006745302 22 -110.17352550758693
		 23 -110.15689418765271 24 -110.13848174205957 25 -110.11866499377084 26 -110.09782062992331
		 27 -110.07632548217387 28 -110.05455623165589 29 -110.03288971333758 30 -110.01170260773837
		 31 -109.99137174774229 32 -109.97227378582772 33 -109.95478561626473 34 -109.93880542760346
		 35 -109.92387030161201 36 -109.90981517967383 37 -109.89647515209937 38 -109.88368515742408
		 39 -109.87128018592193 40 -109.85909518448565 41 -109.8469652300536 42 -109.83472526902374
		 43 -109.82221029167196 44 -109.78407051892552;
createNode animCurveTL -n "keyboardMonster_L_Wrist_IK_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 57.450438598386008 2 54.616562730454206
		 3 52.845359838991207 4 52.030201657394102 5 52.064460726967106 6 52.841508959395128
		 7 54.25471855143936 8 56.197462670806033 9 58.736970454525093 10 61.981529600303432
		 11 65.886096343825315 12 70.425730626520561 13 75.497633478206552 14 80.939972767148888
		 15 86.590916361613068 16 92.288637196698872 17 97.871287940169324 18 103.17705623250484
		 19 108.04409115999977 20 112.18198646095622 21 114.90102665073341 22 115.44047108252147
		 23 113.69108272216968 24 110.76739179749379 25 107.48092369957453 26 103.97525640626081
		 27 100.32344983866875 28 96.59853849495029 29 92.873583080298843 30 89.221617831265391
		 31 85.715703192167965 32 82.428868545029331 33 79.434184891794018 34 76.708430303867516
		 35 74.160932958160572 36 71.763538664237586 37 69.488118876690208 38 67.575150285260506
		 39 65.719768679210389 40 63.897287464344728 41 62.083039496545659 42 60.252338108065359
		 43 58.380503192971283 44 57.450438598386008;
createNode animCurveTA -n "keyboardMonster_L_Wrist_IK_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -55.791411972469959 2 -60.211812739118123
		 3 -64.958310954224885 4 -69.78633472976864 5 -74.451308437156584 6 -78.708659730793045
		 7 -82.31381518593524 8 -85.022202351695256 9 -86.647021422962936 10 -87.296752581496634
		 11 -87.16973646984863 12 -86.464313730571391 13 -85.378825006217141 14 -84.111610939338235
		 15 -82.560831286207744 16 -80.477585552375103 17 -77.839633486820475 18 -74.624719056267523
		 19 -70.810607211961184 20 -63.879860779583012 21 -55.325871863603595 22 -51.142548637632636
		 23 -53.771179774332111 24 -56.650164222871162 25 -57.045772697301288 26 -57.255940124889825
		 27 -57.317754020865173 28 -57.268302961561474 29 -57.144675390674294 30 -56.983959221346836
		 31 -56.823243295189364 32 -56.699615226908982 33 -56.650164222871162 34 -56.650164222871162
		 35 -56.650164222871162 36 -56.650164222871162 37 -56.650164222871162 38 -56.650164222871162
		 39 -56.650164222871162 40 -56.650164222871162 41 -56.650164222871162 42 -56.650164222871162
		 43 -56.650164222871162 44 -55.791411972469959;
createNode animCurveTA -n "keyboardMonster_L_Wrist_IK_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 237.00144515954025 2 235.79567521733443
		 3 232.35895235556518 4 226.96215477754876 5 219.87616455476402 6 211.37186029836462
		 7 201.72012161681818 8 191.19182325651659 9 179.11657522718122 10 165.72860971346836
		 11 152.6557634645356 12 141.52587322954028 13 133.96677575764002 14 131.60630779799223
		 15 136.26416172210574 16 147.36939353950532 17 162.96383043275961 18 181.08938970267027
		 19 199.7878727264972 20 219.63513498399107 21 237.28906222379081 22 244.91480980336266
		 23 239.02910001701068 24 233.14337444393277 25 233.14337444393283 26 233.14337444393283
		 27 233.14337444393283 28 233.14337444393277 29 233.14337444393277 30 233.14337444393283
		 31 233.14337444393283 32 233.14337444393283 33 233.14337444393277 34 233.14337444393277
		 35 233.14337444393277 36 233.14337444393277 37 233.14337444393277 38 233.14337444393277
		 39 233.14337444393277 40 233.14337444393277 41 233.14337444393277 42 233.14337444393277
		 43 233.14337444393277 44 237.00144515954025;
createNode animCurveTA -n "keyboardMonster_L_Wrist_IK_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -248.64687201755169 2 -246.2059066942339
		 3 -243.46129338526612 4 -240.64076738279226 5 -237.97206615154863 6 -235.68292522254259
		 7 -234.00108075721812 8 -233.15426875904475 9 -233.34662953893616 10 -234.48319223267399
		 11 -236.3430283751745 12 -238.70520950135392 13 -241.34880714612831 14 -244.05289284441398
		 15 -247.04938603824525 16 -250.58430884173916 17 -254.45787176064084 18 -258.4703089403564
		 19 -262.42182680006817 20 -267.3670745834055 21 -272.51307448542389 22 -274.88867258838792
		 23 -262.22062192974909 24 -248.3460541575887 25 -246.43951499041322 26 -245.42666400702785
		 27 -245.12876691591407 28 -245.36708431181071 29 -245.96287742867651 30 -246.73741005733814
		 31 -247.51194151410093 32 -248.10773702803215 33 -248.3460541575887 34 -248.3460541575887
		 35 -248.3460541575887 36 -248.3460541575887 37 -248.3460541575887 38 -248.3460541575887
		 39 -248.3460541575887 40 -248.3460541575887 41 -248.3460541575887 42 -248.3460541575887
		 43 -248.3460541575887 44 -248.64687201755169;
createNode animCurveTL -n "keyboardMonster_R_Foot_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 1.696204224435802 2 1.696204224435802
		 3 1.696204224435802 4 1.696204224435802 5 1.696204224435802 6 1.696204224435802 7 1.696204224435802
		 8 1.696204224435802 9 1.696204224435802 10 1.696204224435802 11 1.696204224435802
		 12 1.696204224435802 13 1.696204224435802 14 1.696204224435802 15 1.696204224435802
		 16 1.696204224435802 17 1.696204224435802 18 1.696204224435802 19 1.696204224435802
		 20 1.696204224435802 21 1.696204224435802 22 1.696204224435802 23 1.696204224435802
		 24 1.696204224435802 25 1.696204224435802 26 1.696204224435802 27 1.696204224435802
		 28 1.696204224435802 29 1.696204224435802 30 1.696204224435802 31 1.696204224435802
		 32 1.696204224435802 33 1.696204224435802 34 1.696204224435802 35 1.696204224435802
		 36 1.696204224435802 37 1.696204224435802 38 1.696204224435802 39 1.696204224435802
		 40 1.696204224435802 41 1.696204224435802 42 1.696204224435802 43 1.696204224435802
		 44 1.696204224435802;
createNode animCurveTL -n "keyboardMonster_R_Foot_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -1.4493927001774836 2 -1.4493927001774836
		 3 -1.4493927001774836 4 -1.4493927001774836 5 -1.4493927001774836 6 -1.4493927001774836
		 7 -1.4493927001774836 8 -1.4493927001774836 9 -1.4493927001774836 10 -1.4493927001774836
		 11 -1.4493927001774836 12 -1.6707713824556265 13 -2.2298762882216323 14 -2.9691607058728628
		 15 -3.7310779238066818 16 -4.390627760845577 17 -4.8861621899332874 18 -5.1551675823661398
		 19 -5.135127724280272 20 -4.5933053665704087 21 -3.5746730291279007 22 -2.433273039897454
		 23 -1.1318759855471185 24 0.35998199976685896 25 1.6780882169595195 26 2.4582391478219279
		 27 2.7136664249602802 28 2.7338302316987289 29 2.5688588163179773 30 2.2688800287411226
		 31 1.8840232621670587 32 1.4644148286402192 33 0.70667340055067385 34 -0.39058224059317581
		 35 -1.3242167568690313 36 -1.2720365260442819 37 -1.2720365260442819 38 -1.2720365260442819
		 39 -1.2720365260442819 40 -1.2720365260442819 41 -1.2720365260442819 42 -1.2720365260442819
		 43 -1.2720365260442819 44 -1.2720365260442819;
createNode animCurveTL -n "keyboardMonster_R_Foot_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -7.1054273576010019e-015 2 -3.4815935890318368
		 3 -7.0710303745797933 4 -10.722092880566025 5 -14.388560789282055 6 -18.024216234690769
		 7 -21.582840539844454 8 -25.01821652909193 9 -28.284122436803393 10 -31.334341152755322
		 11 -34.122654 12 -36.582670550096843 13 -38.726122818120167 14 -40.623975792394596
		 15 -42.347194461244776 16 -43.929162304485075 17 -45.134590252631249 18 -45.631446158433477
		 19 -45.087692578433554 20 -42.460740941269378 21 -37.302119260024462 22 -30.170919874824811
		 23 -20.878424167475856 24 -10.006694495694044 25 0.72886163685420335 26 9.6129099104197291
		 27 16.035989261864941 28 20.656337713214029 29 23.660162659449753 30 25.233699205713123
		 31 25.563165643226355 32 24.834785375639058 33 23.415490356913466 34 21.612566378481574
		 35 19.501917434047186 36 17.210425655935637 37 14.927303928493968 38 12.73831934247019
		 39 10.615230779996507 40 8.5297896987724311 41 6.4537698129628307 42 4.3589224961018953
		 43 2.2170066303218743 44 -0.00022699999999709064;
createNode animCurveTA -n "keyboardMonster_R_Foot_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 0 2 0 3 0 4 0 5 0 6 0 7 0 8 0 9 0 10 0
		 11 0 12 0.15569380974793556 13 0.52053115411350515 14 0.94114590577935309 15 1.2641719374281244
		 16 1.5796849772551707 17 1.9258216337571143 18 2.0713100437090475 19 1.784877137094484
		 20 0.43297576151859241 21 -1.5497318765440826 22 -2.7922203617103629 23 -2.5552817991621688
		 24 -1.6058887714579766 25 -0.8322775590695225 26 -1.1226803982402698 27 -2.8671787138920255
		 28 -5.4994174354758467 29 -8.4729568639503672 30 -11.24137604530849 31 -13.258234793490002
		 32 -13.977108532379868 33 -11.730038098035605 34 -6.932429579831016 35 -2.1628834048141803
		 36 0 37 0 38 0 39 0 40 0 41 0 42 0 43 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_Foot_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 0 2 0 3 0 4 0 5 0 6 0 7 0 8 0 9 0 10 0
		 11 0 12 0 13 0 14 0 15 0 16 0 17 0 18 0 19 0 20 0 21 0 22 0 23 0 24 0 25 0 26 0 27 0
		 28 0 29 0 30 0 31 0 32 0 33 0 34 0 35 0 36 0 37 0 38 0 39 0 40 0 41 0 42 0 43 0 44 0;
createNode animCurveTA -n "keyboardMonster_R_Foot_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 0 2 0 3 0 4 0 5 0 6 0 7 0 8 0 9 0 10 0
		 11 0 12 0 13 0 14 0 15 0 16 0 17 0 18 0 19 0 20 0 21 0 22 0 23 0 24 0 25 0 26 0 27 0
		 28 0 29 0 30 0 31 0 32 0 33 0 34 0 35 0 36 0 37 0 38 0 39 0 40 0 41 0 42 0 43 0 44 0;
createNode animCurveTL -n "keyboardMonster_L_Foot_CTRL_translateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -1.69 2 -1.69 3 -1.69 4 -1.69 5 -1.69
		 6 -1.69 7 -1.69 8 -1.69 9 -1.69 10 -1.69 11 -1.69 12 -1.69 13 -1.69 14 -1.69 15 -1.69
		 16 -1.69 17 -1.69 18 -1.69 19 -1.69 20 -1.69 21 -1.69 22 -1.69 23 -1.69 24 -1.69
		 25 -1.69 26 -1.69 27 -1.69 28 -1.69 29 -1.69 30 -1.69 31 -1.69 32 -1.69 33 -1.69
		 34 -1.69 35 -1.69 36 -1.69 37 -1.69 38 -1.69 39 -1.69 40 -1.69 41 -1.69 42 -1.69
		 43 -1.69 44 -1.69;
createNode animCurveTL -n "keyboardMonster_L_Foot_CTRL_translateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -4.6329418360057701 2 -4.6319441172031546
		 3 -4.5916069186059261 4 -4.4559141373612441 5 -4.1688497660018733 6 -3.6743977292488843
		 7 -2.9758387096724697 8 -2.1803692137703727 9 -1.3878465683022796 10 -0.69812701024228119
		 11 -0.21106711794570643 12 -0.026523469767887775 13 0.22955641667299176 14 0.34606426006171631
		 15 -0.23645003116345009 16 -1.0258262441337962 17 -1.4222438584851176 18 -1.4222438584851176
		 19 -1.4222438584851176 20 -1.4222438584851176 21 -1.4222438584851176 22 -1.4222438584851176
		 23 -1.4222438584851176 24 -1.4222438584851176 25 -1.4222438584851176 26 -1.4222438584851176
		 27 -1.4222438584851176 28 -1.4222438584851176 29 -1.4222438584851176 30 -1.4222438584851176
		 31 -1.4222438584851176 32 -1.4222438584851176 33 -1.4222438584851176 34 -1.4970233095821968
		 35 -1.7020637600462152 36 -2.0084195979038633 37 -2.387141691423853 38 -2.8092841180661541
		 39 -3.2459000235899307 40 -3.6680440030670347 41 -4.0467660965870245 42 -4.3531210027438636
		 43 -4.5581618672967199 44 -4.6329418360057701;
createNode animCurveTL -n "keyboardMonster_L_Foot_CTRL_translateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 -45.733644301519895 2 -46.243077784949818
		 3 -41.903249251560233 4 -34.148758736077717 5 -24.414211633615999 6 -14.134208200458069
		 7 -4.6671203515595074 8 3.5968348771689485 9 10.675240328406296 10 16.585690188713269
		 11 21.345774682764883 12 24.993255786728987 13 27.455031128016394 14 28.525191673783603
		 15 28.499467303124398 16 27.829173841574907 17 26.537766862509187 18 24.912659688773047
		 19 23.178329849584415 20 21.263806069569007 21 19.098130291863917 22 16.610329698480115
		 23 13.763909202759528 24 10.612652254248097 25 7.2210514246472286 26 3.6535760403089768
		 27 -0.025279838553373679 28 -3.7510479128928722 29 -7.4592335563979191 30 -11.08536857491309
		 31 -14.564958698442027 32 -17.833540532871055 33 -20.826609301519881 34 -23.56158985057651
		 35 -26.11770980361382 36 -28.523218645584251 37 -30.806340373025918 38 -32.995324959049697
		 39 -35.118413521523379 40 -37.203854602747455 41 -39.279874488557056 42 -41.374721805417991
		 43 -43.516637671198012 44 -45.733871301519883;
createNode animCurveTA -n "keyboardMonster_L_Foot_CTRL_rotateX";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 0 2 -0.28692776758280664 3 -1.0736650799065
		 4 -2.2491434540566897 5 -3.7022935630413669 6 -5.3220468494687054 7 -6.9973345325145635
		 8 -8.6170885264775361 9 -10.070238560984784 10 -11.245716599986505 11 -12.032453862658585
		 12 -12.31938156817686 13 -13.653757139797419 14 -14.035284281161147 15 -6.6022913905636482
		 16 -2.1315353258724339 17 0 18 0 19 0 20 0 21 0 22 0 23 0 24 0 25 0 26 0 27 0 28 0
		 29 0 30 0 31 0 32 0 33 0 34 0 35 0 36 0 37 0 38 0 39 0 40 0 41 0 42 0 43 0 44 0;
createNode animCurveTA -n "keyboardMonster_L_Foot_CTRL_rotateY";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 0 2 0.0009312518131449185 3 0.0034846838310438402
		 4 0.0072998125530281477 5 0.012016151738893701 6 0.01727321764624817 7 0.022710525807528015
		 8 0.027967594011257806 9 0.032683932955399633 10 0.036499060589627229 11 0.039052492446377006
		 12 0.039983744058085489 13 0.034945792294158791 14 0.025909466149639401 15 0.015638086587354216
		 16 0.0049402058092167939 17 0 18 0 19 0 20 0 21 0 22 0 23 0 24 0 25 0 26 0 27 0 28 0
		 29 0 30 0 31 0 32 0 33 0 34 0 35 0 36 0 37 0 38 0 39 0 40 0 41 0 42 0 43 0 44 0;
createNode animCurveTA -n "keyboardMonster_L_Foot_CTRL_rotateZ";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr -s 44 ".ktv[0:43]"  1 0 2 -0.008483679268752849 3 -0.03174537709166788
		 4 -0.066501098357882565 5 -0.10946682299943848 6 -0.15735855370337659 7 -0.20689228655044722
		 8 -0.2547840381743034 9 -0.29774976061376285 10 -0.3325054719705422 11 -0.35576716832539329
		 12 -0.36425084575906536 13 -0.31835523919167491 14 -0.23603454805187432 15 -0.14246255301109798
		 16 -0.045005143530252638 17 0 18 0 19 0 20 0 21 0 22 0 23 0 24 0 25 0 26 0 27 0 28 0
		 29 0 30 0 31 0 32 0 33 0 34 0 35 0 36 0 37 0 38 0 39 0 40 0 41 0 42 0 43 0 44 0;
select -ne :time1;
	setAttr -av -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :renderPartition;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 7 ".st";
	setAttr -cb on ".an";
	setAttr -cb on ".pt";
select -ne :renderGlobalsList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
select -ne :defaultShaderList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 6 ".s";
select -ne :postProcessList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 15 ".u";
select -ne :defaultRenderingList1;
	setAttr -s 2 ".r";
select -ne :defaultTextureList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 6 ".tx";
select -ne :initialShadingGroup;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".mwc";
	setAttr -cb on ".an";
	setAttr -cb on ".il";
	setAttr -cb on ".vo";
	setAttr -cb on ".eo";
	setAttr -cb on ".fo";
	setAttr -cb on ".epo";
	setAttr ".ro" yes;
	setAttr -cb on ".mimt";
	setAttr -cb on ".miop";
	setAttr -k on ".mico";
	setAttr -cb on ".mise";
	setAttr -k on ".mism";
	setAttr -cb on ".mice";
	setAttr -av -cb on ".micc";
	setAttr -k on ".micr";
	setAttr -k on ".micg";
	setAttr -k on ".micb";
	setAttr -cb on ".mica";
	setAttr -cb on ".micw";
	setAttr -cb on ".mirw";
select -ne :initialParticleSE;
	setAttr -av -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".mwc";
	setAttr -cb on ".an";
	setAttr -cb on ".il";
	setAttr -cb on ".vo";
	setAttr -cb on ".eo";
	setAttr -cb on ".fo";
	setAttr -cb on ".epo";
	setAttr ".ro" yes;
	setAttr -cb on ".mimt";
	setAttr -cb on ".miop";
	setAttr -k on ".mico";
	setAttr -cb on ".mise";
	setAttr -k on ".mism";
	setAttr -cb on ".mice";
	setAttr -av -cb on ".micc";
	setAttr -k on ".micr";
	setAttr -k on ".micg";
	setAttr -k on ".micb";
	setAttr -cb on ".mica";
	setAttr -av -cb on ".micw";
	setAttr -cb on ".mirw";
select -ne :defaultRenderGlobals;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".macc";
	setAttr -k on ".macd";
	setAttr -k on ".macq";
	setAttr -k on ".mcfr" 30;
	setAttr -cb on ".ifg";
	setAttr -k on ".clip";
	setAttr -k on ".edm";
	setAttr -k on ".edl";
	setAttr -cb on ".ren";
	setAttr -av -k on ".esr";
	setAttr -k on ".ors";
	setAttr -cb on ".sdf";
	setAttr -av -k on ".outf";
	setAttr -cb on ".imfkey";
	setAttr -k on ".gama";
	setAttr -cb on ".an";
	setAttr -cb on ".ar";
	setAttr -av ".bfs";
	setAttr -cb on ".me";
	setAttr -cb on ".se";
	setAttr -k on ".be";
	setAttr -cb on ".ep";
	setAttr -k on ".fec";
	setAttr -k on ".ofc";
	setAttr -cb on ".ofe";
	setAttr -cb on ".efe";
	setAttr -cb on ".oft";
	setAttr -cb on ".umfn";
	setAttr -cb on ".ufe";
	setAttr -cb on ".pff";
	setAttr -cb on ".peie";
	setAttr -cb on ".ifp";
	setAttr -k on ".comp";
	setAttr -k on ".cth";
	setAttr -k on ".soll";
	setAttr -cb on ".sosl";
	setAttr -k on ".rd";
	setAttr -k on ".lp";
	setAttr -av -k on ".sp";
	setAttr -k on ".shs";
	setAttr -k on ".lpr";
	setAttr -cb on ".gv";
	setAttr -cb on ".sv";
	setAttr -k on ".mm";
	setAttr -k on ".npu";
	setAttr -k on ".itf";
	setAttr -k on ".shp";
	setAttr -cb on ".isp";
	setAttr -k on ".uf";
	setAttr -k on ".oi";
	setAttr -k on ".rut";
	setAttr -k on ".mb";
	setAttr -av -k on ".mbf";
	setAttr -k on ".afp";
	setAttr -k on ".pfb";
	setAttr -k on ".pram" -type "string" "XgPreRendering;";
	setAttr -k on ".poam";
	setAttr -k on ".prlm";
	setAttr -k on ".polm";
	setAttr -cb on ".prm";
	setAttr -cb on ".pom";
	setAttr -cb on ".pfrm";
	setAttr -cb on ".pfom";
	setAttr -av -k on ".bll";
	setAttr -av -k on ".bls";
	setAttr -av -k on ".smv";
	setAttr -k on ".ubc";
	setAttr -k on ".mbc";
	setAttr -cb on ".mbt";
	setAttr -k on ".udbx";
	setAttr -k on ".smc";
	setAttr -k on ".kmv";
	setAttr -cb on ".isl";
	setAttr -cb on ".ism";
	setAttr -cb on ".imb";
	setAttr -k on ".rlen";
	setAttr -av -k on ".frts";
	setAttr -k on ".tlwd";
	setAttr -k on ".tlht";
	setAttr -k on ".jfc";
	setAttr -cb on ".rsb";
	setAttr -k on ".ope";
	setAttr -k on ".oppf";
	setAttr -cb on ".hbl";
select -ne :defaultResolution;
	setAttr ".pa" 1;
select -ne :hardwareRenderGlobals;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr ".ctrs" 256;
	setAttr -av ".btrs" 512;
	setAttr -k off ".fbfm";
	setAttr -k off -cb on ".ehql";
	setAttr -k off -cb on ".eams";
	setAttr -k off -cb on ".eeaa";
	setAttr -k off -cb on ".engm";
	setAttr -k off -cb on ".mes";
	setAttr -k off -cb on ".emb";
	setAttr -av -k off -cb on ".mbbf";
	setAttr -k off -cb on ".mbs";
	setAttr -k off -cb on ".trm";
	setAttr -k off -cb on ".tshc";
	setAttr -k off ".enpt";
	setAttr -k off -cb on ".clmt";
	setAttr -k off -cb on ".tcov";
	setAttr -k off -cb on ".lith";
	setAttr -k off -cb on ".sobc";
	setAttr -k off -cb on ".cuth";
	setAttr -k off -cb on ".hgcd";
	setAttr -k off -cb on ".hgci";
	setAttr -k off -cb on ".mgcs";
	setAttr -k off -cb on ".twa";
	setAttr -k off -cb on ".twz";
	setAttr -k on ".hwcc";
	setAttr -k on ".hwdp";
	setAttr -k on ".hwql";
	setAttr -k on ".hwfr" 30;
select -ne :hardwareRenderingGlobals;
	setAttr ".otfna" -type "stringArray" 22 "NURBS Curves" "NURBS Surfaces" "Polygons" "Subdiv Surface" "Particles" "Particle Instance" "Fluids" "Strokes" "Image Planes" "UI" "Lights" "Cameras" "Locators" "Joints" "IK Handles" "Deformers" "Motion Trails" "Components" "Hair Systems" "Follicles" "Misc. UI" "Ornaments"  ;
	setAttr ".otfva" -type "Int32Array" 22 0 1 1 1 1 1
		 1 1 1 0 0 0 0 0 0 0 0 0
		 0 0 0 0 ;
select -ne :defaultHardwareRenderGlobals;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".rp";
	setAttr -k on ".cai";
	setAttr -k on ".coi";
	setAttr -cb on ".bc";
	setAttr -av -k on ".bcb";
	setAttr -av -k on ".bcg";
	setAttr -av -k on ".bcr";
	setAttr -k on ".ei";
	setAttr -k on ".ex";
	setAttr -av -k on ".es";
	setAttr -av -k on ".ef";
	setAttr -av -k on ".bf";
	setAttr -k on ".fii";
	setAttr -av -k on ".sf";
	setAttr -k on ".gr";
	setAttr -k on ".li";
	setAttr -k on ".ls";
	setAttr -k on ".mb";
	setAttr -k on ".ti";
	setAttr -k on ".txt";
	setAttr -k on ".mpr";
	setAttr -k on ".wzd";
	setAttr -k on ".fn";
	setAttr -k on ".if";
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
	setAttr -k on ".as";
	setAttr -k on ".ds";
	setAttr -k on ".lm";
	setAttr -k on ".fir";
	setAttr -k on ".aap";
	setAttr -k on ".gh";
	setAttr -cb on ".sd";
select -ne :ikSystem;
	setAttr -av ".gsn";
connectAttr "keyboardMonster_Global_CTRL_translateX.o" "Grizzly_weighted3RN.phl[212]"
		;
connectAttr "keyboardMonster_Global_CTRL_translateY.o" "Grizzly_weighted3RN.phl[213]"
		;
connectAttr "keyboardMonster_Global_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[214]"
		;
connectAttr "keyboardMonster_Global_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[215]"
		;
connectAttr "keyboardMonster_Global_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[216]"
		;
connectAttr "keyboardMonster_Global_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[217]"
		;
connectAttr "keyboardMonster_Main_CTRL_translateX.o" "Grizzly_weighted3RN.phl[218]"
		;
connectAttr "keyboardMonster_Main_CTRL_translateY.o" "Grizzly_weighted3RN.phl[219]"
		;
connectAttr "keyboardMonster_Main_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[220]"
		;
connectAttr "keyboardMonster_Main_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[221]"
		;
connectAttr "keyboardMonster_Main_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[222]"
		;
connectAttr "keyboardMonster_Main_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[223]"
		;
connectAttr "keyboardMonster_root_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[224]"
		;
connectAttr "keyboardMonster_root_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[225]"
		;
connectAttr "keyboardMonster_root_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[226]"
		;
connectAttr "Grizzly_weighted3RN.phl[227]" "group1_pointConstraint1.tg[0].tpm";
connectAttr "Grizzly_weighted3RN.phl[228]" "group1_pointConstraint1.tg[0].tt";
connectAttr "keyboardMonster_root_JNT_CTRL_translateX.o" "Grizzly_weighted3RN.phl[229]"
		;
connectAttr "keyboardMonster_root_JNT_CTRL_translateY.o" "Grizzly_weighted3RN.phl[230]"
		;
connectAttr "keyboardMonster_root_JNT_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[231]"
		;
connectAttr "Grizzly_weighted3RN.phl[232]" "group1_pointConstraint1.tg[0].trp";
connectAttr "Grizzly_weighted3RN.phl[233]" "group1_pointConstraint1.tg[0].trt";
connectAttr "keyboardMonster_pelvis_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[234]"
		;
connectAttr "keyboardMonster_pelvis_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[235]"
		;
connectAttr "keyboardMonster_pelvis_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[236]"
		;
connectAttr "keyboardMonster_pelvis_JNT_CTRL_translateX.o" "Grizzly_weighted3RN.phl[237]"
		;
connectAttr "keyboardMonster_pelvis_JNT_CTRL_translateY.o" "Grizzly_weighted3RN.phl[238]"
		;
connectAttr "keyboardMonster_pelvis_JNT_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[239]"
		;
connectAttr "keyboardMonster_L_legadjust_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[240]"
		;
connectAttr "keyboardMonster_L_legadjust_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[241]"
		;
connectAttr "keyboardMonster_L_legadjust_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[242]"
		;
connectAttr "keyboardMonster_L_legadjust_JNT_CTRL_translateX.o" "Grizzly_weighted3RN.phl[243]"
		;
connectAttr "keyboardMonster_L_legadjust_JNT_CTRL_translateY.o" "Grizzly_weighted3RN.phl[244]"
		;
connectAttr "keyboardMonster_L_legadjust_JNT_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[245]"
		;
connectAttr "keyboardMonster_R_legadjust_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[246]"
		;
connectAttr "keyboardMonster_R_legadjust_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[247]"
		;
connectAttr "keyboardMonster_R_legadjust_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[248]"
		;
connectAttr "keyboardMonster_R_legadjust_JNT_CTRL_translateX.o" "Grizzly_weighted3RN.phl[249]"
		;
connectAttr "keyboardMonster_R_legadjust_JNT_CTRL_translateY.o" "Grizzly_weighted3RN.phl[250]"
		;
connectAttr "keyboardMonster_R_legadjust_JNT_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[251]"
		;
connectAttr "keyboardMonster_spine01_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[252]"
		;
connectAttr "keyboardMonster_spine01_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[253]"
		;
connectAttr "keyboardMonster_spine01_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[254]"
		;
connectAttr "keyboardMonster_spine01_JNT_CTRL_translateX.o" "Grizzly_weighted3RN.phl[255]"
		;
connectAttr "keyboardMonster_spine01_JNT_CTRL_translateY.o" "Grizzly_weighted3RN.phl[256]"
		;
connectAttr "keyboardMonster_spine01_JNT_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[257]"
		;
connectAttr "keyboardMonster_spine02_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[258]"
		;
connectAttr "keyboardMonster_spine02_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[259]"
		;
connectAttr "keyboardMonster_spine02_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[260]"
		;
connectAttr "keyboardMonster_spine02_JNT_CTRL_translateX.o" "Grizzly_weighted3RN.phl[261]"
		;
connectAttr "keyboardMonster_spine02_JNT_CTRL_translateY.o" "Grizzly_weighted3RN.phl[262]"
		;
connectAttr "keyboardMonster_spine02_JNT_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[263]"
		;
connectAttr "keyboardMonster_spine03_JNT_CTRL_translateX.o" "Grizzly_weighted3RN.phl[264]"
		;
connectAttr "keyboardMonster_spine03_JNT_CTRL_translateY.o" "Grizzly_weighted3RN.phl[265]"
		;
connectAttr "keyboardMonster_spine03_JNT_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[266]"
		;
connectAttr "keyboardMonster_spine03_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[267]"
		;
connectAttr "keyboardMonster_spine03_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[268]"
		;
connectAttr "keyboardMonster_spine03_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[269]"
		;
connectAttr "keyboardMonster_neck_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[270]"
		;
connectAttr "keyboardMonster_neck_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[271]"
		;
connectAttr "keyboardMonster_neck_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[272]"
		;
connectAttr "keyboardMonster_neck_JNT_CTRL_translateX.o" "Grizzly_weighted3RN.phl[273]"
		;
connectAttr "keyboardMonster_neck_JNT_CTRL_translateY.o" "Grizzly_weighted3RN.phl[274]"
		;
connectAttr "keyboardMonster_neck_JNT_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[275]"
		;
connectAttr "keyboardMonster_head_JNT_CTRL_translateX.o" "Grizzly_weighted3RN.phl[276]"
		;
connectAttr "keyboardMonster_head_JNT_CTRL_translateY.o" "Grizzly_weighted3RN.phl[277]"
		;
connectAttr "keyboardMonster_head_JNT_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[278]"
		;
connectAttr "keyboardMonster_head_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[279]"
		;
connectAttr "keyboardMonster_head_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[280]"
		;
connectAttr "keyboardMonster_head_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[281]"
		;
connectAttr "keyboardMonster_Facial_CTRL_ShowFacial.o" "Grizzly_weighted3RN.phl[282]"
		;
connectAttr "keyboardMonster_Facial_CTRL_PrimaryFacial.o" "Grizzly_weighted3RN.phl[283]"
		;
connectAttr "keyboardMonster_Facial_CTRL_SecondaryFacial.o" "Grizzly_weighted3RN.phl[284]"
		;
connectAttr "keyboardMonster_Facial_CTRL_Nose.o" "Grizzly_weighted3RN.phl[285]";
connectAttr "keyboardMonster_Facial_CTRL_Tongue.o" "Grizzly_weighted3RN.phl[286]"
		;
connectAttr "keyboardMonster_Facial_CTRL_Ear.o" "Grizzly_weighted3RN.phl[287]";
connectAttr "keyboardMonster_Facial_CTRL_Teeth.o" "Grizzly_weighted3RN.phl[288]"
		;
connectAttr "keyboardMonster_L_Clavicle_JNT_CTRL_translateX.o" "Grizzly_weighted3RN.phl[289]"
		;
connectAttr "keyboardMonster_L_Clavicle_JNT_CTRL_translateY.o" "Grizzly_weighted3RN.phl[290]"
		;
connectAttr "keyboardMonster_L_Clavicle_JNT_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[291]"
		;
connectAttr "keyboardMonster_L_Clavicle_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[292]"
		;
connectAttr "keyboardMonster_L_Clavicle_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[293]"
		;
connectAttr "keyboardMonster_L_Clavicle_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[294]"
		;
connectAttr "keyboardMonster_R_Clavicle_JNT_CTRL_translateX.o" "Grizzly_weighted3RN.phl[295]"
		;
connectAttr "keyboardMonster_R_Clavicle_JNT_CTRL_translateY.o" "Grizzly_weighted3RN.phl[296]"
		;
connectAttr "keyboardMonster_R_Clavicle_JNT_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[297]"
		;
connectAttr "keyboardMonster_R_Clavicle_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[298]"
		;
connectAttr "keyboardMonster_R_Clavicle_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[299]"
		;
connectAttr "keyboardMonster_R_Clavicle_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[300]"
		;
connectAttr "keyboardMonster_L_pectoral_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[301]"
		;
connectAttr "keyboardMonster_L_pectoral_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[302]"
		;
connectAttr "keyboardMonster_L_pectoral_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[303]"
		;
connectAttr "keyboardMonster_R_pectoral_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[304]"
		;
connectAttr "keyboardMonster_R_pectoral_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[305]"
		;
connectAttr "keyboardMonster_R_pectoral_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[306]"
		;
connectAttr "keyboardMonster_R_knee_CTRL_ToWorld.o" "Grizzly_weighted3RN.phl[307]"
		;
connectAttr "keyboardMonster_R_knee_CTRL_translateX.o" "Grizzly_weighted3RN.phl[308]"
		;
connectAttr "keyboardMonster_R_knee_CTRL_translateY.o" "Grizzly_weighted3RN.phl[309]"
		;
connectAttr "keyboardMonster_R_knee_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[310]"
		;
connectAttr "keyboardMonster_L_knee_CTRL_ToWorld.o" "Grizzly_weighted3RN.phl[311]"
		;
connectAttr "keyboardMonster_L_knee_CTRL_translateX.o" "Grizzly_weighted3RN.phl[312]"
		;
connectAttr "keyboardMonster_L_knee_CTRL_translateY.o" "Grizzly_weighted3RN.phl[313]"
		;
connectAttr "keyboardMonster_L_knee_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[314]"
		;
connectAttr "keyboardMonster_L_Foot_IKFK_SWITCH_CTRL_FK_IK.o" "Grizzly_weighted3RN.phl[315]"
		;
connectAttr "keyboardMonster_R_Foot_IKFK_SWITCH_CTRL_FK_IK.o" "Grizzly_weighted3RN.phl[316]"
		;
connectAttr "keyboardMonster_L_elbow_CTRL_ToWorld.o" "Grizzly_weighted3RN.phl[317]"
		;
connectAttr "keyboardMonster_L_elbow_CTRL_translateX.o" "Grizzly_weighted3RN.phl[318]"
		;
connectAttr "keyboardMonster_L_elbow_CTRL_translateY.o" "Grizzly_weighted3RN.phl[319]"
		;
connectAttr "keyboardMonster_L_elbow_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[320]"
		;
connectAttr "keyboardMonster_L_Wrist_IKFK_SWITCH_CTRL_FK_IK.o" "Grizzly_weighted3RN.phl[321]"
		;
connectAttr "keyboardMonster_R_Wrist_IKFK_SWITCH_CTRL_FK_IK.o" "Grizzly_weighted3RN.phl[322]"
		;
connectAttr "keyboardMonster_L_index01_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[323]"
		;
connectAttr "keyboardMonster_L_index01_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[324]"
		;
connectAttr "keyboardMonster_L_index01_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[325]"
		;
connectAttr "keyboardMonster_L_index02_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[326]"
		;
connectAttr "keyboardMonster_L_index02_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[327]"
		;
connectAttr "keyboardMonster_L_index02_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[328]"
		;
connectAttr "keyboardMonster_L_middle01_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[329]"
		;
connectAttr "keyboardMonster_L_middle01_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[330]"
		;
connectAttr "keyboardMonster_L_middle01_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[331]"
		;
connectAttr "keyboardMonster_L_middle02_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[332]"
		;
connectAttr "keyboardMonster_L_middle02_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[333]"
		;
connectAttr "keyboardMonster_L_middle02_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[334]"
		;
connectAttr "keyboardMonster_L_pinky01_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[335]"
		;
connectAttr "keyboardMonster_L_pinky01_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[336]"
		;
connectAttr "keyboardMonster_L_pinky01_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[337]"
		;
connectAttr "keyboardMonster_L_pinky02_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[338]"
		;
connectAttr "keyboardMonster_L_pinky02_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[339]"
		;
connectAttr "keyboardMonster_L_pinky02_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[340]"
		;
connectAttr "keyboardMonster_R_pinky01_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[341]"
		;
connectAttr "keyboardMonster_R_pinky01_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[342]"
		;
connectAttr "keyboardMonster_R_pinky01_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[343]"
		;
connectAttr "keyboardMonster_R_pinky02_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[344]"
		;
connectAttr "keyboardMonster_R_pinky02_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[345]"
		;
connectAttr "keyboardMonster_R_pinky02_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[346]"
		;
connectAttr "keyboardMonster_R_middle01_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[347]"
		;
connectAttr "keyboardMonster_R_middle01_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[348]"
		;
connectAttr "keyboardMonster_R_middle01_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[349]"
		;
connectAttr "keyboardMonster_R_middle02_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[350]"
		;
connectAttr "keyboardMonster_R_middle02_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[351]"
		;
connectAttr "keyboardMonster_R_middle02_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[352]"
		;
connectAttr "keyboardMonster_R_index01_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[353]"
		;
connectAttr "keyboardMonster_R_index01_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[354]"
		;
connectAttr "keyboardMonster_R_index01_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[355]"
		;
connectAttr "keyboardMonster_R_index02_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[356]"
		;
connectAttr "keyboardMonster_R_index02_JNT_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[357]"
		;
connectAttr "keyboardMonster_R_index02_JNT_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[358]"
		;
connectAttr "keyboardMonster_L_twistForearm_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[359]"
		;
connectAttr "keyboardMonster_R_twistForearm_JNT_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[360]"
		;
connectAttr "keyboardMonster_R_elbow_CTRL_ToWorld.o" "Grizzly_weighted3RN.phl[361]"
		;
connectAttr "keyboardMonster_R_elbow_CTRL_translateX.o" "Grizzly_weighted3RN.phl[362]"
		;
connectAttr "keyboardMonster_R_elbow_CTRL_translateY.o" "Grizzly_weighted3RN.phl[363]"
		;
connectAttr "keyboardMonster_R_elbow_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[364]"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_Heel_Roll.o" "Grizzly_weighted3RN.phl[365]"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_Ball_Roll.o" "Grizzly_weighted3RN.phl[366]"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_ToeTip_Roll.o" "Grizzly_weighted3RN.phl[367]"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_Toe_Wiggle.o" "Grizzly_weighted3RN.phl[368]"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_Knee_Twist.o" "Grizzly_weighted3RN.phl[369]"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_translateX.o" "Grizzly_weighted3RN.phl[370]"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_translateY.o" "Grizzly_weighted3RN.phl[371]"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[372]"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[373]"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[374]"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[375]"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_Heel_Roll.o" "Grizzly_weighted3RN.phl[376]"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_Ball_Roll.o" "Grizzly_weighted3RN.phl[377]"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_ToeTip_Roll.o" "Grizzly_weighted3RN.phl[378]"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_Toe_Wiggle.o" "Grizzly_weighted3RN.phl[379]"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_Knee_Twist.o" "Grizzly_weighted3RN.phl[380]"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_translateX.o" "Grizzly_weighted3RN.phl[381]"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_translateY.o" "Grizzly_weighted3RN.phl[382]"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[383]"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[384]"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[385]"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[386]"
		;
connectAttr "keyboardMonster_R_Wrist_IK_CTRL_ToSpine.o" "Grizzly_weighted3RN.phl[387]"
		;
connectAttr "keyboardMonster_R_Wrist_IK_CTRL_translateX.o" "Grizzly_weighted3RN.phl[388]"
		;
connectAttr "keyboardMonster_R_Wrist_IK_CTRL_translateY.o" "Grizzly_weighted3RN.phl[389]"
		;
connectAttr "keyboardMonster_R_Wrist_IK_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[390]"
		;
connectAttr "keyboardMonster_R_Wrist_IK_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[391]"
		;
connectAttr "keyboardMonster_R_Wrist_IK_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[392]"
		;
connectAttr "keyboardMonster_R_Wrist_IK_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[393]"
		;
connectAttr "keyboardMonster_L_Wrist_IK_CTRL_ToSpine.o" "Grizzly_weighted3RN.phl[394]"
		;
connectAttr "keyboardMonster_L_Wrist_IK_CTRL_translateX.o" "Grizzly_weighted3RN.phl[395]"
		;
connectAttr "keyboardMonster_L_Wrist_IK_CTRL_translateY.o" "Grizzly_weighted3RN.phl[396]"
		;
connectAttr "keyboardMonster_L_Wrist_IK_CTRL_translateZ.o" "Grizzly_weighted3RN.phl[397]"
		;
connectAttr "keyboardMonster_L_Wrist_IK_CTRL_rotateX.o" "Grizzly_weighted3RN.phl[398]"
		;
connectAttr "keyboardMonster_L_Wrist_IK_CTRL_rotateY.o" "Grizzly_weighted3RN.phl[399]"
		;
connectAttr "keyboardMonster_L_Wrist_IK_CTRL_rotateZ.o" "Grizzly_weighted3RN.phl[400]"
		;
connectAttr "polyPlane1.out" "pPlaneShape1.i";
connectAttr "group1_pointConstraint1.ctz" "group1.tz";
connectAttr "group1_pointConstraint1.w0" "group1_pointConstraint1.tg[0].tw";
connectAttr "group1.pim" "group1_pointConstraint1.cpim";
connectAttr "group1.rp" "group1_pointConstraint1.crp";
connectAttr "group1.rpt" "group1_pointConstraint1.crt";
connectAttr ":mentalrayGlobals.msg" ":mentalrayItemsList.glb";
connectAttr ":miDefaultOptions.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":miDefaultFramebuffer.msg" ":mentalrayItemsList.fb" -na;
connectAttr ":miDefaultOptions.msg" ":mentalrayGlobals.opt";
connectAttr ":miDefaultFramebuffer.msg" ":mentalrayGlobals.fb";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "sharedReferenceNode.sr" "Grizzly_weighted3RN.sr";
connectAttr ":time1.o" "xgmRefreshPreview.tim";
connectAttr "hyperView1.msg" "nodeEditorPanel1Info.b[0]";
connectAttr "hyperLayout1.msg" "hyperView1.hl";
connectAttr "keyboardMonster_Global_CTRL_translateX.msg" "hyperLayout1.hyp[0].dn"
		;
connectAttr "keyboardMonster_Global_CTRL_translateY.msg" "hyperLayout1.hyp[1].dn"
		;
connectAttr "keyboardMonster_Global_CTRL_translateZ.msg" "hyperLayout1.hyp[2].dn"
		;
connectAttr "keyboardMonster_Main_CTRL_translateX.msg" "hyperLayout1.hyp[3].dn";
connectAttr "keyboardMonster_Main_CTRL_translateY.msg" "hyperLayout1.hyp[4].dn";
connectAttr "keyboardMonster_Main_CTRL_translateZ.msg" "hyperLayout1.hyp[5].dn";
connectAttr "keyboardMonster_L_elbow_CTRL_translateX.msg" "hyperLayout1.hyp[12].dn"
		;
connectAttr "keyboardMonster_L_elbow_CTRL_translateY.msg" "hyperLayout1.hyp[13].dn"
		;
connectAttr "keyboardMonster_L_elbow_CTRL_translateZ.msg" "hyperLayout1.hyp[14].dn"
		;
connectAttr "keyboardMonster_L_knee_CTRL_translateX.msg" "hyperLayout1.hyp[15].dn"
		;
connectAttr "keyboardMonster_L_knee_CTRL_translateY.msg" "hyperLayout1.hyp[16].dn"
		;
connectAttr "keyboardMonster_L_knee_CTRL_translateZ.msg" "hyperLayout1.hyp[17].dn"
		;
connectAttr "keyboardMonster_R_elbow_CTRL_translateX.msg" "hyperLayout1.hyp[18].dn"
		;
connectAttr "keyboardMonster_R_elbow_CTRL_translateY.msg" "hyperLayout1.hyp[19].dn"
		;
connectAttr "keyboardMonster_R_elbow_CTRL_translateZ.msg" "hyperLayout1.hyp[20].dn"
		;
connectAttr "keyboardMonster_R_knee_CTRL_translateX.msg" "hyperLayout1.hyp[21].dn"
		;
connectAttr "keyboardMonster_R_knee_CTRL_translateY.msg" "hyperLayout1.hyp[22].dn"
		;
connectAttr "keyboardMonster_R_knee_CTRL_translateZ.msg" "hyperLayout1.hyp[23].dn"
		;
connectAttr "keyboardMonster_pelvis_JNT_CTRL_translateX.msg" "hyperLayout1.hyp[33].dn"
		;
connectAttr "keyboardMonster_pelvis_JNT_CTRL_translateY.msg" "hyperLayout1.hyp[34].dn"
		;
connectAttr "keyboardMonster_pelvis_JNT_CTRL_translateZ.msg" "hyperLayout1.hyp[35].dn"
		;
connectAttr "keyboardMonster_L_legadjust_JNT_CTRL_translateX.msg" "hyperLayout1.hyp[36].dn"
		;
connectAttr "keyboardMonster_L_legadjust_JNT_CTRL_translateY.msg" "hyperLayout1.hyp[37].dn"
		;
connectAttr "keyboardMonster_L_legadjust_JNT_CTRL_translateZ.msg" "hyperLayout1.hyp[38].dn"
		;
connectAttr "keyboardMonster_R_legadjust_JNT_CTRL_translateX.msg" "hyperLayout1.hyp[39].dn"
		;
connectAttr "keyboardMonster_R_legadjust_JNT_CTRL_translateY.msg" "hyperLayout1.hyp[40].dn"
		;
connectAttr "keyboardMonster_R_legadjust_JNT_CTRL_translateZ.msg" "hyperLayout1.hyp[41].dn"
		;
connectAttr "keyboardMonster_spine01_JNT_CTRL_translateX.msg" "hyperLayout1.hyp[42].dn"
		;
connectAttr "keyboardMonster_spine01_JNT_CTRL_translateY.msg" "hyperLayout1.hyp[43].dn"
		;
connectAttr "keyboardMonster_spine01_JNT_CTRL_translateZ.msg" "hyperLayout1.hyp[44].dn"
		;
connectAttr "keyboardMonster_spine02_JNT_CTRL_translateX.msg" "hyperLayout1.hyp[45].dn"
		;
connectAttr "keyboardMonster_spine02_JNT_CTRL_translateY.msg" "hyperLayout1.hyp[46].dn"
		;
connectAttr "keyboardMonster_spine02_JNT_CTRL_translateZ.msg" "hyperLayout1.hyp[47].dn"
		;
connectAttr "keyboardMonster_spine03_JNT_CTRL_translateX.msg" "hyperLayout1.hyp[48].dn"
		;
connectAttr "keyboardMonster_spine03_JNT_CTRL_translateY.msg" "hyperLayout1.hyp[49].dn"
		;
connectAttr "keyboardMonster_spine03_JNT_CTRL_translateZ.msg" "hyperLayout1.hyp[50].dn"
		;
connectAttr "keyboardMonster_L_Clavicle_JNT_CTRL_translateX.msg" "hyperLayout1.hyp[51].dn"
		;
connectAttr "keyboardMonster_L_Clavicle_JNT_CTRL_translateY.msg" "hyperLayout1.hyp[52].dn"
		;
connectAttr "keyboardMonster_L_Clavicle_JNT_CTRL_translateZ.msg" "hyperLayout1.hyp[53].dn"
		;
connectAttr "keyboardMonster_R_Clavicle_JNT_CTRL_translateX.msg" "hyperLayout1.hyp[54].dn"
		;
connectAttr "keyboardMonster_R_Clavicle_JNT_CTRL_translateY.msg" "hyperLayout1.hyp[55].dn"
		;
connectAttr "keyboardMonster_R_Clavicle_JNT_CTRL_translateZ.msg" "hyperLayout1.hyp[56].dn"
		;
connectAttr "keyboardMonster_neck_JNT_CTRL_translateX.msg" "hyperLayout1.hyp[57].dn"
		;
connectAttr "keyboardMonster_neck_JNT_CTRL_translateY.msg" "hyperLayout1.hyp[58].dn"
		;
connectAttr "keyboardMonster_neck_JNT_CTRL_translateZ.msg" "hyperLayout1.hyp[59].dn"
		;
connectAttr "keyboardMonster_head_JNT_CTRL_translateX.msg" "hyperLayout1.hyp[60].dn"
		;
connectAttr "keyboardMonster_head_JNT_CTRL_translateY.msg" "hyperLayout1.hyp[61].dn"
		;
connectAttr "keyboardMonster_head_JNT_CTRL_translateZ.msg" "hyperLayout1.hyp[62].dn"
		;
connectAttr "keyboardMonster_Global_CTRL_rotateX.msg" "hyperLayout1.hyp[63].dn";
connectAttr "keyboardMonster_Global_CTRL_rotateY.msg" "hyperLayout1.hyp[64].dn";
connectAttr "keyboardMonster_Global_CTRL_rotateZ.msg" "hyperLayout1.hyp[65].dn";
connectAttr "keyboardMonster_Main_CTRL_rotateX.msg" "hyperLayout1.hyp[66].dn";
connectAttr "keyboardMonster_Main_CTRL_rotateY.msg" "hyperLayout1.hyp[67].dn";
connectAttr "keyboardMonster_Main_CTRL_rotateZ.msg" "hyperLayout1.hyp[68].dn";
connectAttr "keyboardMonster_pelvis_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[72].dn"
		;
connectAttr "keyboardMonster_pelvis_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[73].dn"
		;
connectAttr "keyboardMonster_pelvis_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[74].dn"
		;
connectAttr "keyboardMonster_L_legadjust_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[75].dn"
		;
connectAttr "keyboardMonster_L_legadjust_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[76].dn"
		;
connectAttr "keyboardMonster_L_legadjust_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[77].dn"
		;
connectAttr "keyboardMonster_R_legadjust_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[78].dn"
		;
connectAttr "keyboardMonster_R_legadjust_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[79].dn"
		;
connectAttr "keyboardMonster_R_legadjust_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[80].dn"
		;
connectAttr "keyboardMonster_spine01_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[81].dn"
		;
connectAttr "keyboardMonster_spine01_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[82].dn"
		;
connectAttr "keyboardMonster_spine01_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[83].dn"
		;
connectAttr "keyboardMonster_spine02_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[84].dn"
		;
connectAttr "keyboardMonster_spine02_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[85].dn"
		;
connectAttr "keyboardMonster_spine02_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[86].dn"
		;
connectAttr "keyboardMonster_spine03_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[87].dn"
		;
connectAttr "keyboardMonster_spine03_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[88].dn"
		;
connectAttr "keyboardMonster_spine03_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[89].dn"
		;
connectAttr "keyboardMonster_neck_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[90].dn"
		;
connectAttr "keyboardMonster_neck_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[91].dn"
		;
connectAttr "keyboardMonster_neck_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[92].dn"
		;
connectAttr "keyboardMonster_head_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[93].dn"
		;
connectAttr "keyboardMonster_head_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[94].dn"
		;
connectAttr "keyboardMonster_head_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[95].dn"
		;
connectAttr "keyboardMonster_Facial_CTRL_ShowFacial.msg" "hyperLayout1.hyp[96].dn"
		;
connectAttr "keyboardMonster_Facial_CTRL_PrimaryFacial.msg" "hyperLayout1.hyp[97].dn"
		;
connectAttr "keyboardMonster_Facial_CTRL_SecondaryFacial.msg" "hyperLayout1.hyp[98].dn"
		;
connectAttr "keyboardMonster_Facial_CTRL_Nose.msg" "hyperLayout1.hyp[99].dn";
connectAttr "keyboardMonster_Facial_CTRL_Tongue.msg" "hyperLayout1.hyp[100].dn";
connectAttr "keyboardMonster_Facial_CTRL_Ear.msg" "hyperLayout1.hyp[101].dn";
connectAttr "keyboardMonster_Facial_CTRL_Teeth.msg" "hyperLayout1.hyp[102].dn";
connectAttr "keyboardMonster_L_Clavicle_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[103].dn"
		;
connectAttr "keyboardMonster_L_Clavicle_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[104].dn"
		;
connectAttr "keyboardMonster_L_Clavicle_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[105].dn"
		;
connectAttr "keyboardMonster_R_Clavicle_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[106].dn"
		;
connectAttr "keyboardMonster_R_Clavicle_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[107].dn"
		;
connectAttr "keyboardMonster_R_Clavicle_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[108].dn"
		;
connectAttr "keyboardMonster_L_pectoral_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[109].dn"
		;
connectAttr "keyboardMonster_L_pectoral_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[110].dn"
		;
connectAttr "keyboardMonster_L_pectoral_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[111].dn"
		;
connectAttr "keyboardMonster_R_pectoral_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[112].dn"
		;
connectAttr "keyboardMonster_R_pectoral_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[113].dn"
		;
connectAttr "keyboardMonster_R_pectoral_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[114].dn"
		;
connectAttr "keyboardMonster_R_knee_CTRL_ToWorld.msg" "hyperLayout1.hyp[115].dn"
		;
connectAttr "keyboardMonster_L_knee_CTRL_ToWorld.msg" "hyperLayout1.hyp[116].dn"
		;
connectAttr "keyboardMonster_L_Foot_IKFK_SWITCH_CTRL_FK_IK.msg" "hyperLayout1.hyp[117].dn"
		;
connectAttr "keyboardMonster_R_Foot_IKFK_SWITCH_CTRL_FK_IK.msg" "hyperLayout1.hyp[118].dn"
		;
connectAttr "keyboardMonster_L_elbow_CTRL_ToWorld.msg" "hyperLayout1.hyp[119].dn"
		;
connectAttr "keyboardMonster_L_Wrist_IKFK_SWITCH_CTRL_FK_IK.msg" "hyperLayout1.hyp[120].dn"
		;
connectAttr "keyboardMonster_R_Wrist_IKFK_SWITCH_CTRL_FK_IK.msg" "hyperLayout1.hyp[121].dn"
		;
connectAttr "keyboardMonster_L_index01_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[122].dn"
		;
connectAttr "keyboardMonster_L_index01_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[123].dn"
		;
connectAttr "keyboardMonster_L_index01_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[124].dn"
		;
connectAttr "keyboardMonster_L_index02_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[125].dn"
		;
connectAttr "keyboardMonster_L_index02_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[126].dn"
		;
connectAttr "keyboardMonster_L_index02_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[127].dn"
		;
connectAttr "keyboardMonster_L_middle01_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[128].dn"
		;
connectAttr "keyboardMonster_L_middle01_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[129].dn"
		;
connectAttr "keyboardMonster_L_middle01_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[130].dn"
		;
connectAttr "keyboardMonster_L_middle02_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[131].dn"
		;
connectAttr "keyboardMonster_L_middle02_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[132].dn"
		;
connectAttr "keyboardMonster_L_middle02_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[133].dn"
		;
connectAttr "keyboardMonster_L_pinky01_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[134].dn"
		;
connectAttr "keyboardMonster_L_pinky01_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[135].dn"
		;
connectAttr "keyboardMonster_L_pinky01_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[136].dn"
		;
connectAttr "keyboardMonster_L_pinky02_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[137].dn"
		;
connectAttr "keyboardMonster_L_pinky02_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[138].dn"
		;
connectAttr "keyboardMonster_L_pinky02_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[139].dn"
		;
connectAttr "keyboardMonster_R_pinky01_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[140].dn"
		;
connectAttr "keyboardMonster_R_pinky01_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[141].dn"
		;
connectAttr "keyboardMonster_R_pinky01_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[142].dn"
		;
connectAttr "keyboardMonster_R_pinky02_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[143].dn"
		;
connectAttr "keyboardMonster_R_pinky02_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[144].dn"
		;
connectAttr "keyboardMonster_R_pinky02_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[145].dn"
		;
connectAttr "keyboardMonster_R_middle01_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[146].dn"
		;
connectAttr "keyboardMonster_R_middle01_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[147].dn"
		;
connectAttr "keyboardMonster_R_middle01_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[148].dn"
		;
connectAttr "keyboardMonster_R_middle02_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[149].dn"
		;
connectAttr "keyboardMonster_R_middle02_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[150].dn"
		;
connectAttr "keyboardMonster_R_middle02_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[151].dn"
		;
connectAttr "keyboardMonster_R_index01_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[152].dn"
		;
connectAttr "keyboardMonster_R_index01_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[153].dn"
		;
connectAttr "keyboardMonster_R_index01_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[154].dn"
		;
connectAttr "keyboardMonster_R_index02_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[155].dn"
		;
connectAttr "keyboardMonster_R_index02_JNT_CTRL_rotateY.msg" "hyperLayout1.hyp[156].dn"
		;
connectAttr "keyboardMonster_R_index02_JNT_CTRL_rotateZ.msg" "hyperLayout1.hyp[157].dn"
		;
connectAttr "keyboardMonster_L_twistForearm_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[158].dn"
		;
connectAttr "keyboardMonster_R_twistForearm_JNT_CTRL_rotateX.msg" "hyperLayout1.hyp[159].dn"
		;
connectAttr "keyboardMonster_R_elbow_CTRL_ToWorld.msg" "hyperLayout1.hyp[160].dn"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_Heel_Roll.msg" "hyperLayout1.hyp[164].dn"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_Ball_Roll.msg" "hyperLayout1.hyp[165].dn"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_ToeTip_Roll.msg" "hyperLayout1.hyp[166].dn"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_Toe_Wiggle.msg" "hyperLayout1.hyp[167].dn"
		;
connectAttr "keyboardMonster_R_Foot_CTRL_Knee_Twist.msg" "hyperLayout1.hyp[168].dn"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_Heel_Roll.msg" "hyperLayout1.hyp[172].dn"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_Ball_Roll.msg" "hyperLayout1.hyp[173].dn"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_ToeTip_Roll.msg" "hyperLayout1.hyp[174].dn"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_Toe_Wiggle.msg" "hyperLayout1.hyp[175].dn"
		;
connectAttr "keyboardMonster_L_Foot_CTRL_Knee_Twist.msg" "hyperLayout1.hyp[176].dn"
		;
connectAttr "keyboardMonster_R_Wrist_IK_CTRL_ToSpine.msg" "hyperLayout1.hyp[180].dn"
		;
connectAttr "keyboardMonster_L_Wrist_IK_CTRL_ToSpine.msg" "hyperLayout1.hyp[184].dn"
		;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "pPlaneShape1.iog" ":initialShadingGroup.dsm" -na;
dataStructure -fmt "raw" -as "name=externalContentTable:string=node:string=key:string=upath:uint32=upathcrc:string=rpath:string=roles";
applyMetadata -fmt "raw" -v "channel\nname externalContentTable\nstream\nname v1.0\nindexType numeric\nstructure externalContentTable\n0\n\"Grizzly_weighted3RN\" \"\" \"S:/gaming/isv_desktop/AMD_EFFECTS/TressFX_Assets/Grizzly/Art/Mesh/t_Grizzly_weighted4.ma\" 3910442671 \"E:/Users/jstewart/Documents/Perforce/AMD/isv_desktop/AMD_EFFECTS/TressFX_3.0/Models/Bear Model/t_Grizzly_weighted4.ma\" \"FileRef\"\nendStream\nendChannel\nendAssociations\n" 
		-scn;
// End of grizzly_walkCycle.ma
