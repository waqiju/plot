#include "file_helper.h"
#include <iostream>
#include <fstream>
#include <sstream>


namespace FileHelper
{
    bool Read(const std::string& path, std::string& content)
    {
        std::ifstream inputFileStream;
        inputFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            inputFileStream.open(path, std::ios::binary);
            std::stringstream inputStream;
            inputStream << inputFileStream.rdbuf();
            inputFileStream.close();
            content = inputStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "Read shader file failed!\n" << e.what() << std::endl;
            return false;
        }

        return true;
    }

} // FileHelper