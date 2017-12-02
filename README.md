# cdecl

Cdecl is a program which will turn a c-language declaration into English-like phrases.

Unfortunately,it is only a very simple and incomplete implementation of cdecl ：(

Now it can only do as below:
```
$ ./simple_cdecl

int (*f)()

Declared f as pointer to function returning int

int (*(*foo)(void ))[3]

Declared foo as pointer to function (void) returning pointer to array 3 of int

char (*p)[29]

Declared p as pointer to array 29 of char
```

To learn more, goto the website [https://cdecl.org/](https://cdecl.org/).
