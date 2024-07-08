//VERTEX SHADER
#version 450

//Uniform buffer object
layout(binding = 0) uniform UniformBufferObject {
//MVP matrix
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

//Position of the vertex in object space
layout(location = 0) in vec3 inPosition;

//Texture coord to be passed to fragment shader
layout(location = 1) in vec2 inTexCoord;

//Instance matrix to transform object into world space
layout(location = 2) in mat4 inInstanceModelMatrix;

//Per-instance texture ID
layout(location = 6) in uint inTextureID;

//Per-instance object ID;
layout(location = 7) in uint inObjID;

//Outs
layout(location = 0) out vec2 fragTexCoord;

layout(location = 1) out uint textureID;

layout(location = 2) out uint objID;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * inInstanceModelMatrix * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
    textureID = inTextureID;
    objID = inObjID;
}

