# 1. Copy tressFX_Exporter.py into Maya plug-in path. The easy place would be C:\Users\YOUR_USER_NAME\Documents\maya\plug-ins\
# 2. If you want to install TressFX menu, go to Windows->Plug-in Managers menu and check Loaded and Auto load for TressFX_Exporter.py. 
# 3. Then TressFX menu would appear in the top menu bar. 
# 4. Within it, there should be export menu item. Click it and it will bring up TressFX Exporter window. 
# 5. Alternatively, without loading TressFX plugin in Plug-in Managers, you can simply run the following python script commands in Script Editor
#    and bring up TressFX Exporter window. 
#
#  import TressFX_Exporter
#  reload(TressFX_Exporter)
#  TressFX_Exporter.UI()

import maya.cmds as cmds
import maya.OpenMaya as OpenMaya
import maya.OpenMayaAnim as OpenMayaAnim
import maya.OpenMayaMPx as OpenMayaMPx
import maya.mel as mel
import pymel.core as pymel
import ctypes
import random
import sys
from maya.OpenMaya import MIntArray, MDagPathArray


selected_mesh_shape_name = ''
tressfx_exporter_version = '4.0'

# Don't change the following maximum joints per vertex value. It must match the one in TressFX loader and simulation
TRESSFX_MAX_INFLUENTIAL_BONE_COUNT  = 4

# Following is a script to unload and load this plugin. It may be useful to reload the plugin quickly during the development. 
# import maya.cmds as cmds
# cmds.unloadPlugin('TressFX_Exporter.py')
# cmds.loadPlugin('TressFX_Exporter.py')

import TressFX_Exporter

bReload = True
if(bReload): 
    reload(TressFX_Exporter)

def InstallShelf():
    #----------------------
    # Add a shelf 
    #----------------------
    shelfName = "TressFX"
            
    layout = mel.eval('$tmp=$gShelfTopLevel')
    
    if cmds.shelfLayout(shelfName, query=True, exists=True): # If the shelf exists, delete buttons and delete shelf
        for buttons in cmds.shelfLayout(shelfName, query=True, childArray=True) or []:
            cmds.deleteUI(buttons) 
            
        cmds.setParent(layout + '|' + shelfName)
    else: # If the shelf doesn't exist, create a new one
        cmds.setParent(layout)
        cmds.shelfLayout(shelfName) 

    # create a button
    cmds.shelfButton( label='Export',
                    command='import TressFX_Exporter\nTressFX_Exporter.UI()',
                    #command='TressFX_Exporter.UI()',
                    sourceType='python',
                    annotation='Launch TressFX exporter',
                    #image='', # This empty icon image will cause a warning message like 'Pixmap file  not found, using default.'.  
                    style='textOnly')
    
                
def initializePlugin(mobject):
        mplugin = OpenMayaMPx.MFnPlugin(mobject, "TressFX", tressfx_exporter_version, "Any")
        
        # install menu
        gMainWindow = mel.eval('$temp1=$gMainWindow')
        
        if cmds.menu('TressFX', exists=True):
            cmds.deleteUI('TressFX')
        
        tressfx_top_menu = cmds.menu('TressFX', parent=gMainWindow, tearOff=False, label='TressFX')
        cmds.menuItem(parent = tressfx_top_menu, 
                      label='Export Hair/Fur', 
                      command = 'import TressFX_Exporter\nTressFX_Exporter.UI()')
        cmds.menuItem(parent = tressfx_top_menu, 
                      label='Export Collision Mesh', 
                      command = 'import TressFX_Exporter\nTressFX_Exporter.CollisionUI()')
        # install shelf
        #InstallShelf() 

def uninitializePlugin(mobject): 
        mplugin = OpenMayaMPx.MFnPlugin(mobject)
        try:
            # Close TressFX plugin windows if it is already open. 
            if cmds.window("TressFXExporterUI", exists = True):
                cmds.deleteUI("TressFXExporterUI") 

            if cmds.window("TressFXCollisionMeshUI", exists = True):
                cmds.deleteUI("TressFXCollisionMeshUI") 
        
            # uninstall menu
            if cmds.menu('TressFX', exists=True):
                cmds.deleteUI('TressFX') 
                
        except:
                sys.stderr.write("Failed to uninitialize TressFX plugin")
                raise
                
        return
        
