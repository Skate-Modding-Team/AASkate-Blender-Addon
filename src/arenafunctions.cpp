// made by tuukkas nov 2023-2025

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <dirent.h>
#include <nlohmann/json.hpp>
#include "arenafunctions.h"
#include "texturehandling.h"
#include "rwmath.h"

namespace fs = std::filesystem;

std::unordered_map<uint32_t, std::string> elementTypesxbox = {
    {0x00000000, "ELEMENTTYPE_NA"},
    {0x00000001, "ELEMENTTYPE_XYZ"},
    {0x00000002, "ELEMENTTYPE_XYZRHW"},
    {0x00000003, "ELEMENTTYPE_NORMAL"},
    {0x00000004, "ELEMENTTYPE_VERTEXCOLOR"},
    {0x00000005, "ELEMENTTYPE_PRELIT"},
    {0x00000006, "ELEMENTTYPE_TEX0"},
    {0x00000007, "ELEMENTTYPE_TEX1"},
    {0x00000008, "ELEMENTTYPE_TEX2"},
    {0x00000009, "ELEMENTTYPE_TEX3"},
    {0x0000000A, "ELEMENTTYPE_TEX4"},
    {0x0000000B, "ELEMENTTYPE_TEX5"},
    {0x0000000C, "ELEMENTTYPE_TEX6"},
    {0x0000000D, "ELEMENTTYPE_TEX7"},
    {0x0000000E, "ELEMENTTYPE_INDICES"},
    {0x0000000F, "ELEMENTTYPE_WEIGHTS"},
    {0x00000010, "ELEMENTTYPE_XYZ2"},
    {0x00000011, "ELEMENTTYPE_NORMAL2"},
    {0x00000012, "ELEMENTTYPE_XZ"},
    {0x00000013, "ELEMENTTYPE_Y"},
    {0x00000014, "ELEMENTTYPE_Y2"},
    {0x00000015, "ELEMENTTYPE_TANGENT"},
    {0x00000016, "ELEMENTTYPE_BINORMAL"},
    {0x00000017, "ELEMENTTYPE_SPECULAR"},
    {0x00000018, "ELEMENTTYPE_FOG"},
    {0x00000019, "ELEMENTTYPE_PSIZE"},
    {0x0000001A, "ELEMENTTYPE_INDICES2"},
    {0x0000001B, "ELEMENTTYPE_WEIGHTS2"},
    {0x0000001C, "ELEMENTTYPE_MAX"},
    {0x7FFFFFFF, "ELEMENTTYPE_FORCEENUMSIZEINT"},
    {0xFFFFFFFF, "VERTEXFORMAT_UNUSED"},
    {0x002C83A4, "VERTEXFORMAT_FLOAT1"},
    {0x002C23A5, "VERTEXFORMAT_FLOAT2"},
    {0x002A23B9, "VERTEXFORMAT_FLOAT3"},
    {0x001A23A6, "VERTEXFORMAT_FLOAT4"},
    {0x00182886, "VERTEXFORMAT_D3DCOLOR"},
    {0x001A2286, "VERTEXFORMAT_UBYTE4"},
    {0x001A2086, "VERTEXFORMAT_UBYTE4N"},
    {0x002C2359, "VERTEXFORMAT_SHORT2"},
    {0x001A235A, "VERTEXFORMAT_SHORT4"},
    {0x002C2159, "VERTEXFORMAT_SHORT2N"},
    {0x001A215A, "VERTEXFORMAT_SHORT4N"},
    {0x002C2059, "VERTEXFORMAT_USHORT2N"},
    {0x001A205A, "VERTEXFORMAT_USHORT4N"},
    {0x002A2287, "VERTEXFORMAT_UDEC3"},
    {0x002A2187, "VERTEXFORMAT_DEC3N"},
    {0x002A2190, "VERTEXFORMAT_HEND3N"},
    {0x002C235F, "VERTEXFORMAT_FLOAT16_2"},
    {0x001A2360, "VERTEXFORMAT_FLOAT16_4"},
};

enum _D3DPRIMITIVETYPE {
    D3DPT_POINTLIST = 1,
    D3DPT_LINELIST = 2,
    D3DPT_LINESTRIP = 3,
    D3DPT_TRIANGLELIST = 4,
    D3DPT_TRIANGLESTRIP = 5,
    D3DPT_TRIANGLEFAN = 6,
    D3DPT_FORCE_DWORD = 0x7fffffff
};

std::unordered_map<uint16_t, std::string> vertexTypesps3 = {
    {0x01, "CELL_GCM_VERTEX_S1"}, //Use an array of a 16-bit signed integer-type vertex attribute Value normalized to[-1 - 1] will be passed to the vertex shader
    {0x02, "CELL_GCM_VERTEX_F"}, //Use an array of a 32-bit float-type vertex attribute Not normalized
    {0x03, "CELL_GCM_VERTEX_SF"}, //Use an array of a 16-bit float-type vertex attribute Not normalized
    {0x04, "CELL_GCM_VERTEX_UB"}, //Use an array of an 8-bit unsigned integer-type vertex attribute Value normalized to[0 - 1] will be passed to the vertex shader
    {0x05, "CELL_GCM_VERTEX_S32K"}, //Use an array of a 16-bit signed integer-type vertex attribute Not normalized
    {0x06, "CELL_GCM_VERTEX_CMP"}, //Use an array of a vertex attribute compressed to 32 bits (11:11:10)
    {0x07, "CELL_GCM_VERTEX_UB256"}, //Use an array of an 8-bit unsigned integer-type vertex attribute Not normalized
};
std::unordered_map<uint16_t, std::string> elementTypesps3 = {
    {0x00, "ELEMENTTYPE_XYZ"},
    {0x01, "ELEMENTTYPE_WEIGHTS"},
    {0x02, "ELEMENTTYPE_NORMAL"},
    {0x03, "ELEMENTTYPE_VERTEXCOLOR"},
    {0x04, "ELEMENTTYPE_SPECULAR"},
    {0x05, "unassigned"},
    {0x06, "unassigned"},
    {0x07, "ELEMENTTYPE_INDICES"},
    {0x08, "ELEMENTTYPE_TEX0"},
    {0x09, "ELEMENTTYPE_TEX1"},
    {0x0A, "ELEMENTTYPE_TEX2"},
    {0x0B, "ELEMENTTYPE_TEX3"},
    {0x0C, "ELEMENTTYPE_TEX4"},
    {0x0D, "ELEMENTTYPE_TEX5"},
    {0x0E, "ELEMENTTYPE_TEX6"}, //tanget
    {0x0F, "ELEMENTTYPE_TEX7"}, //binormal
    {0x10, "ELEMENTTYPE_NUM"},
    {0xFF, "ELEMENTTYPE_NA"},
};

std::unordered_map<uint8_t, std::string> Attribute = {
    {0, "GX_VA_PTNMTXIDX"},
    {1, "GX_VA_TEX0MTXIDX"},
    {2, "GX_VA_TEX1MTXIDX"},
    {3, "GX_VA_TEX2MTXIDX"},
    {4, "GX_VA_TEX3MTXIDX"},
    {5, "GX_VA_TEX4MTXIDX"},
    {6, "GX_VA_TEX5MTXIDX"},
    {7, "GX_VA_TEX6MTXIDX"},
    {8, "GX_VA_TEX7MTXIDX"},
    {9, "GX_VA_POS"},
    {10, "GX_VA_NRM"},
    {11, "GX_VA_CLR0"},
    {12, "GX_VA_CLR1"},
    {13, "GX_VA_TEX0"},
    {14, "GX_VA_TEX1"},
    {15, "GX_VA_TEX2"},
    {16, "GX_VA_TEX3"},
    {17, "GX_VA_TEX4"},
    {18, "GX_VA_TEX5"},
    {19, "GX_VA_TEX6"},
    {20, "GX_VA_TEX7"},
    {21, "GX_POSMTXARRAY"},
    {22, "GX_NRMMTXARRAY"},
    {23, "GX_TEXMTXARRAY"},
    {24, "GX_LIGHTARRAY"},
    {25, "GX_VA_NBT"},
    {26, "GX_VA_MAXATTR"},
    {0xff, "GX_VA_NULL"}
};

std::unordered_map<uint8_t, std::string> componentTypeMapping = {
    {9, "POS"},
    {10, "NRM"},
    {11, "CLR"},
    {12, "CLR"},
    {13, "TEX"},
    {14, "TEX"},
    {15, "TEX"},
    {16, "TEX"},
    {17, "TEX"},
    {18, "TEX"},
    {19, "TEX"},
    {20, "TEX"},
    {21, "POS"},
    {22, "NRM"},
    {23, "TEX"},
};

std::unordered_map<uint8_t, std::string> AttrType{
    {0, "GX_NONE"},
    {1, "GX_DIRECT"},
    {2, "GX_INDEX8"},
    {3, "GX_INDEX16"},
};

std::unordered_map<uint8_t, std::string> unknown{
    {0, "GX_NONE"},
    {1, "GX_DIRECT"},
    {2, "GX_INDEX8"},
    {3, "GX_INDEX16"},
};

std::unordered_map<uint8_t, std::string> ComponentTypeCLR = {
    {0, "GX_CLR_RGB"},
    {1, "GX_CLR_RGBA"},
};

std::unordered_map<uint8_t, std::string> ComponentTypeNRM = {
    {0, "GX_NRM_XYZ"},
    {1, "GX_NRM_NBT"},
    {2, "GX_NRM_NBT3"},
};

std::unordered_map<uint8_t, std::string> ComponentTypePOS = {
    {0, "GX_POS_XY"},
    {1, "GX_POS_XYZ"},
};

std::unordered_map<uint8_t, std::string> ComponentTypeTEX = {
    {0, "GX_TEX_S"},
    {1, "GX_TEX_ST"}
};

std::unordered_map<uint8_t, std::string> ComponentSizeTEX = {
    {0, "GX_RGB565"},
    {1, "GX_RGB8"},
    {2, "GX_RGBX8"},
    {3, "GX_RGBA4"},
    {4, "GX_RGBA6"},
    {5, "GX_RGBA8"},
};

std::unordered_map<uint8_t, std::string> ComponentSizeOTHER = {
    {0, "GX_U8"},
    {1, "GX_S8"},
    {2, "GX_U16"},
    {3, "GX_S16"},
    {4, "GX_F32"},
};

std::unordered_map<uint16_t, std::string> elementClass = {
    {0x00, "ELEMENTCLASS_NA"},
    {0x01, "ELEMENTCLASS_PERVERTEX"},
    {0x02, "ELEMENTCLASS_PERINSTANCE"},
    {0x7FFFFFFF, "ELEMENTCLASS_FORCEENUMSIZEINT"},
};
std::unordered_map<uint8_t, std::string> indexedUsage = {
    {0x00, "NA"}, //0
    {0x01, "D3DDECLUSAGE_POSITION"}, //0
    {0x02, "D3DDECLUSAGE_POSITION"}, //0
    {0x03, "D3DDECLUSAGE_NORMAL"}, //0
    {0x04, "D3DDECLUSAGE_COLOR"}, //0
    {0x05, "D3DDECLUSAGE_COLOR"}, //1
    {0x06, "D3DDECLUSAGE_TEXCOORD"}, //0
    {0x07, "D3DDECLUSAGE_TEXCOORD"}, //1
    {0x08, "D3DDECLUSAGE_TEXCOORD"}, //2
    {0x09, "D3DDECLUSAGE_TEXCOORD"}, //3
    {0x0A, "D3DDECLUSAGE_TEXCOORD"}, //4
    {0x0B, "D3DDECLUSAGE_TEXCOORD"}, //5
    {0x0C, "D3DDECLUSAGE_TEXCOORD"}, //6
    {0x0D, "D3DDECLUSAGE_TEXCOORD"}, //7
    {0x0E, "D3DDECLUSAGE_BLENDINDICES"}, //0
    {0x0F, "D3DDECLUSAGE_BLENDWEIGHT"}, //0
    {0x10, "D3DDECLUSAGE_POSITION"}, //1
    {0x11, "D3DDECLUSAGE_NORMAL"}, //1
    {0x12, "D3DDECLUSAGE_POSITION"}, //0
    {0x13, "D3DDECLUSAGE_POSITION"}, //0
    {0x14, "D3DDECLUSAGE_POSITION"}, //0
    {0x15, "D3DDECLUSAGE_TANGENT"}, //0
    {0x16, "D3DDECLUSAGE_BINORMAL"}, //0
    {0x17, "D3DDECLUSAGE_COLOR"}, //2
    {0x18, "D3DDECLUSAGE_FOG"}, //0
    {0x19, "D3DDECLUSAGE_PSIZE"}, //0
    {0x1A, "D3DDECLUSAGE_BLENDINDICES"}, //1
    {0x1B, "D3DDECLUSAGE_BLENDWEIGHT"}, //1
};

std::unordered_map<uint8_t, std::string> d3ddeclUsage = {
    {0x00, "D3DDECLUSAGE_POSITION"},	    //Position data.
    {0x01, "D3DDECLUSAGE_BLENDWEIGHT"},	    //Blending weight data.
    {0x02, "D3DDECLUSAGE_BLENDINDICES"},	//Blending indices data.
    {0x03, "D3DDECLUSAGE_NORMAL"},	        //Vertex normal data.
    {0x04, "D3DDECLUSAGE_PSIZE"},		    //Point size data.
    {0x05, "D3DDECLUSAGE_TEXCOORD"},	    //Texture coordinate data.
    {0x06, "D3DDECLUSAGE_TANGENT"},	        //Vertex tangent data.
    {0x07, "D3DDECLUSAGE_BINORMAL"},	    //Vertex binormal data.
    {0x08, "D3DDECLUSAGE_TESSFACTOR"},      //Single positive floating - point value.Use D3DDECLUSAGE_TESSFACTOR with a usage index of 0 to specify a tessellation factor used in the tessellation unit to control the rate of tessellation.
    {0x0A, "D3DDECLUSAGE_COLOR"},		    //Vertex data contains diffuse or specular color.
    {0x0B, "D3DDECLUSAGE_FOG"},		        //Vertex data contains fog data.
    {0x0C, "D3DDECLUSAGE_DEPTH"},	        //Vertex data contains depth data.
    {0x0D, "D3DDECLUSAGE_SAMPLE"},	        //Vertex data contains sampler data.
};

enum class RwObjectType : uint32_t {
    RWOBJECTTYPE_NULL = 0x00000000,
    RWOBJECTTYPE_NA = 0x00010000,
    RWOBJECTTYPE_ARENA = 0x00010001,
    RWOBJECTTYPE_RAW = 0x00010002,
    RWOBJECTTYPE_SUBREFERENCE = 0x00010003,
    RWOBJECTTYPE_SECTIONMANIFEST = 0x00010004,
    RWOBJECTTYPE_SECTIONTYPES = 0x00010005,
    RWOBJECTTYPE_SECTIONEXTERNALARENAS = 0x00010006,
    RWOBJECTTYPE_SECTIONSUBREFERENCES = 0x00010007,
    RWOBJECTTYPE_SECTIONATOMS = 0x00010008,
    RWOBJECTTYPE_DEFARENAIMPORTS = 0x00010009,
    RWOBJECTTYPE_DEFARENAEXPORTS = 0x0001000A,
    RWOBJECTTYPE_DEFARENATYPES = 0x0001000B,
    RWOBJECTTYPE_DEFARENADEFINEDARENAID = 0x0001000C,
    RWOBJECTTYPE_ATTRIBUTEPACKET = 0x0001000D,
    RWOBJECTTYPE_ATTRIBUTEPACKET_DELEGATE = 0x0001000E,
    RWOBJECTTYPE_BITTABLE = 0x0001000F,
    RWOBJECTTYPE_ARENALOCALATOMTABLE = 0x00010010,
    RWOBJECTTYPE_BASERESOURCE_START = 0x00010030,
    RWOBJECTTYPE_BASERESOURCE_1 = 0x00010031,
    RWOBJECTTYPE_BASERESOURCE_2 = 0x00010032,
    RWOBJECTTYPE_BASERESOURCE_3 = 0x00010033,
    RWOBJECTTYPE_BASERESOURCE_4 = 0x00010034,
    RWOBJECTTYPE_BASERESOURCE_5 = 0x00010035,
    RWOBJECTTYPE_BASERESOURCE_6 = 0x00010036,
    RWOBJECTTYPE_BASERESOURCE_7 = 0x00010037,
    RWOBJECTTYPE_BASERESOURCE_8 = 0x00010038,
    RWOBJECTTYPE_BASERESOURCE_9 = 0x00010039,
    RWOBJECTTYPE_BASERESOURCE_10 = 0x0001003A,
    RWOBJECTTYPE_BASERESOURCE_11 = 0x0001003B,
    RWOBJECTTYPE_BASERESOURCE_12 = 0x0001003C,
    RWOBJECTTYPE_BASERESOURCE_13 = 0x0001003D,
    RWOBJECTTYPE_BASERESOURCE_14 = 0x0001003E,
    RWOBJECTTYPE_BASERESOURCE_RESERVEDTO = 0x0001003F,

