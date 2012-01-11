#pragma once
#include <Eigen/Core>
#include "RotationModel.h"

class EulerRotation : public RotationModel {
	Vector3d const dimensions;
	Vector3d angular_momentum;
	Matrix3d MoI_body, MoI_body_inv;
	Matrix3d orientation;
	double timestep;
public:
	virtual Matrix38d getPositions();
	virtual Vector3d getDimensions() {return dimensions;}
	virtual void processInput(char c);
	virtual void faster();
	virtual void slower();
	virtual void updateStep();
	virtual void jiggle();
	EulerRotation(Eigen::Vector3d dimensions, Eigen::Vector3d angular_momentum, double timestep);
	virtual ~EulerRotation();
};
