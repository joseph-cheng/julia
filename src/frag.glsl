#version 330 core

out vec4 FragColor;

uniform float left_bound;
uniform float right_bound;
uniform float top_bound;
uniform float bottom_bound;

uniform float screen_width;
uniform float screen_height;

uniform int itcap;
uniform vec3 colours[#ITCAP#];

uniform vec2 julia_c;
uniform int z_power;

vec2 complex_product(vec2 a, vec2 b);
vec2 fractal_func(vec2 z, vec2 c);
vec3 hsl2rgb(vec3 c);

vec2 complex_product(vec2 a, vec2 b) {
    return (vec2(a.x*b.x - a.y * b.y, a.x*b.y + a.y*b.x));
}

vec2 fractal_func(vec2 z, vec2 c) {
    vec2 r = z;
    for (int i = 1; i < z_power; i++) {
        r = complex_product(r, z);
    }

    return (r + c);
    //vec2 r = vec2(abs(z.x), abs(z.y));
    //return (complex_product(r,r) + c);
}

vec3 hsl2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0/3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p-K.xxx, 0.0, 1.0), c.y);
}

void main() {

    //vec2 c = vec2((gl_FragCoord.x / screen_width) * (right_bound - left_bound) + left_bound, (gl_FragCoord.y / screen_height) * (top_bound - bottom_bound) + bottom_bound);
    //vec2 z = vec2(0.0, 0.0);

    vec2 z = vec2((gl_FragCoord.x / screen_width) * (right_bound - left_bound) + left_bound, (gl_FragCoord.y / screen_height) * (top_bound - bottom_bound) + bottom_bound);
    vec2 c = julia_c;


    int iterations_required = itcap;
    bool in_set = true;
    for (int i = 0; i < itcap; i++) {
        z = fractal_func(z, c);
        if (length(z) >= 2 && in_set) {
            iterations_required = i;
            in_set = false;
        }

    }
    vec3 colour = colours[iterations_required];

    FragColor = vec4(colour, 1.0);

}
