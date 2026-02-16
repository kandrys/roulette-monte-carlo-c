#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdbool.h>
#include <time.h>
#include <stdio.h>


#define MAX_NAME 50
#define RANKING_FILE "ranking.bin"
#define WHEEL_SLOTS 37
#define MAX_ROULETTE_NUM 36
#define HISTORY_LIMIT 10

typedef struct {
    int money;
    char name[MAX_NAME];
    int current_bet;
} Player;

typedef struct {
    char name[MAX_NAME];
    int final_score;
    time_t game_date;
} ScoreRecord;

extern int colors[WHEEL_SLOTS];
extern Player g_player;

long read_number(const char *prompt, int min, int max);
void update_ranking_binary(const char *name, int score);
void show_ranking(void);
int get_bet_amount(void);
int choose_bet_type(void);
int bet_details(int bet_type);
int calculate_winnings(int choice, int spin, int spin_color, const int *color_table);
int calculate_winnings_simulation(int choice, int spin, int spin_color, const int *color_table, int stake);
void roulette_animation(int result);
void show_game_status(void);
bool ask_quit(void);
void monte_carlo_simulation(void);

#endif
