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
			PLANE
		*/
		struct Plane {
			intVec normVector;
			int64_t heightOfPlane;
			Plane() {};
			Plane(std::array<intVec, 3> vecs);
			bool isAboveOrEqual(intVec v);
			bool isAbove(intVec v);
		};

		Plane::Plane(std::array<intVec, 3> vecs) {
			intVec v1 = subtract(vecs[1], vecs[0]);
			intVec v2 = subtract(vecs[0], vecs[2]);
			this->normVector = cross(v2, v1);
			this->heightOfPlane = dot(this->normVector, vecs[0]);
		}

		bool Plane::isAboveOrEqual(intVec point) {
			return dot(point, this->normVector) >= this->heightOfPlane;
		}

		bool Plane::isAbove(intVec point) {
			return dot(point, this->normVector) > this->heightOfPlane;
		}

		/*
			OCTAGON
		*/

		struct _OctagonImpl
		{
			int cornerOrder[6] = { 1, 3, 2, 6, 4, 5 };
			Plane tetras[6][4];
		public:
			_OctagonImpl(std::array<intVec, 8> corners);
			void addTetra(int index, std::array<intVec, 4> tetraCorners);
		};
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
			this->tetras[index][0] = Plane({ corners[0], corners[1], corners[2] });
			this->tetras[index][1] = Plane({ corners[0], corners[2], corners[3] });
			this->tetras[index][2] = Plane({ corners[0], corners[3], corners[1] });
			this->tetras[index][3] = Plane({ corners[1], corners[3], corners[2] });
		}

		bool contains(_OctagonImpl oi, intVec point)
		{
			// Iterate over all 6 Tetrahedrons
			for (int t_i = 0; t_i < 6; t_i++) {
				// The point must be above the last plane of the tetrahedron.
				// By not accepting points on the plane we avoid accepting a point by two domains.
				bool tetraContains = oi.tetras[t_i][3].isAbove(point);
				// Only iterate while the all 
				for (int n_i = 0; tetraContains & (n_i < 3); n_i++) {
					// For all other planes we accept also points which are exactly on the plane.
					tetraContains &= oi.tetras[t_i][n_i].isAboveOrEqual(point);
				}
				if (tetraContains) {
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