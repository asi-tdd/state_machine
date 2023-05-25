#ifndef STATE_MACHINE_STATE_MACHINE_H
#define STATE_MACHINE_STATE_MACHINE_H

#include "IState.h"

#include <functional>
#include <memory>
#include <vector>

namespace sm
{

class StateMachine
{
public:
	/**
	 * @brief Construct a State Machine object.
	 * 
	 * @param entry - initial state for state machine
	 */
	StateMachine(std::shared_ptr<IState> entry);

	/**
	 * @brief Run the state machine
	 */
	void RunStateMachine();


	/**
	 * Alias for a callback to observe state changes - note that callback occurs before OnEntry for future state
	 */
	using StateChangeCallback = std::function<void(const IState& /*from*/, const char* /*edge name*/, const IState& /*to*/)>;

	/**
	 * @brief Register a callback to observe state changes
	 * 
	 * @param callback - method to call on state change
	 * @return true registration succeeded
	 * @return false registration failed
	 */
	bool RegisterStateChangeNotification(StateChangeCallback callback);

private:
	std::shared_ptr<IState> m_state;
	std::vector <StateChangeCallback> m_callbacks;

	void NotifyStateObservers(const IState& from, const char* edge_name, const IState& to) const;
};

} // namespace sm

#endif //STATE_MACHINE_STATE_MACHINE_H