# Helper class to show progress bar for lengthy process
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
    # prevents multiple windows
    if cmds.window("TressFXExporterUI", exists = True):
        cmds.deleteUI("TressFXExporterUI") 

    window_width = 255
    window_height = 320
    
    windowTitle = 'TressFX Hair/Fur'
    window = cmds.window("TressFXExporterUI", title = windowTitle, w=window_width, h = window_height, mnb=False, sizeable=False)
    
    mainLayout = cmds.columnLayout() # In case you want to see the area of the main layout, use backgroundColor = (1, 1, 1) as an argument
    cmds.separator(h=10)

    cmds.rowColumnLayout( numberOfColumns=2, columnWidth=[ (1,10),(2,window_width-10) ])
    cmds.separator(style='none', width=10)
    
    cmds.rowColumnLayout( numberOfColumns=3, columnWidth=[ (1,160),(2,10),(3,60) ] )
    
    #cmds.intField("numVerticesPerStrand", w=30, minValue=4, maxValue=64, value=16 )
    
    cmds.text(label='Number of vertices per strand:', align='left')    
    cmds.separator(style='none', width=10)
    
    cmds.optionMenu("numberOfStrandsOptionMenu", label='')
    cmds.menuItem(label='4')
    cmds.menuItem(label='8')
    cmds.menuItem(label='16')
    cmds.menuItem(label='32')
    cmds.menuItem(label='64')

    cmds.text(label='Minimum curve length:', align='left')
    cmds.floatField("minCurveLength", minValue=0, value=0, w=70)
    cmds.setParent('..')
    cmds.separator(style='none',h=10)
    
    cmds.columnLayout()
    cmds.checkBox("bothEndsImmovable", label='Both ends immovable')
    cmds.separator (style='none', width=10)
    cmds.checkBox("InvertZ", label='Invert Z-axis of Hairs', value = False)
    cmds.checkBox("InvertYForUVs", label='Invert Y-axis of UV coordinates', value = False)
    cmds.checkBox("randomStrandCheckBox", label='Randomize strands for LOD', value = True)
    
    cmds.separator(style='none',h=10)
    
    cmds.checkBox("exportHairCheckBox", label='Export hair data (*.tfx)', value = True)
    cmds.checkBox("exportSkinCheckBox", label='Export skin data (*.tfxskin)', value = False)
    cmds.checkBox("exportBoneCheckBox", label='Export bone data (*.tfxbone)', value = False)
    
    cmds.separator(style='none',h=15)
    
    cmds.button(label="Set the base mesh", w=120, h=25, command=SetBaseMesh)
    cmds.textField("MeshNameLabel", w=220, editable=False)
    
    cmds.separator(style='none',h=15)
    cmds.button(label="Export!", w=170, h=30, command=DoExport)
    
    #cmds.separator(style='none',h=15)
    #cmds.button(label="Export collision mesh", w=170, h=30, command=DoExportCollisionMesh)
    
    #cmds.button(label="Goto Bind Pose", w=220, h=30, command=GotoBindPose)
    
    cmds.separator(h=7)
    
    version_text = 'v' + tressfx_exporter_version
    cmds.text(label=version_text, width=window_width-25, align='right')
    
    global selected_mesh_shape_name
    selected_mesh_shape_name = ''
    
    cmds.showWindow(window)
    
    # After showWindow, resize the window to enforce its size. This is a workaround of Maya's bug. 
    cmds.window(window, edit=True, widthHeight=(window_width, window_height))
    
def CollisionUI():
    # prevents multiple windows
    if cmds.window("TressFXCollisionMeshUI", exists = True):
        cmds.deleteUI("TressFXExporterUI") 

    window_width = 255
    window_height = 150
    
    windowTitle = 'TressFX Collision'
    window = cmds.window("TressFXCollisionMeshUI", title = windowTitle, w=window_width, h = window_height, mnb=False, sizeable=False)
    
    cmds.rowColumnLayout( numberOfColumns=3, columnWidth=[ (1,10),(2,window_width-20), (3, 10) ], rowSpacing = (1,10))

    cmds.separator(style='none', width=10)
    cmds.separator(style='none', width=10)
    cmds.separator(style='none', width=10)
    
    cmds.separator(style='none', width=10)
    cmds.button(label="Set the collision mesh", w=120, h=25, command=SetBaseMesh)
    cmds.separator(style='none', width=10)

    cmds.separator(style='none', width=10)
    cmds.textField("MeshNameLabel", w=220, editable=False)
    cmds.separator(style='none', width=10)    
    cmds.separator(style='none', width=10)

    cmds.button(label="Export", w=170, h=30, command=DoExportCollisionMesh)
    cmds.separator(style='none', width=10)    
    version_text = 'v' + tressfx_exporter_version
    cmds.text(label=version_text, width=window_width-25, align='right')
    
    global selected_mesh_shape_name
    selected_mesh_shape_name = ''
    
    cmds.showWindow(window)
    
    # After showWindow, resize the window to enforce its size. This is a workaround of Maya's bug. 
    cmds.window(window, edit=True, widthHeight=(window_width, window_height))

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
        
