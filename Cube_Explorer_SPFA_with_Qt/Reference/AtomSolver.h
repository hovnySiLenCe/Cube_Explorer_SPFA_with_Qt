#ifndef ATOM_SOLVER
#define ATOM_SOLVER
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <stack>
using namespace std;

struct AtomStep {
    int8_t oldposestate;
    int8_t oldhandstate;
    int8_t newposestate;
    int8_t newhandstate;
    int8_t step[22];
    int8_t stepcount;
    uint16_t time;
    void init() {
        oldposestate = -1;
        oldhandstate = -1;
        newposestate = -1;
        newhandstate = -1;
        for (int ii = 0; ii < 22; ii++)step[ii] = -1;
        stepcount = 0;
        time = 65500;
    }
    void set(int8_t num_oldposestate, int8_t num_oldhandstate, int8_t num_newposestate, int num_newhandstate, int8_t series_step[], int8_t num_stepcount, uint16_t num_time) {
        oldposestate = num_oldposestate;
        oldhandstate = num_oldhandstate;
        newposestate = num_newposestate;
        newhandstate = num_newhandstate;
        for (int ii = 0; ii < num_stepcount; ii++) step[ii] = series_step[ii];
        stepcount = num_stepcount;
        time = num_time;
    }
};

struct SolNode {
    int8_t FormerState;
    int8_t Method;
    int8_t CurrentState;
    int CurrentTimeCnt;
    int CurrentStepCnt;
    void init() {
        FormerState = -1;
        Method = -1;
        CurrentState = -1;
        CurrentTimeCnt = 999999;
        CurrentStepCnt = 999999;
    }
};

struct AccurateStepInfo {
    int8_t PoseSta;
    int8_t HandSta;
    int8_t Method;
};

class AtomSolver {
private:
    string mecstep_str[10] = { "M_L1", "M_L2", "M_L3", "M_LC", "M_LO", "M_R1", "M_R2", "M_R3", "M_RC", "M_RO" };
    string mecstepser_str[10] = { "#1P6T200\r\n","#1P8T200\r\n","#1P7T200\r\n","#2P1T200\r\n","#2P0T200\r\n","#3P6T200\r\n","#3P8T200\r\n", "#3P7T200\r\n", "#4P1T200\r\n", "#4P0T200\r\n" };
    map<char, int>Face2FaceID;
    AtomStep AtomLib[24][3][18][16];
    SolNode StateMatrix[55][72];
    //定以时间最少优先还是步数最少优先
    bool TimePrefer = false;

    vector<string>VecMecStr;
    vector<string>VecMecSerStr;
    vector<string>VecKCBStr;
    vector<int8_t>VecMec;
    vector<int8_t>VecKCB;
    
    void InitLib();
    void InitStateMatrix();
    void BFS(int depth);
    void UpdateNode(int8_t state, int8_t method, int depth, int8_t aimstate, int temptime, int tempstep);
    void GenerateAns();
    //******
    void InitLib_FR();
    void InitLib_FU();
    void InitLib_FL();
    void InitLib_FD();
    void InitLib_RF();
    void InitLib_RU();
    void InitLib_RB();
    void InitLib_RD();
    void InitLib_UF();
    void InitLib_UR();
    void InitLib_UB();
    void InitLib_UL();
    void InitLib_BR();
    void InitLib_BU();
    void InitLib_BL();
    void InitLib_BD();
    void InitLib_LF();
    void InitLib_LU();
    void InitLib_LB();
    void InitLib_LD();
    void InitLib_DF();
    void InitLib_DR();
    void InitLib_DB();
    void InitLib_DL();
    //******
public:
    AtomSolver() {
        Init();
    }
    bool HaveAns = false;
    int StepCnt = 0;
    int EstimateTime = 0;
    void Init();
    void ClearTempVar();
    void SetTheoryStep(string str);
    void Calculate(int8_t IniPoseCode, bool TimePreferred);
    vector<string>& GetVecKCBStr();
    vector<string>& GetVecMecStr();
    vector<string>& GetVecMecSerStr();
};


#endif // !ATOM_CUBE_SOLVER