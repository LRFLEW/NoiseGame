in vec4 pos;
in vec2 uv;
out vec2 fuv;

void main() {
    gl_Position = pos;
    fuv = uv;
}
