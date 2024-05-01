#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);

	void DrawTest();
	void DrawParticle();

	void DrawParticleCloud();
	void DrawFSSandbox();
	void DrawGridMesh();

	void DrawTextrueSandbox();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);
	GLuint CreatePngTexture(char* filePath, GLuint samplingMethod);

	void CreateParticlesCloud(int numParticles);
	void CreateGridMesh();

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0;

	GLuint m_TestVBO = 0;

	GLuint m_ParticleShader = 0;
	GLuint m_ParticleVBO = 0;
	GLuint m_ParticleCloudVBO = 0;
	GLuint m_ParticleCloudVertexCount = 0;
	float m_ParticleTime = 0.f;

	GLuint m_TextureSandboxShader = 0;
	GLuint m_TextureSandboxVBO = 0;
	float m_TextureSandboxTime = 0;

	GLuint m_RGBTexture = 0;

	GLuint m_FSSandboxSahder = 0;
	GLuint m_FSSandoboxVBO = 0;
	float m_FSSandboxTime = 0;

	GLuint m_GridMeshShader = 0;
	GLuint m_GridMeshVBO = 0;
	GLuint m_GridMeshVertexCount = 0;
	float m_GridMeshTime = 0;
};

