#include "threadSafeKociemba.h"


//coordcube.c
void threadSafeKociemba::move(coordcube_t* coordcube, int m, const char* cache_dir)
{
    if (PRUNING_INITED == 0) {
        initPruning(cache_dir);
    }
    coordcube->twist = twistMove[coordcube->twist][m];
    coordcube->flip = flipMove[coordcube->flip][m];
    coordcube->parity = parityMove[coordcube->parity][m];
    coordcube->FRtoBR = FRtoBR_Move[coordcube->FRtoBR][m];
    coordcube->URFtoDLF = URFtoDLF_Move[coordcube->URFtoDLF][m];
    coordcube->URtoUL = URtoUL_Move[coordcube->URtoUL][m];
    coordcube->UBtoDF = UBtoDF_Move[coordcube->UBtoDF][m];
    if (coordcube->URtoUL < 336 && coordcube->UBtoDF < 336)// updated only if UR,UF,UL,UB,DR,DF
        // are not in UD-slice
        coordcube->URtoDF = MergeURtoULandUBtoDF[coordcube->URtoUL][coordcube->UBtoDF];
}

coordcube_t* threadSafeKociemba::get_coordcube(cubiecube_t* cubiecube)
{
    coordcube_t* result = (coordcube_t*)calloc(1, sizeof(coordcube_t));

    result->twist = getTwist(cubiecube);
    result->flip = getFlip(cubiecube);
    result->parity = cornerParity(cubiecube);
    result->FRtoBR = getFRtoBR(cubiecube);
    result->URFtoDLF = getURFtoDLF(cubiecube);
    result->URtoUL = getURtoUL(cubiecube);
    result->UBtoDF = getUBtoDF(cubiecube);
    result->URtoDF = getURtoDF(cubiecube);// only needed in phase2

    return result;
}

