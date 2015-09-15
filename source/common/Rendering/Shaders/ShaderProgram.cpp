#include "common/Rendering/Shaders/ShaderProgram.h"
#include <fstream>
#include <iterator>

ShaderProgram::ShaderProgram(const std::unordered_map<GLenum, std::string>& inputShaders):
    shaderProgram(0)
{
    if (inputShaders.find(GL_VERTEX_SHADER) == inputShaders.end() || 
            inputShaders.find(GL_FRAGMENT_SHADER) == inputShaders.end())
    {
        std::cerr <<  "ERROR: Your shader program is either missing a vertex shader or a fragment shader" << std::endl;
        return;
    }

    // Read and setup the vertex and fragment shaders.
    // TODO: Make the shader objects be cached so we don't incur the cost of creating a shader object multiple times if it is being reused.
    shaderObjects[GL_VERTEX_SHADER] = LoadShaderObject(GL_VERTEX_SHADER, inputShaders.at(GL_VERTEX_SHADER));
    shaderObjects[GL_FRAGMENT_SHADER] = LoadShaderObject(GL_FRAGMENT_SHADER, inputShaders.at(GL_FRAGMENT_SHADER));

    // Now link the vertex and fragment shader together to create the shader program.
    shaderProgram = OGL_CALL(glCreateProgram());

    OGL_CALL(glAttachShader(shaderProgram, shaderObjects[GL_VERTEX_SHADER]));
    OGL_CALL(glAttachShader(shaderProgram, shaderObjects[GL_FRAGMENT_SHADER]));
    OGL_CALL(glLinkProgram(shaderProgram));
    
    // Check if any errors happened during the linker stage.
    GLint linkStatus;
    OGL_CALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus));
    if (linkStatus != GL_TRUE) {
        char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
        OGL_CALL(glGetProgramInfoLog(shaderProgram, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
        std::cerr << "ERROR: Program link compilation failure -- " << std::endl << errorLogBuffer << std::endl;
        shaderProgram = 0;
    }

    OGL_CALL(glDetachShader(shaderProgram, shaderObjects[GL_VERTEX_SHADER]));
    OGL_CALL(glDetachShader(shaderProgram, shaderObjects[GL_FRAGMENT_SHADER]));
    OGL_CALL(glDeleteShader(shaderObjects[GL_VERTEX_SHADER]));
    OGL_CALL(glDeleteShader(shaderObjects[GL_FRAGMENT_SHADER]));
}

ShaderProgram::~ShaderProgram()
{
    OGL_CALL(glDeleteProgram(shaderProgram));
}

GLuint ShaderProgram::LoadShaderObject(GLenum type, const std::string& filename)
{
    std::ifstream fs("shaders/" + filename, std::ifstream::in);
    
    // This reads in the whole file into an std::string. Google for more information!
    std::string shaderText((std::istreambuf_iterator<char>(fs)), 
        std::istreambuf_iterator<char>());

    GLuint newShaderObject = OGL_CALL(glCreateShader(type));

    // Need to associate the actual shader text we loaded ('shaderText') with the shader we just
    // made so that the driver knows what to compile!
    const char* shaderTextStr = shaderText.c_str();
    OGL_CALL(glShaderSource(newShaderObject, 1, &shaderTextStr, NULL));

    // Compile the shader and make sure compilation passes! Will spit out compilation debugging information if it does not.
    OGL_CALL(glCompileShader(newShaderObject));

    GLint compileStatus;
    OGL_CALL(glGetShaderiv(newShaderObject, GL_COMPILE_STATUS, &compileStatus));
    if (compileStatus != GL_TRUE) {
        char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
        OGL_CALL(glGetShaderInfoLog(newShaderObject, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
        std::cerr << "ERROR: Shader compilation failure -- " << std::endl << errorLogBuffer << std::endl;
        return 0;
    }

    return newShaderObject;
}

void ShaderProgram::StartUseShader()
{
    OGL_CALL(glUseProgram(shaderProgram));
}

void ShaderProgram::StopUseShader()
{
    OGL_CALL(glUseProgram(0));
}
