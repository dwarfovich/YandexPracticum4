#include "metric_impl/parameters_count.hpp"
#include "files_test_data.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

class ParametersCountTest : public ::testing::TestWithParam<std::tuple<std::string, std::string, int>> {};

TEST_P(ParametersCountTest, CountsProperly) {
    const auto &parameters = GetParam();

    const auto &analysis = FilesTests::FunctionsAnalysisForFile(std::get<0>(parameters));
    const metric::MetricResults &results = FindMetricResultsForFunction(analysis, std::get<1>(parameters));
    auto metric_iter = std::ranges::find(results, "Parameters count", &metric::MetricResult::metric_name);
    ASSERT_TRUE(metric_iter != results.cend());
    EXPECT_EQ(std::get<int>(metric_iter->value), std::get<2>(parameters));
}

INSTANTIATE_TEST_SUITE_P(FilesTests, ParametersCountTest,
                         ::testing::Values(std::make_tuple("comments.py", "Func_comments", 3),
                                           std::make_tuple("exceptions.py", "Try_Exceptions", 0),
                                           std::make_tuple("if.py", "testIf", 1),
                                           std::make_tuple("loops.py", "TestLoops", 1),
                                           std::make_tuple("many_lines.py", "testmultiline", 0),
                                           std::make_tuple("many_parameters.py", "__test_multiparameters__", 5),
                                           std::make_tuple("match_case.py", "test_Match_case", 1),
                                           std::make_tuple("nested_if.py", "Testnestedif", 2),
                                           std::make_tuple("simple.py", "test_simple", 0),
                                           std::make_tuple("ternary.py", "teSt_ternary", 1)));

}  // namespace analyzer::metric::metric_impl
