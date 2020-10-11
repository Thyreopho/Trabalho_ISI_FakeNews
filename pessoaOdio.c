#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define STRING_SM 255
#define STRING_MD 1023 
#define STRING_LG 65535

typedef struct FakeNews {
    char uuid[STRING_SM];
    int ord_in_thread;
    char author[STRING_SM];
    time_t published;
    char title[STRING_SM];
    char text[STRING_LG];
    char language[STRING_SM];
    time_t crawled;
    char site_url[STRING_SM];
    char country[STRING_SM];
    int domain_rank;
    char thread_title[STRING_SM];
    short int spam_score;
    char main_img_url[STRING_SM];
    int replies_count;
    int participants_count;
    int likes;
    int comments;
    int shares;
    char type[STRING_SM];
} FakeNews;

time_t strToDateTime (char source[STRING_SM]) {
    int year, month, day, hour, min, sec;
    sscanf(source, "%4d-%2d-%2dT%2d:%2d:%2d", &year, &month, &day, &hour, &min, &sec);

    struct tm breakdown = {0};

    breakdown.tm_year = year - 1900; /* years since 1900 */
    breakdown.tm_mon = month - 1;
    breakdown.tm_mday = day;
    breakdown.tm_hour = hour;
    breakdown.tm_min = min;
    breakdown.tm_sec = sec;

    time_t result = mktime(&breakdown);
}

void jumpAttr (char source[STRING_LG], int *i) {
    if (source[*i] != ',') {
        
        bool qMark = source[*i] == '"';

        if (source[*i+1] == '"' && source[*i+2] == '"') {
            *i = *i + 2;
        }

        if (qMark) {
            *i = *i + 1; //jump "

            while (source[*i] != '"') {
                *i = *i + 1;
                if (source[*i] == '"' && source[*i+1] == '"') {
                    *i = *i + 2;
                }
            }

            *i = *i + 1; //jump "
        } else {
            while (source[*i] != ',') {
                *i = *i + 1;
            }
        }
        
    }

    *i = *i + 1; //jump ,
}

void readStringSm (char out[STRING_SM], char source[STRING_LG], int *i) {
    if (source[*i] != ',') {
        
        bool qMark = source[*i] == '"';

        if (source[*i+1] == '"' && source[*i+2] == '"') {
            *i = *i + 2;
        }

        int j = 0;
        if (qMark) {
            *i = *i + 1; //jump "

            while (source[*i] != '"') {
                out[j] = source[*i];
                j++;
                *i = *i + 1;
                if (source[*i] == '"' && source[*i+1] == '"') {
                    out[j] = source[*i];
                    *i = *i + 2;
                }
            }

            *i = *i + 1; //jump "
        } else {
            while (source[*i] != ',') {
                out[j] = source[*i];
                j++;
                *i = *i + 1;
            }
        }
        
        out[j] = '\0';
    } else {
        out[0] = '\0';
    }

    *i = *i + 1; //jump ,
}

void readStringMd (char out[STRING_MD], char source[STRING_LG], int *i) {
    if (source[*i] != ',') {
        
        bool qMark = source[*i] == '"';

        if (source[*i+1] == '"' && source[*i+2] == '"') {
            *i = *i + 2;
        }

        int j = 0;
        if (qMark) {
            *i = *i + 1; //jump "

            while (source[*i] != '"') {
                out[j] = source[*i];
                j++;
                *i = *i + 1;
                if (source[*i] == '"' && source[*i+1] == '"') {
                    out[j] = source[*i];
                    *i = *i + 2;
                }
            }

            *i = *i + 1; //jump "
        } else {
            while (source[*i] != ',') {
                out[j] = source[*i];
                j++;
                *i = *i + 1;
            }
        }
        
        out[j] = '\0';
    } else {
        out[0] = '\0';
    }

    *i = *i + 1; //jump ,
}

void readStringLg (char out[STRING_LG], char source[STRING_LG], int *i) {
    if (source[*i] != ',') {
        
        bool qMark = source[*i] == '"';

        if (source[*i+1] == '"' && source[*i+2] == '"') {
            *i = *i + 2;
        }

        int j = 0;
        if (qMark) {
            *i = *i + 1; //jump "

            while (source[*i] != '"') {
                out[j] = source[*i];
                j++;
                *i = *i + 1;
                if (source[*i] == '"' && source[*i+1] == '"') {
                    out[j] = source[*i];
                    *i = *i + 2;
                }
            }

            *i = *i + 1; //jump "
        } else {
            while (source[*i] != ',') {
                out[j] = source[*i];
                j++;
                *i = *i + 1;
            }
        }
        
        out[j] = '\0';
    } else {
        out[0] = '\0';
    }

    *i = *i + 1; //jump ,
}

