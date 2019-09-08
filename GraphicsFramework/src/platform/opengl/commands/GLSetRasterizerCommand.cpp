#include "stdafx.h"

#include "GLSetRasterizerCommand.h"
#include "../GLHelperMethods.h"

GLSetRasterizerCommand::GLSetRasterizerCommand(const RasterizerSettings& m_settings) :
	SetRasterizerCommand(m_settings)
{

}

GLSetRasterizerCommand::~GLSetRasterizerCommand()
{

}

void GLSetRasterizerCommand::Execute()
{
	m_settings.enable_depth_clamp ? glEnable(GL_DEPTH_CLAMP) : glDisable(GL_DEPTH_CLAMP);
	m_settings.is_discarded ? glEnable(GL_RASTERIZER_DISCARD) : glDisable(GL_RASTERIZER_DISCARD);

	FillMode polygon_mode = m_settings.polygon_mode;
	CullMode cull_mode = m_settings.cull_mode;

	glPolygonMode(cull_mode.ToOpenGL(), polygon_mode.ToOpenGL());

	glLineWidth(m_settings.line_width);
	glFrontFace(m_settings.front_face_orientation.ToOpenGL());

	if (m_settings.enable_depth_bias)
	{
		switch (m_settings.polygon_mode)
		{
		case EFillMode::UNDEFINED:	glEnable(-1);
		case EFillMode::FILL:		glEnable(GL_POLYGON_OFFSET_FILL);
		case EFillMode::WIREFRAME:	glEnable(GL_POLYGON_OFFSET_LINE);
		case EFillMode::VERTEX:		glEnable(GL_POLYGON_OFFSET_POINT);
		}
		glPolygonOffset(m_settings.depth_bias_constant_factor, m_settings.depth_bias_slope_factor);
	}

	GLCALL(glDepthRangef(0, m_settings.depth_bias_clamp));

	m_settings.enable_multi_sample ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);

	if (m_settings.enable_scissor)
		spdlog::warn("GL scissoring not implemented yet!");

}