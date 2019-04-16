#include "helpers.h"
#include "Contract.h"
#include "ScannerSubscription.h"

extern "C" {
  K LoadLibrary(K ignore);
  K connect_(K host, K port, K clientId);
  K disconnect(K ignore);
  K onReceive(K ignore);
  K isConnected(K ignore);
  K serverVersion(K ignore);
  K TwsConnectionTime(K ignore);
  K setServerLogLevel(K level);
  K reqIds(K numIds);
  K reqCurrentTime(K ignore);
  K reqContractDetails(K reqId, K contract);
  K reqMatchingSymbols(K reqId, K pattern);
  K reqSmartComponents(K reqId,K exchange);
  K reqMarketRule(K ruleId);
  K reqMarketDataType(K marketDataType);
  K reqMktData(K id, K contract, K genericTicks, K snapshot, K regSnapshot);
  K cancelMktData(K id);
  K reqTickByTickData(K id, K contract, K tickType, K nticks, K ignoreSize);
  K cancelTickByTickData(K id);
  K reqRealTimeBars(K id, K contract, K barSize, K whatToShow, K useRTH, K realTimeBarsOptions);
  K cancelRealTimeBars(K  tickerId);
  K reqHeadTimestamp(K id, K contract, K whatToShow, K useRTH);
  K cancelHeadTimestamp(K id);
  K reqHistoricalTicks(K id, K contract, K time,K lowerUpper, K nticks, K whatToShow, K useRTH, K ignoreSize);
  K reqHistoricalData(K id, K contract, K endDateTime, K durationStr, K barSizeSetting, K whatToShow, K useRTH, K keepUpToDate);
  K cancelHistoricalData(K tickerId);
  K reqHistogramData(K reqId, K contract, K useRTH, K timePeriod);
  K cancelHistogramData(K reqId);
  K reqMktDepth(K reqId, K contract, K numRows, K isSmartDepth, K mktDepthOptions);
  K cancelMktDepth(K reqId, K isSmartDepth);
  K reqMktDepthExchanges(K ignore);
  K reqFundamentalData(K reqId, K contract, K reportType, K options);
  K cancelFundamentalData(K reqId);
  K reqScannerParameters(K ignore);
  K reqScannerSubscription(K reqId, K subscription, K opts, K filter);
  K cancelScannerSubscription(K reqId);
  K reqManagedAccts(K ignore);
  K reqAccountSummary(K reqId, K groupName, K tags);
  K cancelAccountSummary(K reqId);
  K reqAccountUpdates(K subscribe, K acctCode);
  K reqAccountUpdatesMulti(K reqId, K account, K modelCode, K ledgerAndNLV);
  K cancelAccountUpdatesMulti(K reqId);
  K reqPositions(K ignore);
  K cancelPositions(K ignore);
  K reqPositionsMulti(K reqId, K account, K modelCode);
  K cancelPositionsMulti(K reqId);
  K reqPnL(K reqId, K account, K modelCode);
  K cancelPnL(K reqId);
  K reqPnLSingle(K reqId, K account, K modelCode, K conId);
  K cancelPnLSingle(K reqId);
}

Z Contract createContract(K dict, std::string &error);
Z TagValueListSPtr createTagValueList(K dict, std::string &error);
Z ScannerSubscription createScannerSubscription(K dict, std::string &error);

template<typename T> V setList(T &property, G type, K x, I index, std::string &error);
template<typename T> V setProperty(T &property, I expectedType, K x, I index, std::string &error);
template<typename F> V setProperties(K dict, F &props, std::string &error);
V setProperties(K dict, std::map<std::string, std::function<void(Contract&)>> &map, std::string &error);
