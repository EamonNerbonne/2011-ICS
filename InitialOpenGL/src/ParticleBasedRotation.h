#pragma once
#include <Eigen/Core>
#include "RotationModel.h"

class ParticleBasedRotation : public RotationModel {
	double part_mass;
	double energy_scale;
	double k;
	Matrix38d position;
	Matrix38d velocity;
	Matrix<double,8,8> baseLen;

public:
	virtual void resetWithAngularMomentum(Vector3d angular_momentum);
	virtual Matrix38d getPositions() {return position;}
	virtual void updateStep();
	virtual void jiggle();
	ParticleBasedRotation(Eigen::Vector3d dimensions, Eigen::Vector3d angular_momentum, double timestep);
	virtual ~ParticleBasedRotation();
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