def DoExport(*args):
    # TODO: Set the time frame to the first one or go to the bind pose? 
    # first_frame = cmds.playbackOptions(minTime=True, query=True)
    # cmds.currentTime(first_frame)

    bExportHairCheckBox = cmds.checkBox("exportHairCheckBox", q = True, v = True)
    bExportSkinCheckBox = cmds.checkBox("exportSkinCheckBox", q = True, v = True)
    bExportBoneCheckBox = cmds.checkBox("exportBoneCheckBox", q = True, v = True)
    
    #----------------------------------------
    # collect selected nurbs spline curves.
    #----------------------------------------
    minCurveLength = cmds.floatField("minCurveLength",q = True, v = True)
    #print "minCurveLength: %f\n" % minCurveLength
    curves = GetSelectedNurbsCurves(minCurveLength)
    
    if len(curves) == 0:
        cmds.warning("No nurbs curves were selected!")
        return
    else:
        print "TressFX: %d curves were selected.\n" % len(curves)
        
    #-------------------------------
    # Find the selected mesh shape
    #-------------------------------
    meshShapedagPath = OpenMaya.MDagPath()

    if selected_mesh_shape_name == '':
        meshShapedagPath = None
    else:
        allObject = cmds.ls(selected_mesh_shape_name)
        cmds.select(allObject) # TODO: This selection makes hair curves unselected. This is not a problem but just inconvenient for users if they want to keep the curves selected.  
        
        sel_list = OpenMaya.MSelectionList()
        OpenMaya.MGlobal.getActiveSelectionList(sel_list)
        
        if sel_list.length() == 0:
            meshShapedagPath = None
        else:
            sel_list.getDagPath(0, meshShapedagPath)
            meshShapedagPath.extendToShape() # get mesh shape

    # if none of export checkboxes were selected, then exit.     
    if bExportHairCheckBox == 0 and bExportSkinCheckBox == 0 and bExportBoneCheckBox == 0:
        cmds.warning("Please select checkbox for exporting data")
        return 
    
    
        
    rootPositions = []
    
    #-------------------
    # Export a tfx file
    #-------------------
    if bExportHairCheckBox:    
        basicFilter = "*.tfx"
        filepath = cmds.fileDialog2(fileFilter=basicFilter, dialogStyle=2, caption="Save a tfx binary file(*.tfx)", fileMode=0)
        
        if filepath == None or len(filepath) == 0:
            return
        
        bRandomize = cmds.checkBox("randomStrandCheckBox", q = True, v = True)
        
        if bRandomize:
            random.shuffle(curves)
            
        rootPositions = SaveTFXBinaryFile(filepath[0], curves, meshShapedagPath)
    
    #------------------------------------------------------------------------------------
    # collect root positions for tfxskin or tfxbone in case SaveTFXBinaryFile was not run
    #------------------------------------------------------------------------------------
    if bExportHairCheckBox == 0 and (bExportSkinCheckBox == 1 or bExportBoneCheckBox == 1):
        for i in xrange(len(curves)):
            curveFn = curves[i]
            rootPos = OpenMaya.MPoint()
            curveFn.getPointAtParam(0, rootPos, OpenMaya.MSpace.kObject) # kWorld?
            rootPositions.append(rootPos)
     
    if bExportSkinCheckBox == 1 or bExportBoneCheckBox == 1:
        if selected_mesh_shape_name == '':
            cmds.warning("To export skin or bone data, base mesh must be set.\n")
            return
            
    #--------------------    
    # Save tfxskin file
    #--------------------
    if bExportSkinCheckBox == 1:
        basicFilter_tfxskin = "*.tfxskin"
        filepath = cmds.fileDialog2(fileFilter=basicFilter_tfxskin, dialogStyle=2, caption="Save a tfxskin file(*.tfxskin)", fileMode=0)

        if filepath == None or len(filepath) == 0:
            return
        
        SaveTFXSkinBinaryFile(filepath[0], meshShapedagPath, rootPositions)
        
    #------------------------
    # Save the tfxbone file.
    #------------------------
    if bExportBoneCheckBox == 1:
        basicFilter = "*.tfxbone"
        filepath = cmds.fileDialog2(fileFilter=basicFilter, dialogStyle=2, caption="Save a tfx bone file(*.tfxbone)", fileMode=0)
        
        if filepath == None or len(filepath) == 0:
            return
        
        SaveTFXBoneBinaryFile(filepath[0], selected_mesh_shape_name, meshShapedagPath, rootPositions)
        
    return
           
# TODO: Do we need to enforce to go to the bind pose or let user do it? 
def GotoBindPose(*args):
    joints = cmds.ls(type='joint')
    rootJoints = []

    for j in joints:
        while cmds.listRelatives(j, p=True):
            parent = cmds.listRelatives(j, p=True)
            j = parent[0]
        rootJoints.append(j)

    rootJoints = set(rootJoints)

    for rootJoint in rootJoints:
        cmds.select(rootJoint)
        bindpose = cmds.dagPose(q=True, bindPose=True)
        cmds.dagPose(bindpose[0] , restore=True)    
        cmds.select(cl=True)
        
    return    
        
class WeightJointIndexPair:
    weight = 0
    joint_index = -1
    
    # For sorting 
    def __lt__(self, other):
        return self.weight > other.weight
        
# vertexIndices is three vertex indices belong to one triangle
def GetSortedWeightsFromTriangleVertices(_maxJointsPerVertex, vertexIndices, jointIndexArray, weightArray, baryCoord):
    final_pairs = []
    
    for j in range(_maxJointsPerVertex):
        for i in range(3):
            vertex_index = vertexIndices[i]
            bary = baryCoord[i]
            
            weight = weightArray[vertex_index*_maxJointsPerVertex + j] * bary
            joint_index = jointIndexArray[vertex_index*_maxJointsPerVertex + j]
            
            bFound = False
            for k in range(len(final_pairs)):
                if final_pairs[k].joint_index == joint_index:
                    final_pairs[k].weight += weight
                    bFound = True
                    break
                 
            if bFound == False:
                 pair = WeightJointIndexPair()
                 pair.weight = weight
                 pair.joint_index = joint_index
                 final_pairs.append(pair)

    # Set joint index zero if the weight is zero. 
    for i in xrange(len(final_pairs)):
        if final_pairs[i].weight == 0:
            final_pairs[i].joint_index = 0
            
    final_pairs.sort()
    
    # TODO: Is it needed to make the sum of weight equal to 1? 
                 
    # for i in range(len(final_pairs)):
        # print '%d final_pairs.weight:%f final_pairs.joint_index:%d' % (i, final_pairs[i].weight, final_pairs[i].joint_index)
        
    # number of elements of final_pairs could be more than _maxJointsPerVertex but it should be at least _maxJointsPerVertex. 
    # If you really want it to be exactly _maxJointsPerVertex, you can try to pop out elements. 
    return final_pairs            
    
# p0, p1, p2 are three vertices of a triangle and p is inside the triangle
def ComputeBarycentricCoordinates(p0, p1, p2, p):
    v0 = p1 - p0
    v1 = p2 - p0
    v2 = p - p0
 
    v00 = v0 * v0
    v01 = v0 * v1
    v11 = v1 * v1
    v20 = v2 * v0
    v21 = v2 * v1
    d = v00 * v11 - v01 * v01
    v = (v11 * v20 - v01 * v21) / d # TODO: Do I need to care about divide-by-zero case?
    w = (v00 * v21 - v01 * v20) / d
    u = 1.0 - v - w
 
    # make sure u, v, w are non-negative. It could happen sometimes.
    u = max(u, 0)
    v = max(v, 0)
    w = max(w, 0)
    
    # normalize barycentric coordinates so that their sum is equal to 1
    sum = u + v + w
    u /= sum
    v /= sum
    w /= sum
 
    return [u, v, w]
    
