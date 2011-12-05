#include <Eigen/Core>

inline Eigen::Matrix4d asAffine(Eigen::Matrix3d mat) {
	Eigen::Matrix4d retval = Eigen::Matrix4d::Identity();
	retval.topLeftCorner(3,3) = mat;
	return retval;
}

inline Eigen::Matrix3d asTensor(Eigen::Vector3d tensor) {
	Eigen::Matrix3d retval = Eigen::Matrix3d::Zero();
	retval(1,0) = tensor(2);
	retval(2,0) = -tensor(1);
	retval(0,1) = -tensor(2);
	retval(2,1) = tensor(0);
	retval(0,2) = tensor(1);
	retval(1,2) = -tensor(0);
	return retval;
}
