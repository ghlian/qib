//////////////// load configuration and format it into e.g. Cfg.paper.host
.utl.require"qutil/config_parse.q";
.utl.require"qutil/opts.q";
\l tree.q

.tree.makeNs:{[tree]                               / format tree as namespace compatible
  tree:update `$n from tree;                       / names must be symbols
  b:1_distinct tree`p;                             / branches
  ns:enlist[`]!enlist[::];                         / empty namespace
  .[tree;(b;`v);:;count[b]#enlist ns]}             / turn branches into namespaces

.cfg.ty:`host`port`callbacks!"SJ*"                 / config params datatypes
.cfg.load:{[file]                                  / returns Cfg namespace loaded from file
  c:.utl.parseConfig hsym file;
  c:.tree.makeNs .tree.dir c;
  c:update v:$'["*"^.cfg.ty n; v] from c
    where not i in distinct p;
  .tree.nest[c]}

.utl.addOptDef["profile";"S";`paper;`Cfg.profile]; / profile as section of config file; supplied via --profile option
.utl.addOpt["cfg";"S"; (`Cfg.raw;.cfg.load)];      / load Cfg namespace from file supplied in --cfg option
.utl.parseArgs[];                                  / parse above arguments supplied via command line

Cfg,:Cfg.raw Cfg.profile;
Cfg _:`raw;

{                                                  / load callback definitions from separate file(s)
  if[not ()~key`Cfg.load;
    {system"l ",x}each " " vs Cfg.load;];
  }[]
//////////////// End of configuration loading ////////////////
