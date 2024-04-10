#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;
//layout (triangle_strip, max_vertices = 3) out;
//layout (points) out;

uniform mat4 p;

in vec4 n[];

out vec4 color;

void main() {  
    for(int i = 0; i < 3; i++){
        gl_Position = p*(gl_in[i].gl_Position);
        color = vec4(0, 0, 1, 1);
        EmitVertex();

        gl_Position = p*(gl_in[i].gl_Position+n[i]);
        color = vec4(0, 0, 1, 1);
        EmitVertex();

        EndPrimitive();
    }
    //EndPrimitive();
    

    //gl_Position = p*gl_in[0].gl_Position; 
    //gl_Position = vec4(0, 0, 1, 1);
    //color = vec4(0, 0, 1, 1);
    //EmitVertex();
    //EndPrimitive();

    //gl_Position = p*(gl_in[0].gl_Position + n[0]);
    //gl_Position = vec4(1, 0, 0, 1); 
    //color = vec4(0, 0, 1, 1);
    //EmitVertex();

    //gl_Position = vec4(1, 1, 0, 1); 
    //color = vec4(0, 0, 1, 1);
    //EmitVertex();
    
    //EndPrimitive();
}  