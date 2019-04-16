#include "qib.h"
#include "IBClient.h"

IBClient *ib;

__attribute__((constructor))
ZV initialize_api()
{
  ib = new IBClient();
  setm(1);
}

__attribute__((destructor))
ZV destroy_api()
{
  if (ib) {
    ib->disconnect();
    delete ib;
  }
}

K LoadLibrary(K ignore)
{
  auto dict = createDictionary(std::map<std::string, K> {
      { "connect",             dl((V*) connect_,             3) },
      { "disconnect",          dl((V*) disconnect,           1) },
      { "onReceive",           dl((V*) onReceive,            1) },
      { "isConnected",         dl((V*) isConnected,          1) },
      { "serverVersion",       dl((V*) serverVersion,        1) },
      { "TwsConnectionTime",   dl((V*) TwsConnectionTime,    1) },
      { "setServerLogLevel",   dl((V*) setServerLogLevel,    1) },
      { "reqIds",              dl((V*) reqIds,               1) },
      { "reqCurrentTime",      dl((V*) reqCurrentTime,       1) },
      { "reqContractDetails",  dl((V*) reqContractDetails,   2) },
      { "reqMatchingSymbols",  dl((V*) reqMatchingSymbols,   2) },
      { "reqSmartComponents",  dl((V*) reqSmartComponents,   2) },
      { "reqMarketRule",       dl((V*) reqMarketRule,        1) },
      { "reqMarketDataType",   dl((V*) reqMarketDataType,    1) },
      { "reqMktData",          dl((V*) reqMktData,           5) },
      { "cancelMktData",       dl((V*) cancelMktData,        1) },
      { "reqTickByTickData",   dl((V*) reqTickByTickData,    5) },
      { "cancelTickByTickData",dl((V*) cancelTickByTickData, 1) },
      { "reqRealTimeBars",     dl((V*) reqRealTimeBars,      6) },
      { "cancelRealTimeBars",  dl((V*) cancelRealTimeBars,   1) },
      { "reqHeadTimestamp",    dl((V*) reqHeadTimestamp,     4) },
      { "cancelHeadTimestamp", dl((V*) cancelHeadTimestamp,  1) },
      { "reqHistoricalTicks",  dl((V*) reqHistoricalTicks,   8) },
      { "reqHistoricalData",   dl((V*) reqHistoricalData,    8) },
      { "cancelHistoricalData",dl((V*) cancelHistoricalData, 1) },
      { "reqHistogramData",    dl((V*) reqHistogramData,     4) },
      { "cancelHistogramData", dl((V*) cancelHistogramData,  1) },
      { "reqMktDepth",         dl((V*) reqMktDepth,          5) },
      { "cancelMktDepth",      dl((V*) cancelMktDepth,       2) },
      { "reqMktDepthExchanges",dl((V*) reqMktDepthExchanges, 1) },
      { "reqFundamentalData",  dl((V*) reqFundamentalData,   4) },
      { "cancelFundamentalData",dl((V*) cancelFundamentalData, 1)},
      { "reqScannerParameters", dl((V*) reqScannerParameters, 1) },
      { "reqScannerSubscription",dl((V*) reqScannerSubscription, 4) },
      { "cancelScannerSubscription",dl((V*) cancelScannerSubscription, 1) },
      { "reqManagedAccts",     dl((V*) reqManagedAccts,      1) },
      { "reqAccountSummary"  , dl((V*) reqAccountSummary,    3) },
      { "cancelAccountSummary",dl((V*) cancelAccountSummary, 1) },
      { "reqAccountUpdates",   dl((V*) reqAccountUpdates,    2) },
      { "reqAccountUpdatesMulti",dl((V*) reqAccountUpdatesMulti,    4) },
      { "cancelAccountUpdatesMulti",dl((V*) cancelAccountUpdatesMulti,1) },
      { "reqPositions",        dl((V*) reqPositions,         1) },
      { "cancelPositions",     dl((V*) cancelPositions,      1) },
      { "reqPositionsMulti",   dl((V*) reqPositionsMulti,    3) },
      { "cancelPositionsMulti",dl((V*) cancelPositionsMulti, 1) },
      { "reqPnL",              dl((V*) reqPnL,               3) },
      { "cancelPnL",           dl((V*) cancelPnL,            1) },
      { "reqPnLSingle",        dl((V*) reqPnLSingle,         4) },
      { "cancelPnLSingle",     dl((V*) cancelPnLSingle,      1) }
    });
  R dict;
}