    RWGOBJECTTYPE_NA = 0x00020000,
    RWGOBJECTTYPE_CAMERA = 0x00020001,
    RWGOBJECTTYPE_PALETTE_1 = 0x00020002,
    RWGOBJECTTYPE_RASTER = 0x00020003,
    RWGOBJECTTYPE_VDES = 0x00020004,
    RWGOBJECTTYPE_VBUF = 0x00020005,
    RWGOBJECTTYPE_IDES = 0x00020006,
    RWGOBJECTTYPE_IBUF = 0x00020007,
    RWGOBJECTTYPE_LIGHT = 0x00020008,
    RWGOBJECTTYPE_MESH = 0x00020009,
    RWGOBJECTTYPE_SHADER = 0x0002000A,
    RWGOBJECTTYPE_COMPILEDSTATE = 0x0002000B,
    RWGOBJECTTYPE_RENDEROBJECT = 0x0002000C,
    RWGOBJECTTYPE_GSDATA = 0x0002000D,
    RWGOBJECTTYPE_VERTEXSHADER = 0x0002000E,
    RWGOBJECTTYPE_VERTEXDATA = 0x0002000F,
    RWGOBJECTTYPE_INDEXDATA = 0x00020010,
    RWGOBJECTTYPE_RASTERDATA = 0x00020011,
    RWGOBJECTTYPE_BUILDSTATE = 0x00020012,
    RWGOBJECTTYPE_PIXELSHADER = 0x00020013,
    RWGOBJECTTYPE_PROFILEMESH = 0x00020015,
    RWGOBJECTTYPE_DESIGNVIEWOBJECT = 0x00020016,
    RWGOBJECTTYPE_PROFILERENDEROBJECT = 0x00020017,
    RWGOBJECTTYPE_IMAGE = 0x00020018,
    RWGOBJECTTYPE_RENDEROBJECTCONTAINER = 0x00020019,
    RWGOBJECTTYPE_MESHCOMPILEDSTATELINK = 0x0002001A,
    RWGOBJECTTYPE_SHADERCODE = 0x0002001B,
    RWGOBJECTTYPE_FONT = 0x00020020,
    RWGOBJECTTYPE_GLYPHTABLE = 0x00020021,
    RWGOBJECTTYPE_KERNTABLE = 0x00020022,
    RWGOBJECTTYPE_PAGETABLE = 0x00020023,
    RWGOBJECTTYPE_RASTERTEXTURE = 0x00020024,
    RWGOBJECTTYPE_FACENAME = 0x00020025,
    RWGOBJECTTYPE_UPDATELOCATOR = 0x0002007F,
    RWGOBJECTTYPE_MESHHELPER = 0x00020081,
    RWGOBJECTTYPE_NA_2 = 0x000200E0,
    RWGOBJECTTYPE_VIEWPORTPARAMETERS = 0x000200E1,
    RWGOBJECTTYPE_SCISSORRECTPARAMETERS = 0x000200E2,
    RWGOBJECTTYPE_CLEARCOLORPARAMETERS = 0x000200E3,
    RWGOBJECTTYPE_CLEARDEPTHSTENCILPARAMETERS = 0x000200E4,
    RWGOBJECTTYPE_DRAWINDEXEDPARAMETERS = 0x000200E5,
    RWGOBJECTTYPE_DRAWPARAMETERS = 0x000200E6,
    RWGOBJECTTYPE_PIXELBUFFER = 0x000200E7,
    RWGOBJECTTYPE_TEXTURE = 0x000200E8,
    RWGOBJECTTYPE_VERTEXDESCRIPTOR = 0x000200E9,
    RWGOBJECTTYPE_VERTEXBUFFER = 0x000200EA,
    RWGOBJECTTYPE_INDEXBUFFER = 0x000200EB,
    RWGOBJECTTYPE_PROGRAMBUFFER = 0x000200EC,
    RWGOBJECTTYPE_STATEBUFFER = 0x000200ED,
    RWGOBJECTTYPE_PROGRAMSTATEBUFFER = 0x000200EE,
    RWGOBJECTTYPE_PROGRAMSTATES = 0x000200EF,
    RWGOBJECTTYPE_SAMPLERSTATE = 0x000200F1,
    RWGOBJECTTYPE_RENDERTARGETSTATE = 0x000200F3,
    RWGOBJECTTYPE_BLENDSTATE = 0x000200F4,
    RWGOBJECTTYPE_DEPTHSTENCILSTATE = 0x000200F5,
    RWGOBJECTTYPE_RASTERIZERSTATE = 0x000200F6,
    RWGOBJECTTYPE_VERTEXPROGRAMSTATE = 0x000200F7,
    RWGOBJECTTYPE_PROGRAMSTATEHANDLE = 0x000200F8,
    RWGOBJECTTYPE_DRAWINSTANCEDPARAMETERS = 0x000200F9,
    RWGOBJECTTYPE_DRAWINDEXEDINSTANCEDPARAMETERS = 0x000200FA,
    RWGOBJECTTYPE_PALETTE_2 = 0x000200FB,

    OBJECTTYPE_NA = 0x00040000,
    OBJECTTYPE_PART = 0x00040001,
    OBJECTTYPE_PARTDEFINITION = 0x00040002,
    OBJECTTYPE_JOINTEDPAIR = 0x00040003,
    OBJECTTYPE_JOINTLIMIT = 0x00040004,
    OBJECTTYPE_JOINTSKELETON = 0x00040005,
    OBJECTTYPE_ASSEMBLY = 0x00040006,
    OBJECTTYPE_ASSEMBLYDEFINITION = 0x00040007,

    OBJECTTYPE_NA_2 = 0x00070000,
    OBJECTTYPE_KEYFRAMEANIM = 0x00070001,
    OBJECTTYPE_SKELETON = 0x00070002,
    OBJECTTYPE_ANIMATIONSKIN = 0x00070003,
    OBJECTTYPE_INTERPOLATOR = 0x00070004,
    OBJECTTYPE_FEATHERINTERPOLATOR = 0x00070005,
    OBJECTTYPE_ONEBONEIK = 0x00070006,
    OBJECTTYPE_TWOBONEIK = 0x00070007,
    OBJECTTYPE_BLENDER = 0x00070008,
    OBJECTTYPE_WEIGHTEDBLENDER = 0x00070009,
    OBJECTTYPE_REMAPPER = 0x0007000A,
    OBJECTTYPE_SKELETONSINK = 0x0007000B,
    OBJECTTYPE_SKINSINK = 0x0007000C,
    OBJECTTYPE_LIGHTSINK = 0x0007000D,
    OBJECTTYPE_CAMERASINK = 0x0007000E,
    OBJECTTYPE_SKINMATRIXBUFFER = 0x0007000F,
    OBJECTTYPE_TWEAKCONTROLLER = 0x00070010,
    OBJECTTYPE_SHADERSINK = 0x00070011,

    RWCOBJECTTYPE_NA = 0x00080000,
    RWCOBJECTTYPE_VOLUME = 0x00080001,
    RWCOBJECTTYPE_SIMPLEMAPPEDARRAY = 0x00080002,
    RWCOBJECTTYPE_TRIANGLEKDTREEPROCEDURAL = 0x00080003,
    RWCOBJECTTYPE_KDTREEMAPPEDARRAY = 0x00080004,
    RWCOBJECTTYPE_BBOX = 0x00080005,
    RWCOBJECTTYPE_CLUSTEREDMESH = 0x00080006,
    RWCOBJECTTYPE_OCTREE = 0x00080008,

    RWOBJECTTYPE_RENDERMESHDATA = 0x00EB0000,
    RWOBJECTTYPE_RENDERMODELDATA = 0x00EB0001,
    RWOBJECTTYPE_SIMPLETRIMESHDATA = 0x00EB0003,
    RWOBJECTTYPE_SPLINEDATA = 0x00EB0004,
    RWOBJECTTYPE_RENDERMATERIALDATA = 0x00EB0005,
    RWOBJECTTYPE_COLLISIONMATERIALDATA = 0x00EB0006,
    RWOBJECTTYPE_ROLLERDESCDATA = 0x00EB0007,
    RWOBJECTTYPE_VERSIONDATA = 0x00EB0008,
    RWOBJECTTYPE_LOCATIONDESCDATA = 0x00EB0009,
    RWOBJECTTYPE_COLLISIONMODELDATA = 0x00EB000A,
    RWOBJECTTYPE_TABLEOFCONTENTS = 0x00EB000B,
    RWOBJECTTYPE_COLLISIONBEZIERDATA = 0x00EB000C,
    RWOBJECTTYPE_INSTANCEDATA = 0x00EB000D,
    RWOBJECTTYPE_RENDERBLENDSHAPEEDATA = 0x00EB000E,
    RWOBJECTTYPE_WORLDPAINTERLAYERDATA = 0x00EB000F,
    RWOBJECTTYPE_WORLDPAINTERQUADTREEDATA = 0x00EB0010,
    RWOBJECTTYPE_WORLDPAINTERDICTIONARYDATA = 0x00EB0011,
    RWOBJECTTYPE_NAVMESHDATA = 0x00EB0012,
    RWOBJECTTYPE_RAINDATA = 0x00EB0013,
    RWOBJECTTYPE_AIPATHDATA = 0x00EB0014,
    RWOBJECTTYPE_STATSDATA = 0x00EB0015,
    RWOBJECTTYPE_MASSIVEDATA = 0x00EB0016,
    RWOBJECTTYPE_DEPTHMAPDATA = 0x00EB0017,
    RWOBJECTTYPE_LIONDATA = 0x00EB0018,
    RWOBJECTTYPE_TRIGGERINSTANCEDATA = 0x00EB0019,
    RWOBJECTTYPE_WAYPOINTDATA = 0x00EB001A,
    RWOBJECTTYPE_EMBEDDEDDATA = 0x00EB001B,
    RWOBJECTTYPE_EMITTERWAYPOINTDATA = 0x00EB001C,
    RWOBJECTTYPE_DMODATA = 0x00EB001D,
    RWOBJECTTYPE_HOTPOINTDATA = 0x00EB001E,
    RWOBJECTTYPE_GRABDATA = 0x00EB001F,
    RWOBJECTTYPE_SPATIALMAP = 0x00EB0020,
    RWOBJECTTYPE_VISUALINDICATORDATA = 0x00EB0021,
    RWOBJECTTYPE_NAVMESH2DATA = 0x00EB0022,
    RWOBJECTTYPE_RENDEROPTIMESHDATA = 0x00EB0023,
    RWOBJECTTYPE_IRRADIANCEDATA = 0x00EB0024,
    RWOBJECTTYPE_ANTIFRUSTUMDATA = 0x00EB0025,
    RWOBJECTTYPE_BLOBDATA = 0x00EB0026,
    RWOBJECTTYPE_NAVPOWERDATA = 0x00EB0027,
    RWOBJECTTYPE_TEAMBRANDDATA = 0x00EB0028,
    RWOBJECTTYPE_WORLDSCRIPTINSTANCEDATA = 0x00EB0029,

    RWOBJECTTYPE_SPLINESUBREF = 0x00EB0064,
    RWOBJECTTYPE_ROLLERDESCSUBREF = 0x00EB0065,
    RWOBJECTTYPE_RENDERMATERIALSUBREF = 0x00EB0066,
    RWOBJECTTYPE_COLLISIONMATERIALSUBREF = 0x00EB0067,
    RWOBJECTTYPE_LOCATIONDESCSUBREF = 0x00EB0068,
    RWOBJECTTYPE_INSTANCESUBREF = 0x00EB0069,
    RWOBJECTTYPE_WAYPOINTSUBREF = 0x00EB006A,
    RWOBJECTTYPE_TRIGGERINSTANCESUBREF = 0x00EB006B,
    RWOBJECTTYPE_EMITTERWAYPOINTSUBREF = 0x00EB006C,
    RWOBJECTTYPE_DMOSUBREF = 0x00EB006D,
    RWOBJECTTYPE_HOTPOINTSUBREF = 0x00EB006E,
    RWOBJECTTYPE_GRABSUBREF = 0x00EB006F,
    RWOBJECTTYPE_VISUALINDICATORSUBREF = 0x00EB0070,

    ARENADICTIONARY = 0x00EC0010,
    RWCOBJECTTYPE_FORCENUMSIZEINT = 0x7FFFFFFF,
    UNKNOWN = 0x00FF0000,
};

std::unordered_map<uint32_t, std::string> objectTypes = {
    {0x00000000, "RWOBJECTTYPE_NULL"},
    {0x00010000, "RWOBJECTTYPE_NA"},
    {0x00010001, "RWOBJECTTYPE_ARENA"},
    {0x00010002, "RWOBJECTTYPE_RAW"},
    {0x00010003, "RWOBJECTTYPE_SUBREFERENCE"},
    {0x00010004, "RWOBJECTTYPE_SECTIONMANIFEST"},
    {0x00010005, "RWOBJECTTYPE_SECTIONTYPES"},
    {0x00010006, "RWOBJECTTYPE_SECTIONEXTERNALARENAS"},
    {0x00010007, "RWOBJECTTYPE_SECTIONSUBREFERENCES"},
    {0x00010008, "RWOBJECTTYPE_SECTIONATOMS"},
    {0x00010009, "RWOBJECTTYPE_DEFARENAIMPORTS"},
    {0x0001000A, "RWOBJECTTYPE_DEFARENAEXPORTS"},
    {0x0001000B, "RWOBJECTTYPE_DEFARENATYPES"},
    {0x0001000C, "RWOBJECTTYPE_DEFARENADEFINEDARENAID"},
    {0x0001000D, "RWOBJECTTYPE_ATTRIBUTEPACKET"},
    {0x0001000E, "RWOBJECTTYPE_ATTRIBUTEPACKET_DELEGATE"},
    {0x0001000F, "RWOBJECTTYPE_BITTABLE"},
    {0x00010010, "RWOBJECTTYPE_ARENALOCALATOMTABLE"},
    {0x00010030, "RWOBJECTTYPE_BASERESOURCE_START"},
    {0x00010031, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010032, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010033, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010034, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010035, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010036, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010037, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010038, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010039, "RWOBJECTTYPE_BASERESOURCE"},
    {0x0001003A, "RWOBJECTTYPE_BASERESOURCE"},
    {0x0001003B, "RWOBJECTTYPE_BASERESOURCE"},
    {0x0001003C, "RWOBJECTTYPE_BASERESOURCE"},
    {0x0001003D, "RWOBJECTTYPE_BASERESOURCE"},
    {0x0001003E, "RWOBJECTTYPE_BASERESOURCE"},
    {0x0001003F, "RWOBJECTTYPE_BASERESOURCE_RESERVEDTO"},
    {0x00020000, "RWGOBJECTTYPE_NA"},
    {0x00020001, "RWGOBJECTTYPE_CAMERA"},
    {0x00020002, "RWGOBJECTTYPE_PALETTE"},
    {0x00020003, "RWGOBJECTTYPE_RASTER"},
    {0x00020004, "RWGOBJECTTYPE_VDES"},
    {0x00020005, "RWGOBJECTTYPE_VBUF"},
    {0x00020006, "RWGOBJECTTYPE_IDES"},
    {0x00020007, "RWGOBJECTTYPE_IBUF"},
    {0x00020008, "RWGOBJECTTYPE_LIGHT"},
    {0x00020009, "RWGOBJECTTYPE_MESH"},
    {0x0002000A, "RWGOBJECTTYPE_SHADER"},
    {0x0002000B, "RWGOBJECTTYPE_COMPILEDSTATE"},
    {0x0002000C, "RWGOBJECTTYPE_RENDEROBJECT"},
    {0x0002000D, "RWGOBJECTTYPE_GSDATA"},
    {0x0002000E, "RWGOBJECTTYPE_VERTEXSHADER"},
    {0x0002000F, "RWGOBJECTTYPE_VERTEXDATA"},
    {0x00020010, "RWGOBJECTTYPE_INDEXDATA"},
    {0x00020011, "RWGOBJECTTYPE_RASTERDATA"},
    {0x00020012, "RWGOBJECTTYPE_BUILDSTATE"},
    {0x00020013, "RWGOBJECTTYPE_PIXELSHADER"},
    {0x00020015, "RWGOBJECTTYPE_PROFILEMESH"},
    {0x00020016, "RWGOBJECTTYPE_DESIGNVIEWOBJECT"},
    {0x00020017, "RWGOBJECTTYPE_PROFILERENDEROBJECT"},
    {0x00020018, "RWGOBJECTTYPE_IMAGE"},
    {0x00020019, "RWGOBJECTTYPE_RENDEROBJECTCONTAINER"},
    {0x0002001A, "RWGOBJECTTYPE_MESHCOMPILEDSTATELINK"},
    {0x0002001B, "RWGOBJECTTYPE_SHADERCODE"},
    {0x00020020, "RWGOBJECTTYPE_FONT"},
    {0x00020021, "RWGOBJECTTYPE_GLYPHTABLE"},
    {0x00020022, "RWGOBJECTTYPE_KERNTABLE"},
    {0x00020023, "RWGOBJECTTYPE_PAGETABLE"},
    {0x00020024, "RWGOBJECTTYPE_RASTERTEXTURE"},
    {0x00020025, "RWGOBJECTTYPE_FACENAME"},
    {0x0002007F, "RWGOBJECTTYPE_UPDATELOCATOR"},
    {0x00020081, "RWGOBJECTTYPE_MESHHELPER"},
    {0x000200E0, "RWGOBJECTTYPE_NA"},
    {0x000200E1, "RWGOBJECTTYPE_VIEWPORTPARAMETERS"},
    {0x000200E2, "RWGOBJECTTYPE_SCISSORRECTPARAMETERS"},
    {0x000200E3, "RWGOBJECTTYPE_CLEARCOLORPARAMETERS"},
    {0x000200E4, "RWGOBJECTTYPE_CLEARDEPTHSTENCILPARAMETERS"},
    {0x000200E5, "RWGOBJECTTYPE_DRAWINDEXEDPARAMETERS"},
    {0x000200E6, "RWGOBJECTTYPE_DRAWPARAMETERS"},
    {0x000200E7, "RWGOBJECTTYPE_PIXELBUFFER"},
    {0x000200E8, "RWGOBJECTTYPE_TEXTURE"},
    {0x000200E9, "RWGOBJECTTYPE_VERTEXDESCRIPTOR"},
    {0x000200EA, "RWGOBJECTTYPE_VERTEXBUFFER"},
    {0x000200EB, "RWGOBJECTTYPE_INDEXBUFFER"},
    {0x000200EC, "RWGOBJECTTYPE_PROGRAMBUFFER"},
    {0x000200ED, "RWGOBJECTTYPE_STATEBUFFER"},
    {0x000200EE, "RWGOBJECTTYPE_PROGRAMSTATEBUFFER" },
    {0x000200EF, "RWGOBJECTTYPE_PROGRAMSTATES" },
    {0x000200F1, "RWGOBJECTTYPE_SAMPLERSTATE" },
    {0x000200F3, "RWGOBJECTTYPE_RENDERTARGETSTATE" },
    {0x000200F4, "RWGOBJECTTYPE_BLENDSTATE" },
    {0x000200F5, "RWGOBJECTTYPE_DEPTHSTENCILSTATE" },
    {0x000200F6, "RWGOBJECTTYPE_RASTERIZERSTATE" },
    {0x000200F7, "RWGOBJECTTYPE_VERTEXPROGRAMSTATE" },
    {0x000200F8, "RWGOBJECTTYPE_PROGRAMSTATEHANDLE" },
    {0x000200F9, "RWGOBJECTTYPE_DRAWINSTANCEDPARAMETERS" },
    {0x000200FA, "RWGOBJECTTYPE_DRAWINDEXEDINSTANCEDPARAMETERS" },
    {0x000200FB, "RWGOBJECTTYPE_PALETTE" },
    {0x00040000, "OBJECTTYPE_NA" },
    {0x00040001, "OBJECTTYPE_PART" },
    {0x00040002, "OBJECTTYPE_PARTDEFINITION" },
    {0x00040003, "OBJECTTYPE_JOINTEDPAIR" },
    {0x00040004, "OBJECTTYPE_JOINTLIMIT" },
    {0x00040005, "OBJECTTYPE_JOINTSKELETON" },
    {0x00040006, "OBJECTTYPE_ASSEMBLY" },
    {0x00040007, "OBJECTTYPE_ASSEMBLYDEFINITION" },
    {0x00070000, "OBJECTTYPE_NA" },
    {0x00070001, "OBJECTTYPE_KEYFRAMEANIM" },
    {0x00070002, "OBJECTTYPE_SKELETON" },
    {0x00070003, "OBJECTTYPE_ANIMATIONSKIN" },
    {0x00070004, "OBJECTTYPE_INTERPOLATOR" },
    {0x00070005, "OBJECTTYPE_FEATHERINTERPOLATOR" },
    {0x00070006, "OBJECTTYPE_ONEBONEIK" },
    {0x00070007, "OBJECTTYPE_TWOBONEIK" },
    {0x00070008, "OBJECTTYPE_BLENDER" },
    {0x00070009, "OBJECTTYPE_WEIGHTEDBLENDER" },
    {0x0007000A, "OBJECTTYPE_REMAPPER" },
    {0x0007000B, "OBJECTTYPE_SKELETONSINK" },
    {0x0007000C, "OBJECTTYPE_SKINSINK" },
    {0x0007000D, "OBJECTTYPE_LIGHTSINK" },
    {0x0007000E, "OBJECTTYPE_CAMERASINK" },
    {0x0007000F, "OBJECTTYPE_SKINMATRIXBUFFER" },
    {0x00070010, "OBJECTTYPE_TWEAKCONTROLLER" },
    {0x00070011, "OBJECTTYPE_SHADERSINK" },
    {0x00080000, "RWCOBJECTTYPE_NA" },
    {0x00080001, "RWCOBJECTTYPE_VOLUME" },
    {0x00080002, "RWCOBJECTTYPE_SIMPLEMAPPEDARRAY" },
    {0x00080003, "RWCOBJECTTYPE_TRIANGLEKDTREEPROCEDURAL" },
    {0x00080004, "RWCOBJECTTYPE_KDTREEMAPPEDARRAY" },
    {0x00080005, "RWCOBJECTTYPE_BBOX" },
    {0x00080006, "RWCOBJECTTYPE_CLUSTEREDMESH" },
    {0x00080008, "RWCOBJECTTYPE_OCTREE" },
    {0x00EB0000, "RWOBJECTTYPE_RENDERMESHDATA" },
    {0x00EB0001, "RWOBJECTTYPE_RENDERMODELDATA" },
    {0x00EB0003, "RWOBJECTTYPE_SIMPLETRIMESHDATA"},
    {0x00EB0004, "RWOBJECTTYPE_SPLINEDATA" },
    {0x00EB0005, "RWOBJECTTYPE_RENDERMATERIALDATA" },
    {0x00EB0006, "RWOBJECTTYPE_COLLISIONMATERIALDATA" },
    {0x00EB0007, "RWOBJECTTYPE_ROLLERDESCDATA" },
    {0x00EB0008, "RWOBJECTTYPE_VERSIONDATA" },
    {0x00EB0009, "RWOBJECTTYPE_LOCATIONDESCDATA" },
    {0x00EB000A, "RWOBJECTTYPE_COLLISIONMODELDATA" },
    {0x00EB000B, "RWOBJECTTYPE_TABLEOFCONTENTS" },
    {0x00EB000C, "RWOBJECTTYPE_COLLISIONBEZIERDATA" },
    {0x00EB000D, "RWOBJECTTYPE_INSTANCEDATA" },
    {0x00EB000E, "RWOBJECTTYPE_RENDERBLENDSHAPEEDATA" },
    {0x00EB000F, "RWOBJECTTYPE_WORLDPAINTERLAYERDATA" },
    {0x00EB0010, "RWOBJECTTYPE_WORLDPAINTERQUADTREEDATA" },
    {0x00EB0011, "RWOBJECTTYPE_WORLDPAINTERDICTIONARYDATA" },
    {0x00EB0012, "RWOBJECTTYPE_NAVMESHDATA" },
    {0x00EB0013, "RWOBJECTTYPE_RAINDATA" },
    {0x00EB0014, "RWOBJECTTYPE_AIPATHDATA" },
    {0x00EB0015, "RWOBJECTTYPE_STATSDATA" },
    {0x00EB0016, "RWOBJECTTYPE_MASSIVEDATA" },
    {0x00EB0017, "RWOBJECTTYPE_DEPTHMAPDATA" },
    {0x00EB0018, "RWOBJECTTYPE_LIONDATA" },
    {0x00EB0019, "RWOBJECTTYPE_TRIGGERINSTANCEDATA" },
    {0x00EB001A, "RWOBJECTTYPE_WAYPOINTDATA" },
    {0x00EB001B, "RWOBJECTTYPE_EMBEDDEDDATA" },
    {0x00EB001C, "RWOBJECTTYPE_EMITTERWAYPOINTDATA" },
    {0x00EB001D, "RWOBJECTTYPE_DMODATA" },
    {0x00EB001E, "RWOBJECTTYPE_HOTPOINTDATA" },
    {0x00EB001F, "RWOBJECTTYPE_GRABDATA" },
    {0x00EB0020, "RWOBJECTTYPE_SPATIALMAP" },
    {0x00EB0021, "RWOBJECTTYPE_VISUALINDICATORDATA" },
    {0x00EB0022, "RWOBJECTTYPE_NAVMESH2DATA" },
    {0x00EB0023, "RWOBJECTTYPE_RENDEROPTIMESHDATA" },
    {0x00EB0024, "RWOBJECTTYPE_IRRADIANCEDATA" },
    {0x00EB0025, "RWOBJECTTYPE_ANTIFRUSTUMDATA" },
    {0x00EB0026, "RWOBJECTTYPE_BLOBDATA" },
    {0x00EB0027, "RWOBJECTTYPE_NAVPOWERDATA" },
    {0x00EB0028, "RWOBJECTTYPE_TEAMBRANDDATA" },
    {0x00EB0029, "RWOBJECTTYPE_WORLDSCRIPTINSTANCEDATA" },
    {0x00EB0064, "RWOBJECTTYPE_SPLINESUBREF" },
    {0x00EB0065, "RWOBJECTTYPE_ROLLERDESCSUBREF" },
    {0x00EB0066, "RWOBJECTTYPE_RENDERMATERIALSUBREF" },
    {0x00EB0067, "RWOBJECTTYPE_COLLISIONMATERIALSUBREF" },
    {0x00EB0068, "RWOBJECTTYPE_LOCATIONDESCSUBREF" },
    {0x00EB0069, "RWOBJECTTYPE_INSTANCESUBREF" },
    {0x00EB006A, "RWOBJECTTYPE_WAYPOINTSUBREF" },
    {0x00EB006B, "RWOBJECTTYPE_TRIGGERINSTANCESUBREF" },
    {0x00EB006C, "RWOBJECTTYPE_EMITTERWAYPOINTSUBREF" },
    {0x00EB006D, "RWOBJECTTYPE_DMOSUBREF" },
    {0x00EB006E, "RWOBJECTTYPE_HOTPOINTSUBREF" },
    {0x00EB006F, "RWOBJECTTYPE_GRABSUBREF" },
    {0x00EB0070, "RWOBJECTTYPE_VISUALINDICATORSUBREF" },
    {0x00EC0010, "ARENADICTIONARY" },
    {0x7FFFFFFF, "RWCOBJECTTYPE_FORCENUMSIZEINT" },
    {0x00FF0000, "UNKNOWN" },
};

