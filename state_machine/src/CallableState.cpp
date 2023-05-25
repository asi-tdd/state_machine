#include "state_machine/CallableState.h"

#include <stdexcept>
#include <iostream>
namespace sm
{
CallableState::CallableState(std::function<const Edge*()> callable, std::string name)
    : m_target(std::move(callable))
	, m_name(std::move(name))
{
	if (!m_target)
	{
		throw std::invalid_argument("CallableState: callable can't be null");
	}
}

const Edge* CallableState::RunState()
{
	return m_target();
}

void CallableState::OnEntry()
{
	std::cout << "CallableState: On Entry Called\n";
}

const char* CallableState::Name() const
{
	return m_name.c_str();
}

} // namespace sm