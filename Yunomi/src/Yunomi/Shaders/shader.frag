//FRAGMENT SHADER
#version 450

//Texture sampler
layout(set = 0, binding = 1) uniform sampler2D textures[100];

//Texture coordinate the fragment is drawing onto
layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) flat in uint textureID;
layout(location = 2) flat in uint objID;

//Color to render at this fragment
layout(location = 0) out vec4 outColor;

void main() {
    if (textureID >= 0 && textureID < 100) {
        outColor = texture(textures[textureID], fragTexCoord);
    } else {
        outColor = vec4(1.0, 0.0, 1.0, 1.0); // Output magenta for invalid texture IDs
    }
}
