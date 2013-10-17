Play spec

PLAY playname

[APPLICABLE [!]<predicate> [!]<predicate> ...]
... (each line is combined with an or operator)
[APPLICABLE [!]<predicate> [!]<predicate> ...]

[DONE {failure | success} [!]<predicate> ...]
... (each line is combined with an or operator)
[DONE {failure | success} [!]<predicate> ...]

[FIXEDROLES 1 3 4 2]

ROLE 1
	tactic parameters
	none
ROLE 2
	tactic parameters
	none
...

Parameters:


Tcoordinates

[{] [[origin type] side type [d]] coordx coordy   [}]

origin type:
none   : world origin
A      : absolute
B      : ball

side type:
null	: absolute
B	: ball (direction of ball is positive)
A	: Absolute
S	: strong side of opponent team (side they have most robots)
O	: ball side unless ball is in middle then it is S side

optional
d	: dynamic: gets recomputed each frame 

TRegion:
[{] {R {Tcoord1 Tcoord2 width}  | C {Tcoord_center radius} } 

R   : rectangle
width is width of rectangle where length is along Tcoord1 to Tcoord2

