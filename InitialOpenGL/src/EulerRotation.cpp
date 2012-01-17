#include "EulerRotation.h"
#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/SVD>
#include "matrixUtils.h"

#include <iostream>

using namespace Eigen;


EulerRotation::EulerRotation(Vector3d dimensions, Vector3d angular_momentum, double timestep)
	: RotationModel(dimensions, timestep)
	, angular_momentum(angular_momentum)
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

	auto svd = orientation.jacobiSvd(ComputeFullU | ComputeFullV);
	orientation = svd.matrixU() * svd.matrixV().transpose();
}
void EulerRotation::processInput(char c) {
	switch(c) {
	case '1':
		orientation = Matrix3d::Identity();
		angular_momentum = Vector3d(1.0,0.0,0.0);
		break;
	case '2':
		orientation = Matrix3d::Identity();
		angular_momentum= Vector3d(0.0,0.5,0.0);
		break;
	case '3':
		orientation = Matrix3d::Identity();
		angular_momentum= Vector3d(0.0,0.0,1.0);
		break;
	}
}

Matrix38d EulerRotation::getPositions() {return orientation*initialPosition(dimensions);}
EulerRotation::~EulerRotation() { }

void EulerRotation::jiggle() { angular_momentum += Vector3d::Random() * 0.01;}
