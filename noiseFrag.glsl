uniform uint seed;
out vec4 color;

// Murmur2
void subhash(uint value, inout uint h) {
    uint k = value * 0x5bd1e995u;
    k ^= k >> 24u;
    k *= 0x5bd1e995u;
    h = ( h * 0x5bd1e995u ) ^ k;
}

uint hash(uint a, uint b, uint seed) {
    uint h = seed;
    subhash(a, h);
    subhash(b, h);
    h ^= h >> 13u;
    h *= 0x5bd1e995u;
    h ^= h >> 15u;
    return h;
}

void main() {
    uint hashval = hash(uint(gl_FragCoord.x), uint(gl_FragCoord.y), seed);
    float normh = float(hashval)/float(0xffffffffu);
    color = vec4(normh, normh, normh, 1.0);
}
