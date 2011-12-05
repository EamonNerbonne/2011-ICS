#pragma once
#include <Eigen/Core>

struct RotationState {
	Eigen::Vector3d const dimensions;
	Eigen::Vector3d angular_momentum;
	Eigen::Matrix3d MoI_body, MoI_body_inv;
	Eigen::Matrix3d orientation;
	double timestep;

	RotationState(Eigen::Vector3d dimensions, Eigen::Vector3d angular_momentum);
	void updateStepEuler();
};
