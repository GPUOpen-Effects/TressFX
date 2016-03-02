#To use this tool easily, you can create a shelf button using the following code

#  import TressFX_Exporter
#  reload(TressFX_Exporter)
#  TressFX_Exporter.UI()

import maya.cmds as cmds
import maya.OpenMaya as OpenMaya
import maya.mel as mel
import pymel.core as pymel
import ctypes
import random

selected_mesh_shape_name = ''
tressfx_exporter_version = '1.0.1'

class ProgressBar:
    def __init__ (self,title,steps):
        self.window = pymel.window(title, sizeable=False)
        pymel.columnLayout()

        self.progressControls = []
        self.progressbar = pymel.progressBar(maxValue=steps, width=300)
        pymel.showWindow( self.window )
        self.progressbar.step(0)

    def Kill(self):
        pymel.deleteUI(self.window)

    def Increment(self):
        self.progressbar.step(1)

def UI():
    if cmds.window("TressFXExporterUI", exists = True):
        cmds.deleteUI("TressFXExporterUI")

    windowTitle = 'TressFX Exporter' + ' ' + 'v' + tressfx_exporter_version
    window = cmds.window("TressFXExporterUI", title = windowTitle, w=280, h = 260, mnb=False, sizeable=False)
    mainLayout = cmds.columnLayout(w=280, h=260)

    cmds.separator(h=8, style='in')
    cmds.text(label='Number of vertices per strand (4, 8, 16, 32 or 64):', align='left')
    cmds.intField("numVerticesPerStrand", w=30, minValue=4, maxValue=64, value=16 )
    cmds.separator(h=8, style='in')
    cmds.checkBox("bothEndsImmovable", label='Both ends immovable')
    cmds.checkBox("InvertZ", label='Invert Z', value = True)
    cmds.checkBox("exportSkinCheckBox", label='Export skin data', value = False)
    cmds.checkBox("randomStrandCheckBox", label='Randomize strands for LOD', value = True)
    
    cmds.separator(h=15)
    
    cmds.button(label="Set the base mesh", w=170, h=30, command=SetBaseMesh)
    cmds.textField("MeshNameLabel", w=170, editable=False)
    
    cmds.separator(height=10, style='none')
    cmds.button(label="Export as binary (*.tfx)", w=170, h=30, command=ExportBinary)
    
    cmds.separator(h=15)
    
    version_text = 'v' + tressfx_exporter_version
    cmds.text(label=version_text, align='left')
    
    global selected_mesh_shape_name
    selected_mesh_shape_name = ''
    
    cmds.showWindow(window)

def SetBaseMesh(*args):
    #------------------------------
    # Find the selected mesh shape
    #-------------------------------
    cmds.textField("MeshNameLabel", edit=True, text='')
    sel_list = OpenMaya.MSelectionList()
    OpenMaya.MGlobal.getActiveSelectionList(sel_list)

    global selected_mesh_shape_name
    selected_mesh_shape_name = ''
    
    if sel_list.length() == 0:
        return
    
    dagPath = OpenMaya.MDagPath()
    sel_list.getDagPath(0, dagPath)
    dagPath.extendToShape()

    meshFn = OpenMaya.MFnMesh(dagPath)
    selected_mesh_shape_name = meshFn.name()
    cmds.textField("MeshNameLabel", edit=True, text=meshFn.name()) 
    return
        
