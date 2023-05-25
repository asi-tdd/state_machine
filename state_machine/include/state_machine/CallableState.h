#ifndef STATE_MACHINE_CALLABLE_STATE_H
#define STATE_MACHINE_CALLABLE_STATE_H

#include "state_machine/IState.h"

#include <functional>
#include <string>

namespace sm
{
class CallableState : public IState
{
public:
	CallableState(std::function<const Edge*()> callable, std::string name);

	const Edge* RunState() override;

	void OnEntry() override;

	const char* Name() const override;

private:
	std::function<const Edge*()> m_target;
	std::string m_name;
};
} // namespace sm

#endif //STATE_MACHINE_CALLABLE_STATE_H