def SaveTFXBoneBinaryFile(filepath, selected_mesh_shape_name, meshShapedagPath, rootPositions): 
    #---------------------------------------------------------------------------
    # Build a face/triangle index list to convert face index into triangle index
    #---------------------------------------------------------------------------
    faceIter = OpenMaya.MItMeshPolygon(meshShapedagPath)
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

    #print "TressFX: Triangle count:%d\n" % triangleCount

    #----------------------
    # Find the closest face
    #----------------------
    meshFn = OpenMaya.MFnMesh(meshShapedagPath)
    meshIntersector = OpenMaya.MMeshIntersector()
    meshIntersector.create(meshShapedagPath.node())
    
    triangleCounts = OpenMaya.MIntArray()
    triangleVertexIndices = OpenMaya.MIntArray() # the size of this array is three times of the number of total triangles
    meshFn.getTriangles(triangleCounts, triangleVertexIndices)
    
    vertexTriangleList = []

    triangleIdForStrandsList = []
    baryCoordList = []
    uvCoordList = []
    pointOnMeshList = []

    progressBar = ProgressBar('Collecting bone data', len(rootPositions))
    
    for i in range(len(rootPositions)):
        rootPoint = rootPositions[i]

        # Find the closest point info
        meshPt = OpenMaya.MPointOnMesh()
        meshIntersector.getClosestPoint(rootPoint, meshPt)
        pt = meshPt.getPoint()
        
        pointOnMesh = OpenMaya.MPoint()
        pointOnMesh.x = pt.x
        pointOnMesh.y = pt.y
        pointOnMesh.z = pt.z
        pointOnMeshList.append(pointOnMesh)
        
        # Find face index
        faceId = meshPt.faceIndex()
        
        # Find triangle index
        triangleId = faceTriaIndexList[faceId] + meshPt.triangleIndex()
        
        dummy = OpenMaya.MScriptUtil()
        dummyIntPtr = dummy.asIntPtr()
        triangleId_local = meshPt.triangleIndex() # This values is either 0 or 1. It is not a global triangle index. triangleId is the global triangle index. 
        pointArray = OpenMaya.MPointArray()
        vertIdList = OpenMaya.MIntArray()
        faceIter.setIndex(faceId, dummyIntPtr)
        faceIter.getTriangle(triangleId_local, pointArray, vertIdList, OpenMaya.MSpace.kWorld )
        
        vertexTriangleList.append((vertIdList[0], vertIdList[1], vertIdList[2]))
        triangleIdForStrandsList.append(triangleId)

        # Find three vertex indices for the triangle. Following two lines should give us three correct vertex indices for the triangle. I haven't really verified though. 
        #vertexIndex = [triangleVertexIndices[triangleId*3], triangleVertexIndices[triangleId*3+1], triangleVertexIndices[triangleId*3+2]]
        vertexIndex = [vertIdList[0], vertIdList[1], vertIdList[2]]
                    
        # Find barycentric coordinates
        uvw = OpenMaya.MPoint()
        
        # Somehow, below code gives me negative barycentric coordinates sometimes. 
        # uPtr = OpenMaya.MScriptUtil().asFloatPtr()
        # vPtr = OpenMaya.MScriptUtil().asFloatPtr()
        # meshPt.getBarycentricCoords(uPtr,vPtr)
        # uvw.x = OpenMaya.MScriptUtil(uPtr).asFloat()
        # uvw.y = OpenMaya.MScriptUtil(vPtr).asFloat()
        # uvw.z = 1.0 - uvw.x - uvw.y  
        
        # Instead getting barycentric coords from getBarycentricCoords, we compute it by the following function. 
        uvw_a = ComputeBarycentricCoordinates(pointArray[0], pointArray[1], pointArray[2], pointOnMesh)
      
        uvw.x = uvw_a[0]
        uvw.y = uvw_a[1]
        uvw.z = uvw_a[2]
        
        # barycentric coordinates should be non-zero
        uvw.x = max(uvw.x, 0)
        uvw.y = max(uvw.y, 0)
        uvw.z = max(uvw.z, 0)
        
        # normalize barycentric coordinates so that their sum is equal to 1
        sum = uvw.x + uvw.y + uvw.z
        uvw.x /= sum
        uvw.y /= sum
        uvw.z /= sum

        baryCoordList.append(uvw)
        
        # Find UV coordinates - We don't really use UV coords for tfxbone file. 
        # util = OpenMaya.MScriptUtil()
        # util.createFromList([0.0, 0.0], 2)
        # uv_ptr = util.asFloat2Ptr()
        # meshFn.getUVAtPoint(rootPoint, uv_ptr)
        # u = OpenMaya.MScriptUtil.getFloat2ArrayItem(uv_ptr, 0, 0)
        # v = OpenMaya.MScriptUtil.getFloat2ArrayItem(uv_ptr, 0, 1)
        # uv_coord = OpenMaya.MPoint()
        # uv_coord.x = u
        # uv_coord.y = v
        # uv_coord.z = 0
        # uvCoordList.append(uv_coord)

        # update progress gui
        progressBar.Increment()
        
    progressBar.Kill()
    
    #-------------------------
    # Get skin cluster object 
    #-------------------------     
    skinClusterName = ''
    skinClusters = cmds.listHistory(selected_mesh_shape_name)
    skinClusters = cmds.ls(skinClusters, type="skinCluster")
    if skinClusters:
        skinClusterName = skinClusters[0]
    else:
        cmds.warning('No skin cluster found on '+ selected_mesh_shape_name)
        return
    
    #print skinClusterName
    
    #---------------------------------------------------------------------------------------------------
    # TODO: Try the following method. 
    # skins = filter(lambda skin: mesh.getShape() in skin.getOutputGeometry(), ls(type='skinCluster'))
    
    # if len(skins) > 0 :
        # skin = skins[0]    
        # skinJoints = skin.influenceObjects();
        # root = skinJoints[0]
        
        # while root.getParent() :
            # root = root.getParent()
            
        # skin.getWeights(mesh.verts[index])    
        
        # select(root, hierarchy=True, replace=True)
        # joints = ls(selection=True, transforms=True, type='joint')
    #---------------------------------------------------------------------------------------------------

    # get the MFnSkinCluster using skinClusterName
    selList = OpenMaya.MSelectionList() 
    selList.add(skinClusterName) 
    skinClusterNode = OpenMaya.MObject() 
    selList.getDependNode(0, skinClusterNode) 
    skinFn = OpenMayaAnim.MFnSkinCluster(skinClusterNode)

    dagPaths = MDagPathArray()
    skinFn.influenceObjects(dagPaths)

    # influence object is a joint
    influenceObjectsNames = []

    # get joint names
    for i in range(dagPaths.length()):
        influenceName = dagPaths[i].partialPathName()        
        influenceObjectsNames.append(influenceName) # Need to remove namespace?
    
    skinMeshes = cmds.skinCluster(skinClusterName, query=1, geometry=1)
    geoIter = OpenMaya.MItGeometry(meshShapedagPath)
    infCount = OpenMaya.MScriptUtil()
    infCountPtr = infCount.asUintPtr()   
    numVertices = geoIter.count()
    
    weightArray = [0] * TRESSFX_MAX_INFLUENTIAL_BONE_COUNT  * numVertices # joint weight array for all vertices. Each vertex will have TRESSFX_MAX_INFLUENTIAL_BONE_COUNT  weights. 
                                                         # It is initialized with zero for empty weight in case there are less weights than TRESSFX_MAX_INFLUENTIAL_BONE_COUNT .
    jointIndexArray = [-1] * TRESSFX_MAX_INFLUENTIAL_BONE_COUNT  * numVertices # joint index array for all vertices. It is initialized with -1 for an empty element in case 
                                                              # there are less weights than TRESSFX_MAX_INFLUENTIAL_BONE_COUNT . 

    # collect bone weights for all vertices in the mesh
    index = 0

    while geoIter.isDone() == False:
        weights = OpenMaya.MDoubleArray()
        skinFn.getWeights(meshShapedagPath, geoIter.currentItem(), weights, infCountPtr)
        
        weightJointIndexPairs = []
        
        for i in range(len(weights)):
            pair = WeightJointIndexPair()
            pair.weight = weights[i]
            pair.joint_index = i 
            weightJointIndexPairs.append(pair)
    
        weightJointIndexPairs.sort()
        
        a = 0

        for j in range(min(len(weightJointIndexPairs), TRESSFX_MAX_INFLUENTIAL_BONE_COUNT )):
            weightArray[index*TRESSFX_MAX_INFLUENTIAL_BONE_COUNT  + a] = weightJointIndexPairs[j].weight
            jointIndexArray[index*TRESSFX_MAX_INFLUENTIAL_BONE_COUNT  + a] = weightJointIndexPairs[j].joint_index
            a += 1

        index += 1
        geoIter.next()
    
    #------------------------
    # Save the tfxbone file.
    #------------------------
    progressBar = ProgressBar('Saving a tfxbone file', len(influenceObjectsNames) + len(triangleIdForStrandsList))
    f = open(filepath, "wb")
    # Number of Bones
    f.write(ctypes.c_int(len(influenceObjectsNames)))
    
    # Write all bone (joint) names
    for i in range(len(influenceObjectsNames)):
        # Bone Joint Index
        f.write(ctypes.c_int(i))
        # Size of the string, add 1 to leave room for the nullterminate.
        f.write(ctypes.c_int(len(influenceObjectsNames[i]) + 1))
        # Print the characters of the string 1 by 1.
        for j in range(len(influenceObjectsNames[i])):
            f.write(ctypes.c_byte(ord(influenceObjectsNames[i][j])))
        # Add a zero to null terminate the string.
        f.write(ctypes.c_byte(0))
        progressBar.Increment()

    # Number of Strands
    f.write(ctypes.c_int(len(triangleIdForStrandsList)))
   
    for i in range(len(triangleIdForStrandsList)):
        triangleId = triangleIdForStrandsList[i]
        
        # three vertex indices from one triangle - Following two lines should work equally but I haven't verified it yet. 
        #vertexIndices = [triangleVertexIndices[triangleId*3], triangleVertexIndices[triangleId*3+1], triangleVertexIndices[triangleId*3+2]]
        vertexIndices = vertexTriangleList[i]

        baryCoord = baryCoordList[i]    
        weightJointIndexPairs = GetSortedWeightsFromTriangleVertices(TRESSFX_MAX_INFLUENTIAL_BONE_COUNT , vertexIndices, jointIndexArray, weightArray, baryCoord)
        
        # Index, the rest should be self explanatory.
        f.write(ctypes.c_int(i))
        f.write(ctypes.c_int(weightJointIndexPairs[0].joint_index))
        f.write(ctypes.c_float(weightJointIndexPairs[0].weight))
        f.write(ctypes.c_int(weightJointIndexPairs[1].joint_index))
        f.write(ctypes.c_float(weightJointIndexPairs[1].weight))
        f.write(ctypes.c_int(weightJointIndexPairs[2].joint_index))
        f.write(ctypes.c_float(weightJointIndexPairs[2].weight))
        f.write(ctypes.c_int(weightJointIndexPairs[3].joint_index))
        f.write(ctypes.c_float(weightJointIndexPairs[3].weight))
        progressBar.Increment()
        
    f.close()
    progressBar.Kill()
    return
           
