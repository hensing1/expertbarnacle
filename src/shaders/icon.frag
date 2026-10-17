#version 460

in vec2 fragTexCoord;

uniform sampler2D iconTexture;
uniform vec4 foregroundColor;
uniform vec4 backgroundColor;

out vec4 finalColor;

void main() {
    float weight = texture(iconTexture, fragTexCoord).a;
    finalColor = weight * foregroundColor + (1-weight) * backgroundColor;
}
