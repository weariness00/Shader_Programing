#version 330

uniform sampler2D uTextureSampler;

layout(location=0) out vec4 FragColor;

in vec2 v_TexPos;

void main()
{
	// 0
	//float tx = v_TexPos.x;
	//float ty = v_TexPos.y * 2;
	//ty -= 1;
	//ty = abs(ty);
	//ty -= 1;
	//ty = abs(ty);
	//vec2 tp = vec2(tx, ty);

	// 1
	float tx = (v_TexPos.x + 0.3333f);
	float ty = v_TexPos.y / 3;

	vec2 tp = vec2(tx, ty);


	FragColor = texture(uTextureSampler, tp);
}
