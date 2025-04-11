#ifndef CUBE_SAFE_KOCIEMBA
#define CUBE_SAFE_KOCIEMBA
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <io.h>
#include <time.h>
using namespace std;

/**
 * <pre>
 * The names of the facelet positions of the cube
 *             |************|
 *             |*U1**U2**U3*|
 *             |************|
 *             |*U4**U5**U6*|
 *             |************|
 *             |*U7**U8**U9*|
 *             |************|
 * ************|************|************|************|
 * *L1**L2**L3*|*F1**F2**F3*|*R1**R2**F3*|*B1**B2**B3*|
 * ************|************|************|************|
 * *L4**L5**L6*|*F4**F5**F6*|*R4**R5**R6*|*B4**B5**B6*|
 * ************|************|************|************|
 * *L7**L8**L9*|*F7**F8**F9*|*R7**R8**R9*|*B7**B8**B9*|
 * ************|************|************|************|
 *             |************|
 *             |*D1**D2**D3*|
 *             |************|
 *             |*D4**D5**D6*|
 *             |************|
 *             |*D7**D8**D9*|
 *             |************|
 * </pre>
 *
 *A cube definition string "UBL..." means for example: In position U1 we have the U-color, in position U2 we have the
 * B-color, in position U3 we have the L color etc. according to the order U1, U2, U3, U4, U5, U6, U7, U8, U9, R1, R2,
 * R3, R4, R5, R6, R7, R8, R9, F1, F2, F3, F4, F5, F6, F7, F8, F9, D1, D2, D3, D4, D5, D6, D7, D8, D9, L1, L2, L3, L4,
 * L5, L6, L7, L8, L9, B1, B2, B3, B4, B5, B6, B7, B8, B9 of the enum constants.
 */


//color.h
typedef enum { U, R, F, D, L, B } color_t;
#define COLOR_COUNT 6


//coordcube.h
// Representation of the cube on the coordinate level
#define N_TWIST     2187
#define N_FLIP      2048
#define N_SLICE1    495
#define N_SLICE2    24
#define N_PARITY    2
#define N_URFtoDLF  20160
#define N_FRtoBR    11880
#define N_URtoUL    1320
#define N_UBtoDF    1320
#define N_URtoDF    20160
#define N_URFtoDLB  40320
#define N_URtoBR    479001600
#define N_MOVE      18
typedef struct {
    // All coordinates are 0 for a solved cube except for UBtoDF, which is 114
    short twist;
    short flip;
    short parity;
    short FRtoBR;
    short URFtoDLF;
    short URtoUL;
    short UBtoDF;
    int URtoDF;
} coordcube_t;


//corner.h
// The names of the corner positions of the cube. Corner URF e.g., has an U(p), a R(ight) and a F(ront) facelet
typedef enum {
    URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB
} corner_t;

#define CORNER_COUNT 8


//edge.h
// The names of the edge positions of the cube. Edge UR e.g., has an U(p) and R(ight) facelet.
typedef enum {
    UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR
} edge_t;
#define EDGE_COUNT 12


//cubiecube.h
//Cube on the cubie level
struct cubiecube {
    // initialize to Id-Cube
    // corner permutation
    corner_t cp[8];
    // corner orientation
    signed char co[8];
    // edge permutation
    edge_t ep[12];
    // edge orientation
    signed char eo[12];
};
typedef struct cubiecube cubiecube_t;
// forward declaration
struct facecube;


//facecube.h
//Cube on the facelet level
struct facecube {
    color_t f[54];
};
typedef struct facecube facecube_t;
// forward declaration
struct cubiecube;


//facelet.h
typedef enum {
    U1, U2, U3, U4, U5, U6, U7, U8, U9, R1, R2, R3, R4, R5, R6, R7, R8, R9, F1, F2, F3, F4, F5, F6, F7, F8, F9, D1, D2, D3, D4, D5, D6, D7, D8, D9, L1, L2, L3, L4, L5, L6, L7, L8, L9, B1, B2, B3, B4, B5, B6, B7, B8, B9
} facelet_t;
#define FACELET_COUNT 54


