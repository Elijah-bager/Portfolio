#Elijah Baugher
# BY SUBMITTING THIS FILE AS PART OF MY LAB ASSIGNMENT, I CERTIFY THAT
# ALL OF THE CODE FOUND WITHIN THIS FILE WAS CREATED BY ME WITH NO
# ASSISTANCE FROM ANY PERSON OTHER THAN THE INSTRUCTOR OF THIS COURSE
# OR ONE OF OUR UNDERGRADUATE GRADERS. I WROTE THIS CODE BY HAND,
# IT IS NOT MACHINE GENRATED OR TAKEN FROM MACHINE GENERATED CODE
.file "ops.s"

# Assembler directives to allocate storage for static array

.globl add
        .type add, @function 
.globl subtract
        .type subtract, @function 
.globl multiply
        .type multiply, @function 
.globl divide
        .type divide, @function 
.section .text



add:

        #rdi is A
        #rsi is B
        #rax is return value

    pushq %rbp                  #set up stack frame
    movq %rsp, %rbp

    leaq (%rdi,%rsi), %rax      # add first parameter to second parameter
    leave
    ret
        .size add, .-add


subtract:
    #rdi is A
    #rsi is B
    #rax is result

    pushq %rbp                  #set up stack frame
    movq %rsp, %rbp

    subq %rsi, %rdi             #subtract first parameter by second parameter
    movq %rdi, %rax             #move result into return address 

    leave
    ret
        .size subtract, .-subtract


multiply:
        #rdi is A
        #rsi is B
        #rax is result 
        
    pushq %rbp                  #set up stack frame
    movq %rsp, %rbp

    imulq %rdi, %rsi            #multiply first parameter by second parameter
    movq %rsi, %rax             #move result into return address
    leave
    ret
        .size multiply, .-multiply


divide:
        #rdi is A
        #rsi is B
        #rax is result

    pushq %rbp                  #set up stack frame
    movq %rsp, %rbp
    movq %rdi, %rax             #A goes into rax
    cqto                        #trashes rdx
    idivq %rsi                  #divide A by B leaves result in RAX
    leave
    ret
        .size divide, .-divide
