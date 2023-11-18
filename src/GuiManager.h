#pragma once

#include "Panel.h"

class Input;

// Totally unfinished

class GuiManager
{
public:
	void RenderChildren(Panel* panel)
	{
		if (panel)
		{
			//printf("Rendering %s...\n", panel->GetName());

			panel->Render();

			for (Panel* child : panel->GetChildren())
			{
				RenderChildren(child);
			}
		}
	}

	bool PropogateInput(Panel* panel, Input* input)
	{
		bool inputHandled = false;

		if (panel)
		{
			if (panel->HandleInput(input))
			{
				inputHandled = true;
			}

			for (Panel* child : panel->GetChildren())
			{
				if (PropogateInput(child, input))
				{
					inputHandled = true;
					break;
				}
			}
		}

		return inputHandled;
	}


private:

	int m_guiScale = 2;


};

//// GUI test stuff
//m_rootPanel = new Panel("Root", m_guiRenderer, glm::vec2(0, 0), glm::vec2(m_viewportWidth, m_viewportHeight));
//m_rootPanel->SetAlpha(0.0f);
//
//m_testPanel = new Panel("Panel", m_guiRenderer, glm::vec2(0, 0), glm::vec2(150, 170));
//m_rootPanel->AddChild(m_testPanel);
//m_testPanel->SetColor(glm::vec3(0.4, 0.4, 0.4));
//m_testPanel->CenterX();
//m_testPanel->CenterY();
//
//m_button1 = new Button("Button1", m_guiRenderer);
//m_testPanel->AddChild(m_button1);
//m_button1->SetPosition(glm::vec2(25, 20));
//m_button1->SetSize(glm::vec2(100, 30));
//m_button1->AddCallback([]()
//	{
//		printf("Button1 callback!\n");
//	});
//
//
//m_button2 = new Button("Button2", m_guiRenderer);
//m_testPanel->AddChild(m_button2);
//m_button2->SetPosition(glm::vec2(25, 70));
//m_button2->SetSize(glm::vec2(100, 30));
//m_button2->AddCallback([]()
//	{
//		printf("Button2 callback!\n");
//	});
//
//m_button3 = new Button("Button3", m_guiRenderer);
//m_testPanel->AddChild(m_button3);
//m_button3->SetPosition(glm::vec2(25, 120));
//m_button3->SetSize(glm::vec2(100, 30));
//m_button3->SetEnabled(false);