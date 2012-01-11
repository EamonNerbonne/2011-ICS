#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "RotationModel.h"

class QuaternionRotation : public RotationModel {
	Eigen::Vector3d const dimensions;
	Eigen::Vector3d angular_momentum;
	Eigen::Matrix3d MoI_body, MoI_body_inv;
	Eigen::Quaternion<double> orientation;
	double timestep;
public:
	virtual Eigen::Matrix3d getOrientation();
	virtual Eigen::Vector3d getDimensions() {return dimensions;}
	virtual void processInput(char c);
	virtual void faster();
	virtual void slower();
	virtual void updateStep();
	virtual void jiggle();
	QuaternionRotation(Eigen::Vector3d dimensions, Eigen::Vector3d angular_momentum, double timestep);
	virtual ~QuaternionRotation();
};