namespace rw {
    namespace core {
        namespace arena {
            struct ArenaFileHeaderMagicNumber //rw::core::arena
            {
                uint32_t prefix;   //‰RW4
                uint32_t body;     //(platform specification: ps3 + 0x00, xb2 + 0x00, rev + 0x00, w32 + 0x00)
                uint32_t suffix;   //0x0D0A1A0A
            };

            struct ArenaFileHeader { //rw::core::arena
	            ArenaFileHeaderMagicNumber magicNumber;
                uint8_t  isBigEndian;
                uint8_t  pointerSizeInBits; //0x20 (32)
                uint8_t  pointerAlignment;  //0x04 (4)
                uint8_t  unused; 			//0x00 
                uint32_t majorVersion; 	    //0x34353400 (454 + 0x00)
                uint32_t minorVersion;	    //0x30303000
                uint32_t buildNo; 		    //0x00000000
            };

            struct Arena { //rw::core::arena
	            ArenaFileHeader fileHeader;
                uint32_t id;
                uint32_t numEntries;
                uint32_t numUsed;
                uint32_t alignment; //(16 or 32)
                uint32_t virt;
	            uint32_t dictStart; //ArenaDictEntry -- pointer to start of dictionary
	            uint32_t sections; //ArenaSectionManifest -- pointer to sections
                uint32_t base; //void*
	            uint32_t m_unfixContext; //SizeAndAlignment -- pointer
	            uint32_t m_fixContext; //ArenaIterator -- pointer
            };

            struct BaseResourceDescriptor { //rw
                uint32_t size;
                uint32_t alignment; // alignment will be 1 if unused
            };

            //resource descriptors (m_resourceDescriptor) (amount ps3 = 6, xbox = 5, wii = 4)

            //resource descriptors (m_resourcesUsed) (amount ps3 = 7, xbox = 6, wii=5)

            struct TargetResource { //rw
                uint32_t m_baseresource; //offset
            };

            //Arena_Sections //Astart right after the above header or where defined by the section manifest offset in the header. Typically occur in the order listed
            struct Manifest
            {
                uint32_t typeID; 		//0x00010004
                uint32_t numEntries;	//usually 4 entries
                uint32_t dictionary; 	//offset from start of this section
            };

            struct Dictionary_Entry
            {
                uint32_t offset; //offset to section from start of sections
            };

            struct Types
            {
                uint32_t typeID; 		//0x00010005
                uint32_t numEntries;	//number of types
                uint32_t dictionary; 	//offset from start of this section
            };

            //entries typically start with a blank zero entry
            struct Type_Entry
            {
                uint32_t RW_Object_ID;
            };

            struct External_Arenas
            {
                uint32_t typeID; 		//0x00010006
                uint32_t numEntries;
                uint32_t dictionary; 	//offset from start of this section
            };

            struct External_Arena
            {
                uint32_t ID;            //this arena's id
                uint32_t unknown;       //0xFFB00000
                uint32_t ID1;           //this arena's id
            };

            struct External_Arena_Entry
            {
                uint32_t unknown; //always zero
            };

            struct Subreferences
            {
                uint32_t typeID; 				//0x00010007
                uint32_t numEntries;            //number of entries
                uint32_t m_dictAfterRefix; 	    //ArenaDictEntry -- offset to dictionary after refix
                uint32_t m_recordsAfterRefix;   //ArenaSectionSubreferencesRecord -- offset to records after refx
                uint32_t dict; 			        //ArenaDictEntry -- offset to dictionary
                uint32_t records; 				//ArenaSectionSubreferencesRecord -- offset to records
                uint32_t numUsed; 				//number used
            };

            struct Atoms
            {
                uint32_t typeID; 		//0x00010008
                uint32_t numEntries;	//never seen this not be zero
                uint32_t Atom_Table; 	//never seen this not be zero
            };

            struct Arena_Subreference_Record
            {
                uint32_t objectID;
                uint32_t offset; //offset within the item
            };

            struct arenaDictEntry //rw::core::arena
            {
                uint32_t ptr;           //absolute offset in the file. if it is a base resource however, it is the offset to the resource from the start of the resources.
                uint32_t reloc;         //ive never seen this not be zero
                uint32_t size;          //data size for that object/size of the entry
                uint32_t alignment;     //alignment of the entry
                uint32_t typeIndex;     //This refers to the index of the type in the types section.
                uint32_t typeId; 	    //RW Object type ID
            };
        }
    }
    namespace collision {
        enum ObjectType : uint32_t {
            RWCOBJECTTYPE_NA = 0x80000,
            RWCOBJECTTYPE_VOLUME = 0x80001,
            RWCOBJECTTYPE_SIMPLEMAPPEDARRAY = 0x80002,
            RWCOBJECTTYPE_TRIANGLEKDTREEPROCEDURAL = 0x80003,
            RWCOBJECTTYPE_KDTREEMAPPEDARRAY = 0x80004,
            RWCOBJECTTYPE_BBOX = 0x80005,
            RWCOBJECTTYPE_CLUSTEREDMESH = 0x80006,
            RWCOBJECTTYPE_FORCEENUMSIZEINT = 0x7FFFFFFF
        };

        enum VolumeType : uint32_t {
            VOLUMETYPENULL = 0,
            VOLUMETYPESPHERE = 1,
            VOLUMETYPECAPSULE = 2,
            VOLUMETYPETRIANGLE = 3,
            VOLUMETYPEBOX = 4,
            VOLUMETYPECYLINDER = 5,
            VOLUMETYPEAGGREGATE = 6,
            VOLUMETYPENUMINTERNALTYPES = 7,
            VOLUMETYPEFORCEENUMSIZEINT = 0x7FFFFFFF
        };
    }
}

namespace renderengine {
    struct DrawIndexedParameters //renderengine
    {
        uint32_t primType; //_D3DPRIMITIVETYPE
        int32_t baseVertexIndex;
        uint32_t startIndex;
        uint32_t indexCount;
    };

    struct DrawParameters //renderengine
    {
        uint32_t primType; //_D3DPRIMITIVETYPE
        uint32_t startVertex;
        uint32_t vertexCount;
    };

    struct vertexBufferps3 { //renderengine PS3
        uint8_t* m_buffer;	    //Buffer data	In secondary resource portion
        uint32_t m_offset;	    //Offset relative to data start
        uint32_t m_bufferSize;	//Buffer size in bytes
        uint8_t	m_location;	    //Memory location	See location
        uint8_t	m_lockFlags;	//Lock flags	See VertexBuffer lock flags     Always 0 ?
        uint16_t m_pad0;	    //Padding
    };

    struct indexBufferps3 { //renderengine PS3
        uint32_t m_buffer;	    //Offset to buffer data	In secondary resource portion (uint8*)
        uint32_t m_offset;	    //Offset relative to data start
        uint32_t m_numIndices;	//Number of indices
        uint32_t m_stride;	    //Size of each index value	2 or 4
        uint8_t	m_indexType;	//Index type	See index type - Always PS3INDEXTYPE_16
        uint8_t	m_location;	    //Memory location	See location
        uint8_t	m_type;	        //Buffer type	See IndexBuffer type - Always TYPE_STATIC?
        uint8_t	m_lockFlags;	//Lock flags
    };

    struct VertexBuffer {
        uint32_t m_bufferSize;
        uint32_t m_type;
    };

    struct vertexBufferxbox { //renderengine XBOX
        uint32_t Common;            //D3DResource
        uint32_t ReferenceCount;    //D3DResource
        uint32_t Fence;             //D3DResource
        uint32_t ReadFence;         //D3DResource
        uint32_t Identifier;        //D3DResource
        uint32_t BaseFlush;         //D3DResource
        uint32_t GPUVERTEX_FETCH_CONSTANT[2]; //GPUVERTEX_FETCH_CONSTANT
        uint32_t m_bufferSize;
        uint32_t m_type;
    };

    struct indexBufferxbox { //renderengine XBOX
        uint32_t Common;            //D3DResource
        uint32_t ReferenceCount;    //D3DResource
        uint32_t Fence;             //D3DResource
        uint32_t ReadFence;         //D3DResource
        uint32_t Identifier;        //D3DResource
        uint32_t BaseFlush;         //D3DResource
        uint32_t Address;           //D3DIndexBuffer
        uint32_t Size;              //D3DIndexBuffer
        uint32_t m_numIndices;
    };

    struct vertexBufferwii {
        uint32_t m_bufferSize;
        uint32_t m_type;
    };

    struct indexBufferwii {
        uint32_t m_type;
        uint32_t m_depth;
        uint32_t m_bufferSize;
        uint32_t m_numIndices;
    };

    struct meshHelper { //renderengine
        uint32_t m_numIndexBuffers; //00 00 00 01 
        uint32_t m_numVertexBuffers; //00 00 00 01 
        //<--m_buffers // Maybe reference to baseresource index? not sure why wii would have 3...
    };

    struct vertexDescriptorps3 { //renderengine PS3
        int32_t m_refCount;     //Always 0?
        uint32_t m_typesFlags;  //Denotes which element types are present, General formula is 1 << elementType for each element, combined with a bitwise OR.
        uint16_t m_numStreams;  //Number of streams, Always 1? 
        uint16_t m_numElements; //Number of elements, Maximum allowed is 16
        uint16_t m_frequencyDividerOperation;   //Frequency divider operation for each element, One bit per element. See operation Always modulo ?
        uint16_t m_pad0;        //Padding
    };

    struct VertexDescriptorElementps3 { //renderengine PS3
        uint8_t	vertexType;     //Datatype of each coordinate
        uint8_t	numComponents;  //Number of components
        uint8_t	stream;         //Stream index
        uint8_t	offset;         //Starting offset of the vertex attribute array
        uint16_t stride;        //Offset bytes between consecutive vertices	
        uint8_t	type;           //Element type
        uint8_t	elementClass;   //Element class
    };

    struct vertexDescriptorxbox { //renderengine XBOX
        int32_t	m_refCount;     //Always 0 ?
        uint32_t m_typesFlags;  //Type flags, Bits are set by indexed usage of each element(1 << usage index)
        uint16_t m_numElements;	//Number of elements
        uint16_t m_numStreams;  //Number of streams, Always 1 ?
        uint32_t pad0;          //Padding
    };

    struct VertexDescriptorElementxbox { //renderengine XBOX
        uint16_t stream;	    //Stream number.
        uint16_t offset;	    //Offset(if any) from the beginning of the stream to the start of the data.
        uint32_t type;	        //Data type.One of several predefined types that define the data size.See D3DDECLTYPE
        uint8_t method;	        //Tessellation method.Always D3DDECLMETHOD_DEFAULT.See D3DDECLMETHOD
        uint8_t usage;	        //Defines the intended use of the data.D3DDECLUSAGE
        uint8_t usageIndex;     //Modifies the usage data to allow the user to specify multiple usage types.
        uint8_t indexedUsage;   //Indexed usage
        uint32_t elementClass;  //Element class
    };

    struct vertexDescriptorwii { //renderengine WII
        uint8_t m_numStreams;
        uint8_t m_numElements;
    };

    struct VertexDescriptorElementwii { //renderengine WII
		uint8_t attr; //attribute
		uint8_t cnt; //compType
        uint8_t type; //compSize
		uint8_t attributeType; //fraction
        uint8_t frac; 
        uint8_t stride; //stride
        uint8_t stream; //vBuffIndex
        uint8_t pad;
        uint16_t offset;
    };
}

namespace pegasus {
    struct tAABB //pegasus // Axis-aligned bounding box
    {
        rw::math::vpu::Vector4 m_Min;
        rw::math::vpu::Vector4 m_Max;
    };

    struct tAttribPair //pegasus
    {
        uint64_t m_ClassKey;
        uint64_t m_InstanceKey;
    };

    namespace v22 // Skate 1
    { /* structs for 22.0 */ 
        struct tRMaterial //pegasus
        {
            struct tChannel {
                uint32_t m_ShaderInput; //tStringPtr = offset to name
                uint32_t m_StreamName; //pegasus::tRMaterial::tRasterNamePtr -- offset to stream name
                uint64_t m_uiGuid;
				uint32_t m_pStream; // offset
                uint16_t m_uiFlags;
                uint16_t m_uiImageChannel;
            };
            uint32_t m_uiNumChannels;
            uint32_t m_uiFlags;
            uint32_t m_pChannels; //tChannel -- offset to array of channels
        };

