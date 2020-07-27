char frag[5000] = "#version 330 core\n"

"out vec4 FragColor;\n"
"uniform float left_bound;\n"
"uniform float right_bound;\n"
"uniform float top_bound;\n"
"uniform float bottom_bound;\n"

"uniform float screen_width;\n"
"uniform float screen_height;\n"

"uniform int itcap;\n"
"uniform float colours[itcap];\n"

"uniform vec2 julia_c;\n"
"uniform int z_power;\n"

"vec2 complex_product(vec2 a, vec2 b);\n"
"vec2 fractal_func(vec2 z, vec2 c);\n"
"vec3 hsl2rgb(vec3 c);\n"

"vec2 complex_product(vec2 a, vec2 b) {\n"
    "return (vec2(a.x*b.x - a.y * b.y, a.x*b.y + a.y*b.x));\n"
"}\n"

"vec2 fractal_func(vec2 z, vec2 c) {\n"
    "vec2 r = z;\n"
    "for (int i = 1; i < z_power; i++) {\n"
        "r = complex_product(r, z);\n"
    "}\n"
    "return (r + c);\n"
"}\n"

"vec3 hsl2rgb(vec3 c) {\n"
    "vec4 K = vec4(1.0, 2.0/3.0, 1.0 / 3.0, 3.0);\n"
    "vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);\n"
    "return c.z * mix(K.xxx, clamp(p-K.xxx, 0.0, 1.0), c.y);\n"
"}\n"

"void main() {\n"

    "//vec2 c = vec2((gl_FragCoord.x / screen_width) * (right_bound - left_bound) + left_bound, (gl_FragCoord.y / screen_height) * (top_bound - bottom_bound) + bottom_bound);\n"
    "//vec2 z = vec2(0.0, 0.0);\n"

    "vec2 z = vec2((gl_FragCoord.x / screen_width) * (right_bound - left_bound) + left_bound, (gl_FragCoord.y / screen_height) * (top_bound - bottom_bound) + bottom_bound);\n"
    "vec2 c = julia_c;\n"

    "int iterations_required = itcap;\n"
    "bool in_set = true;\n"
    "for (int i = 0; i < itcap; i++) {\n"
        "z = fractal_func(z, c);\n"
        "if (length(z) >= 2 && in_set) {\n"
            "iterations_required = i;\n"
            "in_set = false;\n"
        "}\n"
    "}\n"
    "vec3 colour;

    "float scale = 1.57;\n"
    "if (in_set) {\n"
        "colour = vec3(0.0);\n"
    "}\n"
    "else {\n"
        "float fraction = float(iterations_required)/float(itcap);\n"
        "colour = hsl2rgb(vec3(exp(fraction)-1.0, 1.0,fraction));\n"
    "}\n"
    "FragColor = vec4(colour, 1.0);\n"
"}\n"
