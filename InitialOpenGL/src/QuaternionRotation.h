#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "RotationModel.h"

class QuaternionRotation : public RotationModel {
	Vector3d angular_momentum;
	Matrix3d MoI_body, MoI_body_inv;
	Quaternion<double> orientation;
public:
	virtual void resetWithAngularMomentum(Vector3d angular_momentum);

	virtual Matrix38d getPositions();
	virtual void updateStep(double timestep);
	virtual void jiggle();
	virtual double energy();
	QuaternionRotation(Eigen::Vector3d dimensions);
	virtual ~QuaternionRotation();

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
