project(SimpleParser)

find_package(BISON)
find_package(FLEX)

set(SOURCE_DIR src)

set(
    HEADERS
    ${SOURCE_DIR}/driver.hh
    ${SOURCE_DIR}/parser.hh
    )

BISON_TARGET(
    Parser
    ${SOURCE_DIR}/parser.y
    ${SOURCE_DIR}/parser.cpp
    COMPILE_FLAGS --graph
    DEFINES_FILE ${SOURCE_DIR}/parser.hh
)

FLEX_TARGET(
    Lexer
    ${SOURCE_DIR}/lexer.l
    ${SOURCE_DIR}/lexer.cpp
)

ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

add_executable(
    Run
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/AST.cpp
    ${BISON_OUTPUTS}
    ${FLEX_OUTPUTS}
)
target_include_directories(Run PRIVATE ${SOURCE_DIR})
