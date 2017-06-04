#include "BodyGroupProperties.h"
#include "Display.h"
#include "Sim.h"
#include "SimState.h"

#include "imgui.h"
#include "imgui_sfml.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#define WINDOW_W 800
#define WINDOW_H 600

namespace nbody
{
	Sim::Sim()
	{
		// Load images from disk
		this->loadTextures();
		// Initialise objects
		loadObjects();
		// Load font
		loadFonts();
		// Create SFML window
#ifndef NDEBUG
		m_window.create(sf::VideoMode(WINDOW_W, WINDOW_H), "nbody2");
#else
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		auto mode = sf::VideoMode().getFullscreenModes()[0];
		m_window.create(mode, "nbody2", sf::Style::Fullscreen, settings);
#endif
		m_window.setVerticalSyncEnabled(true);
		m_window.setFramerateLimit(120);
		// Set background texture
		m_background.setTexture(m_asset_mgr.getTextureRef("background"));
		m_background.setScale(
			float(m_window.getSize().x) / float(m_background.getTexture()->getSize().x),
			float(m_window.getSize().y) / float(m_background.getTexture()->getSize().y));
		// Set window icon
		auto icon_image = m_asset_mgr.getTextureRef("icon").copyToImage();
		m_window.setIcon(icon_image.getSize().x, icon_image.getSize().y, icon_image.getPixelsPtr());
		// Initialise GUI
		ImGui::SFML::Init(m_window);
		// Initialise values needed for world-screen coordinate conversion
		Display::screen_size = m_window.getSize();
		Display::aspect_ratio = static_cast<float>(Display::screen_size.x) / static_cast<float>(Display::screen_size.y);
	}

	Sim::~Sim()
	{
		while (!(m_states.empty()))
		{
			popState();
			handlePendingOp();
		}
		// Clean up GUI
		ImGui::SFML::Shutdown();
	}

	void Sim::pushState(SimState* state)
	{
		m_pending.op = PendingStateOp::PUSH;
		m_pending.state = state;
	}

	void Sim::popState()
	{
		m_pending.op = PendingStateOp::POP;
		m_pending.state = nullptr;
	}

	void Sim::changeState(SimState* state)
	{
		m_pending.op = PendingStateOp::CHANGE;
		m_pending.state = state;
	}

	SimState* Sim::peekState()
	{
		if (m_states.empty()) return nullptr;
		else return m_states.top();
	}

	void Sim::loadTextures()
	{
		m_asset_mgr.loadTexture("background", "media/blank.png");
		m_asset_mgr.loadTexture("icon", "media/sfml.png");
	}

	void Sim::loadObjects()
	{
		m_asset_mgr.loadIntegrators();
		m_asset_mgr.loadModels();
		m_asset_mgr.loadDistributors();
	}

	void Sim::loadFonts()
	{
		m_asset_mgr.loadFont("droid_sans", 16.f, "media/DroidSans.ttf");
	}

	void Sim::setProperties(SimProperties const& props)
	{
		m_step = props.timestep;
		m_mod_ptr = m_asset_mgr.getModel(props.mod_type);
		m_mod_ptr->init(props.n_bodies, props.timestep);

		m_int_ptr = m_asset_mgr.getIntegrator(props.int_type, m_mod_ptr.get(), m_step);

		for (auto& bgp : props.bg_props)
		{
			auto dist = m_asset_mgr.getDistributor(bgp.dist);
			m_mod_ptr->addBodies(*dist.get(), bgp);
		}

		m_int_ptr->setInitialState(m_mod_ptr->getInitialState());
	}

	void Sim::simLoop()
	{
		sf::Clock clock;

		while (m_window.isOpen())
		{
			auto dt = clock.restart();

			if (m_pending.op != PendingStateOp::NONE)
				handlePendingOp();

			if (peekState() == nullptr) continue;
			peekState()->handleInput();
			peekState()->update(dt);
			m_window.clear(sf::Color::Black);
			peekState()->draw(dt);
			m_window.display();
		}
	}

	void Sim::handlePendingOp()
	{
		switch (m_pending.op)
		{
		case PendingStateOp::CHANGE:
		{
			if (!m_states.empty())
			{
				delete m_states.top();
				m_states.pop();
			}
			m_states.push(m_pending.state);
			break;
		}
		case PendingStateOp::POP:
		{
			if (!m_states.empty())
			{
				delete m_states.top();
				m_states.pop();
			}
			break;
		}
		case PendingStateOp::PUSH:
		{
			m_states.push(m_pending.state);
			break;
		}
		default:
			break;
		}
		m_pending.op = PendingStateOp::NONE;
		m_pending.state = nullptr;
	}
}