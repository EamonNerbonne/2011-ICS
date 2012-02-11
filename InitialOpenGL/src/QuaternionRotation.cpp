#include "QuaternionRotation.h"
#include <Eigen/Core>
#include <Eigen/SVD>
#include "matrixUtils.h"
#include <iostream>

using namespace Eigen;

QuaternionRotation::QuaternionRotation(Vector3d dimensions, double timestep)
	: RotationModel(dimensions, timestep)
	, orientation(Quaternion<double>::Identity())
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

void QuaternionRotation::updateStep() {
	Matrix3d I_inv = orientation * MoI_body_inv * orientation.conjugate();
	Vector3d omega = I_inv * angular_momentum;

	auto updateStep = Quaterniond(0.0, omega(0), omega(1), omega(2)) * orientation;

	orientation.w() += 0.5 * timestep * updateStep.w();
	orientation.x() += 0.5 * timestep * updateStep.x();
	orientation.y() += 0.5 * timestep * updateStep.y();
	orientation.z() += 0.5 * timestep * updateStep.z();

	if(normalize)
		orientation.normalize();
}

void QuaternionRotation::resetWithAngularMomentum(Vector3d angular_momentum) {
	orientation = Matrix3d::Identity();
	this->angular_momentum = angular_momentum;
}


Matrix38d QuaternionRotation::getPositions() {return orientation.matrix()*initialPosition(dimensions);}
QuaternionRotation::~QuaternionRotation() { }

void QuaternionRotation::jiggle() { angular_momentum += Vector3d::Random() * 0.001;}