//prunetable_helper.h
#if defined(_WIN32)
#include <io.h>
#include "direct.h"
#define R_OK 4 /* Test for read permission. */
#define W_OK 2 /* Test for write permission. */
#define X_OK 1 /* Test for execute permission. */
#define F_OK 0 /* Test for existence. */
#define access _access
#else
#include <unistd.h>
#endif

//search.c
#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))


//search.h
typedef struct {
    int ax[31];             // The axis of the move
    int po[31];             // The power of the move
    int flip[31];           // phase1 coordinates
    int twist[31];
    int slice[31];
    int parity[31];         // phase2 coordinates
    int URFtoDLF[31];
    int FRtoBR[31];
    int URtoUL[31];
    int UBtoDF[31];
    int URtoDF[31];
    int minDistPhase1[31];  // IDA* distance do goal estimations
    int minDistPhase2[31];
} search_t;


class threadSafeKociemba {
private:
    //coordcube.h
    void initPruning(const char* cache_dir);
    // Set pruning value in table. Two values are stored in one char.
    void setPruning(signed char* table, int index, signed char value);
    // Extract pruning value
    signed char getPruning(signed char* table, int index);
    coordcube_t* get_coordcube(cubiecube_t* cubiecube);
    void move(coordcube_t* coordcube, int m, const char* cache_dir);


    //coordcube.c
    short twistMove[N_TWIST][N_MOVE];
    short flipMove[N_FLIP][N_MOVE];
    short parityMove[2][18] = {
        { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },
        { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 }
    };
    short FRtoBR_Move[N_FRtoBR][N_MOVE];
    short URFtoDLF_Move[N_URFtoDLF][N_MOVE] = { {0} };
    short URtoDF_Move[N_URtoDF][N_MOVE] = { {0} };
    short URtoUL_Move[N_URtoUL][N_MOVE] = { {0} };
    short UBtoDF_Move[N_UBtoDF][N_MOVE] = { {0} };
    short MergeURtoULandUBtoDF[336][336] = { {0} };
    signed char Slice_URFtoDLF_Parity_Prun[N_SLICE2 * N_URFtoDLF * N_PARITY / 2] = { 0 };
    signed char Slice_URtoDF_Parity_Prun[N_SLICE2 * N_URtoDF * N_PARITY / 2] = { 0 };
    signed char Slice_Twist_Prun[N_SLICE1 * N_TWIST / 2 + 1] = { 0 };
    signed char Slice_Flip_Prun[N_SLICE1 * N_FLIP / 2] = { 0 };
    int PRUNING_INITED = 0;


    //cubiecube.h
    // this CubieCube array represents the 6 basic cube moves
    cubiecube_t* get_moveCube(void);
    cubiecube_t* get_cubiecube(void);
    // n choose k
    int Cnk(int n, int k);
    void rotateLeft_corner(corner_t* arr, int l, int r);
    void rotateRight_corner(corner_t* arr, int l, int r);
    void rotateLeft_edge(edge_t* arr, int l, int r);
    void rotateRight_edge(edge_t* arr, int l, int r);
    struct facecube* toFaceCube(cubiecube_t* cubiecube);
    void cornerMultiply(cubiecube_t* cubiecube, cubiecube_t* b);
    void edgeMultiply(cubiecube_t* cubiecube, cubiecube_t* b);
    void multiply(cubiecube_t* cubiecube, cubiecube_t* b);
    void invCubieCube(cubiecube_t* cubiecube, cubiecube_t* c);
    short getTwist(cubiecube_t* cubiecube);
    void setTwist(cubiecube_t* cubiecube, short twist);
    short getFlip(cubiecube_t* cubiecube);
    void setFlip(cubiecube_t* cubiecube, short flip);
    short cornerParity(cubiecube_t* cubiecube);
    short edgeParity(cubiecube_t* cubiecube);
    short getFRtoBR(cubiecube_t* cubiecube);
    void setFRtoBR(cubiecube_t* cubiecube, short idx);
    short getURFtoDLF(cubiecube_t* cubiecube);
    void setURFtoDLF(cubiecube_t* cubiecube, short idx);
    int getURtoDF(cubiecube_t* cubiecube);
    void setURtoDF(cubiecube_t* cubiecube, int idx);
    short getURtoUL(cubiecube_t* cubiecube);
    void setURtoUL(cubiecube_t* cubiecube, short idx);
    short getUBtoDF(cubiecube_t* cubiecube);
    void setUBtoDF(cubiecube_t* cubiecube, short idx);
    int getURFtoDLB(cubiecube_t* cubiecube);
    void setURFtoDLB(cubiecube_t* cubiecube, int idx);
    int getURtoBR(cubiecube_t* cubiecube);
    void setURtoBR(cubiecube_t* cubiecube, int idx);
    int verify(cubiecube_t* cubiecube);
    int getURtoDF_standalone(short idx1, short idx2);

