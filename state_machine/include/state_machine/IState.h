#ifndef STATE_MACHINE_ISTATE_H
#define STATE_MACHINE_ISTATE_H

#include <memory>
#include <functional>

namespace sm
{

class IState; 

struct Edge
{
	const char* transition_name;
	std::shared_ptr<IState> next_state;
};

class IState
{
public:
	virtual ~IState(){};
	
	/**
	 * @brief run the state
	 * 
	 * @return const Edge* - an edge to next state, null if none
	 */
	virtual const Edge* RunState() = 0;

	/**
	 * @brief Executed upon entering or re-entering state
	 * 
	 */
	virtual void OnEntry() = 0;

	/**
	 * @brief The Name of the state
	 * 
	 * @return const char* the name of the state
	 */
	virtual const char* Name() const = 0; 
};

} // namespace sm

#endif //STATE_MACHINE_STATE_H