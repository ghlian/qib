#pragma once
#include "StdAfx.h"
#include "EWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h"
#include "helpers.h"
#include "ScannerSubscription.h"

class EClientSocket;

class IBClient : public EWrapper
{
public:
  IBClient();
  ~IBClient();
private:
  EClientSocket * const socket;
  EReaderOSSignal signal;
  EReader *reader;
  // std::unique_ptr<EReader> reader;
public:
  // std::unique_ptr<EClientSocket> socket;
  bool connect(const char * host, int port, int clientId = 0);
  void startReader();
  void disconnect() const;
  int fd() const;
  void onReceive();
  // connection properties
  bool isConnected() const;
  int serverVersion();
  std::string TwsConnectionTime();
  void setServerLogLevel(int level);
public:
  #include "EWrapper_prototypes.h"
private:
  void receiveData(const char *fun, K x);
  K convertContract(const Contract &contract); // TODO: clean convertContract
  K convertContract2(const Contract &contract);
  K convertContractDetails(const ContractDetails &contract);
public: // Requests
  void reqIds(int numIds);
  void reqCurrentTime();
  void reqContractDetails(int reqId, const Contract &contract);
  void reqMatchingSymbols(int reqId, const std::string& pattern);
  void reqSmartComponents(int reqId, std::string bboExchange);
  void reqMarketRule(int marketRuleId);
  void reqMarketDataType(int marketDataType);
  void reqMktData(TickerId tickerId, const Contract& contract, const std::string& genericTicks, bool snapshot, bool regulatorySnaphsot, const TagValueListSPtr& mktDataOptions);
  void cancelMktData(TickerId id);
  void reqTickByTickData(int reqId,
                         const Contract &contract,
                         const std::string& tickType,
                         int numberOfTicks,
                         bool ignoreSize);
  void cancelTickByTickData(int reqId);
  void reqRealTimeBars(TickerId id, const Contract &contract, int barSize, const std::string &whatToShow, bool useRTH, const TagValueListSPtr& realTimeBarsOptions);
  void cancelRealTimeBars(TickerId tickerId);
  void reqHeadTimestamp(int tickerId, const Contract &contract, const std::string& whatToShow, int useRTH, int formatDate);
  void cancelHeadTimestamp(int tickerId);
  void reqHistoricalTicks(int reqId,
                          const Contract &contract,
                          const std::string& startDateTime,
                          const std::string& endDateTime,
                          int numberOfTicks,
                          const std::string& whatToShow,
                          int useRth,
                          bool ignoreSize,
                          const TagValueListSPtr& miscOptions);
  void reqHistoricalData(TickerId id, const Contract &contract, const std::string &endDateTime, const std::string &durationStr, const std::string & barSizeSetting, const std::string &whatToShow, int useRTH, int formatDate, bool keepUpToDate, const TagValueListSPtr& chartOptions);
  void cancelHistoricalData(TickerId tickerId);
  void reqHistogramData(int reqId, const Contract &contract, bool useRTH, const std::string& timePeriod);
  void cancelHistogramData(int reqId);
  void reqMktDepth(TickerId reqId,
                   const Contract &contract,
                   int numRows,
                   bool isSmartDepth,
                   const TagValueListSPtr& mktDepthOptions);
  void cancelMktDepth(TickerId reqId, bool isSmartDepth);
  void reqMktDepthExchanges();
  void reqFundamentalData(TickerId reqId, const Contract& contract, const std::string& reportType, const TagValueListSPtr& fundamentalDataOptions);
  void cancelFundamentalData(TickerId reqId);
  void reqScannerParameters();
  void reqScannerSubscription(int tickerId,
                              const ScannerSubscription &subscription,
                              const TagValueListSPtr& opts,
                              const TagValueListSPtr& filter);
  void cancelScannerSubscription(int tickerId);
  void reqManagedAccts();
  void reqAccountSummary(int reqId,
                         const std::string& groupName,
                         const std::string& tags);
  void cancelAccountSummary(int reqId);
  void reqAccountUpdates(bool subscribe, const std::string& acctCode);
  void reqAccountUpdatesMulti( int reqId,
                               const std::string& account,
                               const std::string& modelCode,
                               bool ledgerAndNLV);
  void cancelAccountUpdatesMulti( int reqId);
  void reqPositions();
  void cancelPositions();
  void reqPositionsMulti( int reqId,
                          const std::string& account,
                          const std::string& modelCode);
  void cancelPositionsMulti(int reqId);
  void reqPnL(int reqId,
              const std::string& account,
              const std::string& modelCode);
  void cancelPnL(int reqId);
  void reqPnLSingle(int reqId,
                    const std::string& account,
                    const std::string& modelCode,
                    int conId);
  void cancelPnLSingle(int reqId);
};