    //facecube.c
    facelet_t cornerFacelet[8][3] = { { U9, R1, F3 }, { U7, F1, L3 }, { U1, L1, B3 }, { U3, B1, R3 },
            { D3, F9, R7 }, { D1, L9, F7 }, { D7, B9, L7 }, { D9, R9, B7 } };
    facelet_t edgeFacelet[12][2] = { { U6, R2 }, { U8, F2 }, { U4, L2 }, { U2, B2 }, { D6, R8 }, { D2, F8 },
                { D4, L8 }, { D8, B8 }, { F6, R4 }, { F4, L6 }, { B6, L4 }, { B4, R6 } };
    color_t cornerColor[8][3] = { { U, R, F }, { U, F, L }, { U, L, B }, { U, B, R }, { D, F, R }, { D, L, F },
                { D, B, L }, { D, R, B } };
    color_t edgeColor[12][2] = { { U, R }, { U, F }, { U, L }, { U, B }, { D, R }, { D, F }, { D, L }, { D, B },
                { F, R }, { F, L }, { B, L }, { B, R } };


    //facecube.h
    facecube_t* get_facecube(void);
    facecube_t* get_facecube_fromstring(char* cubeString);
    void to_String(facecube_t* facecube, char* res);
    struct cubiecube* toCubieCube(facecube_t* facecube);


    //prunetable_helpers.h
    char* join_path(const char* dir, const char* filename);
    int make_dir(const char* cache_dir);
    int check_cached_table(const char* name, void* ptr, int len, const char* cache_dir);
    void dump_to_file(void* ptr, int len, const char* name, const char* cache_dir);
    void read_from_file(void* ptr, int len, const char* name);


    //search.h
    search_t* get_search(void);
    // generate the solution string from the array data including a separator between phase1 and phase2 moves
    char* solutionToString(search_t* search, int length, int depthPhase1);
    /**
     * Computes the solver string for a given cube.
     *
     * @param facelets
     *          is the cube definition string, see {@link Facelet} for the format.
     *
     * @param maxDepth
     *          defines the maximal allowed maneuver length. For random cubes, a maxDepth of 21 usually will return a
     *          solution in less than 0.5 seconds. With a maxDepth of 20 it takes a few seconds on average to find a
     *          solution, but it may take much longer for specific cubes.
     *
     *@param timeOut
     *          defines the maximum computing time of the method in seconds. If it does not return with a solution, it returns with
     *          an error code.
     *
     * @param useSeparator
     *          determines if a " . " separates the phase1 and phase2 parts of the solver string like in F' R B R L2 F .
     *          U2 U D for example.<br>
     * @return The solution string or an error code:<br>
     *         Error 1: There is not exactly one facelet of each colour<br>
     *         Error 2: Not all 12 edges exist exactly once<br>
     *         Error 3: Flip error: One edge has to be flipped<br>
     *         Error 4: Not all corners exist exactly once<br>
     *         Error 5: Twist error: One corner has to be twisted<br>
     *         Error 6: Parity error: Two corners or two edges have to be exchanged<br>
     *         Error 7: No solution exists for the given maxDepth<br>
     *         Error 8: Timeout, no solution within given time
     */
    char* solution(char* facelets, int maxDepth, long timeOut, int useSeparator, const char* cache_dir);
    // Apply phase2 of algorithm and return the combined phase1 and phase2 depth. In phase2, only the moves
    // U,D,R2,F2,L2 and B2 are allowed.
    int totalDepth(search_t* search, int depthPhase1, int maxDepth);
    // Add a pattern to the state of a cube, so that the solution for new_facelets
    // applied to facelets will result into the given pattern
    void patternize(char* facelets, char* pattern, char* patternized);


public:
    char* cube_solve(char*, char*);
};



#endif
