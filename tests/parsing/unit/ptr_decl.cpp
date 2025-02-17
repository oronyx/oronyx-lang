// This file is part of the Klare programming language and is licensed under MIT License;
// See LICENSE.txt for details

#include <catch2.hpp>
#include <compiler/parser/include/parser.h>
#include <compiler/lexer/include/lexer.h>
#include <filesystem>

using namespace klr::compiler;

static AST parse_code(const std::string &code)
{
    std::string test_name = Catch::getResultCapture().getCurrentTestName();
    std::filesystem::path test_file_path = std::filesystem::current_path() / (test_name + ".klr");
    std::string relative_filename = test_file_path.string();

    Lexer lexer(relative_filename, code);
    const auto tokens = lexer.tokenize();
    Parser parser(lexer.module_name, code, *tokens, lexer.get_line_starts());
    auto ast = parser.parse();
    return ast;
}

TEST_CASE("Ptr decl")
{
    SECTION("Own")
    {
        parse_code("var x: Own<i32> = null;");
        parse_code("var x: Own<i32> = cast<Own<i32>>(5);");
        parse_code("var x: Own<Own<i32>> = cast<Own<Own<i32>>>(null);");
        parse_code("var x: Own<i32[]> = cast<Own<i32[]>>(new i32[]{ 1, 2, 3 });");
    }

    SECTION("Share")
    {
        parse_code("var x: Share<i32> = null;");
        parse_code("var x: Share<i32> = cast<Share<i32>>(5);");
        parse_code("var x: Share<Share<i32>> = cast<Share<Share<i32>>>(null);");
        parse_code("var x: Share<i32[]> = cast<Share<i32[]>>(new i32[]{ 1, 2, 3 });");
    }

    SECTION("Ref")
    {
        parse_code("var x: Ref<i32> = null;");
        parse_code("var y: i32 = 5; var x: Ref<i32> = &y;");
        parse_code("var y: Ref<i32> = &x; var z: Ref<Ref<i32>> = &y;");
        parse_code("var arr: i32[] = {1, 2, 3}; var x: Ref<i32[]> = &arr;");
    }

    SECTION("Pin")
    {
        parse_code("var x: Pin<i32> = null;");
        parse_code("var x: Pin<i32> = cast<Pin<i32>>(5);");
        parse_code("var x: Pin<Pin<i32>> = cast<Pin<Pin<i32>>>(null);");
        parse_code("var x: Pin<i32[]> = cast<Pin<i32[]>>(new i32[]{ 1, 2, 3 });");
    }
}