\d .ty0

what:`TRADES`MIDPOINT`BID`ASK`BID_ASK,
  `HISTORICAL_VOLATILITY`,
  `OPTION_IMPLIED_VOLATILITY`FEE_RATE`REBATE_RATE;

contract:(!) . flip (
  (`cid;-7h);
  (`syml;-11h);
  (`ex;-11h);                                      / exchange destination
  (`ex0;-11h);                                     / exchange primary
  (`ccy;-11h);
  (`sym;-11h);
  (`sty;-11h);                                     / security type
  (`class;-11h);
  (`dlast;10h);                                    / last trade date or contract month
  (`strike;-9h);
  (`right;-11h);
  (`mult;10h);
  (`iexp;-1h);                                     / include expired
  (`sidty;-11h);
  (`sid;-11h))

conDetails:(!) . flip (
  (`mkt;10h);
  (`tck;-9h);
  (`oty;10h);
  (`exv;10h);                                      / valid exchanges
  (`pmult;-7h);
  (`under;-7h);                                    / underlying id
  (`long;10h);                                     / long name
  (`month;10h);
  (`industry;10h);
  (`cat;10h);
  (`cat2;10h);
  (`tz;10h);
  (`hrs;10h);                                      / trading hours
  (`hrsl;10h);                                     / liquid hours
  (`ev;10h);                                       / economic value rule
  (`evmult;-9h);
  (`mrule;10h))                                    / market rule ids (csv)
tick:(!) . flip(
  (`auto;-1h);                                     / can auto-execute
  (`pastLmt;-1h);                                  / bid(ask) price <(>) day's min(max)bid(ask)?
  (`preOp;-1h))                                    / pre-open session?
tickR:(!) . flip(
  (`rid;-7h);
  (`done;-1h))
tickAttr:(!) . flip(
  (`pastLmt;1h);
  (`unrep;1h);                                    / unreported
  (`ex;11h);
  (`cond;0h);
  (`islo;1h);
  (`ishi;1h))
tickM:(!) . flip(
  (`ti;12h);
  (`px;9h);
  (`sz;7h))
tickT:tickM,`pastLmt`unrep`ex`cond#tickAttr
tickQ:(!) . flip(
  (`ti;12h);
  (`bid;9h);
  (`ask;9h);
  (`bsz;7h);
  (`asz;7h))
tickQ:tickQ,`islo`ishi#tickAttr
bar:(!) . flip (
  (`rid;-7h);
  (`ts;-12h);
  (`op;-9h);
  (`hi;-9h);
  (`lo;-9h);
  (`cl;-9h);
  (`vol;-7h);
  (`wap;-9h);
  (`cnt;-6h))
\d .ty

connectAck:101h
connectionClosed:101h
disconnected:101h
currentTime:-12h
nextValidId:-7h
managedAccounts:10h
.ty.system:error:warning:(!). flip (
  (`rid;-7h);
  (`code;-7h);
  (`msg;10h))
winError:(!). flip ((`msg;10h);(`code;-7h))
marketDataType:(!). flip ((`rid;-7h);(`mdty;-6h))
contractDetails:(!) . flip (
  (`rid;-7h);
  (`con;.ty0.contract);
  (`conDetails;.ty0.conDetails))
contractDetailsEnd:-7h
/ bondContractDetails:
/ securityDefinitionOptionalParameter
symbolSamples:(!) . flip (
  (`rid;7h);
  (`cid;7h);
  (`sym;11h);
  (`syml;11h);                                     / symbol local to the exchange; unused?
  (`sty;11h);
  (`ex;11h);                                       / destination exchange; unused?
  (`ex0;11h);
  (`ccy;11h);
  (`dty;0h))                                       / derivative security types
smartComponents:(!) . flip (
  (`rid;7h);
  (`eid;7h);                                       / exchange bit number
  (`ex;11h);                                       / exchange
  (`exc;10h))                                      / exchange letter
marketRule:(!) . flip (
  (`mrule;7h);
  (`px;9h);
  (`tck;9h))
tickBase:(!) . flip (
  (`rid;-7h);
  (`ty;-6h))
tickPrice:tickBase,(!) . flip (
  (`px;-9h);
  (`tick;.ty0.tick))
tickSize:tickBase,(!) . flip (enlist (`sz;-7h))
tickGeneric:tickBase,(!) . flip (enlist (`val;-9h))
tickString:tickBase,(!) . flip (enlist (`val;10h))
tickEFP:tickBase,(!) . flip (
  (`bp;-9h);
  (`bp0;10h);
  (`d2ltd;-6h);
  (`futltd;10h);
  (`dv;-9h);
  (`dvImp;-9h);
  (`dv2ltd;-9h))
