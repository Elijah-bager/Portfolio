#Elijah Baugher
# BY SUBMITTING THIS FILE AS PART OF MY LAB ASSIGNMENT, I CERTIFY THAT
# ALL OF THE CODE FOUND WITHIN THIS FILE WAS CREATED BY ME WITH NO
# ASSISTANCE FROM ANY PERSON OTHER THAN THE INSTRUCTOR OF THIS COURSE
# OR ONE OF OUR UNDERGRADUATE GRADERS. I WROTE THIS CODE BY HAND,
# IT IS NOT MACHINE GENRATED OR TAKEN FROM MACHINE GENERATED CODE
.file "stack.s"

# Assembler directives to allocate storage for static array

.globl push_value
        .type push_value, @function 
.globl pop_value
        .type pop_value, @function
.section .text


        


    push_value: 
        
        #rdi holds stack struct
        #rsi holds value to be pushed
        #rdx holds top of stack pointer


        movq 8(%rdi), %rdx      #access top of stack pointer
        subq $8, %rdx           #decrement pointer to make space 
        
        movq %rsi, (%rdx)       #move the value into the stack
        movq %rdx, 8(%rdi)      #update top of stack pointer
       

        ret 
            .size push_value, .-push_value

    pop_value:

        #rdi holds stack struct
        #rsi holds top of stack pointer
        #rax is value popped from stack


         movq 8(%rdi), %rsi             #access top of stack pointer
         movq (%rsi), %rax              #move value to be popped into return address
         movq $0, (%rsi)                #replace value popped with zero
         addq $8, %rsi                  #increment top of stack pointer
         movq %rsi, 8(%rdi)             #update top of stack pointer
         

         ret
            .size pop_value, .-pop_value


