#version 460

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;

// Input uniform values
uniform sampler2D texture0;

// uniform vec2 rectDims;
// uniform vec2 pictureDims;
uniform float picScale;

out vec4 finalColor;

void main() {
    finalColor = texture(texture0, fragTexCoord);
  // finalColor = vec4(fragTexCoord.x, 0, fragTexCoord.y, 1);
}