tickOptionComputation:tickBase,(!) . flip (
  (`ivol;-9h);
  (`delta;-9h);
  (`px;-9h);
  (`dvpv;-9h);
  (`gamma;-9h);
  (`vega;-9h);
  (`theta;-9h);
  (`pxunder;-9h))
tickSnapshotEnd:-6h
tickReqParams:(!) . flip (
  (`rid;-7h);
  (`tck;-9h);
  (`ex;-11h);
  (`perss;-6h))
tickByTickMidPoint:`done`sz _ .ty0.tickR,
  neg .ty0.tickM
tickByTickAllLast:`done _ .ty0.tickR,
  neg .ty0.tickT;tickByTickAllLast[`cond]:10h;
tickByTickBidAsk:`done _ .ty0.tickR,
  neg .ty0.tickQ
realtimeBar:.ty0.bar
rerouteMktDataReq:(!) . flip (
  (`rid;-7h);
  (`cid;-7h);
  (`ex;-11h))
headTimestamp:(!) . flip (
  (`rid;-7h);
  (`ts;-12h))
historicalTicks:.ty0.tickR,(!) . flip(
  enlist (`tck;.ty0.tickM))
historicalTicksLast:.ty0.tickR,(!) . flip(
  enlist (`tck;.ty0.tickT))
historicalTicksBidAsk:.ty0.tickR,(!) . flip(
  enlist (`tck;.ty0.tickQ))
historicalData:.ty0.bar,enlist[`ts]!enlist[10h]
historicalDataUpdate:historicalData
historicalDataEnd:(!) . flip (
  (`rid;-7h);
  (`fr;10h);
  (`to;10h))
histogramData:(!) . flip (
  (`rid;-7h);
  (`hgram;(!) . flip (
      (`px;9h);
      (`sz;7h))))
updateMktDepth:(!) . flip (
  (`rid;-7h);
  (`pos;-7h);
  (`rowop;-6h);
  (`side;-6h);
  (`px;-9h);
  (`sz;-6h))
updateMktDepth2:updateMktDepth,(!) . flip (
  (`mm;-11h);
  (`smart;-1h))
rerouteMktDepthReq:rerouteMktDataReq
mktDepthExchanges:(!) . flip (
  (`ex;11h);
  (`sty;11h);
  (`exl;11h);
  (`sdt;11h);
  (`grp;7h))
fundamentalData:(!) . flip (
  (`rid;-7h);
  (`xml;10h))
scannerParameters:10h;
scannerData:101h
scannerDataEnd:-7h
accountBase:(!) . flip (
  (`acc;-11h);
  (`key;-11h);
  (`val;10h);
  (`ccy;-11h))
accountSummary:,[;accountBase] (!) . flip (
  enlist (`rid;-6h))
accountSummaryEnd:-6h
updateAccountValue:accountBase
accountDownloadEnd:-11h
updateAccountTime:10h
accountUpdateMulti:accountSummary,(!) . flip (
  enlist (`model;-11h))
accountUpdateMultiEnd:-6h
positionBase:(!) . flip (
  (`acc;-11h);
  (`pos;-7h);
  (`costAvg;-9h))
position:positionBase,(!) . flip (
  enlist (`con;.ty0.contract))
positionEnd:101h
positionMulti:((!) . flip (
    (`rid;-6h);
    (`model;-11h))),
    positionBase,
    enlist[`con]!enlist .ty0.contract
positionMultiEnd:-6h
updatePortfolio:positionBase,(!) . flip (
  (`px;-9h);
  (`mv;-9h);
  (`pnlu;-9h);
  (`pnlr;-9h);
  (`con;.ty0.contract))
pnl:(!) . flip (
  (`rid;-7h);
  (`pnld;-9h);
  (`pnlu;-9h);
  (`pnlr;-9h))
pnlSingle:pnl,(!) . flip (
  (`pos;-7h);
  (`mv;-9h))
/
//// nyi /////
newsProviders:
tickNews:
newsArticle:
historicalNews:
historicalNewsEnd:
openOrder:
openOrderEnd:
orderStatus:
orderBound:
softDollarTiers: / only for registered professional advisors, hedge funds and mutual funds
execDetails:
execDetailsEnd:
commissionReport:
deltaNeutralValidation:
receiveFA:
familyCodes:
displayGroupList:
displayGroupUpdated:
displayGroupList:
updateNewsBulletin:
verifyMessageAPI:
verifyCompleted:
verifyAndAuthMessageAPI:
verifyAndAuthCompleted: