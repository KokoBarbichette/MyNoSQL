#ifndef JSON_H

    #define JSON_H

    #include "map.h"
    #include <regex.h>

    #define DEFAULT_JSON_SLOTS 10
    #define DEFAULT_JSON_LOAD_FACTOR 10
    #define DEFAULT_JSON_GROW_FACTOR 10

    void JSON_stringify(HashMap *map, char *outputPath);
    HashMap* JSON_parse(char* json);
    char decodeHexa(char fourBitsIdentifier);

#endif // JSON_H
