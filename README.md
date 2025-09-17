# Deterministic Finit Automat

A Deterministic Finite Automaton (NFA) is a machine used in computer science to recognize patterns.
It’s like a game where you follow arrows between states based on the input symbols.

- If any possible path ends in an accepting state after reading the input, the NFA accepts it.

## Representations of this are:

- Node Diagram
- Table

![diagrams](./images/image%20copy.png)

## C++ Programm

Here I use the table representation for making a DFA in c++, so it can determine weather if a string is accepted by the DFA or not.

### States

- For managing states I used the table form, defining a matrix where the i index (row) is the current state and the j index (column) is the current character in the string, if at the end the current state is an acceptance one, it returns true.
- For making the [i][j] shearch eficient, I used two c++ unordered_maps (HASH MAPS). Making a hash map with the row NAMES as keys and their row INDICES as values, and antoher one where column names are keys and the column indices the values.

### Interface

Despite it being a simple terminal interface, I´ve made it as convinient as I could for editing matrices. Giving it controls for moving the cursor and reasign cel values.

### Usage

1. Fill the table with using your keyboard arrows.
2. Enter the initial state.
3. Enter the acceptance states with format [1,2,...,n].
4. Enter strings with format [1,2,...,n] and recieve the output for each.
