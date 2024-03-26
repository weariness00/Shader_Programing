#version 330

in vec3 a_Position;

uniform float u_Time = 0;
uniform float u_Period = 2.0;

const vec3 c_StartPos = vec3(-1, 0, 0);
const vec3 c_Velocity = vec3(2.0, 0, 0);
const vec3 c_ParaVelocity = vec3(2.0, 2.0, 0);
const float c_PI = 3.141592;
const vec2 c_2DGravity = vec2(0.0, -4.9);

const vec3 c_TriangleStartPos = vec3(0,0,0);
const float c_TriangleLength = 1.0; 
const vec3 c_TrianglePoint[] = vec3[](
	c_TriangleStartPos + vec3(sin(0.0 * c_PI / 180.0),   cos(0.0 * c_PI / 180.0),    0 ) * c_TriangleLength,
	c_TriangleStartPos + vec3(sin(120.0 * c_PI / 180.0), cos(120.0 * c_PI / 180.0),  0 ) * c_TriangleLength,
	c_TriangleStartPos + vec3(sin(240.0 * c_PI / 180.0), cos(240.0 * c_PI / 180.0),  0 ) * c_TriangleLength
	);

void Basic()
{
	vec4 newPosition = vec4(a_Position, 1);
	gl_Position = newPosition;
}

void Line()
{
	float newTime = abs(fract(u_Time / u_Period) - 0.5) * 2.0;
	vec4 newPosition;
	newPosition.xyz = (c_StartPos + a_Position) + c_Velocity * newTime;
	newPosition.w = 1;
	gl_Position = newPosition;
}

void Circle()
{
	float newTime = -fract(u_Time / u_Period) * c_PI * 2.0;
	vec2 trans = vec2(cos(newTime), sin(newTime));
	vec4 newPosition;
	newPosition.xy = (a_Position.xy + trans);
	newPosition.zw = vec2(0, 1);
	gl_Position = newPosition;
}

void Parabola()
{
	float newTime = fract(u_Time / u_Period);
	float t = newTime;
	float tt = t * t;
	vec4 newPosition;
	float transX = (a_Position.x + c_StartPos.x)
		+ c_ParaVelocity.x * newTime 
		+ 0.5 * c_2DGravity.x * tt;
	float transY = (a_Position.y + c_StartPos.y)
		+ c_ParaVelocity.y * newTime
		+ 0.5 * c_2DGravity.y * tt;

	newPosition.xy = vec2(transX, transY);
	newPosition.zw = vec2(0, 1);
	gl_Position = newPosition;

}

void Triangle()
{
	float t = fract(u_Time / 9.0);

    // 세 변을 따라 이동하는 지점 계산
    float localT1 = fract(t * 3.0);
    float localT2 = fract(t * 3.0 - 1.0);
    float localT3 = fract(t * 3.0 - 2.0);
    
    vec3 pos1 = mix(c_TrianglePoint[0], c_TrianglePoint[1], localT1);
    vec3 pos2 = mix(c_TrianglePoint[1], c_TrianglePoint[2], localT2);
    vec3 pos3 = mix(c_TrianglePoint[2], c_TrianglePoint[0], localT3);
    
    // 각 세그먼트의 활성화를 결정하기 위한 가중치 계산
    float weight1 = step(0, t) * step(t, 0.3333);
    float weight2 = step(0.3333, t) * step(t, 0.6666);
    float weight3 = step(0.6666, t) * step(t, 1.0);
    
    // 최종 위치 결정
    vec3 position = pos1 * weight1 + pos2 * weight2 + pos3 * weight3;

	vec4 newPosition;
	float transX = (position.x + a_Position.x + c_TriangleStartPos.x);
	float transY = (position.y + a_Position.y + c_TriangleStartPos.y);

	newPosition.xy = vec2(transX, transY);
	newPosition.zw = vec2(0, 1);
	gl_Position = newPosition;
}

void main()
{
	//Line();
	//Circle();
	//Parabola();
	Triangle();
	//Basic();
}

