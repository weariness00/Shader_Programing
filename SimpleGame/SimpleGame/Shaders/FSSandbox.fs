#version 330

layout(location=0) out vec4 FragColor;

in vec4 vColor;

uniform float u_Time;

const float c_PI = 3.141592;

void Circles()
{
	float circleCount = 10;
	vec2 center = vec2(0.5,0.5);
	float dist = distance(vColor.rg, center);
	float input = circleCount * c_PI * 4 * dist - u_Time * 10.0;
	float sinValue = pow(cos(input), 32);

	FragColor = vec4(sinValue);
}

void SinGraph()
{
	vec2 newTexPos = vec2(vColor.r * 2 * c_PI, vColor.g * 2 - 1);
	float sinValue = sin(newTexPos.x - u_Time);
	float width = 0.03f;

	if(sinValue < newTexPos.y + width && sinValue > newTexPos.y)
		FragColor = vec4(1);
	else
		FragColor = vec4(0);
}

void main()
{
	Circles();
	//SinGraph();
}