def ExportBinary(*args):
    curves = GetSelectedNurbsCurves()    
    
    if len(curves) == 0:
        cmds.warning("No nurbs curves were selected!")
        return
        
    basicFilter = "*.tfx"
    filepath = cmds.fileDialog2(fileFilter=basicFilter, dialogStyle=2, caption="Save a tfx binary file(*.tfx)", fileMode=0)
    
    if filepath == None or len(filepath) == 0:
        return
    
    bRandomize = cmds.checkBox("randomStrandCheckBox", q = True, v = True)
    
    if bRandomize:
        random.shuffle(curves)
        
    rootPositions = SaveTFXBinaryFile(filepath[0], curves)
        
    #------------------------------
    # Find the selected mesh shape
    #------------------------------
    allObject = cmds.ls(selected_mesh_shape_name)
    cmds.select(allObject)
    
    sel_list = OpenMaya.MSelectionList()
    OpenMaya.MGlobal.getActiveSelectionList(sel_list)
    dagPath = OpenMaya.MDagPath()
    
    if sel_list.length() == 0:
        return
    
    sel_list.getDagPath(0, dagPath)
    dagPath.extendToShape()
    
    exportSkinCheckBox = cmds.checkBox("exportSkinCheckBox", q = True, v = True)
    
    if selected_mesh_shape_name == '' or exportSkinCheckBox == False:
        return
        
    #--------------------------------    
    # Get tfxskin file path from user
    #--------------------------------
    basicFilter_tfxskin = "*.tfxskin"
    filepath_tfxskin = cmds.fileDialog2(fileFilter=basicFilter_tfxskin, dialogStyle=2, caption="Save a tfxskin file(*.tfxskin)", fileMode=0)

    if filepath_tfxskin == None or len(filepath_tfxskin) == 0:
        return
    
    #---------------------------------------------------------------------------
    # Build a face/triangle index list to convert face index into triangle index
    #---------------------------------------------------------------------------
    faceIter = OpenMaya.MItMeshPolygon(dagPath)
    triangleCount = 0
    faceTriaIndexList = []
    index = 0

    util = OpenMaya.MScriptUtil()
    util.createFromInt(0)
    
    while not faceIter.isDone():
        faceTriaIndexList.append(triangleCount)
        
        if faceIter.hasValidTriangulation():
            numTrianglesPtr = util.asIntPtr()
            faceIter.numTriangles(numTrianglesPtr)
            numTriangles = util.getInt(numTrianglesPtr)        
            triangleCount += numTriangles

        faceIter.next()
   
    #----------------------
    # Find the closest face
    #----------------------
    meshFn = OpenMaya.MFnMesh(dagPath)
    meshIntersector = OpenMaya.MMeshIntersector()
    meshIntersector.create(dagPath.node())
    
    faceIdList = []
    baryCoordList = []
    uvCoordList = []
    progressBar = ProgressBar('Collecting skin data', len(rootPositions))
    
    for i in range(len(rootPositions)):
        rootPoint = rootPositions[i]

        # Find the closest point info
        meshPt = OpenMaya.MPointOnMesh()
        meshIntersector.getClosestPoint(rootPoint, meshPt)
        pt = meshPt.getPoint()
        
        pointOnMesh = OpenMaya.MPoint()
        pointOnMesh = pt
        
        # Find face index
        faceId = meshPt.faceIndex()
        
        # Find triangle index
        triangleId = faceTriaIndexList[faceId] + meshPt.triangleIndex()
        faceIdList.append(triangleId)
        
        # Find barycentric coordinates
        uPtr = OpenMaya.MScriptUtil().asFloatPtr()
        vPtr = OpenMaya.MScriptUtil().asFloatPtr()
        meshPt.getBarycentricCoords(uPtr,vPtr)
        uvw = OpenMaya.MPoint()
        uvw.x = OpenMaya.MScriptUtil(uPtr).asFloat()
        uvw.y = OpenMaya.MScriptUtil(vPtr).asFloat()
        uvw.z = 1.0 - uvw.x - uvw.y  
        baryCoordList.append(uvw)
        
        # Find UV coordinates 
        util = OpenMaya.MScriptUtil()
        util.createFromList([0.0, 0.0], 2)
        uv_ptr = util.asFloat2Ptr()
        meshFn.getUVAtPoint(rootPoint, uv_ptr)
        u = OpenMaya.MScriptUtil.getFloat2ArrayItem(uv_ptr, 0, 0)
        v = OpenMaya.MScriptUtil.getFloat2ArrayItem(uv_ptr, 0, 1)
        uv_coord = OpenMaya.MPoint()
        uv_coord.x = u
        uv_coord.y = v
        uv_coord.z = 0
        uvCoordList.append(uv_coord)

        # update progress gui
        progressBar.Increment()
        
    progressBar.Kill()
    
    SaveTFXSkinBinaryFile(filepath_tfxskin[0], faceIdList, baryCoordList, uvCoordList)
    return
           
           
