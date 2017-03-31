#include "imgui.h"
#include "imgui-sfml.h"


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML\Graphics.hpp>

#include <iostream>
#include <time.h>
#include <string>
#include <vector>


static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

int main()
{
	

	static int HEIGHT = 600;
	static int WIDTH = 800;
	static int BORDER_ON_TOP = 20;
	static int dragSize = 50;
	static int dragX = WIDTH / 2 - dragSize;
	static int dragY = HEIGHT / 2 - dragSize;


	srand((unsigned int)time(NULL));

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "", sf::Style::None);
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	sf::Color bgColor;

	float color[3] = { 0.f, 0.f, 0.f };

	// let's use char array as buffer, see next part
	// for instructions on using std::string with ImGui
	char windowTitle[255] = "ImGui + SFML = <3";

	window.setTitle(windowTitle);
	window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
	sf::Clock deltaClock;



	/*     HOW TO MOVE A BORDERLESS WINDOW            */
	sf::Vector2i grabbedOffset;
	bool grabbedWindow = false;
	bool redyToMove = false;
	////////////////////////////////////////////////////

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
////////////////////////////////////////////////////////////////////////////////////////////////
/*					 HOW TO MOVE A BORDERLESS WINDOW						        */
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				// this will get the position of the mouse ON THE DESKTOP
				sf::Vector2i mPos = sf::Mouse::getPosition();
				// this will get the position of the WINDOW
				sf::Vector2i wPos = window.getPosition();

				// calculate if the mouse is on the window, but only max 20 pixels from the top 
				int mousePosition = mPos.y - wPos.y;
				if (mousePosition > 0 && mousePosition < BORDER_ON_TOP)
					redyToMove = true;
				else
					redyToMove = false;
				// redyToMove will only be true if the mouse is over the top border
				if(redyToMove)
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					grabbedOffset = window.getPosition() - sf::Mouse::getPosition();
					grabbedWindow = true;
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
					grabbedWindow = false;
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				if (grabbedWindow)
					window.setPosition(sf::Mouse::getPosition() + grabbedOffset);
			}
////////////////////////////////////////////////////////////////////////////////////////////////

		}

		ImGui::SFML::Update(window, deltaClock.restart());



		


		// Setting next window to full screen and with a static size and position
	/*	ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
		ImGui::SetNextWindowSizeConstraints(ImVec2(WIDTH, HEIGHT), ImVec2(WIDTH, HEIGHT));
		ImGui::SetNextWindowPos(ImVec2(0, 0));*/

		ImGui::Begin(u8"Mitt Fönster"); // begin window

		if (ImGui::Button(u8"Återställ", ImVec2(100, 30)))
		{
			dragSize = 50;
			dragX = WIDTH / 2 - dragSize;
			dragY = HEIGHT / 2 - dragSize;
		}
		
		ImGui::DragInt("Circle size", &dragSize, 1);
		ImGui::DragInt("Circle pos.x", &dragX, 1);
		ImGui::DragInt("Circle pos.y", &dragY, 1);
		ImGui::SameLine(); ShowHelpMarker("Click and drag to edit value.\nHold SHIFT/ALT for faster/slower edit.\nDouble-click or CTRL+click to input value.");


		ImGui::End(); // end window

		//ImGui::ShowTestWindow();

		sf::CircleShape circle;
		circle.setRadius(dragSize);
		circle.setOutlineColor(sf::Color::Red);
		circle.setOutlineThickness(5);
		circle.setPosition(dragX, dragY);




		window.clear(bgColor); // fill background with color
		window.draw(circle);
		ImGui::Render();
		window.display();
	}

	ImGui::SFML::Shutdown();
}