K eventLoop(I fd) {ib->onReceive();R (K)0;}
K onReceive(K ignore) {ib->onReceive();R (K)0;}

K connect_(K host, K port, K clientId) {
  Q(host->t != -KS || port->t != -KJ, "type");
  if (!ib->isConnected()) {
    ib->connect(host->s, port->j, clientId->i);
    if (ib->isConnected()){
      sd1(ib->fd(), eventLoop);
      ib->startReader();
      // ib->onReceive();
    }
  }
  R kb(ib->isConnected());}

K disconnect(K ignore) {
  ib->disconnect();
  R (K)0;}

K isConnected(K ignore){R kb(ib->isConnected());}
K serverVersion(K ignore){R ki(ib->serverVersion());}
K TwsConnectionTime(K ignore){R kip(ib->TwsConnectionTime());}
K setServerLogLevel(K level){
  Q(level->t != -KJ, "type");
  ib->setServerLogLevel(level->j);
  R (K)0;}
K reqIds(K numIds){
  Q(numIds->t != -KJ, "type");
  ib->reqIds(numIds->j);
  R (K)0;}
K reqCurrentTime(K ignore) {ib->reqCurrentTime();R (K)0;}
K reqContractDetails(K reqId, K contract) {
  Q(reqId->t != -KJ || contract->t != XD, "type");
  std::string error;
  auto c = createContract(contract, error);
  Q(!error.empty(), error.c_str());
  ib->reqContractDetails(reqId->j, c);
  R (K)0;}
K reqMatchingSymbols(K reqId, K pattern){
  Q(reqId->t != -KJ || pattern->t != KC , "type");
  ib->reqMatchingSymbols(reqId->j,getString(pattern));
  R (K)0;}
K reqSmartComponents(K reqId,K exchange){
  Q(reqId->t != -KJ || exchange->t != KC, "type");
  ib->reqSmartComponents(reqId->j, getString(exchange));
  R (K)0;}
K reqMarketRule(K ruleId) {
  Q(ruleId->t != -KJ, "type");
  ib->reqMarketRule(ruleId->j);
  R (K)0;}
K reqMarketDataType(K marketDataType) {
  Q(marketDataType->t != -KJ, "type");
  ib->reqMarketDataType(marketDataType->j);
  R (K)0;}
K reqMktData(K tickerId,
             K contract,
             K genericTicks,
             K snapsnot,
             K regSnapshot){
  Q(tickerId->t != -KJ || contract->t != XD || genericTicks->t != KC || snapsnot->t != -KB || regSnapshot->t != -KB, "type");
  Q(!ib->isConnected(), "connection");
  std::string error;
  auto c = createContract(contract, error);
  Q(!error.empty(), error.c_str());
  TagValueListSPtr tag;
  // TODO: genericTicks translation
  ib->reqMktData(tickerId->j, c, "", static_cast<I>(snapsnot->g),
                 static_cast<I>(regSnapshot->g),
                 tag);
  R (K)0;}
K cancelMktData(K id) {
  Q(id->t != -KJ, "type");
  ib->cancelMktData(id->j);
  R (K)0;}
K reqTickByTickData(K id,
                    K contract,
                    K tickType,
                    K nticks,
                    K ignoreSize){
  Q(id->t != -KJ || contract->t != XD ||
    tickType->t != -KS || nticks->t != -KJ ||
    ignoreSize->t != -KB, "type");
  std::string error;
  auto c = createContract(contract, error);
  Q(!error.empty(), error.c_str());
  ib->reqTickByTickData(id->j,
                        c,
                        tickType->s,
                        nticks->j,
                        static_cast<bool>(ignoreSize->g));
  R (K)0;}
