#include "metric_impl/cyclomatic_complexity.hpp"
#include "files_test_data.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

class CyclomaticComplexityTest : public ::testing::TestWithParam<std::tuple<std::string, std::string, int>> {};

TEST_P(CyclomaticComplexityTest, CountsProperly) {
    const auto &parameters = GetParam();

    const auto &analysis = FilesTests::FunctionsAnalysisForFile(std::get<0>(parameters));
    const metric::MetricResults &results = FindMetricResultsForFunction(analysis, std::get<1>(parameters));
    auto metric_iter = std::ranges::find(results, "Cyclomatic Complexity", &metric::MetricResult::metric_name);
    ASSERT_TRUE(metric_iter != results.cend());
    EXPECT_EQ(std::get<int>(metric_iter->value), std::get<2>(parameters));
}

INSTANTIATE_TEST_SUITE_P(FilesTests, CyclomaticComplexityTest,
                         ::testing::Values(std::make_tuple("comments.py", "Func_comments", 1),
                                           std::make_tuple("exceptions.py", "Try_Exceptions", 4),
                                           std::make_tuple("if.py", "testIf", 2),
                                           std::make_tuple("loops.py", "TestLoops", 4),
                                           std::make_tuple("many_lines.py", "testmultiline", 2),
                                           std::make_tuple("many_parameters.py", "__test_multiparameters__", 2),
                                           std::make_tuple("match_case.py", "test_Match_case", 4),
                                           std::make_tuple("nested_if.py", "Testnestedif", 4),
                                           std::make_tuple("simple.py", "test_simple", 2),
                                           std::make_tuple("ternary.py", "teSt_ternary", 3)));

}  // namespace analyzer::metric::metric_impl
