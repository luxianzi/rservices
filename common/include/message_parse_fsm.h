#include <iostream>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

using namespace std;
namespace msmf = boost::msm::front;
namespace msmb = boost::msm::back;
namespace mpl = boost::mpl;

struct MessageParseFSMFront:\
		msmf::state_machine_def<MessageParseFSMFront> {
	struct StateGetKey:msmf::state<> {};
	struct StateGetParameter:msmf::state<> {};
	struct StateGetData:msmf::state<> {};

	typedef StateGetKey initial_state;

	struct StateGetKeyAction {
		template<class Event, class Fsm, class SourceState, \
				class TargetState>
		void operator()(Event const&, Fsm&, SourceState&, TargetState&) const {
			cout << "StateGetKeyAction" << endl;
		}
	};
	struct StateGetParameterAction {
		template<class Event, class Fsm, class SourceState, \
				class TargetState>
		void operator()(Event const&, Fsm&, SourceState&, TargetState&) const {
			cout << "StateGetParameterAction" << endl;
		}
	};
	struct StateGetDataAction {
		template<class Event, class Fsm, class SourceState, \
				class TargetState>
		void operator()(Event const&, Fsm&, SourceState&, TargetState&) const {
			cout << "StateGetParameterAction" << endl;
		}
	};

	struct TransitionTable:mpl::vector<
		// Row content: Source, Event, Next, Action, Guard
		msmf::Row<StateGetKey, msmf::none, StateGetParameter, \
				StateGetKeyAction, msmf::none>,
		msmf::Row<StateGetParameter, msmf::none, StateGetData, \
				msmf::none, msmf::none>,
		msmf::Row<StateGetData, msmf::none, StateGetKey, \
				msmf::none, msmf::none >
	> {};
};

typedef msmb::state_machine<MessageParseFSMFront> MessageParseFSM;