K cancelTickByTickData(K id){
  Q(id->t != -KJ, "type");
  ib->cancelTickByTickData(id->j);
  R (K)0;}

K reqRealTimeBars(K id,
                  K contract,
                  K barSize,
                  K whatToShow,
                  K useRTH,
                  K realTimeBarsOptions) {
  Q(id->t != -KJ || contract->t != XD || barSize->t != -KJ ||
    whatToShow->t != -KS || useRTH->t != -KB ||
    realTimeBarsOptions->t != XD, "type");
  std::string error;
  auto c = createContract(contract, error);
  Q(!error.empty(), error.c_str());
  auto tvl = createTagValueList(realTimeBarsOptions, error);
  Q(!error.empty(), error.c_str());
  ib->reqRealTimeBars(id->j,
                      c,
                      barSize->j,
                      whatToShow->s,
                      static_cast<I>(useRTH->g),
                      tvl);
  R (K)0;}
K cancelRealTimeBars(K tickerId) {
  Q(tickerId->t != -KJ, "type");
  ib->cancelRealTimeBars(tickerId->j);
  R (K)0;}
K reqHeadTimestamp(K id,
                   K contract,
                   K whatToShow,
                   K useRTH){
  Q(id->t != -KJ || contract->t != XD || whatToShow->t != -KS ||
    useRTH->t != -KB, "type");
  std::string error;
  auto c = createContract(contract, error);
  Q(!error.empty(), error.c_str());
  ib->reqHeadTimestamp(id->j,
                       c,
                       whatToShow->s,
                       static_cast<I>(useRTH->g),
                       2);
  R (K)0;}
K cancelHeadTimestamp(K id) {
  Q(id->t != -KJ, "type");
  ib->cancelRealTimeBars(id->j);
  R (K)0;}
K reqHistoricalTicks(K id,
                     K contract,
                     K time,
                     K lowerUpper,
                     K nticks,
                     K whatToShow,
                     K useRTH,
                     K ignoreSize){
  // Q(id->t != -KJ || contract->t != XD || start->t != -KP ||
  //   end->t != -KP || nticks->t != -KJ || whatToShow->t != -KS ||
  //   useRTH->t != -KB || ignoreSize->t != -KB || options->t != XD, "type");
  std::string error;
  auto c = createContract(contract, error);
  Q(!error.empty(), error.c_str());
  std::string from = static_cast<bool>(lowerUpper->g) ?
    "" : formatTime("%Y%m%d %H:%M:%S", up(time->j),0);
  std::string to = static_cast<bool>(lowerUpper->g) ?
    formatTime("%Y%m%d %H:%M:%S", up(time->j),0) : "";
  ib->reqHistoricalTicks(id->j,
                         c,
                         from,
                         to,
                         nticks->j,
                         whatToShow->s,
                         static_cast<I>(useRTH->g),
                         static_cast<bool>(ignoreSize->g),
                         TagValueListSPtr());
  R (K)0;}
K reqHistoricalData(K id,
                    K contract,
                    K endDateTime,
                    K durationStr,
                    K barSizeSetting,
                    K whatToShow,
                    K useRTH,
                    // K formatDate,
                    K keepUpToDate){
  Q(id->t != -KJ || contract->t != XD || endDateTime->t != -KP ||
    durationStr->t != KC || barSizeSetting->t != KC ||
    whatToShow->t != -KS || useRTH->t != -KB ||
    keepUpToDate->t != -KB, "type");
  std::string error;
  auto c = createContract(contract, error);
  Q(!error.empty(), error.c_str());
  TagValueListSPtr chartOptions;
  int upd = static_cast<I>(keepUpToDate->g);
  const std::string end = upd ?
    "" : formatTime("%Y%m%d %H:%M:%S GMT",up(endDateTime->j),0);
  ib->reqHistoricalData(id->j,
                        c,
                        end,
                        getString(durationStr),
                        getString(barSizeSetting),
                        whatToShow->s,
                        static_cast<I>(useRTH->g),
                        2,
                        upd,
                        chartOptions);
  R (K)0;}
