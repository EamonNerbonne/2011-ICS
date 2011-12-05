#include "RotationState.h"
#include <Eigen/Core>
#include <Eigen/SVD>
#include "matrixUtils.h"

using namespace Eigen;


RotationState::RotationState(Vector3d dimensions, Vector3d angular_momentum)
: dimensions(dimensions)
, angular_momentum(angular_momentum)
, orientation(Matrix3d::Identity())
, timestep(0.0003)
{
	Vector3d diagOf_MoI=
			Vector3d(
					dimensions(1)*dimensions(1) + dimensions(2)*dimensions(2),
					dimensions(0)*dimensions(0) + dimensions(2)*dimensions(2),
					dimensions(1)*dimensions(1) + dimensions(0)*dimensions(0)
			);
	MoI_body = diagOf_MoI.asDiagonal(); //TODO:we're ignoring density
	MoI_body_inv = diagOf_MoI.array().inverse().matrix().asDiagonal();
}

void RotationState::updateStepEuler() {
	Matrix3d I_inv = orientation * MoI_body_inv * orientation.transpose();
	Vector3d omega = I_inv * angular_momentum;
	orientation += timestep * asTensor(omega) * orientation;

	auto svd = orientation.jacobiSvd(ComputeFullU | ComputeFullV);
	orientation = svd.matrixU() * svd.matrixV().transpose();
}

