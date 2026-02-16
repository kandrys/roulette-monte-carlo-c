#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game_logic.h"

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        printf("==================================================\n");
        printf("                      ROULETTE\n");
        printf("==================================================\n");
        printf("DESCRIPTION:\n");
        printf("  Casino simulator with binary score file.\n");
        printf("USAGE:\n");
        printf("  ./ruletka       -> Start the game\n");
        printf("  ./ruletka -h    -> Show this help\n");
        return 0;
    }

    srand(time(NULL));
    printf("Welcome to the Casino\n");
    printf("Enter your name: ");

    if (fgets(g_player.name, MAX_NAME, stdin)) {
        g_player.name[strcspn(g_player.name, "\n")] = 0;
    }
    if (strlen(g_player.name) == 0) {
        strcpy(g_player.name, "Anonymous");
    }

    int starting_balance = (int)read_number("Enter starting balance (min 10): ", 10, 1000000);
    g_player.money = starting_balance;

    printf("\n1. Start game\n2. Monte Carlo simulation (estimate outcomes)\n");
    int option = (int)read_number("Choice: ", 1, 2);
    if (option == 2) {
        monte_carlo_simulation();
        printf("You can now start a normal game.\n");
    }

    show_ranking();
    bool game_over = false;
    while (!game_over) {
        if (g_player.money <= 0) {
            printf("\nYou have no funds to continue.\n");
            break;
        }

        show_game_status();

        g_player.current_bet = get_bet_amount();
        int bet_type = choose_bet_type();
        int choice = bet_details(bet_type);

        int spin_result = rand() % WHEEL_SLOTS;
        roulette_animation(spin_result);
        int winnings = calculate_winnings(choice, spin_result, colors[spin_result], colors);
        g_player.money += winnings;

        if (g_player.money > 0) {
            game_over = ask_quit();
        }
    }

    int profit = g_player.money - starting_balance;
    printf("\n------------------------------------------\n");
    printf("GAME OVER. Final balance: %d\n", g_player.money);
    printf("Session balance: %s%d\n", profit >= 0 ? "+" : "", profit);
    printf("------------------------------------------\n");

    update_ranking_binary(g_player.name, g_player.money);

    return 0;
}