bool isFakeEnd(char *buff, int i) {
    int j = 0;

    while (buff[i-j] != ',' && j < 14) {
        j++;
    }

    return buff[i-j] == ',' && 
    (
        //Ordened by recurrence
        strcmp(&buff[i-j], ",\"bs\"\n") == 0 ||
        strcmp(&buff[i-j], ",\"bias\"\n") == 0 ||
        strcmp(&buff[i-j], ",\"conspiracy\"\n") == 0 ||
        strcmp(&buff[i-j], ",\"hate\"\n") == 0 ||
        strcmp(&buff[i-j], ",\"satire\"\n") == 0 ||
        strcmp(&buff[i-j], ",\"fake\"\n") == 0 ||
        strcmp(&buff[i-j], ",\"junksci\"\n") == 0 ||
        strcmp(&buff[i-j], ",\"state\"\n") == 0
    );
}

void splitLine (FakeNews *out, FILE *source, char *buffSm, char *buffLg, char *auxBuff) {

    fgets(buffLg, STRING_LG, source);

    int i = 0;
    /*debug*/// printf("reading...\n");

    //#region readAllData

        while (buffLg[i] != '\0') { i++; }
        if (!isFakeEnd(buffLg, i)) {
            do {
                fgets(auxBuff, STRING_LG, source);
                i = 0;
                while (auxBuff[i] != '\0') { i++; }
            } while (!isFakeEnd(auxBuff, i));
            strcat(buffLg, auxBuff);
        }
    //#endregion

    /*debug*/// printf("readable\n");

    i = 0;

    //#region uuid
        //jumpAttr(buffLg, &i);
        readStringSm(out->uuid, buffLg, &i);
        /*debug*/// printf("\tuuid-ok\n");
    //#endregion

    //#region ord_in_thread
        //jumpAttr(buffLg, &i);
        readStringSm(buffSm, buffLg, &i);
        out->ord_in_thread = atoi(buffSm);
        /*debug*/// printf("\tord_in_thread-ok\n");
    //#endregion

    //#region author
        //jumpAttr(buffLg, &i);
        readStringSm(out->author, buffLg, &i);
        /*debug*/// printf("\tauthor-ok\n");
    //#endregion

    //#region published
        readStringSm(buffSm, buffLg, &i);
        out->published = strToDateTime(buffSm);
        /*debug*/// printf("\tpublished-ok\n");
    //#endregion

    //#region title
        readStringSm(out->title, buffLg, &i);
        /*debug*/// printf("\ttitle-ok\n");
    //#endregion

    //#region text
        readStringLg(out->text, buffLg, &i);
        /*debug*/// printf("\ttext-ok\n");
    //#endregion

    //#region language
        readStringSm(out->language, buffLg, &i);
        /*debug*/// printf("\tlanguage-ok\n");
    //#endregion

    //#region crawled
        readStringSm(buffSm, buffLg, &i);
        out->crawled = strToDateTime(buffSm);
        /*debug*/// printf("\tcrawled-ok\n");
    //#endregion

    //#region site_url
        //jumpAttr(buffLg, &i);
        readStringSm(out->site_url, buffLg, &i);
        /*debug*/// printf("\tsite_url-ok\n");
    //#endregion

    //#region country
        readStringSm(out->country, buffLg, &i);
        /*debug*/// printf("\tcountry-ok\n");
    //#endregion

    //#region domain_rank
        //jumpAttr(buffLg, &i);
        readStringSm(buffSm, buffLg, &i);
        out->domain_rank = atoi(buffSm);
        /*debug*/// printf("\tdomain_rank-ok\n");
    //#endregion

    //#region thread_title
        //jumpAttr(buffLg, &i);
        readStringSm(out->thread_title, buffLg, &i);
        /*debug*/// printf("\tthread_title-ok\n");
    //#endregion

    //#region spam_score
        //jumpAttr(buffLg, &i);
        readStringSm(buffSm, buffLg, &i);
        out->spam_score = atoi(buffSm);
        /*debug*/// printf("\tspam_score-ok\n");
    //#endregion

    //#region main_img_url
        //jumpAttr(buffLg, &i);
        readStringSm(out->main_img_url, buffLg, &i);
        /*debug*/// printf("\tmain_img_url-ok\n");
    //#endregion

    //#region replies_count
        readStringSm(buffSm, buffLg, &i);
        out->replies_count = atoi(buffSm);
        /*debug*/// printf("\treplies_count-ok\n");
    //#endregion

    //#region participants_count
        readStringSm(buffSm, buffLg, &i);
        out->participants_count = atoi(buffSm);
        /*debug*/// printf("\tparticipants_count-ok\n");
    //#endregion

    //#region likes
        readStringSm(buffSm, buffLg, &i);
        out->likes = atoi(buffSm);
        /*debug*/// printf("\tlikes-ok\n");
    //#endregion

    //#region comments
        readStringSm(buffSm, buffLg, &i);
        out->comments = atoi(buffSm);
        /*debug*/// printf("\tcomments-ok\n");
    //#endregion

    //#region shares
        readStringSm(buffSm, buffLg, &i);
        out->shares = atoi(buffSm);
        /*debug*/// printf("\tshares-ok\n");
    //#endregion

    //#region type
        readStringSm(out->type, buffLg, &i);
        /*debug*/// printf("\ttype-ok\n");
    //#endregion
}

