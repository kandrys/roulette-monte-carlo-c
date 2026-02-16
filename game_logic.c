#include "game_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>


#ifdef _WIN32
    #include <windows.h>
    #define SLEEP_MS(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define SLEEP_MS(ms) usleep((ms) * 1000)
#endif


Player g_player;

/* 0=green, 1=red, 2=black */
int colors[WHEEL_SLOTS] = {0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2, 1, 2, 1};


/**
 * Reads input from stdin, converts to long and validates.
 * Prevents program crash when non-numeric input is entered.
 */
long read_number(const char *prompt, int min, int max) {
    char buffer[100];
    char *endptr;
    long value;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) return min;
        if (buffer[0] == '\n') continue;

        errno = 0;
        value = strtol(buffer, &endptr, 10);

        if (endptr == buffer || errno == ERANGE) {
            printf(">> Error: Value is not a number.\n");
        } else if (value < min || value > max) {
            printf(">> Error: Please enter a number between %d and %d.\n", min, max);
        } else {
            return value;
        }
    }
}

/**
 * Helper for qsort (descending by score)
 */
int compare_records(const void *a, const void *b) {
    const ScoreRecord *recA = (const ScoreRecord *)a;
    const ScoreRecord *recB = (const ScoreRecord *)b;
    return (recB->final_score - recA->final_score);
}

/**
 * Loads binary ranking file, adds new score, sorts and overwrites file,
 * keeping only HISTORY_LIMIT top scores.
 */
void update_ranking_binary(const char *name, int score) {
    FILE *fp = fopen(RANKING_FILE, "rb");
    ScoreRecord table[100];
    int count = 0;
    if (fp) {
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        rewind(fp);
        count = size / sizeof(ScoreRecord);
        if (count > 90) count = 90;
        fread(table, sizeof(ScoreRecord), count, fp);
        fclose(fp);
    }

    strncpy(table[count].name, name, MAX_NAME - 1);
    table[count].name[MAX_NAME - 1] = '\0';
    table[count].final_score = score;
    table[count].game_date = time(NULL);
    count++;

    qsort(table, count, sizeof(ScoreRecord), compare_records);

    int to_write = (count > HISTORY_LIMIT) ? HISTORY_LIMIT : count;
    fp = fopen(RANKING_FILE, "wb");
    if (!fp) {
        perror("Error writing ranking");
        return;
    }
    fwrite(table, sizeof(ScoreRecord), to_write, fp);
    fclose(fp);
    printf("\n[SYSTEM] Score table updated.\n");
}

void show_ranking(void) {
    FILE *fp = fopen(RANKING_FILE, "rb");
    if (!fp) {
        printf("No game history yet. Be the first!\n");
        return;
    }
    ScoreRecord r;
    printf("\n--- HALL OF FAME (TOP %d) ---\n", HISTORY_LIMIT);
    printf("%-4s %-20s %-10s %-12s\n", "Pos", "Name", "Score", "Date");
    printf("------------------------------------------------\n");

    int i = 1;
    while (fread(&r, sizeof(ScoreRecord), 1, fp)) {
        char dateStr[20];
        strftime(dateStr, 20, "%Y-%m-%d", localtime(&r.game_date));
        printf("%d.   %-20s %-10d %-12s\n", i++, r.name, r.final_score, dateStr);
    }
    printf("------------------------------------------------\n");
    fclose(fp);
}

int get_bet_amount(void) {
    return (int)read_number("Enter bet amount: ", 1, g_player.money);
}

int choose_bet_type(void) {
    printf("\n--- BET TYPES ---\n");
    printf("1. Straight Up (Single number)     [35:1]\n");
    printf("2. Column                         [2:1]\n");
    printf("3. Dozen                          [2:1]\n");
    printf("4. Odd / Even                     [1:1]\n");
    printf("5. Red / Black                    [1:1]\n");
    printf("6. Low (1-18) / High (19-36)      [1:1]\n");
    return (int)read_number("Your choice: ", 1, 6);
}

int bet_details(int b) {
    int c = 0;
    if (b == 1) {
        c = (int)read_number("Pick a number (0-36): ", 0, MAX_ROULETTE_NUM);
    } else if (b == 2) {
        printf("1. Column 1 (1, 4, 7...)\n2. Column 2 (2, 5, 8...)\n3. Column 3 (3, 6, 9...)\n");
        c = (int)read_number("Choose column: ", 1, 3);
        c += 40;
    } else if (b == 3) {
        printf("1. 1-12\n2. 13-24\n3. 25-36\n");
        c = (int)read_number("Choose dozen: ", 1, 3);
        c += 50;
    } else if (b == 4) {
        printf("1. Odd\n2. Even\n");
        c = (int)read_number("Choose: ", 1, 2);
        c += 60;
    } else if (b == 5) {
        printf("1. Red\n2. Black\n");
        c = (int)read_number("Choose: ", 1, 2);
        c += 70;
    } else if (b == 6) {
        printf("1. 1-18\n2. 19-36\n");
        c = (int)read_number("Choose range: ", 1, 2);
        c += 80;
    }
    return c;
}

/**
 * Simulates wheel spin with system-dependent delay (Windows/Linux).
 */
