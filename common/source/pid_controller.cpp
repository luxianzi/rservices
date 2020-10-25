#include "pid_controller.h"

PIDController::PIDController(double kp, double ki, double kd) :
	kp_(kp),
	ki_(ki),
	kd_(kd),
	output_limit_enable_(false),
	output_upper_limit_(0),
	output_lower_limit_(0),
	integrator_limit_enable_(false),
	integrator_upper_limit_(0),
	integrator_lower_limit_(0),
	integrator_(0) {

}

PIDController::~PIDController() {

}

double PIDController::Calculate(double target, double feed_back) {
	double error = target - feed_back;
	integrator_ += ki_ * error;
	integrator_ = integrator_limit_enable_ && \
			integrator_ > integrator_upper_limit_ ? integrator_upper_limit_ : \
			integrator_;
	integrator_ = integrator_limit_enable_ && \
			integrator_ < integrator_lower_limit_ ? integrator_lower_limit_ : \
			integrator_;
	double output = kp_ * error + integrator_ + kd_ * error;
	output = output_limit_enable_ && \
			output > output_upper_limit_ ? output_upper_limit_ : output;
	output = output_limit_enable_ && \
			output < output_lower_limit_ ? output_lower_limit_ : output;
	return output;
}

error_condition PIDController::SetOutputLimit(double upper_limit, \
		double lower_limit) {
	if (upper_limit <= lower_limit)
		return make_error_condition(errc::invalid_argument);
	output_limit_enable_ = true;
	output_upper_limit_ = upper_limit;
	output_lower_limit_ = lower_limit;
	return kNoError;
}

void PIDController::DisableOutputLimit() {
	output_limit_enable_ = false;
}

error_condition PIDController::SetIntegratorLimit(double upper_limit, \
		double lower_limit) {
	if (upper_limit <= lower_limit)
		return make_error_condition(errc::invalid_argument);
	integrator_limit_enable_ = true;
	integrator_upper_limit_ = upper_limit;
	integrator_lower_limit_ = lower_limit;
	return kNoError;
}

void PIDController::DisableIntegratorLimit() {
	integrator_limit_enable_ = false;
}
