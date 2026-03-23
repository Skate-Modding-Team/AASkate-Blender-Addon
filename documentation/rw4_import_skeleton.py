import bpy
import struct
import mathutils
import numpy as np
import math

# Bone name → parent name (None = root)
bone_hierarchy = {
    "TRAJECTORY": None,
    "HIPS": None,
    "SPINE": "HIPS",
    "SPINE1": "SPINE",
    "SPINE2": "SPINE1",
    "SPINE3": "SPINE2",
    "NECK": "SPINE3",
    "NECK1": "NECK",
    "HEAD": "NECK1",
    "RIGHTSHOULDER": "SPINE3",
    "RIGHTARM": "RIGHTSHOULDER",
    "RIGHTFOREARM": "RIGHTARM",
    "RIGHTHAND": "RIGHTFOREARM",
    "LEFTSHOULDER": "SPINE3",
    "LEFTARM": "LEFTSHOULDER",
    "LEFTFOREARM": "LEFTARM",
    "LEFTHAND": "LEFTFOREARM",
    "RIGHTUPLEG": "HIPS",
    "RIGHTLEG": "RIGHTUPLEG",
    "RIGHTFOOT": "RIGHTLEG",
    "RIGHTTOEBASE": "RIGHTFOOT",
    "LEFTUPLEG": "HIPS",
    "LEFTLEG": "LEFTUPLEG",
    "LEFTFOOT": "LEFTLEG",
    "LEFTTOEBASE": "LEFTFOOT",
    "SKATEBOARD_ROOT": None,
    "TRUCK_FRONT": "SKATEBOARD_ROOT",
    "RIGHT_WHEELFRONT": "TRUCK_FRONT",
    "LEFT_WHEELFRONT": "TRUCK_FRONT",
    "TRUCK_BACK": "SKATEBOARD_ROOT",
    "LEFT_WHEELBACK": "TRUCK_BACK",
    "RIGHT_WHEELBACK": "TRUCK_BACK",
    "LEFTHANDTHUMB1": "LEFTHAND",
    "LEFTHANDTHUMB2": "LEFTHANDTHUMB1",
    "LEFTHANDTHUMB3": "LEFTHANDTHUMB2",
    "LEFTHANDINDEX1": "LEFTHAND",
    "LEFTHANDINDEX2": "LEFTHANDINDEX1",
    "LEFTHANDINDEX3": "LEFTHANDINDEX2",
    "LEFTHANDMIDDLE1": "LEFTHAND",
    "LEFTHANDMIDDLE2": "LEFTHANDMIDDLE1",
    "LEFTHANDMIDDLE3": "LEFTHANDMIDDLE2",
    "LEFTINHANDRING": "LEFTHAND",
    "LEFTHANDRING1": "LEFTINHANDRING",
    "LEFTHANDRING2": "LEFTHANDRING1",
    "LEFTHANDRING3": "LEFTHANDRING2",
    "LEFTINHANDPINKY": "LEFTHAND",
    "LEFTHANDPINKY1": "LEFTINHANDPINKY",
    "LEFTHANDPINKY2": "LEFTHANDPINKY1",
    "LEFTHANDPINKY3": "LEFTHANDPINKY2",
    "RIGHTHANDTHUMB1": "RIGHTHAND",
    "RIGHTHANDTHUMB2": "RIGHTHANDTHUMB1",
    "RIGHTHANDTHUMB3": "RIGHTHANDTHUMB2",
    "RIGHTHANDINDEX1": "RIGHTHAND",
    "RIGHTHANDINDEX2": "RIGHTHANDINDEX1",
    "RIGHTHANDINDEX3": "RIGHTHANDINDEX2",
    "RIGHTHANDMIDDLE1": "RIGHTHAND",
    "RIGHTHANDMIDDLE2": "RIGHTHANDMIDDLE1",
    "RIGHTHANDMIDDLE3": "RIGHTHANDMIDDLE2",
    "RIGHTINHANDRING": "RIGHTHAND",
    "RIGHTHANDRING1": "RIGHTINHANDRING",
    "RIGHTHANDRING2": "RIGHTHANDRING1",
    "RIGHTHANDRING3": "RIGHTHANDRING2",
    "RIGHTINHANDPINKY": "RIGHTHAND",
    "RIGHTHANDPINKY1": "RIGHTINHANDPINKY",
    "RIGHTHANDPINKY2": "RIGHTHANDPINKY1",
    "RIGHTHANDPINKY3": "RIGHTHANDPINKY2",
    "RIGHTTOEBASE_REPARENTED": None,
    "LEFTTOEBASE_REPARENTED": None,
    "RIGHTHAND_REPARENTED": None,
    "LEFTHAND_REPARENTED": None,
    "RIGHTSHOULDERHLP": "SPINE3",
    "RIGHTARMTWIST": "RIGHTARM",
    "RIGHTFOREARMTWIST": "RIGHTFOREARM",
    "RIGHTFOREARMTWIST1": "RIGHTFOREARM",
    "LEFTSHOULDERHLP": "SPINE3",
    "LEFTARMTWIST": "LEFTARM",
    "LEFTFOREARMTWIST": "LEFTFOREARM",
    "LEFTFOREARMTWIST1": "LEFTFOREARM",
    "RIGHTUPLEGHLP": "HIPS",
    "RIGHTUPLEGTWIST": "RIGHTUPLEG",
    "LEFTUPLEGHLP": "HIPS",
    "LEFTUPLEGTWIST": "LEFTUPLEG",
    "FACE": "HEAD",
    "OFFSET_JAW": "FACE",
    "JAW": "OFFSET_JAW",
    "OFFSET_CHIN": "JAW",
    "CHIN": "OFFSET_CHIN",
    "OFFSET_LOWERLIP": "JAW",
    "OFFSET_LEFTLOWERLIP": "JAW",
    "OFFSET_RIGHTLOWERLIP": "JAW",
    "OFFSET_TONGUE": "JAW",
    "OFFSET_LEFTCHEEK": "FACE",
    "OFFSET_LEFTEYE": "FACE",
    "OFFSET_LEFTMOUTH": "FACE",
    "OFFSET_LEFTUPCHEEK": "FACE",
    "OFFSET_LEFTUPPERLIP": "FACE",
    "OFFSET_RIGHTCHEEK": "FACE",
    "OFFSET_RIGHTEYE": "FACE",
    "OFFSET_RIGHTMOUTH": "FACE",
    "OFFSET_RIGHTUPCHEEK": "FACE",
    "OFFSET_RIGHTUPPERLIP": "FACE",
    "OFFSET_UPPERLIP": "FACE",
    "TONGUE": "OFFSET_TONGUE",
    "OFFSET_TONGUETIP": "TONGUE",
    "LEFTLOWERLIP": "OFFSET_LEFTLOWERLIP",
    "LOWERLIP": "OFFSET_LOWERLIP",
    "RIGHTLOWERLIP": "OFFSET_RIGHTLOWERLIP",
    "TONGUETIP": "OFFSET_TONGUETIP",
    "LEFTCHEEK": "OFFSET_LEFTCHEEK",
    "LEFTCREASE": "FACE",
    "LEFTEYE": "OFFSET_LEFTEYE",
    "LEFTINNEREYEBROW": "FACE",
    "LEFTLOWEYELID": "FACE",
    "LEFTMOUTH": "OFFSET_LEFTMOUTH",
    "LEFTNOSE": "FACE",
    "LEFTOUTEREYEBROW": "FACE",
    "LEFTUPCHEEK": "OFFSET_LEFTUPCHEEK",
    "LEFTUPEYELID": "FACE",
    "LEFTUPPERLIP": "OFFSET_LEFTUPPERLIP",
    "RIGHTCHEEK": "OFFSET_RIGHTCHEEK",
    "RIGHTCREASE": "FACE",
    "RIGHTEYE": "OFFSET_RIGHTEYE",
    "RIGHTINNEREYEBROW": "FACE",
    "RIGHTLOWEYELID": "FACE",
    "RIGHTMOUTH": "OFFSET_RIGHTMOUTH",
    "RIGHTNOSE": "FACE",
    "RIGHTOUTEREYEBROW": "FACE",
    "RIGHTUPCHEEK": "OFFSET_RIGHTUPCHEEK",
    "RIGHTUPEYELID": "FACE",
    "RIGHTUPPERLIP": "OFFSET_RIGHTUPPERLIP",
    "UPPERLIP": "OFFSET_UPPERLIP",
}

