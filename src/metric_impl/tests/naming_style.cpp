#include "metric_impl/naming_style.hpp"
#include "files_test_data.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

class NamingStyleTest : public ::testing::TestWithParam<std::tuple<std::string, std::string, std::string>> {};

TEST_P(NamingStyleTest, CountsProperly) {
    const auto &parameters = GetParam();

    const auto &analysis = FilesTests::FunctionsAnalysisForFile(std::get<0>(parameters));
    const metric::MetricResults &results = FindMetricResultsForFunction(analysis, std::get<1>(parameters));
    auto metric_iter = std::ranges::find(results, "Naming style", &metric::MetricResult::metric_name);
    ASSERT_TRUE(metric_iter != results.cend());
    EXPECT_EQ(std::get<std::string>(metric_iter->value), std::get<2>(parameters));
}

INSTANTIATE_TEST_SUITE_P(FilesTests, NamingStyleTest,
                         ::testing::Values(std::make_tuple("comments.py", "Func_comments", "Unknown"),
                                           std::make_tuple("exceptions.py", "Try_Exceptions", "Unknown"),
                                           std::make_tuple("if.py", "testIf", "Camel Case"),
                                           std::make_tuple("loops.py", "TestLoops", "Pascal Case"),
                                           std::make_tuple("many_lines.py", "testmultiline", "Lower Case"),
                                           std::make_tuple("many_parameters.py", "__test_multiparameters__",
                                                           "Snake Case"),
                                           std::make_tuple("match_case.py", "test_Match_case", "Unknown"),
                                           std::make_tuple("nested_if.py", "Testnestedif", "Pascal Case"),
                                           std::make_tuple("simple.py", "test_simple", "Snake Case"),
                                           std::make_tuple("ternary.py", "teSt_ternary", "Unknown")));

}  // namespace analyzer::metric::metric_impl
