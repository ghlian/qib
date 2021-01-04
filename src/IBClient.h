#ifndef IBClient_h
#define IBClient_h

#include <EReaderOSSignal.h>
#include <EClientSocket.h>
#include <EReader.h>
#include "EWrapper.h"
#include "helpers.h"
#include "Contract.h"
#include "Order.h"
#include "Execution.h"
#include "ScannerSubscription.h"

//class EPosixClientSocket;
class EclientSocket;

typedef std::string IBString;

enum State {
    ST_CONNECT,
    ST_TICKDATAOPERATION,
    ST_TICKDATAOPERATION_ACK,
    ST_TICKOPTIONCOMPUTATIONOPERATION,
    ST_TICKOPTIONCOMPUTATIONOPERATION_ACK,
    ST_DELAYEDTICKDATAOPERATION,
    ST_DELAYEDTICKDATAOPERATION_ACK,
    ST_MARKETDEPTHOPERATION,
    ST_MARKETDEPTHOPERATION_ACK,
    ST_REALTIMEBARS,
    ST_REALTIMEBARS_ACK,
    ST_MARKETDATATYPE,
    ST_MARKETDATATYPE_ACK,
    ST_HISTORICALDATAREQUESTS,
    ST_HISTORICALDATAREQUESTS_ACK,
    ST_OPTIONSOPERATIONS,
    ST_OPTIONSOPERATIONS_ACK,
    ST_CONTRACTOPERATION,
    ST_CONTRACTOPERATION_ACK,
    ST_MARKETSCANNERS,
    ST_MARKETSCANNERS_ACK,
    ST_FUNDAMENTALS,
    ST_FUNDAMENTALS_ACK,
    ST_BULLETINS,
    ST_BULLETINS_ACK,
    ST_ACCOUNTOPERATIONS,
    ST_ACCOUNTOPERATIONS_ACK,
    ST_ORDEROPERATIONS,
    ST_ORDEROPERATIONS_ACK,
    ST_OCASAMPLES,
    ST_OCASAMPLES_ACK,
    ST_CONDITIONSAMPLES,
    ST_CONDITIONSAMPLES_ACK,
    ST_BRACKETSAMPLES,
    ST_BRACKETSAMPLES_ACK,
    ST_HEDGESAMPLES,
    ST_HEDGESAMPLES_ACK,
    ST_TESTALGOSAMPLES,
    ST_TESTALGOSAMPLES_ACK,
    ST_FAORDERSAMPLES,
    ST_FAORDERSAMPLES_ACK,
    ST_FAOPERATIONS,
    ST_FAOPERATIONS_ACK,
    ST_DISPLAYGROUPS,
    ST_DISPLAYGROUPS_ACK,
    ST_MISCELANEOUS,
    ST_MISCELANEOUS_ACK,
    ST_CANCELORDER,
    ST_CANCELORDER_ACK,
    ST_FAMILYCODES,
    ST_FAMILYCODES_ACK,
    ST_SYMBOLSAMPLES,
    ST_SYMBOLSAMPLES_ACK,
    ST_REQMKTDEPTHEXCHANGES,
    ST_REQMKTDEPTHEXCHANGES_ACK,
    ST_REQNEWSTICKS,
    ST_REQNEWSTICKS_ACK,
    ST_REQSMARTCOMPONENTS,
    ST_REQSMARTCOMPONENTS_ACK,
    ST_NEWSPROVIDERS,
    ST_NEWSPROVIDERS_ACK,
    ST_REQNEWSARTICLE,
    ST_REQNEWSARTICLE_ACK,
    ST_REQHISTORICALNEWS,
    ST_REQHISTORICALNEWS_ACK,
    ST_REQHEADTIMESTAMP,
    ST_REQHEADTIMESTAMP_ACK,
    ST_REQHISTOGRAMDATA,
    ST_REQHISTOGRAMDATA_ACK,
    ST_REROUTECFD,
    ST_REROUTECFD_ACK,
    ST_MARKETRULE,
    ST_MARKETRULE_ACK,
    ST_PNL,
    ST_PNL_ACK,
    ST_PNLSINGLE,
    ST_PNLSINGLE_ACK,
    ST_CONTFUT,
    ST_CONTFUT_ACK,
    ST_PING,
    ST_PING_ACK,
    ST_REQHISTORICALTICKS,
    ST_REQHISTORICALTICKS_ACK,
    ST_REQTICKBYTICKDATA,
    ST_REQTICKBYTICKDATA_ACK,
    ST_WHATIFSAMPLES,
    ST_WHATIFSAMPLES_ACK,
    ST_IDLE
};

