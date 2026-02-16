/*
 * Fills ranking.bin with sample records: various players, scores,
 * and dates spread over the last 2 months. Uses the same binary
 * format as the game (ScoreRecord).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 50
#define RANKING_FILE "ranking.bin"

typedef struct {
    char name[MAX_NAME];
    int final_score;
    time_t game_date;
} ScoreRecord;

static void set_record(ScoreRecord *r, const char *name, int score, time_t when) {
    strncpy(r->name, name, MAX_NAME - 1);
    r->name[MAX_NAME - 1] = '\0';
    r->final_score = score;
    r->game_date = when;
}

/* Seconds per day for easy date math */
#define SEC_PER_DAY (24 * 60 * 60)

int main(void) {
    time_t now = time(NULL);
    ScoreRecord rec[30];
    int n = 0;

    /* ~2 months ago to now: spread sample data */
    time_t two_months_ago = now - (60 * SEC_PER_DAY);

    set_record(&rec[n++], "Anna",      1250,  two_months_ago +  0 * SEC_PER_DAY);
    set_record(&rec[n++], "Bartek",      320,  two_months_ago +  3 * SEC_PER_DAY);
    set_record(&rec[n++], "Celina",     2100,  two_months_ago +  5 * SEC_PER_DAY);
    set_record(&rec[n++], "Dawid",        0,  two_months_ago +  7 * SEC_PER_DAY);
    set_record(&rec[n++], "Ewa",        890,  two_months_ago + 10 * SEC_PER_DAY);
    set_record(&rec[n++], "Filip",     3400,  two_months_ago + 12 * SEC_PER_DAY);
    set_record(&rec[n++], "Gosia",      156,  two_months_ago + 15 * SEC_PER_DAY);
    set_record(&rec[n++], "Hubert",    7800,  two_months_ago + 18 * SEC_PER_DAY);
    set_record(&rec[n++], "Iga",       420,  two_months_ago + 20 * SEC_PER_DAY);
    set_record(&rec[n++], "Jan",      11000,  two_months_ago + 22 * SEC_PER_DAY);
    set_record(&rec[n++], "Kasia",      95,  two_months_ago + 25 * SEC_PER_DAY);
    set_record(&rec[n++], "Lukasz",   25000,  two_months_ago + 28 * SEC_PER_DAY);
    set_record(&rec[n++], "Magda",     330,  two_months_ago + 32 * SEC_PER_DAY);
    set_record(&rec[n++], "Norbert",  1500,  two_months_ago + 35 * SEC_PER_DAY);
    set_record(&rec[n++], "Ola",      6700,  two_months_ago + 38 * SEC_PER_DAY);
    set_record(&rec[n++], "Piotr",      12,  two_months_ago + 40 * SEC_PER_DAY);
    set_record(&rec[n++], "Robert",   4200,  two_months_ago + 42 * SEC_PER_DAY);
    set_record(&rec[n++], "Sylwia",   8900,  two_months_ago + 45 * SEC_PER_DAY);
    set_record(&rec[n++], "Tomek",     555,  two_months_ago + 48 * SEC_PER_DAY);
    set_record(&rec[n++], "Ula",        78,  two_months_ago + 50 * SEC_PER_DAY);
    set_record(&rec[n++], "Wojtek",  12000,  two_months_ago + 52 * SEC_PER_DAY);
    set_record(&rec[n++], "Zosia",    2100,  two_months_ago + 54 * SEC_PER_DAY);
    set_record(&rec[n++], "Marcin",     0,  two_months_ago + 56 * SEC_PER_DAY);
    set_record(&rec[n++], "Natalia",  4400,  two_months_ago + 58 * SEC_PER_DAY);
    set_record(&rec[n++], "Kamil",    990,  now - 5 * SEC_PER_DAY);
    set_record(&rec[n++], "Dominika", 7700,  now - 3 * SEC_PER_DAY);
    set_record(&rec[n++], "Jakub",   18500,  now - 1 * SEC_PER_DAY);
    set_record(&rec[n++], "Aleksandra", 320,  now - 12 * 3600);  /* 12 h ago */
    set_record(&rec[n++], "Michal",   5100,  now);

    FILE *fp = fopen(RANKING_FILE, "wb");
    if (!fp) {
        perror(RANKING_FILE);
        return 1;
    }
    if (fwrite(rec, sizeof(ScoreRecord), (size_t)n, fp) != (size_t)n) {
        perror("fwrite");
        fclose(fp);
        return 1;
    }
    fclose(fp);
    printf("Written %d sample records to %s (last 2 months).\n", n, RANKING_FILE);
    return 0;
}