void threadSafeKociemba::initPruning(const char* cache_dir)
{
    cubiecube_t* a;
    cubiecube_t* moveCube = get_moveCube();

    if (check_cached_table("twistMove", (void*)twistMove, sizeof(twistMove), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_TWIST; i++) {
            setTwist(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    cornerMultiply(a, &moveCube[j]);
                    twistMove[i][3 * j + k] = getTwist(a);
                }
                cornerMultiply(a, &moveCube[j]);// 4. faceturn restores
            }
        }
        free(a);
        dump_to_file((void*)twistMove, sizeof(twistMove), "twistMove", cache_dir);
    }

    if (check_cached_table("flipMove", (void*)flipMove, sizeof(flipMove), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_FLIP; i++) {
            setFlip(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    edgeMultiply(a, &moveCube[j]);
                    flipMove[i][3 * j + k] = getFlip(a);
                }
                edgeMultiply(a, &moveCube[j]);
            }
        }
        free(a);
        dump_to_file((void*)flipMove, sizeof(flipMove), "flipMove", cache_dir);
    }

    if (check_cached_table("FRtoBR_Move", (void*)FRtoBR_Move, sizeof(FRtoBR_Move), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_FRtoBR; i++) {
            setFRtoBR(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    edgeMultiply(a, &moveCube[j]);
                    FRtoBR_Move[i][3 * j + k] = getFRtoBR(a);
                }
                edgeMultiply(a, &moveCube[j]);
            }
        }
        free(a);
        dump_to_file((void*)FRtoBR_Move, sizeof(FRtoBR_Move), "FRtoBR_Move", cache_dir);
    }

    if (check_cached_table("URFtoDLF_Move", (void*)URFtoDLF_Move, sizeof(URFtoDLF_Move), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_URFtoDLF; i++) {
            setURFtoDLF(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    cornerMultiply(a, &moveCube[j]);
                    URFtoDLF_Move[i][3 * j + k] = getURFtoDLF(a);
                }
                cornerMultiply(a, &moveCube[j]);
            }
        }
        free(a);
        dump_to_file((void*)URFtoDLF_Move, sizeof(URFtoDLF_Move), "URFtoDLF_Move", cache_dir);
    }

    if (check_cached_table("URtoDF_Move", (void*)URtoDF_Move, sizeof(URtoDF_Move), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_URtoDF; i++) {
            setURtoDF(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    edgeMultiply(a, &moveCube[j]);
                    URtoDF_Move[i][3 * j + k] = (short)getURtoDF(a);
                    // Table values are only valid for phase 2 moves!
                    // For phase 1 moves, casting to short is not possible.
                }
                edgeMultiply(a, &moveCube[j]);
            }
        }
        free(a);
        dump_to_file((void*)URtoDF_Move, sizeof(URtoDF_Move), "URtoDF_Move", cache_dir);
    }

    if (check_cached_table("URtoUL_Move", (void*)URtoUL_Move, sizeof(URtoUL_Move), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_URtoUL; i++) {
            setURtoUL(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    edgeMultiply(a, &moveCube[j]);
                    URtoUL_Move[i][3 * j + k] = getURtoUL(a);
                }
                edgeMultiply(a, &moveCube[j]);
            }
        }
        free(a);
        dump_to_file((void*)URtoUL_Move, sizeof(URtoUL_Move), "URtoUL_Move", cache_dir);
    }

    if (check_cached_table("UBtoDF_Move", (void*)UBtoDF_Move, sizeof(UBtoDF_Move), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_UBtoDF; i++) {
            setUBtoDF(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    edgeMultiply(a, &moveCube[j]);
                    UBtoDF_Move[i][3 * j + k] = getUBtoDF(a);
                }
                edgeMultiply(a, &moveCube[j]);
            }
        }
        free(a);
        dump_to_file((void*)UBtoDF_Move, sizeof(UBtoDF_Move), "UBtoDF_Move", cache_dir);
    }

    if (check_cached_table("MergeURtoULandUBtoDF", (void*)MergeURtoULandUBtoDF, sizeof(MergeURtoULandUBtoDF), cache_dir) != 0) {
        // for i, j <336 the six edges UR,UF,UL,UB,DR,DF are not in the
        // UD-slice and the index is <20160
        short uRtoUL, uBtoDF;
        for (uRtoUL = 0; uRtoUL < 336; uRtoUL++) {
            for (uBtoDF = 0; uBtoDF < 336; uBtoDF++) {
                MergeURtoULandUBtoDF[uRtoUL][uBtoDF] = (short)getURtoDF_standalone(uRtoUL, uBtoDF);
            }
        }
        dump_to_file((void*)MergeURtoULandUBtoDF, sizeof(MergeURtoULandUBtoDF), "MergeURtoULandUBtoDF", cache_dir);
    }

    if (check_cached_table("Slice_URFtoDLF_Parity_Prun", (void*)Slice_URFtoDLF_Parity_Prun, sizeof(Slice_URFtoDLF_Parity_Prun), cache_dir) != 0) {
        int depth = 0, done = 1;
        int i, j;
        for (i = 0; i < N_SLICE2 * N_URFtoDLF * N_PARITY / 2; i++)
            Slice_URFtoDLF_Parity_Prun[i] = -1;
        setPruning(Slice_URFtoDLF_Parity_Prun, 0, 0);
        printf("1\n");
        while (done != N_SLICE2 * N_URFtoDLF * N_PARITY) {
            // printf("%d %d %d\n", done, N_SLICE2 * N_URFtoDLF * N_PARITY, depth);
            for (i = 0; i < N_SLICE2 * N_URFtoDLF * N_PARITY; i++) {
                int parity = i % 2;
                int URFtoDLF = (i / 2) / N_SLICE2;
                int slice = (i / 2) % N_SLICE2;
                if (getPruning(Slice_URFtoDLF_Parity_Prun, i) == depth) {
                    for (j = 0; j < 18; j++) {
                        int newSlice;
                        int newURFtoDLF;
                        int newParity;
                        switch (j) {
                        case 3:
                        case 5:
                        case 6:
                        case 8:
                        case 12:
                        case 14:
                        case 15:
                        case 17:
                            continue;
                        default:
                            newSlice = FRtoBR_Move[slice][j];
                            newURFtoDLF = URFtoDLF_Move[URFtoDLF][j];
                            newParity = parityMove[parity][j];
                            if (getPruning(Slice_URFtoDLF_Parity_Prun, (N_SLICE2 * newURFtoDLF + newSlice) * 2 + newParity) == 0x0f) {
                                setPruning(Slice_URFtoDLF_Parity_Prun, (N_SLICE2 * newURFtoDLF + newSlice) * 2 + newParity,
                                    (signed char)(depth + 1));
                                done++;
                            }
                        }
                    }
                }
            }
            depth++;
        }
        printf("2\n");
        dump_to_file((void*)Slice_URFtoDLF_Parity_Prun, sizeof(Slice_URFtoDLF_Parity_Prun), "Slice_URFtoDLF_Parity_Prun", cache_dir);
        printf("3\n");
    }

    if (check_cached_table("Slice_URtoDF_Parity_Prun", (void*)Slice_URtoDF_Parity_Prun, sizeof(Slice_URtoDF_Parity_Prun), cache_dir) != 0) {
        int depth = 0, done = 1;
        int i, j;
        for (i = 0; i < N_SLICE2 * N_URtoDF * N_PARITY / 2; i++)
            Slice_URtoDF_Parity_Prun[i] = -1;
        setPruning(Slice_URtoDF_Parity_Prun, 0, 0);
        while (done != N_SLICE2 * N_URtoDF * N_PARITY) {
            for (i = 0; i < N_SLICE2 * N_URtoDF * N_PARITY; i++) {
                int parity = i % 2;
                int URtoDF = (i / 2) / N_SLICE2;
                int slice = (i / 2) % N_SLICE2;
                if (getPruning(Slice_URtoDF_Parity_Prun, i) == depth) {
                    for (j = 0; j < 18; j++) {
                        int newSlice;
                        int newURtoDF;
                        int newParity;
                        switch (j) {
                        case 3:
                        case 5:
                        case 6:
                        case 8:
                        case 12:
                        case 14:
                        case 15:
                        case 17:
                            continue;
                        default:
                            newSlice = FRtoBR_Move[slice][j];
                            newURtoDF = URtoDF_Move[URtoDF][j];
                            newParity = parityMove[parity][j];
                            if (getPruning(Slice_URtoDF_Parity_Prun, (N_SLICE2 * newURtoDF + newSlice) * 2 + newParity) == 0x0f) {
                                setPruning(Slice_URtoDF_Parity_Prun, (N_SLICE2 * newURtoDF + newSlice) * 2 + newParity,
                                    (signed char)(depth + 1));
                                done++;
                            }
                        }
                    }
                }
            }
            depth++;
        }
        dump_to_file((void*)Slice_URtoDF_Parity_Prun, sizeof(Slice_URtoDF_Parity_Prun), "Slice_URtoDF_Parity_Prun", cache_dir);
    }

    if (check_cached_table("Slice_Twist_Prun", (void*)Slice_Twist_Prun, sizeof(Slice_Twist_Prun), cache_dir) != 0) {
        int depth = 0, done = 1;
        int i, j;
        for (i = 0; i < N_SLICE1 * N_TWIST / 2 + 1; i++)
            Slice_Twist_Prun[i] = -1;
        setPruning(Slice_Twist_Prun, 0, 0);
        while (done != N_SLICE1 * N_TWIST) {
            for (i = 0; i < N_SLICE1 * N_TWIST; i++) {
                int twist = i / N_SLICE1, slice = i % N_SLICE1;
                if (getPruning(Slice_Twist_Prun, i) == depth) {
                    for (j = 0; j < 18; j++) {
                        int newSlice = FRtoBR_Move[slice * 24][j] / 24;
                        int newTwist = twistMove[twist][j];
                        if (getPruning(Slice_Twist_Prun, N_SLICE1 * newTwist + newSlice) == 0x0f) {
                            setPruning(Slice_Twist_Prun, N_SLICE1 * newTwist + newSlice, (signed char)(depth + 1));
                            done++;
                        }
                    }
                }
            }
            depth++;
        }
        dump_to_file((void*)Slice_Twist_Prun, sizeof(Slice_Twist_Prun), "Slice_Twist_Prun", cache_dir);
    }

    if (check_cached_table("Slice_Flip_Prun", (void*)Slice_Flip_Prun, sizeof(Slice_Flip_Prun), cache_dir) != 0) {
        int depth = 0, done = 1;
        int i, j;
        for (i = 0; i < N_SLICE1 * N_FLIP / 2; i++)
            Slice_Flip_Prun[i] = -1;
        setPruning(Slice_Flip_Prun, 0, 0);
        while (done != N_SLICE1 * N_FLIP) {
            for (i = 0; i < N_SLICE1 * N_FLIP; i++) {
                int flip = i / N_SLICE1, slice = i % N_SLICE1;
                if (getPruning(Slice_Flip_Prun, i) == depth) {
                    for (j = 0; j < 18; j++) {
                        int newSlice = FRtoBR_Move[slice * 24][j] / 24;
                        int newFlip = flipMove[flip][j];
                        if (getPruning(Slice_Flip_Prun, N_SLICE1 * newFlip + newSlice) == 0x0f) {
                            setPruning(Slice_Flip_Prun, N_SLICE1 * newFlip + newSlice, (signed char)(depth + 1));
                            done++;
                        }
                    }
                }
            }
            depth++;
        }
        dump_to_file((void*)Slice_Flip_Prun, sizeof(Slice_Flip_Prun), "Slice_Flip_Prun", cache_dir);
    }

    PRUNING_INITED = 1;
}

void threadSafeKociemba::setPruning(signed char* table, int index, signed char value) {
    if ((index & 1) == 0)
        table[index / 2] &= 0xf0 | value;
    else
        table[index / 2] &= 0x0f | (value << 4);
}

// Extract pruning value
signed char threadSafeKociemba::getPruning(signed char* table, int index) {
    signed char res;

    if ((index & 1) == 0)
        res = (table[index / 2] & 0x0f);
    else
        res = ((table[index / 2] >> 4) & 0x0f);

    return res;
}