class IBClient : public EWrapper {
public:
    IBClient();

    ~IBClient();

public:
    bool connect(const char *host, unsigned int port, int clientId = 0);

    void disconnect() const;

    bool isConnected() const;

    int fd() const;

    void onReceive();

private:
    //    std::unique_ptr<EclientSocket> socket;
    EClientSocket *const socket;

    //! [socket_declare]
    EReaderOSSignal m_osSignal;
    //	EClientSocket * const m_pClient;
    //! [socket_declare]
    State m_state;
    time_t m_sleepDeadline;

    OrderId m_orderId;
    EReader *m_pReader;
    bool m_extraAuth;
    std::string m_bboExchange;

private:
    void receiveData(const char *fun, K x);

    K convertOrder(const Order &order);

    K convertContract(const Contract &contract);

    K convertContractDetails(const ContractDetails &contract);

    K convertBondContractDetails(const ContractDetails &contract);

    K convertExecution(const Execution &execution);

    K convertCommissionReport(const CommissionReport &report);

    K convertTickAttrib(const TickAttrib &tickAttrib);

//    K convertUnderComp(const UnderComp &comp);
    K convertOrderState(const OrderState &orderState);

    K convertDeltaNeutralContract(const DeltaNeutralContract &deltaNeutralContract);

public:
    // Methods
    bool checkMessages();

    IBString TwsConnectionTime();

    int serverVersion();

    void calculateImpliedVolatility(TickerId reqId, const Contract &contract, double optionPrice, double underPrice);

    void calculateOptionPrice(TickerId reqId, const Contract &contract, double volatility, double underPrice);

    void cancelAccountSummary(int reqId);

    void cancelCalculateImpliedVolatility(TickerId reqId);

    void cancelCalculateOptionPrice(TickerId reqId);

    void cancelFundamentalData(TickerId reqId);

    void cancelHistoricalData(TickerId tickerId);

    void cancelMktData(TickerId id);

    //    void cancelMktDepth(TickerId tickerId);
    void cancelMktDepth(TickerId tickerId, bool isSmartDepth);

    void cancelNewsBulletins();

    void cancelOrder(OrderId id);

    void cancelPositions();

    void cancelRealTimeBars(TickerId tickerId);

    void cancelScannerSubscription(int tickerId);

    void exerciseOptions(TickerId tickerId, const Contract &contract, int exerciseAction, int exerciseQuantity,
                         const IBString &account, int override);

    void placeOrder(OrderId id, const Contract &contract, const Order &order);

    void queryDisplayGroups(int reqId);

    void replaceFA(faDataType pFaDataType, const IBString &cxml);

    void reqAccountSummary(int reqId, const IBString &groupName, const IBString &tags);

    void reqAccountUpdates(bool subscribe, const IBString &acctCode);

    void reqAllOpenOrders();

    void reqAutoOpenOrders(bool bAutoBind);

    void reqContractDetails(int reqId, const Contract &contract);

    void reqCurrentTime();

    void reqExecutions(int reqId, const ExecutionFilter &filter);

    void reqFundamentalData(TickerId reqId, const Contract &, const IBString &reportType);

    void reqGlobalCancel();

    // void reqHistoricalData(TickerId id, const Contract &contract, const IBString &endDateTime, const IBString &durationStr, const IBString & barSizeSetting, const IBString &whatToShow, int useRTH, int formatDate, const TagValueListSPtr& chartOptions);
    void
    reqHistoricalData(TickerId id, const Contract &contract, const IBString &endDateTime, const IBString &durationStr,
                      const IBString &barSizeSetting, const IBString &whatToShow, int useRTH, int formatDate,
                      bool keepUpToDate, const TagValueListSPtr &chartOptions);

    void reqIds(int numIds);

    void reqManagedAccts();

    void reqMarketDataType(int marketDataType);

    // void reqMktData(TickerId id, const Contract &contract, const IBString &genericTicks, bool snapshot, const TagValueListSPtr& mktDataOptions);
    void reqMktData(TickerId id, const Contract &contract, const IBString &genericTicks, bool snapshot,
                    bool regulatorySnapshot,
                    const TagValueListSPtr &mktDataOptions);

    void reqMktDepth(TickerId tickerId, const Contract &contract, int numRows, bool isSmartDepth,
                     const TagValueListSPtr &mktDepthOptions);

    void reqNewsBulletins(bool allMsgs);

    void reqOpenOrders();

    void reqPositions();

    void reqRealTimeBars(TickerId id, const Contract &contract, int barSize, const IBString &whatToShow, bool useRTH,
                         const TagValueListSPtr &realTimeBarsOptions);

    void reqScannerParameters();

