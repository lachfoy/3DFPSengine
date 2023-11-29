#pragma once

#include <glm/glm.hpp>
#include <string>

// base class for UI elements
// panels get constructed with a size and position
// store in a tree structure
// things outside a panel should get clipped by default

#include <vector>
#include <list>

#include "Texture.h"

class Panel;
typedef std::list<Panel*> tPanelList;

class GuiRenderer;
class Input;

class Panel
{
public:
	Panel(const char* name, GuiRenderer* guiRenderer);
	Panel(const char* name, GuiRenderer* guiRenderer, glm::vec2 position, glm::vec2 size);
	Panel(const char* name, GuiRenderer* guiRenderer, glm::vec2 position, glm::vec2 size, Panel* parent);
	~Panel() = default;

	virtual bool HandleInput(Input* input);

	virtual void Render();

	glm::vec2 GetAbsolutePosition() const;
	glm::vec2 GetSize() const;

	bool IsInBounds(glm::vec2 position);

	void SetColor(glm::vec3 color);
	void SetAlpha(float alpha);

	Panel* GetParent() const { return m_parent; }
	tPanelList GetChildren() const { return m_children; }
	const char* GetName() const { return m_name.c_str(); }

	void SetPosition(glm::vec2 position);
	void SetSize(glm::vec2 size);
	void CenterX(); // temp. really we want to have alignment flags and then evaluate these in some kind of layout function
	void CenterY(); // temp. really we want to have alignment flags and then evaluate these in some kind of layout function

	void AddChild(Panel* child);

	void SetScale(float scale);

protected:
	std::string m_name;
	glm::vec2 m_size;

	glm::vec2 m_relativePosition; // top left
	Panel* m_parent = nullptr;

	tPanelList m_children;

	GuiRenderer* m_guiRenderer;

	glm::vec3 m_color = glm::vec3(1.f, 1.f, 1.f);
	float m_alpha = 1.0f;

	float m_scale = 1.0f;

};
