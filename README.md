# CPT316 - Programming Language Implementation and Paradigms (Assignment 1)

<p align="center">
  <img src="https://img.shields.io/badge/Language-C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="Language" />
  <img src="https://img.shields.io/badge/Course-CPT316-24292e?style=for-the-badge" alt="Course" />
  <a href="https://github.com/ahmedtags">
    <img src="https://img.shields.io/badge/Profile-ahmedtags-D9A34A?style=for-the-badge&logo=github&logoColor=white" alt="Profile" />
  </a>
  <a href="https://blxman-37fy.vercel.app/">
    <img src="https://img.shields.io/badge/Portfolio-blxman--37fy-0A66C2?style=for-the-badge&logo=googlechrome&logoColor=white" alt="Portfolio" />
  </a>
</p>

---

This repository contains the C++ compiler scanner and parser implementation and PDF report for **CPT316: Programming Language Implementation and Paradigms - Assignment 1** (Semester 1, Academic Session 2025/2026) at Universiti Sains Malaysia (USM).

## Course Details
- **Course Code:** CPT316
- **Course Name:** Programming Language Implementation and Paradigms
- **Semester:** Semester 1, Year 3 (2025/2026)
- **Project Title:** COMPY Language Lexical & Syntax Analyzer

---

## Assignment Overview

The assignment requires building a mini-compiler (lexical scanner and syntax parser) for a custom simple expression language named **COMPY**:
1. **Lexical Analysis (Scanner):**
   - Scan raw input to identify and generate a token stream.
   - Categorize tokens: identifiers (single-letter lowercase `[a-z]`), numbers (integers), operators (`+`, `-`, `*`, `/`), assignment (`=`), parentheses (`(`, `)`), and the statement terminator (`;`).
   - Identify invalid tokens and report their exact positions.
   - Count total tokens and summarize counts by type.
2. **Syntax Analysis (Parser):**
   - Parse the token stream against the COMPY grammar.
   - Generate and display a hierarchical syntax tree for valid input expressions.
   - Report descriptive syntax errors on parsing failures.
   - **Grammar Specification:**
     - `<stmt> -> id = <expr> ;`
     - `<expr> -> <term> | <expr> '+' <term> | <expr> '-' <term>`
     - `<term> -> <factor> | <term> '*' <factor> | <term> '/' <factor>`
     - `<factor> -> id | int | ( <expr> )`

---

## What I Did
- Developed the scanner and recursive-descent parser in C++: [`cpt316-code.cpp`](cpt316-code.cpp).
- Handled syntax tree output print formatting.
- Documented compiler structure, grammar derivations, implementation paradigm, and test cases in the report: [`Group24_Compy_Report.pdf`](Group24_Compy_Report.pdf).
- Consolidated the original assignment guideline: [`CPT316  ASG1 SEM 1 2025_2026.pdf`](CPT316%20%20ASG1%20SEM%201%202025_2026.pdf).

---

## Tools & Tech Stack
- **Language:** C++ (Standard library streams and string parsing)
- **Paradigm:** Procedural / Object-Oriented Parsing

---

## How to Run

1. Compile the C++ program using any C++11 compliant compiler (e.g., `g++`):
   ```bash
   g++ -std=c++11 cpt316-code.cpp -o compy_parser
   ```
2. Run the executable:
   ```bash
   ./compy_parser
   ```
3. Enter expression statements (e.g., `x = a + (b * 5);`) to view the token breakdown and generated syntax tree.

---

## 📸 Sample Output

Input: `x = a + (b * 5);`

```
***********************************************
*     COMPY LANGUAGE MINI-COMPILER           *
*     CPT316 Assignment 1                    *
***********************************************

Choose an option:
1. Enter custom input
2. Exit
Enter choice: 1

Enter COMPY statement: x = a + (b * 5);
============================================================
INPUT: x = a + (b * 5);
============================================================

--- LEXICAL ANALYSIS ---

========== TOKEN STREAM ==========
No.  Type           Value     Position
----------------------------------------
1    IDENTIFIER     x         0
2    ASSIGN         =         2
3    IDENTIFIER     a         4
4    PLUS           +         6
5    LPAREN         (         8
6    IDENTIFIER     b         9
7    MULTIPLY       *         11
8    NUMBER         5         13
9    RPAREN         )         14
10   SEMICOLON      ;         15
=================================

========== TOKEN STATISTICS ==========
Total tokens: 10
Identifiers: 3
Numbers: 1
Operators: 2
Other tokens: 4
======================================

--- SYNTAX ANALYSIS ---

Parsing successful! Syntax tree generated.

========== SYNTAX TREE ==========
+-- <stmt>
    |-- x
    |-- =
    |-- <expr>
    |   |-- <term>
    |   |   +-- <factor>
    |   |       +-- a
    |   |-- +
    |   +-- <term>
    |       +-- <factor>
    |           |-- (
    |           |-- <expr>
    |           |   +-- <term>
    |           |       |-- <factor>
    |           |       |   +-- b
    |           |       |-- *
    |           |       +-- <factor>
    |           |           +-- 5
    |           +-- )
    +-- ;
=================================
```