    //    void reqScannerSubscription(int tickerId, const ScannerSubscription &subscription, const TagValueListSPtr& scannerSubscriptionOptions);
    void reqScannerSubscription(int tickerId, const ScannerSubscription &subscription,
                                const TagValueListSPtr &scannerSubscriptionOptions,
                                const TagValueListSPtr &scannerSubscriptionFilterOptions);

    void requestFA(faDataType pFaDataType);

    void setServerLogLevel(int level);

    void subscribeToGroupEvents(int reqId, int groupId);

    void unsubscribeFromGroupEvents(int reqId);

    void updateDisplayGroup(int reqId, const IBString &contractInfo);

    void verifyMessage(const IBString &apiData);

    void verifyRequest(const IBString &apiName, const IBString &apiVersion);


public:
    // Events
    void accountDownloadEnd(const IBString &accountName);

    void accountSummary(int reqId, const IBString &account, const IBString &tag, const IBString &value,
                        const IBString &curency);

    void accountSummaryEnd(int reqId);

    void accountUpdateMulti(int reqId, const std::string &account, const std::string &modelCode, const std::string &key,
                            const std::string &value, const std::string &currency);

    void accountUpdateMultiEnd(int reqId);

    void bondContractDetails(int reqId, const ContractDetails &contractDetails);

    void commissionReport(const CommissionReport &commissionReport);

    void completedOrder(const Contract &contract, const Order &order, const OrderState &orderState);

    void completedOrdersEnd();

    void connectionClosed();

    void contractDetails(int reqId, const ContractDetails &contractDetails);

    void contractDetailsEnd(int reqId);

    void connectAck();

    void currentTime(long time);

    void deltaNeutralValidation(int reqId, const DeltaNeutralContract &deltaNeutralContract);

    void displayGroupList(int reqId, const IBString &groups);

    void displayGroupUpdated(int reqId, const IBString &contractInfo);

    void error(int id, int errorCode, const std::string &errorString);

    void execDetails(int reqId, const Contract &contract, const Execution &execution);

    void execDetailsEnd(int reqId);

    void fundamentalData(TickerId reqId, const IBString &data);

    void familyCodes(const std::vector<FamilyCode> &familyCodes) {}; // TODO: Implement
    void headTimestamp(int reqId, const std::string &headTimestamp) {}; // TODO: implement

    void
    historicalData(TickerId reqId, const IBString &date, double open, double high, double low, double close, int volume,
                   int barCount, double WAP, int hasGaps);

    void historicalData(TickerId reqId, const Bar &bar);

    void historicalDataEnd(int reqId, const std::string &startDateStr, const std::string &endDateStr);

    void historicalNews(int requestId, const std::string &time, const std::string &providerCode,
                        const std::string &articleId, const std::string &headline) {}; // TODO: imlement
    void historicalNewsEnd(int requestId, bool hasMore) {}; // TODO: implement

    void historicalDataUpdate(TickerId reqId, const Bar &bar) {}; // TODO: implement
    void historicalTicks(int reqId, const std::vector<HistoricalTick> &ticks, bool done){}; // TODO: implement
    void historicalTicksBidAsk(int reqId, const std::vector<HistoricalTickBidAsk> &ticks, bool done) {}; // TODO: implement
    void historicalTicksLast(int reqId, const std::vector<HistoricalTickLast> &ticks, bool done){}; // TODO: implement
    void histogramData(int reqId, const HistogramDataVector &data) {}; // TODO: implement

    void managedAccounts(const IBString &accountsList);

    void marketDataType(TickerId reqId, int marketDataType);

    void mktDepthExchanges(const std::vector<DepthMktDataDescription> &depthMktDataDescriptions) {}; // TODO: implement
    void marketRule(int marketRuleId, const std::vector<PriceIncrement> &priceIncrements) {}; // TODO: implement

    void newsArticle(int requestId, int articleType, const std::string &articleText) {}; // TODO: implement
    void newsProviders(const std::vector<NewsProvider> &newsProviders) {}; // TODO: implement
    void nextValidId(OrderId orderId);

    void orderBound(long long orderId, int apiClientId, int apiOrderId) {}; // TODO: implement
    void openOrder(OrderId orderId, const Contract &, const Order &, const OrderState &);

    void openOrderEnd();

    void orderStatus(OrderId orderId, const std::string &status, double filled,
                     double remaining, double avgFillPrice, int permId, int parentId,
                     double lastFillPrice, int clientId, const std::string &whyHeld, double mktCapPrice);

