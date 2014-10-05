#version 330 
uniform mat4 mvpMatrix;

in vec3 vert;


void main()
{	
    gl_Position = mvpMatrix * vec4(vert, 1.0);
    // gl_Position = ftransform();
}
