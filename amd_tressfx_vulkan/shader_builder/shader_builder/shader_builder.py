import os;
import array;
import re;

def get_file_content(filename):
    src_file = open("..\..\src\Shaders\\" + filename, 'r')
    return src_file.read()

def load_array(name, filename):
    src_file = open("..\..\src\Shaders\\" + filename)
    if filename.endswith('.vert'):
        temporary_file = open('temp.vert', 'w')
    elif filename.endswith('.frag'):
        temporary_file = open('temp.frag', 'w')
    else:
        temporary_file = open('temp.comp', 'w')
    line = src_file.readline()
    while line != '':
        m = re.search(r'^#include "(.*)"', line)
        if m:
            temporary_file.write(get_file_content(m.group(1)))
        else:
            temporary_file.write(line)
        line = src_file.readline()
    temporary_file.close()
    if filename.endswith('.vert'):
        os.system("glslangValidator.exe -V temp.vert")
        f = open("vert.spv", 'rb')
        filesize = os.path.getsize("vert.spv")
    elif filename.endswith('.frag'):
        os.system("glslangValidator.exe -V temp.frag")
        f = open("frag.spv", 'rb')
        filesize = os.path.getsize("frag.spv")
    else:
        os.system("glslangValidator.exe -V temp.comp")
        f = open("comp.spv", 'rb')
        filesize = os.path.getsize("comp.spv")
    a = array.array('L')
    a.fromfile(f, filesize // 4)
    f.close()
    code = "const std::vector<uint32_t> " + name +" = {"
    for i in a:
        code += str(i) + ",\n"
    code += "};\n"
    return code

output = open("..\..\src\TressFXPrecompiledShadersVulkan.h", 'w')
output.write("#include <vector>\n")
output.write(load_array("hair_shadow_vertex", "HairShadowMap.vert"))
output.write(load_array("hair_shadow_fragment", "HairShadowMap.frag"))
output.write(load_array("render_hair_vertex", "RenderHair.vert"))
output.write(load_array("render_hair_aa_vertex", "RenderHairAA.vert"))
output.write(load_array("render_hair_strand_copies_vertex", "RenderHairStrandCopies.vert"))
output.write(load_array("render_hair_aa_strand_copies_vertex", "RenderHairAAStrandCopies.vert"))
output.write(load_array("pass1_fragment", "TressFXRender.frag"))
output.write(load_array("pass2_vertex", "TressFXRender_pass2.vert"))
output.write(load_array("pass2_fragment", "TressFXRender_pass2.frag"))
output.write(load_array("global_constraints", "GlobalConstraints.comp"))
output.write(load_array("local_constraints", "LocalConstraints.comp"))
output.write(load_array("length_wind_tangent_compute", "LengthWindTangentComputation.comp"))
output.write(load_array("prepare_follow_hair", "PrepareFollowHair.comp"))
output.write(load_array("update_follow_hair", "UpdateFollowHair.comp"))
output.write(load_array("depth_hair_data", "FS_Depth_Hair_Data.frag"))
output.write(load_array("resolve_depth", "FS_ResolveDepth_Hair_Data.frag"))
output.write(load_array("fillcolors_hair_data", "FS_FillColors_Hair_Data.frag"))
output.write(load_array("resolvecolors", "FS_ResolveColor_Hair_Data.frag"))
output.close()
