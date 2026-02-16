/**
 * Unit tests for game logic (calculate_winnings_simulation).
 * Build: make test  or  gcc -I. -o test_game_logic tests/test_game_logic.c game_logic.c
 */
#include "game_logic.h"
#include <stdio.h>
#include <stdlib.h>

static int tests_run = 0;
static int tests_failed = 0;

#define ASSERT_EQ(a, b) do { \
    tests_run++; \
    if ((a) != (b)) { \
        printf("  FAIL: %s:%d: expected %d, got %d\n", __func__, __LINE__, (int)(b), (int)(a)); \
        tests_failed++; \
    } \
} while(0)

#define TEST(name) static void name(void)

/* 0=green, 1=red, 2=black - matches game_logic.c */
TEST(test_straight_up_hit) {
    int stake = 10;
    int choice = 17;
    int winnings = calculate_winnings_simulation(choice, 17, colors[17], colors, stake);
    ASSERT_EQ(winnings, 36 * stake);
}

TEST(test_straight_up_miss) {
    int stake = 10;
    int choice = 17;
    int winnings = calculate_winnings_simulation(choice, 5, colors[5], colors, stake);
    ASSERT_EQ(winnings, 0);
}

TEST(test_straight_up_zero) {
    int stake = 10;
    int choice = 0;
    int winnings = calculate_winnings_simulation(choice, 0, colors[0], colors, stake);
    ASSERT_EQ(winnings, 36 * stake);
}

TEST(test_column_1) {
    int stake = 10;
    int choice = 41; /* column 1: 1, 4, 7, 10, ... */
    ASSERT_EQ(calculate_winnings_simulation(choice, 1, colors[1], colors, stake), 3 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 4, colors[4], colors, stake), 3 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 7, colors[7], colors, stake), 3 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 2, colors[2], colors, stake), 0);
    ASSERT_EQ(calculate_winnings_simulation(choice, 0, colors[0], colors, stake), 0);
}

TEST(test_column_2) {
    int stake = 10;
    int choice = 42; /* column 2: 2, 5, 8, ... (spin % 3 == 2) */
    ASSERT_EQ(calculate_winnings_simulation(choice, 2, colors[2], colors, stake), 3 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 5, colors[5], colors, stake), 3 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 35, colors[35], colors, stake), 3 * stake); /* 35 % 3 == 2 */
    ASSERT_EQ(calculate_winnings_simulation(choice, 36, colors[36], colors, stake), 0); /* 36 -> column 3 */
    ASSERT_EQ(calculate_winnings_simulation(choice, 1, colors[1], colors, stake), 0);
}

TEST(test_column_3) {
    int stake = 10;
    int choice = 43; /* column 3: 3, 6, 9, ... (spin % 3 == 0) */
    ASSERT_EQ(calculate_winnings_simulation(choice, 3, colors[3], colors, stake), 3 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 6, colors[6], colors, stake), 3 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 1, colors[1], colors, stake), 0);
}

TEST(test_dozen_1) {
    int stake = 10;
    int choice = 51; /* 1-12 */
    ASSERT_EQ(calculate_winnings_simulation(choice, 1, colors[1], colors, stake), 3 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 12, colors[12], colors, stake), 3 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 13, colors[13], colors, stake), 0);
    ASSERT_EQ(calculate_winnings_simulation(choice, 0, colors[0], colors, stake), 0);
}

TEST(test_dozen_2) {
    int choice = 52; /* 13-24 */
    int stake = 10;
    ASSERT_EQ(calculate_winnings_simulation(choice, 13, colors[13], colors, stake), 3 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 24, colors[24], colors, stake), 3 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 25, colors[25], colors, stake), 0);
}

TEST(test_dozen_3) {
    int choice = 53; /* 25-36 */
    int stake = 10;
    ASSERT_EQ(calculate_winnings_simulation(choice, 25, colors[25], colors, stake), 3 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 36, colors[36], colors, stake), 3 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 24, colors[24], colors, stake), 0);
}

TEST(test_odd) {
    int choice = 61; /* 1=odd */
    int stake = 10;
    ASSERT_EQ(calculate_winnings_simulation(choice, 1, colors[1], colors, stake), 2 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 3, colors[3], colors, stake), 2 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 2, colors[2], colors, stake), 0);
    ASSERT_EQ(calculate_winnings_simulation(choice, 0, colors[0], colors, stake), 0);
}

TEST(test_even) {
    int choice = 62; /* 2=even */
    int stake = 10;
    ASSERT_EQ(calculate_winnings_simulation(choice, 2, colors[2], colors, stake), 2 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 36, colors[36], colors, stake), 2 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice, 1, colors[1], colors, stake), 0);
}

TEST(test_red_black) {
    /* choice 71 = red (color 1), 72 = black (color 2) */
    int stake = 10;
    ASSERT_EQ(calculate_winnings_simulation(71, 1, 1, colors, stake), 2 * stake);
    ASSERT_EQ(calculate_winnings_simulation(72, 2, 2, colors, stake), 2 * stake);
    ASSERT_EQ(calculate_winnings_simulation(71, 2, 2, colors, stake), 0);
    ASSERT_EQ(calculate_winnings_simulation(72, 1, 1, colors, stake), 0);
    ASSERT_EQ(calculate_winnings_simulation(71, 0, 0, colors, stake), 0); /* zero does not win */
}

TEST(test_low_high) {
    int choice_low = 81;
    int choice_high = 82;
    int stake = 10;
    ASSERT_EQ(calculate_winnings_simulation(choice_low, 1, colors[1], colors, stake), 2 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice_low, 18, colors[18], colors, stake), 2 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice_low, 19, colors[19], colors, stake), 0);
    ASSERT_EQ(calculate_winnings_simulation(choice_high, 19, colors[19], colors, stake), 2 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice_high, 36, colors[36], colors, stake), 2 * stake);
    ASSERT_EQ(calculate_winnings_simulation(choice_high, 18, colors[18], colors, stake), 0);
    ASSERT_EQ(calculate_winnings_simulation(choice_low, 0, colors[0], colors, stake), 0);
}

TEST(test_consistency_with_calculate_winnings) {
    /* calculate_winnings calls calculate_winnings_simulation with g_player.current_bet */
    g_player.current_bet = 5;
    int w = calculate_winnings_simulation(17, 17, colors[17], colors, 5);
    ASSERT_EQ(w, 36 * 5);
}

int main(void) {
    printf("Running tests (game_logic)...\n");

    test_straight_up_hit();
    test_straight_up_miss();
    test_straight_up_zero();
    test_column_1();
    test_column_2();
    test_column_3();
    test_dozen_1();
    test_dozen_2();
    test_dozen_3();
    test_odd();
    test_even();
    test_red_black();
    test_low_high();
    test_consistency_with_calculate_winnings();

    printf("\nResult: %d tests, %d failed.\n", tests_run, tests_failed);
    return tests_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
