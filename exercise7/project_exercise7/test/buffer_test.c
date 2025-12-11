/**
 * Compile and run with:
 * cd project_ex7
 * gcc -I. src/dsp/buffer.* test/buffer_test.c -o test/buffer_test.out
 * ./test/buffer_test.out
 */

#include <stdio.h>
#include "src/dsp/buffer.h"

#define N_TAPS 10
#define BUFFER_SIZE 20

buffer_t my_buffer;

/* TEST DEFINITIONS */

#define FAIL() return 0;
#define PASS() return 1;
#define LOG_FUNC() printf("Running %s...\n", __func__)

int test_buffer_creation(void)
{
    LOG_FUNC();

    if (my_buffer == NULL) {
        printf("Creation failed: buffer pointer is NULL\n");
        FAIL();
    }

    uint16_t len = buffer_getlen(my_buffer);
    if (len != 0) {
        printf("Creation mismatch: length=%u expected=0\n", len);
        FAIL();
    }

    PASS();
}

int test_buffer_insertion_and_length(void)
{
    LOG_FUNC();

    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer_insert(my_buffer, (item_t) i);
        uint16_t len = buffer_getlen(my_buffer);
        uint16_t expected = i + 1;

        if (len != expected) {
            printf("Length mismatch after insert %d: got=%u expected=%u\n",
                   i, len, expected);
            FAIL();
        }
    }

    if (buffer_getlen(my_buffer) != BUFFER_SIZE) {
        printf("Final length mismatch: got=%u expected=%u\n",
               buffer_getlen(my_buffer), BUFFER_SIZE);
        FAIL();
    }

    PASS();
}


int test_buffer_overflow_handling(void)
{
    LOG_FUNC();

    for (int i = 0; i < BUFFER_SIZE + 5; i++) {
        buffer_insert(my_buffer, (item_t) i);
    }

    uint16_t len = buffer_getlen(my_buffer);
    if (len != BUFFER_SIZE) {
        printf("Overflow length mismatch: got=%u expected=%u\n",
               len, BUFFER_SIZE);
        FAIL();
    }

    PASS();
}


int test_buffer_peek(void)
{
    LOG_FUNC();

    for (int i = 0; i < N_TAPS; i++) {
        buffer_insert(my_buffer, (item_t) i);
    }

    for (int i = 0; i < N_TAPS; i++) {
        uint16_t age = N_TAPS - i - 1;
        item_t got = buffer_peek(my_buffer, age);
        item_t expected = (item_t) i;

        if (got != expected) {
            printf("Peek mismatch: age=%u got=%d expected=%d\n",
                   age, got, expected);
            FAIL();
        }
    }

    PASS();
}


int test_buffer_pop(void)
{
    LOG_FUNC();

    for (int i = 0; i < N_TAPS; i++) {
        buffer_insert(my_buffer, (item_t)i);
    }

    for (int i = 0; i < N_TAPS; i++) {
        item_t got = buffer_pop(my_buffer);
        item_t expected = (item_t)i;

        if (got != expected) {
            printf("Pop mismatch: got=%d expected=%d at index=%d\n",
                   got, expected, i);
            FAIL();
        }
    }

    if (buffer_getlen(my_buffer) != 0) {
        printf("Pop final length mismatch: got=%u expected=0\n",
               buffer_getlen(my_buffer));
        FAIL();
    }

    PASS();
}


int test_peek_after_wrap(void)
{
    LOG_FUNC();

    // Fill buffer completely
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer_insert(my_buffer, (item_t)i);
    }

    // Now insert 10 more elements to cause wrap-around
    for (int i = 0; i < 10; i++) {
        buffer_insert(my_buffer, (item_t) (100 + i));
    }

    // Now the buffer should contain:
    // [10 .. 19, 100 .. 109]
    for (int age = 0; age < BUFFER_SIZE; age++)
    {
        item_t expected;
        if (age <= 9) {
            // age==0 -> 109, age==1 -> 108, ..., age=9 -> 100
            expected = (item_t)(100 + 9 - age);
        } else {
            // age==10 -> 19, age==11 -> 18, ..., age==19 -> 10
            expected = (item_t)(BUFFER_SIZE + 9 - age);
        }
        item_t got = buffer_peek(my_buffer, (uint16_t) age);

        if (got != expected) {
            printf("Fail: age=%d, got=%d, expected=%d\n", age, got, expected);
            FAIL();
        }
    }

    PASS();
}

