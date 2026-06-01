#include "metric_accumulator_impl/average_accumulator.hpp"
#include "../../metric_impl/tests/files_test_data.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

TEST_F(FilesTests, Initialization) {}

void AccumulateFilesAnalysis(const std::vector<std::string> &files, MetricsAccumulator &accumulator) {
    rs::for_each(files, [&](const auto &file) {
        const auto &analysis = FilesTests::FunctionsAnalysisForFile(file);
        AccumulateFunctionAnalysis(analysis, accumulator);
    });
}

TEST(AverageAccumulatorTests, Empty) {
    analyzer::metric_accumulator::MetricsAccumulator accumulator;
    using namespace analyzer::metric_accumulator::metric_accumulator_impl;
    using namespace analyzer::metric::metric_impl;
    accumulator.RegisterAccumulator(CodeLinesCountMetric::kName, std::make_unique<AverageAccumulator>());
    analyzer::AccumulateFunctionAnalysis(FunctionsAnalyseResult{}, accumulator);
    const auto &f_acc = accumulator.GetFinalizedAccumulator<AverageAccumulator>(CodeLinesCountMetric::kName);
    const auto r = f_acc.Get();
    ASSERT_DOUBLE_EQ(r, 0.);
}

TEST(AverageAccumulatorTests, SingleFile) {
    MetricsAccumulator accumulator;
    using namespace analyzer::metric_accumulator::metric_accumulator_impl;
    using namespace analyzer::metric::metric_impl;
    accumulator.RegisterAccumulator(CodeLinesCountMetric::kName, std::make_unique<AverageAccumulator>());

    AccumulateFilesAnalysis({"comments.py"}, accumulator);

    const auto &f_acc = accumulator.GetFinalizedAccumulator<AverageAccumulator>(CodeLinesCountMetric::kName);
    const auto r = f_acc.Get();
    ASSERT_DOUBLE_EQ(r, 3.);
}

TEST(AverageAccumulatorTests, MultipleFiles1) {
    MetricsAccumulator accumulator;
    using namespace analyzer::metric_accumulator::metric_accumulator_impl;
    using namespace analyzer::metric::metric_impl;
    accumulator.RegisterAccumulator(CodeLinesCountMetric::kName, std::make_unique<AverageAccumulator>());

    AccumulateFilesAnalysis({"comments.py", "simple.py"}, accumulator);

    const auto &f_acc = accumulator.GetFinalizedAccumulator<AverageAccumulator>(CodeLinesCountMetric::kName);
    const auto r = f_acc.Get();
    ASSERT_DOUBLE_EQ(r, 4.);
}

TEST(AverageAccumulatorTests, MultipleFiles2) {
    MetricsAccumulator accumulator;
    using namespace analyzer::metric_accumulator::metric_accumulator_impl;
    using namespace analyzer::metric::metric_impl;
    accumulator.RegisterAccumulator(CodeLinesCountMetric::kName, std::make_unique<AverageAccumulator>());

    AccumulateFilesAnalysis({"comments.py", "simple.py", "if.py"}, accumulator);

    const auto &f_acc = accumulator.GetFinalizedAccumulator<AverageAccumulator>(CodeLinesCountMetric::kName);
    const auto r = f_acc.Get();
    ASSERT_DOUBLE_EQ(r, 11. / 3);
}

TEST(AverageAccumulatorTests, MultipleFiles3) {
    MetricsAccumulator accumulator;
    using namespace analyzer::metric_accumulator::metric_accumulator_impl;
    using namespace analyzer::metric::metric_impl;
    accumulator.RegisterAccumulator(CodeLinesCountMetric::kName, std::make_unique<AverageAccumulator>());

    AccumulateFilesAnalysis({"comments.py", "simple.py", "if.py", "ternary.py"}, accumulator);

    const auto &f_acc = accumulator.GetFinalizedAccumulator<AverageAccumulator>(CodeLinesCountMetric::kName);
    const auto r = f_acc.Get();
    ASSERT_DOUBLE_EQ(r, 3.);
}

TEST(SumAverageAccumulatorTests, Empty) {
    analyzer::metric_accumulator::MetricsAccumulator accumulator;
    using namespace analyzer::metric_accumulator::metric_accumulator_impl;
    using namespace analyzer::metric::metric_impl;
    accumulator.RegisterAccumulator(CountParametersMetric::kName, std::make_unique<SumAverageAccumulator>());
    analyzer::AccumulateFunctionAnalysis(FunctionsAnalyseResult{}, accumulator);
    const auto &f_acc = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(CountParametersMetric::kName);
    const auto r = f_acc.Get();
    EXPECT_TRUE(std::isnan(r.average));
    ASSERT_EQ(r.sum, 0);
}

TEST(SumAverageAccumulatorTests, SingleFile) {
    MetricsAccumulator accumulator;
    using namespace analyzer::metric_accumulator::metric_accumulator_impl;
    using namespace analyzer::metric::metric_impl;
    accumulator.RegisterAccumulator(CountParametersMetric::kName, std::make_unique<SumAverageAccumulator>());

    AccumulateFilesAnalysis({"comments.py"}, accumulator);

    const auto &f_acc = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(CountParametersMetric::kName);
    const auto r = f_acc.Get();
    ASSERT_DOUBLE_EQ(r.average, 3.);
    ASSERT_EQ(r.sum, 3);
}

TEST(SumAverageAccumulatorTests, MultipleFiles1) {
    MetricsAccumulator accumulator;
    using namespace analyzer::metric_accumulator::metric_accumulator_impl;
    using namespace analyzer::metric::metric_impl;
    accumulator.RegisterAccumulator(CountParametersMetric::kName, std::make_unique<SumAverageAccumulator>());

    AccumulateFilesAnalysis({"comments.py", "simple.py"}, accumulator);

    const auto &f_acc = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(CountParametersMetric::kName);
    const auto r = f_acc.Get();
    ASSERT_DOUBLE_EQ(r.average, 1.5);
    ASSERT_EQ(r.sum, 3);
}

TEST(SumAverageAccumulatorTests, MultipleFiles2) {
    MetricsAccumulator accumulator;
    using namespace analyzer::metric_accumulator::metric_accumulator_impl;
    using namespace analyzer::metric::metric_impl;
    accumulator.RegisterAccumulator(CountParametersMetric::kName, std::make_unique<SumAverageAccumulator>());

    AccumulateFilesAnalysis({"comments.py", "simple.py", "if.py"}, accumulator);

    const auto &f_acc = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(CountParametersMetric::kName);
    const auto r = f_acc.Get();
    ASSERT_DOUBLE_EQ(r.average, 4. / 3);
    ASSERT_EQ(r.sum, 4);
}

TEST(SumAverageAccumulatorTests, MultipleFiles3) {
    MetricsAccumulator accumulator;
    using namespace analyzer::metric_accumulator::metric_accumulator_impl;
    using namespace analyzer::metric::metric_impl;
    accumulator.RegisterAccumulator(CountParametersMetric::kName, std::make_unique<SumAverageAccumulator>());

    AccumulateFilesAnalysis({"comments.py", "simple.py", "if.py", "ternary.py"}, accumulator);

    const auto &f_acc = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(CountParametersMetric::kName);
    const auto r = f_acc.Get();
    ASSERT_DOUBLE_EQ(r.average, 1.25);
    ASSERT_EQ(r.sum, 5);
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