def RecursiveSearchCurve(curves, objNode, minCurveLength):
    if objNode.hasFn(OpenMaya.MFn.kNurbsCurve):
        curveFn = OpenMaya.MFnNurbsCurve(objNode)
        curveLength = curveFn.length()
                
        # We only export open type curves.         
        if curveFn.form() == OpenMaya.MFnNurbsCurve.kOpen:
            if curveLength >= minCurveLength:
                curves.append(curveFn)
            
    elif objNode.hasFn(OpenMaya.MFn.kTransform):
        objFn = OpenMaya.MFnTransform(objNode)
        
        for j in range(objFn.childCount()):
            childObjNode = objFn.child(j)
            RecursiveSearchCurve(curves, childObjNode, minCurveLength)
            
def GetSelectedNurbsCurves(minCurveLength):
    slist = OpenMaya.MSelectionList()
    OpenMaya.MGlobal.getActiveSelectionList( slist )
    iter = OpenMaya.MItSelectionList(slist)
    curves = []

    # Find all nurbs curves under the selected node recursively. 
    for i in range(slist.length()):
        selObj = OpenMaya.MObject()
        slist.getDependNode(i, selObj)
        RecursiveSearchCurve(curves, selObj, minCurveLength)
        
    return curves    

class TressFXTFXFileHeader(ctypes.Structure):
    _fields_ = [('version', ctypes.c_float),
                ('numHairStrands', ctypes.c_uint),
                ('numVerticesPerStrand', ctypes.c_uint),
                ('offsetVertexPosition', ctypes.c_uint),
                ('offsetStrandUV', ctypes.c_uint),
                ('offsetVertexUV', ctypes.c_uint),
                ('offsetStrandThickness', ctypes.c_uint),
                ('offsetVertexColor', ctypes.c_uint),
                ('reserved', ctypes.c_uint * 32)]

