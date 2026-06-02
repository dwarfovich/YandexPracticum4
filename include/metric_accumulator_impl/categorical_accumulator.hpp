#pragma once
//#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "metric_accumulator.hpp"

namespace analyzer::metric_accumulator::metric_accumulator_impl {

struct CategoricalAccumulator : public IAccumulator {
    void Accumulate(const metric::MetricResult &metric_result) override { 
        using ValueType = std::variant<int, std::string>;  // если захотите реализовывать метрику
        ++categories_freq[std::get<std::string>(metric_result.value)];
    }

    virtual void Finalize() override {}

    virtual void Reset() override { categories_freq.clear();
    }

    const std::unordered_map<std::string, int> &Get() const { return categories_freq; }

private:
    std::unordered_map<std::string, int> categories_freq;
};

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl
