#pragma once
#include <Eigen/Core>

class ParticleBasedRotation {
	Eigen::Vector3d const dimensions;
	double part_mass;
	double timestep;
	double k;
	Eigen::MatrixXd position;
	Eigen::MatrixXd velocity;
	Eigen::MatrixXd baseLen;

	void resetWithAngularMomentum(Eigen::Vector3d angular_momentum);

public:
	virtual void processInput(char c);
	virtual void faster();
	virtual void slower();
	virtual void updateStep();
	virtual void jiggle();
	ParticleBasedRotation(Eigen::Vector3d dimensions, Eigen::Vector3d angular_momentum, double timestep);
	virtual ~ParticleBasedRotation();
};

