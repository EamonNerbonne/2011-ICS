#pragma once
#include <Eigen/Core>
#include "RotationModel.h"

class EulerRotation : public RotationModel {
	Vector3d angular_momentum;
	Matrix3d MoI_body, MoI_body_inv;
	Matrix3d orientation;
public:
	virtual Matrix38d getPositions();
	virtual void processInput(char c);
	virtual void updateStep();
	virtual void jiggle();
	EulerRotation(Eigen::Vector3d dimensions, Eigen::Vector3d angular_momentum, double timestep);
	virtual ~EulerRotation();
};
