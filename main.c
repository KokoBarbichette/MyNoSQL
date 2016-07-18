#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "map.h"
#include "json.h"

#define COLLECTION "-collection="
#define SORT "-collection="
#define INSERT "-insert="
#define SET "-set="
#define FIND "-find="
#define REMOVE "-remove="
#define WHERE "-where="

int main(int argc, char** argv)
{
    char* collection = NULL;
    char* opt = NULL;
    char* modifier = NULL;
    char* sort = NULL;
    char* projection = NULL;
    char* where = NULL;
    unsigned int i = 0;
    unsigned char opmask = 0x00;

    for(i = 1 ; i < argc ; i++)
    {
        printf("argv %d : %s\n", i, argv[i]);
        if(argv[i] == strstr(argv[i], COLLECTION))
        {
            collection = argv[i] + strlen(COLLECTION);
        }

        else if(argv[i] == strstr(argv[i], SORT))
        {
            sort = argv[i] + strlen(SORT);
        }

        else if(argv[i] == strstr(argv[i], WHERE))
        {
            where = argv[i] + strlen(WHERE);
        }

        else if(argv[i] == strstr(argv[i], SET))
        {
            modifier = argv[i] + strlen(SET);
            opmask |= 0x01;
        }

        else if(argv[i] == strstr(argv[i], FIND))
        {
            modifier = argv[i] + strlen(FIND);
            opmask |= 0x02;
        }

        else if(argv[i] == strstr(argv[i], INSERT))
        {
            modifier = argv[i] + strlen(INSERT);
            opmask |= 0x04;
        }

        else
        {
            exit(1);
        }
    }

    FILE* jsonFile = fopen(collection, "r+");
    printf("collection : %s : %p\n", collection, jsonFile);
    char json[4096] = "";
    HashMap* data = NULL;

    fread(json, 4096, 1, jsonFile);

    data = JSON_parse(json);
    JSON_stringify(data, "out.json");

    HashMap_free(&data);
    fclose(jsonFile);

    HashMap *map = HashMap_create(10, 0.7, 2);
    HashMap *map2 = HashMap_create(10, 0.7, 2);
    clock_t start = clock();
    int max = 50;

    printf("Start : %li\n", start);
    for(i = 0 ; i < max ; i++)
    {
        char str[16];
        sprintf(str, "str%d", i + 1);
        HashMap_put(map, str, &i, sizeof(i));
    }
    printf("Time after put : %li\n", clock() - start);

    for(i = 0 ; i < max ; i++)
    {
        char str[150];
        sprintf(str, "str%d", i + 1);
        HashMap_get(map, str);
    }

    printf("Time after get : %li\n", clock() - start);

    for(i = 0 ; i < map->slots ; i++)
    {
        HashMapEntry* entryList = map->entries[i];
        while(entryList != NULL)
        {
            printf("On slot %d : ", i);
            HashMapEntry_print(entryList);
            entryList = entryList->next;
        }
    }

    HashMap_put(map2, "map", map, sizeof(map));

    JSON_stringify(map2, "out2.json");

    HashMap_free(&map2);
    return 0;
}

