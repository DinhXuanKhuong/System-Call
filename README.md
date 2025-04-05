# System-Call


we got 2 solutions for print page table:
    1/ use new sys_exec_vmprint (just copy for exec and add vmprint(pagetable) before return argc) 
        => cmd: vmprint wc README

    2/ modify exec and use print page table flag 
        => cmd: wc README -printpgt