K cancelHistoricalData(K tickerId){
  Q(tickerId->t != -KJ, "type");
  ib->cancelHistoricalData(tickerId->j);
  R (K)0;}
K reqHistogramData(K reqId,
                   K contract,
                   K useRTH,
                   K timePeriod){
  Q(reqId->t != -KJ || contract->t != XD ||
    useRTH->t != -KB || timePeriod->t != KC, "type");
  std::string error;
  auto c = createContract(contract, error);
  Q(!error.empty(), error.c_str());
  ib->reqHistogramData(reqId->j,
                       c,
                       static_cast<I>(useRTH->g),
                       getString(timePeriod));
  R (K)0;}
K cancelHistogramData(K reqId){
  Q(reqId->t != -KJ, "type");
  ib->cancelHistogramData(reqId->j);
  R (K)0;}
K reqMktDepth(K reqId,
              K contract,
              K numRows,
              K isSmartDepth,
              K mktDepthOptions){
  Q(reqId->t != -KJ || contract->t != XD ||
    numRows->t != -KJ || isSmartDepth->t != -KB ||
    mktDepthOptions->t != XD, "type");
  std::string error;
  auto c = createContract(contract, error);
  Q(!error.empty(), error.c_str());
  auto tvl = createTagValueList(mktDepthOptions, error);
  Q(!error.empty(), error.c_str());
  ib->reqMktDepth(reqId->j,
                  c,
                  numRows->j,
                  static_cast<I>(isSmartDepth->g),
                  tvl);
  R (K)0;}
K cancelMktDepth(K reqId, K isSmartDepth){
  Q(reqId->t != -KJ || isSmartDepth->t != -KB, "type");
  ib->cancelMktDepth(reqId->j,static_cast<I>(isSmartDepth->g));
  R (K)0;}
K reqMktDepthExchanges(K ignore){ib->reqMktDepthExchanges();R (K)0;}
K reqFundamentalData(K reqId,
                     K contract,
                     K reportType,
                     K options) {
  Q(reqId->t != -KJ || contract->t != XD || reportType->t != -KS ||
     options->t != XD, "type");
  std::string error;
  auto c = createContract(contract, error);
  Q(!error.empty(), error.c_str());
  auto tvl = createTagValueList(options, error);
  Q(!error.empty(), error.c_str());
  ib->reqFundamentalData(reqId->j,
                         c,
                         reportType->s,
                         tvl);
  R (K)0;}
K cancelFundamentalData(K reqId) {
  Q(reqId->t != -KJ, "type");
  ib->cancelFundamentalData(reqId->j);
  R (K)0;}
K reqScannerParameters(K ignore){ib->reqScannerParameters(); R (K)0;}
K reqScannerSubscription(K reqId,
                         K subscription,
                         K opts,
                         K filter){
  Q(reqId->t != -KJ || subscription->t != XD ||
    opts->t != XD, "type");
  std::string error;
  auto ss = createScannerSubscription(subscription, error);
  Q(!error.empty(), error.c_str());
  auto tvopts = createTagValueList(opts, error);
  Q(!error.empty(), error.c_str());
  auto tvfilter = createTagValueList(filter, error);
  Q(!error.empty(), error.c_str());
  ib->reqScannerSubscription(reqId->j, ss, tvopts, tvfilter);
  R (K)0;}
K cancelScannerSubscription(K reqId){
  Q(reqId->t != -KJ, "type");
  ib->cancelScannerSubscription(reqId->j);
  R (K)0;}
