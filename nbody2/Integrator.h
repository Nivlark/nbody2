#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "Constants.h"
#include "Vector.h"

/*namespace nbody
{
	enum class IntegratorType
	{
		EULER,
		N_INTEGRATORS,
		INVALID = -1
	};

	struct IntegratorProperties
	{
		IntegratorType const type;
		char const* name;
	};
	
	class Integrator
	{
	public:

		Integrator() = default;

		virtual ~Integrator() = default;

		virtual void step(const size_t id, Vector2d& pos, Vector2d& vel, const Vector2d& accel, const double dt) const = 0;
	};

	class EulerIntegrator : public Integrator
	{
	public:
		inline virtual void step(const size_t id, Vector2d& pos, Vector2d& vel, const Vector2d& accel, const double dt) const
		{
			vel += accel * dt;
			pos += vel * dt;
		}
	};
}*/

#endif // INTEGRATOR_H