#include "state_machine/CallableState.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class CallableStateTests : public ::testing::Test
{
public:
	MOCK_METHOD(sm::Edge*, MockCallable, ());
};

using namespace sm;
using ::testing::Return;

TEST_F(CallableStateTests, construct_throwsonnullcallable)
{
	EXPECT_ANY_THROW(sm::CallableState({}, {}));
}

TEST_F(CallableStateTests, runstate_executescallable)
{
	EXPECT_CALL(*this, MockCallable());
	auto          target = [&]() { return MockCallable(); };
	CallableState e(target, "foo");
	e.RunState();
}

TEST_F(CallableStateTests, runstate_forwardscallablereturn)
{
	Edge edge{"state1_to_state2", nullptr};

	EXPECT_CALL(*this, MockCallable()).WillOnce([&edge]() { return &edge; });
	auto          target = [&]() { return MockCallable(); };
	CallableState e(target, "bar");
	auto          returned_edge = e.RunState();
	EXPECT_EQ(returned_edge, &edge);
}

TEST_F(CallableStateTests, onentry_overridden)
{
	auto          target = [&]() { return MockCallable(); };
	CallableState e(target, "biz");
	e.OnEntry();
}

TEST_F(CallableStateTests, name_returnsprovided)
{
	auto          target = [&]() { return MockCallable(); };
	CallableState e(target, "biz");
	EXPECT_STREQ("biz", e.Name());
}
