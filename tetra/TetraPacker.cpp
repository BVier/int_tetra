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
			TETRA
		*/
		Tetra::Tetra(std::array<intVec, 4> corners)
		{
			this->norms[0] = computeNorm(corners[0], corners[1], corners[2]);
			this->norms[1] = computeNorm(corners[0], corners[2], corners[3]);
			this->norms[2] = computeNorm(corners[0], corners[3], corners[1]);
			this->norms[3] = computeNorm(corners[1], corners[3], corners[2]);

			this->valid = true;
		}

		bool Tetra::contains(intVec p) {
			if (!this->valid) {
				return false;
			}

			bool contains = true;
			for (int i = 0; i < 3 && contains; i++) {
				contains &= this->norms[i].isAboveOrEqual(p);
			}
			contains &= this->norms[3].isAbove(p);
			return contains;
		}


		/*
			TETRAPACKER
		*/

		_OctagonImpl::_OctagonImpl(std::array<intVec, 8> corners)
		{
			intVec start = corners[0];
			intVec end = corners[7];
			intVec last = corners[5];
			for (int i = 0; i < 6; i++) {
				intVec next = corners[this->cornerOrder[i]];
				std::array<intVec, 4> tetraCorners = { start, end, last, next };
				this->tetras[i] = Tetra(tetraCorners);
				last = next;
			}
		}

		bool contains(_OctagonImpl oi, intVec point)
		{
			for (Tetra tetra : oi.tetras) {
				if (tetra.contains(point)) {
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