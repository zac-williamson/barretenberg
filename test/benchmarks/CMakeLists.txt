include(FetchContent)

### DEPENDENCIES
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark
    GIT_TAG v1.5.0
)

FetchContent_GetProperties(benchmark)
if(NOT benchmark_POPULATED)
  fetchcontent_populate(benchmark)
  add_subdirectory(${benchmark_SOURCE_DIR} ${benchmark_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_compile_options(-Werror -Wall -Wextra -Wshadow)
add_compile_options(-Wconversion -Wsign-conversion)

add_executable(
    barretenberg_bench
    bench_barretenberg.cpp
)

if (WIN32)
    target_link_libraries(
        barretenberg_bench
        PUBLIC
        barretenberg
        PRIVATE
        benchmark
        wsock32
        ws2_32
    )
else()
    target_link_libraries(
        barretenberg_bench
        PUBLIC
        barretenberg
        PRIVATE
        benchmark
    )
endif()

target_include_directories(
    barretenberg_bench
    PRIVATE
        ${private_include_dir}
        ${include_dir}
)

set_target_properties(
    barretenberg_bench
    PROPERTIES

    RUNTIME_OUTPUT_DIRECTORY ..
)

add_executable(
    plonk_bench
    bench_plonk.cpp
)

if (WIN32)
    target_link_libraries(
        plonk_bench
        PUBLIC
        barretenberg
        PRIVATE
        benchmark
        wsock32
        ws2_32
    )
else()
    target_link_libraries(
        plonk_bench
        PUBLIC
        barretenberg
        PRIVATE
        benchmark
    )
endif()

target_include_directories(
    plonk_bench
    PRIVATE
        ${private_include_dir}
        ${include_dir}
)

set_target_properties(
    plonk_bench
    PROPERTIES

    RUNTIME_OUTPUT_DIRECTORY ..
)

add_executable(
    sha256_bench
    bench_sha256.cpp
)

if (WIN32)
    target_link_libraries(
        sha256_bench
        PUBLIC
        barretenberg
        PRIVATE
        benchmark
        wsock32
        ws2_32
    )
else()
    target_link_libraries(
        sha256_bench
        PUBLIC
        barretenberg
        PRIVATE
        benchmark
    )
endif()

target_include_directories(
    sha256_bench
    PRIVATE
        ${private_include_dir}
        ${include_dir}
)

set_target_properties(
    sha256_bench
    PROPERTIES

    RUNTIME_OUTPUT_DIRECTORY ..
)


add_executable(
    mimc_bench
    bench_mimc.cpp
)

if (WIN32)
    target_link_libraries(
        mimc_bench
        PUBLIC
        barretenberg
        PRIVATE
        benchmark
        wsock32
        ws2_32
    )
else()
    target_link_libraries(
        mimc_bench
        PUBLIC
        barretenberg
        PRIVATE
        benchmark
    )
endif()

target_include_directories(
    mimc_bench
    PRIVATE
        ${private_include_dir}
        ${include_dir}
)

set_target_properties(
    mimc_bench
    PROPERTIES

    RUNTIME_OUTPUT_DIRECTORY ..
)
