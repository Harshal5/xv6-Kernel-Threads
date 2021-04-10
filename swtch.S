# Context switch
#
#   void swtch(struct context **old, struct context *new);
# 
# Save the current registers on the stack, creating
# a struct context, and save its address in *old.
# Switch stacks to new and pop previously-saved registers.

# remember operands were pushed in right->left order. Finally EIP was pushed.
# esp contains ret-address, esp+4 = old, esp+8 = new
.globl swtch
swtch:
  movl 4(%esp), %eax    # eax = old
  movl 8(%esp), %edx    # edx = new

  # Save old callee-saved registers
  pushl %ebp
  pushl %ebx
  pushl %esi
  pushl %edi

  # Switch stacks
  movl %esp, (%eax)   #  *eax = *(old) = current stack = {old context with 5 values on it eip, ebp, ebx, esi, edi in reverse order}
  movl %edx, %esp     #  esp = edx = {new context address} // this must have 5 values as a result of previous swtch()

  # Load new callee-saved registers
  popl %edi
  popl %esi
  popl %ebx
  popl %ebp
  ret                 #  this will return in the context of the "new" now 
