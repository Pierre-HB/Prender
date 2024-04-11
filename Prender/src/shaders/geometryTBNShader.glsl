#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 18) out;

uniform mat4 p;

in vec4 n[];
in vec4 t[];

out vec4 color;

void emitLine(in vec4 p0, in vec4 p1, in vec4 color_){
    gl_Position = p*(p0);
    color = color_;
    EmitVertex();

    gl_Position = p*(p1);
    color = color_;
    EmitVertex();

    EndPrimitive();
}

void main() {  

    for(int i = 0; i < 3; i++){
        if(n[i].z > 0){

            emitLine(gl_in[i].gl_Position, gl_in[i].gl_Position+n[i], vec4(0, 0, 1, 1));
            emitLine(gl_in[i].gl_Position, gl_in[i].gl_Position+t[i], vec4(1, 0, 0, 1));

            vec4 b = vec4(cross(n[i].xyz, t[i].xyz), 0);
            emitLine(gl_in[i].gl_Position, gl_in[i].gl_Position+b, vec4(0, 1, 0, 1));

        }
    }
}  