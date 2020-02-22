#include "TetraPacker.hpp"

namespace tetra {

	namespace __detail {

		/*
			FUNCTIONS
		*/

		intVec subtract(intVec v1, intVec v2) { return { {v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]} }; }

		intVec cross(intVec v1, intVec v2) {
			intVec result;
			result[0] = v1[1] * v2[2] - v1[2] * v2[1];
			result[1] = v1[2] * v2[0] - v1[0] * v2[2];
			result[2] = v1[0] * v2[1] - v1[1] * v2[0];
			return result;
		}

		int64_t dot(intVec v1, intVec v2) { return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]; }

		Norm computeNorm(intVec p1, intVec p2, intVec p3) {
			intVec v1 = subtract(p2, p1);
			intVec v2 = subtract(p1, p3);
			intVec c = cross(v2, v1);
			return { c, dot(c, p1) };
		}

		intVec integerized(Vec3d vec)
		{
			return { {int(vec[0] * precision), int(vec[1] * precision), int(vec[2] * precision)} };
		}

		std::array<intVec, 8> integerizedArray(std::array<Vec3d, 8> vertices)
		{
			std::array<intVec, 8> intVert = {};
			for (int i = 0; i < 8; i++) intVert[i] = integerized(vertices[i]);
			return intVert;
		}
		
		/*
			NORM
		*/
		bool Norm::isAboveOrEqual(intVec point) {
			return dot(point, this->normVector) >= this->heightOfPlane;
		}

		bool Norm::isAbove(intVec point) {
			return dot(point, this->normVector) > this->heightOfPlane;
		}

		/*
			OCTAGON
		*/

		_OctagonImpl::_OctagonImpl(std::array<intVec, 8> corners)
		{
			intVec start = corners[0];
			intVec end = corners[7];
			intVec last = corners[5];
			for (int i = 0; i < 6; i++) {
				intVec next = corners[this->cornerOrder[i]];
				this->addTetra(i, { start, end, last, next });
				last = next;
			}
		}

		void _OctagonImpl::addTetra(int index, std::array<intVec,4> corners)
		{
			this->tetras[index][0] = computeNorm(corners[0], corners[1], corners[2]);
			this->tetras[index][1] = computeNorm(corners[0], corners[2], corners[3]);
			this->tetras[index][2] = computeNorm(corners[0], corners[3], corners[1]);
			this->tetras[index][3] = computeNorm(corners[1], corners[3], corners[2]);
		}

		bool contains(_OctagonImpl oi, intVec point)
		{
			for (int t_i = 0; t_i < 6; t_i++) {
				bool contains = oi.tetras[t_i][3].isAbove(point);
				for (int n_i = 0; contains & (n_i < 3); n_i++) {
					contains &= oi.tetras[t_i][n_i].isAboveOrEqual(point);
				}
				if (contains) {
					return true;
				}
			}
			return false;
		}

	} //namespace __detail


	// These are declared here because _Octagon_Impl is an imcomplete type in the header.
	Octagon::Octagon() = default;
	Octagon::~Octagon() = default;
	Octagon::Octagon(Octagon && o) = default;

	Octagon::Octagon(const std::array<Vec3d, 8> & vertices)
		: oi(std::make_unique<__detail::_OctagonImpl>(__detail::integerizedArray(vertices))) {}

	bool Octagon::contains(const Vec3d & p) const
	{
		if (!oi)
			throw std::runtime_error("contains() on empty octagon");
		return __detail::contains(*oi, __detail::integerized(p));
	}

	bool Octagon::contains(double x, double y, double z) const
	{
		return contains({ {x, y, z} });
	}

	void Octagon::operator=(Octagon o)
	{
		swap(*this, o);
	}

	void swap(Octagon & a, Octagon & b) {
		std::swap(a.oi, b.oi);
	}

}