void roulette_animation(int result) {
    printf("\nThe wheel is spinning...\n");
    for (int i = 0; i < 15; i++) {
        int r = rand() % WHEEL_SLOTS;
        printf("\r[ %d ]   ", r);
        fflush(stdout);
        SLEEP_MS(50 + (i * 20));
    }

    printf("\n\n>>> RESULT: ");
    if (colors[result] == 1) printf("\033[1;31m%d (Red)\033[0m", result);
    else if (colors[result] == 2) printf("\033[1;30m%d (Black)\033[0m", result);
    else printf("\033[1;32m%d (Green/Zero)\033[0m", result);
    printf(" <<<\n\n");
}

/**
 * Calculates winnings for given stake and result (used in Monte Carlo simulation).
 * Returns amount won (0 on loss). Does not modify g_player or print.
 */
int calculate_winnings_simulation(int choice, int spin, int spin_color, const int *color_table, int stake) {
    (void)color_table;
    int multiplier = 0;

    if (choice <= MAX_ROULETTE_NUM) {
        if (choice == spin) multiplier = 36;
    }
    else if (choice > 40 && choice < 50) {
        int column = choice - 40;
        if (spin != 0) {
            if ((column == 1 && spin % 3 == 1) ||
                (column == 2 && spin % 3 == 2) ||
                (column == 3 && spin % 3 == 0)) multiplier = 3;
        }
    }
    else if (choice > 50 && choice < 60) {
        int dozen = choice - 50;
        if (spin != 0) {
            if ((dozen == 1 && spin <= 12) ||
                (dozen == 2 && spin > 12 && spin <= 24) ||
                (dozen == 3 && spin > 24)) multiplier = 3;
        }
    }
    else if (choice > 60 && choice < 70) {
        int type = choice - 60;
        if (spin != 0) {
            if ((type == 2 && spin % 2 == 0) || (type == 1 && spin % 2 != 0)) multiplier = 2;
        }
    }
    else if (choice > 70 && choice < 80) {
        int type = choice - 70;
        if (spin != 0 && spin_color == type) multiplier = 2;
    }
    else if (choice > 80) {
        int type = choice - 80;
        if (spin != 0) {
            if ((type == 1 && spin <= 18) || (type == 2 && spin >= 19)) multiplier = 2;
        }
    }

    return (multiplier > 0) ? (stake * multiplier) : 0;
}

int calculate_winnings(int choice, int spin, int spin_color, const int *color_table) {
    int winnings = calculate_winnings_simulation(choice, spin, spin_color, color_table, g_player.current_bet);

    if (winnings > 0) {
        printf(">>> CONGRATULATIONS! You win: %d <<<\n", winnings);
    } else {
        printf(">>> Sorry, you lost your bet of %d. <<<\n", g_player.current_bet);
    }

    g_player.money -= g_player.current_bet;
    return winnings;
}

/**
 * Monte Carlo simulation: many independent sessions with the same bet.
 * Estimates outcome distribution (mean, min, max, bankruptcy rate) without changing player state.
 */
void monte_carlo_simulation(void) {
    printf("\n========== MONTE CARLO SIMULATION ==========\n");
    printf("Simulating many sessions with a fixed bet.\n\n");

    int start_balance = (int)read_number("Starting balance per session (min 10): ", 10, 10000000);
    int stake = (int)read_number("Stake per spin (1 to starting balance): ", 1, start_balance);
    int num_sessions = (int)read_number("Number of sessions to simulate (e.g. 1000): ", 10, 100000);
    int max_spins = (int)read_number("Max spins per session (e.g. 500): ", 10, 100000);

    printf("\n--- Bet parameters (same as in game) ---\n");
    int bet_type = choose_bet_type();
    int choice = bet_details(bet_type);

    printf("\nRunning %d sessions, max %d spins each...\n", num_sessions, max_spins);

    long long total_final = 0;
    int min_final = start_balance;
    int max_final = start_balance;
    int bankruptcies = 0;

    for (int s = 0; s < num_sessions; s++) {
        int balance = start_balance;
        int spins_done = 0;

        while (balance >= stake && spins_done < max_spins) {
            int spin = rand() % WHEEL_SLOTS;
            int winnings = calculate_winnings_simulation(choice, spin, colors[spin], colors, stake);
            balance -= stake;
            balance += winnings;
            spins_done++;
        }

        if (balance <= 0) bankruptcies++;
        if (balance < min_final) min_final = balance;
        if (balance > max_final) max_final = balance;
        total_final += balance;
    }

    double mean = (double)total_final / num_sessions;
    double pct_bankrupt = 100.0 * bankruptcies / num_sessions;

    printf("\n---------- MONTE CARLO SIMULATION RESULTS ----------\n");
    printf("  Sessions:              %d\n", num_sessions);
    printf("  Starting balance:      %d\n", start_balance);
    printf("  Stake:                 %d\n", stake);
    printf("  Mean final balance:    %.2f\n", mean);
    printf("  Min final balance:     %d\n", min_final);
    printf("  Max final balance:     %d\n", max_final);
    printf("  Bankruptcies:          %d (%.2f%%)\n", bankruptcies, pct_bankrupt);
    printf("  Mean profit per session: %.2f\n", mean - start_balance);
    printf("-----------------------------------------------------\n\n");
}

void show_game_status(void) {
    printf("################################\n");
    printf("# PLAYER: %-20s #\n", g_player.name);
    printf("# BALANCE: %-19d #\n", g_player.money);
    printf("################################\n");
}

bool ask_quit(void) {
    printf("\nDecision:\n1. Keep playing\n2. Cash out and quit\n");
    int decision = (int)read_number("Choice: ", 1, 2);
    return (decision == 2);
}
