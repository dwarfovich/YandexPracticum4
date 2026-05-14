#pragma once

#include <gtest/gtest.h>

#include <string>
#include <ranges>

class FileBasedTest : public ::testing::Test {
protected:
    static std::string file_content;
    inline static auto filenames = std::to_array(
    {"comments.py", "exceptions.py"});

    static void SetUpTestSuite() { 
        static_assert(__cplusplus >= 202302L);
        std::ranges::for_each(filenames, ()[]{});
    }
    static void TearDownTestSuite();
};