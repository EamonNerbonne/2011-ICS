#pragma once
#include <Eigen/Core>
#include "RotationModel.h"

class ParticleBasedRotation : public RotationModel {
	Eigen::Vector3d const dimensions;
	double part_mass;
	double timestep;
	double energy_scale;
	double k;
	Matrix38d position;
	Matrix38d velocity;
	Matrix<double,8,8> baseLen;

	void resetWithAngularMomentum(Eigen::Vector3d angular_momentum);

public:
	virtual Matrix38d getPositions() {return position;}
	virtual Vector3d getDimensions() {return dimensions;}
	virtual void processInput(char c);
	virtual void faster();
	virtual void slower();
	virtual void updateStep();
	virtual void jiggle();
	ParticleBasedRotation(Eigen::Vector3d dimensions, Eigen::Vector3d angular_momentum, double timestep);
	virtual ~ParticleBasedRotation();
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

