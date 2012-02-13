#include "ParticleBasedRotation.h"
#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/Geometry>
#include <Eigen/SVD>
#include "matrixUtils.h"
#include <iostream>

using namespace Eigen;

ParticleBasedRotation::ParticleBasedRotation(Vector3d dimensions)
	: RotationModel(dimensions), damping(true)
{
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
void ParticleBasedRotation::toggleDamping() {
	damping = !damping;
	std::cout <<"Damping: Now "<< (damping?"ON":"OFF")<<"\n";
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
	totalEnergy = 0.5 * part_mass* velocity.squaredNorm();
}


ParticleBasedRotation::~ParticleBasedRotation() {}

inline static double sqr(double x ) {return x*x;}

void ParticleBasedRotation::updateStep(double timestep) {
	Matrix38d accel = Matrix38d::Zero();
	double potentialEnergy = 0.0;
	for(ptrdiff_t i=0;i<position.cols(); ++i) {
		for(ptrdiff_t j=i+1;j<position.cols(); ++j) {
			Vector3d JtoI = position.col(i) - position.col(j);
			double JtoIdist = JtoI.norm();
			potentialEnergy += 0.5*k*sqr(baseLen(i,j) - JtoIdist);
			Vector3d JtoIunit = JtoI / JtoIdist;
			Vector3d forceOnI = k / part_mass * (baseLen(i,j) - JtoIdist) * JtoIunit;
			
			if(damping) {
				Vector3d vIwrtJ = velocity.col(i) - velocity.col(j);
				Vector3d vParallelComponent = JtoIunit * JtoIunit.dot(vIwrtJ);
				Vector3d dampingForce = -2. * sqrt(k / part_mass) * vParallelComponent;
				forceOnI += dampingForce;
			}
			accel.col(i) += forceOnI;
			accel.col(j) -= forceOnI;
		}
	}

	position.noalias() += timestep * velocity;

	velocity.noalias() += timestep * accel;
	if(normalize) {
		double kineticEnergy = 0.5*part_mass*velocity.squaredNorm();
		double normalizedKineticEnergy = std::max (0.1*totalEnergy, totalEnergy - potentialEnergy); //Never remove _all_ kinetic energy to avoid corner cases when turning normalization on suddenly
		velocity *= sqrt(normalizedKineticEnergy/kineticEnergy);
		position = position.colwise() - position.rowwise().mean();
	}
}


void ParticleBasedRotation::jiggle() {
	Vector3d randomRotVector = Vector3d::Random() * 0.001;
	Matrix3d randomRot;
	randomRot = AngleAxisd(randomRotVector(0), Vector3d::UnitZ())
					  * AngleAxisd(randomRotVector(0), Vector3d::UnitY())
					  * AngleAxisd(randomRotVector(0), Vector3d::UnitZ());

	velocity = randomRot * velocity;
}

