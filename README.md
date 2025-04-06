# 📌 Lab2: Page Tables

> This project aims to help students know more about xv6 and page tables in RISC-V architechture.

---

## Members
- Phan Trung Nhựt - 23127240
- Đinh Xuân Khương - 23127398
- Nguyễn Đồng Thanh - 23127538
---

## 📝 Index

1. [Print a page table](#1-print-a-page-table)
2. [Detect which pages have been accessed](#2-detect-which-pages-have-been-accsessed)
---

## 1. Print a page table

we got two solutions. 
Example: wc README (not show print table)
    1/ $ vmprint wc README
    2/ $ wc README -printpgt
    
    both will print page table

## 2. Detect which pages have been accsessed
- In the terminal (shell), run the following prompt to debug which pages have been accessed:
```bash
$ make qemu # compile the whole system
# after running make qemu
$ pageAccessTest # let us debug which pages have been accessed
```