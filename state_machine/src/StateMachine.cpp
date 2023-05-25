#include "state_machine/StateMachine.h"

#include <stdexcept>

namespace sm
{

StateMachine::StateMachine(std::shared_ptr<IState> entry)
    : m_state(entry)
{
	if (!entry)
	{
		throw std::invalid_argument("Entry is required to be nonempty");
	}
	m_state->OnEntry();
}

void StateMachine::RunStateMachine()
{
	auto edge = m_state->RunState();
	if (edge != nullptr) //non-null edge indicates we are taking a transition
	{
		if (edge->next_state != nullptr)
		{
			NotifyStateObservers(*m_state, edge->transition_name, *edge->next_state);
			m_state = edge->next_state;
			m_state->OnEntry();
		}
		else //Self transition 
		{
			NotifyStateObservers(*m_state, edge->transition_name, *m_state);
			m_state->OnEntry();
		}
	}
}

bool StateMachine::RegisterStateChangeNotification(StateChangeCallback callback)
{
	m_callbacks.push_back(callback);
	return true;
}

void StateMachine::NotifyStateObservers(const IState& from, const char* edge_name, const IState& to) const
{
	for(const auto& i: m_callbacks)
	{
		i(from, edge_name, to);
	}
}


} // namespace sm