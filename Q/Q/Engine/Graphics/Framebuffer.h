/*
	File Name: Framebuffer.h
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <vector>

enum class FramebufferTextureFormat
{
	None = 0,

	//Color
	RGBA8,
	RED_INTEGER,

	//depth stencil
	DEPTH24STENCIL8,

	//default
	Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification
{
	FramebufferTextureSpecification() = default;
	FramebufferTextureSpecification(FramebufferTextureFormat format)
		:TextureFormat(format) {}

	FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
};

struct FramebufferAttachmentSpecification
{
	FramebufferAttachmentSpecification() = default;
	FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
		:Attachments(attachments) {}
	std::vector<FramebufferTextureSpecification> Attachments;
};


struct FramebufferSpecification
{
	uint32_t Width, Height;
	//FramebufferFormat
	uint32_t Samples = 1;
	FramebufferAttachmentSpecification Attachments;
};
class Framebuffer
{
public:
	Framebuffer(const FramebufferSpecification& spec);
	~Framebuffer();
	void Invalidate();
	
	void Bind();
	
	void Unbind();

	void Resize(uint32_t width, uint32_t height);

	uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const{ return m_ColorAttachments[index]; }

	const FramebufferSpecification& GetSpecification() const { return m_Specification; }
private:
	uint32_t m_RendererID = 0;

	FramebufferSpecification m_Specification;


	std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
	FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

	std::vector<uint32_t> m_ColorAttachments;
	uint32_t m_DepthAttachment = 0;
};