def read_bone_struct(f):
    # Read correct bone struct: 4x u32, 4x u16, 2x u32
    data = f.read(32)  # 4*4 + 4*2 + 2*4 = 32 bytes
    parts = struct.unpack('>4I4H2I', data)  # Big endian
    return {
        'pIBPMatrices': parts[0],
        'pMeshTable': parts[1],
        'pBoneNameTable': parts[2],
        'pBoneNameList': parts[3],
        'iNumTotalBones': parts[4],
        'iNumMeshes': parts[5],
        'iNumBones': parts[6],
        'iNumIslands': parts[7],
        'pIslandAABBs': parts[8],
        'pIslandAreas': parts[9],
    }

def get_all_children(hierarchy, root):
    return {k for k, v in hierarchy.items() if v == root} | {
        child for k, v in hierarchy.items() if v == root
        for child in get_all_children(hierarchy, k)
    }

def read_matrix(f, offset):
    f.seek(offset)
    mat = struct.unpack('>16f', f.read(64))  # 16 big-endian floats
    return [mat[i:i+4] for i in range(0, 16, 4)]

def read_name(f, offset):
    f.seek(offset)
    name_bytes = bytearray()
    while True:
        b = f.read(1)
        if b == b'\x00' or b == b'':  # Null byte signifies end of name
            break
        name_bytes.extend(b)
    return name_bytes.decode('utf-8')

