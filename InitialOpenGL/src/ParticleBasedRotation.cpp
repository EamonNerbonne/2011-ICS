#include "ParticleBasedRotation.h"
#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/Geometry>
#include <Eigen/SVD>
#include "matrixUtils.h"
#include <iostream>

using namespace Eigen;

ParticleBasedRotation::ParticleBasedRotation(Vector3d dimensions, Vector3d angular_momentum, double timestep)
	: RotationModel(dimensions, timestep)
{
	resetWithAngularMomentum(angular_momentum);
	k = 10.0;
	part_mass = 1.0;
}
typedef Matrix<double,8,8> Matrix88d;

Matrix88d computePairwiseDistances(Matrix38d const & position) {
	Matrix88d dist= Matrix88d::Zero();
	for(ptrdiff_t i=0; i<position.cols(); ++i)
		for(ptrdiff_t j=i+1; j<position.cols(); ++j)
			dist(i,j) = (position.col(i) - position.col(j)).norm();
	dist.triangularView<StrictlyLower>() = dist.triangularView<StrictlyUpper>().transpose();
	return dist;
}

void ParticleBasedRotation::resetWithAngularMomentum(Vector3d angular_momentum) {
	position = initialPosition(dimensions);
	baseLen = computePairwiseDistances(position);

	Vector3d diagOf_MoI=
			Vector3d(
					dimensions(1)*dimensions(1) + dimensions(2)*dimensions(2),
					dimensions(0)*dimensions(0) + dimensions(2)*dimensions(2),
					dimensions(1)*dimensions(1) + dimensions(0)*dimensions(0)
			);
	//Matrix3d MoI_body = diagOf_MoI.asDiagonal(); //TODO:we're ignoring density
	Matrix3d MoI_body_inv = diagOf_MoI.array().inverse().matrix().asDiagonal();

	//assume orientation is the identity; compute angular velocity:
	Vector3d omega = MoI_body_inv * angular_momentum;
	Matrix3d angular_velocity_tensor = asTensor(omega);

	velocity = angular_velocity_tensor * position;
	energy_scale = velocity.norm();
}


ParticleBasedRotation::~ParticleBasedRotation() {}

void ParticleBasedRotation::updateStep() {
	Matrix38d accel = Matrix38d::Zero();

	for(ptrdiff_t i=0;i<position.cols(); ++i) {
		for(ptrdiff_t j=i+1;j<position.cols(); ++j) {
			Vector3d JtoI = position.col(i) - position.col(j);
			Vector3d onI = k / part_mass * (baseLen(i,j) / JtoI.norm() - 1) * JtoI;
			accel.col(i) += onI;
			accel.col(j) -= onI;
		}
	}

	position.noalias() += timestep * velocity;

	velocity.noalias() += timestep * accel;
	velocity *= energy_scale/velocity.norm();
}


void ParticleBasedRotation::jiggle() {
	Vector3d randomRotVector = Vector3d::Random() * 0.01;
	Matrix3d randomRot;
	randomRot = AngleAxisd(randomRotVector(0), Vector3d::UnitZ())
					  * AngleAxisd(randomRotVector(0), Vector3d::UnitY())
					  * AngleAxisd(randomRotVector(0), Vector3d::UnitZ());

	velocity = randomRot * velocity;
}

