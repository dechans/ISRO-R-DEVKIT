#pragma once

#include "IFWnd.h"

class CIFStretchWnd : public CIFWnd {
public: /* interface functions */

private: /* members */
    char pad_0x036C[0x34]; //0x036C
    char *N000062E6; //0x03A0
    char pad_0x03A4[0x18]; //0x03A4
    char *N000062F0; //0x03BC
    char pad_0x03C0[0x18]; //0x03C0
    char *N000062F7; //0x03D8
    char pad_0x03DC[0x18]; //0x03DC
    char *N000062FE; //0x03F4
    char pad_0x03F8[0x18]; //0x03F8
    char *N00006305; //0x0410
    char pad_0x0414[0x18]; //0x0414
    char *N0000630C; //0x042C
    char pad_0x0430[0x18]; //0x0430
    char *N00006313; //0x0448
    char pad_0x044C[0x18]; //0x044C
    char *N0000631A; //0x0464
    char pad_0x0468[0x35C]; //0x0468

private:
    BEGIN_FIXTURE()
        ENSURE_SIZE(0x07C4)
    END_FIXTURE()

    RUN_FIXTURE(CIFStretchWnd)
};
