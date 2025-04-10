#Elijah Baugher
# BY SUBMITTING THIS FILE AS PART OF MY LAB ASSIGNMENT, I CERTIFY THAT
# ALL OF THE CODE FOUND WITHIN THIS FILE WAS CREATED BY ME WITH NO
# ASSISTANCE FROM ANY PERSON OTHER THAN THE INSTRUCTOR OF THIS COURSE
# OR ONE OF OUR UNDERGRADUATE GRADERS. I WROTE THIS CODE BY HAND,
# IT IS NOT MACHINE GENRATED OR TAKEN FROM MACHINE GENERATED CODE
.file "execute.s"

# Assembler directives to allocate storage for static array
.section .rodata
    Print_bad:
        .string "bad type %d\n"         #first string
    Print_push:
        .string "pushed %ld\n"          #second string
    Print_result:
        .string "%ld %c %ld = %ld\n"       #third string





.globl execute
        .type execute, @function 

.text
execute: 

    #rdi holds stack struct 
    #rsi holds op struct
    #rdx holds type member of op struct
    #rcx holds third parameter for print call
    #r8  holds result for print call
    #rbx stores stack struct
    #r12 stores op struct
    #r13 holds popped B value
    #r14 holds popped A value
    #r15 holds final result 


    pushq %rbp          #Set up stack frame
    movq %rsp, %rbp     #Done
    subq $8, %rsp

    pushq %rbx          #Push callee saved registers
    pushq %r12          #
    pushq %r13          #
    pushq %r14          #
    pushq %r15          #Done
    
    xorq %rax, %rax     #zero return address
    movl 4(%esi), %edx     #access type member of op struct
    
    cmpq $0, %rdx        #case operator
    je case_0

    cmpq $1, %rdx        #case operand 
    je case_1

    jmp default             #default case of switch case

    case_1:
        movq %rdi, %rbx         #save stack struct in callee saved register
        movq %rsi, %r12         #save op struct in callee saved register
        movq 8(%r12), %rsi      #access value member of op struct and store it in second parameter for push call
        call push_value         #push value into stack 

        movq $Print_push, %rdi          #"pushed %ld\n"  
        movq 8(%r12), %rsi              #store value into second paramter print call
        call print                      #call print
        
        jmp done                        #break
        
        
    case_0:
        movq %rdi, %rbx         #save stack structs into callee saved registers
        movq %rsi, %r12         #op struct into callee saved register

        call pop_value          #pop B off stack 

        movq %rax, %r13         #move returned value into callee saved register
        movq %rbx, %rdi         #restore stack struct as first parameter for pop call
        
        call pop_value          #pop A off stack  

        movq %rax, %r14       #store returned value into callee saved register

        movq %r13, %rsi         #Store B in second parameter
        movq %r14, %rdi         #store A in first parameter

        call *16(%r12)          #indirect call to math function 
        movq %rax, %r15        #move result into callee saved register

        movq %rbx, %rdi         #move stack struct into first parameter for push call
        movq %r15, %rsi         #move value into second parameter for push call
    
        call push_value         #push value into stack

        movq $Print_result, %rdi    #"%ld %c %ld = %ld\n" 
        movq %r14, %rsi             #A is our first parameter for print call
        movzbq 0(%r12), %rdx             #tag member of op struct is our second parameter for print call
        movq %r13, %rcx             #B is our third parameter for print call
        movq %r15, %r8              #the result is our fourth parameter for print call
        
        call print              #print call
        
        jmp done                #break


        

    default:
        movq $Print_bad, %rdi          #"bad type %d\n"
        movl %edx, %esi                #store type member of op struct into second parameter for print call
        call print                     #call print
        jmp done                       #break

    done:

    popq %r15           #return the callee saved registers after use
    popq %r14           #
    popq %r13           #
    popq %r12           #
    popq %rbx           #done
    leave   
    ret

.size execute, .-execute
