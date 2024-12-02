
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <fstream>

using namespace std;


#define instructionList0Addr 0x0                 // Start address of instruction list
#define instructionList1Addr 0x0                 // Start address of instruction list
#define stack0Addr 0x0200                        // Start address of stack 0
#define stack1Addr 0x0300                        // Start address of stack 1

#define ARRAY_A_ADDR 0x0400                     // Start address of Array A
#define ARRAY_B_ADDR 0x0800                     // Start address of Array B
#define ARRAY_C_ADDR 0x0C00                     // Start address of Array C
#define ARRAY_D_ADDR 0x1000                     // Start address of Array D

typedef struct memoryArray {                    // Structure that contains array in memory
    float mainArray[256] = {};
    float* mainArrayAddress[256] = {};
};

float* ARRAY_A = (float *)(void *)ARRAY_A_ADDR;                     // Assigns address to array
float* ARRAY_B = (float *)(void *)ARRAY_B_ADDR;                     // Assigns address to array
float* ARRAY_C = (float *)(void *)ARRAY_C_ADDR;                     // Assigns address to array
float* ARRAY_D = (float *)(void *)ARRAY_D_ADDR;                     // Assigns address to array
int* instruction0List = (int*)(void*)instructionList0Addr;          // Assigns address to instruction list
int* instruction1List = (int *)(void *)instructionList1Addr;          // Assigns address to instruction list
float* stack0 = (float *)(void *)stack0Addr;                          // Assigns address to stack 0
float* stack1 = (float*)(void*)stack1Addr;                          // Assigns address to stack 1


// List of variable used to perform simulation
bool rTypeAdd = false;
bool iTypeAdd = false;
bool sTypeAdd = false;             // Variables to keep track of which instruction type each instruction is
bool bTypeAdd = false;
bool uTypeAdd = false;
bool jTypeAdd = false;

bool rTypeSub = false;
bool iTypeSub = false;
bool sTypeSub = false;             // Variables to keep track of which instruction type each instruction is
bool bTypeSub = false;
bool uTypeSub = false;
bool jTypeSub = false;

bool rTypeExecuteAdd = false;
bool iTypeExecuteAdd = false;
bool sTypeExecuteAdd = false;             // Variables to keep track of which instruction type each instruction is
bool bTypeExecuteAdd = false;
bool uTypeExecuteAdd = false;
bool jTypeExecuteAdd = false;

int iTypeCateExecuteAdd = 0;
int iTypeCateAdd = 0;          // Determines which type of I instruction it is

bool rTypeExecuteSub = false;
bool iTypeExecuteSub = false;
bool sTypeExecuteSub = false;             // Variables to keep track of which instruction type each instruction is
bool bTypeExecuteSub = false;
bool uTypeExecuteSub = false;
bool jTypeExecuteSub = false;

int iTypeCateExecuteSub = 0;
int iTypeCateSub = 0;          // Determines which type of I instruction it is

int rdAdd[5] = {};                     // Rd is x8 (0b01000)
int rs1Add[5] = {};                    // Rs1 is x9 (0b01001)
int rs2Add[5] = {};                    // Rs2 is x10 (0b01010)
int imm_ItypeAdd[12] = {};
int imm_StypeUpperAdd[7] = {};
int imm_StypeLowerAdd[5] = {};
int imm_BtypeUpperAdd[7] = {};
int imm_BtypeLowerAdd[5] = {};
int imm_UtypeAdd[20] = {};
int imm_JtypeAdd[20] = {};

int rdSub[5] = {};                     // Rd is x8 (0b01000)
int rs1Sub[5] = {};                    // Rs1 is x9 (0b01001)
int rs2Sub[5] = {};                    // Rs2 is x10 (0b01010)
int imm_ItypeSub[12] = {};
int imm_StypeUpperSub[7] = {};
int imm_StypeLowerSub[5] = {};
int imm_BtypeUpperSub[7] = {};
int imm_BtypeLowerSub[5] = {};
int imm_UtypeSub[20] = {};
int imm_JtypeSub[20] = {};

int rdToDecimalAdd = 0;                // Rd decoded decimal equivalent
int rs1ToDecimalAdd = 0;               // Rs1 decoded decimal equivalent
int rs2ToDecimalAdd = 0;               // Rs2 decoded decimal equivalent
int immToDecimalAdd = 0;               // Immediate decoded decimal equivalent

int rdToDecimalExecuteAdd = 0;                // Rd decoded decimal equivalent
int rs1ToDecimalExecuteAdd = 0;               // Rs1 decoded decimal equivalent
int rs2ToDecimalExecuteAdd = 0;               // Rs2 decoded decimal equivalent
int immToDecimalExecuteAdd = 0;               // Immediate decoded decimal equivalent

int rdToDecimalSub = 0;                // Rd decoded decimal equivalent
int rs1ToDecimalSub = 0;               // Rs1 decoded decimal equivalent
int rs2ToDecimalSub = 0;               // Rs2 decoded decimal equivalent
int immToDecimalSub = 0;               // Immediate decoded decimal equivalent

int rdToDecimalExecuteSub = 0;                // Rd decoded decimal equivalent
int rs1ToDecimalExecuteSub = 0;               // Rs1 decoded decimal equivalent
int rs2ToDecimalExecuteSub = 0;               // Rs2 decoded decimal equivalent
int immToDecimalExecuteSub = 0;               // Immediate decoded decimal equivalent

int inputArrayAdd[32] = {};   // LSB is the rightmost bit
int inputArraySub[32] = {};   // LSB is the rightmost bit


// Variables that store info/states for printing
string opcodeTypeAdd;
string opcodeTypeSub;
string instAdd;
string instSub;
string instExecuteAdd;
string instExecuteSub;
int aluOpAdd;
int branchAdd;
int memReadAdd;
int writeBackSelAdd;
int memWriteAdd;
int aluSrcAdd;
int regWriteAdd;
int signExtAdd;
int pcSelAdd;
int branchConditionAdd;
int branchSelAdd;
int aluOpSub;
int branchSub;
int memReadSub;
int writeBackSelSub;
int memWriteSub;
int aluSrcSub;
int regWriteSub;
int signExtSub;
int pcSelSub;
int branchConditionSub;
int branchSelSub;
int arrayIndexAdd = 0;
int arrayIndexSub = 0;

double clockcycle = 0;
int offsetAdd = 0;
int offsetSub = 0;
int loadNewStage = 0;
int floatPresent = 0;
int memBusSelect = 0;

double cpiCount = 0;
double instructionAddCount = 0;
double instructionSubCount = 0;


// Call to the functions that will decode and print output
void determineOperationAdd();
void determineOperationSub();
void printEventDecodeAdd();
void printEventDecodeSub();
void assignRegisterFieldsAdd();
void assignRegisterFieldsSub();
void create32bitInstructionAdd(int instructionInLoop);
void create32bitInstructionSub(int instructionInLoop);
void decToBinaryNormal(int n, int addOrSub);
void decToBinarySplit(int n, int floatOrInt, int addOrSub);
void printEventExecuteAdd();
void printEventExecuteSub();
void fetchStage(int loadPrevStage);
void decodeStage(int loadPrevStage);
void executeStage(int loadPrevStage);
void writebackStage(int loadPrevStage);


fstream myFileAdd;         // Used to open and write to instruction file
fstream myFileSub;

string lineFetchAdd;                   // Used to get Fetched instruction from list
string lineFetchSub;                   // Used to get Fetched instruction from list
string lineDecodeAdd;                  // Used to decode instruction from fetched instruction
string lineDecodeSub;                  // Used to decode instruction from fetched instruction
int tempArray[32];                  // Variable used to tranlate ASCII to integer numbers in decode process
int count32InstructionAdd = 0;         // Determines what type of instruction was created
int count32InstructionSub = 0;         // Determines what type of instruction was created
int timesFetchOccured = 0;          // Increments pipeline

memoryArray A;      // Array A
memoryArray B;      // Array B
memoryArray C;      // Array C
memoryArray D;      // Array D

// Main Function

// Function to save array contents to a file
void saveArrayContents(const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    // Write contents of ARRAY_A
    outfile << "Contents of ARRAY_A:\n";

    // Write contents of ARRAY_A
    outfile << "Contents of ARRAY_A:\n";
    for (int i = 0; i < 256; ++i) {
        outfile << A.mainArray[i] << " ";
        if ((i + 1) % 10 == 0) outfile << "\n";
    }
    outfile << "\n\n";

    // Write contents of ARRAY_B
    outfile << "Contents of ARRAY_B:\n";
    for (int i = 0; i < 256; ++i) {
        outfile << B.mainArray[i] << " ";
        if ((i + 1) % 10 == 0) outfile << "\n";
    }
    outfile << "\n\n";
    for (int i = 0; i < 256; ++i) {
        A.mainArray[i] = 100 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100)); // Values above 100
        if ((i + 1) % 10 == 0) outfile << "\n"; // Line break after 10 elements
    }
    outfile << "\n\n";

    // Write contents of ARRAY_B
    outfile << "Contents of ARRAY_B:\n";
    for (int i = 0; i < 256; ++i) {
        B.mainArray[i] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 30)); // Values between 0 and 30
        if ((i + 1) % 10 == 0) outfile << "\n";
    }
    outfile << "\n\n";

    // Write contents of ARRAY_C
    outfile << "Contents of ARRAY_C:\n";
    for (int i = 0; i < 256; ++i) {
        outfile << C.mainArray[i] << " ";
        if ((i + 1) % 10 == 0) outfile << "\n";
    }
    outfile << "\n\n";

    // Write contents of ARRAY_D
    outfile << "Contents of ARRAY_D:\n";
    for (int i = 0; i < 256; ++i) {
        outfile << D.mainArray[i] << " ";
        if ((i + 1) % 10 == 0) outfile << "\n";
    }
    outfile << "\n\n";

    outfile.close();
    std::cout << "Array contents saved to " << filename << "\n";
}

