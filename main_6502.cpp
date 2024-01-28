#include <STDIO.H>
#include <STDLIB.H>

using BYTE = unsigned char;
using WORD = unsigned short;
using u32 = unsigned int;


struct Mem
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    BYTE Data[MAX_MEM];
    void Initialize()
    {
        for( u32 i = 0; i < MAX_MEM; i++ )
        {
            Data[i] = 0;
        }
    }
    /*Read 1 Byte*/
    BYTE operator[]( u32 Address ) const 
    {
        //assert here Address is < MAX_MEM
        return Data[Address];
    }
    /* write one byte to mem */
    BYTE& operator[]( u32 Address ) 
    {
        //assert here Address is < MAX_MEM
        return Data[Address];
    }
};

struct CPU 
{

    
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

    void Reset(Mem& memory)
    {
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
        memory.Initialize(); 
    }

    BYTE FetchByte( u32& Cycles, Mem& memory )
    {                         //reads a byte from the mem and increments the PC
        BYTE Data  = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    BYTE ReadByte( u32& Cycles, BYTE memloc, Mem& memory )
    {              //reads a byte from the mem without incrementing the PC
        BYTE Data  = memory[memloc];
        Cycles--;
        return Data;
    }
    void LDASetStatus()
    {
        Z = (A == 0);                       // checks what the value of A is and sets the flag to true if the value of the A is 0
        N = (A & 0b10000000) > 0;           // ANDs the value of A and 10000000 to see if the 7th bit is set i.e the number is negetive or not if it is greater than 1 then the N flag gets set
    }

    // opcodes
    static constexpr BYTE INS_LDA_IM = 0xA9;
    static constexpr BYTE INS_LDA_ZP = 0xA5;
    static constexpr BYTE INS_LDA_ZX = 0xB5;

    void Execute( u32 Cycles, Mem& memory )
    {
        while( Cycles > 0 )
        {
            printf("%d Cycles\n", Cycles);
            BYTE Ins = FetchByte( Cycles, memory );     // Instructions are fetched
            printf("%d Ins\n", Ins);
            
            switch (Ins)
            {
                case(INS_LDA_IM):       // 2 bytes, 2 cycles : Fetches the data in the from the address in the PC
                {                      
                    BYTE Value = FetchByte(Cycles, memory);
                    A = Value;
                    printf("%d A \n", A);
                    LDASetStatus();
                }break;
                case(INS_LDA_ZP):
                {                      // 2 bytes, 3 cycles : fetches the data fromt he address in the ZeroPage
                    BYTE ZeroPageAddress = FetchByte(Cycles, memory);
                    A = ReadByte(Cycles, ZeroPageAddress, memory);
                    printf("%d A \n", A);
                    LDASetStatus();
                }break;
                case(INS_LDA_ZX):       // 2 bytes, 3 cycles : fetches the data fromt he address in the ZeroPage
                {                      
                    BYTE ZeroPageAddress = FetchByte(Cycles, memory);
                    BYTE AddressToFetch = ZeroPageAddress + X;      // takes 1 cycle
                    Cycles--;
                    A = ReadByte(Cycles, AddressToFetch, memory);
                    printf("%d A \n", A);
                    LDASetStatus();
                }break;
                default:
                {
                    printf("Instruction not handled %d \n", Ins);
                }    
                break;
            }

        }
    }
};

int main()
{
    CPU cpu;
    Mem mem;
    cpu.Reset( mem );
    /* start - Inline a little program to check INS_LDA_IM*/
        // mem[0xFFFC] = CPU::INS_LDA_IM;
        // mem[0xFFFD] = 0x0042;
        //cpu.Execute( 2, mem );
    /* end - Inline a little program*/

    /* start - Inline a little program to check INS_LDA_ZP */
        // mem[0xFFFC] = CPU::INS_LDA_ZP;
        // mem[0xFFFD] = 0x0042;
        // mem[0x0042] = 0x0006;
        // cpu.Execute( 3, mem );
        // cpu.Reset( mem );
    /* end - Inline a little program*/  
   
    /* start - Inline a little program to check INS_LDA_ZX */
        cpu.X = 1;
        mem[0xFFFC] = CPU::INS_LDA_ZX;
        mem[0xFFFD] = 0x0042;
        mem[0x0043] = 0x0009;
        cpu.Execute( 4, mem );
        cpu.Reset( mem );
    /* end - Inline a little program*/
    return 0;
}