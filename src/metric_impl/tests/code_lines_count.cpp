#include "metric_impl/code_lines_count.hpp"
#include "files_test_data.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

const metric::MetricResults &FindMetricResultsForFunction(const FunctionsAnalyseResult &analysis,
                                                          const std::string &function) {
    auto iter = std::ranges::find_if(analysis, [&](const auto &result) { return result.first.name == function; });
    if (iter != analysis.cend()) {
        return iter->second;
    } else {
        throw std::runtime_error("Couldn't find analysis for function " + function);
    }
}

class LinesCountTest : public ::testing::TestWithParam<std::tuple<std::string, std::string, int>> {};

TEST_P(LinesCountTest, CountsProperly) {
    const auto &parameters = GetParam();

    const auto &analysis = FilesTests::FunctionsAnalysisForFile(std::get<0>(parameters));
    const metric::MetricResults &results = FindMetricResultsForFunction(analysis, std::get<1>(parameters));
    auto metric_iter = std::ranges::find(results, "Code lines count" , &metric::MetricResult::metric_name);
    ASSERT_TRUE(metric_iter != results.cend());
    EXPECT_EQ(std::get<int>(metric_iter->value), std::get<2>(parameters));
}

TEST_F(FilesTests, Initialization) {}

INSTANTIATE_TEST_SUITE_P(FilesTests, LinesCountTest,
                         ::testing::Values(std::make_tuple("comments.py", "Func_comments", 3),
                                           std::make_tuple("exceptions.py", "Try_Exceptions", 7),
                                           std::make_tuple("if.py", "testIf", 3),
                                           std::make_tuple("loops.py", "TestLoops", 6),
                                           std::make_tuple("many_lines.py", "testmultiline", 11),
                                           std::make_tuple("many_parameters.py", "__test_multiparameters__", 1),
                                           std::make_tuple("match_case.py", "test_Match_case", 7),
                                           std::make_tuple("nested_if.py", "Testnestedif", 8),
                                           std::make_tuple("simple.py", "test_simple", 5),
                                           std::make_tuple("ternary.py", "teSt_ternary", 1)));

}  // namespace analyzer::metric::metric_impl
