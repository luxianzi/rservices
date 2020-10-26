#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

#include "error_category.h"

class PIDController {
public:
	PIDController(double kp, double ki, double kd);
	virtual ~PIDController();

	virtual double Calculate(double target, double feed_back);
	virtual error_condition SetOutputLimit(double upper_limit, double lower_limit);
	virtual void DisableOutputLimit();
	virtual error_condition SetIntegratorLimit(double upper_limit, double lower_limit);
	virtual void DisableIntegratorLimit();

private:
	double kp_;
	double ki_;
	double kd_;
	bool output_limit_enable_;
	double output_upper_limit_;
	double output_lower_limit_;
	bool integrator_limit_enable_;
	double integrator_upper_limit_;
	double integrator_lower_limit_;
	double integrator_;
};

#endif /* PID_CONTROLLER_H_ */
