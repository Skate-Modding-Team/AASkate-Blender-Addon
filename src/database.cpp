#include <iostream>

namespace Attrib {
    namespace Types {
        struct Colour {
            float a;
            float r;
            float g;
            float b;
        };

        struct FloatColour {
            float a;
            float r;
            float g;
            float b;
        };

        struct FloatColour {
            float a;
            float r;
            float g;
            float b;
		};

        struct Vector2 {
            float x;
			float y;
		};

        struct Vector3 {
            float x;
            float y;
            float z;
		};

        struct Vector4 {
            float x;
            float y;
            float z;
            float w;
		};

        struct Quaternion {
            float v[4];
		};

        struct Matrix {
            Vector4 _v0;
            Vector4 _v1;
            Vector4 _v2;
            Vector4 _v3;
        };
    };
    struct Array {
        uint16_t mAlloc;
        uint16_t mCount;
        uint16_t mSize;
		uint16_t mEncodedTypeAndPad;
    };

    struct Instance {
		uint32_t mCollection;     // offset
		uint32_t mLayoutPtr;      // offset
		uint32_t mMsgPort;
		uint32_t mFlags;
    };

    struct Definition {
	    uint64_t mKey;
        uint64_t mType;
		uint16_t mOffset;
		uint16_t mSize;
		uint16_t mMaxCount;
		uint8_t mFlags;
		uint8_t mAlignment;
    };

    struct HashMap {
		uint32_t mTable; // offset
		uint16_t mTableSize;
		uint16_t mNumEntries;
		uint16_t mRefCount;
		uint8_t  mWorstCollision;
		uint8_t  mKeyShift;
    };

    struct Collection {
        HashMap mTable;
        uint32_t mParent; // offset
		uint64_t mKey;
		uint32_t mClass; // offset
		uint32_t mLayout; // offset
		uint32_t mSource; // offset
        uint32_t padding;
    };

    struct PtrRef {
        uint32_t mFixupOffset;
        uint16_t mPtrType;
        uint16_t mIndex;
		uint64_t mOffset; //or mExportID
    };

    struct CollectionLoadData {
        uint64_t mKey;
        uint64_t mClass;
        uint64_t mParent;
        uint32_t mTableReserve;
        uint32_t mTableKeyShift;
        uint32_t mNumEntries;
        uint16_t mNumTypes;
        uint16_t mTypesLen;
        uint32_t mLayout; // offset
		uint32_t padding;

        struct AttribEntry {
            uint64_t mKey;
            uint32_t mData; // offset
            uint16_t mType;
            uint8_t  mNodeFlags;
            uint8_t  mEntryFlags;
        };
    };

    struct Vault {
        struct ChunkBlock {
            uint32_t mType;
            uint32_t mSize;
        };

        struct DataBlock {
            uint32_t mData;
            uint32_t _bf_4;
        };

        struct DependencyNode : public ChunkBlock {
            uint64_t mCount;
        };

        struct ExportEntry {
            uint64_t mID;
            uint64_t mType;
            uint32_t mDataBytes;
            uint32_t mDataOffset;
        };

        struct ExportNode : public ChunkBlock {
            uint64_t mCount;
        };

        struct PointerNode : public ChunkBlock {
        };

        struct VersionNode : public ChunkBlock {
            uint64_t mVersion;
        };

        uint64_t mVersion;
        uint64_t mUserID; 
        uint32_t mRefCount;
        uint32_t mExportMgr;
        uint32_t mGC;
        uint32_t mDependencies;
        uint32_t mDepData;
        uint32_t mDepIDs;   
        uint32_t mNumDependencies;
        uint32_t mResolvedCount;
        uint32_t mPointers;
        uint32_t mTransientData;
        uint32_t mExports;
        uint32_t mExportData;
        uint32_t mExportIDs;
        uint32_t mNumExports;
        uint32_t mNumAllocExports;
        uint32_t mNumLoadedExports;
        uint8_t  mInited;
        uint8_t  mDeinited;
        uint8_t  _pad[6];
    };

    struct Database {
        uint32_t __vftable;   // 0x00 – pointer to vtable
        uint32_t mPrivates;   // 0x04 – pointer to DatabasePrivate
    };

    struct IExportPolicy {
        uint32_t __vftable;   // 0x00 – pointer to virtual function table
    };

    struct DatabaseExportPolicy : public IExportPolicy {
    };

    struct DatabaseLoadData {
        uint32_t mNumClasses;       // 0x00
        uint32_t mDefaultDataSize;  // 0x04
        uint32_t mNumTypes;         // 0x08
        uint32_t mTypenames;        // 0x0C – pointer to table
    };

    struct VecHashMap_u64_Class_TablePolicy_0_16 {
        uint32_t mTable;        // 0x00 – pointer to table
        int16_t  mTableSize;    // 0x04
        int16_t  mNumEntries;   // 0x06
        int16_t  mFixedAlloc;   // 0x08
        int16_t  mWorstCollision; // 0x0A
    };

    struct ClassTable : public VecHashMap_u64_Class_TablePolicy_0_16 {
    };

    struct TypeDescVec {
        uint32_t begin;     // 0x00
        uint32_t end;       // 0x04
        uint32_t capacity;  // 0x08
        uint32_t alloc;     // 0x0C (EASTL internal allocator pointer or flags)
    };

    struct TypeDescPtrTable {
        uint32_t treeRoot;      // 0x00
        uint32_t sentinel;      // 0x04
        uint32_t size;          // 0x08
        uint32_t alloc1;        // 0x0C
        uint32_t alloc2;        // 0x10
        uint32_t alloc3;        // 0x14
        uint32_t alloc4;        // 0x18
    };

    struct CollectionList {
        uint32_t head;   // 0x00
        uint32_t tail;   // 0x04
        uint32_t size;   // 0x08
        uint32_t alloc;  // 0x0C
    };

    struct ClassList {
        uint32_t head;   // 0x00
        uint32_t tail;   // 0x04
        uint32_t size;   // 0x08
        uint32_t alloc;  // 0x0C
    };

    struct DatabasePrivate : public Database { // base size: 0x08
        ClassTable* mClasses;             // 0x08
        uint32_t mNumCompiledTypes;    // 0x14
        TypeDescVec* mCompiledTypes;       // 0x18
        TypeDescPtrTable* mTypes;               // 0x28
        CollectionList* mGarbageCollections;  // 0x44
        ClassList* mGarbageClasses;      // 0x54
    };
}
