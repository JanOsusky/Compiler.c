# FACULTY OF INFORMATION TECHNOLOGIES HIGHER TECHNICAL EDUCATION IN BRNO
>
> ![](vertopal_fae3b0e5fc834e4eae81e6332c25e483/media/image1.png)
>
# Project documentation
>
# **Implementation of imperative translator** **language IFJ22**\
> Team xproch0q variant I BVS


**Introduction**

> The aim of the project was to program a program in the C language that would load
> source code
>
> written in the IFJ22 source language and translates it into the target language
> IFJcode22
>
> (intermediate code). If the entire translation takes place without errors, a return is returned
> value "0".

**Implementation**

> Description of the implementation of individual parts needed and overall
> functionality
>
> **Lexical Analysis**\
> The first part of the translator is the **scanner**, which reads and "detects"
> individual **lexems** and passes them to the **parser** in the form of tokens.
> The scanner was implemented according to a previously created diagram
> deterministic finite automaton without **epsilon** transitions. Diagram
> machine, you can find figure 1 at the end of the documentation.
>
> Scanner is implemented in the files **scanner.c** and in the header
> **scanner.h**. The main function of this part is
> **get_token(lexToken \*token)** where the characters from are constantly read
> standard input using the **getchar()** function. The ultimate automaton is
> implemented as a simple **switch**, where each case is one
> KA status. The final states are defined in the header as macros using
> #define**.**
>
> Scanner writes the token string into the **lexToken** structure. When
> errors will be returned
>
> corresponding **ERROR_CODE** **(number)**, according to loading error or
>
> **ERROR_NO_ERROR** on success.
>
> **Syntax parsing**\
> The second and most important part of the compiler is implemented in files
> **parser.c** and in the header **parser.h**. The **Parser** controls the entire process
> translation and is taken as a skeleton.
>
> Syntax analysis follows the established LL-grammar and method
> recursive
>
> descent (from top to bottom). To each rule in our constructed LL-grammar
> one answers
>
> function in **parser.c**. Our LL-grammar is at the end of the documentation
> picture 2 and 3.
>
> The main **parser** function is the **parser()** function called in the file
> **main.c**. The first token is loaded and according to the first rule check,
> they call other functions to check the rules. The entire **parser** continues to work
> recursively. A defined macro is used to load new tokens
> **GET_NEXT_TOKEN**.
>
> When the **parser** detects an error while checking the rules, it returns
> **ERROR_BAD_SYNTAX** otherwise returns **ERROR_NO_ERROR** which is
> value 0.
>
> When **parser** encounters an expression, the function on precedent is called
> parse **parse(&ParserData)** where the entire structure is passed
> **ParserData.**
>
> **Semantic Analysis**
>
> Semantic analysis takes place simultaneously with syntactic analysis. Help
> tables
>
> of symbols which is implemented as a binary search tree and has
> helpful
>
> function, it is checked whether functions and variables have been defined, these
> the function calls itself

2

> **parser**. Variable checking takes place in the **parser** using
> symbol tables for variables and checking functions as well.

**Expression Processing**\
Expressions are analyzed separately in the **precanal.c** files and in the header
**precanal.h**. The main function is **parse(ParserData \*data)** which
calls itself **parser**. The function retrieves the first token and on success
returns the number 0, otherwise returns the number **ERROR** corresponding to the detected error.
Expression processing takes place using precedent analysis (from bottom to top).
The new token is compared to the token at the top of the created stack where
a dollar sign is inserted into it at the beginning, and according to the precedent table
**table** is placed on the stack, or the expression is processed according to the rules
within the **reduce** function. The precedent table is at the end of the documentation
figure 4.

> **Values in the table:**
>
> 1.'\<' means that the loaded character is added to the stack (function
> **shift**)

2.'\>' means that the rule will be applied by calling the **reduce** function

> 3.'=' means that the parentheses are reduced
>
> 4.' ' semantic error
>
> 5.'#' successful return
>
> **Code Generator**
>
> **Abstract Data Structures**

Description of the data structures used

> **Table of Symbols**\
> We solved the symbol table in the form of recursive binary
> search tree.

We chose the recursive implementation for its simplicity, which we
made it possible to quickly

programming basic operations over a binary tree, thanks to which we
they could earlier

to address more complex problems.

> The table is divided into two parts: a table of symbols for functions and
> symbol table for

variables. One instance of the symbol table for variables is used for
the main body of the program. Other local variable tables (e.g. for
variables in functions) are then arranged to
+----------------------------------+------------- ---------------------+
| > •\ | > variable name\ |
| > •\ | > datatype\ |
| > •\ | > whether it can contain NULL |
| > • | > variable definition |
+===================================+============= ======================+
+----------------------------------+------------- ---------------------+

3

> **Table of symbols for functions**\
> Stores the following information:\
> • function name\
> • number of parameters\
> • parameter list\
> • function definition\
> • return value
>
> **Stack for working with expressions**\
> The stack is implemented in the files **precstack.c** and
> **precstack.h**.

The structure **prec_elem_t** data type token and is stored in the stack
if it is a terminal. Functions like
**init, push, pop, free**.

> **Linked list for generator**

**Dynamic String**\
The structure is used to hold the token string, contains the length, etc.
Functions like **dynStrInit, addChar** are implemented for string.

**Testing**

Testing was done manually throughout development.

**Team work**

> We created the team via Discord, but mostly we knew each other from the z team
> last year. On

we used Discord and Messenger to communicate. At school meetings,
we always just agreed on what needed to be done, how it should be done, what
function and we entered dates. Terms

we also had it written out on Microsoft Planner. He solved some parts
each member alone, we helped each other with the more difficult parts. End
submission, we mostly used Discord for calls and screen sharing.

> **Code Sharing**\
> We used the technology offered by GitHub. Thus, we
> made editing shared code easier. We programmed in Visual
> Studio Code and Sublime.

4



**Conclusion**

The project was large, but we tried to use all our skills
also from other subjects at school. The project gave us valuable experience within
programming, team work and agreements, and especially time allocation for
future major projects.

> **Finite State Machine Diagram**

![](vertopal_fae3b0e5fc834e4eae81e6332c25e483/media/image2.png){width="6.301388888888889in"
height="4.588888888888889in"}

*finite automaton receiving language IFJ22 Fig. 1*

5

**LL-grammar**

![](vertopal_fae3b0e5fc834e4eae81e6332c25e483/media/image3.png){width="6.301388888888889in"
height="3.3in"}

*LL-grammar list of rules Fig. 2*

![](vertopal_fae3b0e5fc834e4eae81e6332c25e483/media/image4.png){width="6.301388888888889in"
height="2.85in"}

*LL-grammar list of rules Fig. 3*

6

> **Table for precedent analysis**

stack of symbol tables for variables. Thanks to the stack, it is possible to determine
which variable

use when diving.

> **Table of symbols for variables**\
> Stores the following information:
