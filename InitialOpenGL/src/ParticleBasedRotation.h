#pragma once
#include <Eigen/Core>
#include "RotationModel.h"

class ParticleBasedRotation : public RotationModel {
	double part_mass;
	double totalEnergy;
	double k;
	bool damping;
	Matrix38d position;
	Matrix38d velocity;
	Matrix<double,8,8> baseLen;

public:
	virtual void resetWithAngularMomentum(Vector3d angular_momentum);
	virtual Matrix38d getPositions() {return position;}
	virtual void updateStep(double timestep);
	virtual void jiggle();
	virtual double energy();
	virtual void toggleDamping();
	ParticleBasedRotation(Eigen::Vector3d dimensions);
	virtual ~ParticleBasedRotation();
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

