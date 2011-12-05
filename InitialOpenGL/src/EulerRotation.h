#pragma once
#include <Eigen/Core>
#include "RotationModel.h"

class EulerRotation : public RotationModel {
	Eigen::Vector3d const dimensions;
	Eigen::Vector3d angular_momentum;
	Eigen::Matrix3d MoI_body, MoI_body_inv;
	Eigen::Matrix3d orientation;
	double timestep;
public:
	virtual Eigen::Matrix3d getOrientation();
	virtual Eigen::Vector3d getDimensions() {return dimensions;}
	virtual void processInput(char c);
	virtual void faster();
	virtual void slower();
	virtual void updateStep();
	virtual void jiggle();
	EulerRotation(Eigen::Vector3d dimensions, Eigen::Vector3d angular_momentum);
	virtual ~EulerRotation();
};