class tressfx_float4(ctypes.Structure):
    _fields_ = [('x', ctypes.c_float),
                ('y', ctypes.c_float),
                ('z', ctypes.c_float),
                ('w', ctypes.c_float)]

class tressfx_float2(ctypes.Structure):
    _fields_ = [('x', ctypes.c_float),
                ('y', ctypes.c_float)]
                
def SaveTFXBinaryFile(filepath, curves, meshShapedagPath):
    numCurves = len(curves)
    numVerticesPerStrand = cmds.optionMenu("numberOfStrandsOptionMenu", query=True, value=True);
    numVerticesPerStrand = int(numVerticesPerStrand)
    
    bothEndsImmovable = cmds.checkBox("bothEndsImmovable",q = True, v = True)
    invertZ = cmds.checkBox("InvertZ",q = True, v = True)
     
    rootPositions = []

    tfxHeader = TressFXTFXFileHeader()
    tfxHeader.version = 4.0
    tfxHeader.numHairStrands = numCurves
    tfxHeader.numVerticesPerStrand = numVerticesPerStrand
    tfxHeader.offsetVertexPosition = ctypes.sizeof(TressFXTFXFileHeader)
    tfxHeader.offsetStrandUV = 0
    tfxHeader.offsetVertexUV = 0
    tfxHeader.offsetStrandThickness = 0
    tfxHeader.offsetVertexColor = 0
    
    meshFn = None
    meshIntersector = None
    
    # if meshShapedagPath is passed then let's get strand texture coords. To do this, we need MFnMesh and MMeshIntersector objects. 
    if meshShapedagPath != None:
        meshFn = OpenMaya.MFnMesh(meshShapedagPath)
        meshIntersector = OpenMaya.MMeshIntersector()
        meshIntersector.create(meshShapedagPath.node())
        tfxHeader.offsetStrandUV = tfxHeader.offsetVertexPosition + numCurves * numVerticesPerStrand * ctypes.sizeof(tressfx_float4)
  
    bInvertYForUVs = cmds.checkBox("InvertYForUVs",q = True, v = True)
  
    totalProgress = numCurves*numVerticesPerStrand
    
    if meshFn != None:
        totalProgress = numCurves*numVerticesPerStrand + numCurves
    
    progressBar = ProgressBar('Saving a tfx file', totalProgress)

    f = open(filepath, "wb")
    f.write(tfxHeader)

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
            curveFn.getPointAtParam(param, pos, OpenMaya.MSpace.kObject) # kObject
            
            p = tressfx_float4()
            p.x = pos.x
            p.y = pos.y
            
            if invertZ:
                p.z = -pos.z # flip in z-axis
            else:
                p.z = pos.z
            
            # w component is an inverse mass
            if j == 0 or j == 1: # the first two vertices are immovable always. 
                p.w = 0
            else:
                p.w = 1.0; 

            f.write(p)
            progressBar.Increment()

        # # find face index
        rootPos = OpenMaya.MPoint()
        curveFn.getPointAtParam(0, rootPos, OpenMaya.MSpace.kObject) # kWorld?
        rootPositions.append(rootPos)
        
    # if meshShapedagPath is passed then let's get strand texture coords by using raycasting to the mesh from each root position of hair strand.     
    if meshFn != None:   
        for i in range(len(rootPositions)):
            rootPoint = rootPositions[i]
        
             # Find UV coordinates 
            util = OpenMaya.MScriptUtil()
            util.createFromList([0.0, 0.0], 2)
            uv_ptr = util.asFloat2Ptr()
            meshFn.getUVAtPoint(rootPoint, uv_ptr)
            u = OpenMaya.MScriptUtil.getFloat2ArrayItem(uv_ptr, 0, 0)
            v = OpenMaya.MScriptUtil.getFloat2ArrayItem(uv_ptr, 0, 1)
            uv_coord = tressfx_float2()
            uv_coord.x = u
            uv_coord.y = v
    
            if bInvertYForUVs:
                uv_coord.y = 1.0 - uv_coord.y; # DirectX has it inverted
                
            #print "uv:%g, %g\n" % (uv_coord.x, uv_coord.y)
            f.write(uv_coord)
            progressBar.Increment()
                
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
    
