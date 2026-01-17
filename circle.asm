IN ; radius
PUSH 5
MUL

IN ; x0
PUSH 5
MUL
POPR RGX

IN ; y0
PUSH 6
MUL
POPR RIX

PUSH 51 ; длина строки
POPR RHX

POPR RAX

PUSH 0
POPR RDX ; cчетчик

;afbffdab

:cir

PUSHR RDX 
PUSH 3000 ; количество ячеек

JAE :end

CALL :x
PUSHR RGX
SUB
CALL :pow

CALL :y
PUSHR RIX
SUB
CALL :pow

ADD

PUSHR RAX
CALL :pow

JA :dot

PUSH 42
POPM [RDX]
CALL :inc

J :cir

:dot
    PUSH 46
    POPM [RDX]
    CALL :inc

J :cir

:end
    DRAW
    HLT

:inc         ;увеличение счетчика на 1
    PUSHR RDX
    PUSH 1
    ADD
    POPR RDX
    RET

:x
    PUSHR RDX
    PUSHR RHX
    REM
    PUSH 5
    MUL
    RET

:y
    PUSHR RDX
    PUSHR RHX
    DIV
    PUSH 6
    MUL
    RET

:pow
    POPR REX
    PUSHR REX
    PUSHR REX
    MUL
    RET

HLT