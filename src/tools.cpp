#include "tools.h"
#include <iostream>

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
   // TODO: Calculate the RMSE here.
	VectorXd rmse(ground_truth.size());
	rmse.setZero(ground_truth.size());
	if (estimations.size() != ground_truth.size()
		|| estimations.size() == 0) {
		std::cout << "Invalid estimation or ground_truth data" << std::endl;
		return rmse;
	}
	// accumulate squared residuals
	for (unsigned int i = 0; i < estimations.size(); ++i) {

		VectorXd residual = estimations[i] - ground_truth[i];

		// coefficient-wise multiplication
		residual = residual.array()*residual.array();
		rmse += residual;
	}

	// calculate the mean
	rmse = rmse / ground_truth.size();

	// calculate the squared root
	rmse = rmse.array().sqrt();

	// return the result
	return rmse;
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
   // TODO: Calculate a Jacobian here.
	MatrixXd Hj(3, 4);
	float px = x_state(0);
	float py = x_state(1);
	float vx = x_state(2);
	float vy = x_state(3);

	float c1 = px*px + py*py;
	float c2 = sqrt(c1);
	float c3 = (c1*c2);

	// Checking for division by zero
	if (fabs(c1) < 0.0001) {
		std::cout << "CalculateJacobian () - Error - Division by Zero" << std::endl;
		return Hj;
	}
	// compute the Jacobian matrix
	Hj << (px / c2), (py / c2), 0, 0,
		-(py / c1), (px / c1), 0, 0,
		py*(vx*py - vy*px) / c3, px*(px*vy - py*vx) / c3, px / c2, py / c2;

	return Hj;
}
