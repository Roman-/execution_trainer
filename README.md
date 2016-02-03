# execution_trainer
The basic idea is to generate a chain of threecycles such that after executing them on solved cube, the cube remain solved again. The restrictions here are: the chain can't contain similar threecycles (otherwise trivial cases like "AB AB AB" could occur) or each-other-inverse threecycles (prevent cases like "AB CD DC BA").

# Hotkeys
Spacebar: generate chan
Arrows: navigation
Return: change difficulty for current letter-pair
S: save difficulty changes and clear screen
X: x-centers or obliques
T: t-centers
W: wings
E: edges
C: corners
L: change chain Length
+: increase font size
-: decrease font size
Esc: clear layout without saving; If layuot is already clean then exit

# settings file
current element
current length
font size