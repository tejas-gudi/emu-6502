#include <STDIO.H>
#include <STDLIB.H>

using BYTE = unsigned char;
using WORD = unsigned short;
using u32 = unsigned int;


struct Mem{
    static constexpr u32 MAX_MEM = 1024 * 64;
    BYTE Data[MAX_MEM];
    void Initialize(){
        for( u32 i = 0; i < MAX_MEM; i++ ){
            Data[i] = 0;
        }
    }
    /*Read 1 Byte*/
    BYTE operator[]( u32 Address ) const {
        //assert here Address is < MAX_MEM
        return Data[Address];
    }
};

struct CPU {

    
    WORD PC;            // Program Counter
    WORD SP;            // Stack Pointer

    BYTE A, X, Y;       // Accumulator A, and GPR X and Y
    
    BYTE C : 1;     //status flags carry
    BYTE Z : 1;     //status flags zero
    BYTE I : 1;     //status flags intrupt disable
    BYTE D : 1;     //status flags decimal mode
    BYTE B : 1;     //status flags break cmd
    BYTE V : 1;     //status flags overflow
    BYTE N : 1;     //status flags negetive

    void Reset(Mem& memory){
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
        memory.Initialize(); 
    }

    BYTE FetchByte( u32 Cycles, Mem& memory ){
        BYTE Data  = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    void Execute( u32 Cycles, Mem& memory ){
        while( Cycles > 0 ){
            BYTE Ins = FetchByte( Cycles, memory );     // Instructions are fetched
            //(void)Ins;
        }
    }
};

int main(){
    CPU cpu;
    Mem mem;
    cpu.Reset( mem );
    cpu.Execute( 2, mem );
    printf("\nrunning...\n");
    return 0;
}