K reqManagedAccts(K ignore) {ib->reqManagedAccts();R (K)0;}
K reqAccountSummary(K reqId, K groupName, K tags) {
  // TODO: reqAccountSummary not yet implemented
  Q(reqId->t != -KJ || groupName->t != KC || tags->t != KC, "type");
  R krr((S)"nyi");}
K cancelAccountSummary(K reqId) {
  Q(reqId->t != -KJ, "type");
  ib->cancelAccountSummary(reqId->j);
  R (K)0;}
K reqAccountUpdates(K subscribe, K acctCode) {
  Q(subscribe->t != -KB || acctCode->t != -KS, "type");
  Q(!ib->isConnected(), "connection");
  ib->reqAccountUpdates(static_cast<I>(subscribe->g), acctCode->s);
  R (K)0;}
K reqAccountUpdatesMulti(K reqId, K account, K modelCode, K ledgerAndNLV){
  Q(reqId->t != -KJ || account->t != -KS || modelCode->t != -KS || ledgerAndNLV->t != -KB, "type");
  Q(!ib->isConnected(), "connection");
  ib->reqAccountUpdatesMulti(reqId->j,
                             account->s,
                             modelCode->s,
                             static_cast<I>(ledgerAndNLV->g));
  R (K)0;}
K cancelAccountUpdatesMulti(K reqId) {
  Q(reqId->t != -KJ, "type");
  ib->cancelAccountUpdatesMulti(reqId->j);
  R (K)0;}
K reqPositions(K ignore) { ib->reqPositions(); R (K)0; }
K cancelPositions(K ignore){ib->cancelPositions();R (K)0;}
K reqPositionsMulti(K reqId, K account, K modelCode){
  Q(reqId->t != -KJ || account->t != -KS || modelCode->t != -KS, "type");
  Q(!ib->isConnected(), "connection");
  ib->reqPositionsMulti(reqId->j, account->s, modelCode->s);
  R (K)0;}
K cancelPositionsMulti(K reqId) {
  Q(reqId->t != -KJ, "type");
  ib->cancelAccountUpdatesMulti(reqId->j);
  R (K)0;}
K reqPnL(K reqId, K account, K modelCode){
  Q(reqId->t != -KJ || account->t != -KS || modelCode->t != -KS, "type");
  Q(!ib->isConnected(), "connection");
  ib->reqPnL(reqId->j, account->s, modelCode->s);
  R (K)0;}
K cancelPnL(K reqId){
  Q(reqId->t != -KJ, "type");
  ib->cancelPnL(reqId->j);
  R (K)0;}
K reqPnLSingle(K reqId, K account, K modelCode, K conId){
  Q(reqId->t != -KJ || account->t != -KS || modelCode->t != -KS || conId->t != -KJ, "type");
  Q(!ib->isConnected(), "connection");
  ib->reqPnLSingle(reqId->j, account->s, modelCode->s, conId->j);
  R (K)0;};
K cancelPnLSingle(K reqId){
  Q(reqId->t != -KJ, "type");
  ib->cancelPnLSingle(reqId->j);
  R (K)0;}
////////////////////////////////////////////////////////


template<class F, class V>
auto partial(F&&f, V&&v)
{
  return
    [f=std::forward<F>(f), v=std::forward<V>(v)](auto&&... args)
    {
      return f(v, decltype(args)(args)...);
    };
}

template<class F, class A0, class...Args>
auto partial(F&&f, A0&&a0, Args&&...args)
{
  return partial(partial(std::forward<F>(f), std::forward<A0>(a0)), std::forward<Args>(args)...);
}

template<typename T> std::string typeError(T *property, G expectedType)
{
    R stringFormat("Type %s not handled. Expected type id %s",
                   typeid(T).name(),
                   reinterpret_cast<char*>(expectedType));
}

template<typename T> V setAtom(T *property, K x, G type, std::string &error)
{
    // Base case. Should never be called.
    error = typeError(property, type);
}

template<> V setAtom(bool *property, K x, G type, std::string &error)
{
    SW(xt) {
        CS(-1, *property = static_cast<I>(x->g));
        CD: typeError(property, type);
    }
}

