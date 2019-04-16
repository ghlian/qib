\l test/fixtures/timer.q
\l test/fixtures/ty.q
\l tree.q

paperCfg:{
  .tst.mock[`.utl.arg.posArgs;()];
  .tst.mock[`.utl.arg.boolOpts;()];
  .tst.mock[`.utl.arg.regOpts;()];
  .tst.mock[`.utl.arg.regDefOpts;()];
  .tst.mock[`.utl.arg.args ; (
    "--cfg";".qib";"--profile";"paper")];
  system"l cfg.q";
  }

mockCallbacks:{
    {.ib.reg[x]
      {.ib.u.oe[x;y];.tst.mock[x;y];} x;
    } each key 1_.ty;
  }
types:{
  Ty.t:@[get;`Ty.t;.tree.dict .ty];
  Ty.c:@[get;`Ty.c;.tree.child[Ty.t`p]];
  d:.tree.descend[Ty.c;] Ty.t.n?x;
  Ty.t.v d where not d in key Ty.c}

repeat:{[f;d;to]                                   / repeat (f)unction in (d)elay with (t)ime(o)ut
  e:(`.timer.until;d;.z.P+to;f);
  .timer.add[`timer.job;`repeat;e;.z.P];
  }

waitFor:{[e;d;to]                                  / wait for (e)vent
  repeat[`.ib.onReceive;d;to];
    while[
    (()~key e) & 1<count timer.job;
      .timer.loop[`timer.job;.z.p];];
  timer.job::1#timer.job;
  }[;0D00:00:00.1;0D00:00:05]                    / 1ms delay, 1s timeout

mustReturn:{[e]
  waitFor[e];
  v:e~key e;s:string e;
  .tst.asserts[`must;v;"return ",s," callback"];
  if[v;
    d:(),$[d:type g:get e;d;type each g];
    if[not d~t:types e;
      g:.tree.list[g];
      d:{.tree.path[x`p] each til count x} g;
      g@:where 3>=count each d;                    / don't drill more than two levels
      g@:where not til[count g] in distinct g`p;
      d:type each g`v;];
    s,:" type mismatch at positions: ",
      .j.j where not c:@[d=;t;count[t]#0b];
    .tst.asserts[`must;all c;s]];
  }


paperCfg[];
mockCallbacks[];
//////////////////////////////////////////////

.tst.desc["Configuration and connection"]{
  before {
    };
  should["support configuration from file"]{
    Cfg.host mustmatch `127.0.0.1;
    Cfg.port mustmatch 4002;
    Cfg.profile mustmatch `paper;
    };
  should["start disconnected"]{
    0b musteq .ib.isConnected[];
    };
  should["connect and disconnect successfully"]{
    must[;"connect"]
      .ib.connect[ Cfg.host;Cfg.port;1i];
    mustReturn[`connectAck];
    .ib.isConnected[] must "show connected status";
    -14 -19 -11h mustmatch type each
      "DTS"$" "vs .ib.TwsConnectionTime[];        / IB returns date time timezone string
   mustReturn[`managedAccounts];
   0 mustne count "," vs managedAccounts;
    148i musteq .ib.serverVersion[];
    mustReturn[`nextValidId];
    1 musteq nextValidId;

   / .ib.disconnect[];                            / TODO: test disconnect as last test
   /  (not .ib.isConnected[]) must "disconnect";
                                                  / TODO: solve connectionClosed (on server close) is followed by blocked q session
    };
  };

.tst.desc["Server, client & connection details"]{
  should["provide current time upon request"]{
    .ib.reqCurrentTime[];
    mustReturn[`currentTime];
    };
  };

.tst.desc["Error handling"]{
  should["set TWS/GW server logging level"]{
    .ib.setServerLogLevel[5];                      / The default is 2. Possible values: 1 = SYSTEM; 2 = ERROR; 3 = WARNING; 4 = INFORMATION; 5 = DETAIL
    };
  should["throw error messages with levels system,warning,error"]{
    };
  };

.tst.desc["Contract Details"]{
  before {
    fixture`syms;
    fixture`eustocks;
    };
  should["request contract details"]{
    s:first syms;
    c:enlist[`conId]!enlist 272093;
    .ib.reqContractDetails[1;s];
    mustReturn[`contractDetails];
    mustReturn[`contractDetailsEnd];
    .ib.reqContractDetails[2;c];
    mustReturn[`contractDetails];
    mustReturn[`contractDetailsEnd];
    };
  should["get market rule info"]{
    .ib.reqMarketRule[26];
    mustReturn[`marketRule];
    };
  should["search symbols"]{
    .ib.reqMatchingSymbols[1;"IBM"];
    mustReturn[`symbolSamples];
    };
  should["get exchange source of the quote"]{
    .ib.reqSmartComponents[1;"a6"];
    mustReturn[`smartComponents];
    };
  should["request bond details"]{
    };
  should["request option details"]{
    / .ib.reqSecDefOptParams[0;`IBM;"";"STK";8314];
    };
  };

.tst.desc["Real-time price data"]{
  before {
    fixture`syms;
    fixture`eustocks;
    fixture`cfd;
    };
  should["get ticks (in interval)"]{
    s:first syms;
    .ib.reqMktData[1;s;"";0b;0b];
    mustReturn[`tickPrice];
    mustReturn[`tickSize];
    mustReturn[`tickReqParams];
    .ib.cancelMktData[1];
    .ib.reqMktData[2;s;"";1b;0b];                  / snapshot
    mustReturn[`tickPrice];
    .ib.cancelMktData[2];
    .ib.reqMktData[3;s;"mdoff,292:BZ";0b;0b];      / news
    mustReturn[`tickPrice];
    .ib.cancelMktData[3];
    .ib.reqMktData[4;s;"233,236,258";0b;0b];       / RTVolume (Time & Sales), shortable and Fundamental Ratios generic ticks
    mustReturn[`tickPrice];
    .ib.cancelMktData[4];
    };
  should["get ticks"]{
    s:first syms;
    f:`Last`AllLast`BidAsk`MidPoint;
    n:10;nosz:0b;                                  / number of ticks;ignore size flag
    .ib.reqTickByTickData[5;s;f 0;n;nosz];
    mustReturn `tickByTickAllLast;
    .ib.reqTickByTickData[6;s;f 1;n;nosz];
    mustReturn `tickByTickAllLast;
    .ib.reqTickByTickData[7;s;f 2;n;nosz];
    mustReturn `tickByTickBidAsk;
    .ib.reqTickByTickData[8;s;f 3;n;nosz];
    mustReturn `tickByTickMidPoint;
    .ib.cancelTickByTickData each 5 6 7 8;
    };
  should["get realtime bars"]{
    s:last syms;bs:5;f:`MIDPOINT;rth:0b;           / contract, bar size,field,regular trading hrs;
    .ib.reqRealTimeBars[5;s;bs;f;rth;()!()];
    mustReturn[`realtimeBar];
    .ib.cancelRealTimeBars[5];
    };
  should["get option market data"]{
    fixture`fop;                                   / futures on options
    o:first fop;
    / .ib.reqMktData[1;o;"";0b;0b];                  / TODO: debug options reqMktData + tickOptionCalculation
    / .ib.cancelMktData[1];
    };
  should["get delayed-frozen market data"]{
    s:first syms;
    .ib.reqMarketDataType[4];                      / TODO: marketDataType callback does not work
    .ib.reqMktData[5;s;"";0b;0b];
    .ib.cancelMktData[5];
    };
  should["get market depth"]{
    fx:exec from syms where symbol=`EUR;
    s:first eustocks;
    sd:01b; nr:5;                                  / isSmartDepth options; #rows
    .ib.reqMktDepth[1;fx;nr;sd 0;()!()];           / TODO: MktDepth
    .ib.cancelMktDepth[1;sd 0];
    .ib.reqMktDepth[1;s;nr;sd 1;()!()];
    .ib.cancelMktDepth[1;sd 1];
    .ib.reqMktDepthExchanges[];
    mustReturn[`mktDepthExchanges];
    };
  should["reroute CFD requests"]{
    c:first cfd;
    .ib.reqMktData[6;c;"";0b;0b];
    mustReturn[`rerouteMktDataReq];
    .ib.cancelMktData[6];
    sd:01b; nr:5;                                  / isSmartDepth options; #rows
    .ib.reqMktDepth[7;c;nr;sd 0;()!()];
    mustReturn[`rerouteMktDepthReq];
    .ib.cancelMktDepth[7;sd 0];
    };
  };

.tst.desc["Historical price data"]{
  before {
    fixture`syms;
    fixture`eustocks;
    fixture`cfd;
    };
  should["get historical ticks"]{
    s:first syms;
    ti:2017.06.21D09:38:33; lu:01b;                / time; lower or upper bound
    n:10;f:`TRADES`MIDPOINT`BID_ASK;               / number of ticks (max 1k); field
    rth:1b;nosz:1b;                                / regular trading hrs;ignore size ticks;
    .ib.reqHistoricalTicks[10;s;ti;lu 0;n;f 0;rth;nosz];
    mustReturn[`historicalTicksLast];
    .ib.reqHistoricalTicks[11;s;ti;lu 0;n;f 1;rth;nosz];
    mustReturn[`historicalTicks];
    .ib.reqHistoricalTicks[12;s;ti;lu 0;n;f 2;rth;nosz];
    mustReturn[`historicalTicksBidAsk];
    };
  should["get historical bars"]{
    f:`TRADES`MIDPOINT;
    dur:"1 ",/:"SDWMY";                            / period
    s:last syms;rth:0b;                            / contract,regular trading hrs
    e:"p"$2018.11.15;bs:"1 day";                   / end date;bar size
    .ib.reqHeadTimestamp[6;s;f 1;rth];             / TODO: reqHeadTimestamp formatDate
    mustReturn[`headTimestamp];                    / TODO: headTimestamp not returned
    .ib.cancelHeadTimestamp[6];
    .ib.reqHistoricalData[7;s;e;dur 2;bs;f 1;rth;0b];
    mustReturn[`historicalData];
    mustReturn[`historicalDataEnd];
    .ib.cancelHistoricalData[7];
    .ib.reqHistoricalData[8;s;0Np;"1 D";"8 hours";f 1;rth;1b]; / uncomment when realtime access
    mustReturn[`historicalDataUpdate];
    .ib.cancelHistoricalData[8];
    };
  should["get histogram data"]{
    s:first syms;rth:1b;p:"3 days";
    .ib.reqHistogramData[8;s;rth;p];
    mustReturn[`histogramData];
    .ib.cancelHistogramData[8];
    };
  };

.tst.desc["Fundamental data"]{
  before {
    fixture`syms;
    };
  should["get fundamental data"]{
    s:first syms;
    rty:`ReportSnapshot`ReportsFinSummary`ReportRatios,
      `ReportsFinStatements`RESC`CalendarReport;
    .ib.reqFundamentalData[8;s;rty 0;()!()];
    mustReturn[`fundamentalData];
    };
  };

.tst.desc["Scanner"]{
  before{
    {.ib.reg[x] .tst.mock[x;]}`scannerParameters;  / custom callback without printing large xml;
    };
  should["get scanner parameters"]{
    .ib.reqScannerParameters[];
    mustReturn[`scannerParameters];
    };
  should["get scanner data"]{
    s:`instrument`locationCode`scanCode!
      (`IND.US;"IND.US";"HIGH_OPT_VOLUME_PUT_CALL_RATIO");
    f:`usdMarketCapAbove`optVolumeAbove`avgVolumeAbove!
       10000 1000 100000000;
    / .ib.reqScannerSubscription[1;s;()!();f];       / TODO: scanner subscription
    / mustReturn[`scannerData];
    / mustReturn[`scannerDataEnd];
    / .ib.cancelScannerSubscription[1];
    };
  };

.tst.desc["Get account data"]{
  before {
    .ib.reqManagedAccts[];
    waitFor[`managedAccounts];
    `a mock first `$csv vs managedAccounts;
    `m mock ``Core;                                / possible model codes
    };
  should["request the accounts to which",
         "the logged user has access to"]{
    mustReturn[`managedAccounts];
    };
  should["get summary"]{
   / .ib.reqAccountSummary[1;"all";"$LEDGER:EUR"];  / TODO: reqAccountSummary nyi
    /  .ib.cancelAccountSummary[1];
    };
  should["get updates"]{
    .ib.reqAccountUpdates[1b;a];                   / subscribe
    mustReturn[`updateAccountValue];
    mustReturn[`updateAccountTime];
    mustReturn[`accountDownloadEnd];
    .ib.reqAccountUpdates[0b;a];                   / unsubscribe
    };
  should["get updates for multi accounts/models"]{
    .ib.reqAccountUpdatesMulti[1;a;m 0;1b];
    mustReturn[`accountUpdateMulti];
    mustReturn[`accountUpdateMultiEnd];
    .ib.cancelAccountUpdatesMulti[1];
    };
  should["get positions"]{
    .ib.reqPositions[];
    mustReturn[`positionEnd];
    .ib.cancelPositions[];
    .ib.reqPositionsMulti[1;a;m 0];
    mustReturn[`positionMultiEnd];
    .ib.cancelPositionsMulti[1];
    };
  should["get pnl"]{
    .ib.reqPnL[1;a;m 0];                           / TODO: test pnl callback after orders
    mustReturn[`pnl];
    .ib.cancelPnL[1];
   / .ib.reqPnLSingle[2;a;m 0;] 12345;             / contract must be in the account
   / mustReturn[`pnlSingle];                       / TODO: test pnlSingle callback after orders
   / .ib.cancelPnLSingle[2];
    };
  };

.tst.desc["Orders"]{                               / TODO: orders
  / .ib.reqAllOpenOrders[];
  / lmtOrder:`action`totalQuantity`orderType`lmtPrice!(`BUY;1000;`LMT;0.01);
  / mktOrder:`action`totalQuantity`orderType!(`BUY;1000;`MKT);
 / .ib.placeOrder[1^nextValidId;first con] lmtOrder;
  };

.tst.desc["Execution"]{                            / TODO: execution
  / .ib.reqExecutions[first 1?100;
  /   enlist[`acctCode]!enlist a];
  };

.tst.desc["News data"]{                            / TODO: news
  before{
    fixture`news;
    };
  should["get news providers subscription info"]{
    / .ib.reqNewsProviders[];
    / mustReturn`newsProviders;
    };
  should["get real time contract-specific news"]{
    };
  should["get realtime broad news"]{
    / .ib.reqMktData[10;n 0;"mdoff,292",0b;0b;()!()];
    / mustReturn`tickNews;
    };
  should["get historical news"]{
    /  o:enlist[`manual]!enlist"1";
    / .ib.reqHistoricalNews[
    /   1200;8314;"BZ+FLY";"";"";5;o];
    / mustReturn`historicalNews;
    / mustReturn`historicalNesEnd;
    / .ib.reqNewsArticle[1300;"MST";"MST$06f53098";o];
    / mustReturn`newsArticle;
    };
  };

.tst.desc["Financial Advisors"]{                   / TODO: financial advisors
  should["get family codes"]{
    / .ib.reqFamilyCodes[];
    / mustReturn`familyCodes;
    };
  should["configure allocation"]{
    / .ib.requestFA[];
    / mustReturn `receiveFA;
    / .ib.replaceFA[];
    };
  should["place order to a model"]{
    / https://interactivebrokers.github.io/tws-api/model_portfolios.html
    };
  should["place orders to FA account"]{
    / https://interactivebrokers.github.io/tws-api/financial_advisor_methods_and_orders.html
    };
  };

.tst.desc["Display Groups"]{                       / TODO: display groups
  should["query"]{
    / .ib.queryDisplayGroups[];
    / mustReturn`displayGroupList;
    };
  should["update"]{
    / .ib.subscribeToGroupEvents[];
    / mustReturn`displayGroupUpdated;
    / .ib.updateDisplayGroup[];
    / .ib.unsubscribeFromGroupEvents[];
    };
  };

.tst.desc["IB News Bulletins"]{
  should["get IB News Bulletins"]{
    / .ib.reqNewsBulletins[1b];
    / mustReturn`updateNewsBulletin;
    / .ib.cancelNewsBulletins[];
    };
  };