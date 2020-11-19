#ifndef MAIN_SERVICE_FSM_H_
#define MAIN_SERVICE_FSM_H_

#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/stl.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include "error_category.h"
#include "message.h"

using namespace std;
using namespace boost::msm::front::euml;
namespace msm = boost::msm;

enum WorkMode {
	kUnknownMode = -1,
	kManualMode = 0,
	kAutoMode = 1
};

BOOST_MSM_EUML_EVENT(EventTransit)

BOOST_MSM_EUML_DECLARE_ATTRIBUTE(WorkMode, work_mode_current)
BOOST_MSM_EUML_DECLARE_ATTRIBUTE(WorkMode, work_mode_recv)
BOOST_MSM_EUML_ATTRIBUTES((attributes_ << work_mode_current << work_mode_recv), switch_mode_attributes)
BOOST_MSM_EUML_EVENT_WITH_ATTRIBUTES(EventInit, switch_mode_attributes)
BOOST_MSM_EUML_EVENT_WITH_ATTRIBUTES(EventSwitchAuto,switch_mode_attributes)
BOOST_MSM_EUML_EVENT_WITH_ATTRIBUTES(EventAutoWork, switch_mode_attributes)
BOOST_MSM_EUML_EVENT_WITH_ATTRIBUTES(EventSwitchManual, switch_mode_attributes)

BOOST_MSM_EUML_ACTION(ActionInit)
{
	template <class Fsm, class Evt, class SourceState, class TargetState>
	void operator()(Evt const&, Fsm&, SourceState&, TargetState&) {
		WorkMode work_mode = kManualMode;
		Message message;
		message.SetSource(Config::kMainServiceName);
		message.SetDestination(Config::kRemoteControlServerName);
		message.SetType(Message::kRequest);
		message.SetItem("work_mode", static_cast<int>(work_mode));
//		MainService::SendMessage(message);
	}
};
BOOST_MSM_EUML_ACTION(ActionAuto)
{
	template <class Fsm, class Evt, class SourceState, class TargetState>
	void operator()(Evt const& evt, Fsm&, SourceState&, TargetState&) {
		WorkMode work_mode = kAutoMode ;
		if (evt.get_attribute(work_mode_current) != evt.get_attribute(work_mode_recv))
		{
			Message message;
			message.SetSource(Config::kMainServiceName);
			message.SetDestination(Config::kMotionServiceName);
			message.SetType(Message::kRequest);
			// if set mode OK, send kAutoMode, else send kManualMode
			message.SetItem("work_mode", static_cast<int>(work_mode));
//			MainService::SendMessage(message);
		}
	}
};
BOOST_MSM_EUML_ACTION(ActionManual)
{
	template <class Fsm, class Evt, class SourceState, class TargetState>
	void operator()(Evt const&, Fsm&, SourceState&, TargetState&) {

	}
};

BOOST_MSM_EUML_STATE((), StateManual)
BOOST_MSM_EUML_STATE((), StateAuto)

// Transition table
BOOST_MSM_EUML_TRANSITION_TABLE((
	StateManual == StateManual  + EventInit \
		[event_(work_mode_current) == Int_<kManualMode>()]  / ActionInit,
	StateAuto   == StateManual  + EventSwitchAuto\
		[event_(work_mode_current) == Int_<kManualMode>()]  / ActionAuto,
	StateAuto   == StateAuto    + EventAutoWork\
		[event_(work_mode_current) == Int_<kAutoMode>()]    / ActionAuto,
	StateManual == StateAuto    + EventSwitchManual\
		[event_(work_mode_current) == Int_<kAutoMode>()]  / ActionManual
	), working_transition_table)

BOOST_MSM_EUML_DECLARE_STATE_MACHINE((working_transition_table,\
		init_ << StateManual\
		), working_)

typedef msm::back::state_machine<working_> WorkingFSM;

#endif /* MAIN_SERVICE_FSM_H_ */