def RecursiveSearchCurve(curves, objNode):
    if objNode.hasFn(OpenMaya.MFn.kNurbsCurve):
        curveFn = OpenMaya.MFnNurbsCurve(objNode)
        curves.append(curveFn)
    elif objNode.hasFn(OpenMaya.MFn.kTransform):
        objFn = OpenMaya.MFnTransform(objNode)
        
        for j in range(objFn.childCount()):
            childObjNode = objFn.child(j)
            RecursiveSearchCurve(curves, childObjNode)
            
def GetSelectedNurbsCurves():
    slist = OpenMaya.MSelectionList()
    OpenMaya.MGlobal.getActiveSelectionList( slist )
    iter = OpenMaya.MItSelectionList(slist)
    curves = []

    # Find all nurbs curves under the selected node recursively. 
    for i in range(slist.length()):
        selObj = OpenMaya.MObject()
        slist.getDependNode(i, selObj)
        RecursiveSearchCurve(curves, selObj)
        
    return curves    

class TressFXFileObject(ctypes.Structure):
    _fields_ = [('version', ctypes.c_float),
                ('numHairStrands', ctypes.c_uint),
                ('numVerticesPerStrand', ctypes.c_uint),
                ('followToGuideStrandRatio', ctypes.c_uint),
                ('bothEndsImmovable', ctypes.c_bool),
                ('reserved1', ctypes.c_uint * 31),
                ('verticesOffset', ctypes.c_uint),
                ('perStrandThicknessOffset', ctypes.c_uint),
                ('perStrandTexCoordOffset', ctypes.c_uint),
                ('perVertexColorOffset', ctypes.c_uint),
                ('guideHairStrandOffset', ctypes.c_uint),
                ('perVertexTexCoordOffset', ctypes.c_uint),
                ('reserved2', ctypes.c_uint * 32)]

class Point(ctypes.Structure):
    _fields_ = [('x', ctypes.c_float),
                ('y', ctypes.c_float),
                ('z', ctypes.c_float)]

