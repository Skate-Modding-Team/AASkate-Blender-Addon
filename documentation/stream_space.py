import bpy
import struct
import mathutils

# ---------------------------------------------------------
# Struct definitions (matching your C++ layout)
# ---------------------------------------------------------
HEADER_STRUCT = ">I I Q I I"  
# m_ID (uint32), version(uint32), stamp(uint64), numCollections(uint32), unused(uint32)

ENTRY_STRUCT = ">Q B 7x ffff ffff ffff ffff"
# m_ID (uint64)
# m_LOD (uint8)
# padding (7 bytes skipped)
# HullMin (4 floats)
# HullMax (4 floats)
# GeoMin  (4 floats)
# GeoMax  (4 floats)

ENTRY_SIZE = struct.calcsize(ENTRY_STRUCT)
HEADER_SIZE = struct.calcsize(HEADER_STRUCT)

# ---------------------------------------------------------
# Function to create a bounding box mesh in Blender
# ---------------------------------------------------------
def create_bbox(name, min_v, max_v):
    """
    min_v, max_v are (x, y, z, w) tuples but w is unused.
    """
    min_x, min_z, min_y, _ = min_v
    max_x, max_z, max_y, _ = max_v

    # 8 corner vertices of a box
    verts = [
        (min_x, min_y, min_z),
        (min_x, min_y, max_z),
        (min_x, max_y, min_z),
        (min_x, max_y, max_z),
        (max_x, min_y, min_z),
        (max_x, min_y, max_z),
        (max_x, max_y, min_z),
        (max_x, max_y, max_z),
    ]

    faces = [
        (0,1,3,2),
        (4,5,7,6),
        (0,1,5,4),
        (2,3,7,6),
        (0,2,6,4),
        (1,3,7,5)
    ]

    mesh = bpy.data.meshes.new(name)
    mesh.from_pydata(verts, [], faces)
    obj = bpy.data.objects.new(name, mesh)
    bpy.context.collection.objects.link(obj)
    return obj


# ---------------------------------------------------------
# Main CSPA Import Function
# ---------------------------------------------------------
def import_cspa(filepath):
    print("Importing:", filepath)
    with open(filepath, "rb") as f:
        header_data = f.read(HEADER_SIZE)
        if len(header_data) < HEADER_SIZE:
            print("File too small for header.")
            return
        
        m_ID, version, stamp, numCollections, unused = struct.unpack(HEADER_STRUCT, header_data)
        
        print(f"Header:")
        print(f"  ID             = {hex(m_ID)}")
        print(f"  Version        = {version}")
        print(f"  Stamp          = {stamp}")
        print(f"  Num Collections= {numCollections}")

        for i in range(numCollections):
            entry_data = f.read(ENTRY_SIZE)
            if len(entry_data) < ENTRY_SIZE:
                print("Unexpected end of file.")
                break

            unpacked = struct.unpack(ENTRY_STRUCT, entry_data)

            idx = 0
            entry_id = unpacked[idx]; idx+=1
            lod = unpacked[idx]; idx+=1

            hullMin = unpacked[idx:idx+4]; idx+=4
            hullMax = unpacked[idx:idx+4]; idx+=4
            geoMin  = unpacked[idx:idx+4]; idx+=4
            geoMax  = unpacked[idx:idx+4]; idx+=4

            # Create bounding boxes in Blender
            hull_name = f"Hull_{entry_id}"
            geo_name  = f"Geo_{entry_id}"

            create_bbox(hull_name, hullMin, hullMax)
            create_bbox(geo_name, geoMin, geoMax)

            print(f"Loaded entry {i}: ID={entry_id}, LOD={lod}")


# ---------------------------------------------------------
# CALL IMPORTER MANUALLY BELOW:
# ---------------------------------------------------------
# Example usage:
# Change path as needed:
filepath = r"G:\Skate 3 (USA, Asia) (En,Fr,Es)\PS3_GAME\USRDIR\data\content\world\stream\DIST_NewSanVan\DIST_NewSanVan_Pres.pss"
import_cspa(filepath)
