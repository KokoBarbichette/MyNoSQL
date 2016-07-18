#include "json.h"

void JSON_stringify(HashMap *map, char *outputPath)
{
    HashMapEntry *entry = NULL, *subEntry = NULL;
    HashMap *subMap = NULL;
    int i, j;
    FILE* outputFile = fopen(outputPath, "w");
    if(map == NULL || outputFile == NULL)
        return;
    for(i = 0 ; i < map->slots ; i++)
    {
        if(i > 0)
        {
            fprintf(outputFile, ",\n");
        }
        fprintf(outputFile, "{");
        entry = map->entries[i];
        while(entry != NULL)
        {
            subMap = (HashMap*) entry;
            for(j = 0 ; j < subMap->slots ; j++)
            {
                if(j > 0)
                {
                    fprintf(outputFile, ",");
                }
                fprintf(outputFile, "{");
                subEntry = subMap->entries[j];
                while(subEntry != NULL)
                {
                    fprintf(outputFile, "%s:#%0.2x", subEntry->key, ((char*)(subEntry->value)));
                    subEntry = subEntry->next;
                    if(subEntry != NULL)
                    {
                        fprintf(outputFile, ",");
                    }
                }
                fprintf(outputFile, "}");
            }
            entry = entry->next;
            fprintf(outputFile, "}");
        }
    }
}

char* encodeEntry(HashMapEntry* entry)
{
    char *str = NULL;
    if(entry != NULL)
    {
        str = (char*) malloc((entry->elemSize << 1) + 3);
        char hexa[50] = "";
        int i = 0;
        str[0] = '#';
        for(i = entry->elemSize - 3 ; i >= 0 ; i--)
        {
            sprintf(hexa, "%0.2x", *(((char*)(entry->value)) + i));
            strcat(str, hexa);
        }
        str[entry->elemSize - 2] = '#';
        str[entry->elemSize - 1] = '\0';
    }
    return str;
}

char* decodeEntry(char *entry)
{
    char *buff = NULL;
    int i = 0;
    entry++;
    if(entry != NULL)
    {
        buff = (char*) malloc((strlen(entry) * sizeof(char)) >> 1);
        while(*entry != '\0')
        {
            buff[i] = (decodeHexa(*entry) << 4) + decodeHexa(*(++entry));
            ++entry;
        }
    }
    return buff;
}

char decodeHexa(char fourBitsIdentifier)
{
    return fourBitsIdentifier >= '0' && fourBitsIdentifier <= '9' ? fourBitsIdentifier - '0' : fourBitsIdentifier - 'A' + 10;
}

HashMap* JSON_parse(char* json)
{
    char *lastPos = NULL;
    int err;
    regex_t preg;
    const char *regex = "({.*)|(([a-zA-Z][:alnum:]*)[:space:]*:[:space:]*([#a-zA-Z0-9 {_-]+))";
    regmatch_t *pmatch = NULL;
    HashMap* map = NULL;
    unsigned char reached = 0;
    int end = 0;
    char key[256] = "";
    char value[256] = "";

    if(json == NULL || *json == '\0')
    {
        return NULL;
    }

    lastPos = strchr(json, '}');
    err = regcomp (&preg, regex, REG_EXTENDED);
    if (err == 0)
    {
        printf("NO ERROR\n");
        int match;
        size_t nmatch = 0;

        nmatch = preg.re_nsub;
        pmatch = malloc (sizeof (*pmatch) * nmatch);
        if (pmatch)
        {
            printf("PMATCH\n");
            while(!reached)
            {
                printf("NOT REACHED\n");
                match = regexec(&preg, json, nmatch, pmatch, 0);
                regfree (&preg);
                if (match == 0)
                {

                    end = pmatch[1].rm_eo;

                    strncpy(key, json+pmatch[0].rm_so, pmatch[0].rm_eo - pmatch[0].rm_so);
                    key[pmatch[0].rm_eo - pmatch[0].rm_so] = '\0';
                    printf("KEY : %s\n", key);

                    if(*(json+pmatch[1].rm_so) == '{')
                    {
                        JSON_parse(json + pmatch[1].rm_so);
                        json = lastPos;
                        lastPos = strchr(lastPos + 1, '}');
                    }

                    else if(*(json+pmatch[1].rm_so) == '#')
                    {
                        strncpy(value, json+pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);
                        value[pmatch[1].rm_eo - pmatch[1].rm_so] = '\0';
                        reached = end >= lastPos;

                        if(!reached)
                        {
                            HashMap_put(map, key, decodeEntry(value), (pmatch[1].rm_eo - pmatch[1].rm_so - 1) / 2);
                        }

                        json = pmatch[1].rm_eo;
                    }
                    else
                    {
                        strncpy(value, json+pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);
                        value[pmatch[1].rm_eo - pmatch[1].rm_so] = '\0';
                        reached = end >= lastPos;

                        if(!reached)
                        {
                            HashMap_put(map, key, value, strlen(value) * sizeof(char));
                        }

                        json = pmatch[1].rm_eo;
                    }
                }
                else
                {
                    reached = 1;
                }
            }

            free(pmatch);
        }
    }

    return map;
}