def SaveTFXSkinBinaryFile(filepath, meshShapedagPath, rootPositions): 
    #---------------------------------------------------------------------------
    # Build a face/triangle index list to convert face index into triangle index
    #---------------------------------------------------------------------------
    faceIter = OpenMaya.MItMeshPolygon(meshShapedagPath)
    triangleCount = 0
    faceTriaIndexList = []
    index = 0
    bInvertYForUVs = cmds.checkBox("InvertYForUVs",q = True, v = True)
    
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
    meshFn = OpenMaya.MFnMesh(meshShapedagPath)
    meshIntersector = OpenMaya.MMeshIntersector()
    meshIntersector.create(meshShapedagPath.node())
    
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
        
        # TODO: Why are there negative barycentric coords?
        #if uvw.x < 0 or uvw.y < 0 or uvw.z < 0:
        #    print 'uvw:', uvw.x, uvw.y, uvw.z
        
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

    #--------------------
    # Save a tfxskin file
    #--------------------
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
        
        if bInvertYForUVs:
            p.y = 1.0 - uv_coord.y # DirectX has it inverted
        
        p.z = uv_coord.z         
        f.write(p)    
        progressBar.Increment()
    
    f.close()
    progressBar.Kill()
     
    return
 
def GetSortedWeightsFromOneVertex(_maxJointsPerVertex, vertexIndex, jointIndexArray, weightArray):
    final_pairs = []
    
    for j in range(_maxJointsPerVertex):
        weight = weightArray[vertexIndex*_maxJointsPerVertex + j]
        joint_index = jointIndexArray[vertexIndex*_maxJointsPerVertex + j]
        
        bFound = False
        for k in range(len(final_pairs)):
            if final_pairs[k].joint_index == joint_index:
                final_pairs[k].weight += weight
                bFound = True
                break
             
        if bFound == False:
             pair = WeightJointIndexPair()
             pair.weight = weight
             pair.joint_index = joint_index
             final_pairs.append(pair)
             
    # Set joint index zero if the weight is zero. 
    for i in xrange(len(final_pairs)):
        if final_pairs[i].weight == 0:
            final_pairs[i].joint_index = 0
            
    final_pairs.sort()
    
    # TODO: Is it needed to make the sum of weight equal to 1? 
    
    # for i in range(len(final_pairs)):
        # print '%d final_pairs.weight:%f final_pairs.joint_index:%d' % (i, final_pairs[i].weight, final_pairs[i].joint_index)
        
    # number of elements of final_pairs could be more than _maxJointsPerVertex but it should be at least _maxJointsPerVertex. 
    # If you really want it to be exactly _maxJointsPerVertex, you can try to pop out elements. 
    return final_pairs  
 
