#include "EulerRotation.h"
#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/SVD>
#include "matrixUtils.h"

#include <iostream>

using namespace Eigen;

EulerRotation::EulerRotation(Vector3d dimensions, double timestep)
	: RotationModel(dimensions, timestep)
	, orientation(Matrix3d::Identity())
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

void EulerRotation::updateStep() {
	Matrix3d I_inv = orientation * MoI_body_inv * orientation.transpose();
	Vector3d omega = I_inv * angular_momentum;
	orientation += timestep * asTensor(omega) * orientation;

	//std::cout << orientation.squaredNorm() << ", "<< orientation.determinant() << "\n";

	if(normalize) {
		auto svd = orientation.jacobiSvd(ComputeFullU | ComputeFullV);
		orientation = svd.matrixU() * svd.matrixV().transpose();
	}
}

void EulerRotation::resetWithAngularMomentum(Vector3d angular_momentum) {
	orientation = Matrix3d::Identity();
	this->angular_momentum = angular_momentum;
}


Matrix38d EulerRotation::getPositions() {return orientation*initialPosition(dimensions);}
EulerRotation::~EulerRotation() { }

void EulerRotation::jiggle() { angular_momentum += Vector3d::Random() * 0.01;}
