#include "utility.h"


namespace schess
{
    // why not
    std::optional<std::size_t> GetFileSize(std::FILE* file)
    {   
        std::size_t result;

        if(std::fseek(file, 0L, SEEK_END) != 0) return {};

        const long temp = std::ftell(file);
        if(temp < 0) return {};

        result = static_cast<std::size_t>(temp);

        if(std::fseek(file, 0L, SEEK_SET) != 0) return {};

        return result;
    }

    bool sGetTextFile(const std::string_view& sFilePath, std::string& res)
    {
        std::FILE* file = std::fopen(sFilePath.data(), "r");
        
        if(!file) return false;

        res.clear();

        const auto fileSize = GetFileSize(file);
        if(fileSize) res.reserve(fileSize.value() / sizeof(char));

        int c;
        while((c = std::fgetc(file)) != EOF) res.push_back(static_cast<char>(c));

        return !std::fclose(file);
    }

    bool gl::TellError(const std::string_view& sFuncName, const std::string_view& sFileName, const int Line)
    {
        bool result = false;
        GLenum error;

        while((error = glGetError()) != GL_NO_ERROR)
        {   
            std::cerr << "[OpenGL Error]: (" << error << ") "
            << sFuncName.data() << "{In:" << sFileName.data() << " At:" << Line << "}\n"; 
            result = true;
        }

        return result;
    }

}