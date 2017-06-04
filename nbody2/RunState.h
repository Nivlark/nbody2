#ifndef RUN_STATE_H
#define RUN_STATE_H

#include "BodyManager.h"
#include "QuadManager.h"
#include "IState.h"
#include "TrailManager.h"
#include "Vector.h"

#include <SFML/Graphics.hpp>

namespace nbody
{
	class BHTree;
	class Quad;
	class Sim;

	struct Flags
	{
		Flags() :
			running(false),
			show_bodies(true), show_trails(false), show_grid(false),
			view_centre(true), view_dragging(false),
			tree_exists(false), grid_mode_complete(true) {}

		bool running : 1;
		
		bool show_bodies : 1;
		bool show_trails : 1;
		bool show_grid : 1;

		bool view_centre : 1;
		bool view_dragging : 1;
		bool tree_exists : 1;
		bool grid_mode_complete : 1;
	};

	class RunState : public IState
	{
	private:
		sf::View m_main_view;
		sf::View m_gui_view;

		BodyManager m_body_mgr;
		TrailManager m_trail_mgr;
		QuadManager m_quad_mgr;

		Flags m_flags;

	public:
		virtual void draw(sf::Time const dt);
		virtual void update(sf::Time const dt);
		virtual void handleInput();

		RunState(Sim * sim);
	};
}

#endif // RUN_STATE_H
