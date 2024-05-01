#version 330

in vec3 a_Position;
in vec4 a_Color;
in vec3 a_Velocity;
in float a_StartTime;
in float a_LifeTime;
in float a_amp;
in float a_Period;
in float a_Theta;

out vec4 vColor;

uniform float u_Time = 0;
uniform float u_Period = 2.0;

const vec3 c_StartPos = vec3(-1, 0, 0);
const vec3 c_Velocity = vec3(2.0, 2.0, 0);
const vec3 c_ParaVelocity = vec3(2.0, 2.0, 0);
const float c_PI = 3.141592;
const vec2 c_2DGravity = vec2(0.0, -9.8);

const vec3 c_TriangleStartPos = vec3(0,0,0);
const float c_TriangleLength = 1.0; 
const vec3 c_TrianglePoint[] = vec3[](
	c_TriangleStartPos + vec3(sin(0.0 * c_PI / 180.0),   cos(0.0 * c_PI / 180.0),    0 ) * c_TriangleLength,
	c_TriangleStartPos + vec3(sin(120.0 * c_PI / 180.0), cos(120.0 * c_PI / 180.0),  0 ) * c_TriangleLength,
	c_TriangleStartPos + vec3(sin(240.0 * c_PI / 180.0), cos(240.0 * c_PI / 180.0),  0 ) * c_TriangleLength
	);


void Velocity()
{
	vec4 newPosition = vec4(a_Position, 1);
	float t = u_Time - a_StartTime;

	if(t > 0)
	{
		t = a_LifeTime * fract(t/a_LifeTime);
		float tt = t * t;

		newPosition.xy += a_Velocity.xy * t + 0.5 * tt * c_2DGravity;
	}
	else
	{
		newPosition.x = 100000;
	}

	gl_Position = newPosition;
}

void Basic()
{
	vec4 newPosition = vec4(a_Position, 1);
	gl_Position = newPosition;
}

void Line()
{
	float newTime = abs(fract(u_Time / u_Period) - 0.5) * 2.0;
	vec4 newPosition;
	newPosition.xyz = (c_StartPos + a_Position) + a_Velocity * newTime;
	newPosition.w = 1;
	gl_Position = newPosition;
}

void Circle()
{
	float newTime = fract(u_Time / u_Period);
	float radian = newTime * c_PI * 2;
	vec4 newPosition;
	newPosition.xy = a_Position.xy + vec2(cos(radian), sin(radian));
	gl_Position = newPosition;
}

void Parabola()
{
	float newTime = fract(u_Time / u_Period);
	float tt = newTime * newTime;

	vec4 newPosition;
	newPosition.xy = c_StartPos.xy + a_Position.xy + c_Velocity.xy * newTime + c_2DGravity * tt / 2;
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

void SinShape()
{
	vec4 newPosition = vec4(a_Position, 1);

	float t = u_Time - a_StartTime;
	float amp = a_amp;
	float period = a_Period;

	if(t > 0)
	{
		t = a_LifeTime * fract(t/a_LifeTime);
		float tt = t * t;

		newPosition.x += cos(a_Theta * 2.0 * c_PI);
		newPosition.y += sin(a_Theta * 2.0 * c_PI);

		vec2 newVel = a_Velocity.xy + c_2DGravity * t;
		vec2 newDir = vec2(-newVel.y, newVel.x);
		newDir = normalize(newDir);
		newPosition.xy += a_Velocity.xy *t + 0.5 * c_2DGravity * tt;
		newPosition.xy += newDir * amp * (t*0.2) * sin((newPosition.x + 1) * c_PI * period);
	}
	else
	{
		newPosition.x = 100000;
	}

	gl_Position = newPosition;
}

void SinShapeCycle()
{
	vec4 newPosition = vec4(a_Position, 1);

	float t = u_Time - a_StartTime;
	float amp = a_amp;
	float period = a_Period;

	if(t > 0)
	{
		t = a_LifeTime * fract(t/a_LifeTime);
		float tt = t * t;

		float radian = a_Theta * a_StartTime * 2.0 * c_PI;

		newPosition.x += cos(radian);
		newPosition.y += sin(radian);

		vec2 newVel = a_Velocity.xy + c_2DGravity * t;
		vec2 newDir = vec2(-newVel.y, newVel.x);
		newDir = normalize(newDir);
		newPosition.xy += a_Velocity.xy *t + 0.5 * c_2DGravity * tt;
		newPosition.xy += newDir * amp * (t*0.1) * sin((newPosition.x + 1) * c_PI * period);
	}
	else
	{
		newPosition.x = 100000;
	}

	gl_Position = newPosition;
}

void main()
{
	//Line();
	//Circle();
	//Parabola();
	//Triangle();
	//Basic();

	//SinShape();
	//SinShapeCycle();

	Velocity();

	vColor = a_Color;
}
