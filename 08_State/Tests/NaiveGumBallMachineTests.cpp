#include "stdafx.h"
#include "NaiveGumBallMachine\GumballMachine.h"

using namespace std;
using boost::test_tools::output_test_stream;
using State = CGumballMachine::State;

BOOST_AUTO_TEST_SUITE(NaiveGumBallMachine)

struct empty_gummball_machine_
{
	output_test_stream out;
	CGumballMachine machine = CGumballMachine(out);
};

BOOST_FIXTURE_TEST_SUITE(when_empty, empty_gummball_machine_)
BOOST_AUTO_TEST_CASE(has_no_balls_and_sold_out_state)
{
	BOOST_REQUIRE_EQUAL(machine.GetBallCount(), 0);
	BOOST_REQUIRE(machine.GetCurrentState() == State::SoldOut);
}
BOOST_AUTO_TEST_CASE(no_shopper_actions_are_valid)
{
	BOOST_REQUIRE(!machine.EjectQuarter());
	BOOST_REQUIRE(!machine.InsertQuarter());
	BOOST_REQUIRE(!machine.TurnCrank());
	BOOST_REQUIRE(machine.GetCurrentState() == State::SoldOut);
}
BOOST_AUTO_TEST_SUITE_END()

struct non_empty_gumball_machine_
{
	output_test_stream out;
	CGumballMachine machine = CGumballMachine(out, 2);
};

BOOST_FIXTURE_TEST_SUITE(when_not_empty, non_empty_gumball_machine_)
BOOST_AUTO_TEST_CASE(has_balls_and_no_quarter_state)
{
	BOOST_REQUIRE_EQUAL(machine.GetBallCount(), 2);
	BOOST_REQUIRE(machine.GetCurrentState() == State::NoQuarter);
}
BOOST_AUTO_TEST_CASE(quarter_can_be_inserted)
{
	BOOST_REQUIRE(machine.InsertQuarter());
	BOOST_REQUIRE(machine.GetCurrentState() == State::HasQuarter);
}
BOOST_AUTO_TEST_CASE(quarter_cannot_be_ejected)
{
	BOOST_REQUIRE(!machine.EjectQuarter());
	BOOST_REQUIRE(machine.GetCurrentState() == State::NoQuarter);
}
BOOST_AUTO_TEST_CASE(crunk_cannot_be_turned)
{
	BOOST_REQUIRE(!machine.TurnCrank());
	BOOST_REQUIRE(machine.GetCurrentState() == State::NoQuarter);
}
BOOST_AUTO_TEST_SUITE_END()

struct non_empty_gumball_machine_with_quarter_ : non_empty_gumball_machine_
{
	non_empty_gumball_machine_with_quarter_()
	{
		machine.InsertQuarter();
	}
};
BOOST_FIXTURE_TEST_SUITE(when_insert_quarter_in_not_empty_machine, non_empty_gumball_machine_with_quarter_)
BOOST_AUTO_TEST_CASE(ball_count_is_the_same_and_state_changed_to_has_quarter)
{
	BOOST_REQUIRE_EQUAL(machine.GetBallCount(), 2);
	BOOST_REQUIRE(machine.GetCurrentState() == State::HasQuarter);
}
BOOST_AUTO_TEST_CASE(quarter_can_be_ejected)
{
	BOOST_REQUIRE(machine.EjectQuarter());
	BOOST_REQUIRE(machine.GetCurrentState() == State::NoQuarter);
}
BOOST_AUTO_TEST_CASE(quarter_cannot_be_inserted_again)
{
	BOOST_REQUIRE(!machine.InsertQuarter());
	BOOST_REQUIRE(machine.GetCurrentState() == State::HasQuarter);
}
BOOST_AUTO_TEST_CASE(crank_can_be_turned)
{
	BOOST_REQUIRE(machine.TurnCrank());
	BOOST_REQUIRE_EQUAL(machine.GetBallCount(), 1);
	BOOST_REQUIRE(machine.GetCurrentState() == State::NoQuarter);
}
BOOST_AUTO_TEST_SUITE_END()

struct last_ball_sold_ : non_empty_gumball_machine_
{
	last_ball_sold_()
	{
		while (machine.GetBallCount() != 0)
		{
			machine.InsertQuarter();
			machine.TurnCrank();
		}
	}
};

BOOST_FIXTURE_TEST_SUITE(when_last_ball_sold, last_ball_sold_)
BOOST_AUTO_TEST_CASE(has_no_balls_and_state_sold_out)
{
	BOOST_REQUIRE_EQUAL(machine.GetBallCount(), 0);
	BOOST_REQUIRE(machine.GetCurrentState() == State::SoldOut);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()