## Практикум по формальным языкам №2 - Кудрявцев Иван 822 группа


Earley algorithm


### TO RUN:

mkdir build

cd build

cmake ..

make

./earley_alg

cd ..

rm -r build

## To check if word is in the language
### Input:
number of terminal symbols - int
number of nonterminal symbols - int
number of rules - int
Terminals - char
Nonterminals - char
Starting nonterminal - char
Rules
Left part - char
Right part - string

### Example:

4 3 6

+ * ( )

S T F

S

S

T+S

S

T

T

F*T

T

F

(S)

F

a

(a+a)