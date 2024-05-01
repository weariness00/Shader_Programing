#version 330

in vec3 a_Position;

out vec4 vColor;

uniform float u_Time = 0;
const float c_PI = 3.141592;

void Flag()
{
	vec4 newPosition = vec4(a_Position, 1);
	float scaledValue = newPosition.x + 0.5;
	float period = 2;
	float radian = period * scaledValue * c_PI * 2;
	float sinValue = scaledValue * sin(radian + u_Time * 6);
	float width = 0.5 * ( 1 - scaledValue );

	newPosition.y = newPosition.y * width + sinValue * scaledValue * width;

	gl_Position = newPosition;
	vColor = vec4((sinValue + 1) / 2);
}

void main()
{
	Flag();
}
