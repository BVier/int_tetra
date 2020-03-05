#include "TetraPacker.hpp"
#include <deque>

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
		
		double euclid(intVec v1) { 
			return std::sqrt( std::pow(v1[0], 2) + std::pow(v1[1], 2) + std::pow(v1[2], 2)); 
		}

		intVec integerized(Vec3d vec)
		{
			return { {int(vec[0] * precision), int(vec[1] * precision), int(vec[2] * precision)} };
		}

		/*
			PLANE
		*/
		struct Plane {
			intVec normVector;
			int64_t heightOfPlane;
			int64_t sqrt3Dist;
			Plane() {};
			Plane(std::array<intVec, 3> vecs);
			bool isAboveOrEqual(intVec v);
			bool isAbove(intVec v);
			bool toSmallDistanceToVertex(intVec v);
		};

		Plane::Plane(std::array<intVec, 3> vecs) {
			intVec v1 = subtract(vecs[1], vecs[0]);
			intVec v2 = subtract(vecs[0], vecs[2]);
			this->normVector = cross(v2, v1);
			this->heightOfPlane = dot(this->normVector, vecs[0]);
			this->sqrt3Dist = static_cast<int>(std::sqrt(3) * euclid(normVector)) - heightOfPlane;
		}

		bool Plane::isAboveOrEqual(intVec point) {
			return dot(point, this->normVector) >= this->heightOfPlane;
		}

		bool Plane::isAbove(intVec point) {
			return dot(point, this->normVector) > this->heightOfPlane;
		}

		bool Plane::toSmallDistanceToVertex(intVec point) {
			return dot(point, this->normVector) < this->sqrt3Dist;
		}

		/*
			OCTAGON
		*/

		struct _OctagonImpl
		{
			int cornerOrder[8] = { 1, 3, 2, 6, 4, 5, 0, 7 };
			Plane tetras[6][4];
			std::array<intVec, 8> integerizedArray(std::array<Vec3d, 8> corners);
			bool verify(std::array<intVec, 8> corners);
		public:
			bool valid;
			_OctagonImpl(std::array<Vec3d, 8> corners);
			void addTetra(int index, std::array<intVec, 4> tetraCorners);
		};

		_OctagonImpl::_OctagonImpl(std::array<Vec3d, 8> doubleCorners)
		{
			std::array<intVec, 8> corners = integerizedArray(doubleCorners);
			intVec start = corners[6];
			intVec end = corners[7];
			intVec last = corners[5];
			for (int i = 0; i < 6; i++) {
				intVec next = corners[i];
				this->addTetra(i, { start, end, last, next });
				last = next;
			}
			this->valid = verify(corners);
		}

		std::array<intVec, 8> _OctagonImpl::integerizedArray(std::array<Vec3d, 8> vertices)
		{
			std::array<intVec, 8> intVert = {};
			for (int i = 0; i < 8; i++) intVert[i] = integerized(vertices[this->cornerOrder[i]]);
			return intVert;
		}

		void _OctagonImpl::addTetra(int index, std::array<intVec,4> corners)
		{
			this->tetras[index][0] = Plane({ corners[1], corners[3], corners[2] });
			this->tetras[index][1] = Plane({ corners[0], corners[1], corners[2] });
			this->tetras[index][2] = Plane({ corners[0], corners[2], corners[3] });
			this->tetras[index][3] = Plane({ corners[0], corners[3], corners[1] });
		}

		bool _OctagonImpl::verify(std::array<intVec, 8> corners) {
			// Iterating over all sides of the cube.
			// A side consists of the plane with index (k,0) and (k+1,1).
			for (int k = 0; k < 6; k++) {
				// Defining the index of corners to test.
				std::deque<int> opposite_corners = { 2,3,4 };
				for (int i : opposite_corners) {
					if (tetras[k][0].toSmallDistanceToVertex(corners[i]))
						return false;
					if (tetras[(k + 1) % 6][1].toSmallDistanceToVertex(corners[i]))
						return false;
				}
				// Remove first corner-index and add the next one at the back
				// e.g: {2,3,4} -> {3,4,5}
				opposite_corners.push_back((opposite_corners.front() + 3) % 6);
				opposite_corners.pop_front();
			}

			for (int k = 0; k < 6; k++) {
				// Testing the first and last vertice. They have only one opposite side.
				if (tetras[k][0].toSmallDistanceToVertex(corners[6])) { return false; }
				if (tetras[k][1].toSmallDistanceToVertex(corners[7])) { return false; }
			}
			return true;
		}

		bool isValid(_OctagonImpl oi) {
			return oi.valid;
		}

		bool contains(_OctagonImpl oi, intVec point)
		{
			// Iterate over all 6 Tetrahedrons
			for (int t_i = 0; t_i < 6; t_i++) {
				// The point must be above the last plane of the tetrahedron.
				// By not accepting points on the plane we avoid accepting a point by two domains.
				bool tetraContains = oi.tetras[t_i][0].isAbove(point);
				// Only iterate while the all 
				for (int n_i = 1; tetraContains & (n_i < 4); n_i++) {
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
		: oi(std::make_unique<__detail::_OctagonImpl>(vertices)) {}

	bool Octagon::isValid() {
		return __detail::isValid(*oi);
	}

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