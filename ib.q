///////////////  Utilities  /////////////////

\d .ib
u.o:{-1 string[.z.Z]," ",x;}                       / output timestamped x string

u.zu:{"z"$-10957+x%8.64e4}                         / kdb+ datetime from unix
u.fmt:{ssr[ssr[;"\"";""] .j.j x;",";", "]}         / format
u.oe:{u.o string[x],":",u.fmt y}
\d .

///////////////  End of Utilities  ////////


.ib,:(`:./bin/qib.0.0.2 2:(`LoadLibrary;1))`       / requests
.ib.cb:()!();                                      / callbacks function!code

\d .ib

cb[`unknown]:{[fname;args]                         / unknown callback handler
  u.o" unknown function ",string[fname],           / print fname and args
     ", args:\n", args;
  }

onrecv:{[fname;args]                               / on receive call fname
  f:$[null f:cb fname;cb`unknown;f];
  a:(::;enlist) type[args] in 0 10 98 99h;
  value enlist[f],a args}

reg:{[fname;code] @[`.ib.cb;fname;:;code];}        / register   callback fname
dreg:{[fname] cb _::fname;}                        / deregister callback fname
\d .