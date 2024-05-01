#version 330

in vec3 a_Position;

out vec4 vColor;

uniform float u_Time;

void main()
{
	vec4 newPosition = vec4(a_Position, 1);
	gl_Position = newPosition;
	newPosition.x += 0.5;
	newPosition.y += 0.5;

	vColor = vec4(newPosition.xy,0,1) ;
}