        struct tInstance {
            rw::math::Matrix44Affine m_TransformMatrix;
            tAABB m_BBox;
            uint64_t m_uiGuid;
            uint64_t m_uiGuidLocal;
            tAttribPair m_AttribKey;
            uint32_t m_pRModel; // tRModelData -- index to render model data in dictionary
            uint32_t m_pCModel; // tCModelData -- index to collision model data in dictionary
            uint32_t m_pEmbedded; // tEmbeddedData -- index to embedded data in dictionary
            uint32_t m_Name; // tStringPtr = offset to name
            uint32_t m_Description; // tStringPtr = offset to description
            uint32_t m_Component; // tStringPtr = offset to component
            uint32_t m_Category; // tStringPtr = offset to category
        };
    } 
    namespace v24 // Skate It
    { /* structs for 24.0 */ 
        struct tRMaterial //pegasus
        {
            struct tChannel {
                uint32_t m_ShaderInput; //tStringPtr = offset to name
                uint16_t m_uiFlags;
                uint16_t m_uiImageChannel;
                uint64_t m_uiGuid;
                uint32_t m_StreamName; //pegasus::tRMaterial::tRasterNamePtr -- offset to stream name
                uint32_t m_pStream; // offset
            };
            uint32_t m_uiNumChannels;
            uint32_t m_uiFlags;
            uint32_t m_pChannels; //tChannel -- offset to array of channels
        };

        struct Vector3 { float x, y, z; };
        struct Matrix44Affine { Vector3 xAxis, yAxis, zAxis, wAxis; }; //Wii specific version

        struct tInstance {
            Matrix44Affine m_TransformMatrix;
            tAABB m_BBox;
            uint64_t m_uiGuid;
            uint64_t m_uiGuidLocal;
            tAttribPair m_AttribKey;
            uint32_t m_pRModel; // tRModelData -- index to render model data in dictionary
            uint32_t m_pCModel; // tCModelData -- index to collision model data in dictionary
            uint32_t m_pEmbedded; // tEmbeddedData -- index to embedded data in dictionary
            uint32_t m_Name; // tStringPtr = offset to name
            uint32_t m_Description; // tStringPtr = offset to description
            uint32_t m_Component; // tStringPtr = offset to component
            uint32_t m_Category; // tStringPtr = offset to category
        };
    }
	namespace v25 // Skate 2 and Skate 3
    { /* structs for 25.0 */ 
        struct tRMaterial //pegasus
        {
            struct tChannel {
                uint32_t m_ShaderInput; //tStringPtr = offset to name
                uint16_t m_uiFlags;
                uint16_t m_uiImageChannel;
                uint32_t m_uiPad[2];
                uint64_t m_uiGuid;
                uint32_t m_StreamName; //pegasus::tRMaterial::tRasterNamePtr -- offset to stream name
                uint32_t m_pStream; // offset
            };
            uint32_t m_uiNumChannels;
            uint32_t m_uiFlags;
            uint32_t m_pChannels; //tChannel -- offset to array of channels
        };

        struct tInstance {
            rw::math::Matrix44Affine m_TransformMatrix;
            tAABB m_BBox;
            uint64_t m_uiGuid;
            uint64_t m_uiGuidLocal;
            tAttribPair m_AttribKey;
            uint32_t m_pRModel; // tRModelData -- index to render model data in dictionary
            uint32_t m_pCModel; // tCModelData -- index to collision model data in dictionary
            uint32_t m_pEmbedded; // tEmbeddedData -- index to embedded data in dictionary
            uint32_t m_Name; // tStringPtr = offset to name
            uint32_t m_Description; // tStringPtr = offset to description
            uint32_t m_Component; // tStringPtr = offset to component
            uint32_t m_Category; // tStringPtr = offset to category
        };
    }

    struct tVersionData
    {
        uint32_t m_uiVersion;
        uint32_t m_uiRevision;
    };

    struct tInstanceData {
        uint32_t m_TypeID; // offset 0x0, size 0x4
        uint32_t m_uiNumInstances; // offset 0x4, size 0x4
        uint32_t m_uiNumStrings; // offset 0x8, size 0x4
        uint32_t m_Instances; // tInstancePtr = offset to instances
		uint32_t m_StringList; // tStringPtr = offset to string list
    };

    struct tRMeshData {
        union DrawParams //pegasus::tRMeshData
        {
            uint32_t m_pDrawParameters; //DrawParameters -- offset to draw parameters
            uint32_t m_pDrawIndexedParameters; //DrawIndexedParameters -- offset to draw indexed parameters
        };
        struct Parameters //pegasus::tRMeshData
        {
            uint32_t m_iNumParams;
            uint32_t m_bIndexed;
            uint32_t m_iNumOfBlendShapes;
            uint32_t m_iBlendNamesLength;
        };
        struct tBlendShapeInfo //pegasus::tRMeshData
        {
            uint32_t m_pBlendShapeItem; //tRBlendShapeData -- offset to blend shape data
            uint32_t m_szBlendShapeName; //tStringPtr = offset to namem_ShaderConstants
            uint64_t m_ui64HashName;
        };

        tAABB m_BBox;
        uint32_t m_pVertexDescriptor; //offset to vertex descriptor
        uint32_t m_pMeshHelper; //offset to mesh helper
        uint32_t m_pIndexBuffer; //offset to index buffer
        uint32_t m_pVertexBuffer; //offset to vertex buffer
        uint32_t m_uiNumVerts;
        DrawParams m_DrawParams;
        uint32_t m_pRemapTable; //offset to index buffer
        uint32_t m_bIsIndexed;
        uint32_t m_uiNumBoneMats;
        uint32_t m_uiNumBlendShapes;
        uint32_t m_pBlendShapeTable; //tBlendShapeInfo -- offset to blend shape table
        uint32_t m_szBlendShapeNames; //tStringPtr = offset to names
    };

    struct tROptiMeshData {
        struct Parameters //pegasus::tROptiMeshData
        {
            uint32_t m_iNumIslands;
            uint32_t m_iNumRemapIndices;
            uint32_t m_iNumBlendShapes;
            uint32_t m_iBlendShapesNameLength;
        };
        struct tBlendShapeInfo //pegasus::tROptiMeshData
        {
            uint64_t m_ui64HashName;
            uint32_t m_pBlendShapeItem; //tRBlendShapeData -- offset to blend shape data
            uint32_t m_szBlendShapeName; //tStringPtr = offset to name
        };
        tAABB m_BBox;
        uint32_t m_uiNumVerts;
        uint32_t m_pMaterial; //tRMaterial -- offset to material
        uint32_t m_pVertexDescriptor; //offset to vertex descriptor
        uint32_t m_pMeshHelper; //offset to mesh helper
        uint32_t m_pIndexBuffer; //offset to index buffer
        uint32_t m_pVertexBuffer; //offset to vertex buffer
        uint32_t m_uiNumIslands;
        uint32_t m_pIslandAreas; //rw::math::vpu::Vector4 -- offset to island areas
        uint32_t m_pIslandAABBs; //tAABB -- offset to island AABBs
        uint32_t m_pIslandDrawParams; //DrawIndexedParameters -- offset to island draw parameters
        uint32_t m_uiNumRemapIndices;
        uint32_t m_pRemapTable; //uint16_t -- offset to remap table 
        uint32_t m_uiNumBlendShapes;
        uint32_t m_pBlendShapeTable; //tBlendShapeInfo -- offset to blend shape table
        uint32_t m_szBlendShapeNames; //tStringPtr = offset to names
    };

    struct tRModelData {
        struct tMeshData { //pegasus::tRModelData
			uint32_t m_pMaterial; //tRMaterial -- offset to material
        };
        tAABB m_BBox;
        uint32_t m_pIBPMatrices; //00 00 00 40 -- rw::math::vpu::Matrix44 offset
        uint32_t m_pMeshTable; //00 00 09 40 -- tRModelData::tMeshData offset
        uint32_t m_pBoneNameTable; //00 00 09 48  -- offset to offset
        uint32_t m_pBoneNameList; //00 00 09 D8 -- offset
        uint16_t m_iNumTotalBones; //00 18 
        uint16_t m_iNumMeshes; //00 01 
        uint16_t m_iNumBones; //00 24 
        uint16_t m_iNumIslands; //00 01 
        uint32_t m_pIslandAABBs; //00 00 0B 70 -- tAABB offset
        uint32_t m_pIslandAreas; //00 00 0B F0 -- rw::math::vpu::Vector4 offset
    };

    struct tRBlendShapeData //pegasus
    {
        struct Parameters {
            uint32_t m_iNumOfMeshDatas;
            uint32_t m_iNumOfBlendVertexWeights;
        };
        uint32_t m_pVertexDescriptor; //offset to vertex descriptor
        uint32_t m_pMeshHelper; //offset to mesh helper
        uint32_t m_uiNumMeshes;
        uint32_t m_ppMeshTable; //tRMeshData -- offset to array of mesh data offsets
        uint32_t m_uiNumBlendVertexWeights;
        uint32_t m_pBlendVertexWeights; //float - offset to array of blend vertex weights
    };

    union tStringPtr //pegasus
    {
        uint32_t Offset;
        uint32_t Pointer; //uint8_t -- offset to string data
    };

    struct tChannel	//pegasus::tRMaterial
    {
        uint32_t m_ShaderInput; //tStringPtr = offset to name
        uint16_t m_uiFlags;
        uint16_t m_uiImageChannel;
        uint32_t m_uiPad[2];
    };

    struct tRMaterialData //pegasus
    {
        struct Parameters {
            uint32_t m_uiNumChannels;
            uint32_t m_uiNumMaterials;
            uint32_t m_uiStringSize;
        };
        uint32_t m_uiNumMaterials;
        uint32_t m_uiNumChannels;
        uint32_t m_pMaterials; // offset
        uint32_t m_pChannels; // offset
        uint32_t m_StringList; //pegasus::tStringPtr -- offset to string list
    };

    struct tTableOfContents {
        struct tTypeMap { //pegasus::tTableOfContents
            uint32_t m_Type; //RwObjectType
            uint32_t m_uiIndex;
        };
        uint32_t m_uiItemsCount;
        uint32_t m_pArray; //tTOCEntry -- offset to TOCEntries
        uint32_t m_pNames;  //tStringPtr = offset to names
        uint32_t m_uiTypeCount;
        uint32_t m_pTypeMap; //tTypeMap -- offset to type map
    };

    struct tTOCEntry //pegasus
    {
        uint32_t m_Name;  //tStringPtr = offset to name
        uint32_t unknown; //can be 7C 0F 16 78, 9B 0F 16 78, FE FF FF FF
        uint64_t m_uiGuid;
        uint32_t m_Type; //RwObjectType or RwHash32String (Texture, MiniMapTexture or CubeMapTexture)
        uint32_t m_pObject; //index void*
    };

    struct tIrradianceProbeData //pegasus
    {
        rw::math::vpu::Vector4 m_fSHRGB[9];
        rw::math::vpu::Vector3 m_fPos;
    };

    struct tIrradianceData //pegasus
    {
        uint32_t m_uiItems;     //number of probes
        uint32_t m_aProbes;     //tIrradianceProbeData -- offset to array of probes
    };

    struct tMassiveData //pegasus
    {
        struct Parameters //pegasus::tMassiveData
        {
            uint32_t m_iNumEntities;
        };
        struct tEntity //pegasus::tMassiveData
        {
            rw::math::vpu::Vector4 m_Coords[4];
            rw::math::vpu::Vector4 m_Centroid;
            rw::math::vpu::Vector4 m_Normal;
            uint64_t m_uiGuid;
            uint32_t m_uiRotationType;
            uint32_t m_Pad[1];
        };
        int32_t m_iNumEntities;
        uint32_t m_pEntities; //pegasus::tMassiveData::tEntity -- offset to array of entities
    };

    struct tSplineHeader { //pegasus
        uint64_t m_uiGuid;
        uint64_t m_uiGuidLocal;
        uint32_t m_Instance; //pegasus::tInstancePtr -- offset to instance
        uint32_t m_Head; //pegasus::tSplineSegmentPtr -- offset to head segment
        uint32_t m_Tail; //pegasus::tSplineSegmentPtr -- offset to tail segment
        uint32_t m_Pad;
    };

    struct tSplineSegment { //pegasus
        rw::math::vpu::Matrix44 m_BasisMatrix;
        rw::math::vpu::Vector4 m_Inverse;
        tAABB m_BBox;
        float m_fLength;
        float m_fDistance;
		uint32_t m_Spline; //pegasus::tSplineHeaderPtr -- offset to parent spline
        uint32_t m_Prev; //pegasus::tSplineSegmentPtr -- offset to previous segment
        uint32_t m_Next; //pegasus::tSplineSegmentPtr -- offset to next segment
    };

    struct tSplineData { //pegasus
        unsigned int m_uiNumSplines;
        unsigned int m_uiNumSegments;
		uint32_t m_Splines; //pegasus::tSplineHeaderPtr -- offset to array of splines
        uint32_t m_Segments; //pegasus::tSplineSegmentPtr -- offset to array of segments
    };

    struct tEmbeddedData //pegasus
    {
        struct Parameters	//pegasus::tEmbeddedData
        {
            int32_t m_iNumHotPoints;
            int32_t m_iNumGrindSplines;
            int32_t m_iNumGrabSplines;
            int32_t m_iNumVisualIndicators;
        };
        uint32_t m_TypeID;
        int32_t m_iNumHotPoints;
        int32_t m_iNumGrindSplines;
        int32_t m_iNumGrabSplines;
        int32_t m_iNumVisualIndicators;
        uint32_t m_pHotPoints; //pegasus::tEmbeddedDataPtr -- offset to hot points
        uint32_t m_pGrindSplines; //pegasus::tEmbeddedDataPtr -- offset to grind splines
        uint32_t m_pGrabSplines; //pegasus::tEmbeddedDataPtr -- offset to grab splines
        uint32_t m_pVisualIndicators; //pegasus::tEmbeddedDataPtr -- offset to visual indicators
    };

    struct tGrab //pegasus
    {
        tAABB m_BBox;
        tAttribPair m_AttribulatorKey;
        rw::math::vpu::Vector4* m_pSplinePoints;
        rw::math::vpu::Vector4* m_pApproachPoints;
        rw::math::vpu::Vector4* m_pApproachVectors;
        uint32_t m_Flags;
        uint32_t m_Type;
        uint8_t m_iNumSplinePoints;
        uint8_t m_iNumApproachPoints;
        uint8_t m_iNumApproachVectors;
        uint8_t m_iPad;
    };

	struct tHotPoint { //pegasus
        rw::math::vpu::Matrix44Affine m_Transform;
        tAABB m_BBox;
        unsigned int m_eType;
    };

    struct tCollisionBezier {
        rw::math::fpu::Vector4Template<float> m_Points[16];
        rw::math::vpu::Matrix44 m_Ax;
        rw::math::vpu::Matrix44 m_Ay;
        rw::math::vpu::Matrix44 m_Az;
        rw::math::vpu::Vector3 m_min; //AABBoxTemplate<rw::math::vpu::Matrix44Affine, rw::math::vpu::Vector3, rw::collision::AABBoxMemoryDumpPolicy> m_AABB;
        rw::math::vpu::Vector3 m_max;
        uint16_t m_SurfaceID;
    };

    struct tCModelData{ //pegasus
        union tCollisionMesh
        {
            uint32_t m_pVolume; //rw::collision::Volume -- offset to collision volume
            uint32_t m_pSimpleTriMesh; //pegasus::tSimpleTriMeshData -- offset to simple tri mesh data
        };
        struct tMeshData
        {
            tCollisionMesh m_Mesh;
			uint32_t m_BoneName; //pegasus::tStringPtr = offset to bone name
        };
		tCollisionMesh m_BoundingVolume;
        uint32_t m_iNumMeshes;
		uint32_t m_pMeshTable; //or m_pAssembly
    };

    struct tCMaterial {
        uint32_t m_pAudioMaterial; //pegasus::tAudioMaterial -- offset to audio material
        uint32_t m_pPhysicsMaterial; //pegasus::tPhysicsMaterial -- offset to physics material
        uint32_t m_Name; //pegasus::tStringPtr -- offset to name
    };

    struct tCMaterialData {
        struct Parameters {
            uint32_t m_uiNumAudioMaterials;
            uint32_t m_uiNumPhysicsMaterials;
            uint32_t m_uiNumCMaterials;
            uint32_t m_uiStringSize;
        };
        uint32_t m_uiNumAudioMaterials;
        uint32_t m_uiNumPhysicsMaterials;
        uint32_t m_uiNumCMaterials;
        uint32_t m_pAudioMaterials; //pegasus::tAudioMaterial -- offset to array of audio materials
        uint32_t m_pPhysicsMaterials; //pegasus::tPhysicsMaterial -- offset to array of physics materials
        uint32_t m_pCMaterials; //pegasus::tCMaterial -- offset to array of cmaterials
        uint32_t m_pStringList; //char -- offset to string list
    };

    struct tTriggerInstance //pegasus
    {
        rw::math::vpu::Matrix44Affine m_TransformMatrix;
        tAABB m_BBox;
        rw::math::vpu::Vector3 m_BasePlaneNormal;
        rw::math::vpu::Vector3 m_BasePlaneLeadingEdge[2];
        rw::math::vpu::Vector3 m_BasePlaneTrailingEdge[2];
        uint64_t m_uiGuid;
        uint64_t m_uiGuidLocal;
        tAttribPair m_AttribKey;
        uint64_t m_TriggerData;
        uint32_t m_TriggerType;
        tCModelData* m_pCModel;
        tStringPtr m_Name;
        uint8_t m_PadBuffer[12];
    };

    struct tAIPathNodeExtData {
        float m_TrajectoryStartPos[3];
        float m_TrajectoryStartVel[3];
        float m_TrajectoryOffset[3];
        int16_t m_iTrickIndex;
        char m_AirSpin180Count;
        uint8_t m_i8Flags;
    };

    struct tAIPathNode {
        float m_Position[3];
        float m_Direction[3];
        uint8_t m_BoardOrientation[4];
        uint8_t m_SkaterOrientation[4];
		uint32_t m_pExtData; //pegasus::tAIPathNodeExtData -- offset to extra data
        uint8_t m_uiFramesSinceLastNode;
        uint8_t m_uiNodeWidthLeft;
        uint8_t m_uiNodeWidthRight;
        uint8_t m_uiEventType;
        uint8_t m_i8Flags;
        uint8_t m_uiExtraData1;
        uint8_t m_uiExtraData2;
        uint8_t m_uiExtraData3;
    };

    struct tAIPath {
        enum ESkillLevel {
            eSkillLevel_Easy = 0,
            eSkillLevel_Medium = 1,
            eSkillLevel_Hard = 2,
            eSkillLevel_VeryHard = 3,
            eSkillLevel_Extreme = 4,
            eSkillLevel_Max = 5,
            eSkillLevel_Invalid = -1
        };
        tAABB m_BBox;
        uint8_t m_ID[16];
		uint32_t m_pNodes; //pegasus::tAIPathNode -- offset to array of nodes
        int32_t m_uiNumNodes;
        uint32_t m_pExtra; //pegasus::tAIPathNodeExtData -- offset to array of extra node data
		uint32_t m_pBranchGroup; //pegasus::tAIPathBranchGroup -- offset to array of branch groups
        int32_t m_uiNumGroups;
        uint32_t m_BitFlags;
        uint64_t m_AllowedSkaters;
		uint32_t m_SkillLevel; //pegasus::tAIPath::ESkillLevel -- skill level enum
        int32_t m_ExtraData1;
        int32_t m_ExtraData2;
        int32_t m_ExtraData3;
    };

