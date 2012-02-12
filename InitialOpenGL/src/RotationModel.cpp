#include "RotationModel.h"

RotationModel::~RotationModel(){}

RotationModel::RotationModel(Vector3d dimensions) 
	: dimensions(dimensions)
	, normalize(false)
	{ }
