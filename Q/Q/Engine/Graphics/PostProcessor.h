#pragma once
#include <memory>
#include "Engine/Graphics/Framebuffer.h"
#include "glm/glm.hpp"
enum PostProcessorMode
{
	Normal,
	Blur,
	Inversion,
	Redout,
	Greenout,
	MenuBlur,
};

class PostProcessor
{
public:
	PostProcessor(std::shared_ptr<Framebuffer> m_framebuffer);
	~PostProcessor();
	void UpdateScreenSize(const glm::vec2& screen_size);
	void UpdateTimeCounter(float dt);
	void Draw();

	void SetMode(PostProcessorMode mode);
	void SetMode(PostProcessorMode mode, float time);
	PostProcessorMode GetMode() const;
	void Destroy();

private:
	void Init();
	void UseShader(unsigned shaderID);
	
	void InitVertexArray();

	unsigned CreateShader(char* vs, char* fs);


private:
	std::shared_ptr<Framebuffer> m_Framebuffer;
	PostProcessorMode m_Mode;

	glm::vec2 ScreenSize;
	bool CountTime = false;
	float RemainTime = 0.f;
	
	unsigned VertexArray;
	unsigned VertexBuffer;

	
	unsigned NormalShader;
	unsigned BlurShader;
	unsigned InversionShader;
	unsigned RedoutShader;
	unsigned GreenoutShader;
	unsigned MenuBlurShader;
	
};