    struct tAIPathBranchGroup {
        uint32_t m_Branch; //pegasus::tAIPathBranch -- offset to array of branches
        uint32_t m_iCount;
        int32_t m_iSourceNode;
    };

    struct tAIPathBranch {
        uint8_t m_PathID[16];
        int32_t m_NodeIx;
        float m_fRatio;
    };

    struct tAIPathData {
        struct Parameters {
            uint32_t m_uiSize;
            uint32_t m_uiAlignment;
        };
        uint32_t m_uiNumOfPaths;
        tAIPath* m_pPaths;
    };

    struct tRollerDesc {
        rw::math::vpu::Matrix44 m_TransformMatrix;
        tAABB m_BBox;
        uint32_t m_pRModel; //pegasus::tRModelData -- offset to rmodel data
        uint32_t m_pCModel; //pegasus::tCModelData -- offset to cmodel data
        uint32_t m_Name; //pegasus::tStringPtr = offset to name
        uint32_t m_Description; //pegasus::tStringPtr = offset to description
    };

    struct tRollerDescData {
        struct Parameters {
            uint32_t m_uiNumRollerDescs;
            uint32_t m_uiNumStrings;
            uint32_t m_uiStringSize;
        };
        uint32_t m_uiNumRollerDescs;
        uint32_t m_uiNumStrings;
		uint32_t m_RollerDescs; //pegasus::tRollerDescPtr -- offset to array of roller descriptions
        uint32_t m_StringList; //pegasus::tStringPtr -- offset to string list
    };

    struct tStatsData {
		uint32_t m_iNumStats; //number of stats
        uint32_t m_pStats; //offset to array of stats
    };

    union tLocationDescPtr {
        uint32_t Pointer;
        uint32_t Offset;
    };

    union tSubLocationDescPtr {
        uint32_t Pointer;
        uint32_t Offset;
    };

    struct tSubLocationDesc { // A single sub-location
        rw::math::vpu::Matrix44Affine m_TransformMatrix; // 0x00
        tAABB m_BBox;                                    // 0x40
        uint32_t m_Name;                                 // 0x60 offset
        uint32_t m_Description;                          // 0x64 offset
        uint8_t m_PadBuffer[8];                          // 0x68 - 0x70
    };

    struct tLocationDesc { // A single location
        rw::math::vpu::Matrix44Affine m_TransformMatrix; // 0x00
        tAABB m_BBox;                                    // 0x40
        uint32_t m_iNumSubLocations;                     // 0x60
        tSubLocationDescPtr m_SubLocations;              // 0x64
        uint32_t m_Name;                                 // 0x68 offset
        uint32_t m_Description;                          // 0x6C offset
        uint64_t m_uiGuid;                               // 0x70
        uint8_t m_PadBuffer[8];                          // 0x78 - 0x80
    };

    struct tLocationDescData { // Top-level location descriptor structure
        uint32_t m_uiNumLocationDescs;          // 0x00
        uint32_t m_uiNumSubLocationDescs;       // 0x04
        uint32_t m_uiNumStrings;                // 0x08
        tLocationDescPtr m_LocationDescs;       // 0x0C
        tSubLocationDescPtr m_SubLocationDescs; // 0x10
        uint32_t m_StringList;                  // 0x14  offset
    };
}

// Texture Management
struct textureInformationPs3
{
    uint8_t format; //0x81, 0x82, 0x83, 0x84, 0x85, 0x86(DXT1), 0x87(DXT3), 0x88(DXT5), 0x8B, 0x8D, 0x8E, 0x8F, 0x91(bandw), 0x92, 0x93(gray), 0x94, 0x95, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E(A5 but swizzled?), 0x9F, 0xA1(gray), 0xA2, 0xA3, 0xA4, 0xA5(argb8888), A6, A7, A8
    uint8_t mipmap; //Number of mipmap levels (1-13)
    uint8_t dimension; //0x02 Texture dimension
    uint8_t cubemap; //Enable or disable cube mapping
    uint32_t remap; //Method of selecting each element of the color value
    uint16_t width; //Width of texture (1-4096)
    uint16_t height; //Height of texture (1-4096)
    uint16_t depth; //0x0001 Depth of texture (1-512)
    uint8_t location; //Location of texture data (main memory or local memory)
    uint8_t padding;
    uint32_t pitch; //pitch = floor((width+3)/4)*block_size     Where block_size is 8 for DXT1 and 16 for DXT2 through DXT5.
    uint32_t offset; //	Offset value from the base address of the texture data location
    uint32_t buffer;
    uint32_t storeType; //0x00000002, TYPE_NA = -1, TYPE_1D = 1, TYPE_2D = 2, TYPE_3D = 3, TYPE_CUBE = 0x10002, TYPE_FORCEENUMSIZEINT = 0x7FFFFFFF
    uint32_t storeFlags; //0x00000000 Seemingly unused
    uint16_t unknown; //seems to always be 0x5572
    uint8_t padding2; //00000000
    uint8_t format2; //format written another time.
};

struct textureInformationXbox
{
    uint32_t Common; //00 00 00 03
    uint32_t ReferenceCount; //00 00 00 01
    uint32_t Fence; //00 00 00 00
    uint32_t ReadFence; //00 00 00 00
    uint32_t Identifier; //00 00 00 00
    uint32_t BaseFlush; //FF FF 00 00
    uint32_t MipFlush; //FF FF 00 00
    unsigned char GPUTEXTURE_FETCH_CONSTANT[0x18];
};

struct textureInformationWii
{
    uint32_t m_type; //Type
	uint32_t m_format; //PixelFormat
    uint16_t m_width;
	uint16_t m_height;
    uint8_t m_numMipLevels;
    uint8_t m_flags;
};

struct Palette {
	uint32_t m_format; //PixelFormat
	uint16_t m_length;
};

static std::string getParentName(const std::string& filePath) {;
    // Extracting the parent directory name
    std::string parentDirName = fs::path(filePath).parent_path().filename().string();
    return parentDirName;
}

static std::string getFileName(const std::string& filePath) {
    // Extracting just the filename without the path
    std::string fileName = fs::path(filePath).filename().string();
    return fileName;
}

static std::string getFileNameWithoutExtension(const std::string& filePath) {
    // Extracting just the filename without the path
    std::string fileName = fs::path(filePath).filename().string();

    // Removing the extension
    size_t lastDotPos = fileName.find_last_of('.');
    if (lastDotPos != std::string::npos) {
        fileName = fileName.substr(0, lastDotPos);
    }

    return fileName;
}

template <typename T>
T swapEndianness(T value, bool bigEndian) {
    if (bigEndian) {
        constexpr size_t size = sizeof(T);
        T result = 0;

        for (size_t i = 0; i < size; ++i) {
            result |= (static_cast<T>((value >> (8 * i)) & 0xFF) << (8 * (size - 1 - i)));
        }

        return result;
    }
    else {
        return value;
    }
}

enum class Platform { XBOX, PS3, WII, PC, UNKNOWN };

static Platform detectPlatform(const rw::core::arena::ArenaFileHeaderMagicNumber& magicnumber) {
    const uint32_t RW4 = 0x34575289;
    const uint32_t xbox = 0x00326278;
    const uint32_t ps3 = 0x00337370;
    const uint32_t wii = 0x00766572;
    const uint32_t pc = 0x00323377;

    if (magicnumber.prefix != RW4) return Platform::UNKNOWN;

    switch (magicnumber.body) {
    case xbox: return Platform::XBOX;
    case ps3: return Platform::PS3;
    case wii: return Platform::WII;
    case pc: return Platform::PC;
    default: return Platform::UNKNOWN;
    }
}

struct PlatformInfo {
    int numDescriptors;
    int numResources;
};

const std::unordered_map<Platform, PlatformInfo> platformMapping = {
    { Platform::XBOX, {5, 6} },  //0, 2
    { Platform::PS3,  {6, 7} },  //0, 5
    { Platform::WII,  {4, 5} },  //0
    { Platform::PC,   {4, 5} }   //0, 1
};

static std::pair<std::vector<rw::core::arena::BaseResourceDescriptor>,size_t> loadResourceDescriptors(
    const std::vector<uint8_t>& fileData,
    size_t offset,
    int count,
    bool bigEndian
) {
    std::vector<rw::core::arena::BaseResourceDescriptor> resources;
    resources.reserve(count);

    for (int i = 0; i < count; ++i) {
        // Safety check
        if (offset + sizeof(rw::core::arena::BaseResourceDescriptor) > fileData.size()) {
            std::cerr << "Error: File truncated while reading resource descriptors.\n";
            break;
        }

        rw::core::arena::BaseResourceDescriptor res{};
        std::memcpy(&res, fileData.data() + offset, sizeof(res));

        resources.push_back(res);
        offset += sizeof(res);
    }

    return { resources, offset };
}

/*
static void load_RW4_Model(std::ifstream& inputFile, uint32_t tocstartoffset, pegasus::tTableOfContents header, uint32_t rendermatoffset, bool e) { //modelfound
	std::cout << "Loading models...\n";
	//tableofcontents
    inputFile.seekg(tocstartoffset + swapEndianness(header.m_pNames, e));
    std::vector<rw::core::arena::Arena_Subreference_Record> arenasrrecords;
    for (int i = 0; i < swapEndianness(header.m_uiTypeCount, e); ++i) {
        rw::core::arena::Arena_Subreference_Record arenasrrecord{};
        inputFile.read(reinterpret_cast<char*>(&arenasrrecord), sizeof(arenasrrecord));
        arenasrrecords.push_back(arenasrrecord);
    }
    //rendermaterialdata
    inputFile.seekg(rendermatoffset);
    pegasus::tRMaterialData materialdata{};
    std::vector<pegasus::tRMaterial> materials;
    inputFile.read(reinterpret_cast<char*>(&materialdata), sizeof(materialdata));
    for (int i = 0; i < swapEndianness(materialdata.m_uiNumMaterials, e); ++i) {
        pegasus::tRMaterial material{};
        inputFile.read(reinterpret_cast<char*>(&material), sizeof(material));
        materials.push_back(material);
    }
    inputFile.seekg(rendermatoffset + swapEndianness(materialdata.m_pChannels, e));
    std::vector<pegasus::tRMaterial::tChannel> channels;

    for (int i = 0; i < swapEndianness(materialdata.m_uiNumChannels, e); ++i) {
        pegasus::tRMaterial::tChannel identry{};
        inputFile.read(reinterpret_cast<char*>(&identry), sizeof(identry));
        channels.push_back(identry);
    }
    std::vector<std::string> names;
    for (int i = 0; i < swapEndianness(materialdata.m_uiNumChannels, e); ++i) {
        inputFile.seekg(swapEndianness(channels[i].m_ShaderInput, e) + rendermatoffset);
        int nameEnd = swapEndianness(channels[i].m_StreamName, e);
        if (nameEnd == 0) {
            nameEnd = swapEndianness(channels[i + 1].m_ShaderInput, e);
        }
        //std::cout << swapEndianness(channels[i].m_ShaderInput) + startoffset2 << "\n";
        int nameLength = nameEnd - swapEndianness(channels[i].m_ShaderInput, e);
        //std::cout << nameEnd << " \n";
        //std::cout << swapEndianness(channels[i].m_ShaderInput) << " \n";
        char* buffer = new char[nameLength];
        inputFile.read(buffer, nameLength);
        std::cout << buffer << " \n";
        delete buffer;
    }
    //std::cout << inputFile.tellg();
}
*/

static void load_RW4_Texture(std::ifstream& inputFile, Platform platform, std::vector<rw::core::arena::arenaDictEntry> dictionaries, std::vector<rw::core::arena::BaseResourceDescriptor> resourceDescriptors, pegasus::tTableOfContents header, uint32_t startoffset, std::vector<uint32_t> textureoffsets, bool e) { //texturefound
	std::cout << "Loading textures...\n";
	//tableofcontents
    std::vector<pegasus::tTOCEntry> tocentries;
    for (int i = 0; i < swapEndianness(header.m_uiItemsCount, e); ++i) {
        pegasus::tTOCEntry tocentry{};
        inputFile.read(reinterpret_cast<char*>(&tocentry), sizeof(tocentry));
        tocentries.push_back(tocentry);
    }
    inputFile.seekg(startoffset + swapEndianness(header.m_pNames, e));
    std::vector< std::pair<std::string, int>> names;

    for (const auto& tocentry : tocentries) {
        bool haveName = true;
        uint32_t currentName = swapEndianness(tocentry.m_Name, e);
        if (currentName == 0) {
            haveName = false;
        }
        auto nextEntry = std::next(std::begin(tocentries), &tocentry - &tocentries[0] + 1);

        uint32_t nameDifference = 0;
        if (nextEntry != std::end(tocentries)) {
            uint32_t nextName = swapEndianness(nextEntry->m_Name, e);
            nameDifference = nextName - currentName;
        }
        else {
            nameDifference = swapEndianness(header.m_pTypeMap, e) - currentName;
        }
        std::vector<char> buffer(nameDifference);
        inputFile.read(buffer.data(), nameDifference);
        std::string name(buffer.begin(), buffer.end());
        if (!haveName) {
            name = "";
        }
        names.push_back(std::make_pair(name, swapEndianness(tocentry.m_pObject, e)));
    }

	//texture, We are basically reading using every TOC entry that we got from the table of contents earlier.
    if (platform == Platform::XBOX) {
        std::vector<textureInformationXbox> texinfosxbox;

        for (int i = 0; i < swapEndianness(header.m_uiItemsCount, e); ++i) {
            inputFile.seekg(textureoffsets[i]);
            textureInformationXbox texinfoxbox{};
            inputFile.read(reinterpret_cast<char*>(&texinfoxbox), sizeof(texinfoxbox));
            texinfosxbox.push_back(texinfoxbox);
            int index = names[i].second - 1;

            int offset1 = swapEndianness(dictionaries[index].ptr, e) + swapEndianness(resourceDescriptors[0].size, e);
            int offset2 = swapEndianness(dictionaries[index].size, e);
            std::vector<uint8_t> buffer(offset2);
            inputFile.seekg(offset1);
            inputFile.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(offset2));
            std::string name = names[i].first;

            getTextureInformationAndUntile(name, buffer, texinfoxbox.GPUTEXTURE_FETCH_CONSTANT, sizeof(texinfoxbox.GPUTEXTURE_FETCH_CONSTANT));
        }
    }
    else if (platform == Platform::PS3) {
        std::vector<textureInformationPs3> texinfosps3;
        for (int i = 0; i < swapEndianness(header.m_uiItemsCount, e); ++i) {
            inputFile.seekg(textureoffsets[i]);
            textureInformationPs3 texinfops3{};
            inputFile.read(reinterpret_cast<char*>(&texinfops3), sizeof(texinfops3));
            texinfosps3.push_back(texinfops3);
            int index = names[i].second - 1;

            uint16_t curheight = swapEndianness(texinfops3.height, e);
            uint16_t curwidth = swapEndianness(texinfops3.width, e);
            uint16_t mips = texinfops3.mipmap;

            int offset1 = swapEndianness(dictionaries[index].ptr, e) + swapEndianness(resourceDescriptors[0].size, e);
            int offset2 = swapEndianness(dictionaries[index].size, e);
            std::vector<uint8_t> buffer(offset2);
            inputFile.seekg(offset1);
            inputFile.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(offset2));
            std::string name = names[i].first;

            write_ps3_textures_to_DDS(name, buffer, curwidth, curheight, mips, texinfops3.format, swapEndianness(texinfops3.storeType, e));
        }
    }
    else if (platform == Platform::WII) {
        std::vector<textureInformationWii> texinfoswii;
        for (int i = 0; i < textureoffsets.size(); ++i) {
            inputFile.seekg(textureoffsets[i]);
            textureInformationWii texinfowii{};
            inputFile.read(reinterpret_cast<char*>(&texinfowii), sizeof(texinfowii));
            texinfoswii.push_back(texinfowii);

            int index = names[i].second;

            uint32_t type = swapEndianness(texinfowii.m_type, e);
            uint32_t format = swapEndianness(texinfowii.m_format, e);
            uint16_t curheight = swapEndianness(texinfowii.m_height, e);
            uint16_t curwidth = swapEndianness(texinfowii.m_width, e);
            uint8_t mips = texinfowii.m_numMipLevels;
            uint8_t flags = texinfowii.m_flags;

            int offset1 = swapEndianness(dictionaries[index].ptr, e) + swapEndianness(dictionaries[index].alignment, e);
            int offset2 = swapEndianness(dictionaries[index].size, e);
            std::vector<uint8_t> buffer(offset2);
            inputFile.seekg(offset1);
            inputFile.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(offset2));
            std::string name = names[i].first;

            unswizzle_wii_textures_and_write_to_DDS(name, buffer, curwidth, curheight, mips, format, type, flags);
        }
    }
}

struct ObjectTypeMap { //add all supported object types here
    rw::core::arena::arenaDictEntry tableOfContents;
    rw::core::arena::arenaDictEntry versionData;
    rw::core::arena::arenaDictEntry renderMaterialData;
    rw::core::arena::arenaDictEntry instanceData;
    rw::core::arena::arenaDictEntry statsData;
    std::vector<rw::core::arena::arenaDictEntry> baseResources;
    std::vector<rw::core::arena::arenaDictEntry> textures;
    std::vector<rw::core::arena::arenaDictEntry> palettes;
    std::vector<rw::core::arena::arenaDictEntry> vertexBuffers;
    std::vector<rw::core::arena::arenaDictEntry> indexBuffers;
    std::vector<rw::core::arena::arenaDictEntry> vertexDescriptors;
    std::vector<rw::core::arena::arenaDictEntry> meshHelpers;
    std::vector<rw::core::arena::arenaDictEntry> renderMeshDatas;
    std::vector<rw::core::arena::arenaDictEntry> renderModelDatas;
};

