.text

/*  Calculates determinant of 3 x 3 matrix like:
    a b c
    d e f
    g h i
*/

.global count_det
count_det:
    ldr r2, [r0]        @ r2 = a
    ldr r3, [r0, #16]   @ r3 = e
    mul r2, r2, r3      @ r2 = a * e
    ldr r3, [r0, #32]   @ r3 = i
    mul r2, r2, r3      @ r2 = a * e * i
    mov r1, r2          @ r1 = a * e * i

    ldr r2, [r0, #4]    @ r2 = b
    ldr r3, [r0, #20]   @ r3 = f
    mul r2, r2, r3      @ r2 = b * f
    ldr r3, [r0, #24]   @ r3 = g
    mul r2, r2, r3      @ r2 = b * f * g
    add r1, r1, r2      @ r1 = (a * e * i) + (b * f * g)

    ldr r2, [r0, #8]    @ r2 = c
    ldr r3, [r0, #12]   @ r3 = d
    mul r2, r2, r3      @ r2 = c * d
    ldr r3, [r0, #28]   @ r3 = h
    mul r2, r2, r3      @ r2 = c * d * h
    add r1, r1, r2      @ r1 = (a * e * i) + (b * f * g) + (c * d * h)

    ldr r2, [r0, #8]    @ r2 = c
    ldr r3, [r0, #16]   @ r3 = e
    mul r2, r2, r3      @ r2 = c * e
    ldr r3, [r0, #24]   @ r3 = g
    mul r2, r2, r3      @ r2 = c * e * g
    sub r1, r1, r2      @ r1 = (a * e * i) + (b * f * g) + (c * d * h) - (c * e * g)

    ldr r2, [r0, #4]    @ r2 = b
    ldr r3, [r0, #12]   @ r3 = d
    mul r2, r2, r3      @ r2 = b * d
    ldr r3, [r0, #32]   @ r3 = i
    mul r2, r2, r3      @ r2 = b * d * i
    sub r1, r1, r2      @ r1 = (a * e * i) + (b * f * g) + (c * d * h) - (c * e * g) - (b * d * i)

    ldr r2, [r0]        @ r2 = a
    ldr r3, [r0, #20]   @ r3 = f
    mul r2, r2, r3      @ r2 = a * f
    ldr r3, [r0, #28]   @ r3 = h
    mul r2, r2, r3      @ r2 = a * f * h
    sub r1, r1, r2      @ r1 = (a * e * i) + (b * f * g) + (c * d * h) - (c * e * g) - (b * d * i) - (a * f * h)

    mov r0, r1
    bx lr
