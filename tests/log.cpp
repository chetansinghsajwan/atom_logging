import atom.core;
import atom.logging;
#include "catch2/catch_test_macros.hpp"
#include "catch2/benchmark/catch_benchmark.hpp"

using namespace atom::logging;

TEST_CASE("static_log", "[implementation]")
{
    BENCHMARK("log")
    {
        log_trace("holla! this is an error.");
        return 0;
    };

    BENCHMARK("static_log - optimized")
    {
        static_log_trace("holla! this is trace.");
        return 0;
    };

    BENCHMARK("static_log - not_optimized")
    {
        static_log_error("holla! this is error.");
        return 0;
    };

    BENCHMARK("empty")
    {
        return 0;
    };
}
