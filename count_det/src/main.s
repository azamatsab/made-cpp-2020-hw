.text

/*  Calculates determinant of 3 x 3 matrix like:
    a b c
    d e f
    g h i
*/

.global count_det
count_det:
    push {r4-r10}
    ldr r2, [r0]            @ r2 = a
    ldr r3, [r0, #4]        @ r3 = b
    ldr r4, [r0, #8]        @ r4 = c
    ldr r5, [r0, #12]       @ r5 = d
    ldr r6, [r0, #16]       @ r6 = e
    ldr r7, [r0, #20]       @ r7 = f
    ldr r8, [r0, #24]       @ r8 = g
    ldr r9, [r0, #28]       @ r9 = h
    ldr r10, [r0, #32]      @ r10 = i

    mul r1, r2, r6          @ r2 = a * e
    mul r1, r1, r10         @ r2 = a * e * i
    mov r0, r1              @ r0 = a * e * i

    mul r1, r3, r7          @ r2 = b * f
    mul r1, r1, r8          @ r2 = b * f * g
    add r0, r0, r1          @ r0 = (a * e * i) + (b * f * g)
    
    mul r1, r4, r5          @ r2 = c * d
    mul r1, r1, r9          @ r2 = c * d * h
    add r0, r0, r1          @ r0 = (a * e * i) + (b * f * g) + (c * d * h)

    mul r1, r4, r6          @ r2 = c * e
    mul r1, r1, r8          @ r2 = c * e * g
    sub r0, r0, r1          @ r0 = (a * e * i) + (b * f * g) + (c * d * h) - (c * e * g)

    mul r1, r3, r5          @ r2 = b * d
    mul r1, r1, r10         @ r2 = b * d * i
    sub r0, r0, r1          @ r0 = (a * e * i) + (b * f * g) + (c * d * h) - (c * e * g) - (b * d * i)

    mul r1, r2, r7          @ r2 = a * f
    mul r1, r1, r9          @ r2 = a * f * h
    sub r0, r0, r1          @ r0 = (a * e * i) + (b * f * g) + (c * d * h) - (c * e * g) - (b * d * i) - (a * f * h)

    pop {r4-r10}
    bx lr
