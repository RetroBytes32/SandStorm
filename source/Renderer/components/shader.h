#ifndef COMPONENT_SHADER
#define COMPONENT_SHADER

#include <string>

#define GLEW_STATIC
#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../types/color.h"

#include "../../std/logging.h"


class Shader {
    
public:
    
    Shader();
    ~Shader();
    
    /// Set the uniform model matrix.
    void SetModelMatrix(glm::mat4 &ModelMatrix);
    /// Set the uniform projection matrix.
    void SetProjectionMatrix(glm::mat4 &projectionMatrix);
    
    /// Set the color of the material.
    void SetMaterialColor(Color color);
    /// Set the texture sampler index.
    void SetTextureSampler(unsigned int index);
    
    /// Set default uniform locations.
    void SetUniformLocations(void);
    
    /// Compile a vertex and fragment script into a shader program.
    int CreateShaderProgram(std::string VertexScript, std::string FragmentScript);
    
    /// Bind the shader program for rendering.
    void Bind(void);
    
    /// Build a default shader program.
    bool BuildDefault(void);
    
    
private:
    
    unsigned int shaderProgram;
    
    // Uniform locations
    int projectionMatrixLocation;
    int modelMatrixLocation;
    int materialColorLocation;
    int samplerLocation;
    
    bool  isShaderLoaded;
    
    unsigned int CompileSource(unsigned int Type, std::string Script);
    
};


#endif
