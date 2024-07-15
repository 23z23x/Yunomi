#include "pch.h"
#include "Utility.h"

namespace ynm
{
    //reads the full contents of a file and returns it as a buffer
    std::vector<char> readFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw YunomiError("Utility: Failed to open file", "11");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    //Reads a whole file, returning a string for each line in the file
    std::vector<std::string> readLines(const std::string& filename)
    {
        std::ifstream file(filename);

        if (!file.is_open())
        {
            throw YunomiError("Utility: Failed to open file", "33");
        }

        std::vector<std::string> retVec;
        std::string line;

        while (getline(file, line))
        {
            retVec.push_back(line);
        }

        file.close();

        return retVec;
    }
}