template<> V setAtom(int *property, K x, G type, std::string &error)
{
    SW(xt) {
        CS(-KI, *property = x->i);
        CD: typeError(property, type);
    }
}

template<> V setAtom(long *property, K x, G type, std::string &error)
{
    SW(xt) {
        CS(-KJ, *property = x->j);
        CD: typeError(property, type);
    }
}

template<> V setAtom(double *property, K x, G type, std::string &error)
{
    SW(xt) {
        CS(-KF, *property = x->f);
        CD: typeError(property, type);
    }
}

template<> V setAtom(std::string *property, K x, G type, std::string &error)
{
    SW(xt) {
        CS(-KS, *property = x->s);                                                          // symbol
        CS(-KM, *property = stringFormat("%04d%02d", (x->i)/12+2000, (x->i)%12+1));         // month
        CS(-KD, *property = formatTime("%Y%m%d", ((x->i) + 10957)*8.64e4, 0));              // date
        CS(-KZ, *property = formatTime("%Y%m%d %H:%M:%S", ((x->f) + 10957)*8.64e4, 0));     // datetime
        CD: typeError(property, type);
    }
}

template<typename T>
V setList(T &property, I type, K x, I index, std::string &error)
{
    SW(xt) {
        CS( 0, setProperty(property, type, kK(x)[index], index, error))
        CS( 1, setAtom(property, kb(kG(x)[index]), type, error))
        CS( 4, setAtom(property, kg(kG(x)[index]), type, error))
        CS( 5, setAtom(property, kh(kH(x)[index]), type, error))
        CS( 6, setAtom(property, ki(kI(x)[index]), type, error))
        CS( 7, setAtom(property, kj(kJ(x)[index]), type, error))
        CS( 8, setAtom(property, ke(kE(x)[index]), type, error))
        CS( 9, setAtom(property, kf(kF(x)[index]), type, error))
        CS(10, setAtom(property, kc(kC(x)[index]), type, error))
        CS(11, setAtom(property, ks(kS(x)[index]), type, error))
        CS(14, setAtom(property, kd(kI(x)[index]), type, error))
        CS(15, setAtom(property, kz(kF(x)[index]), type, error))
        CD: R;
    }
}

template<typename T>
V setProperty(T &property, I expectedType, K x, I index, std::string &error)
{
    I xt0 = xt < 0 ? xt : (0 < xt) && (xt < 20) ? -xt : kK(x)[index]->t;
    if (xt0 != expectedType) {
        error = stringFormat("Invalid type: %i. Expected %i", xt0, expectedType);
        R;
    }

    if (xt < 0) setAtom(property, x, expectedType, error);
    else if ((0 <= xt) && (xt < 20)) setList(property, expectedType, x, index, error);
    else {
        error = stringFormat("nyi for type %i", xt);
        R;
    }
}

V setProperties(K dict, std::map<std::string, std::function<V(K x, I i, std::string &err)>> &props, std::string &error)
{
    K keys = kK(dict)[0];
    K vals = kK(dict)[1];

    if (keys->t != KS) {
        error = "Keys must be syms";
        R;
    }

    std::string key;
    for (I i = 0; i < keys->n; i++) {
        key = kS(keys)[i];

        auto it = props.find(key);
        if (it != props.end()) {
            (it->second)(vals, i, error);
            if (!error.empty()) {
                error = stringFormat("Key: %s - %s", key.c_str(), error.c_str());
                R;
            }
        } else {
            error = stringFormat("Key not recognized: %s", key.c_str());
            R;
        }
    }
}

auto f = [](auto property, I expectedType, K x, I index, std::string &error) {
    R setProperty(property, expectedType, x, index, error);
};

