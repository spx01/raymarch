#!/usr/bin/env bash

# will problably ditch glsltc because it has some issues with forwarding function parameters

compile_shader () {
    glsltc -o "$1.gen" "$1"
    # for some reason when i include the version directive directly in the shader file,
    # glsltc puts it second in the genenrated output, which makes the final shader not compile
    echo -e '// generated with glsltc\n#version 460 core\n' | cat - "$1.gen" > "$1.tmp" && \
    mv "$1.tmp" "$1.gen"
}

for shader in $(find assets -name '*.frag' -o -name '*.vert' -o -name '*.glsl'); do
    compile_shader "$shader"
done
