#pragma once
#ifdef _MSC_VER
#pragma warning (disable:4512)
#endif

#include <Eigen/Core>
using namespace Eigen;

typedef Matrix<double, 3, 8> Matrix38d;

class RotationModel {
protected:
	Matrix38d initialPosition(Vector3d const & dimensions) {
		Vector3d scaled = 0.5 * dimensions;
		Matrix38d cubeCoords(Matrix38d::Zero().colwise() - scaled);
		Matrix3d dimOffsets = dimensions.asDiagonal();

		for(int pointI=0;pointI<8;++pointI)
			for(int dimI=0;dimI<3;++dimI)
				if(pointI & (1<<dimI))
					cubeCoords.col(pointI) += dimOffsets.col(dimI);

		return cubeCoords;
	}

	Vector3d dimensions;

	RotationModel(Vector3d dimensions);
public:
	bool normalize;
	virtual void resetWithAngularMomentum(Vector3d angular_momentum)=0;
	Vector3d getDimensions() {return dimensions;}

	virtual Matrix38d getPositions()=0;
	virtual void jiggle()=0;
	virtual double energy()=0;
	virtual void updateStep(double timestep)=0;
	virtual ~RotationModel();
	virtual void toggleDamping() {}
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
