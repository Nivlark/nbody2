#ifndef MODELBRUTEFORCE_H
#define MODELBRUTEFORCE_H

#include "IModel.h"

namespace nbody
{
	class ModelBruteForce : public IModel
	{
	public:
		ModelBruteForce();
		~ModelBruteForce();

		static std::unique_ptr<IModel> create();

		virtual void addBodies(BodyDistributor const& dist, BodyGroupProperties const& bgp);

		virtual void eval(Vector2d * state_in, double time, Vector2d * deriv_out);

	private:
		Vector2d m_centre_mass;
		double m_tot_mass;
	};
}

#endif // !MODELBRUTEFORCE_H