Z TagValueListSPtr createTagValueList(K dict, std::string &error){
  TagValueListSPtr list;
  if (dict->t != XD) {error = "type"; R list;}
  K keys = kK(dict)[0];
  K vals = kK(dict)[1];
  if (keys->n > 0 && keys->t != KS) {
    error = "Keys must be syms";
    R list;}
  for (I i = 0; i < keys->n; i++) {
    TagValueSPtr tagValue(new TagValue());
    tagValue->tag = kS(keys)[i];
    partial(f, &tagValue->value, KC)(vals, i, error);
    if (!error.empty()) R list;
    list->push_back(tagValue);
  }
  R list;}

Z Contract createContract(K dict, std::string &error){
    Contract c;
    auto map = new std::map<std::string, std::function<V(K, I, std::string&)>> {
        { "conId",          partial(f, &c.conId,        -KJ) },
        { "currency",       partial(f, &c.currency,     -KS) },
        { "exchange",       partial(f, &c.exchange,     -KS) },
        { "lastTradeDateOrContractMonth", partial(f, &c.lastTradeDateOrContractMonth,       -KM) },
        { "includeExpired", partial(f, &c.includeExpired, -KB) },
        { "localSymbol",    partial(f, &c.localSymbol,  -KS) },
        { "multiplier",     partial(f, &c.multiplier,   -KS) },
        { "primaryExchange",partial(f, &c.primaryExchange, -KS) },
        { "right",          partial(f, &c.right,        -KS) },
        { "secId",          partial(f, &c.secId,        -KS) },
        { "secType",        partial(f, &c.secType,      -KS) },
        { "strike",         partial(f, &c.strike,       -KF) },
        { "symbol",         partial(f, &c.symbol,       -KS) },
        { "tradingClass",   partial(f, &c.tradingClass, -KS) }
    };
    setProperties(dict, *map, error);
    if (!error.empty()) error = "createContract: " + error;
    R c;}

Z ScannerSubscription createScannerSubscription(K dict, std::string &error)
{
    ScannerSubscription ss;
    auto map = new std::map<std::string, std::function<V(K, I, std::string&)>> {
        { "numberOfRows",           partial(f, &ss.numberOfRows,        -KJ) },
        { "instrument",             partial(f, &ss.instrument,          -KS) },
        { "locationCode",           partial(f, &ss.locationCode,         KC) },
        { "scanCode",               partial(f, &ss.scanCode,             KC) },
        { "abovePrice",             partial(f, &ss.abovePrice,          -KF) },
        { "belowPrice",             partial(f, &ss.belowPrice,          -KF) },
        { "aboveVolume",            partial(f, &ss.aboveVolume,         -KJ) },
        { "averageOptionVolumeAbove", partial(f, &ss.averageOptionVolumeAbove, -KJ) },
        { "marketCapAbove",         partial(f, &ss.marketCapAbove,      -KF) },
        { "marketCapBelow",         partial(f, &ss.marketCapBelow,      -KF) },
        { "moodyRatingAbove",       partial(f, &ss.moodyRatingAbove,     KC) },
        { "moodyRatingBelow",       partial(f, &ss.moodyRatingBelow,     KC) },
        { "spRatingAbove",          partial(f, &ss.spRatingAbove,        KC) },
        { "spRatingBelow",          partial(f, &ss.spRatingBelow,        KC) },
        { "maturityDateAbove",      partial(f, &ss.maturityDateAbove,   -KD) },
        { "maturityDateBelow",      partial(f, &ss.maturityDateBelow,   -KD) },
        { "couponRateAbove",        partial(f, &ss.couponRateAbove,     -KF) },
        { "couponRateBelow",        partial(f, &ss.couponRateBelow,     -KF) },
        { "excludeConvertible",     partial(f, &ss.excludeConvertible,  -KB) },
        { "scannerSettingPairs",    partial(f, &ss.scannerSettingPairs,  KC) },
        { "stockTypeFilter",        partial(f, &ss.stockTypeFilter,      KC) }
    };
    setProperties(dict, *map, error);
    if (!error.empty()) error = "createScannerSubscription: " + error;
    R ss;
}
