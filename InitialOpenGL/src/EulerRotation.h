#pragma once
#include <Eigen/Core>
#include "RotationModel.h"

class EulerRotation : public RotationModel {
	Vector3d angular_momentum;
	Matrix3d MoI_body, MoI_body_inv;
	Matrix3d orientation;
public:

	virtual void resetWithAngularMomentum(Vector3d angular_momentum);
	virtual Matrix38d getPositions();
	virtual void updateStep(double timestep);
	virtual void jiggle();
	EulerRotation(Eigen::Vector3d dimensions);
	virtual ~EulerRotation();
};