//cubiecube.c
cubiecube_t* threadSafeKociemba::get_moveCube()
{
    static cubiecube_t moveCube[6];
    static int moveCube_initialized = 0;
    static const corner_t     cpU[8] = { UBR, URF, UFL, ULB, DFR, DLF, DBL, DRB };
    static const signed char  coU[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    static const edge_t       epU[12] = { UB, UR, UF, UL, DR, DF, DL, DB, FR, FL, BL, BR };
    static const signed char  eoU[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    static const corner_t     cpR[8] = { DFR, UFL, ULB, URF, DRB, DLF, DBL, UBR };
    static const signed char  coR[8] = { 2, 0, 0, 1, 1, 0, 0, 2 };
    static const edge_t       epR[12] = { FR, UF, UL, UB, BR, DF, DL, DB, DR, FL, BL, UR };
    static const signed char  eoR[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    static const corner_t     cpF[8] = { UFL, DLF, ULB, UBR, URF, DFR, DBL, DRB };
    static const signed char  coF[8] = { 1, 2, 0, 0, 2, 1, 0, 0 };
    static const edge_t       epF[12] = { UR, FL, UL, UB, DR, FR, DL, DB, UF, DF, BL, BR };
    static const signed char  eoF[12] = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 };
    static const corner_t     cpD[8] = { URF, UFL, ULB, UBR, DLF, DBL, DRB, DFR };
    static const signed char  coD[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    static const edge_t       epD[12] = { UR, UF, UL, UB, DF, DL, DB, DR, FR, FL, BL, BR };
    static const signed char  eoD[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    static const corner_t     cpL[8] = { URF, ULB, DBL, UBR, DFR, UFL, DLF, DRB };
    static const signed char  coL[8] = { 0, 1, 2, 0, 0, 2, 1, 0 };
    static const edge_t       epL[12] = { UR, UF, BL, UB, DR, DF, FL, DB, FR, UL, DL, BR };
    static const signed char  eoL[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    static const corner_t     cpB[8] = { URF, UFL, UBR, DRB, DFR, DLF, ULB, DBL };
    static const signed char  coB[8] = { 0, 0, 1, 2, 0, 0, 2, 1 };
    static const edge_t       epB[12] = { UR, UF, UL, BR, DR, DF, DL, BL, FR, FL, UB, DB };
    static const signed char  eoB[12] = { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 };

    if (!moveCube_initialized) {
        memcpy(moveCube[0].cp, cpU, sizeof(cpU));
        memcpy(moveCube[0].co, coU, sizeof(coU));
        memcpy(moveCube[0].ep, epU, sizeof(epU));
        memcpy(moveCube[0].eo, eoU, sizeof(eoU));
        memcpy(moveCube[1].cp, cpR, sizeof(cpR));
        memcpy(moveCube[1].co, coR, sizeof(coR));
        memcpy(moveCube[1].ep, epR, sizeof(epR));
        memcpy(moveCube[1].eo, eoR, sizeof(eoR));
        memcpy(moveCube[2].cp, cpF, sizeof(cpF));
        memcpy(moveCube[2].co, coF, sizeof(coF));
        memcpy(moveCube[2].ep, epF, sizeof(epF));
        memcpy(moveCube[2].eo, eoF, sizeof(eoF));
        memcpy(moveCube[3].cp, cpD, sizeof(cpD));
        memcpy(moveCube[3].co, coD, sizeof(coD));
        memcpy(moveCube[3].ep, epD, sizeof(epD));
        memcpy(moveCube[3].eo, eoD, sizeof(eoD));
        memcpy(moveCube[4].cp, cpL, sizeof(cpL));
        memcpy(moveCube[4].co, coL, sizeof(coL));
        memcpy(moveCube[4].ep, epL, sizeof(epL));
        memcpy(moveCube[4].eo, eoL, sizeof(eoL));
        memcpy(moveCube[5].cp, cpB, sizeof(cpB));
        memcpy(moveCube[5].co, coB, sizeof(coB));
        memcpy(moveCube[5].ep, epB, sizeof(epB));
        memcpy(moveCube[5].eo, eoB, sizeof(eoB));
    }

    return moveCube;
}

cubiecube_t* threadSafeKociemba::get_cubiecube()
{
    cubiecube_t* result = (cubiecube_t*)calloc(1, sizeof(cubiecube_t));

    static const corner_t   cp[8] = { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };
    static const signed char       co[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    static const edge_t     ep[12] = { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };
    static const signed char       eo[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    memcpy(result->cp, cp, sizeof(cp));
    memcpy(result->co, co, sizeof(co));
    memcpy(result->ep, ep, sizeof(ep));
    memcpy(result->eo, eo, sizeof(eo));

    return result;
}

int threadSafeKociemba::Cnk(int n, int k) {
    int i, j, s;
    if (n < k)
        return 0;
    if (k > n / 2)
        k = n - k;
    for (s = 1, i = n, j = 1; i != n - k; i--, j++) {
        s *= i;
        s /= j;
    }
    return s;
}

void threadSafeKociemba::rotateLeft_corner(corner_t* arr, int l, int r)
// Left rotation of all array elements between l and r
{
    int i;
    corner_t temp = arr[l];
    for (i = l; i < r; i++)
        arr[i] = arr[i + 1];
    arr[r] = temp;
}

void threadSafeKociemba::rotateRight_corner(corner_t* arr, int l, int r)
// Right rotation of all array elements between l and r
{
    int i;
    corner_t temp = arr[r];
    for (i = r; i > l; i--)
        arr[i] = arr[i - 1];
    arr[l] = temp;
}


void threadSafeKociemba::rotateLeft_edge(edge_t* arr, int l, int r)
// Left rotation of all array elements between l and r
{
    int i;
    edge_t temp = arr[l];
    for (i = l; i < r; i++)
        arr[i] = arr[i + 1];
    arr[r] = temp;
}

void threadSafeKociemba::rotateRight_edge(edge_t* arr, int l, int r)
// Right rotation of all array elements between l and r
{
    int i;
    edge_t temp = arr[r];
    for (i = r; i > l; i--)
        arr[i] = arr[i - 1];
    arr[l] = temp;
}

facecube_t* threadSafeKociemba::toFaceCube(cubiecube_t* cubiecube)
{
    int i, j, n;
    signed char ori;
    facecube_t* fcRet = get_facecube();
    for (i = 0; i < CORNER_COUNT; i++) {
        j = cubiecube->cp[i];// cornercubie with index j is at
        // cornerposition with index i
        ori = cubiecube->co[i];// Orientation of this cubie
        for (n = 0; n < 3; n++)
            fcRet->f[cornerFacelet[i][(n + ori) % 3]] = cornerColor[j][n];
    }
    for (i = 0; i < EDGE_COUNT; i++)
    {
        j = cubiecube->ep[i];// edgecubie with index j is at edgeposition
        // with index i
        ori = cubiecube->eo[i];// Orientation of this cubie
        for (n = 0; n < 2; n++)
            fcRet->f[edgeFacelet[i][(n + ori) % 2]] = edgeColor[j][n];
    }
    return fcRet;
}

void threadSafeKociemba::cornerMultiply(cubiecube_t* cubiecube, cubiecube_t* b)
{
    int corn;
    signed char oriA, oriB, ori;
    /*corner_t cPerm[8] = {0};*/    //struct初始化错误
    corner_t cPerm[8] = {};
    memset(cPerm, 0, sizeof(cPerm));

    signed char cOri[8] = { 0 };
    for (corn = 0; corn < CORNER_COUNT; corn++) {
        cPerm[corn] = cubiecube->cp[b->cp[corn]];

        oriA = cubiecube->co[b->cp[corn]];
        oriB = b->co[corn];
        ori = 0;

        if (oriA < 3 && oriB < 3) // if both cubes are regular cubes...
        {
            ori = oriA + oriB; // just do an addition modulo 3 here
            if (ori >= 3)
                ori -= 3; // the composition is a regular cube

            // +++++++++++++++++++++not used in this implementation +++++++++++++++++++++++++++++++++++
        }
        else if (oriA < 3 && oriB >= 3) // if cube b is in a mirrored
     // state...
        {
            ori = oriA + oriB;
            if (ori >= 6)
                ori -= 3; // the composition is a mirrored cube
        }
        else if (oriA >= 3 && oriB < 3) // if cube a is an a mirrored
     // state...
        {
            ori = oriA - oriB;
            if (ori < 3)
                ori += 3; // the composition is a mirrored cube
        }
        else if (oriA >= 3 && oriB >= 3) // if both cubes are in mirrored
     // states...
        {
            ori = oriA - oriB;
            if (ori < 0)
                ori += 3; // the composition is a regular cube
            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        }
        cOri[corn] = ori;
    }
    for (corn = 0; corn < CORNER_COUNT; corn++) {
        cubiecube->cp[corn] = cPerm[corn];
        cubiecube->co[corn] = cOri[corn];
    }
}

void threadSafeKociemba::edgeMultiply(cubiecube_t* cubiecube, cubiecube_t* b)
{
    int edge;
    /*edge_t ePerm[12] = {0};*/     //struct初始化错误
    edge_t ePerm[12];
    memset(ePerm, 0, sizeof(ePerm));
    signed char eOri[12] = { 0 };

    for (edge = 0; edge < EDGE_COUNT; edge++) {
        ePerm[edge] = cubiecube->ep[b->ep[edge]];
        eOri[edge] = (b->eo[edge] + cubiecube->eo[b->ep[edge]]) % 2;
    }
    for (edge = 0; edge < EDGE_COUNT; edge++) {
        cubiecube->ep[edge] = ePerm[edge];
        cubiecube->eo[edge] = eOri[edge];
    }
}

void threadSafeKociemba::multiply(cubiecube_t* cubiecube, cubiecube_t* b)
{
    cornerMultiply(cubiecube, b);
    edgeMultiply(cubiecube, b);
}

void threadSafeKociemba::invCubieCube(cubiecube_t* cubiecube, cubiecube_t* c)
{
    int edge, corn;
    for (edge = 0; edge < EDGE_COUNT; edge++)
        c->ep[cubiecube->ep[edge]] = edge_t(edge);      //类型转换错误，源代码为去掉强制转换的形式
    for (edge = 0; edge < EDGE_COUNT; edge++)
        c->eo[edge] = cubiecube->eo[c->ep[edge]];
    for (corn = 0; corn < CORNER_COUNT; corn++)
        c->cp[cubiecube->cp[corn]] = corner_t(corn);    //类型转换错误，源代码为去掉强制转换的形式
    for (corn = 0; corn < CORNER_COUNT; corn++) {
        signed char ori = cubiecube->co[c->cp[corn]];
        if (ori >= 3)// Just for completeness. We do not invert mirrored
            // cubes in the program.
            c->co[corn] = ori;
        else {// the standard case
            c->co[corn] = -ori;
            if (c->co[corn] < 0)
                c->co[corn] += 3;
        }
    }
}

short threadSafeKociemba::getTwist(cubiecube_t* cubiecube)
{
    short ret = 0;
    int i;
    for (i = URF; i < DRB; i++)
        ret = (short)(3 * ret + cubiecube->co[i]);
    return ret;
}

void threadSafeKociemba::setTwist(cubiecube_t* cubiecube, short twist)
{
    int twistParity = 0;
    int i;
    for (i = DRB - 1; i >= URF; i--) {
        twistParity += cubiecube->co[i] = (signed char)(twist % 3);
        twist /= 3;
    }
    cubiecube->co[DRB] = (signed char)((3 - twistParity % 3) % 3);
}

short threadSafeKociemba::getFlip(cubiecube_t* cubiecube)
{
    int i;
    short ret = 0;
    for (i = UR; i < BR; i++)
        ret = (short)(2 * ret + cubiecube->eo[i]);
    return ret;
}

void threadSafeKociemba::setFlip(cubiecube_t* cubiecube, short flip)
{
    int i;
    int flipParity = 0;
    for (i = BR - 1; i >= UR; i--) {
        flipParity += cubiecube->eo[i] = (signed char)(flip % 2);
        flip /= 2;
    }
    cubiecube->eo[BR] = (signed char)((2 - flipParity % 2) % 2);
}

short threadSafeKociemba::cornerParity(cubiecube_t* cubiecube)
{
    int i, j;
    int s = 0;
    for (i = DRB; i >= URF + 1; i--)
        for (j = i - 1; j >= URF; j--)
            if (cubiecube->cp[j] > cubiecube->cp[i])
                s++;
    return (short)(s % 2);
}

short threadSafeKociemba::edgeParity(cubiecube_t* cubiecube)
{
    int i, j;
    int s = 0;
    for (i = BR; i >= UR + 1; i--)
        for (j = i - 1; j >= UR; j--)
            if (cubiecube->ep[j] > cubiecube->ep[i])
                s++;
    return (short)(s % 2);
}

short threadSafeKociemba::getFRtoBR(cubiecube_t* cubiecube)
{
    int a = 0, x = 0, j;
    int b = 0;
    /*edge_t edge4[4] = {0};*/      //初始化结构体错误
    edge_t edge4[4];
    memset(edge4, 0, sizeof(edge4));
    // compute the index a < (12 choose 4) and the permutation array perm.
    for (j = BR; j >= UR; j--)
        if (FR <= cubiecube->ep[j] && cubiecube->ep[j] <= BR) {
            a += Cnk(11 - j, x + 1);
            edge4[3 - x++] = cubiecube->ep[j];
        }

    for (j = 3; j > 0; j--)// compute the index b < 4! for the
    // permutation in perm
    {
        int k = 0;
        while (edge4[j] != j + 8) {
            rotateLeft_edge(edge4, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return (short)(24 * a + b);
}
void threadSafeKociemba::setFRtoBR(cubiecube_t* cubiecube, short idx)
{
    int x, j, k, e;
    edge_t sliceEdge[4] = { FR, FL, BL, BR };
    edge_t otherEdge[8] = { UR, UF, UL, UB, DR, DF, DL, DB };
    int b = idx % 24; // Permutation
    int a = idx / 24; // Combination
    for (e = 0; e < EDGE_COUNT; e++)
        cubiecube->ep[e] = DB;// Use UR to invalidate all edges

    for (j = 1; j < 4; j++)// generate permutation from index b
    {
        k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            rotateRight_edge(sliceEdge, 0, j);
    }

    x = 3;// generate combination and set slice edges
    for (j = UR; j <= BR; j++)
        if (a - Cnk(11 - j, x + 1) >= 0) {
            cubiecube->ep[j] = sliceEdge[3 - x];
            a -= Cnk(11 - j, x-- + 1);
        }
    x = 0; // set the remaining edges UR..DB
    for (j = UR; j <= BR; j++)
        if (cubiecube->ep[j] == DB)
            cubiecube->ep[j] = otherEdge[x++];
}

short threadSafeKociemba::getURFtoDLF(cubiecube_t* cubiecube)
{
    int a = 0, x = 0, j, b = 0;
    /*corner_t corner6[6] = {0};*/  //初始化结构体错误
    corner_t corner6[6];
    memset(corner6, 0, sizeof(corner6));
    // compute the index a < (8 choose 6) and the corner permutation.
    for (j = URF; j <= DRB; j++)
        if (cubiecube->cp[j] <= DLF) {
            a += Cnk(j, x + 1);
            corner6[x++] = cubiecube->cp[j];
        }

    for (j = 5; j > 0; j--)// compute the index b < 6! for the
    // permutation in corner6
    {
        int k = 0;
        while (corner6[j] != j) {
            rotateLeft_corner(corner6, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return (short)(720 * a + b);
}

void threadSafeKociemba::setURFtoDLF(cubiecube_t* cubiecube, short idx)
{
    int x;
    corner_t corner6[6] = { URF, UFL, ULB, UBR, DFR, DLF };
    corner_t otherCorner[2] = { DBL, DRB };
    int b = idx % 720; // Permutation
    int a = idx / 720; // Combination
    int c, j, k;
    for (c = 0; c < CORNER_COUNT; c++)
        cubiecube->cp[c] = DRB;// Use DRB to invalidate all corners

    for (j = 1; j < 6; j++)// generate permutation from index b
    {
        k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            rotateRight_corner(corner6, 0, j);
    }
    x = 5;// generate combination and set corners
    for (j = DRB; j >= 0; j--)
        if (a - Cnk(j, x + 1) >= 0) {
            cubiecube->cp[j] = corner6[x];
            a -= Cnk(j, x-- + 1);
        }
    x = 0;
    for (j = URF; j <= DRB; j++)
        if (cubiecube->cp[j] == DRB)
            cubiecube->cp[j] = otherCorner[x++];
}

int threadSafeKociemba::getURtoDF(cubiecube_t* cubiecube)
{
    int a = 0, x = 0;
    int b = 0, j;
    /*edge_t edge6[6] = {0};*/  //初始化结构体错误
    edge_t edge6[6];
    memset(edge6, 0, sizeof(edge6));
    // compute the index a < (12 choose 6) and the edge permutation.
    for (j = UR; j <= BR; j++)
        if (cubiecube->ep[j] <= DF) {
            a += Cnk(j, x + 1);
            edge6[x++] = cubiecube->ep[j];
        }

    for (j = 5; j > 0; j--)// compute the index b < 6! for the
    // permutation in edge6
    {
        int k = 0;
        while (edge6[j] != j) {
            rotateLeft_edge(edge6, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return 720 * a + b;
}

void threadSafeKociemba::setURtoDF(cubiecube_t* cubiecube, int idx)
{
    int x, e, j, k;
    edge_t edge6[6] = { UR, UF, UL, UB, DR, DF };
    edge_t otherEdge[6] = { DL, DB, FR, FL, BL, BR };
    int b = idx % 720; // Permutation
    int a = idx / 720; // Combination

    for (e = 0; e < EDGE_COUNT; e++)
        cubiecube->ep[e] = BR;// Use BR to invalidate all edges

    for (j = 1; j < 6; j++)// generate permutation from index b
    {
        k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            rotateRight_edge(edge6, 0, j);
    }
    x = 5;// generate combination and set edges
    for (j = BR; j >= 0; j--)
        if (a - Cnk(j, x + 1) >= 0) {
            cubiecube->ep[j] = edge6[x];
            a -= Cnk(j, x-- + 1);
        }
    x = 0; // set the remaining edges DL..BR
    for (j = UR; j <= BR; j++)
        if (cubiecube->ep[j] == BR)
            cubiecube->ep[j] = otherEdge[x++];
}

short threadSafeKociemba::getURtoUL(cubiecube_t* cubiecube)
{
    int a = 0, b = 0, x = 0, j;
    /*edge_t edge3[3] = {0};*/  //初始化结构体错误
    edge_t edge3[3];
    memset(edge3, 0, sizeof(edge3));
    // compute the index a < (12 choose 3) and the edge permutation.
    for (j = UR; j <= BR; j++)
        if (cubiecube->ep[j] <= UL) {
            a += Cnk(j, x + 1);
            edge3[x++] = cubiecube->ep[j];
        }

    for (j = 2; j > 0; j--)// compute the index b < 3! for the
    // permutation in edge3
    {
        int k = 0;
        while (edge3[j] != j) {
            rotateLeft_edge(edge3, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return (short)(6 * a + b);
}

void threadSafeKociemba::setURtoUL(cubiecube_t* cubiecube, short idx)
{
    int x, e, j, k;
    edge_t edge3[3] = { UR, UF, UL };
    int b = idx % 6; // Permutation
    int a = idx / 6; // Combination
    for (e = 0; e < EDGE_COUNT; e++) {
        cubiecube->ep[e] = BR;// Use BR to invalidate all edges
    }

    for (j = 1; j < 3; j++) {// generate permutation from index b
        k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            rotateRight_edge(edge3, 0, j);
    }
    x = 2;// generate combination and set edges
    for (j = BR; j >= 0; j--) {
        if (a - Cnk(j, x + 1) >= 0) {
            cubiecube->ep[j] = edge3[x];
            a -= Cnk(j, x-- + 1);
        }
    }
}

short threadSafeKociemba::getUBtoDF(cubiecube_t* cubiecube)
{
    int a = 0, x = 0, b = 0, j;
    /*edge_t edge3[3] = {0};*/  //初始化结构体错误
    edge_t edge3[3];
    memset(edge3, 0, sizeof(edge3));
    // compute the index a < (12 choose 3) and the edge permutation.
    for (j = UR; j <= BR; j++)
        if (UB <= cubiecube->ep[j] && cubiecube->ep[j] <= DF) {
            a += Cnk(j, x + 1);
            edge3[x++] = cubiecube->ep[j];
        }

    for (j = 2; j > 0; j--)// compute the index b < 3! for the
    // permutation in edge3
    {
        int k = 0;
        while (edge3[j] != UB + j) {
            rotateLeft_edge(edge3, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return (short)(6 * a + b);
}

void threadSafeKociemba::setUBtoDF(cubiecube_t* cubiecube, short idx)
{
    int x, e, j, k;
    edge_t edge3[3] = { UB, DR, DF };
    int b = idx % 6; // Permutation
    int a = idx / 6; // Combination
    for (e = 0; e < EDGE_COUNT; e++)
        cubiecube->ep[e] = BR;// Use BR to invalidate all edges

    for (j = 1; j < 3; j++)// generate permutation from index b
    {
        k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            rotateRight_edge(edge3, 0, j);
    }
    x = 2;// generate combination and set edges
    for (j = BR; j >= 0; j--)
        if (a - Cnk(j, x + 1) >= 0) {
            cubiecube->ep[j] = edge3[x];
            a -= Cnk(j, x-- + 1);
        }
}

int threadSafeKociemba::getURFtoDLB(cubiecube_t* cubiecube)
{
    /*corner_t perm[8] = {0};*/     //初始化结构体错误
    corner_t perm[8];
    memset(perm, 0, sizeof(perm));
    int b = 0, i, j;
    for (i = 0; i < 8; i++)
        perm[i] = cubiecube->cp[i];
    for (j = 7; j > 0; j--)// compute the index b < 8! for the permutation in perm
    {
        int k = 0;
        while (perm[j] != j) {
            rotateLeft_corner(perm, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return b;
}

void threadSafeKociemba::setURFtoDLB(cubiecube_t* cubiecube, int idx)
{
    corner_t perm[8] = { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };
    int k, j;
    int x = 7;// set corners
    for (j = 1; j < 8; j++) {
        k = idx % (j + 1);
        idx /= j + 1;
        while (k-- > 0)
            rotateRight_corner(perm, 0, j);
    }

    for (j = 7; j >= 0; j--)
        cubiecube->cp[j] = perm[x--];
}

int threadSafeKociemba::getURtoBR(cubiecube_t* cubiecube)
{
    /*edge_t perm[12] = {0};*/      //初始化结构体错误
    edge_t perm[12];
    memset(perm, 0, sizeof(perm));
    int b = 0, i, j;
    for (i = 0; i < 12; i++)
        perm[i] = cubiecube->ep[i];
    for (j = 11; j > 0; j--)// compute the index b < 12! for the permutation in perm
    {
        int k = 0;
        while (perm[j] != j) {
            rotateLeft_edge(perm, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return b;
}

void threadSafeKociemba::setURtoBR(cubiecube_t* cubiecube, int idx)
{
    edge_t perm[12] = { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };
    int k, j;
    int x = 11;// set edges
    for (j = 1; j < 12; j++) {
        k = idx % (j + 1);
        idx /= j + 1;
        while (k-- > 0)
            rotateRight_edge(perm, 0, j);
    }
    for (j = 11; j >= 0; j--)
        cubiecube->ep[j] = perm[x--];
}

int threadSafeKociemba::verify(cubiecube_t* cubiecube)
{
    int sum = 0, e, i, c;
    int edgeCount[12] = { 0 };
    int cornerCount[8] = { 0 };

    for (e = 0; e < EDGE_COUNT; e++)
        edgeCount[cubiecube->ep[e]]++;
    for (i = 0; i < 12; i++)
        if (edgeCount[i] != 1)
            return -2;

    for (i = 0; i < 12; i++)
        sum += cubiecube->eo[i];
    if (sum % 2 != 0)
        return -3;

    for (c = 0; c < CORNER_COUNT; c++)
        cornerCount[cubiecube->cp[c]]++;
    for (i = 0; i < 8; i++)
        if (cornerCount[i] != 1)
            return -4;// missing corners

    sum = 0;
    for (i = 0; i < 8; i++)
        sum += cubiecube->co[i];
    if (sum % 3 != 0)
        return -5;// twisted corner

    if ((edgeParity(cubiecube) ^ cornerParity(cubiecube)) != 0)
        return -6;// parity error

    return 0;// cube ok
}

int threadSafeKociemba::getURtoDF_standalone(short idx1, short idx2)
{
    int res, i;
    cubiecube_t* a = get_cubiecube();
    cubiecube_t* b = get_cubiecube();
    setURtoUL(a, idx1);
    setUBtoDF(b, idx2);
    for (i = 0; i < 8; i++) {
        if (a->ep[i] != BR) {
            if (b->ep[i] != BR) {// collision
                return -1;
            }
            else {
                b->ep[i] = a->ep[i];
            }
        }
    }
    res = getURtoDF(b);
    free(a);
    free(b);
    return res;
}





//facecube.c
facecube_t* threadSafeKociemba::get_facecube()
{
    facecube_t* res = (facecube_t*)calloc(1, sizeof(facecube_t));
    static const color_t f[54] = { U, U, U, U, U, U, U, U, U, R, R, R, R, R, R, R, R, R, F, F, F, F, F, F, F, F, F, D, D, D, D, D, D, D, D, D, L, L, L, L, L, L, L, L, L, B, B, B, B, B, B, B, B, B };
    memcpy(res->f, f, sizeof(f));
    return res;
}

facecube_t* threadSafeKociemba::get_facecube_fromstring(char* cubeString)
{
    int i;
    facecube_t* res = (facecube_t*)calloc(1, sizeof(facecube_t));
    for (i = 0; i < 54; i++) {
        switch (cubeString[i]) {
        case 'U':
            res->f[i] = U;
            break;
        case 'R':
            res->f[i] = R;
            break;
        case 'F':
            res->f[i] = F;
            break;
        case 'D':
            res->f[i] = D;
            break;
        case 'L':
            res->f[i] = L;
            break;
        case 'B':
            res->f[i] = B;
            break;
        }
    }
    return res;
}

void threadSafeKociemba::to_String(facecube_t* facecube, char* res)
{
    int i;
    for (i = 0; i < 54; i++)
        switch (facecube->f[i]) {
        case U:
            res[i] = 'U';
            break;
        case R:
            res[i] = 'R';
            break;
        case F:
            res[i] = 'F';
            break;
        case D:
            res[i] = 'D';
            break;
        case L:
            res[i] = 'L';
            break;
        case B:
            res[i] = 'B';
            break;
        }
    res[54] = 0;
}

cubiecube_t* threadSafeKociemba::toCubieCube(facecube_t* facecube)
{
    int i, j;
    signed char ori;
    color_t col1, col2;
    cubiecube_t* ccRet = (cubiecube_t*)calloc(1, sizeof(cubiecube_t));
    for (i = 0; i < 8; i++)
        ccRet->cp[i] = URF;// invalidate corners
    for (i = 0; i < 12; i++)
        ccRet->ep[i] = UR;// and edges

    for (i = 0; i < CORNER_COUNT; i++) {
        // get the colors of the cubie at corner i, starting with U/D
        for (ori = 0; ori < 3; ori++)
            if (facecube->f[cornerFacelet[i][ori]] == U || facecube->f[cornerFacelet[i][ori]] == D)
                break;
        col1 = facecube->f[cornerFacelet[i][(ori + 1) % 3]];
        col2 = facecube->f[cornerFacelet[i][(ori + 2) % 3]];

        for (j = 0; j < CORNER_COUNT; j++) {
            if (col1 == cornerColor[j][1] && col2 == cornerColor[j][2]) {
                // in cornerposition i we have cornercubie j
                ccRet->cp[i] = corner_t(j);                 //类型转换错误，源代码为去掉强制转换的形式
                ccRet->co[i] = ori % 3;
                break;
            }
        }
    }

    for (i = 0; i < EDGE_COUNT; i++) {
        for (j = 0; j < EDGE_COUNT; j++) {
            if (facecube->f[edgeFacelet[i][0]] == edgeColor[j][0]
                && facecube->f[edgeFacelet[i][1]] == edgeColor[j][1]) {
                ccRet->ep[i] = edge_t(j);                   //类型转换错误，源代码为去掉强制转换的形式
                ccRet->eo[i] = 0;
                break;
            }
            if (facecube->f[edgeFacelet[i][0]] == edgeColor[j][1]
                && facecube->f[edgeFacelet[i][1]] == edgeColor[j][0]) {
                ccRet->ep[i] = edge_t(j);                   //类型转换错误，源代码为去掉强制转换的形式
                ccRet->eo[i] = 1;
                break;
            }
        }
    }
    return ccRet;
}








//prunetable_helpers.c
char* threadSafeKociemba::join_path(const char* dir, const char* filename)
{
    size_t path_len = strnlen(dir, 500);
    char* fpath = (char *)calloc(path_len + 32, 1);
    if (path_len == 500) {
        return NULL;
    }
    strcpy(fpath, dir);
    strcat(fpath, "/");
    strncat(fpath, filename, 30);
    return fpath;
}

int threadSafeKociemba::check_cached_table(const char* name, void* ptr, int len, const char* cache_dir)
{
    int res = 0;
    char* fname = join_path(cache_dir, name);
    if (fname == NULL) {
        fprintf(stderr, "Path to cache tables is too long\n");
        return -1;
    }

    if (access(fname, F_OK | R_OK) != -1) {
        // fprintf(stderr, "Found cache for %s. Loading...", name);
        read_from_file(ptr, len, fname);
        // fprintf(stderr, "done.\n");
        res = 0;
    }
    else {
        fprintf(stderr, "Cache table %s was not found. Recalculating.\n", fname);
        res = 1;
    }
    free(fname);
    return res;
}

void threadSafeKociemba::read_from_file(void* ptr, int len, const char* name)
{
    FILE* f = fopen(name, "rb");
    if (!fread(ptr, len, 1, f))
        ((void)0); // suppress -Wunused-result warning
    fclose(f);
}

int threadSafeKociemba::make_dir(const char* cache_dir)
{
#if defined(_WIN32)
    return _mkdir(cache_dir);
#else
    return mkdir(cache_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

void threadSafeKociemba::dump_to_file(void* ptr, int len, const char* name, const char* cache_dir)
{
    int status;
    status = make_dir(cache_dir);
    if (status == 0 || errno == EEXIST) {
        char* fname = join_path(cache_dir, name);
        if (fname == NULL) {
            fprintf(stderr, "Path to cache tables is too long\n");
        }
        else {
            FILE* f = fopen(fname, "wb");
            fwrite(ptr, len, 1, f);
            free(fname);
            fclose(f);
        }
    }
    else {
        fprintf(stderr, "cannot create cache tables directory\n");
    }
}






//search.c
char* threadSafeKociemba::solutionToString(search_t* search, int length, int depthPhase1)
{
    char* s = (char*)calloc(length * 3 + 5, 1);
    int cur = 0, i;
    for (i = 0; i < length; i++) {
        switch (search->ax[i]) {
        case 0:
            s[cur++] = 'U';
            break;
        case 1:
            s[cur++] = 'R';
            break;
        case 2:
            s[cur++] = 'F';
            break;
        case 3:
            s[cur++] = 'D';
            break;
        case 4:
            s[cur++] = 'L';
            break;
        case 5:
            s[cur++] = 'B';
            break;
        }
        switch (search->po[i]) {
        case 1:
            s[cur++] = ' ';
            break;
        case 2:
            s[cur++] = '2';
            s[cur++] = ' ';
            break;
        case 3:
            s[cur++] = '\'';
            //s[cur++] = '3';
            s[cur++] = ' ';
            break;
        }
        if (i == depthPhase1 - 1) {
            s[cur++] = '.';
            s[cur++] = ' ';
        }
    }
    return s;
}


char* threadSafeKociemba::solution(char* facelets, int maxDepth, long timeOut, int useSeparator, const char* cache_dir)
{
    search_t* search = (search_t*)calloc(1, sizeof(search_t));
    facecube_t* fc;
    cubiecube_t* cc;
    coordcube_t* c;

    int s, i;
    int mv, n;
    int busy;
    int depthPhase1;
    time_t tStart;
    // +++++++++++++++++++++check for wrong input +++++++++++++++++++++++++++++
    int count[6] = { 0 };

    if (PRUNING_INITED == 0) {
        initPruning(cache_dir);
    }

    for (i = 0; i < 54; i++)
        switch (facelets[i]) {
        case 'U':
            count[U]++;
            break;
        case 'R':
            count[R]++;
            break;
        case 'F':
            count[F]++;
            break;
        case 'D':
            count[D]++;
            break;
        case 'L':
            count[L]++;
            break;
        case 'B':
            count[B]++;
            break;
        }

    for (i = 0; i < 6; i++)
        if (count[i] != 9) {
            free(search);
            return NULL;
        }

    fc = get_facecube_fromstring(facelets);
    cc = toCubieCube(fc);
    if ((s = verify(cc)) != 0) {
        free(search);
        return NULL;
    }

    // +++++++++++++++++++++++ initialization +++++++++++++++++++++++++++++++++
    c = get_coordcube(cc);

    search->po[0] = 0;
    search->ax[0] = 0;
    search->flip[0] = c->flip;
    search->twist[0] = c->twist;
    search->parity[0] = c->parity;
    search->slice[0] = c->FRtoBR / 24;
    search->URFtoDLF[0] = c->URFtoDLF;
    search->FRtoBR[0] = c->FRtoBR;
    search->URtoUL[0] = c->URtoUL;
    search->UBtoDF[0] = c->UBtoDF;

    search->minDistPhase1[1] = 1;// else failure for depth=1, n=0
    mv = 0;
    n = 0;
    busy = 0;
    depthPhase1 = 1;

    tStart = time(NULL);

    // +++++++++++++++++++ Main loop ++++++++++++++++++++++++++++++++++++++++++
    do {
        do {
            if ((depthPhase1 - n > search->minDistPhase1[n + 1]) && !busy) {

                if (search->ax[n] == 0 || search->ax[n] == 3)// Initialize next move
                    search->ax[++n] = 1;
                else
                    search->ax[++n] = 0;
                search->po[n] = 1;
            }
            else if (++search->po[n] > 3) {
                do {// increment axis
                    if (++search->ax[n] > 5) {

                        if (time(NULL) - tStart > timeOut)
                            return NULL;

                        if (n == 0) {
                            if (depthPhase1 >= maxDepth)
                                return NULL;
                            else {
                                depthPhase1++;
                                search->ax[n] = 0;
                                search->po[n] = 1;
                                busy = 0;
                                break;
                            }
                        }
                        else {
                            n--;
                            busy = 1;
                            break;
                        }

                    }
                    else {
                        search->po[n] = 1;
                        busy = 0;
                    }
                } while (n != 0 && (search->ax[n - 1] == search->ax[n] || search->ax[n - 1] - 3 == search->ax[n]));
            }
            else
                busy = 0;
        } while (busy);

        // +++++++++++++ compute new coordinates and new minDistPhase1 ++++++++++
        // if minDistPhase1 =0, the H subgroup is reached
        mv = 3 * search->ax[n] + search->po[n] - 1;
        search->flip[n + 1] = flipMove[search->flip[n]][mv];
        search->twist[n + 1] = twistMove[search->twist[n]][mv];
        search->slice[n + 1] = FRtoBR_Move[search->slice[n] * 24][mv] / 24;
        search->minDistPhase1[n + 1] = MAX(
            getPruning(Slice_Flip_Prun, N_SLICE1 * search->flip[n + 1] + search->slice[n + 1]),
            getPruning(Slice_Twist_Prun, N_SLICE1 * search->twist[n + 1] + search->slice[n + 1])
        );
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // System.out.format("%d %d\n", n, depthPhase1);
        if (search->minDistPhase1[n + 1] == 0 && n >= depthPhase1 - 5) {
            search->minDistPhase1[n + 1] = 10;// instead of 10 any value >5 is possible
            if (n == depthPhase1 - 1 && (s = totalDepth(search, depthPhase1, maxDepth)) >= 0) {
                if (s == depthPhase1
                    || (search->ax[depthPhase1 - 1] != search->ax[depthPhase1] && search->ax[depthPhase1 - 1] != search->ax[depthPhase1] + 3)) {
                    char* res;
                    free((void*)fc);
                    free((void*)cc);
                    free((void*)c);
                    if (useSeparator) {
                        res = solutionToString(search, s, depthPhase1);
                    }
                    else {
                        res = solutionToString(search, s, -1);
                    }
                    free((void*)search);
                    return res;
                }
            }

        }
    } while (1);
}

int threadSafeKociemba::totalDepth(search_t* search, int depthPhase1, int maxDepth)
{
    int mv = 0, d1 = 0, d2 = 0, i;
    int maxDepthPhase2 = MIN(10, maxDepth - depthPhase1);// Allow only max 10 moves in phase2
    int depthPhase2;
    int n;
    int busy;
    for (i = 0; i < depthPhase1; i++) {
        mv = 3 * search->ax[i] + search->po[i] - 1;
        // System.out.format("%d %d %d %d\n", i, mv, ax[i], po[i]);
        search->URFtoDLF[i + 1] = URFtoDLF_Move[search->URFtoDLF[i]][mv];
        search->FRtoBR[i + 1] = FRtoBR_Move[search->FRtoBR[i]][mv];
        search->parity[i + 1] = parityMove[search->parity[i]][mv];
    }

    if ((d1 = getPruning(Slice_URFtoDLF_Parity_Prun,
        (N_SLICE2 * search->URFtoDLF[depthPhase1] + search->FRtoBR[depthPhase1]) * 2 + search->parity[depthPhase1])) > maxDepthPhase2)
        return -1;

    for (i = 0; i < depthPhase1; i++) {
        mv = 3 * search->ax[i] + search->po[i] - 1;
        search->URtoUL[i + 1] = URtoUL_Move[search->URtoUL[i]][mv];
        search->UBtoDF[i + 1] = UBtoDF_Move[search->UBtoDF[i]][mv];
    }
    search->URtoDF[depthPhase1] = MergeURtoULandUBtoDF[search->URtoUL[depthPhase1]][search->UBtoDF[depthPhase1]];

    if ((d2 = getPruning(Slice_URtoDF_Parity_Prun,
        (N_SLICE2 * search->URtoDF[depthPhase1] + search->FRtoBR[depthPhase1]) * 2 + search->parity[depthPhase1])) > maxDepthPhase2)
        return -1;

    if ((search->minDistPhase2[depthPhase1] = MAX(d1, d2)) == 0)// already solved
        return depthPhase1;

    // now set up search

    depthPhase2 = 1;
    n = depthPhase1;
    busy = 0;
    search->po[depthPhase1] = 0;
    search->ax[depthPhase1] = 0;
    search->minDistPhase2[n + 1] = 1;// else failure for depthPhase2=1, n=0
    // +++++++++++++++++++ end initialization +++++++++++++++++++++++++++++++++
    do {
        do {
            if ((depthPhase1 + depthPhase2 - n > search->minDistPhase2[n + 1]) && !busy) {

                if (search->ax[n] == 0 || search->ax[n] == 3)// Initialize next move
                {
                    search->ax[++n] = 1;
                    search->po[n] = 2;
                }
                else {
                    search->ax[++n] = 0;
                    search->po[n] = 1;
                }
            }
            else if ((search->ax[n] == 0 || search->ax[n] == 3) ? (++search->po[n] > 3) : ((search->po[n] = search->po[n] + 2) > 3)) {
                do {// increment axis
                    if (++search->ax[n] > 5) {
                        if (n == depthPhase1) {
                            if (depthPhase2 >= maxDepthPhase2)
                                return -1;
                            else {
                                depthPhase2++;
                                search->ax[n] = 0;
                                search->po[n] = 1;
                                busy = 0;
                                break;
                            }
                        }
                        else {
                            n--;
                            busy = 1;
                            break;
                        }

                    }
                    else {
                        if (search->ax[n] == 0 || search->ax[n] == 3)
                            search->po[n] = 1;
                        else
                            search->po[n] = 2;
                        busy = 0;
                    }
                } while (n != depthPhase1 && (search->ax[n - 1] == search->ax[n] || search->ax[n - 1] - 3 == search->ax[n]));
            }
            else
                busy = 0;
        } while (busy);
        // +++++++++++++ compute new coordinates and new minDist ++++++++++
        mv = 3 * search->ax[n] + search->po[n] - 1;

        search->URFtoDLF[n + 1] = URFtoDLF_Move[search->URFtoDLF[n]][mv];
        search->FRtoBR[n + 1] = FRtoBR_Move[search->FRtoBR[n]][mv];
        search->parity[n + 1] = parityMove[search->parity[n]][mv];
        search->URtoDF[n + 1] = URtoDF_Move[search->URtoDF[n]][mv];

        search->minDistPhase2[n + 1] = MAX(getPruning(Slice_URtoDF_Parity_Prun, (N_SLICE2
            * search->URtoDF[n + 1] + search->FRtoBR[n + 1])
            * 2 + search->parity[n + 1]), getPruning(Slice_URFtoDLF_Parity_Prun, (N_SLICE2
                * search->URFtoDLF[n + 1] + search->FRtoBR[n + 1])
                * 2 + search->parity[n + 1]));
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    } while (search->minDistPhase2[n + 1] != 0);
    return depthPhase1 + depthPhase2;
}

void threadSafeKociemba::patternize(char* facelets, char* pattern, char* patternized)
{
    facecube_t* fc;
    facecube_t* start_fc = get_facecube_fromstring(facelets);
    facecube_t* pattern_fc = get_facecube_fromstring(pattern);
    cubiecube_t* start_cc = toCubieCube(start_fc);
    cubiecube_t* pattern_cc = toCubieCube(pattern_fc);
    cubiecube_t* inv_pattern_cc = get_cubiecube();
    invCubieCube(pattern_cc, inv_pattern_cc);
    multiply(inv_pattern_cc, start_cc);
    fc = toFaceCube(inv_pattern_cc);
    to_String(fc, patternized);
    free(start_fc);
    free(pattern_fc);
    free(start_cc);
    free(pattern_cc);
    free(inv_pattern_cc);
    free(fc);
}





//solve.c
char* threadSafeKociemba::cube_solve(char* arg1, char* arg2)
{
    char patternized[64];
    char* facelets = arg1;
    if (arg2 != NULL) {
        patternize(facelets, arg2, patternized);
        facelets = patternized;
    }
    char* sol = solution(
        facelets,
        24,
        1000,
        0,
        "cache"
    );
    return sol;
}