#include "state_machine/StateMachine.h"

#include "mocks/IStateMock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class StateMachineTests : public testing::Test
{
public:
};

using namespace sm;
using testing::Expectation;
using testing::Return;

TEST_F(StateMachineTests, construct_requiresentry)
{
	EXPECT_ANY_THROW(StateMachine({}));
}

TEST_F(StateMachineTests, onentry_calledonconstructor)
{
	auto state_mock = std::make_shared<IStateMock>();
	EXPECT_CALL(*state_mock, OnEntry()).Times(1);
	StateMachine sm(state_mock);
}

TEST_F(StateMachineTests, runstate_callsentrystate)
{
	auto        state_mock           = std::make_shared<IStateMock>();
	Expectation state1onentry_expect = EXPECT_CALL(*state_mock, OnEntry()).Times(1);
	EXPECT_CALL(*state_mock, RunState()).After(state1onentry_expect);
	StateMachine sm(state_mock);
	sm.RunStateMachine();
}

TEST_F(StateMachineTests, onentry_singlecallenforced)
{
	auto        state_mock           = std::make_shared<IStateMock>();
	Expectation state1onentry_expect = EXPECT_CALL(*state_mock, OnEntry()).Times(1);
	EXPECT_CALL(*state_mock, RunState()).Times(4).After(state1onentry_expect);
	StateMachine sm(state_mock);
	sm.RunStateMachine();
	sm.RunStateMachine();
	sm.RunStateMachine();
	sm.RunStateMachine();
}

TEST_F(StateMachineTests, runstate_movestonewstate)
{
	auto state1_mock = std::make_shared<IStateMock>();
	auto state2_mock = std::make_shared<IStateMock>();
	Edge state1_out_edge{"state1_to_state2", state2_mock};

	Expectation state1onentry_expect = EXPECT_CALL(*state1_mock, OnEntry());
	Expectation state2onentry_expect = EXPECT_CALL(*state2_mock, OnEntry());

	EXPECT_CALL(*state1_mock, RunState())
	    .After(state1onentry_expect)
	    .WillOnce([&state1_out_edge]() { return &state1_out_edge; });
	EXPECT_CALL(*state2_mock, RunState()).After(state2onentry_expect);

	StateMachine sm(state1_mock);
	sm.RunStateMachine();
	sm.RunStateMachine();
}

TEST_F(StateMachineTests, registernotification_canregister)
{
	auto state1_mock = std::make_shared<IStateMock>();
	auto state2_mock = std::make_shared<IStateMock>();
	Edge state1_out_edge{"state1_to_state2", state2_mock};

	Expectation state1onentry_expect = EXPECT_CALL(*state1_mock, OnEntry());

	StateMachine sm(state1_mock);

	EXPECT_TRUE(
	    sm.RegisterStateChangeNotification([](const IState&, const char*, const IState&) {}));
}

TEST_F(StateMachineTests, registernotification_runstate_statechanges_observechange)
{

	auto state1_mock = std::make_shared<IStateMock>();
	auto state2_mock = std::make_shared<IStateMock>();
	Edge state1_out_edge{"state1_to_state2", state2_mock};

	Expectation state1onentry_expect = EXPECT_CALL(*state1_mock, OnEntry());
	Expectation state2onentry_expect = EXPECT_CALL(*state2_mock, OnEntry());

	EXPECT_CALL(*state1_mock, RunState())
	    .After(state1onentry_expect)
	    .WillOnce([&state1_out_edge]() { return &state1_out_edge; });

	StateMachine sm(state1_mock);

	const IState* reported_from{nullptr};
	const char*   reported_via_name{nullptr};
	const IState* reported_to{nullptr};
	int           edges_reported = 0;

	sm.RegisterStateChangeNotification(
	    [&](const IState& from, const char* edge_name, const IState& to)
	    {
		    reported_from     = &from;
		    reported_via_name = edge_name;
		    reported_to       = &to;
		    ++edges_reported;
	    });

	sm.RunStateMachine();

	EXPECT_EQ(1, edges_reported);
	EXPECT_EQ(state1_mock.get(), reported_from);
	EXPECT_STREQ("state1_to_state2", reported_via_name);
	EXPECT_EQ(state2_mock.get(), reported_to);
}

TEST_F(StateMachineTests, registernotification_runstate_stateselftransitions_observe)
{
	auto state1_mock = std::make_shared<IStateMock>();
	auto state2_mock = std::make_shared<IStateMock>();
	Edge state1_out_edge{"state1_to_state1", {}};

	EXPECT_CALL(*state1_mock, OnEntry()).Times(2);
	

	EXPECT_CALL(*state1_mock, RunState())
	    .WillOnce([&state1_out_edge]() { return &state1_out_edge; });

	StateMachine sm(state1_mock);

	const IState* reported_from{nullptr};
	const char*   reported_via_name{nullptr};
	const IState* reported_to{nullptr};
	int           edges_reported = 0;

	sm.RegisterStateChangeNotification(
	    [&](const IState& from, const char* edge_name, const IState& to)
	    {
		    reported_from     = &from;
		    reported_via_name = edge_name;
		    reported_to       = &to;
		    ++edges_reported;
	    });

	sm.RunStateMachine();

	EXPECT_EQ(1, edges_reported);
	EXPECT_EQ(state1_mock.get(), reported_from);
	EXPECT_STREQ("state1_to_state1", reported_via_name);
	EXPECT_EQ(state1_mock.get(), reported_to);
}
