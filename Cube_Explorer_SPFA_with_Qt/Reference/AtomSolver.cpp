#include "AtomSolver.h"

//定义所有魔方姿态
#define FR 0
#define FU 1
#define FL 2
#define FD 3
#define RF 4
#define RU 5
#define RB 6
#define RD 7
#define UF 8
#define UR 9
#define UB 10
#define UL 11
#define BR 12
#define BU 13
#define BL 14
#define BD 15
#define LF 16
#define LU 17
#define LB 18
#define LD 19
#define DF 20
#define DR 21
#define DB 22
#define DL 23

//定义机械臂归位情况
#define L0R0 0
#define L0R1 1
#define L1R0 2

//定以时间最少优先还是步数最少优先
//#define TimePrefer 0

void AtomSolver::Init() {
    InitLib();
    Face2FaceID['F'] = 0; Face2FaceID['R'] = 1; Face2FaceID['U'] = 2; Face2FaceID['B'] = 3; Face2FaceID['L'] = 4; Face2FaceID['D'] = 5;
    InitStateMatrix();
    ClearTempVar();
}

void AtomSolver::ClearTempVar(){
    InitStateMatrix();
    VecKCBStr.clear();
    VecKCB.clear();
    VecMecStr.clear();
    VecMecSerStr.clear();
    VecMec.clear();
    HaveAns = false;
    StepCnt = 0;
    EstimateTime = 0;
}

void AtomSolver::SetTheoryStep(string str){
    vector<string>raw;
    istringstream iss(str);
    string token;

    while (getline(iss, token, ' ')) {
        if (!token.empty()) {
            raw.push_back(token);
        }
    }
    for (int i = 0; i < raw.size(); i++) {
        if (raw[i].length() == 1)raw[i] = raw[i] + '1';
        else if (raw[i][1] == '\'')raw[i][1] = '3';
        VecKCBStr.push_back(raw[i]);
        int OperaID = Face2FaceID[raw[i][0]] * 3 + (raw[i][1] - '0' - 1);
        VecKCB.push_back(OperaID);
    }
    return;
}

void AtomSolver::InitStateMatrix() {
    for (int i = 0; i < 55; i++) {
        for (int j = 0; j < 72; j++) {
            StateMatrix[i][j].init();
        }
    }
}

void AtomSolver::Calculate(int8_t IniPoseCode, bool TimePreferred){
    TimePrefer = TimePreferred;
    //从初始状态开始，先生成根节点在第0层，默认姿态为IniPoseCode，两爪归位
    StateMatrix[0][IniPoseCode * 3 + L0R0].CurrentState = IniPoseCode * 3 + L0R0;
    for (int i = 0; i < 16; i++) UpdateNode(IniPoseCode * 3 + L0R0, i, 1, AtomLib[IniPoseCode][L0R0][VecKCB[0]][i].newposestate * 3 + AtomLib[IniPoseCode][L0R0][VecKCB[0]][i].newhandstate, 0 + AtomLib[IniPoseCode][L0R0][VecKCB[0]][i].time, 0 + AtomLib[IniPoseCode][L0R0][VecKCB[0]][i].stepcount);
    BFS(1);
    GenerateAns();
}

void AtomSolver::BFS(int depth) {
    if (depth == VecKCB.size())return;
    int NodeCnt = 0;
    for (int i = 0; i < 72; i++) {
        if (StateMatrix[depth][i].CurrentState != -1) {
            NodeCnt++;
            for (int j = 0; j < 16; j++) {
                int8_t PoseSta = i / 3;
                int8_t HandSta = i % 3;
                AtomStep CurrentStep = AtomLib[PoseSta][HandSta][VecKCB[depth]][j];
                UpdateNode(i, j, depth + 1, CurrentStep.newposestate * 3 + CurrentStep.newhandstate, StateMatrix[depth][i].CurrentTimeCnt + CurrentStep.time, StateMatrix[depth][i].CurrentStepCnt + CurrentStep.stepcount);
            }
        }
    }
    if (NodeCnt != 16) {
        printf("Node Cnt Problem\n");
        return;
    }
    BFS(depth + 1);
    return;
}

void AtomSolver::UpdateNode(int8_t state, int8_t method, int depth, int8_t aimstate, int temptime, int tempstep) {
    bool NeedUpdate = false;
    if (TimePrefer == 1) {
        if (temptime < StateMatrix[depth][aimstate].CurrentTimeCnt || (temptime == StateMatrix[depth][aimstate].CurrentTimeCnt && tempstep < StateMatrix[depth][aimstate].CurrentStepCnt))
            NeedUpdate = true;
    }
    else {
        if (tempstep < StateMatrix[depth][aimstate].CurrentStepCnt || (tempstep == StateMatrix[depth][aimstate].CurrentStepCnt && temptime < StateMatrix[depth][aimstate].CurrentTimeCnt))
            NeedUpdate = true;
    }
    
    if (NeedUpdate) {
        StateMatrix[depth][aimstate].FormerState = state;
        StateMatrix[depth][aimstate].Method = method;
        StateMatrix[depth][aimstate].CurrentState = aimstate;
        StateMatrix[depth][aimstate].CurrentTimeCnt = temptime;
        StateMatrix[depth][aimstate].CurrentStepCnt = tempstep;
    }
    return;
}

void AtomSolver::GenerateAns() {
    int beststa = -1;
    int beststepcnt = 99999;
    int besttimecnt = 99999;
    
    for (int i = 0; i < 72; i++) {
        if (TimePrefer == 1) {
            if (StateMatrix[VecKCB.size()][i].CurrentTimeCnt < besttimecnt || (StateMatrix[VecKCB.size()][i].CurrentTimeCnt == besttimecnt && StateMatrix[VecKCB.size()][i].CurrentStepCnt < beststepcnt)) {
                beststa = i;
                beststepcnt = StateMatrix[VecKCB.size()][i].CurrentStepCnt;
                besttimecnt = StateMatrix[VecKCB.size()][i].CurrentTimeCnt;
            }
        }
        else {
            if (StateMatrix[VecKCB.size()][i].CurrentStepCnt < beststepcnt || (StateMatrix[VecKCB.size()][i].CurrentStepCnt == beststepcnt && StateMatrix[VecKCB.size()][i].CurrentTimeCnt < besttimecnt)) {
                beststa = i;
                beststepcnt = StateMatrix[VecKCB.size()][i].CurrentStepCnt;
                besttimecnt = StateMatrix[VecKCB.size()][i].CurrentTimeCnt;
            }
        }
    }

    if (beststa == -1) {
        printf("No Legal State Found!\n");
        HaveAns = false;
        ClearTempVar();
        return;
    }

    int depth = VecKCB.size();
    int sta = beststa;
    stack<AccurateStepInfo>AllStepInfo;
    while (depth > 0) {
        AccurateStepInfo TempInfo;
        TempInfo.PoseSta = StateMatrix[depth][sta].FormerState / 3;
        TempInfo.HandSta = StateMatrix[depth][sta].FormerState % 3;
        TempInfo.Method = StateMatrix[depth][sta].Method;
        AllStepInfo.push(TempInfo);
        sta = StateMatrix[depth][sta].FormerState;
        depth--;
    }

    if (AllStepInfo.size() != VecKCB.size()) {
        printf("No Enough Step Info Found!\n");
        HaveAns = false;
        ClearTempVar();
        return;
    }
    for (int i = 0; i < VecKCB.size(); i++) {
        AccurateStepInfo TempInfo;
        AtomStep TheAtomStep;
        TempInfo = AllStepInfo.top();
        AllStepInfo.pop();
        TheAtomStep = AtomLib[TempInfo.PoseSta][TempInfo.HandSta][VecKCB[i]][TempInfo.Method];
        StepCnt += TheAtomStep.stepcount;
        EstimateTime += TheAtomStep.time;
        for (int j = 0; j < TheAtomStep.stepcount; j++) {
            VecMec.push_back(TheAtomStep.step[j]);
            VecMecStr.push_back(mecstep_str[TheAtomStep.step[j]]);
            VecMecSerStr.push_back(mecstepser_str[TheAtomStep.step[j]]);
        }
    }
    HaveAns = true;
    return;
}

vector<string>& AtomSolver::GetVecMecStr(){
    return VecMecStr;
}
vector<string>& AtomSolver::GetVecKCBStr() {
    return VecKCBStr;
}
vector<string>& AtomSolver::GetVecMecSerStr() {
    return VecMecSerStr;
}