void AddToObjectTypeMap(
    const rw::core::arena::arenaDictEntry& entry,
    ObjectTypeMap& map,
    bool e)
{
    uint32_t typeId = swapEndianness(entry.typeId, e);

    if (typeId >= (uint32_t)RwObjectType::RWOBJECTTYPE_BASERESOURCE_START &&
        typeId <= (uint32_t)RwObjectType::RWOBJECTTYPE_BASERESOURCE_RESERVEDTO)
    {
        map.baseResources.push_back(entry);
        return;
    }

    switch (typeId) {
    case (uint32_t)RwObjectType::RWOBJECTTYPE_TABLEOFCONTENTS:
        map.tableOfContents = entry;
        break;
    case (uint32_t)RwObjectType::RWOBJECTTYPE_VERSIONDATA:
        map.versionData = entry;
        break;
    case (uint32_t)RwObjectType::RWOBJECTTYPE_RENDERMATERIALDATA:
        map.renderMaterialData = entry;
        break;
    case (uint32_t)RwObjectType::RWOBJECTTYPE_INSTANCEDATA:
        map.instanceData = entry;
        break;
    case (uint32_t)RwObjectType::RWOBJECTTYPE_STATSDATA:
        map.statsData = entry;
        break;
    case (uint32_t)RwObjectType::RWGOBJECTTYPE_TEXTURE:
        map.textures.push_back(entry);
        break;
    case (uint32_t)RwObjectType::RWGOBJECTTYPE_PALETTE_1:
    case (uint32_t)RwObjectType::RWGOBJECTTYPE_PALETTE_2:
        map.palettes.push_back(entry);
        break;
    case (uint32_t)RwObjectType::RWGOBJECTTYPE_VERTEXBUFFER:
        map.vertexBuffers.push_back(entry);
        break;
    case (uint32_t)RwObjectType::RWGOBJECTTYPE_INDEXBUFFER:
        map.indexBuffers.push_back(entry);
        break;
    case (uint32_t)RwObjectType::RWGOBJECTTYPE_VERTEXDESCRIPTOR:
        map.vertexDescriptors.push_back(entry);
        break;
    case (uint32_t)RwObjectType::RWGOBJECTTYPE_MESHHELPER:
        map.meshHelpers.push_back(entry);
        break;
    case (uint32_t)RwObjectType::RWOBJECTTYPE_RENDEROPTIMESHDATA:
        map.renderMeshDatas.push_back(entry);
        break;
    case (uint32_t)RwObjectType::RWOBJECTTYPE_RENDERMODELDATA:
        map.renderModelDatas.push_back(entry);
        break;
    default:
        break;
    }
}

template <typename T>
inline const T* ptrAt(const std::vector<uint8_t>& buf, size_t offset) {
    return reinterpret_cast<const T*>(buf.data() + offset);
}

template <typename T>
inline T readAt(const std::vector<uint8_t>& buf, size_t offset) {
    T tmp;
    std::memcpy(&tmp, buf.data() + offset, sizeof(T));
    return tmp;
}

auto toStr = [](uint32_t v) {
    const char* c = reinterpret_cast<const char*>(&v);
    return std::string(c, strnlen(c, 4)); // read up to 4 chars or until '\0'
    };

std::string readNullTerminated(const std::vector<uint8_t>& fileData,
    uint32_t offset)
{
    std::string result;

    if (offset == 0)
        return result;

    const char* base = reinterpret_cast<const char*>(
        fileData.data() + offset
        );

    const char* ptr = base;

    // Read until '\0'
    while (*ptr != '\0') {
        result.push_back(*ptr);
        ++ptr;
    }

    return result;
}

struct PegasusV22 { 
    using Material = pegasus::v22::tRMaterial; 
    using Instance = pegasus::v22::tInstance;
};
struct PegasusV24 { 
    using Material = pegasus::v24::tRMaterial; 
    using Instance = pegasus::v24::tInstance;
};
struct PegasusV25 { 
    using Material = pegasus::v25::tRMaterial; 
    using Instance = pegasus::v25::tInstance;
};

template<typename Pegasus>
void ReadRenderMaterials(const std::vector<uint8_t>& fileData, uint32_t rMatOffset, int e)
{
    using MaterialType = typename Pegasus::Material;
    using ChannelType = typename MaterialType::tChannel;

    // Get material data (this is common across versions)
    const auto* materialData = ptrAt<pegasus::tRMaterialData>(fileData, rMatOffset);

    uint32_t numMaterials = swapEndianness(materialData->m_uiNumMaterials, e);
    uint32_t numChannels = swapEndianness(materialData->m_uiNumChannels, e);

    // Read materials
    const auto* materialArray = ptrAt<MaterialType>(
        fileData, rMatOffset + swapEndianness(materialData->m_pMaterials, e)
    );
    std::vector<MaterialType> materials(materialArray, materialArray + numMaterials);

    // Read channels
    const auto* channelsArray = ptrAt<ChannelType>(
        fileData, rMatOffset + swapEndianness(materialData->m_pChannels, e)
    );
    std::vector<ChannelType> channels(channelsArray, channelsArray + numChannels);

    // Read channel names
    for (uint32_t i = 0; i < numChannels; ++i) {
        uint32_t shaderInputOffset = swapEndianness(channels[i].m_ShaderInput, e);
        uint32_t streamNameOffset = swapEndianness(channels[i].m_StreamName, e);
        uint64_t streamID = swapEndianness(channels[i].m_uiGuid, e);

        std::string shaderName = readNullTerminated(fileData, rMatOffset + shaderInputOffset);
        std::string streamName = readNullTerminated(fileData, rMatOffset + streamNameOffset);

        std::cout << "Channel[" << i << "]\n";
        std::cout << "  StreamID    : " << streamID << "\n";
        std::cout << "  ShaderInput : " << shaderName << "\n";
        std::cout << "  StreamName  : " << (streamName.empty() ? "<none>" : streamName) << "\n";
    }
}

template<typename Pegasus>
void ReadInstanceData(const std::vector<uint8_t>& fileData, uint32_t instanceOffset, int e)
{
    using Instance = typename Pegasus::Instance;

    // Base pointer to tInstanceData
    const auto* instanceData = ptrAt<pegasus::tInstanceData>(fileData, instanceOffset);

    uint32_t numInstances = swapEndianness(instanceData->m_uiNumInstances, e);
    uint32_t instancesOffset = swapEndianness(instanceData->m_Instances, e);

    uint32_t numStrings = swapEndianness(instanceData->m_uiNumStrings, e);
    uint32_t stringsOffset = swapEndianness(instanceData->m_StringList, e);

    // Read instances
    const auto* instancesArray = ptrAt<Instance>(fileData, instanceOffset + instancesOffset);
    std::vector<Instance> instances(instancesArray, instancesArray + numInstances);

    // Read instance strings
    for (uint32_t i = 0; i < numInstances; ++i) {
        uint32_t name = swapEndianness(instances[i].m_Name, e);
        uint32_t cModel = swapEndianness(instances[i].m_pCModel, e);
        uint32_t rModel = swapEndianness(instances[i].m_pRModel, e);
        uint64_t guid = swapEndianness(instances[i].m_uiGuid, e);
        uint64_t guidLocal = swapEndianness(instances[i].m_uiGuidLocal, e);
        uint32_t desc = swapEndianness(instances[i].m_Description, e);
        uint32_t comp = swapEndianness(instances[i].m_Component, e);
        uint32_t categ = swapEndianness(instances[i].m_Category, e);

        std::string instanceName = readNullTerminated(fileData, instanceOffset + name);
        std::string instanceDescription = readNullTerminated(fileData, instanceOffset + desc);
        std::string instanceComponent = readNullTerminated(fileData, instanceOffset + comp);
        std::string instanceCategory = readNullTerminated(fileData, instanceOffset + categ);

        std::cout << "Instance[" << i << "]\n";
        std::cout << "  CModel      : " << cModel << "\n";
        std::cout << "  RModel      : " << rModel << "\n";
        std::cout << "  GUID        : " << guid << "\n";
        std::cout << "  Local GUID  : " << guidLocal << "\n";
        std::cout << "  Name        : " << instanceName << "\n";
        std::cout << "  Description : " << instanceDescription << "\n";
        std::cout << "  Component   : " << instanceComponent << "\n";
        std::cout << "  Category    : " << instanceCategory << "\n";
    }
}

