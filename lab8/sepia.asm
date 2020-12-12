default rel
global sepia_sse

section rodata
    red_first_iter:     dd 0.393, 0.349, 0.272, 0.393
    green_first_iter:   dd 0.769, 0.686, 0.534, 0.769
    blue_first_iter:    dd 0.189, 0.168, 0.131, 0.189

    red_second_iter:    dd 0.349, 0.272, 0.393, 0.349
    green_second_iter:  dd 0.686, 0.534, 0.769, 0.686
    blue_second_iter:   dd 0.168, 0.131, 0.189, 0.168

    red_third_iter:     dd 0.272, 0.393, 0.349, 0.272
    green_third_iter:   dd 0.534, 0.769, 0.686, 0.534
    blue_third_iter:    dd 0.131, 0.189, 0.168, 0.131

section text
sepia_sse:
    sub rsp, 16
    pxor xmm15, xmm15

    ; rdi:
    ; |  r1 |  g1  |  b1  |  r2  |  g2  |  b2  |  r3  |  g3  |  b3  |  r4  |  g4  |  b4
    ; ↑     ↑      ↑      ↑      ↑      ↑      ↑      ↑      ↑      ↑      ↑      ↑
    ; +0    +4     +8     +12    +16    +20    +24    +28    +32    +36    +40    +44

    ; read - part 1/3
    mov eax, dword[rdi] ; rax <- r1
    mov dword[rsp], eax
    mov dword[rsp + 4], eax
    mov dword[rsp + 8], eax
    mov eax, dword[rdi + 12] ; rax <- r2
    mov dword[rsp + 12], eax
    movups xmm0, [rsp]  ; xmm0 = r1 | r1 | r1 | r2

    mov eax, dword[rdi + 4] ; rax <- g1
    mov dword[rsp], eax
    mov dword[rsp + 4], eax
    mov dword[rsp + 8], eax
    mov eax, dword[rdi + 16] ; rax <- g2
    mov dword[rsp + 12], eax
    movups xmm1, [rsp]  ; xmm1 = g1 | g1 | g1 | g2

    mov eax, dword[rdi + 8] ; rax <- b1
    mov dword[rsp], eax
    mov dword[rsp + 4], eax
    mov dword[rsp + 8], eax
    mov eax, dword[rdi + 20] ; rax <- b2
    mov dword[rsp + 12], eax
    movups xmm2, [rsp]  ; xmm2 = b1 | b1 | b1 | b2

    ; read - part 2/3
    mov eax, dword[rdi + 12]
    mov dword[rsp], eax
    mov dword[rsp + 4], eax
    mov eax, dword[rdi + 24]
    mov dword[rsp + 8], eax
    mov dword[rsp + 12], eax
    movups xmm3, [rsp]  ; xmm3 = r2 | r2 | r3 | r3

    mov eax, dword[rdi + 16]
    mov dword[rsp], eax
    mov dword[rsp + 4], eax
    mov eax, dword[rdi + 28]
    mov dword[rsp + 8], eax
    mov dword[rsp + 12], eax
    movups xmm4, [rsp]  ; xmm4 = g2 | g2 | g3 | g3

    mov eax, dword[rdi + 20]
    mov dword[rsp], eax
    mov dword[rsp + 4], eax
    mov eax, dword[rdi + 32]
    mov dword[rsp + 8], eax
    mov dword[rsp + 12], eax
    movups xmm5, [rsp]  ; xmm5 = b2 | b2 | b3 | b3

    ; read - part 3/3
    mov eax, dword[rdi + 24]
    mov dword[rsp], eax
    mov eax, dword[rdi + 36]
    mov dword[rsp + 4], eax
    mov dword[rsp + 8], eax
    mov dword[rsp + 12], eax
    movups xmm6, [rsp]  ; xmm6 = r3 | r4 | r4 | r4

    mov eax, dword[rdi + 28]
    mov dword[rsp], eax
    mov eax, dword[rdi + 40]
    mov dword[rsp + 4], eax
    mov dword[rsp + 8], eax
    mov dword[rsp + 12], eax
    movups xmm7, [rsp]  ; xmm7 = g3 | g4 | g4 | g4

    mov eax, dword[rdi + 32]
    mov dword[rsp], eax
    mov eax, dword[rdi + 44]
    mov dword[rsp + 4], eax
    mov dword[rsp + 8], eax
    mov dword[rsp + 12], eax
    movups xmm8, [rsp]  ; xmm8 = b3 | b4 | b4 | b4

    ; move coefs (part 1/3 & part 2/3)
    movups xmm9, [red_first_iter]    ; xmm9 = c11 | c21 | c31 | c11
    movups xmm10, [green_first_iter]  ; xmm10 = c12 | c22 | c32 | c12
    movups xmm11, [blue_first_iter]   ; xm11 = c13 | c23 | c33 | c13

    movups xmm12, [red_second_iter]    ; xmm12 = c21 | c31 | c11 | c21
    movups xmm13, [green_second_iter]  ; xmm13 = c22 | c32 | c12 | c22
    movups xmm14, [blue_second_iter]   ; xmm14 = c23 | c33 | c13 | c23

    ; mul (part 1/3)
    mulps xmm0, xmm9 ; xmm0 = r1*c11 | r1*c21 | r1*c31 | r2*c11
    mulps xmm1, xmm10 ; xmm1 = g1*c12 | g1*c22 | g1*c32 | g2*c12
    mulps xmm2, xmm11 ; xmm2 = b1*c13 | b1*c23 | b1*c33 | b2*c13

    ; mul (part 2/3)
    mulps xmm3, xmm12 ; xmm3 = r2*c21 | r2*c31 | r3*c11 | r3*c21
    mulps xmm4, xmm13 ; xmm4 = g2*c22 | g2*c32 | g3*c12 | g3*c22
    mulps xmm5, xmm14 ; xmm5 = b2*c23 | b2*c33 | b3*c13 | b3*c23

    addps xmm1, xmm2
    addps xmm0, xmm1 ; xmm0 = R1 | G1 | B1 | R2

    addps xmm4, xmm5
    addps xmm3, xmm4 ; xmm3 = G2 | B2 | R3 | G3

    cvtps2dq xmm0, xmm0 ; -> 4 byte int
    packusdw xmm0, xmm0 ; -> 2 byte int
    packuswb xmm0, xmm0 ; -> 1 byte int
    punpcklbw xmm0, xmm15 ; -> 2 byte int
    punpcklwd xmm0, xmm15 ; -> 4 byte int
    cvtdq2ps xmm0, xmm0 ; -> float
    movups [rsi], xmm0

    cvtps2dq xmm3, xmm3 ; -> 4 byte int
    packusdw xmm3, xmm3 ; -> 2 byte int
    packuswb xmm3, xmm3 ; -> 1 byte int
    punpcklbw xmm3, xmm15 ; -> 2 byte int
    punpcklwd xmm3, xmm15 ; -> 4 byte int
    cvtdq2ps xmm3, xmm3 ; -> float
    movups [rsi + 16], xmm3

    ;-------------------------------------------------------------------
    ; move coefs (part 3/3)
    movups xmm9, [red_third_iter]    ; xmm9 = c31 | c11 | c21 | c31
    movups xmm10, [green_third_iter]  ; xmm10 = c32 | c12 | c22 | c32
    movups xmm11, [blue_third_iter]   ; xmm11 = c33 | c13 | c23 | c33

    ; mul (part 3/3)
    mulps xmm6, xmm9 ; xmm6 = r3*c31 | r4*c11 | r4*c21 | r4*c31
    mulps xmm7, xmm10 ; xmm7 = g3*c32 | g4*c12 | g4*c22 | g4*c32
    mulps xmm8, xmm11 ; xmm8 = b3*c33 | b4*c13 | b4*c23 | b4*c33

    addps xmm7, xmm8
    addps xmm6, xmm7 ; xmm6 = B3 | R4 | G4 | B4

    cvtps2dq xmm6, xmm6 ; -> 4 byte int
    packusdw xmm6, xmm6 ; -> 2 byte int
    packuswb xmm6, xmm6 ; -> 1 byte int
    punpcklbw xmm6, xmm15 ; -> 2 byte int
    punpcklwd xmm6, xmm15 ; -> 4 byte int
    cvtdq2ps xmm6, xmm6 ; -> float
    movups [rsi + 32], xmm6

    add rsp, 16
    ret