int main() {


    srand(time(0));                             // Randomizes seed
    int waitOneLoop = 0;                        // Variable to wait an extra loop if stall
    int stallAmount = 0;                        // Variable to help determine how many stalls needed
    int stallOccured = 0;                       // Variable to kepp track if a stall occured


    for (int i = 0; i < 256; i++) {             // Loop randomly assigns values to Array A and Array B
        
        A.mainArrayAddress[i] = ARRAY_A + (0x1) * i;        // Maps all the addresses to Array A
        B.mainArrayAddress[i] = ARRAY_B + (0x1) * i;        // Maps all the addresses to Array B
        C.mainArrayAddress[i] = ARRAY_C + (0x1) * i;        // Maps all the addresses to Array C
        D.mainArrayAddress[i] = ARRAY_D + (0x1) * i;

        int floatOrInt = rand() % 2;

        if (floatOrInt == 1) {                                      // If 1 assigns float value
        A.mainArray[i] = 100 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100)); // Values above 100
        B.mainArray[i] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 30)); // Values between 0 and 30
        }
        else {                                                      // If 0 assigns int value
        A.mainArray[i] = 100 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100)); // Values above 100
        B.mainArray[i] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 30)); // Values between 0 and 30
        }

    }


    while (arrayIndexAdd < 257 && arrayIndexSub < 257 && ((count32InstructionAdd != 2 && count32InstructionSub != 2) || (arrayIndexAdd != 256 || arrayIndexSub != 256))) {          // Runs for all index values

        cout << endl << "##############################################################" << endl;

        if (loadNewStage == 2) {                // In case of stall
            writebackStage(loadNewStage);
            waitOneLoop = 1;
        }
        else {
            if ((timesFetchOccured >= 3 && loadNewStage != 0) || (waitOneLoop == 1)) {          // In case of normal operation
                loadNewStage = 1;   
                writebackStage(loadNewStage);

                if (count32InstructionAdd == 3 ) {          // Create stall after executing load A (if integer)

                    if (instExecuteAdd == "Lb " || instExecuteSub == "Lb ") {
                        loadNewStage = 2;
                    }
                    else if (instExecuteAdd == "Flw " || instExecuteSub == "Flw ") {
                        loadNewStage = 1;
                    }


                }
                else if (count32InstructionAdd == 4) {   // Create stall after executing load B (both float and integer)

                    if (instExecuteAdd == "Add " || instExecuteSub == "Sub ") {
                        loadNewStage = 2;
                    }
                    else if (instExecuteAdd == "FADD.S " || instExecuteSub == "FSUB.S ") {
                        loadNewStage = 2;
                    }


                }
                else if (count32InstructionAdd == 5) {   // Create stall after executing fload addition

                    if (instExecuteAdd == "Sb " || instExecuteSub == "Sb ") {
                        loadNewStage = 1;
                    }
                    else if (instExecuteAdd == "Fsw " || instExecuteSub == "Fsw ") {
                        stallAmount++;
                        loadNewStage = 2;                   
                    }


                }

                if (waitOneLoop == 1 && stallAmount == 0) {             // If no stall is no longer needed
                    loadNewStage = 1;
                    waitOneLoop = 0;
                }
                else if (waitOneLoop == 1 && stallAmount == 1) {        // If one stall is needed
                    loadNewStage = 2;
                }
                else if (waitOneLoop == 1 && stallAmount == 3) {        // If two stalls are needed
                    loadNewStage = 1;
                    waitOneLoop = 0;
                    stallAmount = 0;
                }

            }
            else {                                      // In case of no operation
                loadNewStage = 0;
                writebackStage(loadNewStage);
            }
        }

        if (loadNewStage == 2 ) {            // In case of stall

            executeStage(loadNewStage);

        }
        else {
            if (timesFetchOccured >= 2) {         // In case of normal operation

                loadNewStage = 1;
                executeStage(loadNewStage);

            }
            else {                                 // In case of no operation
                loadNewStage = 0;
                executeStage(loadNewStage);
            }
        }

        if (loadNewStage == 2) {                // In case of stall

            decodeStage(loadNewStage);

        }
        else {
            if (timesFetchOccured >= 1) {       // In case of normal operation 
                loadNewStage = 1;
                decodeStage(loadNewStage);
            }
            else {
                loadNewStage = 0;               // In case of no operation
                decodeStage(loadNewStage);
            }
        }


        if (loadNewStage == 2) {                // In case of stall

            fetchStage(loadNewStage);
            loadNewStage = 1;
            waitOneLoop = 1;
            stallOccured = 1;
        }
        else {
            if (timesFetchOccured >= 0) {       // In case of normal operation
                loadNewStage = 1;
                fetchStage(loadNewStage);
            }
            else {                              // In case of no operation
                loadNewStage = 0;
                fetchStage(loadNewStage);
            }
        }



        timesFetchOccured++;        // Increment pipeline

        if ((count32InstructionAdd == 3 || count32InstructionAdd == 4 || count32InstructionAdd == 6) && floatPresent == 0 && stallOccured == 0) {        // Update clockcycle according to instruction last processed
            clockcycle += 3;
        }
        else if ((count32InstructionAdd == 3 || count32InstructionAdd == 4 || count32InstructionAdd == 6) && floatPresent == 1 && stallOccured == 0) {
            clockcycle += 7;
        }
        else if ((count32InstructionAdd == 5  && floatPresent == 1 && stallOccured == 0)) {
            clockcycle += 5;
        }
        else {
            stallOccured = 0;
            clockcycle++;
        }

        cout << endl << "##############################################################" << endl;

    }

    cpiCount = (clockcycle - 1) / instructionAddCount;      // Subtract one to make it current printed clock cycle

    printf("\nCPI Count for CPU 0 is %f", cpiCount);

    cpiCount = (clockcycle - 1) / instructionSubCount;      // Subtract one to make it current printed clock cycle

    printf("\nCPI Count for CPU 1 is %f\n", cpiCount);

	
    // Save array contents to a file
    saveArrayContents("result.txt");
    return 0;
    
}

// Function that displays the output of each instruction including control signals (CPU 0)
void printEventDecodeAdd() {

    cout << "Opcode Type: " << opcodeTypeAdd << endl;

    cout << "Inst: " << instAdd;

    if (rTypeAdd == true) {
        printf("x%d, x%d, x%d \n", rdToDecimalAdd, rs1ToDecimalAdd, rs2ToDecimalAdd);        // Displays the correct instruction structure based on type of instruction
    }
    else if (iTypeAdd == true) {
        if (iTypeCateAdd == 1) {
            printf("x%d, x%d, %d \n", rdToDecimalAdd, rs1ToDecimalAdd, immToDecimalAdd);
        }
        else if (iTypeCateAdd == 2) {
            printf("x%d, %d(x%d) \n", rdToDecimalAdd, immToDecimalAdd, rs1ToDecimalAdd);
        }
    }
    else if (sTypeAdd == true) {
        printf("x%d, %d(x%d) \n", rs2ToDecimalAdd, immToDecimalAdd, rs1ToDecimalAdd);
    }
    else if (bTypeAdd == true) {
        printf("x%d, x%d, Loop \n", rs1ToDecimalAdd, rs2ToDecimalAdd);
    }
    else if (uTypeAdd == true) {
        printf("x%d, %d \n", rdToDecimalAdd, immToDecimalAdd);
    }
    else if (jTypeAdd == true) {
        printf("x%d, %d(x%d) \n", rdToDecimalAdd, immToDecimalAdd, rs1ToDecimalAdd);
    }

    cout << "Control Signals:" << endl;
    cout << "Reg_Write_Enable: " << regWriteAdd << ", Reg_Read_Enable: " << regWriteAdd << ", Alu_op: " << aluOpAdd << endl;                   // Outputs active/non-active control signals
    cout << "Mem_Write_Enable: " << memWriteAdd << ", Mem_Read_Enable: " << memReadAdd << ", Writeback_Enable: " << writeBackSelAdd << endl;
    cout << "Imm_Sel: " << aluSrcAdd << ", Sign_Extend: " << signExtAdd << ", branchAdd: " << branchAdd << ", Branch_Condition: " << branchConditionAdd << endl;
    cout << "Branch_Select: " << branchSelAdd << ", PC_Select " << pcSelAdd << endl;

    instExecuteAdd = instAdd;
    rdToDecimalExecuteAdd = rdToDecimalAdd;
    rs1ToDecimalExecuteAdd = rs1ToDecimalAdd;
    rs2ToDecimalExecuteAdd = rs2ToDecimalAdd;
    immToDecimalExecuteAdd = immToDecimalAdd;

    rTypeExecuteAdd = rTypeAdd;
    iTypeExecuteAdd = iTypeAdd;
    sTypeExecuteAdd = sTypeAdd;
    bTypeExecuteAdd = bTypeAdd;
    uTypeExecuteAdd = uTypeAdd;
    jTypeExecuteAdd = jTypeAdd;
    iTypeCateExecuteAdd = iTypeCateAdd;
}

// Function that displays the output of each instruction including control signals (CPU 1)
void printEventDecodeSub() {

    cout << "Opcode Type: " << opcodeTypeSub << endl;

    cout << "Inst: " << instSub;

    if (rTypeSub == true) {
        printf("x%d, x%d, x%d \n", rdToDecimalSub, rs1ToDecimalSub, rs2ToDecimalSub);        // Displays the correct instruction structure based on type of instruction
    }
    else if (iTypeSub == true) {
        if (iTypeCateSub == 1) {
            printf("x%d, x%d, %d \n", rdToDecimalSub, rs1ToDecimalSub, immToDecimalSub);
        }
        else if (iTypeCateSub == 2) {
            printf("x%d, %d(x%d) \n", rdToDecimalSub, immToDecimalSub, rs1ToDecimalSub);
        }
    }
    else if (sTypeSub == true) {
        printf("x%d, %d(x%d) \n", rs2ToDecimalSub, immToDecimalSub, rs1ToDecimalSub);
    }
    else if (bTypeSub == true) {
        printf("x%d, x%d, Loop \n", rs1ToDecimalSub, rs2ToDecimalSub);
    }
    else if (uTypeSub == true) {
        printf("x%d, %d \n", rdToDecimalSub, immToDecimalSub);
    }
    else if (jTypeSub == true) {
        printf("x%d, %d(x%d) \n", rdToDecimalSub, immToDecimalSub, rs1ToDecimalSub);
    }

    cout << "Control Signals:" << endl;
    cout << "Reg_Write_Enable: " << regWriteSub << ", Reg_Read_Enable: " << regWriteSub << ", Alu_op: " << aluOpSub << endl;                   // Outputs active/non-active control signals
    cout << "Mem_Write_Enable: " << memWriteSub << ", Mem_Read_Enable: " << memReadSub << ", Writeback_Enable: " << writeBackSelSub << endl;
    cout << "Imm_Sel: " << aluSrcSub << ", Sign_Extend: " << signExtSub << ", branchAdd: " << branchSub << ", Branch_Condition: " << branchConditionSub << endl;
    cout << "Branch_Select: " << branchSelSub << ", PC_Select " << pcSelSub << endl;

    instExecuteSub = instSub;
    rdToDecimalExecuteSub = rdToDecimalSub;
    rs1ToDecimalExecuteSub = rs1ToDecimalSub;
    rs2ToDecimalExecuteSub = rs2ToDecimalSub;
    immToDecimalExecuteSub = immToDecimalSub;

    rTypeExecuteSub = rTypeSub;
    iTypeExecuteSub = iTypeSub;
    sTypeExecuteSub = sTypeSub;
    bTypeExecuteSub = bTypeSub;
    uTypeExecuteSub = uTypeSub;
    jTypeExecuteSub = jTypeSub;
    iTypeCateExecuteSub = iTypeCateSub;
}