void load_RW4_Arena(const std::string& inputfile) {
    // --- Load full file into memory ---
    std::ifstream file(inputfile, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << inputfile << "\n";
        return;
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    file.seekg(0);

    std::vector<uint8_t> fileData(fileSize);
    file.read(reinterpret_cast<char*>(fileData.data()), fileSize);
    file.close();

    std::cout << "Loaded " << fileSize << " bytes into memory.\n";

    // --- Read Arena header ---
    size_t offset = 0;
    const auto* arena = ptrAt<rw::core::arena::Arena>(fileData, offset);
    offset += sizeof(*arena);

    if (arena->fileHeader.magicNumber.prefix != 0x34575289) {
        std::cout << "Not a valid RW4 Arena file.\n";
        return;
    }

    bool e = arena->fileHeader.isBigEndian;
    Platform platform = detectPlatform(arena->fileHeader.magicNumber);
    PlatformInfo platformInfo = platformMapping.at(platform);

    std::cout << "RW4 Arena version: "
        << toStr(arena->fileHeader.majorVersion) << "."
        << toStr(arena->fileHeader.minorVersion)
        << "\n";

    // --- Load resource descriptors ---
    auto [resourceDescriptors, offsetAfterRes] =
        loadResourceDescriptors(fileData, offset, platformInfo.numDescriptors, e);

    auto [resourcesUsed, offsetAfterUsed] =
        loadResourceDescriptors(fileData, offsetAfterRes, platformInfo.numDescriptors, e);

    offset = offsetAfterUsed;

    // --- Load TargetResources ---
    std::vector<rw::core::arena::TargetResource> targetResources;
    targetResources.reserve(platformInfo.numResources);
    for (int i = 0; i < platformInfo.numResources; ++i) {
        rw::core::arena::TargetResource res{};
        std::memcpy(&res, fileData.data() + offset, sizeof(res));
        targetResources.push_back(res);
        offset += sizeof(res);
    }

    // --- Manifest ---
    const auto* manifest = ptrAt<rw::core::arena::Manifest>(fileData, offset);
    offset += sizeof(*manifest);

    uint32_t dictEntryCount = swapEndianness(manifest->numEntries, e);

    std::vector<rw::core::arena::Dictionary_Entry> dictEntries(dictEntryCount);
    std::memcpy(dictEntries.data(), fileData.data() + offset, dictEntryCount * sizeof(rw::core::arena::Dictionary_Entry));
    offset += dictEntryCount * sizeof(rw::core::arena::Dictionary_Entry);

    // --- Types ---
    const auto* types = ptrAt<rw::core::arena::Types>(fileData, offset);
    offset += sizeof(*types);

    uint32_t typeCount = swapEndianness(types->numEntries, e);

    std::vector<rw::core::arena::Type_Entry> typeEntries(typeCount);
    std::memcpy(typeEntries.data(), fileData.data() + offset, typeCount * sizeof(rw::core::arena::Type_Entry));
    offset += typeCount * sizeof(rw::core::arena::Type_Entry);

    // --- External Arenas ---
    const auto* externalArenas = ptrAt<rw::core::arena::External_Arenas>(fileData, offset);
    offset += sizeof(*externalArenas);

    const auto* externalArena = ptrAt<rw::core::arena::External_Arena>(fileData, offset);
    offset += sizeof(*externalArena);

    uint32_t extArenaCount = swapEndianness(externalArenas->numEntries, e);
    std::vector<rw::core::arena::External_Arena_Entry> externalArenaEntries(extArenaCount);
    std::memcpy(externalArenaEntries.data(), fileData.data() + offset, extArenaCount * sizeof(rw::core::arena::External_Arena_Entry));
    offset += extArenaCount * sizeof(rw::core::arena::External_Arena_Entry);

    // --- Subreferences ---
    const auto* subrefs = ptrAt<rw::core::arena::Subreferences>(fileData, offset);
    offset += sizeof(*subrefs);

	uint32_t records = swapEndianness(subrefs->records, e);
    uint32_t subrefCount = swapEndianness(subrefs->numEntries, e);
    std::vector<rw::core::arena::Arena_Subreference_Record> subrefRecords(subrefCount);
    std::memcpy(subrefRecords.data(), fileData.data() + records, subrefCount * sizeof(rw::core::arena::Arena_Subreference_Record));

    // --- Atoms ---
    const auto* atoms = ptrAt<rw::core::arena::Atoms>(fileData, offset);
    offset += sizeof(*atoms);

    uint32_t atomCount = swapEndianness(atoms->numEntries, e);

    // --- Dictionary ---
    uint32_t dictStart = swapEndianness(arena->dictStart, e);
    uint32_t dictNum = swapEndianness(arena->numEntries, e);

    ObjectTypeMap objectTypeMap{};
    const auto* dictPtr = ptrAt<rw::core::arena::arenaDictEntry>(fileData, dictStart);

    for (uint32_t i = 0; i < dictNum; ++i) {
        rw::core::arena::arenaDictEntry entry = dictPtr[i];

        //std::cout << "typeIndex: " << swapEndianness(entry.typeIndex, e) << "\n";
		//std::cout << "objectType: " << objectTypes[swapEndianness(entry.typeId, e)] << "\n";
        AddToObjectTypeMap(entry, objectTypeMap, e);
    }

    // --- Parse arena types ---  //parseArenaTypes(objectTypeMap, fileData, e);
	// --- Pegasus Version Data ---
    uint32_t uiVersion = 0;
    uint32_t uiRevision = 0;
    if (objectTypeMap.versionData.ptr != 0) {
        auto versionOffset = swapEndianness(objectTypeMap.versionData.ptr, e);

        const auto* versionData = ptrAt<pegasus::tVersionData>(fileData, versionOffset);
        uiVersion = swapEndianness(versionData->m_uiVersion, e);
        uint32_t uiRevision = swapEndianness(versionData->m_uiRevision, e);
        std::cout << "Pegasus Version: " << uiVersion << "." << uiRevision << "\n";
        std::cout << "\n";
    }
    // --- Table of Contents ---
    if (objectTypeMap.tableOfContents.ptr != 0) {
        auto tocOffset = swapEndianness(objectTypeMap.tableOfContents.ptr, e);
        std::cout << "TOC Offset: " << tocOffset << "\n";
    
        // Access header directly
        const auto* tocHeader = ptrAt<pegasus::tTableOfContents>(fileData, tocOffset);

        uint32_t itemCount = swapEndianness(tocHeader->m_uiItemsCount, e);
        uint32_t namesOffset = swapEndianness(tocHeader->m_pNames, e);
        uint32_t tocEntriesOffset = swapEndianness(tocHeader->m_pArray, e);
        uint32_t typeMapOffset = swapEndianness(tocHeader->m_pTypeMap, e);

        std::cout << "TOC Entries Offset: " << tocEntriesOffset << "\n";
        std::cout << "Names Offset: " << namesOffset << "\n";
        std::cout << "Type Map: " << typeMapOffset << "\n";
        std::cout << "\n";

        // Access entries in memory
        const auto* entries = ptrAt<pegasus::tTOCEntry>(fileData, tocOffset + tocEntriesOffset);
        const char* namesData = reinterpret_cast<const char*>(fileData.data() + tocOffset + namesOffset);
        size_t namesSize = typeMapOffset - namesOffset;

        std::vector<std::pair<std::string, uint32_t>> names;
        names.reserve(itemCount);

        for (uint32_t i = 0; i < itemCount; ++i) {
            uint32_t nameOffset = swapEndianness(entries[i].m_Name, e);
            uint32_t object = swapEndianness(entries[i].m_pObject, e);

            std::string name;
            if (nameOffset && nameOffset >= namesOffset && nameOffset < typeMapOffset) {
                const char* str = namesData + (nameOffset - namesOffset);
                name = std::string(str);
            }

            uint16_t objectType = static_cast<uint16_t>((object >> 16) & 0xFFFF);
            uint16_t objectIndex = static_cast<uint16_t>(object & 0xFFFF);

            if (!name.empty()) {
                std::cout << "name: " << name << "\n";
			}
            std::cout << "ID: " << swapEndianness(entries[i].m_uiGuid, e) << "\n";
            std::cout << "Type: " << swapEndianness(entries[i].m_Type, e) << " " << objectTypes[swapEndianness(entries[i].m_Type, e)] << "\n";
            std::cout << "unknown: " << swapEndianness(entries[i].unknown, e) << "\n";
            uint32_t dictIndex = 0;

			if (objectType == 0x80) { //subreference
			    auto subrefRecord = subrefRecords[objectIndex];
			    std::cout << "  Offset: " << swapEndianness(subrefRecord.offset, e) << "\n";
				dictIndex = swapEndianness(subrefRecord.objectID, e);
            }
			else { //regular object
                dictIndex = objectIndex;
            }

            rw::core::arena::arenaDictEntry entry = dictPtr[dictIndex];
            std::cout << "typeIndex: " << swapEndianness(entry.typeIndex, e) << "\n";
            std::cout << "objectType: " << objectTypes[swapEndianness(entry.typeId, e)] << "\n";
			std::cout << "\n";
            names.emplace_back(std::move(name), object);
        }
        std::cout << "\n";
    }

    // -- RenderMaterialData ---
    if (objectTypeMap.renderMaterialData.ptr != 0) {
        auto rMatOffset = swapEndianness(objectTypeMap.renderMaterialData.ptr, e);

        switch (uiVersion) {
        case 22: ReadRenderMaterials<PegasusV22>(fileData, rMatOffset, e); break;
        case 24: ReadRenderMaterials<PegasusV24>(fileData, rMatOffset, e); break;
        case 25: ReadRenderMaterials<PegasusV25>(fileData, rMatOffset, e); break;
        default: throw std::runtime_error("Unsupported Pegasus version");
        }
        std::cout << "\n";
    }

	// -- InstanceData ---
    if (objectTypeMap.instanceData.ptr != 0) {
        auto instanceOffset = swapEndianness(objectTypeMap.instanceData.ptr, e);

        switch (uiVersion) {
        case 22: ReadInstanceData<PegasusV22>(fileData, instanceOffset, e); break;
        case 24: ReadInstanceData<PegasusV24>(fileData, instanceOffset, e); break;
        case 25: ReadInstanceData<PegasusV25>(fileData, instanceOffset, e); break;
        default: throw std::runtime_error("Unsupported Pegasus version");
        }
        std::cout << "\n";
    }

    // maybe also figure out how to use types from indices inferred originally?
    
	// --- StatsData ---
    if (objectTypeMap.statsData.ptr != 0) {
        auto statsOffset = swapEndianness(objectTypeMap.statsData.ptr, e);
        const auto* statsdata = ptrAt<pegasus::tStatsData>(fileData, statsOffset);
    }

	// --- Base Resources ---
    for (const auto& baseresource : objectTypeMap.baseResources) {
        auto baseResOffset = swapEndianness(baseresource.ptr, e);
        std::cout << "Base Resource Data Offset: " << baseResOffset << "\n";
        std::cout << "\n";
	}
	// --- Vertex Buffers ---
    for (const auto& vertexBuffer : objectTypeMap.vertexBuffers) {
        auto vertexBufferOffset = swapEndianness(vertexBuffer.ptr, e);
        std::cout << "Vertex Buffer Data Offset: " << vertexBufferOffset << "\n";
        if (platform == Platform::PS3) {
            const auto* vertexbuffer = ptrAt<renderengine::vertexBufferps3>(fileData, vertexBufferOffset);
            std::cout << "buffer: " << vertexbuffer->m_buffer << "\n";
            std::cout << "offset: " << swapEndianness(vertexbuffer->m_offset, e) << "\n";
			std::cout << "bufferSize: " << swapEndianness(vertexbuffer->m_bufferSize, e) << "\n";
            std::cout << "location: " << vertexbuffer->m_location << "\n";
            std::cout << "lockFlags: " << vertexbuffer->m_lockFlags << "\n";
        }
        else if (platform == Platform::XBOX) {
            const auto* vertexbuffer = ptrAt<renderengine::vertexBufferxbox>(fileData, vertexBufferOffset);
            std::cout << "Common: " << swapEndianness(vertexbuffer->Common, e) << "\n";
            std::cout << "ReferenceCount: " << swapEndianness(vertexbuffer->ReferenceCount, e) << "\n";
            std::cout << "Fence: " << swapEndianness(vertexbuffer->Fence, e) << "\n";
            std::cout << "ReadFence: " << swapEndianness(vertexbuffer->ReadFence, e) << "\n";
            std::cout << "Identifier: " << swapEndianness(vertexbuffer->Identifier, e) << "\n";
            std::cout << "BaseFlush: " << swapEndianness(vertexbuffer->BaseFlush, e) << "\n";
            std::cout << "GPUVERTEX_FETCH_CONSTANT[0]: " << swapEndianness(vertexbuffer->GPUVERTEX_FETCH_CONSTANT[0], e) << "\n";
            std::cout << "GPUVERTEX_FETCH_CONSTANT[1]: " << swapEndianness(vertexbuffer->GPUVERTEX_FETCH_CONSTANT[1], e) << "\n";
            std::cout << "bufferSize: " << swapEndianness(vertexbuffer->m_bufferSize, e) << "\n";
            std::cout << "type: " << swapEndianness(vertexbuffer->m_type, e) << "\n";
        }
        else if (platform == Platform::WII) {
            const auto* vertexbuffer = ptrAt<renderengine::vertexBufferwii>(fileData, vertexBufferOffset);
            std::cout << "bufferSize: " << swapEndianness(vertexbuffer->m_bufferSize, e) << "\n";
            std::cout << "type: " << swapEndianness(vertexbuffer->m_type, e) << "\n";
        }
		std::cout << "\n";
	}
    // --- Index Buffers ---
    for (const auto& indexBuffer : objectTypeMap.indexBuffers) {
        auto indexBufferOffset = swapEndianness(indexBuffer.ptr, e);
		std::cout << "Index Buffer Data Offset: " << indexBufferOffset << "\n";
        if (platform == Platform::PS3) {
            const auto* indexbuffer = ptrAt<renderengine::indexBufferps3>(fileData, indexBufferOffset);
            std::cout << "indexbuffer: " << swapEndianness(indexbuffer->m_buffer, e) << "\n";
            std::cout << "indextype: " << indexbuffer->m_indexType << "\n";
            std::cout << "location: " << indexbuffer->m_location << "\n";
            std::cout << "lockflags: " << indexbuffer->m_lockFlags << "\n";
			std::cout << "numindices: " << swapEndianness(indexbuffer->m_numIndices, e) << "\n";
            std::cout << "offset: " << swapEndianness(indexbuffer->m_offset, e) << "\n";
			std::cout << "stride: " << swapEndianness(indexbuffer->m_stride, e) << "\n";
            std::cout << "type: " << indexbuffer->m_type << "\n";
        }
        else if (platform == Platform::XBOX) {
            const auto* indexbuffer = ptrAt<renderengine::indexBufferxbox>(fileData, indexBufferOffset);
            std::cout << "Common: " << swapEndianness(indexbuffer->Common, e) << "\n";
            std::cout << "ReferenceCount: " << swapEndianness(indexbuffer->ReferenceCount, e) << "\n";
            std::cout << "Fence: " << swapEndianness(indexbuffer->Fence, e) << "\n";
            std::cout << "ReadFence: " << swapEndianness(indexbuffer->ReadFence, e) << "\n";
            std::cout << "Identifier: " << swapEndianness(indexbuffer->Identifier, e) << "\n";
            std::cout << "BaseFlush: " << swapEndianness(indexbuffer->BaseFlush, e) << "\n";
            std::cout << "Address: " << swapEndianness(indexbuffer->Address, e) << "\n";
            std::cout << "Size: " << swapEndianness(indexbuffer->Size, e) << "\n";
            std::cout << "numindices: " << swapEndianness(indexbuffer->m_numIndices, e) << "\n";
        }
        else if (platform == Platform::WII) {
            const auto* indexbuffer = ptrAt<renderengine::indexBufferwii>(fileData, indexBufferOffset);
            std::cout << "type: " << swapEndianness(indexbuffer->m_type, e) << "\n";
            std::cout << "depth: " << swapEndianness(indexbuffer->m_depth, e) << "\n";
            std::cout << "bufferSize: " << swapEndianness(indexbuffer->m_bufferSize, e) << "\n";
            std::cout << "numIndices: " << swapEndianness(indexbuffer->m_numIndices, e) << "\n";
        }
        std::cout << "\n";
	}
	// --- Vertex Descriptors ---
    for (const auto& vertexDescriptor : objectTypeMap.vertexDescriptors) {
        auto vertexDescriptorOffset = swapEndianness(vertexDescriptor.ptr, e);
        std::cout << "Vertex Descriptor Data Offset: " << vertexDescriptorOffset << "\n";

        if (platform == Platform::PS3) {
            const auto* vertexdps3 = ptrAt<renderengine::vertexDescriptorps3>(fileData, vertexDescriptorOffset);
            std::cout << "m_numElements: " << swapEndianness(vertexdps3->m_numElements, e) << "\n";
            std::cout << "m_numStreams: " << swapEndianness(vertexdps3->m_numStreams, e) << "\n";
            std::cout << "m_refCount: " << swapEndianness(vertexdps3->m_refCount, e) << "\n";
            std::cout << "m_typesFlags: " << swapEndianness(vertexdps3->m_typesFlags, e) << "\n";

			uint32_t numElements = swapEndianness(vertexdps3->m_numElements, e);
            std::vector<renderengine::VertexDescriptorElementps3> vertexdelemps3(numElements);
            std::memcpy(vertexdelemps3.data(), fileData.data() + vertexDescriptorOffset + sizeof(renderengine::vertexDescriptorps3), numElements * sizeof(renderengine::VertexDescriptorElementps3));

            for (size_t i = 0; i < vertexdelemps3.size(); ++i) {
                const auto& elem = vertexdelemps3[i];
                std::cout << "VertexDescriptorElement " << i << "\n";
                std::cout << "elementClass: " << elementClass[elem.elementClass] << "\n";
                std::cout << "numComponents: " << static_cast<int>(elem.numComponents) << "\n";
                std::cout << "offset: " << static_cast<int>(elem.offset) << "\n";
                std::cout << "stream: " << static_cast<int>(elem.stream) << "\n";
                std::cout << "stride: " << swapEndianness(elem.stride, e) << "\n";
                std::cout << "type: " << elementTypesps3[elem.type] << "\n";
                std::cout << "vertexType: " << vertexTypesps3[elem.vertexType] << "\n";
            }
        }
        else if (platform == Platform::XBOX) {
            const auto* vertexdxbox = ptrAt<renderengine::vertexDescriptorxbox>(fileData, vertexDescriptorOffset);
            std::cout << "m_numElements: " << swapEndianness(vertexdxbox->m_numElements, e) << "\n";
            std::cout << "m_numStreams: " << swapEndianness(vertexdxbox->m_numStreams, e) << "\n";
            std::cout << "m_refCount: " << swapEndianness(vertexdxbox->m_refCount, e) << "\n";
            std::cout << "m_typesFlags: " << swapEndianness(vertexdxbox->m_typesFlags, e) << "\n";

            uint32_t numElements = swapEndianness(vertexdxbox->m_numElements, e);
            std::vector<renderengine::VertexDescriptorElementxbox> vertexdelemxbox(numElements);
            std::memcpy(vertexdelemxbox.data(), fileData.data() + vertexDescriptorOffset + sizeof(renderengine::vertexDescriptorxbox), numElements * sizeof(renderengine::VertexDescriptorElementxbox));

            for (size_t i = 0; i < vertexdelemxbox.size(); ++i) {
                const auto& elem = vertexdelemxbox[i];
                std::cout << "VertexDescriptorElement " << i << "\n";
                std::cout << "elementClass: " << elementClass[swapEndianness(elem.elementClass, e)] << "\n";
                std::cout << "indexedUsage: " << indexedUsage[elem.indexedUsage] << "\n";
                std::cout << "offset: " << swapEndianness(elem.offset, e) << "\n";
                std::cout << "stream: " << swapEndianness(elem.stream, e) << "\n";
                std::cout << "method: " << static_cast<int>(elem.method) << "\n";
                std::cout << "type: " << elementTypesxbox[swapEndianness(elem.type, e)] << "\n";
                std::cout << "usage: " << d3ddeclUsage[elem.usage] << "\n";
                std::cout << "usageIndex: " << static_cast<int>(elem.usageIndex) << "\n";
            }
        }
        else if (platform == Platform::WII) {
            const auto* vertexdwii = ptrAt<renderengine::vertexDescriptorwii>(fileData, vertexDescriptorOffset);
            std::cout << "numVBuffs: " << static_cast<int>(swapEndianness(vertexdwii->m_numStreams, e)) << "\n";
            std::cout << "numElements: " << static_cast<int>(swapEndianness(vertexdwii->m_numElements, e)) << "\n";

            uint32_t numElements = swapEndianness(vertexdwii->m_numElements, e);
            std::vector<renderengine::VertexDescriptorElementwii> vertexdelemwii(numElements);
            std::memcpy(vertexdelemwii.data(), fileData.data() + vertexDescriptorOffset + sizeof(renderengine::vertexDescriptorwii), numElements * sizeof(renderengine::VertexDescriptorElementwii));

            for (size_t i = 0; i < vertexdelemwii.size(); ++i) {
                const auto& elem = vertexdelemwii[i];
                std::cout << "VertexDescriptorElement " << i << "\n";
                std::cout << "attribute: " << Attribute[swapEndianness(elem.attr, e)] << "\n";
                if (componentTypeMapping[swapEndianness(elem.attr, e)] == "POS") {
                    std::cout << "compType: " << ComponentTypePOS[static_cast<int>(swapEndianness(elem.cnt, e))] << "\n";
                    std::cout << "compSize: " << ComponentSizeOTHER[static_cast<int>(swapEndianness(elem.type, e))] << "\n";
                }
                else if (componentTypeMapping[swapEndianness(elem.attr, e)] == "NRM") {
                    std::cout << "compType: " << ComponentTypeNRM[static_cast<int>(swapEndianness(elem.cnt, e))] << "\n";
                    std::cout << "compSize: " << ComponentSizeOTHER[static_cast<int>(swapEndianness(elem.type, e))] << "\n";
                }
                else if (componentTypeMapping[swapEndianness(elem.attr, e)] == "CLR") {
                    std::cout << "compType: " << ComponentTypeCLR[static_cast<int>(swapEndianness(elem.cnt, e))] << "\n";
                    std::cout << "compSize: " << ComponentSizeOTHER[static_cast<int>(swapEndianness(elem.type, e))] << "\n";
                }
                else if (componentTypeMapping[swapEndianness(elem.attr, e)] == "TEX") {
                    std::cout << "compType: " << ComponentTypeTEX[static_cast<int>(swapEndianness(elem.cnt, e))] << "\n";
                    std::cout << "compSize: " << ComponentSizeTEX[static_cast<int>(swapEndianness(elem.type, e))] << "\n";
                }
                else {
                    std::cout << "compType: " << static_cast<int>(swapEndianness(elem.cnt, e)) << "\n";
                    std::cout << "compSize: " << ComponentSizeOTHER[static_cast<int>(swapEndianness(elem.type, e))] << "\n";
                }
                std::cout << "attributeType: " << static_cast<int>(swapEndianness(elem.attributeType, e)) << "\n";
                std::cout << "frac: " << static_cast<int>(swapEndianness(elem.frac, e)) << "\n";
                std::cout << "stride: " << static_cast<int>(swapEndianness(elem.stride, e)) << "\n";
                std::cout << "stream: " << static_cast<int>(swapEndianness(elem.stream, e)) << "\n";
                std::cout << "stride: " << static_cast<int>(swapEndianness(elem.stride, e)) << "\n";
                std::cout << "offset: " << swapEndianness(elem.offset, e) << "\n";
            }
        }
        std::cout << "\n";
	}
	// --- Mesh Helpers ---
    for (const auto& meshHelper : objectTypeMap.meshHelpers) {
        auto meshHelperOffset = swapEndianness(meshHelper.ptr, e);
        std::cout << "Mesh Helper Data Offset: " << meshHelperOffset << "\n";
        const auto* meshhelper = ptrAt<renderengine::meshHelper>(fileData, meshHelperOffset);
        uint32_t numIndexBuffers = swapEndianness(meshhelper->m_numIndexBuffers, e);
		uint32_t numVertexBuffers = swapEndianness(meshhelper->m_numVertexBuffers, e);

        uint32_t currentBuffer = meshHelperOffset + sizeof(renderengine::meshHelper);
        for (size_t i = 0; i < numIndexBuffers; ++i) {
            uint32_t raw = *ptrAt<uint32_t>(fileData, currentBuffer);
            uint32_t value = swapEndianness(raw, e);
            std::cout << "Index Buffer[" << i << "]: " << value << "\n";
            currentBuffer += 4;
        }

        for (size_t i = 0; i < numVertexBuffers; ++i) {
            uint32_t raw = *ptrAt<uint32_t>(fileData, currentBuffer);
            uint32_t value = swapEndianness(raw, e);
            std::cout << "Vertex Buffer[" << i << "]: " << value << "\n";
            currentBuffer += 4;
        }

        std::cout << "\n";
	}
	// --- Render Mesh Datas ---
    for (const auto& renderMeshData : objectTypeMap.renderMeshDatas) {
        auto renderMeshDataOffset = swapEndianness(renderMeshData.ptr, e);
        std::cout << "Render Mesh Data Offset: " << renderMeshDataOffset << "\n";
        const auto* rendermeshdata = ptrAt<pegasus::tRMeshData>(fileData, renderMeshDataOffset);
        std::cout << "MeshHelper: " << swapEndianness(rendermeshdata->m_pMeshHelper, e) << "\n";
        std::cout << "indexBuffer: " << swapEndianness(rendermeshdata->m_pIndexBuffer, e) << "\n";
        std::cout << "VertexBuffer: " << swapEndianness(rendermeshdata->m_pVertexBuffer, e) << "\n";
        std::cout << "uiNumVerts: " << swapEndianness(rendermeshdata->m_uiNumVerts, e) << "\n";
        std::cout << "DrawParams" "\n";
        std::cout << "  DrawIndexedParameters: " << swapEndianness(rendermeshdata->m_DrawParams.m_pDrawIndexedParameters, e) << "\n";
        std::cout << "  DrawParameters: " << swapEndianness(rendermeshdata->m_DrawParams.m_pDrawParameters, e) << "\n";
        std::cout << "pRemapTable: " << swapEndianness(rendermeshdata->m_pRemapTable, e) << "\n";
        std::cout << "bIsIndexed: " << swapEndianness(rendermeshdata->m_bIsIndexed, e) << "\n";
        std::cout << "uiNumBoneMats: " << swapEndianness(rendermeshdata->m_uiNumBoneMats, e) << "\n";
        std::cout << "NumBlendShapes: " << swapEndianness(rendermeshdata->m_uiNumBlendShapes, e) << "\n";
        std::cout << "BlendShapeTable: " << swapEndianness(rendermeshdata->m_pBlendShapeTable, e) << "\n";
        std::cout << "BlendShapeNames: " << swapEndianness(rendermeshdata->m_szBlendShapeNames, e) << "\n";
        std::cout << "\n";
	}
	// --- Render Model Datas ---
    for (const auto& renderModelData : objectTypeMap.renderModelDatas) {
        auto renderModelDataOffset = swapEndianness(renderModelData.ptr, e);
        std::cout << "Render Model Data Offset: " << renderModelDataOffset << "\n";
        const auto* rendermodeldata = ptrAt<pegasus::tRModelData>(fileData, renderModelDataOffset);
        std::cout << "IBPMatrices: " << swapEndianness(rendermodeldata->m_pIBPMatrices, e) << "\n";
        std::cout << "MeshTable: " << swapEndianness(rendermodeldata->m_pMeshTable, e) << "\n";
        std::cout << "BoneNameTable: " << swapEndianness(rendermodeldata->m_pBoneNameTable, e) << "\n";
        std::cout << "BoneNameList: " << swapEndianness(rendermodeldata->m_pBoneNameList, e) << "\n";
        std::cout << "NumTotalBones: " << swapEndianness(rendermodeldata->m_iNumTotalBones, e) << "\n";
        std::cout << "NumMeshes: " << swapEndianness(rendermodeldata->m_iNumMeshes, e) << "\n";
        std::cout << "NumBones: " << swapEndianness(rendermodeldata->m_iNumBones, e) << "\n";
        std::cout << "NumIslands: " << swapEndianness(rendermodeldata->m_iNumIslands, e) << "\n";
        std::cout << "IslandAABBs: " << swapEndianness(rendermodeldata->m_pIslandAABBs, e) << "\n";
        std::cout << "IslandAreas: " << swapEndianness(rendermodeldata->m_pIslandAreas, e) << "\n";
        std::cout << "\n";
    }

    // --- Palettes ---
	for (const auto& palette : objectTypeMap.palettes) {
        auto paletteOffset = swapEndianness(palette.ptr, e);
        std::cout << "Palette Data Offset: " << paletteOffset << "\n";

        // Get base pointer to Palette in memory
        const auto* palettedata = ptrAt<Palette>(fileData, paletteOffset);

        std::cout << swapEndianness(palettedata->m_format, e) << "\n";
        std::cout << swapEndianness(palettedata->m_length, e) << "\n";
        std::cout << "\n";
    }

    // --- Textures ---
    for (auto& texture : objectTypeMap.textures) {
        auto textureOffset = swapEndianness(texture.ptr, e);
		texture.alignment = swapEndianness(texture.alignment, e);
        std::cout << "Texture Data Offset: " << textureOffset << "\n";
        std::cout << "\n";
    }
}
/*
void parseArenaTypes(ObjectTypeMap& objectTypeMap, std::ifstream& inputFile, bool e) {
    auto tocOffset = swapEndianness(objectTypeMap.tableOfContents.ptr, e);
	std::cout << "TOC Offset: " << tocOffset << "\n";

    // Read header
    pegasus::tTableOfContents header;
    inputFile.seekg(tocOffset);
    inputFile.read(reinterpret_cast<char*>(&header), sizeof(header));

    // Swap endian values
    uint32_t itemCount = swapEndianness(header.m_uiItemsCount, e);
    uint32_t namesOffset = swapEndianness(header.m_pNames, e);
    uint32_t typeCount = swapEndianness(header.m_uiTypeCount, e);
    uint32_t tocEntriesOffset = swapEndianness(header.m_pArray, e);
	std::cout << "TOC Entries Offset: " << tocEntriesOffset << "\n";
    std::cout << "Names Offset: " << namesOffset << "\n";

    // Read TOC entries
    std::vector<pegasus::tTOCEntry> entries(itemCount);
    inputFile.seekg(tocOffset + tocEntriesOffset);
    inputFile.read(reinterpret_cast<char*>(entries.data()), sizeof(pegasus::tTOCEntry) * itemCount);

    // Read all names section into memory
    inputFile.seekg(tocOffset + namesOffset);
	std::cout << "Names Size: " << (swapEndianness(header.m_pTypeMap, e) - namesOffset) << "\n";
    std::vector<char> namesData(swapEndianness(header.m_pTypeMap, e) - namesOffset);
    inputFile.read(namesData.data(), namesData.size());

    // Build names list
    std::vector<std::pair<std::string, uint32_t>> names;
    for (auto& entry : entries) {
        uint32_t nameOffset = swapEndianness(entry.m_Name, e);
		std::cout << "Name offset: " << nameOffset << "\n";
        std::string name;
        if (nameOffset && nameOffset >= namesOffset && nameOffset - namesOffset < namesData.size()) {
            name = std::string(&namesData[nameOffset - namesOffset]);
        }
		std::cout << "Found type: " << name << "\n";
        names.emplace_back(name, swapEndianness(entry.m_pObject, e));
    }
};

void load_RW4_Arena(const std::string& inputfile) {
    std::ifstream inputFile(inputfile, std::ios::binary);

    if (!inputFile.is_open()) {
        std::cerr << "Error: could not open file " << inputfile << "\n";
        return;
    }
    rw::core::arena::Arena arena{};
    inputFile.read(reinterpret_cast<char*>(&arena), sizeof(arena));

    if (arena.fileHeader.magicNumber.prefix != 0x34575289) {
        std::cout << "Not a valid RW4 Arena file.\n";
        return;
	}

    std::string filename = getFileNameWithoutExtension(inputfile);
;
    Platform platform = detectPlatform(arena.fileHeader.magicNumber);

    std::cout << "RW4 Arena version " << toStr(arena.fileHeader.majorVersion) << "." << toStr(arena.fileHeader.minorVersion) << " detected.\n";

    bool e = arena.fileHeader.isBigEndian;

	PlatformInfo platformInfo = platformMapping.at(platform);

    int dataStart = 0;

    auto resourceDescriptors = loadResourceDescriptors(inputFile, platformInfo.numDescriptors, e);
    auto resourcesUsed = loadResourceDescriptors(inputFile, platformInfo.numDescriptors, e);

    std::vector<rw::core::arena::TargetResource> resources;
    for (int i = 0; i < platformInfo.numResources; ++i) {
        rw::core::arena::TargetResource resource{};
        inputFile.read(reinterpret_cast<char*>(&resource), sizeof(resource));
        resources.push_back(resource);
    }

    rw::core::arena::Manifest manifest{};
    inputFile.read(reinterpret_cast<char*>(&manifest), sizeof(manifest));
    std::vector<int> dictOffsets;
    std::vector<rw::core::arena::Dictionary_Entry> dictEntries;
    for (int i = 0; i < swapEndianness(manifest.numEntries, e); ++i) {
        rw::core::arena::Dictionary_Entry entry{};
        int offset = static_cast<int>(inputFile.tellg()) + offsetof(rw::core::arena::Dictionary_Entry, offset);
        inputFile.read(reinterpret_cast<char*>(&entry), sizeof(entry));
        //std::cout << "dictOffset: " << offset << "\n";
        dictOffsets.push_back(offset);
        dictEntries.push_back(entry);
    }

    rw::core::arena::Types types{};
    int typeNumEntriesOffset = static_cast<int>(inputFile.tellg()) + offsetof(rw::core::arena::Types, numEntries);
    inputFile.read(reinterpret_cast<char*>(&types), sizeof(types));
    std::vector<rw::core::arena::Type_Entry> typeEntries;

    for (int i = 0; i < swapEndianness(types.numEntries, e); ++i) {
        rw::core::arena::Type_Entry entry{};
        inputFile.read(reinterpret_cast<char*>(&entry), sizeof(entry));
        typeEntries.push_back(entry);
        uint32_t entryID = swapEndianness(entry.RW_Object_ID, e);
    }

    rw::core::arena::External_Arenas externalarenas{};
    inputFile.read(reinterpret_cast<char*>(&externalarenas), sizeof(externalarenas));

    rw::core::arena::External_Arena externalarena{};
    inputFile.read(reinterpret_cast<char*>(&externalarena), sizeof(externalarena));

    std::vector<rw::core::arena::External_Arena_Entry> externalArenaEntries;
    for (int i = 0; i < swapEndianness(externalarenas.numEntries, e); ++i) {
        rw::core::arena::External_Arena_Entry entry{};
        inputFile.read(reinterpret_cast<char*>(&entry), sizeof(entry));
        externalArenaEntries.push_back(entry);
    }

    rw::core::arena::Subreferences subreferences{};
    inputFile.read(reinterpret_cast<char*>(&subreferences), sizeof(subreferences));

    rw::core::arena::Atoms atoms{};
    inputFile.read(reinterpret_cast<char*>(&atoms), sizeof(atoms));

    std::vector<rw::core::arena::Arena_Subreference_Record> arenaSubReferenceRecords;
    for (int i = 0; i < swapEndianness(atoms.numEntries, e); ++i) {
        rw::core::arena::Arena_Subreference_Record entry{};
        inputFile.read(reinterpret_cast<char*>(&entry), sizeof(entry));
        arenaSubReferenceRecords.push_back(entry);
    }

    //read Dictionary
    int alignment = swapEndianness(arena.alignment, e);
    inputFile.seekg(swapEndianness(arena.dictStart, e));
    std::vector<rw::core::arena::arenaDictEntry> dictionaries;


	ObjectTypeMap objectTypeMap{};

    for (int i = 0; i < swapEndianness(arena.numEntries, e); ++i) {
        rw::core::arena::arenaDictEntry arenadictionary{};
        inputFile.read(reinterpret_cast<char*>(&arenadictionary), sizeof(arenadictionary));
		AddToObjectTypeMap(arenadictionary, objectTypeMap, e);
        dictionaries.push_back(arenadictionary);
	}

    parseArenaTypes(objectTypeMap, inputFile, e);

    //-->>>
    for (int i = 0; i < swapEndianness(arena.numEntries, e); ++i) {
        rw::core::arena::arenaDictEntry arenadictionary{};
        inputFile.read(reinterpret_cast<char*>(&arenadictionary), sizeof(arenadictionary));
       
        std::cout << objectTypes[swapEndianness(arenadictionary.typeId, e)] << "\n";
		std::cout << "typeId: " << swapEndianness(arenadictionary.typeId, e) << "\n";
        std::cout << "ptr: " << swapEndianness(arenadictionary.ptr, e) << "\n";
        std::cout << "reloc: " << swapEndianness(arenadictionary.reloc, e) << "\n";
        std::cout << "size: " << swapEndianness(arenadictionary.size, e) << "\n";
        std::cout << "alignment: " << swapEndianness(arenadictionary.alignment, e) << "\n";
        std::cout << "typeIndex: " << swapEndianness(arenadictionary.typeIndex, e) << "\n";

        if (objectTypes[swapEndianness(arenadictionary.typeId, e)] == "RWOBJECTTYPE_TABLEOFCONTENTS") {
            tocstartoffset = swapEndianness(arenadictionary.ptr, e);
            tocsize = swapEndianness(arenadictionary.size, e);
        }
        else if (objectTypes[swapEndianness(arenadictionary.typeId, e)] == "RWOBJECTTYPE_RENDERMATERIALDATA") {
            modelfound = true;
            rendermatoffset = swapEndianness(arenadictionary.ptr, e);
        }
        else if (objectTypes[swapEndianness(arenadictionary.typeId, e)] == "RWGOBJECTTYPE_PALETTE") {
            paletteoffsets.push_back(swapEndianness(arenadictionary.ptr, e));
        }
        else if (objectTypes[swapEndianness(arenadictionary.typeId, e)] == "RWGOBJECTTYPE_TEXTURE") {
            textureoffsets.push_back(swapEndianness(arenadictionary.ptr, e));
            texturefound = true;
        }

        if (objectTypes[swapEndianness(arenadictionary.typeId, e)] == "RWGOBJECTTYPE_VERTEXDESCRIPTOR") {
            int oldoffset = inputFile.tellg();
            inputFile.seekg(swapEndianness(arenadictionary.ptr, e));
            if (platform == Platform::PS3) {
                inputFile.read(reinterpret_cast<char*>(&vertexdps3), sizeof(vertexdps3));
                std::cout << "m_numElements: " << swapEndianness(vertexdps3.m_numElements, e) << "\n";
                std::cout << "m_numStreams: " << swapEndianness(vertexdps3.m_numStreams, e) << "\n";
                std::cout << "m_refCount: " << swapEndianness(vertexdps3.m_refCount, e) << "\n";
                std::cout << "m_typesFlags: " << swapEndianness(vertexdps3.m_typesFlags, e) << "\n";
                for (int i = 0; i < swapEndianness(vertexdps3.m_numElements, e); ++i) {
                    renderengine::VertexDescriptorElementps3 vertexdelemps3{};
                    inputFile.read(reinterpret_cast<char*>(&vertexdelemps3), sizeof(vertexdelemps3));
                    vertexdelemsps3.push_back(vertexdelemps3);
                    std::cout << "VertexDescriptorElement " << i << "\n";
                    std::cout << "elementClass: " << elementClass[swapEndianness(vertexdelemps3.elementClass, e)] << "\n";
                    std::cout << "numComponents: " << static_cast<int>(swapEndianness(vertexdelemps3.numComponents, e)) << "\n";
                    std::cout << "offset: " << static_cast<int>(swapEndianness(vertexdelemps3.offset, e)) << "\n";
                    std::cout << "stream: " << static_cast<int>(swapEndianness(vertexdelemps3.stream, e)) << "\n";
                    std::cout << "stride: " << swapEndianness(vertexdelemps3.stride, e) << "\n";
                    std::cout << "type: " << elementTypesps3[swapEndianness(vertexdelemps3.type, e)] << "\n";
                    std::cout << "vertexType: " << vertexTypesps3[swapEndianness(vertexdelemps3.vertexType, e)] << "\n";
                }
            }
            else if (platform == Platform::XBOX) {
                inputFile.read(reinterpret_cast<char*>(&vertexdxbox), sizeof(vertexdxbox));
                std::cout << "m_numElements: " << swapEndianness(vertexdxbox.m_numElements, e) << "\n";
                std::cout << "m_numStreams: " << swapEndianness(vertexdxbox.m_numStreams, e) << "\n";
                std::cout << "m_refCount: " << swapEndianness(vertexdxbox.m_refCount, e) << "\n";
                std::cout << "m_typesFlags: " << swapEndianness(vertexdxbox.m_typesFlags, e) << "\n";
                for (int i = 0; i < swapEndianness(vertexdxbox.m_numElements, e); ++i) {
                    renderengine::VertexDescriptorElementxbox vertexdelemxbox{};
                    inputFile.read(reinterpret_cast<char*>(&vertexdelemxbox), sizeof(vertexdelemxbox));
                    vertexdelemsxbox.push_back(vertexdelemxbox);
                    std::cout << "VertexDescriptorElement " << i << "\n";
                    std::cout << "elementClass: " << elementClass[swapEndianness(vertexdelemxbox.elementClass, e)] << "\n";
                    std::cout << "indexedUsage: " << indexedUsage[swapEndianness(vertexdelemxbox.indexedUsage, e)] << "\n";
                    std::cout << "offset: " << swapEndianness(vertexdelemxbox.offset, e) << "\n";
                    std::cout << "stream: " << swapEndianness(vertexdelemxbox.stream, e) << "\n";
                    std::cout << "method: " << static_cast<int>(swapEndianness(vertexdelemxbox.method, e)) << "\n";
                    std::cout << "type: " << elementTypesxbox[swapEndianness(vertexdelemxbox.type, e)] << "\n";
                    std::cout << "usage: " << d3ddeclUsage[swapEndianness(vertexdelemxbox.usage, e)] << "\n";
                    std::cout << "usageIndex: " << static_cast<int>(swapEndianness(vertexdelemxbox.usageIndex, e)) << "\n";
                }
                for (int i = 0; i < swapEndianness(vertexdxbox.m_numElements, e); ++i) {
                    uint8_t stride = 0;
                    inputFile.read(reinterpret_cast<char*>(&stride), sizeof(stride));
                    std::cout << "stride: " << static_cast<int>(swapEndianness(stride, e)) << "\n";
                }
            }
            else if (platform == Platform::WII) {
                inputFile.read(reinterpret_cast<char*>(&vertexdwii), sizeof(vertexdwii));
                std::cout << "numVBuffs: " << static_cast<int>(swapEndianness(vertexdwii.numVBuffs, e)) << "\n";
                std::cout << "numElements: " << static_cast<int>(swapEndianness(vertexdwii.numElements, e)) << "\n";
                for (int i = 0; i < swapEndianness(vertexdwii.numElements, e); ++i) {
                    renderengine::VertexDescriptorElementwii vertexdelemwii{};
                    inputFile.read(reinterpret_cast<char*>(&vertexdelemwii), sizeof(vertexdelemwii));
                    vertexdelemswii.push_back(vertexdelemwii);
                    std::cout << "VertexDescriptorElement " << i << "\n";
                    std::cout << "attribute: " << Attribute[swapEndianness(vertexdelemwii.attribute, e)] << "\n";
                    if (componentTypeMapping[swapEndianness(vertexdelemwii.attribute, e)] == "POS") {
                        std::cout << "compType: " << ComponentTypePOS[static_cast<int>(swapEndianness(vertexdelemwii.compType, e))] << "\n";
                        std::cout << "compSize: " << ComponentSizeOTHER[static_cast<int>(swapEndianness(vertexdelemwii.compSize, e))] << "\n";
                    }
                    else if (componentTypeMapping[swapEndianness(vertexdelemwii.attribute, e)] == "NRM") {
                        std::cout << "compType: " << ComponentTypeNRM[static_cast<int>(swapEndianness(vertexdelemwii.compType, e))] << "\n";
                        std::cout << "compSize: " << ComponentSizeOTHER[static_cast<int>(swapEndianness(vertexdelemwii.compSize, e))] << "\n";
                    }
                    else if (componentTypeMapping[swapEndianness(vertexdelemwii.attribute, e)] == "CLR") {
                        std::cout << "compType: " << ComponentTypeCLR[static_cast<int>(swapEndianness(vertexdelemwii.compType, e))] << "\n";
                        std::cout << "compSize: " << ComponentSizeOTHER[static_cast<int>(swapEndianness(vertexdelemwii.compSize, e))] << "\n";
                    }
                    else if (componentTypeMapping[swapEndianness(vertexdelemwii.attribute, e)] == "TEX") {
                        std::cout << "compType: " << ComponentTypeTEX[static_cast<int>(swapEndianness(vertexdelemwii.compType, e))] << "\n";
                        std::cout << "compSize: " << ComponentSizeTEX[static_cast<int>(swapEndianness(vertexdelemwii.compSize, e))] << "\n";
                    }
                    else {
                        std::cout << "compType: " << static_cast<int>(swapEndianness(vertexdelemwii.compType, e)) << "\n";
                        std::cout << "compSize: " << ComponentSizeOTHER[static_cast<int>(swapEndianness(vertexdelemwii.compSize, e))] << "\n";
                    }
                    std::cout << "fraction: " << static_cast<int>(swapEndianness(vertexdelemwii.fraction, e)) << "\n";
                    std::cout << "unknown: " << static_cast<int>(swapEndianness(vertexdelemwii.unknown, e)) << "\n";
                    std::cout << "stride: " << static_cast<int>(swapEndianness(vertexdelemwii.stride, e)) << "\n";
                    std::cout << "vBuffIndex: " << static_cast<int>(swapEndianness(vertexdelemwii.vBuffIndex, e)) << "\n";
                    std::cout << "unknown2: " << static_cast<int>(swapEndianness(vertexdelemwii.unknown2, e)) << "\n";
                    std::cout << "offset: " << swapEndianness(vertexdelemwii.offset, e) << "\n";
                }
                for (int i = 0; i < swapEndianness(vertexdwii.numElements, e); ++i) {
                    uint8_t test = 0;
                    inputFile.read(reinterpret_cast<char*>(&test), sizeof(test));
                    std::cout << "test: " << static_cast<int>(swapEndianness(test, e)) << "\n";
                }
            }
            inputFile.seekg(oldoffset);
        }
        if (objectTypes[swapEndianness(arenadictionary.typeId, e)] == "RWGOBJECTTYPE_VERTEXBUFFER") {
            //vertexCountOffset = doffset + 0x8;
        }
        if (objectTypes[swapEndianness(arenadictionary.typeId, e)] == "RWGOBJECTTYPE_INDEXBUFFER") {
            //indexCountOffset = swapEndianness(arenadictionary.ptr, e) + 0x8;
        }
        if (objectTypes[swapEndianness(arenadictionary.typeId, e)] == "RWOBJECTTYPE_RENDEROPTIMESHDATA") {
            indexoffset_size_2 = swapEndianness(arenadictionary.ptr, e) + 100;
        }
        if (objectTypes[swapEndianness(arenadictionary.typeId, e)] == "RWOBJECTTYPE_BASERESOURCE") {
            //vertex
            vertexoffset = swapEndianness(arenadictionary.ptr, e);
            vertexsize = swapEndianness(arenadictionary.size, e);
        }
        else if (objectTypes[swapEndianness(arenadictionary.typeId, e)] == "RWOBJECTTYPE_BASERESOURCE") {
            //index
            indexoffset = swapEndianness(arenadictionary.ptr, e);
            indexsize = swapEndianness(arenadictionary.size, e);
        }
        if (objectTypes[swapEndianness(arenadictionary.typeId, e)] == "RWOBJECTTYPE_BASERESOURCE") {
            //dataOffsets.push_back(doffset);
        }
        else {
            //absoluteOffsets.push_back(doffset);
        }
        dictionaries.push_back(arenadictionary);
    }
    
    inputFile.seekg(tocstartoffset);
    inputFile.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (modelfound) {
        load_RW4_Model(inputFile, tocstartoffset, header, rendermatoffset, e);
	}

    if(texturefound) {
        load_RW4_Texture(inputFile, platform, dictionaries, resourceDescriptors, header, tocstartoffset, textureoffsets, e);
	}

    inputFile.seekg(vertexDescriptor);
    uint32_t vertexDescOffset = 0;
    inputFile.read(reinterpret_cast<char*>(&vertexDescOffset), sizeof(vertexDescOffset));

    int vertexOffset = dataStart + vertexoffset;
    int faceOffset = dataStart + indexoffset;
    std::cout << "vertexOffset: " << vertexOffset << "\n";
    std::cout << "faceOffset: " << faceOffset << "\n";
    std::cout << "vertexCountOffset: " << vertexCountOffset << "\n";
    std::cout << "indexCountOffset: " << indexCountOffset << "\n";

    std::string parentname = getParentName(inputfile);
    filename = getFileName(inputfile);
    //
    inputFile.clear();
    inputFile.close();
}
*/