//FRAGMENT SHADER
#version 450

//Test fragment shader that just draws everything as green

//Texture sampler
layout(binding = 1) uniform sampler2D texSampler;

//Texture coordinate the fragment is drawing onto
layout(location = 0) in vec2 fragTexCoord;

//Color to render at this fragment
layout(location = 0) out vec4 outColor;

void main() {
    vec3 green = vec3(0, 1.0f, 0);
    outColor = vec4(green, 1);
}
