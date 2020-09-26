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

    mul r1, r2, r6          @ r1 = a * e
    mul r0, r1, r10         @ r0 = a * e * i

    mul r1, r3, r7          @ r1 = b * f
    mla r0, r1, r8, r0      @ r0 = (a * e * i) + (b * f * g)
    
    mul r1, r4, r5          @ r1 = c * d
    mla r0, r1, r9, r0      @ r0 = (a * e * i) + (b * f * g) + (c * d * h)

    mul r1, r4, r6          @ r1 = c * e
    mls r0, r1, r8, r0      @ r0 = (a * e * i) + (b * f * g) + (c * d * h) - (c * e * g)

    mul r1, r3, r5          @ r1 = b * d
    mls r0, r1, r10, r0     @ r0 = (a * e * i) + (b * f * g) + (c * d * h) - (c * e * g) - (b * d * i)

    mul r1, r2, r7          @ r1 = a * f
    mls r0, r1, r9, r0      @ r0 = (a * e * i) + (b * f * g) + (c * d * h) - (c * e * g) - (b * d * i) - (a * f * h)

    pop {r4-r10}
    bx lr
