
#include "config.h"
#include "hashmap/map.h"
#include "ctest/test.h"
#include "ctest/assert.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void setup_env() {
}

char before_test() {
    return 0;
}

char after_test() {
    return 0;
}

void clean_env() {
}

char test_single_string_argument_in_long_form_happy_path() {

    arg_t arg_desc[] = {
        {"t", "test-arg", STRING}
    };

    char *argv[] = {"test", "--test-arg", "some value"};

    map *args = arg_parse(sizeof(arg_desc)/sizeof(arg_t), arg_desc, 3, argv);

    char *parsed_value = (char *)map_get(args, "test-arg");
    int assertion_error = assert_str_equals("Big int is incremented by one.", parsed_value, "some value");
    
    // Clean up
    map_destroy(args);

    // Return test status
    if (assertion_error) {
        return 1;
    }
    return 0;
}

char test_two_string_arguments_in_long_form_happy_path() {

    arg_t arg_desc[] = {
        {"t", "test-arg", STRING},
        {"o", "other-arg", STRING}
    };

    char *argv[] = {"test", "--test-arg", "some value", "--other-arg", "other value"};

    map *args = arg_parse(sizeof(arg_desc)/sizeof(arg_t), arg_desc, 3, argv);

    char *parsed_value = (char *)map_get(args, "test-arg");
    char *other_parsed_value = (char *)map_get(args, "other-arg");

    int assertion_error = assert_str_equals("Parsed value is the given value.", parsed_value, "some value")
            && assert_str_equals("Parsed value is the given value.", other_parsed_value, "other value");;
    
    // Clean up
    map_destroy(args);

    // Return test status
    if (assertion_error) {
        return 1;
    }
    return 0;
}

char test_one_integer_argument_in_long_form_happy_path() {

    arg_t arg_desc[] = {
        {"i", "int-arg", INTEGER}
    };

    char *argv[] = {"test", "--int-arg", "100"};

    map *args = arg_parse(sizeof(arg_desc)/sizeof(arg_t), arg_desc, 3, argv);

    int parsed_value = (int)map_get(args, "int-arg");
    int assertion_error = assert_int_equals("Parsed value is the given value.", parsed_value, 100);
    
    // Clean up
    map_destroy(args);

    // Return test status
    if (assertion_error) {
        return 1;
    }
    return 0;
}

test test_suite[] = {
    {
        "Test single program argument, single argument given, happy path", test_single_string_argument_in_long_form_happy_path
    },
    {
        "Test two program arguments, two arguments given, happy path", test_two_string_arguments_in_long_form_happy_path
    },
    {
        "Test parsing an integer argument given in the long form", test_one_integer_argument_in_long_form_happy_path
    }
};

int main(int argc, char **argv) {
    suite_report report = run_test_suite(test_suite, sizeof(test_suite)/sizeof(test));

    if (report.failures > 0) {
        return -1;
    }

    return 0;
}
