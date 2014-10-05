#version 330

uniform vec4 frag_color;
in vec4 varyingColor;
out vec4 finalColor;

void main()
{
    //finalColor = vec4(1.0, 0.0, 0.0, 1.0);
	finalColor = frag_color;
    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
