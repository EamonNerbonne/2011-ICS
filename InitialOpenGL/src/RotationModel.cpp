#include "RotationModel.h"

RotationModel::~RotationModel(){}

RotationModel::RotationModel(Vector3d dimensions, double timestep) 
	: dimensions(dimensions)
	, timestep(timestep)	
	, normalize(false)
	{ }
