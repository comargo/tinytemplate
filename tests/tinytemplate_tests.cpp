// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <tinytemplate.hpp>

#include <gtest/gtest.h>

TEST(TinyTemplate, SimplePass)
{
    std::string input = "test";
    std::string expected = input;
    std::string output = tinytemplate::render(input, {});
    EXPECT_EQ(expected, output);
}

TEST(TinyTemplate, SimpleVar)
{
    std::string input = "t{{var}}t";
    std::string expected = "test";
    std::string output = tinytemplate::render(input, {
                                                  {"var", "es"}
                                              });
    EXPECT_EQ(expected, output);
}

TEST(TinyTemplate, ComposedVar)
{
    std::string input = "t{{v{{var2}}}}t";
    std::string expected = "test";
    std::string output = tinytemplate::render(input, {
                                                  {"var", "es"},
                                                  {"var2", "ar"}
                                              });
    EXPECT_EQ(expected, output);
}

TEST(TinyTemplate, ComposedVar2)
{
    std::string input = "t{{v{{var1}}{{var2}}}}t";
    std::string expected = "test";
    std::string output = tinytemplate::render(input, {
                                                  {"var", "es"},
                                                  {"var1", "a"},
                                                  {"var2", "r"}
                                              });
    EXPECT_EQ(expected, output);
}

TEST(TinyTemplate, LostVar)
{
    std::string input = "t{{var}}t";
    std::string expected = input;
    std::string output = tinytemplate::render(input, {});
    EXPECT_EQ(expected, output);
}

TEST(TinyTemplate, Escaped)
{
    std::string input = "t\\{est\\}";
    std::string expected = input;
    std::string output;
    EXPECT_NO_THROW(
                output = tinytemplate::render(input, {}, true);
            );
    EXPECT_EQ(expected, input);
}

TEST(TinyTemplate, UnbalancedOpenBracesError)
{
    std::string input = "t{{var";
    EXPECT_THROW(
                std::string output = tinytemplate::render(input, {});
            , tinytemplate::render_error);
}

TEST(TinyTemplate, UnbalancedOpenBracesNoError)
{
    std::string input = "t{{var";
    std::string expected = input;
    std::string output;
    EXPECT_NO_THROW(
                output = tinytemplate::render(input, {}, true);
            );
    EXPECT_EQ(expected, input);
}

TEST(TinyTemplate, UnbalancedCloseBracesError)
{
    std::string input = "tvar}}";
    EXPECT_THROW(
                std::string output = tinytemplate::render(input, {});
            , tinytemplate::render_error);
}

TEST(TinyTemplate, UnbalancedCloseBracesNoError)
{
    std::string input = "tvar}}";
    std::string expected = input;
    std::string output;
    EXPECT_NO_THROW(
                output = tinytemplate::render(input, {}, true);
            );
    EXPECT_EQ(expected, input);
}

TEST(TinyTemplate, Unescaped)
{
    std::string input = "t{est}";
    EXPECT_THROW(
                std::string output = tinytemplate::render(input, {});
            , tinytemplate::render_error);
}

TEST(TinyTemplate, UnescapedNoError)
{
    std::string input = "t{est}";
    std::string expected = input;
    std::string output;
    EXPECT_NO_THROW(
                output = tinytemplate::render(input, {}, true);
            );
    EXPECT_EQ(expected, input);
}
