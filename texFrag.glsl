in vec2 fuv;
uniform sampler2D tex;
out vec4 color;

void main() {
    color = texture(tex, fuv).rrra;
}
