#version 150
uniform mat4 View;
uniform mat4 Projection;

in vec3 Position;
in vec3 Normal;

out vec3 vertPosition;
out vec3 vertNormal;

void main() {
    vertPosition = vec3(View * vec4(Position, 1.0));
    gl_Position = Projection * View * vec4(Position, 1.0);
    vertNormal = mat3(View) * Normal;
}
