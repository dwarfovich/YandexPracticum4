#pragma once

#include "analyse.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"
#include "metric_impl/naming_style.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <ranges>
#include <string>

inline const analyzer::metric::MetricResults &
FindMetricResultsForFunction(const analyzer::FunctionsAnalyseResult &analysis, const std::string &function) {
    auto iter = std::ranges::find_if(analysis, [&](const auto &result) { return result.first.name == function; });
    if (iter != analysis.cend()) {
        return iter->second;
    } else {
        throw std::runtime_error("Couldn't find analysis for function " + function);
    }
}

class FilesTests : public ::testing::Test {
public:
    static const analyzer::FunctionsAnalyseResult &FunctionsAnalysisForFile(const std::string &file) {
        auto iter = functions_analysis.find(file);
        if (iter != functions_analysis.cend()) {
            return iter->second;
        } else {
            throw std::runtime_error("Unknown file: " + file);
        }
    }

public:
    inline static std::unordered_map<std::string, analyzer::FunctionsAnalyseResult> functions_analysis;
    inline static auto filenames =
        std::to_array({"comments.py", "exceptions.py", "if.py", "loops.py", "many_lines.py", "many_parameters.py",
                       "match_case.py", "nested_if.py", "simple.py", "ternary.py"});

    static void SetUpTestSuite() {
        using namespace analyzer::metric::metric_impl;
        analyzer::metric::MetricExtractor metric_extractor;
        metric_extractor.RegisterMetric(std::make_unique<CyclomaticComplexityMetric>());
        metric_extractor.RegisterMetric(std::make_unique<CodeLinesCountMetric>());
        metric_extractor.RegisterMetric(std::make_unique<NamingStyleMetric>());
        metric_extractor.RegisterMetric(std::make_unique<CountParametersMetric>());

        std::ranges::for_each(filenames, [&metric_extractor](const auto &filename) {
            auto analysis = analyzer::AnalyseFunctions({filename}, metric_extractor);
            functions_analysis.insert({filename, std::move(analysis)});
        });
    }
    static void TearDownTestSuite() {}
};