def SaveTFXBinaryFile(filepath, curves):
    numCurves = len(curves)
    numVerticesPerStrand = cmds.intField("numVerticesPerStrand", q=True, value=True)
    bothEndsImmovable = cmds.checkBox("bothEndsImmovable",q = True, v = True)
    invertZ = cmds.checkBox("InvertZ",q = True, v = True)
     
    print bothEndsImmovable
     
    rootPositions = []

    tfxobj = TressFXFileObject()
    tfxobj.version = 3.0
    tfxobj.numHairStrands = numCurves
    tfxobj.numVerticesPerStrand = numVerticesPerStrand
    tfxobj.followToGuideStrandRatio = 0
    tfxobj.bothEndsImmovable = 1 if bothEndsImmovable else 0
    tfxobj.verticesOffset = ctypes.sizeof(TressFXFileObject)
    tfxobj.perStrandThicknessOffset = 0
    tfxobj.perStrandTexCoordOffset = 0
    tfxobj.perVertexColorOffset = 0
    tfxobj.guideHairStrandOffset = 0
    tfxobj.perVertexTexCoordOffset = 0
  
    progressBar = ProgressBar('Saving a tfx file', numCurves*numVerticesPerStrand)

    f = open(filepath, "wb")
    f.write(tfxobj)

    for i in xrange(numCurves):
        curveFn = curves[i]
        
        # getting Min/Max value of the nurbs curve
        min = OpenMaya.MScriptUtil()
        min.createFromDouble(0) 
        minPtr = min.asDoublePtr() 
        max = OpenMaya.MScriptUtil() 
        max.createFromDouble(0) 
        maxPtr = max.asDoublePtr() 
        curveFn.getKnotDomain(minPtr, maxPtr)        
        min_val = OpenMaya.MScriptUtil(minPtr).asDouble()
        max_val = OpenMaya.MScriptUtil(maxPtr).asDouble()

        for j in range(0, numVerticesPerStrand):
            param = j/ float(numVerticesPerStrand-1)       
            pos = OpenMaya.MPoint()
            
            param = param * (max_val - min_val)
            curveFn.getPointAtParam(param, pos, OpenMaya.MSpace.kObject)
            
            p = Point()
            p.x = pos.x
            p.y = pos.y
            
            if invertZ:
                p.z = -pos.z # flip in z-axis
            else:
                p.z = pos.z

            f.write(p)
            progressBar.Increment()

        # # find face index
        rootPos = OpenMaya.MPoint()
        curveFn.getPointAtParam(0, rootPos, OpenMaya.MSpace.kObject) # kWorld?
        rootPositions.append(rootPos)
                
    f.close()
    progressBar.Kill()
    
    return rootPositions
    
class TressFXSkinFileObject(ctypes.Structure):
    _fields_ = [('version', ctypes.c_uint),
                ('numHairs', ctypes.c_uint),
                ('numTriangles', ctypes.c_uint),
                ('reserved1', ctypes.c_uint * 31), 
                ('hairToMeshMap_Offset', ctypes.c_uint),
                ('perStrandUVCoordniate_Offset', ctypes.c_uint),
                ('reserved1', ctypes.c_uint * 31)]      
    
class HairToTriangleMapping(ctypes.Structure):
    _fields_ = [('mesh', ctypes.c_uint),
                ('triangle', ctypes.c_uint),
                ('barycentricCoord_x', ctypes.c_float),
                ('barycentricCoord_y', ctypes.c_float),
                ('barycentricCoord_z', ctypes.c_float),
                ('reserved', ctypes.c_uint)]    
    
def SaveTFXSkinBinaryFile(filepath, faceIdList, baryCoordList, uvCoordList): 
    tfxSkinObj = TressFXSkinFileObject()
    tfxSkinObj.version = 1
    tfxSkinObj.numHairs = len(faceIdList)
    tfxSkinObj.numTriangles = 0
    tfxSkinObj.hairToMeshMap_Offset = ctypes.sizeof(TressFXSkinFileObject)
    tfxSkinObj.perStrandUVCoordniate_Offset = tfxSkinObj.hairToMeshMap_Offset + len(faceIdList) * ctypes.sizeof(HairToTriangleMapping)
    
    f = open(filepath, "wb")
    f.write(tfxSkinObj)
    
    progressBar = ProgressBar('Saving a tfxskin file', len(faceIdList) + len(uvCoordList))
    
    for i in xrange(len(faceIdList)):
        mapping = HairToTriangleMapping()
        mapping.mesh = 0
        mapping.triangle = faceIdList[i]
        
        uvw = baryCoordList[i]
        mapping.barycentricCoord_x = uvw.x
        mapping.barycentricCoord_y = uvw.y
        mapping.barycentricCoord_z = uvw.z
        
        f.write(mapping)
        progressBar.Increment()
        
    # per strand uv coordinate
    for i in xrange(len(uvCoordList)):
        uv_coord = uvCoordList[i]
        p = Point()
        p.x = uv_coord.x
        p.y = 1.0 - uv_coord.y # DirectX has it inverted
        p.z = uv_coord.z 
        
        f.write(p)    
        progressBar.Increment()
    
    f.close()
    progressBar.Kill()
     
    return