int test_pop_after_overflow(void)
{
    LOG_FUNC();

    // Fill buffer completely
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer_insert(my_buffer, (item_t)i);
    }

    // Now insert 5 more elements to cause overflow
    for (int i = 0; i < 5; i++) {
        buffer_insert(my_buffer, (item_t)(100 + i));
    }

    // Now the buffer should contain:
    // [5 .. BUFFER_SIZE-1, 100 .. 104]

    int expected_start = BUFFER_SIZE - (BUFFER_SIZE - 5);
    int index = expected_start;

    for (int i = 0; i < BUFFER_SIZE; i++) {
        item_t val = buffer_pop(my_buffer);
        item_t expected;

        if (i < BUFFER_SIZE - 5)
            expected = i + 5;
        else
            expected = 100 + (i - (BUFFER_SIZE - 5));

        if (val != expected) {
            printf("pop mismatch: got %d, expected %d\n", val, expected);
            FAIL();
        }
    }

    if (buffer_getlen(my_buffer) == 0)
        PASS();
    
    FAIL();
}

int test_overflow_counter(void)
{
    LOG_FUNC();

    // No overflows at the beginning
    if (buffer_get_overflow_count(my_buffer) != 0)
        FAIL();

    // Fill buffer completely
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer_insert(my_buffer, (item_t)i);
    }

    int num_overflows = 7;

    // Now insert num_overflows more elements to cause overflows
    for (int i = 0; i < num_overflows; i++) {
        buffer_insert(my_buffer, (item_t)(1000 + i));
    }

    if (buffer_get_overflow_count(my_buffer) == num_overflows)
        PASS();

    printf("Overflow count mismatch: expected %u, got %u\n",
           num_overflows, buffer_get_overflow_count(my_buffer));
    FAIL();
}

int test_negative_age_peek(void)
{
    LOG_FUNC();

    buffer_insert(my_buffer, 42);

    uint16_t negative = (uint16_t) -1; // 65535

    item_t val = buffer_peek(my_buffer, negative);

    if (val == INVALID_VAL)
        PASS();

    printf("Peek with negative age unexpectedly succeeded.\n");
    FAIL();
}

int test_stress_insert_pop(void)
{
    LOG_FUNC();

    for (int i = 0; i < 50000; i++) {
        buffer_insert(my_buffer, (item_t)i);

        if (i % 3 == 0) {
            item_t v = buffer_pop(my_buffer);
        }
    }

    PASS(); // as long as no crash occurs
}


#define NUM_TESTS 10 // Increment this when adding new tests

int (*test_case[NUM_TESTS]) (void) = {
    test_buffer_creation,
    test_buffer_insertion_and_length,
    test_buffer_overflow_handling,
    test_buffer_peek,
    test_buffer_pop,
    test_peek_after_wrap,
    test_pop_after_overflow,
    test_overflow_counter,
    test_negative_age_peek,
    test_stress_insert_pop,
};

/* TEST EXECUTION */

int run_test(int test_id)
{
    if (test_id < 0 || test_id >= NUM_TESTS) {
        return -1; // Invalid test ID
    }
    if (test_case[test_id]()) {
        printf("Test %d passed!\n", test_id);
        PASS();
    } else {
        printf("Test %d failed!\n", test_id);
        FAIL();
    }
}

void setup(void)
{
    my_buffer = buffer_create(BUFFER_SIZE);
}

void teardown(void)
{
    buffer_delete(&my_buffer);
}

int main(void)
{
    int tests_passed = 0;
    int tests_failed = 0;

    for (int i = 0; i < NUM_TESTS; i++) {
        setup();
        if (run_test(i) > 0) {
            tests_passed++;
        } else {
            tests_failed++;
        }
        teardown();
    }

    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    
    return 0;
}
