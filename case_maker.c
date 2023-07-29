#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "autocomplete.h"

#define CITIES "cities.txt"
#define CITIESNAME "Cities"
#define WIKI "wiktionary.txt"
#define WIKINAME "Wiki"

#define CITIES_OUT "cases/cities_sorted.c"
#define WIKI_OUT "cases/wiktionary_sorted.c"

#define CITIES_TEST_CASES "cases/cities_cases.c"
#define WIKI_TEST_CASES "cases/wiktionary_cases.c"

#define HEADER "cases/cases.h"

#define ENDCASE "i don't really know a better way to do this"
#define EMPTYCASE "{\"AUTOCOMPLETE RETURNS EMPTY 0 VALUES THIS IS NOT A VALUE\", 0.}"

int main(void)
{
    char *files[] = { CITIES, WIKI };
    char *filesName[] = { CITIESNAME, WIKINAME };
    char *files_out[] = { CITIES_OUT, WIKI_OUT };
    char *files_cases[] = { CITIES_TEST_CASES, WIKI_TEST_CASES };

    // ADD MORE CASES HERE
    // No illegal intensifiers in case names because it prints to a c file
    char cases[][100][200] = { {"Tor", "Cock", "noCityWithThisName", "Bru", "Pen", "Dick", "Los", "Cam", "Tabon", "Lig", "Ball", "Hi", "Hell", ENDCASE},{"sus", "impo", "ap", "de", "be", "tre", "th", "can", "dick", "balls", "never", "going", "to", "give", "you", "up", "mom", ENDCASE} };

    int filesLen = (sizeof files) / (sizeof(char *));

    FILE *header = fopen(HEADER, "w");
    fputs("#ifndef CASES_H\n#define CASES_H\n", header);

    FILE *lazy = fopen("lazy.txt", "w"); // i got tired of typing new test cases

    for (int i = 0; i < filesLen; i++) {
        // Full sorted read in lists
        FILE *out = fopen(files_out[i], "w");

        struct term *terms;
        int nterms;
        read_in_terms(&terms, &nterms, files[i]);

        // fputs("#include \"../autocomplete.h\"\nstruct term terms[] = {\n", out);
        fputs("#include \"../autocomplete.h\"\n", out);
        fputs("struct term ", out);
        fputs(filesName[i], out);
        fputs("Sorted", out);
        fputs("[] = {\n", out);

        fputs("extern struct term ", header);
        fputs(filesName[i], header);
        fputs("Sorted", header);
        fputs("[", header);
        char temp[200];
        sprintf(temp, "%d", nterms); // wow useful function would have been nice to learn about before hand
        fputs(temp, header);
        fputs("];\n", header);

        for (int i = 0; i < nterms; i++) {
            fputs("{\"", out);
            fputs(terms[i].term, out);
            fputs("\", ", out);
            char buf[200];
            gcvt(terms[i].weight, 200, buf);
            fputs(buf, out);
            fputs("},\n", out);
        }
        fputs("};", out);


        // Test cases
        FILE *tests = fopen(files_cases[i], "w");
        fputs("#include \"../autocomplete.h\"\n", tests);

        int j = 0;
        while (strcmp(cases[i][j], ENDCASE) != 0) {
            char caseTest[200];
            sprintf(caseTest, "testAutocomplete(terms, nterms, &%sCases[0], (sizeof %sCases) / (sizeof %sCases[0]), \"%s\");\n", cases[i][j], cases[i][j], cases[i][j], cases[i][j]);
            fputs(caseTest, lazy);

            struct term *answer;
            int n_answer;
            autocomplete(&answer, &n_answer, terms, nterms, cases[i][j]);

            fputs("struct term ", tests);
            fputs(cases[i][j], tests);
            fputs("Cases", tests);
            fputs("[] = {\n", tests);

            fputs("extern struct term ", header);
            fputs(cases[i][j], header);
            fputs("Cases", header);
            fputs("[", header);
            char temps[200];
            if (n_answer > 0) {

                sprintf(temps, "%d", n_answer);
            } else {
                sprintf(temps, "%d", 1);
            }
            fputs(temps, header);
            fputs("];\n", header);

            for (int k = 0; k < n_answer; k++) {
                fputs("{\"", tests);
                fputs(answer[k].term, tests);
                fputs("\", ", tests);
                char buf[200];
                gcvt(answer[k].weight, 200, buf);
                fputs(buf, tests);
                fputs("},\n", tests);
            }
            if (n_answer == 0) {
                fputs(EMPTYCASE, tests);
            }

            fputs("};\n", tests);
            free(answer);
            j++;
        }

        free(terms);

        fclose(tests);
        fclose(out);
    }
    fputs("#endif", header);

    fclose(header);
    fclose(lazy);
}