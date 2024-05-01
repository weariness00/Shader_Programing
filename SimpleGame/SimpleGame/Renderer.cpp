#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	
	//Load shaders
	m_ParticleShader = CompileShaders("./Shaders/Particle.vs", "./Shaders/Particle.fs");

	m_TextureSandboxShader = CompileShaders("./Shaders/TextureSandbox.vs", "./Shaders/TextureSandbox.fs");

	m_FSSandboxSahder = CompileShaders("./Shaders/FSSandbox.vs", "./Shaders/FSSandbox.fs");
	
	m_GridMeshShader = CompileShaders("./Shaders/GridMesh.vs", "./Shaders/GridMesh.fs");

	//Create VBOs
	CreateVertexBufferObjects();

	CreateParticlesCloud(1000);

	CreateGridMesh();

	//Create RGB Texture
	m_RGBTexture = CreatePngTexture("./rgb.png", GL_NEAREST);

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float vertices[] = { 
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 1.0, 0.0 };

	glGenBuffers(1, &m_TestVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TestVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	float size = 0.5f;

	float particleVerts[] = {
		 size,  size, 0.0,
		-size,  size, 0.0,
		-size, -size, 0.0,
		 size,  size, 0.0,
		-size, -size, 0.0,
		 size, -size, 0.0,
	};

	glGenBuffers(1, &m_ParticleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleVerts), particleVerts, GL_STATIC_DRAW);

	//TextureSandBox
	float TexturesandboxVerts[] =
	{
	   -size,   -size,   0, 0.0f, 1.0f,
	   size,    size,   0, 1.0f, 0.0f,
	   -size,   size,   0, 0.0f, 0.0f,
	   -size,   -size,   0, 0.0f, 1.0f,
	   size,    -size,   0, 1.0f, 1.0f,
	   size,   size,   0, 1.0f, 0.0f
	};

	glGenBuffers(1, &m_TextureSandboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexturesandboxVerts), TexturesandboxVerts, GL_STATIC_DRAW);


	float FSSandboxVertices[] = {
		 size,  size, 0.0,
		-size,  size, 0.0,
		-size, -size, 0.0,
		 size,  size, 0.0,
		-size, -size, 0.0,
		 size, -size, 0.0,
	};

	glGenBuffers(1, &m_FSSandoboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_FSSandoboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FSSandboxVertices), FSSandboxVertices, GL_STATIC_DRAW);

}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done." << '\n';

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

GLuint Renderer::CreatePngTexture(char* filePath, GLuint samplingMethod)
{
	//Load Png
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);

	if (error != 0)
	{
		std::cout << "PNG image loading failed:" << filePath << std::endl;
		//assert(0);
	}

	GLuint temp;

	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,GL_UNSIGNED_BYTE, &image[0]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);

	return temp;
}


void Renderer::CreateParticlesCloud(int numParticles)
{
	float centerX, centerY;
	centerY = centerX = 0;
	float size = 0.01f;
	int particleCount = numParticles;
	int vertexCount = numParticles * 6;
	int floatCount = vertexCount * (3 + 3 + 1 + 1 + 1 + 1 + 1 + 4);
	
	float vx, vy, vz;
	float startTime, lifeTime;
	float amp, period;
	float value;
	float r, g, b, a;

	float* vertices = NULL;
	vertices = new float[floatCount];

	int index = 0;
	float s[12]{ -size, -size, size, size, -size,size,-size,-size, size,-size,size,size };
	for (int i = 0; i < particleCount; ++i)
	{
		//centerX = centerY = 0.f;
		//vx = vy = vz = 0.f;
		centerX = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		centerY = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		vx = 0.0f;//((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		vy = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		vz = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		startTime = 8.f * ((float)rand() / (float)RAND_MAX);
		lifeTime = 0.2f * ((float)rand() / (float)RAND_MAX) + 1.f;
		amp = ((float)rand() / (float)RAND_MAX - 0.5f) * 2.0f;
		period = (float)rand() / (float)RAND_MAX;
		value = ((float)rand() / (float)RAND_MAX);
		r = ((float)rand() / (float)RAND_MAX);
		g = ((float)rand() / (float)RAND_MAX);
		b = ((float)rand() / (float)RAND_MAX);
		a = 1;

		for (int v = 0; v < 6; v++)
		{
			vertices[index++] = centerX + s[v * 2];
			vertices[index++] = centerY + s[v * 2 + 1];
			vertices[index++] = 0.f;
			vertices[index++] = vx;
			vertices[index++] = vy;
			vertices[index++] = vz;
			vertices[index++] = startTime;
			vertices[index++] = lifeTime;
			vertices[index++] = amp;
			vertices[index++] = period;
			vertices[index++] = value;
			vertices[index++] = r;
			vertices[index++] = g;
			vertices[index++] = b;
			vertices[index++] = a;
		}
	}

	glGenBuffers(1, &m_ParticleCloudVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices, GL_STATIC_DRAW);

	m_ParticleCloudVertexCount = vertexCount;

	delete[] vertices;
}

