#include "shader.h"

Shader::Shader() {
    mShaderProgram = 0;
    
    mProjectionMatrixLocation = 0;
    mModelMatrixLocation = 0;
    mMaterialColorLocation = 0;
    mSamplerLocation = 0;
    
    mIsShaderLoaded = false;
    return;
}

Shader::~Shader() {
    
    if (mIsShaderLoaded) glDeleteProgram(mShaderProgram);
    return;
}

void Shader::SetModelMatrix(glm::mat4 &ModelMatrix) {
    glUniformMatrix4fv(mModelMatrixLocation, 1, 0, &ModelMatrix[0][0]);
    return;
}

void Shader::SetProjectionMatrix(glm::mat4 &projectionMatrix) {
    glUniformMatrix4fv(mProjectionMatrixLocation, 1, 0, &projectionMatrix[0][0]);
    return;
}

void Shader::SetMaterialColor(Color color) {
    glUniform4f(mMaterialColorLocation, color.r, color.g, color.b, color.a);
    return;
}

void Shader::SetTextureSampler(unsigned int index) {
    glUniform1i(mSamplerLocation, index);
    return;
}

void Shader::SetUniformLocations(void) {
    
    std::string projUniformName     = "u_proj";
    std::string modelUniformName    = "u_model";
    std::string colorUniformName    = "m_color";
    std::string samplerUniformName  = "u_sampler";
    
    mProjectionMatrixLocation  = glGetUniformLocation(mShaderProgram, projUniformName.c_str());;
    mModelMatrixLocation       = glGetUniformLocation(mShaderProgram, modelUniformName.c_str());
    mMaterialColorLocation     = glGetUniformLocation(mShaderProgram, colorUniformName.c_str());
    mSamplerLocation           = glGetUniformLocation(mShaderProgram, samplerUniformName.c_str());
    return;
}

int Shader::CreateShaderProgram(std::string VertexScript, std::string FragmentScript) {
    
    // Compile the scripts into a shader program
    unsigned int vs = CompileSource(GL_VERTEX_SHADER,   VertexScript);
    unsigned int fs = CompileSource(GL_FRAGMENT_SHADER, FragmentScript);
    
    if (vs==0) return -1;
    if (fs==0) return -2;
    
    mShaderProgram = glCreateProgram();
    
    glAttachShader(mShaderProgram, vs);
    glAttachShader(mShaderProgram, fs);
    
    glLinkProgram(mShaderProgram);
    
    glDetachShader(mShaderProgram, vs);
    glDetachShader(mShaderProgram, fs);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    GLint state;
    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &state);
    if (state == GL_FALSE) {
        std::cout << " ! Shader link error" << std::endl;
        assert(0);
        return 0;
    }
    
    SetUniformLocations();
    
    mIsShaderLoaded = true;
    return 1;
}

unsigned int Shader::CompileSource(unsigned int Type, std::string Script) {
    
    unsigned int ShaderID = glCreateShader(Type);
    const char*  SourceScript = Script.c_str();
    
    // Compile source script
    glShaderSource(ShaderID, 1, &SourceScript, nullptr);
    glCompileShader(ShaderID);
    
    // Check compile status
    int vResult;
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &vResult);
    if (!vResult) {
        std::cout << " ! Shader compilation error" << std::endl;
        assert(0);
        return 0;
    }
    
    return ShaderID;
}

void Shader::Bind(void) {
    glUseProgram(mShaderProgram);
    return;
}

bool Shader::BuildDefault(void) {
    
    std::string vertexShader = 
        "#version 330 core\n"
        ""
        "layout(location = 0) in vec3 l_position;"
        "layout(location = 1) in vec3 l_color;"
        "layout(location = 2) in vec2 l_uv;"
        ""
        "uniform mat4 u_proj;"
        "uniform mat4 u_model;"
        ""
        "varying vec2 v_coord;"
        "varying vec3 v_color;"
        ""
        "void main() "
        "{"
        ""
        "v_color = l_color;"
        "v_coord = l_uv;"
        ""
        "gl_Position = u_proj * u_model * vec4(l_position, 1);"
        ""
        "};";
    
    std::string fragmentShader = 
        "#version 330 core\n"
        ""
        "uniform vec4 m_color;"
        "varying vec3 v_color;"
        "varying vec2 v_coord;"
        ""
        "uniform sampler2D u_sampler;"
        ""
        "out vec4 color;"
        ""
        "void main()"
        "{"
        "  "
        "  float Gamma = 2.2;"
        "  "
        "  vec4 vColor = vec4(v_color, 1);"
        "  "
        "  color = vec4( pow(vColor.rgb, vec3(1.0/Gamma)), 1) * texture(u_sampler, v_coord);"
        "  "
        "}";
    
    if (CreateShaderProgram(vertexShader, fragmentShader) == 1) return true;
    
    return false;
}
