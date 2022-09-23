#include "PostProcessor.h"
#include <iostream>
#include <GL/glew.h>
static const float quad_vertex_buffer_data[] =
{
-1.0f, -1.0f, 0.0f,
 1.0f, -1.0f, 0.0f,
-1.0f,  1.0f, 0.0f,
-1.0f,  1.0f, 0.0f,
 1.0f, -1.0f, 0.0f,
 1.0f,  1.0f, 0.0f,
};

static char* vs = R"(
#version 330 core
	
layout(location = 0) in vec3 vtxPos;

out vec2 UV;

void main()
{
	gl_Position =  vec4(vtxPos,1);
	UV = (vtxPos.xy+vec2(1,1))/2.0;
}
)";

static char* normal_fs = R"(
#version 330 core

// Ouput data
layout(location = 0) out vec4 color;

uniform sampler2D finaltexture;

in vec2 UV;

void main()
{
	color = texture(finaltexture,UV);
}
)";

static char* inversion_fs = R"(
#version 330 core

// Ouput data
layout(location = 0) out vec4 color;

uniform sampler2D finaltexture;

in vec2 UV;
		
void main()
{
	color = texture(finaltexture,UV);
	color=vec4(vec3(1.f)-color.xyz,1.f);
}
)";


static char* blur_fs = R"(#version 330 core

// Ouput data
layout(location = 0) out vec4 color;

uniform sampler2D finaltexture;

in vec2 UV;


const float offset = 1.0 / 300.0;  

void main()
{
	vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right    
	);

    float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
	);
    
	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
	    sampleTex[i] = vec3(texture(finaltexture, UV+ offsets[i]));
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
	    col += sampleTex[i] * kernel[i];
	color = vec4(col, 1.0);
}
)";



static char* redout_vs = R"(
#version 330 core
	
layout(location = 0) in vec3 vtxPos;

out vec2 UV;

out vec2 screenPos;

void main()
{
	gl_Position =  vec4(vtxPos,1);
	UV = (vtxPos.xy+vec2(1,1))/2.0;
	screenPos=gl_Position.xy;
}
)";

static char* redout_fs = R"(
#version 330 core

// Ouput data
layout(location = 0) out vec4 color;

uniform sampler2D finaltexture;

in vec2 UV;
in vec2 screenPos;

		
void main()
{
	float dist=distance(screenPos,vec2(0.0))*0.3;
	color = texture(finaltexture,UV)+vec4(1.f,0.f,0.f,1.f)*(dist);
}
)";

static char* greenout_vs = R"(
#version 330 core
	
layout(location = 0) in vec3 vtxPos;

out vec2 UV;

out vec2 screenPos;

void main()
{
	gl_Position =  vec4(vtxPos,1);
	UV = (vtxPos.xy+vec2(1,1))/2.0;
	screenPos=gl_Position.xy;
}
)";

static char* greenout_fs = R"(
#version 330 core

// Ouput data
layout(location = 0) out vec4 color;

uniform sampler2D finaltexture;

in vec2 UV;
in vec2 screenPos;

		
void main()
{
	float dist=distance(screenPos,vec2(0.0))*0.3;
	color = texture(finaltexture,UV)+vec4(0.023f,0.25f,0.039f,1.f)*(dist);
}
)";


static char* menublur_fs = R"(#version 330 core

// Ouput data
layout(location = 0) out vec4 color;

uniform sampler2D finaltexture;
uniform vec2 ScreenSize;


in vec2 UV;



const float offset = 1.0 / 300.0;  

void main()
{
	float x=gl_FragCoord.x/ScreenSize.x;
	float y=gl_FragCoord.y/ScreenSize.y;
	if((0.2<=x && x <= 0.8) && (0.2<=y && y<=0.8))
	{
		color = texture(finaltexture,UV);
	}
	else
	{
		vec2 offsets[9] = vec2[](
	    vec2(-offset,  offset), // top-left
	    vec2( 0.0f,    offset), // top-center
	    vec2( offset,  offset), // top-right
	    vec2(-offset,  0.0f),   // center-left
	    vec2( 0.0f,    0.0f),   // center-center
	    vec2( offset,  0.0f),   // center-right
	    vec2(-offset, -offset), // bottom-left
	    vec2( 0.0f,   -offset), // bottom-center
	    vec2( offset, -offset)  // bottom-right    
		);

	    float kernel[9] = float[](
	    1.0 / 16, 2.0 / 16, 1.0 / 16,
	    2.0 / 16, 4.0 / 16, 2.0 / 16,
	    1.0 / 16, 2.0 / 16, 1.0 / 16  
		);
	    
		vec3 sampleTex[9];
		for(int i = 0; i < 9; i++)
		{
		    sampleTex[i] = vec3(texture(finaltexture, UV+ offsets[i]));
		}
		vec3 col = vec3(0.0);
		for(int i = 0; i < 9; i++)
		    col += sampleTex[i] * kernel[i];
		color = vec4(col, 1.0);
	}
}
)";

