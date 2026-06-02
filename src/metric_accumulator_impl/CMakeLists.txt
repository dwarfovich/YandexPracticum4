set(target metric_accumulator_test)

add_executable(${target}
    tests/average_accumulator.cpp
    tests/categorical_accumulator.cpp
    tests/sum_average_accumulator.cpp
    ../metric_impl/tests/files_test_data.cpp
)

target_link_libraries(${target}
    PRIVATE
        GTest::GTest
        GTest::Main
        metric
        metric_accumulator
        function
        file
)

message(STATUS "Source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Binary dir: ${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB test_files "${CMAKE_CURRENT_SOURCE_DIR}/tests/files/*.py")
foreach(file IN LISTS test_files)
    get_filename_component(file_name ${file} NAME)
    configure_file(${file} "${CMAKE_CURRENT_BINARY_DIR}/${file_name}" COPYONLY)
endforeach()

add_test(NAME ${target} COMMAND ${target})