#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "RotationModel.h"

class QuaternionRotation : public RotationModel {
	Vector3d angular_momentum;
	Matrix3d MoI_body, MoI_body_inv;
	Quaternion<double> orientation;
public:
	virtual Matrix38d getPositions();
	virtual void processInput(char c);
	virtual void updateStep();
	virtual void jiggle();
	QuaternionRotation(Eigen::Vector3d dimensions, Eigen::Vector3d angular_momentum, double timestep);
	virtual ~QuaternionRotation();

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
