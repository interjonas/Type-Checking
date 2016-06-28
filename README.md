# Type Checking
I designed a program which was required to satisfy type checking rules and specifications as outlined by an assigned project. The project had required that a program should be able to handle a set of strict rules all centered around variable declarations especially the nature of implicit variable declarations. This project is based off of lexical analysis and a set of grammar rules which were to be implented by the use of a recusrive descent parser. The project had also specified that the program must build a data structure that constantly updates itself during runtime. The data structure would be a symbol table that stores essential characteristics of each variable. If a variable has an explicit declaration it is stored in the symbol table with its correspnding declaration. If a variable however has an implicit declaration the program must run through an exhaustive set of conditions to determine how to save the implicit declaration. By using Hindley-Milner principles I implemented functions that unified variables of different types into the same implicit/explicit variable type. If a variable is determined to have a specific type (int,real,bool,etc.) in the latter parts of a user input then the program must find all associated implicit variables connected to the newly declared variable and update the symbol table such that all "unified" variables update their implicit type (i.e. T1,T2) in unison. Overall goal of this project was to understand how a compiler would interpret unknown variables and how it would decide to store a given variable before considering variable errors.

# Examples

**Example 1: Below is user input**
```
{
  if (a)
  {
    b = * 2 b;
  } 
}
```
** Below is program output **
```
a: bool #
b: int #
```
**Example 2: Below is user input**
```
a, b: int;
c: int;
{
  x = + a * b c;
  y = ! true; 
}
```
** Below is program output **
```
a: int #
b: int #
c: int #
x: int #
y: bool #
```
**Example 3: Below is user input**
```
{
    if (> r s) 
    {
        r = * s t;
    }
    if (= t u) 
    {
        t = + u v;
    }
    v = 0.0;
}
```
** Below is program output **
```
r: real #
s: real #
t: real #
u: real #
v: real #
```