// Function that displays the output of execute stage (CPU 0)
void printEventExecuteAdd() {

    cout << endl << "--------------------------------------------------------------" << endl;
    cout << endl << "CPU 0 Execute Stage: " << endl;

    cout << "Instruction: " << instExecuteAdd;

    if (rTypeExecuteAdd == true) {
        printf("x%d, x%d, x%d \n", rdToDecimalExecuteAdd, rs1ToDecimalExecuteAdd, rs2ToDecimalExecuteAdd);        // Displays the correct instruction structure based on type of instruction
    }
    else if (iTypeExecuteAdd == true) {
        if (iTypeCateExecuteAdd == 1) {
            printf("x%d, x%d, %d \n", rdToDecimalExecuteAdd, rs1ToDecimalExecuteAdd, immToDecimalExecuteAdd);
        }
        else if (iTypeCateExecuteAdd == 2) {
            printf("x%d, %d(x%d) \n", rdToDecimalExecuteAdd, immToDecimalExecuteAdd, rs1ToDecimalExecuteAdd);
        }
    }
    else if (sTypeExecuteAdd == true) {
        printf("x%d, %d(x%d) \n", rs2ToDecimalExecuteAdd, immToDecimalExecuteAdd, rs1ToDecimalExecuteAdd);
    }
    else if (bTypeExecuteAdd == true) {
        printf("x%d, x%d, Loop \n", rs1ToDecimalExecuteAdd, rs2ToDecimalExecuteAdd);
    }
    else if (uTypeExecuteAdd == true) {
        printf("x%d, %d \n", rdToDecimalExecuteAdd, immToDecimalExecuteAdd);
    }
    else if (jTypeExecuteAdd == true) {
        printf("x%d, %d(x%d) \n", rdToDecimalExecuteAdd, immToDecimalExecuteAdd, rs1ToDecimalExecuteAdd);
    }

    cout << "Clock Cycle: " << clockcycle << endl;

    instructionAddCount++;
}

// Function that displays the output of execute stage (CPU 1)
void printEventExecuteSub() {

    cout << endl << "--------------------------------------------------------------" << endl;
    cout << endl << "CPU 1 Execute Stage: " << endl;

    cout << "Instruction: " << instExecuteSub;

    if (rTypeExecuteSub == true) {
        printf("x%d, x%d, x%d \n", rdToDecimalExecuteSub, rs1ToDecimalExecuteSub, rs2ToDecimalExecuteSub);        // Displays the correct instruction structure based on type of instruction
    }
    else if (iTypeExecuteSub == true) {
        if (iTypeCateExecuteSub == 1) {
            printf("x%d, x%d, %d \n", rdToDecimalExecuteSub, rs1ToDecimalExecuteSub, immToDecimalExecuteSub);
        }
        else if (iTypeCateExecuteSub == 2) {
            printf("x%d, %d(x%d) \n", rdToDecimalExecuteSub, immToDecimalExecuteSub, rs1ToDecimalExecuteSub);
        }
    }
    else if (sTypeExecuteSub == true) {
        printf("x%d, %d(x%d) \n", rs2ToDecimalExecuteSub, immToDecimalExecuteSub, rs1ToDecimalExecuteSub);
    }
    else if (bTypeExecuteSub == true) {
        printf("x%d, x%d, Loop \n", rs1ToDecimalExecuteSub, rs2ToDecimalExecuteSub);
    }
    else if (uTypeExecuteSub == true) {
        printf("x%d, %d \n", rdToDecimalExecuteSub, immToDecimalExecuteSub);
    }
    else if (jTypeExecuteSub == true) {
        printf("x%d, %d(x%d) \n", rdToDecimalExecuteSub, immToDecimalExecuteSub, rs1ToDecimalExecuteSub);
    }

    cout << "Clock Cycle: " << clockcycle << endl;

    instructionSubCount++;
}

// Function that maps and decodes the various parts of the input instruction based on instruction types (CPU 0)
void assignRegisterFieldsAdd() {

    if (rTypeAdd == true) {
        rdAdd[0] = inputArrayAdd[24];         // Decodes register destination
        rdAdd[1] = inputArrayAdd[23];
        rdAdd[2] = inputArrayAdd[22];
        rdAdd[3] = inputArrayAdd[21];
        rdAdd[4] = inputArrayAdd[20];

        rs1Add[0] = inputArrayAdd[16];        // Decodes register 1 field
        rs1Add[1] = inputArrayAdd[15];
        rs1Add[2] = inputArrayAdd[14];
        rs1Add[3] = inputArrayAdd[13];
        rs1Add[4] = inputArrayAdd[12];

        rs2Add[0] = inputArrayAdd[11];        // Decodes register 2 field
        rs2Add[1] = inputArrayAdd[10];
        rs2Add[2] = inputArrayAdd[9];
        rs2Add[3] = inputArrayAdd[8];
        rs2Add[4] = inputArrayAdd[7];
    }
    else if (iTypeAdd == true) {
        rdAdd[0] = inputArrayAdd[24];         // Decodes register destination
        rdAdd[1] = inputArrayAdd[23];
        rdAdd[2] = inputArrayAdd[22];
        rdAdd[3] = inputArrayAdd[21];
        rdAdd[4] = inputArrayAdd[20];

        rs1Add[0] = inputArrayAdd[16];        // Decodes register 1 field
        rs1Add[1] = inputArrayAdd[15];
        rs1Add[2] = inputArrayAdd[14];
        rs1Add[3] = inputArrayAdd[13];
        rs1Add[4] = inputArrayAdd[12];

        imm_ItypeAdd[0] = inputArrayAdd[11];      // Decodes immediate value
        imm_ItypeAdd[1] = inputArrayAdd[10];
        imm_ItypeAdd[2] = inputArrayAdd[9];
        imm_ItypeAdd[3] = inputArrayAdd[8];
        imm_ItypeAdd[4] = inputArrayAdd[7];
        imm_ItypeAdd[5] = inputArrayAdd[6];
        imm_ItypeAdd[6] = inputArrayAdd[5];
        imm_ItypeAdd[7] = inputArrayAdd[4];
        imm_ItypeAdd[8] = inputArrayAdd[3];
        imm_ItypeAdd[9] = inputArrayAdd[2];
        imm_ItypeAdd[10] = inputArrayAdd[1];
        imm_ItypeAdd[11] = inputArrayAdd[0];

        for (int i = 11; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimalAdd += ((imm_ItypeAdd[i]) * pow(2, i));
        }

    }
    else if (sTypeAdd == true) {
        imm_StypeLowerAdd[0] = inputArrayAdd[24];         // Decodes immediate value
        imm_StypeLowerAdd[1] = inputArrayAdd[23];
        imm_StypeLowerAdd[2] = inputArrayAdd[22];
        imm_StypeLowerAdd[3] = inputArrayAdd[21];
        imm_StypeLowerAdd[4] = inputArrayAdd[20];

        rs1Add[0] = inputArrayAdd[16];                    // Decodes register 1 field
        rs1Add[1] = inputArrayAdd[15];
        rs1Add[2] = inputArrayAdd[14];
        rs1Add[3] = inputArrayAdd[13];
        rs1Add[4] = inputArrayAdd[12];

        rs2Add[0] = inputArrayAdd[11];                    // Decodes register 2 field
        rs2Add[1] = inputArrayAdd[10];
        rs2Add[2] = inputArrayAdd[9];
        rs2Add[3] = inputArrayAdd[8];
        rs2Add[4] = inputArrayAdd[7];

        imm_StypeUpperAdd[0] = inputArrayAdd[6];          // Decodes immediate value
        imm_StypeUpperAdd[1] = inputArrayAdd[5];
        imm_StypeUpperAdd[2] = inputArrayAdd[4];
        imm_StypeUpperAdd[3] = inputArrayAdd[3];
        imm_StypeUpperAdd[4] = inputArrayAdd[2];
        imm_StypeUpperAdd[5] = inputArrayAdd[1];
        imm_StypeUpperAdd[6] = inputArrayAdd[0];

        int immIndex = 11;

        for (int i = 6; i > -1; i--) {                                      // Calculates immediate value in decimal (locations are split)
            immToDecimalAdd += ((imm_StypeUpperAdd[i]) * pow(2, immIndex));
            immIndex--;
        }

        for (int i = 4; i > -1; i--) {                                      // Calculates immediate value in decimal (locations are split)
            immToDecimalAdd += ((imm_StypeLowerAdd[i]) * pow(2, immIndex));
            immIndex--;
        }

    }
    else if (bTypeAdd == true) {
        imm_BtypeLowerAdd[0] = inputArrayAdd[24];       // Decodes immediate value
        imm_BtypeLowerAdd[1] = inputArrayAdd[23];
        imm_BtypeLowerAdd[2] = inputArrayAdd[22];
        imm_BtypeLowerAdd[3] = inputArrayAdd[21];
        imm_BtypeLowerAdd[4] = inputArrayAdd[20];

        rs1Add[0] = inputArrayAdd[16];                  // Decodes register 1 field
        rs1Add[1] = inputArrayAdd[15];
        rs1Add[2] = inputArrayAdd[14];
        rs1Add[3] = inputArrayAdd[13];
        rs1Add[4] = inputArrayAdd[12];

        rs2Add[0] = inputArrayAdd[11];                  // Decodes register 2 field
        rs2Add[1] = inputArrayAdd[10];
        rs2Add[2] = inputArrayAdd[9];
        rs2Add[3] = inputArrayAdd[8];
        rs2Add[4] = inputArrayAdd[7];

        imm_BtypeUpperAdd[0] = inputArrayAdd[6];        // Decodes immediate value
        imm_BtypeUpperAdd[1] = inputArrayAdd[5];
        imm_BtypeUpperAdd[2] = inputArrayAdd[4];
        imm_BtypeUpperAdd[3] = inputArrayAdd[3];
        imm_BtypeUpperAdd[4] = inputArrayAdd[2];
        imm_BtypeUpperAdd[5] = inputArrayAdd[1];
        imm_BtypeUpperAdd[6] = inputArrayAdd[0];

        int immIndex = 11;

        for (int i = 6; i > -1; i--) {                                          // Calculates immediate value in decimal (locations are split)
            immToDecimalAdd += ((imm_BtypeUpperAdd[i]) * pow(2, immIndex));
            immIndex--;
        }

        for (int i = 4; i > -1; i--) {                                          // Calculates immediate value in decimal (locations are split)
            immToDecimalAdd += ((imm_BtypeLowerAdd[i]) * pow(2, immIndex));
            immIndex--;
        }

    }
    else if (uTypeAdd == true) {
        rdAdd[0] = inputArrayAdd[24];                     // Decodes register destination
        rdAdd[1] = inputArrayAdd[23];
        rdAdd[2] = inputArrayAdd[22];
        rdAdd[3] = inputArrayAdd[21];
        rdAdd[4] = inputArrayAdd[20];

        imm_UtypeAdd[0] = inputArrayAdd[19];              // Decodes immediate value
        imm_UtypeAdd[1] = inputArrayAdd[18];
        imm_UtypeAdd[2] = inputArrayAdd[17];
        imm_UtypeAdd[3] = inputArrayAdd[16];
        imm_UtypeAdd[4] = inputArrayAdd[15];
        imm_UtypeAdd[5] = inputArrayAdd[14];
        imm_UtypeAdd[6] = inputArrayAdd[13];
        imm_UtypeAdd[7] = inputArrayAdd[12];
        imm_UtypeAdd[8] = inputArrayAdd[11];
        imm_UtypeAdd[9] = inputArrayAdd[10];
        imm_UtypeAdd[10] = inputArrayAdd[9];
        imm_UtypeAdd[11] = inputArrayAdd[8];
        imm_UtypeAdd[12] = inputArrayAdd[7];
        imm_UtypeAdd[13] = inputArrayAdd[6];
        imm_UtypeAdd[14] = inputArrayAdd[5];
        imm_UtypeAdd[15] = inputArrayAdd[4];
        imm_UtypeAdd[16] = inputArrayAdd[3];
        imm_UtypeAdd[17] = inputArrayAdd[2];
        imm_UtypeAdd[18] = inputArrayAdd[1];
        imm_UtypeAdd[19] = inputArrayAdd[0];

        for (int i = 11; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimalAdd += ((imm_UtypeAdd[i]) * pow(2, i));
        }

    }
    else if (jTypeAdd == true) {
        rdAdd[0] = inputArrayAdd[24];                 // Decodes register destination
        rdAdd[1] = inputArrayAdd[23];
        rdAdd[2] = inputArrayAdd[22];
        rdAdd[3] = inputArrayAdd[21];
        rdAdd[4] = inputArrayAdd[20];

        imm_JtypeAdd[0] = inputArrayAdd[19];          // Decodes immediate value
        imm_JtypeAdd[1] = inputArrayAdd[18];
        imm_JtypeAdd[2] = inputArrayAdd[17];
        imm_JtypeAdd[3] = inputArrayAdd[16];
        imm_JtypeAdd[4] = inputArrayAdd[15];
        imm_JtypeAdd[5] = inputArrayAdd[14];
        imm_JtypeAdd[6] = inputArrayAdd[13];
        imm_JtypeAdd[7] = inputArrayAdd[12];
        imm_JtypeAdd[8] = inputArrayAdd[11];
        imm_JtypeAdd[9] = inputArrayAdd[10];
        imm_JtypeAdd[10] = inputArrayAdd[9];
        imm_JtypeAdd[11] = inputArrayAdd[8];
        imm_JtypeAdd[12] = inputArrayAdd[7];
        imm_JtypeAdd[13] = inputArrayAdd[6];
        imm_JtypeAdd[14] = inputArrayAdd[5];
        imm_JtypeAdd[15] = inputArrayAdd[4];
        imm_JtypeAdd[16] = inputArrayAdd[3];
        imm_JtypeAdd[17] = inputArrayAdd[2];
        imm_JtypeAdd[18] = inputArrayAdd[1];
        imm_JtypeAdd[19] = inputArrayAdd[0];

        for (int i = 19; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimalAdd += ((imm_JtypeAdd[i]) * pow(2, i));
        }

    }

    for (int i = 4; i > -1; i--) {
        rdToDecimalAdd += ((rdAdd[i]) * pow(2, i));       // Decodes register destination (coverts to decimal)
    }

    for (int i = 4; i > -1; i--) {
        rs1ToDecimalAdd += ((rs1Add[i]) * pow(2, i));     // Decodes register 1 location (coverts to decimal)
    }

    for (int i = 4; i > -1; i--) {
        rs2ToDecimalAdd += ((rs2Add[i]) * pow(2, i));     // Decodes register 2 location (coverts to decimal)
    }

}