//#region user area

    //#region Global vars

        long int trump_attacks;
        long int obama_attacks;
        long int hillary_attacks;

        int new_eng_p;

        //#region election_set
            char trump_set[6];
            char hillary_set[8];
            char obama_set[6];
        //#endregion

    //#endregion

    void before () {
        trump_attacks = 0;
        obama_attacks = 0;
        hillary_attacks = 0;
        //#region election_set settings
            strcpy(trump_set, "trump");
            strcpy(hillary_set, "hillary");
            strcpy(obama_set, "obama");
        //#endregion
    }
    
    bool sayTrump (char* str) {
        int i = 0;
        short int trump_p = 0;
        
        char tmp = str[i];
        char e_low;
        char e_upp;
        while (
            tmp != '\0' &&
            trump_set[trump_p] != '\0'
        ) {

            e_low = trump_set[trump_p];
            e_upp = (char)((int)e_low - 32);
            if (tmp == e_low || tmp == e_upp) {
                trump_p++;
            } else {
                trump_p = 0;
            }

            i++;
            tmp = str[i];
        }

        return tmp != '\0';
    }
    
    bool sayHillary (char* str) {
        int i = 0;
        short int hillary_p = 0;
        
        char tmp = str[i];
        char e_low;
        char e_upp;
        while (
            tmp != '\0' &&
            hillary_set[hillary_p] != '\0'
        ) {

            e_low = hillary_set[hillary_p];
            e_upp = (char)((int)e_low - 32);
            if (tmp == e_low || tmp == e_upp) {
                hillary_p++;
            } else {
                hillary_p = 0;
            }

            i++;
            tmp = str[i];
        }

        return tmp != '\0';
    }
    
    bool sayObama (char* str) {
        int i = 0;
        short int obama_p = 0;
        
        char tmp = str[i];
        char e_low;
        char e_upp;
        while (
            tmp != '\0' &&
            obama_set[obama_p] != '\0'
        ) {

            e_low = obama_set[obama_p];
            e_upp = (char)((int)e_low - 32);
            if (tmp == e_low || tmp == e_upp) {
                obama_p++;
            } else {
                obama_p = 0;
            }

            i++;
            tmp = str[i];
        }

        return tmp != '\0';
    }

    bool filter (FakeNews *line) {
        return
            strcmp(line->country, "US") == 0 &&
            strcmp(line->language, "english") == 0 &&
            strcmp(line->type, "hate") == 0;
    }

    void middleware (FakeNews *line) {

        if (filter(line)) {
            bool sT = sayTrump(line->title) || sayTrump(line->text), 
            sH = sayHillary(line->title) || sayHillary(line->text), 
            sO = sayObama(line->title) || sayObama(line->text);

            if (sT) {
                trump_attacks++;
            } 
            if (sH) {
                hillary_attacks++;
            } 
            if (sO) {
                obama_attacks++;
            }
        }
    }

    void finally () {
        printf("\"pessoa_publica\",\"quantidade_de_noticias_de_odio\"\n");
        printf("Trump, %li\n", trump_attacks);
        printf("Hillary, %li\n", hillary_attacks);
        printf("Obama, %li\n", obama_attacks);
    }

//#endregion

int main (int argc, char **argv) {

    char buff[STRING_LG];
    FakeNews *out = (FakeNews*) malloc(sizeof(FakeNews));

    char *buffSm, *buffLg, *auxBuff;
    buffSm = (char *) malloc(STRING_SM);
    buffLg = (char *) malloc(STRING_LG);
    auxBuff = (char *) malloc(STRING_LG);

    before();

    FILE *fp = fopen("fake.csv", "r");
    fgets(buff, STRING_LG, fp); //jump header

    while (!feof(fp)) {
        /*debug*/// printf("splitting...\n");
        splitLine(out, fp, buffSm, buffLg, auxBuff);
        /*debug*/// printf("splittable\n");
        middleware(out);
    }
    fclose(fp);
    finally();

    free(buffSm);
    free(buffLg);
    free(auxBuff);

    return 0;
}