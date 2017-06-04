#ifndef DISTRIBUTOR_H
#define DISTRIBUTOR_H

#include <functional>
#include <chrono>
#include <memory>
#include <random>
#include <vector>

namespace nbody
{
	template<typename T, size_t N>
	class Vector;
	
	using Vector2d = Vector<double, 2>;
	
	namespace priv
	{
		// Calculate the sign of a value
		// Returns -1 if val < 0, +1 if val > 0, and 0 if val == 0 
		template <typename T>
		int sgn(T val)
		{
			return (T(0) < val) - (val < T(0));
		}

		// Calculate the circular velocity at radius r around a mass central_mass
		Vector2d vCirc(Vector2d const& r, double const central_mass);
	}

	// forward declaration
	struct BodyGroupProperties;
	struct ParticleData;

	using param_t = typename std::uniform_real_distribution<>::param_type;

	enum class DistributorType
	{
		EXPONENTIAL,
		ISOTHERMAL,
		PLUMMER,
		N_DISTRIBUTIONS,
		INVALID = -1
	};

	struct DistributorProperties
	{
		DistributorType const type;
		char const* name;
		char const* tooltip;
		bool const has_central_mass;
	};

	class BodyDistributor
	{
	public:
		BodyDistributor()
		{
			get_rand = [](double lower, double upper) -> double
			{
				return dist(gen, param_t{ lower, upper });
			};
		}

		virtual void createDistribution(ParticleData & bodies, BodyGroupProperties const&) const = 0;

	protected:
		std::function<double(double, double)> get_rand;
		static std::default_random_engine gen;
		static std::uniform_real_distribution<> dist;
	};

	class ExponentialDistributor : public BodyDistributor
	{
	private:
		// exponential distribution function
		double expDist(double lambda) const
		{
			return log(1 - get_rand(0, 1)) / lambda;
		}

	public:
		static std::unique_ptr<BodyDistributor> create();
		
		ExponentialDistributor() : BodyDistributor() {}

		virtual void createDistribution(ParticleData & bodies, BodyGroupProperties const& props) const;

	private:
		double static constexpr lambda = -1.5;
	};

	class IsothermalDistributor : public BodyDistributor
	{
	public:
		static std::unique_ptr<BodyDistributor> create();
		
		IsothermalDistributor() : BodyDistributor() {}

		virtual void createDistribution(ParticleData & bodies, BodyGroupProperties const& props) const;
	};

	class PlummerDistributor : public BodyDistributor
	{
	public:
		static std::unique_ptr<BodyDistributor> create();

		PlummerDistributor() : BodyDistributor() {}

		virtual void createDistribution(ParticleData & bodies, BodyGroupProperties const& props) const;
	};
}

#endif // DISTRIBUTOR_H