void Renderer::CreateGridMesh()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 16;
	int pointCountY = 16;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];
	m_GridMeshVertexCount = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_GridMeshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);

}

void Renderer::DrawTest()
{
	//Program select
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_TestVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attribPosition);

}

void Renderer::DrawParticle()
{
	//Program select
	GLuint shader = m_ParticleShader;
	glUseProgram(shader);

	m_ParticleTime += 0.016;

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glUniform1f(glGetUniformLocation(shader, "u_Time"), m_ParticleTime);
	glUniform1f(glGetUniformLocation(shader, "u_Period"), 2.0);


	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

}

void Renderer::DrawParticleCloud()
{
	//Program select
	GLuint shader = m_ParticleShader;
	glUseProgram(shader);

	m_ParticleTime += 0.016;

	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);

	int stride = sizeof(float) * (3 + 3 + 1 + 1 + 1 + 1 + 1 + 4);
	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(attribPosition);

	int attribVelocity = glGetAttribLocation(shader, "a_Velocity");
	glVertexAttribPointer(attribVelocity, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(attribVelocity);

	int attribStartTime = glGetAttribLocation(shader, "a_StartTime");
	glVertexAttribPointer(attribStartTime, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(attribStartTime);

	int attribLifeTime = glGetAttribLocation(shader, "a_LifeTime");
	glVertexAttribPointer(attribLifeTime, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 7));
	glEnableVertexAttribArray(attribLifeTime);

	int attribAMP = glGetAttribLocation(shader, "a_amp");
	glVertexAttribPointer(attribAMP, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 8));
	glEnableVertexAttribArray(attribAMP);

	int attribPeriod = glGetAttribLocation(shader, "a_Period");
	glVertexAttribPointer(attribPeriod, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 9));
	glEnableVertexAttribArray(attribPeriod);

	int attribTheta = glGetAttribLocation(shader, "a_Theta");
	glVertexAttribPointer(attribTheta, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 10));
	glEnableVertexAttribArray(attribTheta);

	int attribColor = glGetAttribLocation(shader, "a_Color");
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 11));
	glEnableVertexAttribArray(attribColor);

	glUniform1f(glGetUniformLocation(shader, "u_Time"), m_ParticleTime);
	glUniform1f(glGetUniformLocation(shader, "u_Period"), 2.0);

	glDrawArrays(GL_TRIANGLES, 0, m_ParticleCloudVertexCount);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribVelocity);
	glDisableVertexAttribArray(attribStartTime);
	glDisableVertexAttribArray(attribLifeTime);
	glDisableVertexAttribArray(attribAMP);
	glDisableVertexAttribArray(attribPeriod);
	glDisableVertexAttribArray(attribTheta);
	glDisableVertexAttribArray(attribColor);
}

void Renderer::DrawFSSandbox()
{
	GLuint shader = m_FSSandboxSahder;
	glUseProgram(shader);

	m_FSSandboxTime += 0.016;

	glBindBuffer(GL_ARRAY_BUFFER, m_FSSandoboxVBO);

	int stride = sizeof(float) * (3);
	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(attribPosition);

	glUniform1f(glGetUniformLocation(shader, "u_Time"), m_FSSandboxTime);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::DrawGridMesh()
{
	//Program select
	GLuint shader = m_GridMeshShader;
	glUseProgram(shader);

	m_GridMeshTime += 0.016;

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glUniform1f(glGetUniformLocation(shader, "u_Time"), m_GridMeshTime);
	glUniform1f(glGetUniformLocation(shader, "u_Period"), 2.0);

	glDrawArrays(GL_TRIANGLES, 0, m_GridMeshVertexCount);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::DrawTextrueSandbox()
{
	//Program select
	GLuint shader = m_TextureSandboxShader;

	glUseProgram(shader);

	int cnt = 5;   // 0~2:Pos 3~4:UV

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * cnt, 0);

	int attribTexPos = glGetAttribLocation(shader, "a_Texture");
	glEnableVertexAttribArray(attribTexPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glVertexAttribPointer(attribTexPos, 2, GL_FLOAT, GL_FALSE, sizeof(float) * cnt, (GLvoid*)(sizeof(float) * 3));

	GLuint uTimeID = glGetUniformLocation(shader, "u_Time");
	float timeScale = 0.016f;
	m_TextureSandboxTime += timeScale;
	glUniform1f(uTimeID, m_TextureSandboxTime);

	GLuint ul_Texture = glGetUniformLocation(shader, "uTextureSampler");
	glUniform1i(ul_Texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RGBTexture);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
	glDisableVertexAttribArray(attribPosition);

	glDisable(GL_BLEND);
}
