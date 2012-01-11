#include "QuaternionRotation.h"
#include <Eigen/Core>
#include <Eigen/SVD>
#include "matrixUtils.h"
#include <iostream>

using namespace Eigen;

QuaternionRotation::QuaternionRotation(Vector3d dimensions, Vector3d angular_momentum, double timestep)
: dimensions(dimensions)
, angular_momentum(angular_momentum)
, orientation(Quaternion<double>::Identity())
, timestep(timestep)
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

//	std::cout << orientation.squaredNorm()<< ", "<< orientation.matrix().squaredNorm() << ", "<< orientation.matrix().determinant() << "\n";
	orientation.normalize();
}
void QuaternionRotation::processInput(char c) {
	switch(c) {
	case '1':
		orientation = Quaterniond::Identity();
		angular_momentum = Vector3d(1.0,0.0,0.0);
		break;
	case '2':
		orientation = Quaterniond::Identity();
		angular_momentum= Vector3d(0.0,0.5,0.0);
		break;
	case '3':
		orientation = Quaterniond::Identity();
		angular_momentum= Vector3d(0.0,0.0,1.0);
		break;
	}
}


Matrix3d QuaternionRotation::getOrientation() {return orientation.matrix();}
void QuaternionRotation::faster() { timestep *= 1.2; }
void QuaternionRotation::slower() { timestep /=1.2; }
QuaternionRotation::~QuaternionRotation() { }

void QuaternionRotation::jiggle() { angular_momentum += Vector3d::Random() * 0.01;}




