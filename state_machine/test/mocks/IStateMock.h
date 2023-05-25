#ifndef STATE_MACHINE_I_STATE_MOCK_H
#define STATE_MACHINE_I_STATE_MOCK_H

#include "state_machine/IState.h"

#include <gmock/gmock.h>

namespace sm
{
class IStateMock : public IState
{
public:
	MOCK_METHOD(const Edge*, RunState, ());
	MOCK_METHOD(void, OnEntry, ());
	MOCK_METHOD(const char*, Name, (), (const));
};

} //end namespace sm

#endif //STATE_MACHINE_I_STATE_MOCK_H