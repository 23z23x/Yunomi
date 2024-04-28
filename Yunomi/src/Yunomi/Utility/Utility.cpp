#include "pch.h"
#include "Utility.h"

namespace ynm
{
    //reads the full contents of a file and returns it as a buffer
    std::vector<char> readFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            YNM_CORE_ERROR("Utility: Failed to open file {0}", filename);
            throw std::runtime_error("");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }
}