# System-Call

## How to run lab2
### Step 1:
```bash
make qemu
```
### Step 2:
After running the prompt from 'Step 1', run this following prompt to check which pages have been accessed:
```bash
pageAccessTest
```


we got 2 solutions for print page table:
    1/ use new sys_exec_vmprint (just copy for exec and add vmprint(pagetable) before return argc) 
        => cmd: vmprint wc README

    2/ modify exec and use print page table flag 
        => cmd: wc README -printpgt