def create_bone(edit_bones, name, matrix, child_position=None):
    # Invert the inverse bind pose matrix
    inverse_bind = np.array(matrix)
    matrix = np.linalg.inv(inverse_bind)

    # Extract the rotation part of the matrix
    rotation = mathutils.Matrix((
        (matrix[0][0], matrix[1][0], matrix[2][0]),
        (matrix[0][1], matrix[1][1], matrix[2][1]),
        (matrix[0][2], matrix[1][2], matrix[2][2]),
    ))

    # Translation part of the matrix (bone's position)
    translation = mathutils.Vector((
        matrix[3][0], 
        matrix[3][1], 
        matrix[3][2]
    ))
    
    # Create the bone
    bone = edit_bones.new(name)
    bone.head = translation

    # If a child position is given, use it for the tail, else use a small offset for the tail
    if child_position:
        direction = (child_position - bone.head)
        forward = rotation @ mathutils.Vector((1, 0, 0))
        bone.tail = bone.head + forward * direction.length
    else:
        # Set tail along local X axis
        forward = rotation @ mathutils.Vector((1, 0, 0))
        bone.tail = bone.head + forward * 0.05

    return bone

def create_wireframe_bounding_box(x_Min, y_Min, z_Min, x_Max, y_Max, z_Max):
    # Create a new mesh for the bounding box (wireframe only)
    mesh = bpy.data.meshes.new("BoundingBoxMesh")
    obj = bpy.data.objects.new("BoundingBox", mesh)
    
    # Link the object to the current scene
    bpy.context.collection.objects.link(obj)
    
    # Create the bounding box's vertices using the min/max values
    vertices = [
        (x_Min, y_Min, z_Min),  # 0
        (x_Min, y_Min, z_Max),  # 1
        (x_Min, y_Max, z_Min),  # 2
        (x_Min, y_Max, z_Max),  # 3
        (x_Max, y_Min, z_Min),  # 4
        (x_Max, y_Min, z_Max),  # 5
        (x_Max, y_Max, z_Min),  # 6
        (x_Max, y_Max, z_Max)   # 7
    ]
    
    # Define edges (wireframe) that connect the vertices
    edges = [
        (0, 1), (1, 3), (3, 2), (2, 0),  # Front face edges
        (4, 5), (5, 7), (7, 6), (6, 4),  # Back face edges
        (0, 4), (1, 5), (2, 6), (3, 7)   # Connecting edges between front and back faces
    ]
    
    # Create the mesh object with vertices and edges (no faces)
    mesh.from_pydata(vertices, edges, [])
    
    # Update the mesh
    mesh.update()
    return obj