PostProcessor::PostProcessor(std::shared_ptr<Framebuffer> m_framebuffer)
	:m_Framebuffer(m_framebuffer), m_Mode(Normal)
{
	Init();
}

PostProcessor::~PostProcessor()
{
}

void PostProcessor::UpdateScreenSize(const glm::vec2& screen_size)
{
	ScreenSize = screen_size;
}

void PostProcessor::UpdateTimeCounter(float dt)
{
	if(CountTime==false)
		return;
	
	if (RemainTime <= 0.f)
	{
		m_Mode = Normal;
		CountTime = false;
		return;
	}
	RemainTime -= dt;
}

void PostProcessor::Draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Framebuffer->GetColorAttachmentRendererID());
	switch (m_Mode)
	{
	case Normal:
		UseShader(NormalShader);
		break;
	case Blur:
		UseShader(BlurShader);
		break;
	case Inversion:
		UseShader(InversionShader);
		break;
	case Redout:
		UseShader(RedoutShader);
		break;
	case Greenout:
		UseShader(GreenoutShader);
		break;
	case MenuBlur:
		UseShader(MenuBlurShader);
		glUniform2f(glGetUniformLocation(MenuBlurShader, "ScreenSize"), ScreenSize.x, ScreenSize.y);
		break;
	}
	glBindVertexArray(VertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);
}

void PostProcessor::SetMode(PostProcessorMode mode)
{
	m_Mode = mode;
	CountTime = false;
}

void PostProcessor::SetMode(PostProcessorMode mode, float time)
{
	SetMode(mode);
	RemainTime = time;
	CountTime = true;
}

PostProcessorMode PostProcessor::GetMode() const
{
	return m_Mode;
}

void PostProcessor::Init()
{
	InitVertexArray();
	NormalShader = CreateShader(vs, normal_fs);
	BlurShader = CreateShader(vs, blur_fs);
	InversionShader = CreateShader(vs, inversion_fs);
	RedoutShader = CreateShader(redout_vs, redout_fs);
	GreenoutShader = CreateShader(greenout_vs, greenout_fs);
	MenuBlurShader = CreateShader(vs, menublur_fs);
}

void PostProcessor::Destroy()
{
	glDeleteBuffers(1, &VertexBuffer);
	glDeleteVertexArrays(1, &VertexArray);
	glDeleteProgram(NormalShader);
	glDeleteProgram(BlurShader);
	glDeleteProgram(InversionShader);
	glDeleteProgram(RedoutShader);
	glDeleteProgram(GreenoutShader);
	glDeleteProgram(MenuBlurShader);
}

void PostProcessor::UseShader(unsigned shaderID)
{
	glUniform1i(glGetUniformLocation(shaderID, "finaltexture"), 0);
	glUseProgram(shaderID);
}

void PostProcessor::InitVertexArray()
{
	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	glCreateBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_buffer_data), quad_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
}

unsigned PostProcessor::CreateShader(char* vs, char* fs)
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderID, 1, &vs, nullptr);
	glCompileShader(VertexShaderID);
	GLint result;
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		std::cout << "Error! : vert" << std::endl;
		GLint log_length = 0;
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<char> error(log_length);
		glGetShaderInfoLog(VertexShaderID, log_length, nullptr, error.data());
		glDeleteShader(VertexShaderID);
		VertexShaderID = 0;
		std::string error_log = std::string(error.begin(), error.end());
		std::cout << error_log << std::endl;
	}



	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderID, 1, &fs, nullptr);
	glCompileShader(FragmentShaderID);
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		std::cout << "Error! : frag" << std::endl;
		GLint log_length = 0;
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<char> error(log_length);
		glGetShaderInfoLog(FragmentShaderID, log_length, nullptr, error.data());
		glDeleteShader(FragmentShaderID);
		FragmentShaderID = 0;
		std::string error_log = std::string(error.begin(), error.end());
		std::cout << error_log << std::endl;
	}

	unsigned FinalDrawShaderID = glCreateProgram();

	glAttachShader(FinalDrawShaderID, VertexShaderID);
	glAttachShader(FinalDrawShaderID, FragmentShaderID);
	glLinkProgram(FinalDrawShaderID);
	GLint lnk_log;
	GLint lnk_status;
	glGetProgramiv(FinalDrawShaderID, GL_INFO_LOG_LENGTH, &lnk_log);
	char* error_log = new char[lnk_log];
	error_log[0] = '\0';
	glGetProgramiv(FinalDrawShaderID, GL_LINK_STATUS, &lnk_status);
	if (GL_FALSE == lnk_status)
	{
		std::cerr << "Failed to link shader program\n";
		glGetProgramInfoLog(FinalDrawShaderID, lnk_log, NULL, error_log);
		std::cout << error_log << std::endl;
	}
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return FinalDrawShaderID;
}

