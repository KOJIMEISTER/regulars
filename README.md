# regulars
This programm build regular set on entered regular expression

This is OOP realization so speed of programm can be lower than it supposed to be

Debug mode - you can create unit test using template - ("regular expression", {"first element", "second elem", ...}, deep of iteration), where deep of iteration is integer, 0 - only epsilon

P.S. Unit test can be modified in UnitTest.hpp file

Release mode - you can enter your expression and see a regular set

Operations

* - iteration from 0
^+ - iteration from 1
+ - union
(nothing) - intersect

Examples

iteration num = 3
(a+b)* = { eps, a, b, aa, ab, ba, bb, aaa, aab, aba, abb, baa, bab, bba, bbb, ... }

iteration num = 2
(a+b)*+(b+a(b+c^+)*)+(c*b+a^+)(a+b) = { eps, a, b, aa, ab, ba, bb, ac, acc, abb, abc, acb, cba, cbb, aaa, aab, abcc, accc, accb, ccba, ccbb, acccc, ... }

P.S. Sorting and adding "..." in the end is only interface friendly option
P.S.S. Please be careful with big value of iteration and ammount of iterations in regular expressions