def read_tAABB(f):
    # Assuming we're reading 32 bytes from a file
    data = f.read(32)  # 2 * 4 * 32-bit floats = 32 bytes
    if len(data) < 32:
        return None  # If there's not enough data
    # Unpack the data as two sets of 4 floats (for m_Min and m_Max)
    x_Min, y_Min, z_Min, w_Min, x_Max, y_Max, z_Max, w_Max = struct.unpack('>8f', data)
    # Return raw floats for min and max
    return x_Min, y_Min, z_Min, w_Min, x_Max, y_Max, z_Max, w_Max

def import_bones(filepath):
    with open(filepath, 'rb') as f:
        # Read AABB data
        x_Min, y_Min, z_Min, w_Min, x_Max, y_Max, z_Max, w_Max = read_tAABB(f)
        create_wireframe_bounding_box(x_Min, y_Min, z_Min, x_Max, y_Max, z_Max)

        # Read the bone struct
        bone_struct = read_bone_struct(f)
        pIBPMatrices = bone_struct['pIBPMatrices']
        pBoneNameTable = bone_struct['pBoneNameTable']
        pBoneNameList = bone_struct['pBoneNameList']
        iNumBones = bone_struct['iNumBones']

        print(f"Data Offset: {hex(pIBPMatrices)}")
        print(f"Name Offset: {hex(pBoneNameTable)}")
        print(f"Namedata Offset: {hex(pBoneNameList)}")
        print(f"Bone Count: {iNumBones}")

        # Read matrices
        matrices = []
        for i in range(iNumBones):
            matrices.append(read_matrix(f, pIBPMatrices + i * 64))

        # Read name offsets
        f.seek(pBoneNameTable)
        name_offsets = []
        for i in range(iNumBones):
            offset_bytes = f.read(4)
            if len(offset_bytes) < 4:
                break
            (name_rel_offset,) = struct.unpack('>I', offset_bytes)
            name_offsets.append(name_rel_offset)

        # Read names
        names = []
        for noff in name_offsets:
            name = read_name(f, noff)
            names.append(name)
            
        mirrored_bones = get_all_children(bone_hierarchy, "RIGHTHAND")

        # Create armature and bones
        armature = bpy.data.armatures.new('ImportedArmature')
        obj = bpy.data.objects.new('ImportedArmature', armature)
        bpy.context.collection.objects.link(obj)
        bpy.context.view_layer.objects.active = obj
        bpy.ops.object.mode_set(mode='EDIT')

        edit_bones = obj.data.edit_bones
        created_bones = {}  # To hold references to created bones for parenting

        # First, compute all translations (bone heads)
        bone_translations = {}
        for i, mat in enumerate(matrices):
            matrix = np.linalg.inv(np.array(mat))
            translation = mathutils.Vector((matrix[3][0], matrix[3][1], matrix[3][2]))
            bone_name = names[i] if i < len(names) else f"Bone_{i}"
            bone_translations[bone_name.upper()] = translation

        # Then, create bones with proper tail direction
        for i, mat in enumerate(matrices):
            bone_name = names[i] if i < len(names) else f"Bone_{i}"
            parent_name = bone_hierarchy.get(bone_name.upper())
            # If has child, try to use the first matching child position as tail
            child_pos = None
            for child_name, p_name in bone_hierarchy.items():
                if p_name == bone_name.upper() and child_name in (n.upper() for n in bone_translations):
                    child_pos = bone_translations[child_name.upper()]
                    break
            
            # Mirrors right hand bones
            if bone_name.upper() in mirrored_bones:
                mirror_x = np.diag([-1, 1, 1, 1])
                mat = (np.array(mat) @ mirror_x).tolist()
                
            bone = create_bone(edit_bones, bone_name, mat, child_position=child_pos)
            created_bones[bone_name.upper()] = bone

        for bone_name, bone in created_bones.items():
            parent_name = bone_hierarchy.get(bone_name)
            actual_parent = parent_name
            while actual_parent and actual_parent not in created_bones:
                actual_parent = bone_hierarchy.get(actual_parent)

            if actual_parent and actual_parent in created_bones:
                bone.parent = created_bones[actual_parent]

        bpy.ops.object.mode_set(mode='OBJECT')
        print("Bone import finished!")

# Example usage
file = r'C:\path\to\RWOBJECTTYPE_RENDERMODELDATA'

import_bones(file)