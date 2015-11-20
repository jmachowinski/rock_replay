#include <boost/test/unit_test.hpp>
#include <rock_replay/Dummy.hpp>

using namespace rock_replay;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    rock_replay::DummyClass dummy;
    dummy.welcome();
}
