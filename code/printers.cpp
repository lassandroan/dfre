// Copyright (c) 2016-2017 Andrew Kallmeyer <fsmv@sapium.net>
// Provided under the MIT License: https://mit-license.org

#include "nfa.h"

void PrintNFA(nfa *NFA) {
    Print(Out, "Number of states: %u\n", NFA->NumStates);

    for (size_t ArcListIdx = 0; ArcListIdx < NFA->ArcListCount; ++ArcListIdx) {
        nfa_arc_list *ArcList = NFAGetArcList(NFA, ArcListIdx);

        Print(Out, "Label: %u, %c, %c\n",
              ArcList->Label.Type, ArcList->Label.A, ArcList->Label.B);

        for (size_t TransitionIdx = 0;
             TransitionIdx < ArcList->TransitionCount;
             ++TransitionIdx)
        {
            nfa_transition *Transition = &ArcList->Transitions[TransitionIdx];

            Print(Out, "\t%u => %u\n", Transition->From, Transition->To);
        }
    }
}

#include "x86_opcode.h"

void PrintInstructions(instruction *Instructions, size_t NumInstructions) {
    Print(Out, "   Op #   | Mode | Op  |     Arg1      | Arg2\n");
    Print(Out, "----------------------------------------------\n");
    //           12345678 | REG  | INC | Op # 12345678 |
    //           12345678 | REG  | INC | 12345678      | EAX
    //           12345678 | REG  | INC | EAX           |
    //           12345678 | REG  | INC | EAX           | EAX

    const size_t MaxIntLen = 8;
    const char IntPaddingStr[MaxIntLen+1] = "        ";
    const char *Separator = " | ";

    for (size_t Idx = 0; Idx < NumInstructions; ++Idx) {
        instruction *Instruction = &Instructions[Idx];
        char IntBuf[MaxIntLen+1];
        size_t IntLen;

        // Op #
        Print(Out, " ");
        IntLen = WriteInt((uint32_t)Idx, IntBuf);
        IntBuf[IntLen] = '\0';
        Print(Out, IntBuf);
        Print(Out, IntPaddingStr + IntLen);

        Print(Out, Separator);

        // Mode
        switch (Instruction->Mode) {
        case REG:
            Print(Out, "REG ");
            break;
        case MEM:
            Print(Out, "MEM ");
            break;
        case MODE_NONE:
            Print(Out, "J   ");
            break;
        }

        Print(Out, Separator);

        // Op
        Print(Out, op_strings[Instruction->Op]);

        Print(Out, Separator);

        // Args
        switch (Instruction->Type) {
        case JUMP:
            Print(Out, "Op # ");

            IntLen = WriteInt((uint32_t)(Instruction->JumpDest - Instructions), IntBuf);
            IntBuf[IntLen] = '\0';
            Print(Out, IntBuf);
            Print(Out, IntPaddingStr + IntLen);

            Print(Out, Separator);
            break;
        case ONE_REG:
            Print(Out, reg_strings[Instruction->Dest]);
            Print(Out, "         "); // 9

            Print(Out, Separator);
            break;
        case TWO_REG:
            Print(Out, reg_strings[Instruction->Dest]);
            Print(Out, "         "); // 9

            Print(Out, Separator);

            Print(Out, reg_strings[Instruction->Src]);
            break;
        case REG_IMM:
            IntLen = WriteInt(Instruction->Imm, IntBuf);
            IntBuf[IntLen] = '\0';
            Print(Out, IntBuf);
            Print(Out, IntPaddingStr + IntLen);
            Print(Out, "     "); // 5

            Print(Out, Separator);

            Print(Out, reg_strings[Instruction->Dest]);
            break;
        }

        Print(Out, "\n");
    }
}

void PrintUnpackedOpcodes(opcode_unpacked *UnpackedOpcodes, size_t NumOpcodes) {
    // TODO
}

void PrintByteCode(uint8_t *Code, size_t Size) {
    for (size_t i = 0; i < Size; ++i) {
        char IntStr[4];
        if (Code[i] < 0x10) {
            IntStr[0] = '0';
            WriteInt(Code[i], IntStr+1);
        } else {
            WriteInt(Code[i], IntStr);
        }

        IntStr[2] = ' ';
        IntStr[3] = '\0';

        Print(Out, IntStr);
    }
}