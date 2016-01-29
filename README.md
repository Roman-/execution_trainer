# execution_trainer
The basic idea is to generate a chain of threecycles such that after executing them on solved cube, the cube remain solved again. The restrictions here are: the chain can't contain similar threecycles (otherwise trivial cases like "AB AB AB" could occur) or each-other-inverse threecycles (prevent cases like "AB CD DC BA")