def ExportMesh(filepath, meshShapedagPath): 
    meshFn = OpenMaya.MFnMesh(meshShapedagPath)
    meshIntersector = OpenMaya.MMeshIntersector()
    meshIntersector.create(meshShapedagPath.node())
    
    faceIdList = []
    baryCoordList = []
    
    points = OpenMaya.MPointArray()
    meshFn.getPoints(points, OpenMaya.MSpace.kWorld)
    
    normals = OpenMaya.MFloatVectorArray()
    meshFn.getVertexNormals(False, normals, OpenMaya.MSpace.kWorld)
    
    triangleCounts = OpenMaya.MIntArray()
    triangleVertexIndices = OpenMaya.MIntArray() # the size of this array is three times of the number of total triangles
    meshFn.getTriangles(triangleCounts, triangleVertexIndices)
        
    #-------------------------
    # Get skin cluster object 
    #-------------------------     
    skinClusterName = ''
    mesh_shape_name = meshFn.name()
    skinClusters = cmds.listHistory(mesh_shape_name)
    skinClusters = cmds.ls(skinClusters, type="skinCluster")
    if skinClusters:
        skinClusterName = skinClusters[0]
    else:
        cmds.warning('No skin cluster found on '+ mesh_shape_name)
        return
    
    #print skinClusterName
    
    # get the MFnSkinCluster using skinClusterName
    selList = OpenMaya.MSelectionList() 
    selList.add(skinClusterName) 
    skinClusterNode = OpenMaya.MObject() 
    selList.getDependNode(0, skinClusterNode) 
    skinFn = OpenMayaAnim.MFnSkinCluster(skinClusterNode)

    dagPaths = MDagPathArray()
    skinFn.influenceObjects(dagPaths)

    # influence object is a joint
    influenceObjectsNames = []

    # get joint names
    for i in range(dagPaths.length()):
        influenceName = dagPaths[i].partialPathName()        
        influenceObjectsNames.append(influenceName) # Need to remove namespace?
    
    skinMeshes = cmds.skinCluster(skinClusterName, query=1, geometry=1)
    geoIter = OpenMaya.MItGeometry(meshShapedagPath)
    infCount = OpenMaya.MScriptUtil()
    infCountPtr = infCount.asUintPtr()   
    numVertices = geoIter.count()
    
    weightArray = [0] * TRESSFX_MAX_INFLUENTIAL_BONE_COUNT  * numVertices # joint weight array for all vertices. Each vertex will have TRESSFX_MAX_INFLUENTIAL_BONE_COUNT  weights. 
                                                         # It is initialized with zero for empty weight in case there are less weights than TRESSFX_MAX_INFLUENTIAL_BONE_COUNT .
    jointIndexArray = [-1] * TRESSFX_MAX_INFLUENTIAL_BONE_COUNT  * numVertices # joint index array for all vertices. It is initialized with -1 for an empty element in case 
                                                              # there are less weights than TRESSFX_MAX_INFLUENTIAL_BONE_COUNT . 

    # collect bone weights for all vertices in the mesh
    index = 0

    progressBar = ProgressBar('Collect data', numVertices)
    
    while geoIter.isDone() == False:
        weights = OpenMaya.MDoubleArray()
        skinFn.getWeights(meshShapedagPath, geoIter.currentItem(), weights, infCountPtr)
        
        weightJointIndexPairs = []
        
        for i in range(len(weights)):
            pair = WeightJointIndexPair()
            pair.weight = weights[i]
            pair.joint_index = i 
            weightJointIndexPairs.append(pair)
    
        weightJointIndexPairs.sort()
        
        a = 0

        for j in range(min(len(weightJointIndexPairs), TRESSFX_MAX_INFLUENTIAL_BONE_COUNT )):
            weightArray[index*TRESSFX_MAX_INFLUENTIAL_BONE_COUNT  + a] = weightJointIndexPairs[j].weight
            jointIndexArray[index*TRESSFX_MAX_INFLUENTIAL_BONE_COUNT  + a] = weightJointIndexPairs[j].joint_index
            a += 1

        index += 1
        progressBar.Increment()
        geoIter.next()

    progressBar.Kill()
    
    #----------------------------------------------------------    
    # We collected all necessary data. Now save them in file.  
    #----------------------------------------------------------  
    totalProgress = points.length() + triangleVertexIndices.length() / 3 + len(influenceObjectsNames)
    progressBar = ProgressBar('Export collision mesh', totalProgress)    
    
    f = open(filepath, "w")
    f.write("# TressFX collision mesh exported by TressFX Exporter in Maya\n")
    
    # Write all bone (joint) names
    f.write("numOfBones %g\n" % (len(influenceObjectsNames)))
    f.write("# bone index, bone name\n")
    for i in range(len(influenceObjectsNames)):
        f.write("%d %s\n" % (i, influenceObjectsNames[i]))
        progressBar.Increment()
        
    # write vertex positions and skinning data
    f.write("numOfVertices %g\n" % (points.length()))
    f.write("# vertex index, vertex position x, y, z, normal x, y, z, joint index 0, joint index 1, joint index 2, joint index 3, weight 0, weight 1, weight 2, weight 3\n")
    for vertexIndex in xrange(points.length()):
        point = points[vertexIndex]
        normal = normals[vertexIndex]
        weightJointIndexPairs = GetSortedWeightsFromOneVertex(TRESSFX_MAX_INFLUENTIAL_BONE_COUNT , vertexIndex, jointIndexArray, weightArray)
        f.write("%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g\n" % (vertexIndex, point.x, point.y, point.z, normal.x, normal.y, normal.z, weightJointIndexPairs[0].joint_index, weightJointIndexPairs[1].joint_index, weightJointIndexPairs[2].joint_index, weightJointIndexPairs[3].joint_index,
                                                              weightJointIndexPairs[0].weight, weightJointIndexPairs[1].weight, weightJointIndexPairs[2].weight, weightJointIndexPairs[3].weight))
        progressBar.Increment()
        
    # write triangle face indices    
    f.write("numOfTriangles %g\n" % (triangleVertexIndices.length() / 3))    
    f.write("# triangle index, vertex index 0, vertex index 1, vertex index 2\n")
    for i in range(triangleVertexIndices.length() / 3):
        f.write("%g %d %d %d\n" % (i, triangleVertexIndices[i * 3 + 0], triangleVertexIndices[i * 3 + 1], triangleVertexIndices[i * 3 + 2]))
        progressBar.Increment()
        
    f.close()
    progressBar.Kill()
    return
    
def DoExportCollisionMesh(*args):
    #------------------------------
    # Find the selected mesh shape
    #------------------------------
    meshShapedagPath = OpenMaya.MDagPath()
    
    if selected_mesh_shape_name == '':
        cmds.warning("To export skin or bone data, base mesh must be set.\n")
        return

    allObject = cmds.ls(selected_mesh_shape_name)
    cmds.select(allObject) # TODO: This selection makes hair curves unselected. This is not a problem but just inconvenient for users if they want to keep the curves selected.  
    
    sel_list = OpenMaya.MSelectionList()
    OpenMaya.MGlobal.getActiveSelectionList(sel_list)
    
    if sel_list.length() == 0:
        return
    
    sel_list.getDagPath(0, meshShapedagPath)
    meshShapedagPath.extendToShape() # get mesh shape

    basicFilter = "*.tfxmesh"
    filepath = cmds.fileDialog2(fileFilter=basicFilter, dialogStyle=2, caption="Save a tfx collision mesh file(*.tfxmesh)", fileMode=0)
    
    if filepath == None or len(filepath) == 0:
        return
    
    ExportMesh(filepath[0], meshShapedagPath)    
    return
    
    