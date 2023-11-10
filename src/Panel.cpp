#include "Panel.h"

#include "GuiRenderer.h"
#include "Input.h"

Panel::Panel(const char* name, GuiRenderer* guiRenderer) : m_guiRenderer(guiRenderer)
{
	m_name = name;
	m_color = glm::vec3(0.3, 0.3, 0.3);
}

Panel::Panel(const char* name, GuiRenderer* guiRenderer, glm::vec2 position, glm::vec2 size) : m_guiRenderer(guiRenderer), m_relativePosition(position), m_size(size)
{
	m_name = name;
	m_color = glm::vec3(0.3, 0.3, 0.3);
}

Panel::Panel(const char* name, GuiRenderer* guiRenderer, glm::vec2 position, glm::vec2 size, Panel* parent) : m_guiRenderer(guiRenderer), m_relativePosition(position), m_size(size), m_parent(parent)
{
	m_name = name;
	m_color = glm::vec3(0.3, 0.3, 0.3);
}

bool Panel::HandleInput(Input* input)
{
	// return true to consume the input
	// return false to allow the input to be read by other panels

	//if (input->IsKeyPressed(SDL_SCANCODE_Z))
	//{
	//	printf("%s consumed input\n", m_name.c_str());
	//	return true;
	//}

	return false;
}

void Panel::Render()
{
	glm::vec2 position = GetAbsolutePosition();
	glm::vec2 size = GetSize();

	// Apply scale
	glm::vec2 center = position + (size * 0.5f);
	size *= m_scale;
	glm::vec2 newCenter = position + (size * 0.5f);
	glm::vec2 centerDiff = newCenter - center;
	position -= centerDiff;

	m_guiRenderer->AddQuadToBatch(position.x, position.y, size.x, size.y, m_color, m_alpha);
}

//void Panel::DebugRenderBounds()
//{
//	// Render outline
//	glm::vec2 position = GetAbsolutePosition();
//	glm::vec2 size = GetSize();
//
//	gDebugRenderer.AddLine(glm::vec2(position.x, position.y + size.y), position + size, glm::vec3(0.0f, 1.0f, 0.0f)); // bl to br
//	gDebugRenderer.AddLine(position + size, glm::vec2(position.x + size.x, position.y), glm::vec3(0.0f, 1.0f, 0.0f)); // br to tr
//	gDebugRenderer.AddLine(glm::vec2(position.x + size.x, position.y), position, glm::vec3(0.0f, 1.0f, 0.0f)); // tr to tl
//	gDebugRenderer.AddLine(position, glm::vec2(position.x, position.y + size.y), glm::vec3(0.0f, 1.0f, 0.0f)); // tl to bl
//}

glm::vec2 Panel::GetAbsolutePosition() const
{
	if (m_parent)
	{
		return m_parent->GetAbsolutePosition() + m_relativePosition;
	}

	return m_relativePosition;
}

glm::vec2 Panel::GetSize() const
{
	return m_size;
}

bool Panel::IsInBounds(glm::vec2 point)
{
	glm::vec2 position = GetAbsolutePosition();
	glm::vec2 size = GetSize();
	glm::vec2 bottomRight = position + size;

	return point.x >= position.x && point.x <= bottomRight.x && point.y >= position.y && point.y <= bottomRight.y;
}

void Panel::SetAlpha(float alpha)
{
	m_alpha = alpha;
}

void Panel::SetColor(glm::vec3 color)
{
	m_color = color;
}

void Panel::SetPosition(glm::vec2 position)
{
	m_relativePosition = position;
}

void Panel::SetSize(glm::vec2 size)
{
	m_size = size;
}

void Panel::CenterX()
{
	if (m_parent)
	{
		m_relativePosition.x = (m_parent->GetSize().x / 2.0f) - (m_size.x / 2.0f);
	}
}

void Panel::CenterY()
{
	if (m_parent)
	{
		m_relativePosition.y = (m_parent->GetSize().y / 2.0f) - (m_size.y / 2.0f);
	}
}

void Panel::AddChild(Panel* child)
{
	child->m_parent = this;
	m_children.push_back(child);
}

void Panel::SetScale(float scale)
{
	m_scale = scale;
}
