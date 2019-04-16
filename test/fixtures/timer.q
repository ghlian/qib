timer.job:flip `name`func`time!"s*p"$\:()          / timer jobs
timer.job,:(`;();0Wp)                              / init so that `func col doesn't change; this row will never execute (infinity)

\d .timer

merge:`time xdesc upsert                           / merge record(y) into table(x) in reverse chronological order

add:{[t;n;f;tm]                                    / add new timer (f)unction with (n)ame and (t)i(m)e into (t)able
 r:(n;f;gtime tm);
 t:merge[t;$[0h>type tm;r;reverse flip r]];
 t}

run:{[t;i;tm]                                      / run timer job at (i)ndex from (t)able and current time tm
 j:t i;
 t:.[t;();_;i];
 r:value (f:j `func),ltime tm;
 if[not null r;merge[t;(j `name;f;tm+r)]];
 t}

loop:{[t;tm]                                       / scan timer (t)able for runable jobs
  while[tm>=last tms:t `time;
        t:run[t;-1+count tms;tm]];
  t}

until:{[d;et;f;tm]                                 / (d)elay, (e)nd (t)ime, (f)unction, tm:current time
  if[tm<et;@[value;f,tm;0N!];:d]}                  / helper function to generate repeating jobs

.z.ts:loop`timer.job


/
usage:
event to print the current time every second for the next 30 seconds:
e:(`.timer.until;0D00:00:01;.z.P+0D00:00:30;`q.show)
.timer.add[`timer.job;`a;e;.z.P]