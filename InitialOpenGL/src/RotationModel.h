#pragma once
#include <Eigen/Core>

class RotationModel {
public:
	virtual Eigen::Matrix3d getOrientation()=0;
	virtual Eigen::Vector3d getDimensions()=0;
	virtual void processInput(char c)=0;
	virtual void faster()=0;
	virtual void slower()=0;
	virtual void jiggle()=0;
	virtual void updateStep()=0;
	virtual ~RotationModel();
};
