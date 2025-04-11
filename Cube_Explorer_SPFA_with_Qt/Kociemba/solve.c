#include <stdio.h>
#include <stdlib.h>
#include "search.h"

char* CubeSolver(char *arg1,char*arg2)
{
        char patternized[64];
        char* facelets = arg1;
        if (arg2 !=NULL) {
            patternize(facelets, arg2, patternized);
            facelets = patternized;
        }
        char *sol = solution(
            facelets,
            22,
            1000,
            0,
            "cache"
        );
        return sol;
}
