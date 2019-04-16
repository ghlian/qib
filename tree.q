\d .tree
child:{[parent] group parent}                      / (parentid!childrenids) from parent vector
parent:{[child]                                    / parent vector from (parentid!childrenids)
  @[raze child;value child;:;key child]}
descend:{[child;node] (raze/) child scan node}     / all descendants of the node (children of children)

unnest:{[parser;tree]                              / nested data structure into (parent id;name;value) table
  e:{@\:[x y;y]} parser;                           / eval node (leaf or branch) into (d;n;v)i using supplied parser
  lvl:{x each raze y[;0]} e;                       / evaluate every descendant at current level
  r:raze lvl\[enlist e tree];                      / flattened scan of all the levels of the tree
  ([]p:0N,where count each r[;0];                  / parent vector: think 0 0 0 1 1 3 3 3 3~where 3 2 0 4
     n:$[neg abs type n;" "],n:raze r[;1];         / names of nodes (branches or leaves); root name is typed " "
     v:r[;2])}                                     / values stored in nodes

path:{1_reverse x scan y};                         / path from root to y traversing the x parent vector

nest:{[tree]                                       / nested structure from table representing tree hierarchy
  p:1_'tree[`n] path[tree`p] each til count tree;  / paths except first (root)
  .[;;:;]/[ first tree`v ; p ; tree`v ]}           / apply values over paths, starting from the root

\d .pars                                           / predefined parsers; input is a tree node; output a list of functions
emptyDir:{![1#0#key x;enlist(::)]};                / empty namespace with keys of same type as key x and any (::) values
leaf:{(();();x)};                                  / default parser for leaf node
branch.list:(::;til count@;#[;()]count@)           / 0h type list branch node parsed into (members;indices;placeholder)
branch.dir:(value;key;emptyDir)                    / directory branch node parsed into (members;names;placeholder)
branch.dict:(value;key;()!())                      / dictionary branch node
as.nest:{$[type[z]=y;branch[x];leaf]}              / parser dependent on type;
nest.list:as.nest[`list;0h]                        / nested list parser
nest.dir:as.nest[`dir;99h]                         / nested directory/namespace parser
nest.dict:as.nest[`dict;99h]                       / nested dictionary parser
/ nested dictionary or general list parser:
nest.t099:(branch.list;branch.dict;leaf)0 99h?type@
\d .

.tree.dir:.tree.unnest .pars.nest.dir              / unnest nested directories - flatten into (parent;name;value) table
.tree.list:.tree.unnest .pars.nest.list            / unnest nested lists - flatten into (parent;name;value) table
.tree.dict:.tree.unnest .pars.nest.dict            / unnest nested dictionaries
.tree.t099:.tree.unnest .pars.nest.t099            / unnest nested dictionaries or general lists