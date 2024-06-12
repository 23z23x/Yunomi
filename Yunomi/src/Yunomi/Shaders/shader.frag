//FRAGMENT SHADER
#version 450

//Texture sampler
layout(binding = 1) uniform sampler2D texSampler;

//Texture coordinate the fragment is drawing onto
layout(location = 0) in vec2 fragTexCoord;

//Color to render at this fragment
layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(texSampler, fragTexCoord);
}
