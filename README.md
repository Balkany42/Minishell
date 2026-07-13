*This project has been created as part of the 42 curriculum by mgrager, megrelli.*

# Minishell

## Description

This project's goal was to create our own shell, mimicking the behavior of the bash POSIX shell.

Like many software tools we use daily, the shell is something we often take for granted. Before this project, our understanding of the shell was limited to basic usage and simple command execution. Minishell provided us with the opportunity to uncover the mysteries of shell architecture and understand how this fundamental program operates internally.

This project encompasses many interconnected topics and allowed us to deepen our knowledge on subjects such as software architecture, system calls, formal languages and grammar, file descriptors, and teamwork.

This project required extensive documentation reading and planning. We could not have completed it without the invaluable resources available on the Internet. We wish to express our gratitude to all the contributors of these resources. They embody the open source philosophy that makes this field of computer science so captivating.

### Architecture

The main challenge of this project lays in the processing of the user's input, that we receive as a string and need to convert into a comprehensible structure from which we can derive actions.

![alt text](project_map.png)

Our ./minishell program architecture is divided into 2 main sections: user input **parsing** and **execution**. The parsing part is the analysis and formating of the user's input. The execution part is pretty self-explanatory and consists of the execution of the user's commands.

These 2 steps are performed for each of our shell lines. On top of them lies an outer layer for prompt display, input line reading and signal management.

Here are the main architecture structure regarding parsing and execution:

- **Lexer**: we divide the command line received as a simple string into comprehensive tokens (or lexems) stored in a linked list.

	A token has a type *(ex: literal, redirection_in, left_parenthesis...)* and a value *(ex: "ls", ">", "(")*.
- **Parser**: we convert our tokens' list into an **Abstract Syntax Tree**.

## Instructions

- Create a `readline.supp` file with the following content in the root directory:
```
{
    ignore_libreadline_leaks
    Memcheck:Leak
	...
    obj:*/libreadline.so.*
}
```

- Compile with valgrind:
```shell
valgrind --leak-check=full -s --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./minishell
```

- Launch `./minishell` and enjoy:
```shell
./minishell
```

## Resources

- ### Understanding the Thing
	- [**Writing Your Own Shell** (2014, Gustavo Rodriguez-Rivera and Justin Ennen)](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)
	- [**Shell Command Language** (2004, The Open Group)](https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html)
	- [**Minishell: Building a mini-bash** (2023, MannBell)](https://m4nnb3ll.medium.com/minishell-building-a-mini-bash-a-42-project-b55a10598218)

- ### Tokenization

- ### Parsing
	- [**Context Free Grammar** (Wikipedia)](https://en.wikipedia.org/wiki/Context-free_grammar)
	- [**Advances in Computers (2017, Language Theory)**](https://www.sciencedirect.com/topics/computer-science/nonterminal-symbol)
	- [**Abstract Syntax Tree** (Wikipedia)](https://en.wikipedia.org/wiki/Abstract_syntax_tree)
	- [**Recursive Descent Parser** (Wikipedia)](https://en.wikipedia.org/wiki/Recursive_descent_parser)
	- [**Crafting Interpreters** (2021, Robert Nystrom, Chapter 3: A Tree-Walk Interpreter)](https://craftinginterpreters.com/contents.html) ⭐
- ### Wildcards Pattern Matching
	- [**Wildcard Pattern Matching** (Geeks For Geeks)](https://www.geeksforgeeks.org/dsa/wildcard-pattern-matching/)
	- [VIDEO - **Wildcard Matching Dynamic Programming** (2016, Tushar Roy - Coding Made Simple)](https://www.youtube.com/watch?v=3ZDZ-N0EPV0)
- ### Expansion of wildcards and variables

- ### Execution
	- [VIDEO - **Unix Processes in C** (2020, CodeVault series of videos)](https://www.youtube.com/playlist?list=PLfqABt5AS4FkW5mOn2Tn9ZZLLDwA3kZUY) ⭐