// Function that maps and decodes the various parts of the input instruction based on instruction types (CPU 1)
void assignRegisterFieldsSub() {

    if (rTypeSub == true) {
        rdSub[0] = inputArraySub[24];         // Decodes register destination
        rdSub[1] = inputArraySub[23];
        rdSub[2] = inputArraySub[22];
        rdSub[3] = inputArraySub[21];
        rdSub[4] = inputArraySub[20];

        rs1Sub[0] = inputArraySub[16];        // Decodes register 1 field
        rs1Sub[1] = inputArraySub[15];
        rs1Sub[2] = inputArraySub[14];
        rs1Sub[3] = inputArraySub[13];
        rs1Sub[4] = inputArraySub[12];

        rs2Sub[0] = inputArraySub[11];        // Decodes register 2 field
        rs2Sub[1] = inputArraySub[10];
        rs2Sub[2] = inputArraySub[9];
        rs2Sub[3] = inputArraySub[8];
        rs2Sub[4] = inputArraySub[7];
    }
    else if (iTypeSub == true) {
        rdSub[0] = inputArraySub[24];         // Decodes register destination
        rdSub[1] = inputArraySub[23];
        rdSub[2] = inputArraySub[22];
        rdSub[3] = inputArraySub[21];
        rdSub[4] = inputArraySub[20];

        rs1Sub[0] = inputArraySub[16];        // Decodes register 1 field
        rs1Sub[1] = inputArraySub[15];
        rs1Sub[2] = inputArraySub[14];
        rs1Sub[3] = inputArraySub[13];
        rs1Sub[4] = inputArraySub[12];

        imm_ItypeSub[0] = inputArraySub[11];      // Decodes immediate value
        imm_ItypeSub[1] = inputArraySub[10];
        imm_ItypeSub[2] = inputArraySub[9];
        imm_ItypeSub[3] = inputArraySub[8];
        imm_ItypeSub[4] = inputArraySub[7];
        imm_ItypeSub[5] = inputArraySub[6];
        imm_ItypeSub[6] = inputArraySub[5];
        imm_ItypeSub[7] = inputArraySub[4];
        imm_ItypeSub[8] = inputArraySub[3];
        imm_ItypeSub[9] = inputArraySub[2];
        imm_ItypeSub[10] = inputArraySub[1];
        imm_ItypeSub[11] = inputArraySub[0];

        for (int i = 11; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimalSub += ((imm_ItypeSub[i]) * pow(2, i));
        }

    }
    else if (sTypeSub == true) {
        imm_StypeLowerSub[0] = inputArraySub[24];         // Decodes immediate value
        imm_StypeLowerSub[1] = inputArraySub[23];
        imm_StypeLowerSub[2] = inputArraySub[22];
        imm_StypeLowerSub[3] = inputArraySub[21];
        imm_StypeLowerSub[4] = inputArraySub[20];

        rs1Sub[0] = inputArraySub[16];                    // Decodes register 1 field
        rs1Sub[1] = inputArraySub[15];
        rs1Sub[2] = inputArraySub[14];
        rs1Sub[3] = inputArraySub[13];
        rs1Sub[4] = inputArraySub[12];

        rs2Sub[0] = inputArraySub[11];                    // Decodes register 2 field
        rs2Sub[1] = inputArraySub[10];
        rs2Sub[2] = inputArraySub[9];
        rs2Sub[3] = inputArraySub[8];
        rs2Sub[4] = inputArraySub[7];

        imm_StypeUpperSub[0] = inputArraySub[6];          // Decodes immediate value
        imm_StypeUpperSub[1] = inputArraySub[5];
        imm_StypeUpperSub[2] = inputArraySub[4];
        imm_StypeUpperSub[3] = inputArraySub[3];
        imm_StypeUpperSub[4] = inputArraySub[2];
        imm_StypeUpperSub[5] = inputArraySub[1];
        imm_StypeUpperSub[6] = inputArraySub[0];

        int immIndex = 11;

        for (int i = 6; i > -1; i--) {                                      // Calculates immediate value in decimal (locations are split)
            immToDecimalSub += ((imm_StypeUpperSub[i]) * pow(2, immIndex));
            immIndex--;
        }

        for (int i = 4; i > -1; i--) {                                      // Calculates immediate value in decimal (locations are split)
            immToDecimalSub += ((imm_StypeLowerSub[i]) * pow(2, immIndex));
            immIndex--;
        }

    }
    else if (bTypeSub == true) {
        imm_BtypeLowerSub[0] = inputArraySub[24];       // Decodes immediate value
        imm_BtypeLowerSub[1] = inputArraySub[23];
        imm_BtypeLowerSub[2] = inputArraySub[22];
        imm_BtypeLowerSub[3] = inputArraySub[21];
        imm_BtypeLowerSub[4] = inputArraySub[20];

        rs1Sub[0] = inputArraySub[16];                  // Decodes register 1 field
        rs1Sub[1] = inputArraySub[15];
        rs1Sub[2] = inputArraySub[14];
        rs1Sub[3] = inputArraySub[13];
        rs1Sub[4] = inputArraySub[12];

        rs2Sub[0] = inputArraySub[11];                  // Decodes register 2 field
        rs2Sub[1] = inputArraySub[10];
        rs2Sub[2] = inputArraySub[9];
        rs2Sub[3] = inputArraySub[8];
        rs2Sub[4] = inputArraySub[7];

        imm_BtypeUpperSub[0] = inputArraySub[6];        // Decodes immediate value
        imm_BtypeUpperSub[1] = inputArraySub[5];
        imm_BtypeUpperSub[2] = inputArraySub[4];
        imm_BtypeUpperSub[3] = inputArraySub[3];
        imm_BtypeUpperSub[4] = inputArraySub[2];
        imm_BtypeUpperSub[5] = inputArraySub[1];
        imm_BtypeUpperSub[6] = inputArraySub[0];

        int immIndex = 11;

        for (int i = 6; i > -1; i--) {                                          // Calculates immediate value in decimal (locations are split)
            immToDecimalSub += ((imm_BtypeUpperSub[i]) * pow(2, immIndex));
            immIndex--;
        }

        for (int i = 4; i > -1; i--) {                                          // Calculates immediate value in decimal (locations are split)
            immToDecimalSub += ((imm_BtypeLowerSub[i]) * pow(2, immIndex));
            immIndex--;
        }

    }
    else if (uTypeSub == true) {
        rdSub[0] = inputArraySub[24];                     // Decodes register destination
        rdSub[1] = inputArraySub[23];
        rdSub[2] = inputArraySub[22];
        rdSub[3] = inputArraySub[21];
        rdSub[4] = inputArraySub[20];

        imm_UtypeSub[0] = inputArraySub[19];              // Decodes immediate value
        imm_UtypeSub[1] = inputArraySub[18];
        imm_UtypeSub[2] = inputArraySub[17];
        imm_UtypeSub[3] = inputArraySub[16];
        imm_UtypeSub[4] = inputArraySub[15];
        imm_UtypeSub[5] = inputArraySub[14];
        imm_UtypeSub[6] = inputArraySub[13];
        imm_UtypeSub[7] = inputArraySub[12];
        imm_UtypeSub[8] = inputArraySub[11];
        imm_UtypeSub[9] = inputArraySub[10];
        imm_UtypeSub[10] = inputArraySub[9];
        imm_UtypeSub[11] = inputArraySub[8];
        imm_UtypeSub[12] = inputArraySub[7];
        imm_UtypeSub[13] = inputArraySub[6];
        imm_UtypeSub[14] = inputArraySub[5];
        imm_UtypeSub[15] = inputArraySub[4];
        imm_UtypeSub[16] = inputArraySub[3];
        imm_UtypeSub[17] = inputArraySub[2];
        imm_UtypeSub[18] = inputArraySub[1];
        imm_UtypeSub[19] = inputArraySub[0];

        for (int i = 11; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimalSub += ((imm_UtypeSub[i]) * pow(2, i));
        }

    }
    else if (jTypeSub == true) {
        rdSub[0] = inputArraySub[24];                 // Decodes register destination
        rdSub[1] = inputArraySub[23];
        rdSub[2] = inputArraySub[22];
        rdSub[3] = inputArraySub[21];
        rdSub[4] = inputArraySub[20];

        imm_JtypeSub[0] = inputArraySub[19];          // Decodes immediate value
        imm_JtypeSub[1] = inputArraySub[18];
        imm_JtypeSub[2] = inputArraySub[17];
        imm_JtypeSub[3] = inputArraySub[16];
        imm_JtypeSub[4] = inputArraySub[15];
        imm_JtypeSub[5] = inputArraySub[14];
        imm_JtypeSub[6] = inputArraySub[13];
        imm_JtypeSub[7] = inputArraySub[12];
        imm_JtypeSub[8] = inputArraySub[11];
        imm_JtypeSub[9] = inputArraySub[10];
        imm_JtypeSub[10] = inputArraySub[9];
        imm_JtypeSub[11] = inputArraySub[8];
        imm_JtypeSub[12] = inputArraySub[7];
        imm_JtypeSub[13] = inputArraySub[6];
        imm_JtypeSub[14] = inputArraySub[5];
        imm_JtypeSub[15] = inputArraySub[4];
        imm_JtypeSub[16] = inputArraySub[3];
        imm_JtypeSub[17] = inputArraySub[2];
        imm_JtypeSub[18] = inputArraySub[1];
        imm_JtypeSub[19] = inputArraySub[0];

        for (int i = 19; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimalSub += ((imm_JtypeSub[i]) * pow(2, i));
        }

    }

    for (int i = 4; i > -1; i--) {
        rdToDecimalSub += ((rdSub[i]) * pow(2, i));       // Decodes register destination (coverts to decimal)
    }

    for (int i = 4; i > -1; i--) {
        rs1ToDecimalSub += ((rs1Sub[i]) * pow(2, i));     // Decodes register 1 location (coverts to decimal)
    }

    for (int i = 4; i > -1; i--) {
        rs2ToDecimalSub += ((rs2Sub[i]) * pow(2, i));     // Decodes register 2 location (coverts to decimal)
    }

}

