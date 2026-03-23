#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "arenafunctions.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: skate_blender_addon.exe <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    load_RW4_Arena(filename);
    /*
    // Dummy vertex buffer (3 vertices)
    std::vector<float> vertices = { 0,0,0,  1.1,0,0,  0,1,0 };
    const size_t bufferSize = vertices.size() * sizeof(float);

    // Use a global shared memory name so 64-bit processes can open it
    const char* sharedMemName = "MySharedVertexBuffer";

    HANDLE hMapFile = CreateFileMappingA(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        (DWORD)bufferSize,
        sharedMemName
    );

    if (!hMapFile) {
        std::cerr << "CreateFileMapping failed: " << GetLastError() << std::endl;
        return 1;
    }

    LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, bufferSize);
    if (!pBuf) {
        std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    memcpy(pBuf, vertices.data(), bufferSize);

    // Build JSON metadata
    nlohmann::json j;
    j["type"] = "mesh";
    j["source_file"] = filename;
    j["shared_memory"] = sharedMemName;
    j["buffer_size"] = bufferSize;
    j["vertex_count"] = 3;
    j["stride"] = 12;
    j["layout"]["POSITION"] = { {"offset", 0}, {"type", "float32"}, {"count", 3} };

    // Print JSON and flush so Python can read it immediately
    //std::cout << j.dump() << std::endl;
    std::cout.flush();

    // Keep process alive for Python to map memory
    //Sleep(15000);

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    return 0;
    */
}