    void pnl(int reqId, double dailyPnL, double unrealizedPnL, double realizedPnL);
    void pnlSingle(int reqId, int pos, double dailyPnL, double unrealizedPnL, double realizedPnL, double value) {}; // TODO: implement

    void position(const std::string &account, const Contract &contract, double position, double avgCost);

    void positionEnd();

    void positionMulti(int reqId, const std::string &account, const std::string &modelCode, const Contract &contract,
                       double pos, double avgCost);

    void positionMultiEnd(int reqId);

    void
    realtimeBar(TickerId reqId, long time, double open, double high, double low, double close, long volume, double wap,
                int count);

    void receiveFA(faDataType pFaDataType, const IBString &cxml) {}; // TODO: implement

    void rerouteMktDataReq(int reqId, int conid, const std::string &exchange) {}; // TODO: implement
    void rerouteMktDepthReq(int reqId, int conid, const std::string &exchange) {}; // TODO: implement

    void scannerData(int reqId, int rank, const ContractDetails &contractDetails, const IBString &distance,
                     const IBString &benchmark, const IBString &projection, const IBString &legsStr) {};

    void scannerDataEnd(int reqId);

    void scannerParameters(const IBString &xml);

    void securityDefinitionOptionalParameter(int reqId, const std::string &exchange, int underlyingConId,
                                             const std::string &tradingClass,
                                             const std::string &multiplier, const std::set<std::string> &expirations,
                                             const std::set<double> &strikes) {};// TODO: implement
    void securityDefinitionOptionalParameterEnd(int reqId) {};// TODO: implement

    void smartComponents(int reqId, const SmartComponentsMap &theMap) {}; // TODO: implement
    void softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers) {}; // TODO: implement
    void symbolSamples(int reqId, const std::vector<ContractDescription> &contractDescriptions) {};// TODO: implement

    void tickByTickAllLast(int reqId, int tickType, time_t time, double price, int size, const TickAttribLast& tickAttribLast, const std::string& exchange, const std::string& specialConditions) {};// TODO: implement
    void tickByTickBidAsk(int reqId, time_t time, double bidPrice, double askPrice, int bidSize, int askSize,
                          const TickAttribBidAsk &tickAttribBidAsk) {};// TODO: implement
    void tickByTickMidPoint(int reqId, time_t time, double midPoint) {}; // TODO: implement
    void tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const IBString &formattedBasisPoints,
                 double totalDividends, int holdDays, const IBString &futureExpiry, double dividendImpact,
                 double dividendsToExpiry);

    void tickNews(int tickerId, time_t timeStamp, const std::string &providerCode, const std::string &articleId,
                  const std::string &headline, const std::string &extraData) {}; // TODO: implement

    void tickGeneric(TickerId tickerId, TickType tickType, double value);

    void tickOptionComputation(TickerId tickerId, TickType tickType, double impliedVol, double delta, double optPrice,
                               double pvDividend, double gamma, double vega, double theta, double undPrice);

    void tickReqParams(int tickerId, double minTick, const std::string &bboExchange,
                       int snapshotPermissions) {}; // TODO: implement

    void tickSize(TickerId tickerId, TickType field, int size);

    void tickSnapshotEnd(int reqId);

    void tickString(TickerId tickerId, TickType tickType, const IBString &value);

    void updateAccountTime(const IBString &timeStamp);

    void
    updateAccountValue(const IBString &key, const IBString &val, const IBString &currency, const IBString &accountName);

    void updateMktDepth(TickerId id, int position, int operation, int side, double price, int size);

    void
    updateMktDepthL2(TickerId id, int position, const std::string &marketMaker, int operation, int side, double price,
                     int size,
                     bool isSmartDepth);

    void updateNewsBulletin(int msgId, int msgType, const IBString &newsMessage, const IBString &originExch);

//    void updatePortfolio(const Contract &contract, int position, double marketPrice, double marketValue, double averageCost,
//                    double unrealizedPNL, double realizedPNL, const IBString &accountName);
    void updatePortfolio(const Contract &contract, double position, double marketPrice, double marketValue,
                         double averageCost,
                         double unrealizedPNL, double realizedPNL, const std::string &accountName);

    void verifyCompleted(bool isSuccessful, const IBString &errorText);

    void verifyMessageAPI(const IBString &apiData);

    void verifyAndAuthMessageAPI(const std::string &apiData, const std::string &xyzChallange);

    void verifyAndAuthCompleted(bool isSuccessful, const std::string &errorText);

    void winError(const IBString &str, int lastError);

    void tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute);

    virtual void tickPrice(TickerId tickerId, TickType field, double price, const TickAttrib &attrib);


//public:
//    // events
//    #include "EWrapper_prototypes.h"
};


#endif
