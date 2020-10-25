#ifndef MAIN_SERVICE_FSM_H_
#define MAIN_SERVICE_FSM_H_

#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/stl.hpp>
#include "error_category.h"

BOOST_MSM_EUML_EVENT(EventInit)
BOOST_MSM_EUML_EVENT(EventTransit)

BOOST_MSM_EUML_ACTION(ActionAuto)
{
	template <class Fsm, class Evt, class SourceState, class TargetState>
	void operator()(Evt const&, Fsm&, SourceState&, TargetState&) {

	}
};
BOOST_MSM_EUML_ACTION(ActionManual)
{
	template <class Fsm, class Evt, class SourceState, class TargetState>
	void operator()(Evt const&, Fsm&, SourceState&, TargetState&) {

	}
};

#endif /* MAIN_SERVICE_FSM_H_ */
