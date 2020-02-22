#pragma once
#include <array>

namespace tetra {
	namespace __detail {
		typedef std::array<int64_t, 3> intVec;

		struct _OctagonImpl;
		
	}


	const int16_t precision = 10;

	typedef std::array<double, 3> Vec3d;


	class Octagon
	{
	public:
		Octagon();
		Octagon(const std::array<Vec3d, 8>& vertices);
		Octagon(const Octagon& o) = delete;
		Octagon(Octagon&& o);
		~Octagon();
		void operator=(Octagon o);

		bool contains(const Vec3d& p) const;
		bool contains(double x, double y, double z) const;

	private:
		std::unique_ptr<__detail::_OctagonImpl> oi;

		friend void swap(Octagon&, Octagon&);
	};

	__detail::intVec castPointToInt(Vec3d v);
	std::array<__detail::intVec, 8> castVerticesToInt(std::array<Vec3d, 8>);
}