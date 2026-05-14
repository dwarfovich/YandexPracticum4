#include "metric_impl/code_lines_count.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

class FilesTestData : public ::testing::Test {
protected:
    static std::vector<std::string> files;

    static void SetUpTestSuite() {
        files.push_back(ReadFile("a.txt"));
        files.push_back(ReadFile("b.txt"));
    }

    static std::string ReadFile(const std::string &path) {
        std::ifstream in(path);
        return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    }
};

std::vector<std::string> FilesTestData::files = {};

TEST(BasicCheck, Sum) { EXPECT_EQ(1 + 1, 2); }

}  // namespace analyzer::metric::metric_impl