// Function that determines which instruction is to be performed from machine code (modified for RV32I and RV32F Base Instruction Sets) (CPU 0)
void determineOperationAdd() {

    int opcode[7] = {};
    int func3[3] = {};
    int func7[7] = {};
    int func3ToDecimal = 0;
    int func7ToDecimal = 0;
    int opcodeToDecimal = 0;
    bool validInstruction = true;

    opcode[0] = inputArrayAdd[31];             // Decodes opcode from input array
    opcode[1] = inputArrayAdd[30];
    opcode[2] = inputArrayAdd[29];
    opcode[3] = inputArrayAdd[28];
    opcode[4] = inputArrayAdd[27];
    opcode[5] = inputArrayAdd[26];
    opcode[6] = inputArrayAdd[25];

    func3[0] = inputArrayAdd[19];              // Decodes func3 from input array
    func3[1] = inputArrayAdd[18];
    func3[2] = inputArrayAdd[17];

    func7[0] = inputArrayAdd[6];               // Decodes func7 from input array
    func7[1] = inputArrayAdd[5];
    func7[2] = inputArrayAdd[4];
    func7[3] = inputArrayAdd[3];
    func7[4] = inputArrayAdd[2];
    func7[5] = inputArrayAdd[1];
    func7[6] = inputArrayAdd[0];

    for (int i = 6; i > -1; i--) {
        opcodeToDecimal += ((opcode[i]) * pow(2, i));           // Calculates opcode value in decimal
    }

    for (int i = 2; i > -1; i--) {                              // Calculates func3 value in decimal
        func3ToDecimal += ((func3[i]) * pow(2, i));
    }

    for (int i = 6; i > -1; i--) {                              // Calculates func7 value in decimal
        func7ToDecimal += ((func7[i]) * pow(2, i));
    }

    /* Jumps to each section of instruction depending on instruction */
    switch (opcodeToDecimal) {

        /* Start of Data Transfer Opcode Section */

    case 3:                                                         // Load instructions    
        switch (func3ToDecimal) {

        case 0:             // Lb instruction 
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Lb ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 1:             // Lh instruction
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Lh ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 2:             // Lw instruction 
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Lw ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;


        case 4:             // Lbu instruction
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Lbu ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 5:             // Lhu instruction
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Lhu ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 35:                                                        // Store instructions     
        switch (func3ToDecimal) {

        case 0:             // Sb instruction 
            sTypeAdd = true;
            opcodeTypeAdd = "S";
            instAdd = "Sb ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 1;
            writeBackSelAdd = 2;
            memWriteAdd = 1;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 1:             // Sh instruction
            sTypeAdd = true;
            opcodeTypeAdd = "S";
            instAdd = "Sh ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 1;
            writeBackSelAdd = 2;
            memWriteAdd = 1;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 2:             // Sw instruction 
            sTypeAdd = true;
            opcodeTypeAdd = "S";
            instAdd = "Sw ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 1;
            writeBackSelAdd = 2;
            memWriteAdd = 1;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 7:                                                         // Float load instructions    
        switch (func3ToDecimal) {

        case 2:             // Flw instruction 
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Flw ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 39:                                                        // Float store instructions   
        switch (func3ToDecimal) {

        case 2:             // Fsw instruction 
            sTypeAdd = true;
            opcodeTypeAdd = "S";
            instAdd = "Fsw ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 1;
            writeBackSelAdd = 2;
            memWriteAdd = 1;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 83:                                                        // Float/integer copy to/from instructions 
        switch (func3ToDecimal) {

        case 0:             // FMV._.X and FMV.X._ instructions 
            switch (func7ToDecimal) {

                case 120:                       // FMV.W.X instruction
                    rTypeAdd = true;
                    opcodeTypeAdd = "R";
                    instAdd = "FMV.W.X ";
                    aluOpAdd = 1;
                    branchAdd = 0;
                    memReadAdd = 0;
                    writeBackSelAdd = 0;
                    memWriteAdd = 0;
                    aluSrcAdd = 0;
                    regWriteAdd = 1;
                    signExtAdd = 1;
                    pcSelAdd = 0;
                    branchConditionAdd = 0;
                    branchSelAdd = 0;
                    break;

                case 0:                       // FADD.S instruction
                    rTypeAdd = true;
                    opcodeTypeAdd = "R";
                    instAdd = "FADD.S ";
                    aluOpAdd = 1;
                    branchAdd = 0;
                    memReadAdd = 0;
                    writeBackSelAdd = 0;
                    memWriteAdd = 0;
                    aluSrcAdd = 1;
                    regWriteAdd = 1;
                    signExtAdd = 1;
                    pcSelAdd = 0;
                    branchConditionAdd = 0;
                    branchSelAdd = 0;
                    break;

                case 4:                       // FSUB.S instruction
                    rTypeAdd = true;
                    opcodeTypeAdd = "R";
                    instAdd = "FSUB.S ";
                    aluOpAdd = 1;
                    branchAdd = 0;
                    memReadAdd = 0;
                    writeBackSelAdd = 0;
                    memWriteAdd = 0;
                    aluSrcAdd = 1;
                    regWriteAdd = 1;
                    signExtAdd = 1;
                    pcSelAdd = 0;
                    branchConditionAdd = 0;
                    branchSelAdd = 0;
                    break;


                default:
                    cout << "Error has occured in func7" << endl;
                    break;
            }

            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

        /* Start of Arithmetic and Logical Opcode Section */

    case 51:                                                        // Arithmetic and Logical Instructions   
        switch (func3ToDecimal) {

        case 0:
            switch (func7ToDecimal) {

            case 0:                     // Add instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Add ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            case 32:                    // Sub instruction
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Sub ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 1:
            switch (func7ToDecimal) {

            case 0:                     // Sll instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Sll ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 2:
            switch (func7ToDecimal) {

            case 0:                     // Slt instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Slt ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 3:
            switch (func7ToDecimal) {

            case 0:                     // Sltu instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Sltu ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 4:
            switch (func7ToDecimal) {

            case 0:                     // Xor instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Xor ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 5:
            switch (func7ToDecimal) {

            case 0:                     // Srl instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Srl ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            case 32:                    // Sra instruction
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Sra ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 6:
            switch (func7ToDecimal) {

            case 0:                     // Or instruction
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Or ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 7:
            switch (func7ToDecimal) {

                case 0:                     // And instruction 
                    rTypeAdd = true;
                    opcodeTypeAdd = "R";
                    instAdd = "And ";
                    aluOpAdd = 1;
                    branchAdd = 0;
                    memReadAdd = 0;
                    writeBackSelAdd = 0;
                    memWriteAdd = 0;
                    aluSrcAdd = 0;
                    regWriteAdd = 1;
                    signExtAdd = 0;
                    pcSelAdd = 0;
                    branchConditionAdd = 0;
                    branchSelAdd = 0;
                    break;

                default:
                    cout << "Error has occured in func7" << endl;
                    break;
            }

            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 19:                                                         // Immediate Instructions     
        switch (func3ToDecimal) {
        case 0:                             // Addi instruction
            iTypeAdd = true;
            iTypeCateAdd = 1;
            opcodeTypeAdd = "I";
            instAdd = "Addi ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 1:
            switch (func7ToDecimal) {

            case 0:                     // Slli instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Slli ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 1;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 2:                             // Slti instruction
            iTypeAdd = true;
            iTypeCateAdd = 1;
            opcodeTypeAdd = "I";
            instAdd = "Slti ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 3:                             // Sltiu instruction
            iTypeAdd = true;
            iTypeCateAdd = 1;
            opcodeTypeAdd = "I";
            instAdd = "Sltiu ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 4:                             // Xori instruction
            iTypeAdd = true;
            iTypeCateAdd = 1;
            opcodeTypeAdd = "I";
            instAdd = "Xori ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 5:
            switch (func7ToDecimal) {

            case 0:                         // Srli instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Srli ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 1;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            case 32:                        // Srai instruction
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Srai ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 1;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 6:                             // Ori instruction
            iTypeAdd = true;
            iTypeCateAdd = 1;
            opcodeTypeAdd = "I";
            instAdd = "Ori ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 7:                             // Andi instruction
            iTypeAdd = true;
            iTypeCateAdd = 1;
            opcodeTypeAdd = "I";
            instAdd = "Andi ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 55:                                                    // Lui instruction  
        uTypeAdd = true;
        opcodeTypeAdd = "U";
        instAdd = "Lui ";
        aluOpAdd = 1;
        branchAdd = 0;
        memReadAdd = 0;
        writeBackSelAdd = 0;
        memWriteAdd = 0;
        aluSrcAdd = 1;
        regWriteAdd = 1;
        signExtAdd = 1;
        pcSelAdd = 0;
        branchConditionAdd = 0;
        branchSelAdd = 0;
        break;

    case 23:                                                    // Auipc instruction
        uTypeAdd = true;
        opcodeTypeAdd = "U";
        instAdd = "Auipc ";
        aluOpAdd = 1;
        branchAdd = 0;
        memReadAdd = 0;
        writeBackSelAdd = 1;
        memWriteAdd = 0;
        aluSrcAdd = 1;
        regWriteAdd = 1;
        signExtAdd = 0;
        pcSelAdd = 1;
        branchConditionAdd = 0;
        branchSelAdd = 0;
        break;

        /* Start of Control Opcode Section */

    case 99:                                                    // branchAdd instructions    
        switch (func3ToDecimal) {
        case 0:                            // Beq instruction
            bTypeAdd = true;
            opcodeTypeAdd = "B";
            instAdd = "Beq ";
            aluOpAdd = 1;
            branchAdd = 1;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 0;
            signExtAdd = 0;
            pcSelAdd = 2;
            branchConditionAdd = 1;
            branchSelAdd = 1;
            break;

        case 1:                            // Bne instruction
            bTypeAdd = true;
            opcodeTypeAdd = "B";
            instAdd = "Bne ";
            aluOpAdd = 1;
            branchAdd = 1;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 0;
            signExtAdd = 0;
            pcSelAdd = 2;
            branchConditionAdd = 1;
            branchSelAdd = 1;
            break;

        case 4:                            // Blt instruction
            bTypeAdd = true;
            opcodeTypeAdd = "B";
            instAdd = "Blt ";
            aluOpAdd = 1;
            branchAdd = 1;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 0;
            signExtAdd = 0;
            pcSelAdd = 2;
            branchConditionAdd = 1;
            branchSelAdd = 1;
            break;

        case 5:                            // Bge instruction
            bTypeAdd = true;
            opcodeTypeAdd = "B";
            instAdd = "Bge ";
            aluOpAdd = 1;
            branchAdd = 1;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 0;
            signExtAdd = 0;
            pcSelAdd = 2;
            branchConditionAdd = 1;
            branchSelAdd = 1;
            break;

        case 6:                            // Bltu instruction
            bTypeAdd = true;
            opcodeTypeAdd = "B";
            instAdd = "Bltu ";
            aluOpAdd = 1;
            branchAdd = 1;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 0;
            signExtAdd = 0;
            pcSelAdd = 2;
            branchConditionAdd = 1;
            branchSelAdd = 1;
            break;

        case 7:                            // Bgeu instruction
            bTypeAdd = true;
            opcodeTypeAdd = "B";
            instAdd = "Bgeu ";
            aluOpAdd = 1;
            branchAdd = 1;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 0;
            signExtAdd = 0;
            pcSelAdd = 2;
            branchConditionAdd = 1;
            branchSelAdd = 1;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }
        break;

    case 111:                                                   // Jal instruction 
        jTypeAdd = true;
        opcodeTypeAdd = "J";
        instAdd = "Jal ";
        aluOpAdd = 1;
        branchAdd = 0;
        memReadAdd = 0;
        writeBackSelAdd = 1;
        memWriteAdd = 0;
        aluSrcAdd = 1;
        regWriteAdd = 1;
        signExtAdd = 0;
        pcSelAdd = 2;
        branchConditionAdd = 0;
        branchSelAdd = 0;
        break;

    case 103:
        switch (func3ToDecimal) {
        case 0:                                             // Jalr instruction
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Jalr ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 1;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 1;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }
        break;

        /* Default Case */

    default:
        cout << "Invalid Instruction" << endl;
        validInstruction = false;
        break;
    }

    if (validInstruction == true) {     // If instruction is valid, process the rest

        assignRegisterFieldsAdd();     // Call function to decode the remaining parts of machine code input
        printEventDecodeAdd();               // Prints the output of instruction and corresponding control signals

    }

    rTypeAdd = false;              // Resets all instruction types 
    iTypeAdd = false;
    sTypeAdd = false;
    bTypeAdd = false;
    uTypeAdd = false;
    jTypeAdd = false;

    iTypeCateAdd = 0;              // Resets all values 
    rdToDecimalAdd = 0;
    rs1ToDecimalAdd = 0;
    rs2ToDecimalAdd = 0;
    immToDecimalAdd = 0;

    validInstruction = true;
}

// Function that determines which instruction is to be performed from machine code (modified for RV32I and RV32F Base Instruction Sets) (CPU 1)
void determineOperationSub() {

    int opcode[7] = {};
    int func3[3] = {};
    int func7[7] = {};
    int func3ToDecimal = 0;
    int func7ToDecimal = 0;
    int opcodeToDecimal = 0;
    bool validInstruction = true;

    opcode[0] = inputArraySub[31];             // Decodes opcode from input array
    opcode[1] = inputArraySub[30];
    opcode[2] = inputArraySub[29];
    opcode[3] = inputArraySub[28];
    opcode[4] = inputArraySub[27];
    opcode[5] = inputArraySub[26];
    opcode[6] = inputArraySub[25];

    func3[0] = inputArraySub[19];              // Decodes func3 from input array
    func3[1] = inputArraySub[18];
    func3[2] = inputArraySub[17];

    func7[0] = inputArraySub[6];               // Decodes func7 from input array
    func7[1] = inputArraySub[5];
    func7[2] = inputArraySub[4];
    func7[3] = inputArraySub[3];
    func7[4] = inputArraySub[2];
    func7[5] = inputArraySub[1];
    func7[6] = inputArraySub[0];

    for (int i = 6; i > -1; i--) {
        opcodeToDecimal += ((opcode[i]) * pow(2, i));           // Calculates opcode value in decimal
    }

    for (int i = 2; i > -1; i--) {                              // Calculates func3 value in decimal
        func3ToDecimal += ((func3[i]) * pow(2, i));
    }

    for (int i = 6; i > -1; i--) {                              // Calculates func7 value in decimal
        func7ToDecimal += ((func7[i]) * pow(2, i));
    }

    /* Jumps to each section of instruction depending on instruction */
    switch (opcodeToDecimal) {

        /* Start of Data Transfer Opcode Section */

    case 3:                                                         // Load instructions    
        switch (func3ToDecimal) {

        case 0:             // Lb instruction 
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Lb ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 1:             // Lh instruction
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Lh ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 2:             // Lw instruction 
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Lw ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;


        case 4:             // Lbu instruction
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Lbu ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 5:             // Lhu instruction
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Lhu ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 35:                                                        // Store instructions     
        switch (func3ToDecimal) {

        case 0:             // Sb instruction 
            sTypeSub = true;
            opcodeTypeAdd = "S";
            instSub = "Sb ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 1;
            writeBackSelSub = 2;
            memWriteSub = 1;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 1:             // Sh instruction
            sTypeSub = true;
            opcodeTypeAdd = "S";
            instSub = "Sh ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 1;
            writeBackSelSub = 2;
            memWriteSub = 1;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 2:             // Sw instruction 
            sTypeSub = true;
            opcodeTypeAdd = "S";
            instSub = "Sw ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 1;
            writeBackSelSub = 2;
            memWriteSub = 1;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 7:                                                         // Float load instructions    
        switch (func3ToDecimal) {

        case 2:             // Flw instruction 
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Flw ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 39:                                                        // Float store instructions   
        switch (func3ToDecimal) {

        case 2:             // Fsw instruction 
            sTypeSub = true;
            opcodeTypeAdd = "S";
            instSub = "Fsw ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 1;
            writeBackSelSub = 2;
            memWriteSub = 1;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 83:                                                        // Float/integer copy to/from instructions 
        switch (func3ToDecimal) {

        case 0:             // FMV._.X and FMV.X._ instructions 
            switch (func7ToDecimal) {

            case 120:                       // FMV.W.X instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "FMV.W.X ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 1;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            case 0:                       // FADD.S instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "FADD.S ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 1;
                regWriteSub = 1;
                signExtSub = 1;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            case 4:                       // FSUB.S instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "FSUB.S ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 1;
                regWriteSub = 1;
                signExtSub = 1;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;


            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

        /* Start of Arithmetic and Logical Opcode Section */

    case 51:                                                        // Arithmetic and Logical Instructions   
        switch (func3ToDecimal) {

        case 0:
            switch (func7ToDecimal) {

            case 0:                     // Add instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Add ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            case 32:                    // Sub instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Sub ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 1:
            switch (func7ToDecimal) {

            case 0:                     // Sll instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Sll ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 2:
            switch (func7ToDecimal) {

            case 0:                     // Slt instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Slt ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 3:
            switch (func7ToDecimal) {

            case 0:                     // Sltu instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Sltu ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 4:
            switch (func7ToDecimal) {

            case 0:                     // Xor instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Xor ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 5:
            switch (func7ToDecimal) {

            case 0:                     // Srl instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Srl ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            case 32:                    // Sra instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Sra ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 6:
            switch (func7ToDecimal) {

            case 0:                     // Or instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Or ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 7:
            switch (func7ToDecimal) {

            case 0:                     // And instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "And ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 19:                                                         // Immediate Instructions     
        switch (func3ToDecimal) {
        case 0:                             // Addi instruction
            iTypeSub = true;
            iTypeCateSub = 1;
            opcodeTypeAdd = "I";
            instSub = "Addi ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 1:
            switch (func7ToDecimal) {

            case 0:                     // Slli instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Slli ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 1;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 2:                             // Slti instruction
            iTypeSub = true;
            iTypeCateSub = 1;
            opcodeTypeAdd = "I";
            instSub = "Slti ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 3:                             // Sltiu instruction
            iTypeSub = true;
            iTypeCateSub = 1;
            opcodeTypeAdd = "I";
            instSub = "Sltiu ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 4:                             // Xori instruction
            iTypeSub = true;
            iTypeCateSub = 1;
            opcodeTypeAdd = "I";
            instSub = "Xori ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 5:
            switch (func7ToDecimal) {

            case 0:                         // Srli instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Srli ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 1;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            case 32:                        // Srai instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Srai ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 1;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 6:                             // Ori instruction
            iTypeSub = true;
            iTypeCateSub = 1;
            opcodeTypeAdd = "I";
            instSub = "Ori ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 7:                             // Andi instruction
            iTypeSub = true;
            iTypeCateSub = 1;
            opcodeTypeAdd = "I";
            instSub = "Andi ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 55:                                                    // Lui instruction  
        uTypeSub = true;
        opcodeTypeAdd = "U";
        instSub = "Lui ";
        aluOpSub = 1;
        branchSub = 0;
        memReadSub = 0;
        writeBackSelSub = 0;
        memWriteSub = 0;
        aluSrcSub = 1;
        regWriteSub = 1;
        signExtSub = 1;
        pcSelSub = 0;
        branchConditionSub = 0;
        branchSelSub = 0;
        break;

    case 23:                                                    // Auipc instruction
        uTypeSub = true;
        opcodeTypeAdd = "U";
        instSub = "Auipc ";
        aluOpSub = 1;
        branchSub = 0;
        memReadSub = 0;
        writeBackSelSub = 1;
        memWriteSub = 0;
        aluSrcSub = 1;
        regWriteSub = 1;
        signExtSub = 0;
        pcSelSub = 1;
        branchConditionSub = 0;
        branchSelSub = 0;
        break;

        /* Start of Control Opcode Section */

    case 99:                                                    // branchSub instructions    
        switch (func3ToDecimal) {
        case 0:                            // Beq instruction
            bTypeSub = true;
            opcodeTypeAdd = "B";
            instSub = "Beq ";
            aluOpSub = 1;
            branchSub = 1;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 0;
            signExtSub = 0;
            pcSelSub = 2;
            branchConditionSub = 1;
            branchSelSub = 1;
            break;

        case 1:                            // Bne instruction
            bTypeSub = true;
            opcodeTypeAdd = "B";
            instSub = "Bne ";
            aluOpSub = 1;
            branchSub = 1;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 0;
            signExtSub = 0;
            pcSelSub = 2;
            branchConditionSub = 1;
            branchSelSub = 1;
            break;

        case 4:                            // Blt instruction
            bTypeSub = true;
            opcodeTypeAdd = "B";
            instSub = "Blt ";
            aluOpSub = 1;
            branchSub = 1;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 0;
            signExtSub = 0;
            pcSelSub = 2;
            branchConditionSub = 1;
            branchSelSub = 1;
            break;

        case 5:                            // Bge instruction
            bTypeSub = true;
            opcodeTypeAdd = "B";
            instSub = "Bge ";
            aluOpSub = 1;
            branchSub = 1;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 0;
            signExtSub = 0;
            pcSelSub = 2;
            branchConditionSub = 1;
            branchSelSub = 1;
            break;

        case 6:                            // Bltu instruction
            bTypeSub = true;
            opcodeTypeAdd = "B";
            instSub = "Bltu ";
            aluOpSub = 1;
            branchSub = 1;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 0;
            signExtSub = 0;
            pcSelSub = 2;
            branchConditionSub = 1;
            branchSelSub = 1;
            break;

        case 7:                            // Bgeu instruction
            bTypeSub = true;
            opcodeTypeAdd = "B";
            instSub = "Bgeu ";
            aluOpSub = 1;
            branchSub = 1;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 0;
            signExtSub = 0;
            pcSelSub = 2;
            branchConditionSub = 1;
            branchSelSub = 1;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }
        break;

    case 111:                                                   // Jal instruction 
        jTypeSub = true;
        opcodeTypeAdd = "J";
        instSub = "Jal ";
        aluOpSub = 1;
        branchSub = 0;
        memReadSub = 0;
        writeBackSelSub = 1;
        memWriteSub = 0;
        aluSrcSub = 1;
        regWriteSub = 1;
        signExtSub = 0;
        pcSelSub = 2;
        branchConditionSub = 0;
        branchSelSub = 0;
        break;

    case 103:
        switch (func3ToDecimal) {
        case 0:                                             // Jalr instruction
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Jalr ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 1;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 1;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }
        break;

        /* Default Case */

    default:
        cout << "Invalid Instruction" << endl;
        validInstruction = false;
        break;
    }

    if (validInstruction == true) {     // If instruction is valid, process the rest

        assignRegisterFieldsSub();     // Call function to decode the remaining parts of machine code input
        printEventDecodeSub();               // Prints the output of instruction and corresponding control signals

    }

    rTypeSub = false;              // Resets all instruction types 
    iTypeSub = false;
    sTypeSub = false;
    bTypeSub = false;
    uTypeSub = false;
    jTypeSub = false;

    iTypeCateSub = 0;              // Resets all values 
    rdToDecimalSub = 0;
    rs1ToDecimalSub = 0;
    rs2ToDecimalSub = 0;
    immToDecimalSub = 0;

    validInstruction = true;
}

// Function that creates each instruction and writes in into an instruction file to be decoded (CPU 0)
void create32bitInstructionAdd(int instructionInLoop) {
    int addOrSub = 0;

    switch (instructionInLoop) {
        case 0:

            if (fmod(A.mainArray[arrayIndexAdd], 1) == 0) {          // Load integer instruction Array A

                decToBinaryNormal(offsetAdd, addOrSub);                                    // Outputs the immediate value offset
                myFileAdd << "00001" << "000" << "01010" << "0000011\n";

                floatPresent = 0;
            }
            else {                                  // Load float instruction Array A

                decToBinaryNormal(offsetAdd, addOrSub);                                    // Outputs the immediate value offset
                myFileAdd << "00001" << "010" << "01010" << "0000111\n";

                floatPresent = 1;
            }

            break; 

        case 1:

            if (fmod(B.mainArray[arrayIndexAdd], 1) == 0) {          // Load integer instruction Array B

                decToBinaryNormal(offsetAdd, addOrSub);                                    // Outputs the immediate value offset
                myFileAdd << "00010" << "000" << "01011" << "0000011\n";
                floatPresent = 0;
            }
            else {                                  // Load float instruction Array B

                decToBinaryNormal(offsetAdd, addOrSub);                                    // Outputs the immediate value offset
                myFileAdd << "00010" << "010" << "01011" << "0000111\n";
                floatPresent = 1;
            }


            break; 

        case 2:

            if (floatPresent == 0) {                // Add instruction 

                myFileAdd << "0000000" << "01011" << "01010" << "000" << "01101" << "0110011\n";

            }
            else {                                   // Float Add instruction

                myFileAdd << "0000000" << "01011" << "01010" << "000" << "01101" << "1010011\n";

            }

            break;

        case 3: 

            if (floatPresent == 0) {                // Store instruction 

                decToBinarySplit(offsetAdd, floatPresent, addOrSub);
                myFileAdd << "0100011\n";
            }
            else {                                   // Float Store instruction

                decToBinarySplit(offsetAdd, floatPresent, addOrSub);
                myFileAdd << "0100111\n";
            }

            break;

        case 4:

            myFileAdd << "000000000100" << "00001" << "000" << "00001" << "0010011\n";         // Addi instruction Array A
            break;

        case 5:

            myFileAdd << "000000000100" << "00010" << "000" << "00010" << "0010011\n";         // Addi instruction Array B
            break;

        case 6:

            myFileAdd << "000000000100" << "00011" << "000" << "00011" << "0010011\n";         // Addi instruction Array C
            break;

        case 7:

            floatPresent = 2;
            decToBinarySplit(offsetAdd, floatPresent, addOrSub);                     // BNE instruction 
            myFileAdd << "1100011\n";

            offsetAdd += 4;
            arrayIndexAdd += 1;
            break;

        default: 
            break;

    }

 
}

// Function that creates each instruction and writes in into an instruction file to be decoded (CPU 1)
void create32bitInstructionSub(int instructionInLoop) {

    int addOrSub = 1;

    switch (instructionInLoop) {
        case 0:

            if (fmod(A.mainArray[arrayIndexSub], 1) == 0) {          // Load integer instruction Array A

                decToBinaryNormal(offsetSub, addOrSub);                                    // Outputs the immediate value offset
                myFileSub << "00001" << "000" << "01111" << "0000011\n";

                floatPresent = 0;
            }
            else {                                  // Load float instruction Array A

                decToBinaryNormal(offsetSub, addOrSub);                                    // Outputs the immediate value offset
                myFileSub << "00001" << "010" << "01111" << "0000111\n";

                floatPresent = 1;
            }

            break;

        case 1:

            if (fmod(B.mainArray[arrayIndexSub], 1) == 0) {          // Load integer instruction Array B

                decToBinaryNormal(offsetSub, addOrSub);                                    // Outputs the immediate value offset
                myFileSub << "00010" << "000" << "10000" << "0000011\n";
                floatPresent = 0;
            }
            else {                                  // Load float instruction Array B

                decToBinaryNormal(offsetSub, addOrSub);                                    // Outputs the immediate value offset
                myFileSub << "00010" << "010" << "10000" << "0000111\n";
                floatPresent = 1;
            }


            break;

        case 2:

            if (floatPresent == 0) {                // Sub instruction 

                myFileSub << "0100000" << "10000" << "01111" << "000" << "01110" << "0110011\n";

            }
            else {                                   // Float Sub instruction

                myFileSub << "0000100" << "10000" << "01111" << "000" << "01110" << "1010011\n";

            }

            break;

        case 3:

            if (floatPresent == 0) {                // Store instruction 

                decToBinarySplit(offsetSub, floatPresent, addOrSub);
                myFileSub << "0100011\n";
            }
            else {                                   // Float Store instruction

                decToBinarySplit(offsetSub, floatPresent, addOrSub);
                myFileSub << "0100111\n";
            }

            break;

        case 4:

            myFileSub << "000000000100" << "00001" << "000" << "00001" << "0010011\n";         // Addi instruction Array A
            break;

        case 5:

            myFileSub << "000000000100" << "00010" << "000" << "00010" << "0010011\n";         // Addi instruction Array B
            break;

        case 6:

            myFileSub << "000000000100" << "00011" << "000" << "00011" << "0010011\n";         // Addi instruction Array C
            break;

        case 7:

            floatPresent = 2;
            decToBinarySplit(offsetSub, floatPresent, addOrSub);                     // BNE instruction 
            myFileSub << "1100011\n";

            offsetSub += 4;
            arrayIndexSub += 1;
            break;

        default:
            break;

    }

}

// Function that convert decimal to binary for normal immediate values in instruction
void decToBinaryNormal(int n, int addOrSub)
{
    if (addOrSub == 0) {        // Add case

        // Size of an integer is assumed to be 32 bits
        for (int i = 11; i >= 0; i--) {
            int k = n >> i;
            if (k & 1)
                myFileAdd << "1";
            else
                myFileAdd << "0";
        }
    }
    else {          // Sub case

        // Size of an integer is assumed to be 32 bits
        for (int i = 11; i >= 0; i--) {
            int k = n >> i;
            if (k & 1)
                myFileSub << "1";
            else
                myFileSub << "0";
        }
    }

}

// Function that converts decimal to binary for split immediate values in instruction
void decToBinarySplit(int n, int floatOrInt, int addOrSub) {

    if (addOrSub == 0) {            // Add case                   

        // Size of an integer is assumed to be 32 bits
        for (int i = 11; i >= 5; i--) {
            int k = n >> i;
            if (k & 1)
                myFileAdd << "1";
            else
                myFileAdd << "0";
        }

        if (floatOrInt == 0) {      // Integer instruction 
            myFileAdd << "01101" << "00011" << "000";
        }
        else if (floatOrInt == 1) {                      // Float instruction 
            myFileAdd << "01101" << "00011" << "010";
        }
        else {
            myFileAdd << "01000" << "00111" << "001";
        }

        for (int i = 4; i >= 0; i--) {
            int k = n >> i;
            if (k & 1)
                myFileAdd << "1";
            else
                myFileAdd << "0";
        }

    }
    else {      // Sub case

        // Size of an integer is assumed to be 32 bits
        for (int i = 11; i >= 5; i--) {
            int k = n >> i;
            if (k & 1)
                myFileSub << "1";
            else
                myFileSub << "0";
        }

        if (floatOrInt == 0) {      // Integer instruction 
            myFileSub << "01110" << "00100" << "000";
        }
        else if (floatOrInt == 1) {                      // Float instruction 
            myFileSub << "01110" << "00100" << "010";
        }
        else {
            myFileSub << "01000" << "00111" << "001";
        }

        for (int i = 4; i >= 0; i--) {
            int k = n >> i;
            if (k & 1)
                myFileSub << "1";
            else
                myFileSub << "0";
        }
    }



}

// Function that simulates the fetch stage of the pipeline
void fetchStage(int loadPrevStage) {

    if (loadPrevStage == 1) {                                                                   // In case of normal operation
        myFileAdd.open("ECGR 5181 Assignment 4 Instruction List Add.txt", ios::out | ios::trunc);
    if (!myFileAdd.is_open()) {
    std::cerr << "Error opening 'ECGR 5181 Assignment 4 Instruction List Add.txt'" << std::endl;
}

        myFileSub.open("ECGR 5181 Assignment 4 Instruction List Sub.txt", ios::out | ios::trunc);
    if (!myFileSub.is_open()) {
         std::cerr << "Error opening 'ECGR 5181 Assignment 4 Instruction List Sub.txt'" << std::endl;
}     // Opens file for instructions in write mode
fstream faddFile, fsubFile;

    // Open files for reading
    faddFile.open("fadd.s", ios::in);
    fsubFile.open("fsub.s", ios::in);


        if (!myFileAdd.is_open() || !myFileSub.is_open()) {

            cout << "Error opening file" << endl;

        }
        else {



            create32bitInstructionAdd(count32InstructionAdd);       // Creates the instructions and puts them in file
            count32InstructionAdd++;

            create32bitInstructionSub(count32InstructionSub);       // Creates the instructions and puts them in file
            count32InstructionSub++;


            if (count32InstructionAdd > 7) {           // Resets loop of instructions
                count32InstructionAdd = 0;
            }

            if (count32InstructionSub > 7) {           // Resets loop of instructions
                count32InstructionSub = 0;
            }

            myFileAdd.close();
            myFileSub.close();

            myFileAdd.open("ECGR 5181 Assignment 4 Instruction List Add.txt", ios::in);
            myFileSub.open("ECGR 5181 Assignment 4 Instruction List Sub.txt", ios::in);

            getline(myFileAdd, lineFetchAdd);

            cout << endl << "--------------------------------------------------------------" << endl;       // Output instruction fetched
            cout << endl << "CPU 0 Fetch Stage:" << endl;
            cout << "Instruction: " << lineFetchAdd << endl;
            cout << "Clock Cycle: " << clockcycle << endl;

            lineDecodeAdd = lineFetchAdd;

            getline(myFileSub, lineFetchSub);

            cout << endl << "--------------------------------------------------------------" << endl;       // Output instruction fetched
            cout << endl << "CPU 1 Fetch Stage:" << endl;
            cout << "Instruction: " << lineFetchSub << endl;
            cout << "Clock Cycle: " << clockcycle << endl;

        }

        lineDecodeSub = lineFetchSub;

        myFileAdd.close();
        myFileSub.close();
    }
    else if (loadPrevStage == 2) {                                                                          // In case of stall

        cout << endl << "--------------------------------------------------------------" << endl;           // Reprint old data (don't update)
        cout << endl << "CPU 0 Fetch Stage:" << endl;
        cout << "Instruction: " << lineFetchAdd << endl;
        cout << "Clock Cycle: " << clockcycle << endl;

        cout << endl << "--------------------------------------------------------------" << endl;           // Reprint old data (don't update)
        cout << endl << "CPU 1 Fetch Stage:" << endl;
        cout << "Instruction: " << lineFetchSub << endl;
        cout << "Clock Cycle: " << clockcycle << endl;

    }
    else {
        cout << endl << "CPU 0 Fetch Stage:" << endl;                                                             // In case of no operation
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycle << endl;

        cout << endl << "CPU 1 Fetch Stage:" << endl;                                                             // In case of no operation
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycle << endl;
    }
}

// Function that simulates the decode stage of the pipeline
void decodeStage(int loadPrevStage) {

    if (loadPrevStage == 1) {                                                       // In case of normal operation
        myFileAdd.open("ECGR 5181 Assignment 4 Instruction List Add.txt", ios::in);       // Opens file for instructions in read mode
        myFileSub.open("ECGR 5181 Assignment 4 Instruction List Sub.txt", ios::in);

        while (getline(myFileAdd, lineDecodeAdd)) {     // CPU 0

            for (int i = 0; i < 32; i++) {
                tempArray[i] = (int)lineDecodeAdd[i];

                if (tempArray[i] == 48) {               // If '0' in ACSII set 0 integer
                    inputArrayAdd[i] = 0;
                }
                else if (tempArray[i] == 49) {        // If '1' in ACSII set 1 integer
                    inputArrayAdd[i] = 1;
                }
            }
            cout << endl << "--------------------------------------------------------------" << endl;
            cout << endl << "CPU 0 Decode Stage:" << endl;
            cout << "Clock Cycle: " << clockcycle << endl << endl;
            determineOperationAdd();           // Decodes and determines the instruction
        }

        while (getline(myFileSub, lineDecodeSub)) {     // CPU 1

            for (int i = 0; i < 32; i++) {
                tempArray[i] = (int)lineDecodeSub[i];

                if (tempArray[i] == 48) {               // If '0' in ACSII set 0 integer
                    inputArraySub[i] = 0;
                }
                else if (tempArray[i] == 49) {        // If '1' in ACSII set 1 integer
                    inputArraySub[i] = 1;
                }
            }
            cout << endl << "--------------------------------------------------------------" << endl;
            cout << endl << "CPU 1 Decode Stage:" << endl;
            cout << "Clock Cycle: " << clockcycle << endl << endl;
            determineOperationSub();           // Decodes and determines the instruction
        }

        myFileAdd.close();
        myFileSub.close();
    }
    else if (loadPrevStage == 2) {                                                                  // In case of stall

        cout << endl << "--------------------------------------------------------------" << endl;       // Reprint old data (don't update)
        cout << endl << "CPU 0 Decode Stage:" << endl;
        cout << "Clock Cycle: " << clockcycle << endl << endl;
        determineOperationAdd();           // Decodes and determines the instruction

        cout << endl << "--------------------------------------------------------------" << endl;       // Reprint old data (don't update)
        cout << endl << "CPU 1 Decode Stage:" << endl;
        cout << "Clock Cycle: " << clockcycle << endl << endl;
        determineOperationSub();            // Decodes and determines the instruction

    }           
    else {                                                                                          // In case of no operation
        cout << endl << "--------------------------------------------------------------" << endl;
        cout << endl << "CPU 0 Decode Stage:" << endl;
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycle << endl;

        cout << endl << "--------------------------------------------------------------" << endl;
        cout << endl << "CPU 1 Decode Stage:" << endl;
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycle << endl;
    }
}

// Function that simulates the execute stage of the pipeline
void executeStage(int loadPrevStage) {

    if (loadPrevStage == 1) {                                                                           // In case of normal operation
        printEventExecuteAdd();                                                                            // Print instruction to be executed
        printEventExecuteSub();

        if (instExecuteAdd == "Add " || instExecuteAdd == "FADD.S ") {                                        // Perform operation
            C.mainArray[arrayIndexAdd] = A.mainArray[arrayIndexAdd] + B.mainArray[arrayIndexAdd];
        }

        if (instExecuteSub == "Sub " || instExecuteSub == "FSUB.S ") {                                        // Perform operation
            D.mainArray[arrayIndexAdd] = A.mainArray[arrayIndexSub] - B.mainArray[arrayIndexSub];
        }

    }
    else if (loadPrevStage == 2) {                                                                      // In case of stall

        cout << endl << "--------------------------------------------------------------" << endl;       // Output stall
        cout << endl << "CPU 0 Execute Stage: " << endl;

        cout << "Instruction: Stall" << endl;
        cout << "Clock Cycle: " << clockcycle << endl;

        cout << endl << "--------------------------------------------------------------" << endl;       // Output stall
        cout << endl << "CPU 1 Execute Stage: " << endl;

        cout << "Instruction: Stall" << endl;
        cout << "Clock Cycle: " << clockcycle << endl;

        instructionAddCount++;
        instructionSubCount++;

    }
    else {
        cout << endl << "--------------------------------------------------------------" << endl;      // In case of no operation
        cout << endl << "CPU 0 Execute Stage:" << endl;
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycle << endl;

        cout << endl << "--------------------------------------------------------------" << endl;      // In case of no operation
        cout << endl << "CPU 1 Execute Stage:" << endl;
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycle << endl;
    }
}

// Function that simulates the writeback stage of the pipeline
void writebackStage(int loadPrevStage) {

    if (loadPrevStage == 1) {                                                                                               // In case of normal operation 

        if (instExecuteAdd == "Addi " && count32InstructionAdd == 6) {                                                               // If store executed, output value and at its saved address

           
            cout << endl << "--------------------------------------------------------------" << endl;
            cout << endl << "CPU 0 Writeback Stage: " << endl;
            printf("Value: %f is saved at memory address 0x%p\n", C.mainArray[arrayIndexAdd], C.mainArrayAddress[arrayIndexAdd]);
            cout << "Clock Cycle: " << clockcycle << endl;

        } 
        else {    
                                                                                                              // Else no operation
        cout << endl << "--------------------------------------------------------------" << endl;
        cout << endl << "CPU 0 Writeback Stage:" << endl;
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycle << endl;
        }

        if (instExecuteSub == "Addi " && count32InstructionSub == 6) {                                                               // If store executed, output value and at its saved address

            cout << endl << "--------------------------------------------------------------" << endl;
            cout << endl << "CPU 1 Writeback Stage: " << endl;
            printf("Value: %f is saved at memory address 0x%p\n", D.mainArray[arrayIndexSub], D.mainArrayAddress[arrayIndexSub]);
            cout << "Clock Cycle: " << clockcycle << endl;

        }
        else {                                                                                                                  // Else no operation
            cout << endl << "--------------------------------------------------------------" << endl;
            cout << endl << "CPU 1 Writeback Stage:" << endl;
            cout << "Instruction: No Operation" << endl;
            cout << "Clock Cycle: " << clockcycle << endl;
        }



    }
    else if (loadPrevStage == 2) {                                                                                              // In case of stall

        cout << endl << "--------------------------------------------------------------" << endl;
        cout << endl << "CPU 0 Writeback Stage: " << endl;
        cout << "Clock Cycle: " << clockcycle << endl;

        cout << endl << "--------------------------------------------------------------" << endl;
        cout << endl << "CPU 1 Writeback Stage: " << endl;
        cout << "Clock Cycle: " << clockcycle << endl;
    }
    else {                                                                                                                      // In case of no operation
        cout << endl << "--------------------------------------------------------------" << endl;
        cout << endl << "CPU 0 Writeback Stage:" << endl;
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycle << endl;

        cout << endl << "--------------------------------------------------------------" << endl;
        cout << endl << "CPU 1 Writeback Stage:" << endl;
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycle << endl;
    }
}
