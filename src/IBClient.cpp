#include "IBClient.h"
#include "EClientSocket.h"
#include "helpers.h"

IBClient::IBClient() :
  signal(2000),  // was signal(2000) for 2s timeout
  socket(new EClientSocket(this, &signal)),
  reader(0)
{}

IBClient::~IBClient() {
  if(reader)
    delete reader;
  delete socket;}

bool IBClient::connect(const char *host, int port, int clientId) {
  return socket->eConnect( host, port, clientId);}

void IBClient::startReader(){
    reader = new EReader(socket, &signal);
    reader->start();}

void IBClient::disconnect() const {
  sd0(fd());
  socket->eDisconnect();}
int IBClient::fd() const {return socket->fd();}

void IBClient::onReceive() {
  signal.waitForSignal();
  errno=0;
  reader->processMsgs();}

void IBClient::receiveData(const char *fun, K x) {
  K r = k(0, (S)".ib.onrecv", ks((S)fun), x, (K)0);
  if (!r) {
    O("Broken socket");
  } else if (r->t == -128) {
    O("Error calling '%s': %s. Type: %i. Length: %lli\n", fun, r->s, xt, xn);
  }
  r0(r);}


////////////////////////////////////////////
///             Error handling       //////
//////////////////////////////////////////


void IBClient::error(int id, int errorCode, const std::string& errorString){
  std::string type;
  if (1100 <= errorCode && errorCode <= 1300) {
    type = "system";
  } else if (2100 <= errorCode && errorCode <= 2137) {
    type = "warning";
  } else {
    type = "error";
  }
  receiveData(type.c_str(), knk(3, kj(id), kj(errorCode), kip(errorString)));
  // "Connectivity between IB and TWS has been lost"
  if (id == -1 && errorCode == 1100) {
    receiveData("disconnected", identity());
    disconnect();
  }
  // Exception caught while reading socket - Connection reset by peer
  if (id == -1 && errorCode == 509) {
    O("Connection reset by peer\n");
    receiveData("disconnected", identity());
    disconnect();
  }
}
void IBClient::winError( const std::string& str, int lastError) {  // This event is called when there is an error on the client side.
  receiveData("winError", knk(2, kip(str), ki(lastError)));}
void IBClient::setServerLogLevel(int level){socket->setServerLogLevel(level);}

////////////////////////////////////////////
///             Connection           //////
//////////////////////////////////////////

bool IBClient::isConnected() const {return socket->isConnected();}
int IBClient::serverVersion(){return socket->EClient::serverVersion();}
std::string IBClient::TwsConnectionTime(){return socket->TwsConnectionTime();}

////////////////////////////////////////////
///             Requests             //////
//////////////////////////////////////////

void IBClient::reqIds(int numIds){socket->reqIds(numIds);}
void IBClient::reqCurrentTime(){socket->reqCurrentTime();}
void IBClient::reqContractDetails(int reqId,
                                  const Contract &contract){
  socket->reqContractDetails(reqId, contract);}
void IBClient::reqMatchingSymbols(int reqId, const std::string& pattern){
  socket->reqMatchingSymbols(reqId,pattern);}
void IBClient::reqSmartComponents(int reqId, std::string bboExchange){
  socket->reqSmartComponents(reqId, bboExchange);}
void IBClient::reqMarketRule( int marketRuleId ) {
  socket->reqMarketRule( marketRuleId );}
void IBClient::reqMarketDataType(int marketDataType){
  socket->reqMarketDataType(marketDataType);}
void IBClient::reqMktData(TickerId tickerId,
                          const Contract& contract,
                          const std::string& genericTicks,
                          bool snapshot,
                          bool regulatorySnaphsot,
                          const TagValueListSPtr& mktDataOptions){
  socket->reqMktData(tickerId, contract, genericTicks, snapshot,
                     regulatorySnaphsot, mktDataOptions);}
void IBClient::cancelMktData(TickerId id){ socket->cancelMktData(id);}
void IBClient::reqTickByTickData(int reqId,
                                 const Contract &contract,
                                 const std::string& tickType,
                                 int numberOfTicks,
                                 bool ignoreSize){
socket->reqTickByTickData(reqId,
                          contract,
                          tickType,
                          numberOfTicks,
                          ignoreSize);}
void IBClient::cancelTickByTickData(int reqId){
  socket->cancelTickByTickData(reqId);}
void IBClient::reqRealTimeBars(TickerId id,
                               const Contract &contract,
                               int barSize,
                               const std::string &whatToShow,
                               bool useRTH,
                               const TagValueListSPtr &realTimeBarsOptions){
  socket->reqRealTimeBars(id, contract, barSize,
                          whatToShow, useRTH, realTimeBarsOptions);}
void IBClient::cancelRealTimeBars(TickerId tickerId){
  socket->cancelRealTimeBars(tickerId);}
void IBClient::reqHeadTimestamp(int tickerId,
                                const Contract &contract,
                                const std::string& whatToShow,
                                int useRTH,
                                int formatDate){
  socket->reqHeadTimestamp(tickerId,contract,whatToShow,useRTH,formatDate);}
void IBClient::cancelHeadTimestamp(int tickerId) {
  socket->cancelHeadTimestamp(tickerId);}
void IBClient::reqHistoricalTicks(int reqId,
                                  const Contract &contract,
                                  const std::string& startDateTime,
                                  const std::string& endDateTime,
                                  int numberOfTicks,
                                  const std::string& whatToShow,
                                  int useRth,
                                  bool ignoreSize,
                                  const TagValueListSPtr& miscOptions) {
  socket->reqHistoricalTicks(reqId,
                             contract,
                             startDateTime,
                             endDateTime,
                             numberOfTicks,
                             whatToShow,
                             useRth,
                             ignoreSize,
                             miscOptions );}
void IBClient::reqHistoricalData(TickerId id,
                                 const Contract &contract,
                                 const std::string &endDateTime,
                                 const std::string &durationStr,
                                 const std::string &barSizeSetting,
                                 const std::string &whatToShow,
                                 int useRTH,
                                 int formatDate,
                                 bool keepUpToDate,
                                 const TagValueListSPtr &chartOptions){
  socket->reqHistoricalData(id,
                            contract,
                            endDateTime,
                            durationStr,
                            barSizeSetting,
                            whatToShow,
                            useRTH,
                            formatDate,
                            keepUpToDate,
                            chartOptions);}
void IBClient::cancelHistoricalData(TickerId tickerId){
  socket->cancelHistoricalData(tickerId);}
void IBClient::reqHistogramData(int reqId,
                                const Contract &contract,
                                bool useRTH,
                                const std::string& timePeriod) {
  socket->reqHistogramData(reqId,contract,useRTH,timePeriod);}
void IBClient::cancelHistogramData(int reqId){
  socket->cancelHistogramData(reqId);}
void IBClient::reqMktDepth(TickerId reqId,
                           const Contract &contract,
                           int numRows,
                           bool isSmartDepth,
                           const TagValueListSPtr &mktDepthOptions){
  socket->reqMktDepth(reqId,
                      contract,
                      numRows,
                      isSmartDepth,
                      mktDepthOptions);}
void IBClient::cancelMktDepth(TickerId reqId, bool isSmartDepth){
  socket->cancelMktDepth(reqId,isSmartDepth);}
void IBClient::reqMktDepthExchanges(){socket->reqMktDepthExchanges();}
void IBClient::reqFundamentalData(TickerId reqId,
                                  const Contract& contract,
                                  const std::string& reportType,
                                  const TagValueListSPtr& fundamentalDataOptions){
  socket->reqFundamentalData(reqId, contract, reportType, fundamentalDataOptions);}
void IBClient::cancelFundamentalData(TickerId reqId){
  socket->cancelFundamentalData(reqId);}
void IBClient::reqScannerParameters(){socket->reqScannerParameters();}
void IBClient::reqScannerSubscription(int tickerId,
                                      const ScannerSubscription &subscription,
                                      const TagValueListSPtr &opts,
                                      const TagValueListSPtr &filter) {
  socket->reqScannerSubscription(tickerId, subscription, opts, filter);}
void IBClient::cancelScannerSubscription(int tickerId){
  socket->cancelScannerSubscription(tickerId);}
void IBClient::reqManagedAccts(){socket->reqManagedAccts();}
void IBClient::reqAccountSummary(int reqId,
                       const std::string& groupName,
                       const std::string& tags){
  socket->reqAccountSummary(reqId, groupName, tags);}
void IBClient::cancelAccountSummary(int reqId){socket->cancelAccountSummary(reqId);}
void IBClient::reqAccountUpdates(bool subscribe, const std::string& acctCode){
  socket->reqAccountUpdates(subscribe, acctCode);}

void IBClient::reqAccountUpdatesMulti( int reqId,
                                       const std::string& account,
                                       const std::string& modelCode,
                                       bool ledgerAndNLV){
  socket->reqAccountUpdatesMulti(reqId,account,modelCode,ledgerAndNLV);}
void IBClient::cancelAccountUpdatesMulti( int reqId){
  socket->cancelAccountUpdatesMulti(reqId);}
void IBClient::reqPositions(){socket->reqPositions();}
void IBClient::cancelPositions(){socket->cancelPositions();}
void IBClient::reqPositionsMulti( int reqId,
                                  const std::string& account,
                                  const std::string& modelCode){
  socket->reqPositionsMulti(reqId,account,modelCode);}
void IBClient::cancelPositionsMulti(int reqId){
  socket->cancelPositionsMulti(reqId);}
void IBClient::reqPnL(int reqId,
                      const std::string& account,
                      const std::string& modelCode){
  socket->reqPnL( reqId, account, modelCode);}
void IBClient::cancelPnL(int reqId){socket->cancelPnL(reqId);}
void IBClient::reqPnLSingle(int reqId,
                            const std::string& account,
                            const std::string& modelCode,
                            int conId){
  socket->reqPnLSingle( reqId, account, modelCode, conId);}
void IBClient::cancelPnLSingle(int reqId) { socket->cancelPnLSingle( reqId);}

////////////////////////////////////////////
///           Status Events          //////
//////////////////////////////////////////

void IBClient::connectAck() {   // deprecated mode of connection. Callback initially acknowledging connection attempt connection handshake not complete until nextValidID is received
  receiveData("connectAck", identity());}
void IBClient::nextValidId( OrderId orderId) {
  receiveData("nextValidId", kj(orderId));}
void IBClient::currentTime(long time) {
  receiveData("currentTime", kts(time));}
void IBClient::managedAccounts( const std::string& accountsList){
  receiveData("managedAccounts", kip(accountsList));}
void IBClient::connectionClosed() {
  receiveData("connectionClosed", identity());}

void IBClient::marketDataType(TickerId reqId, int marketDataType) {
  receiveData("marketDataType", knk(2, kj(reqId), ki(marketDataType)));}

////////////////////////////////////////////
///         Contract Events          //////
//////////////////////////////////////////


void IBClient::contractDetails( int reqId,
                                const ContractDetails& details) {
  K list = knk(1,kj(reqId));
  jv(&list,convertContract2(details.contract));
  jv(&list,convertContractDetails(details));
  receiveData("contractDetails",list);}
void IBClient::contractDetailsEnd( int reqId) {
  receiveData("contractDetailsEnd",kj(reqId));
}
void IBClient::bondContractDetails( int reqId, const ContractDetails& contractDetails) { }
void IBClient::securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string& tradingClass,
                                                   const std::string& multiplier, const std::set<std::string>& expirations, const std::set<double>& strikes) {
}
void IBClient::securityDefinitionOptionalParameterEnd(int reqId) {
}
void IBClient::marketRule(int marketRuleId,
                          const std::vector<PriceIncrement> &priceIncrements){
  K x=ktn(0,3);
  K id=kK(x)[0]=ktn(KJ,priceIncrements.size());
  K low=kK(x)[1]=ktn(KF,priceIncrements.size());
  K incr=kK(x)[2]=ktn(KF,priceIncrements.size());

  for (unsigned int i = 0; i < priceIncrements.size();i++) {
    kJ(id)[i]=marketRuleId;
    kF(low)[i]=priceIncrements[i].lowEdge;
    kF(incr)[i]=priceIncrements[i].increment;
   }
  receiveData("marketRule",x);
}

void IBClient::symbolSamples(int reqId,
                             const std::vector<ContractDescription> &contractDescriptions) {
  int n=contractDescriptions.size();
  K x=knk(7,
          ktn(KJ,n),
          ktn(KJ,n),
          ktn(KS,n),
          ktn(KS,n),
          ktn(KS,n),
          ktn(KS,n),
          ktn(0,n));
  DO(n,
     kJ(kK(x)[0])[i]=reqId;
     kJ(kK(x)[1])[i]=contractDescriptions[i].contract.conId;
     kS(kK(x)[2])[i]=ss((S)contractDescriptions[i].contract.symbol.c_str());
     kS(kK(x)[3])[i]=ss((S)contractDescriptions[i].contract.secType.c_str());
     kS(kK(x)[4])[i]=ss((S)contractDescriptions[i].contract.primaryExchange.c_str());
     kS(kK(x)[5])[i]=ss((S)contractDescriptions[i].contract.currency.c_str());
     int _j = contractDescriptions[i].derivativeSecTypes.size();
     kK(kK(x)[6])[i]= ktn(KS,_j);
     for (unsigned int j = 0; j < _j; j++) {
       kS(kK(kK(x)[6])[i])[j] = ss((S)contractDescriptions[i].derivativeSecTypes[j].c_str());
     };
     );
  receiveData("symbolSamples",x);
}

////////////////////////////////////////////
///   Realtime Events - ticks & bars //////
//////////////////////////////////////////

void IBClient::tickPrice( TickerId tickerId,
                          TickType field,
                          double price,
                          const TickAttrib& attribs) {
  receiveData("tickPrice",
              knk(6,
                  kj(tickerId),
                  ki(field),
                  kf(price),
                  kb(attribs.canAutoExecute),
                  kb(attribs.pastLimit),
                  kb(attribs.preOpen)));
}
void IBClient::tickSize( TickerId tickerId,
                         TickType field,
                         int size) {
  receiveData("tickSize", knk(3, kj(tickerId), ki(field), kj(size)));}

void IBClient::tickGeneric( TickerId tickerId,
                            TickType tickType,
                            double value) {
  receiveData("tickGeneric", knk(3, kj(tickerId), ki(tickType), kf(value)));}

void IBClient::tickString( TickerId tickerId,
                           TickType tickType,
                           const std::string& value) {
  receiveData("tickString", knk(3, kj(tickerId), ki(tickType), kip(value)));}

void IBClient::tickEFP( TickerId tickerId,
                        TickType tickType,
                        double basisPoints,
                        const std::string& formattedBasisPoints,
                        double totalDividends,
                        int holdDays,
                        const std::string& futureLastTradeDate,
                        double dividendImpact,
                        double dividendsToLastTradeDate) {
  receiveData("tickEFP",
              knk(9,
                  kj(tickerId),
                  ki(tickType),
                  kf(basisPoints),
                  kip(formattedBasisPoints),
                  ki(holdDays),
                  kip(futureLastTradeDate),
                  kf(totalDividends),
                  kf(dividendImpact),
                  kf(dividendsToLastTradeDate)));
}

void IBClient::tickOptionComputation( TickerId tickerId,
                                      TickType tickType,
                                      double impliedVol,
                                      double delta,
                                      double optPrice,
                                      double pvDividend,
                                      double gamma,
                                      double vega,
                                      double theta,
                                      double undPrice) {
  receiveData("tickOptionComputation",
              knk(10,
                  kj(tickerId),
                  ki(tickType),
                  kf(impliedVol),
                  kf(delta),
                  kf(optPrice),
                  kf(pvDividend),
                  kf(gamma),
                  kf(vega),
                  kf(theta),
                  kf(undPrice)));
}

void IBClient::tickSnapshotEnd( int reqId) {
  receiveData("tickSnapshotEnd", ki(reqId));}

void IBClient::tickReqParams(int tickerId,
                             double minTick,
                             const std::string& bboExchange,
                             int snapshotPermissions) {
  receiveData("tickReqParams", knk(4,
                                   kj(tickerId),
                                   kf(minTick),
                                   ks((S)bboExchange.c_str()),
                                   ki(snapshotPermissions)));}
void IBClient::tickByTickAllLast(int reqId,
                                 int tickType,
                                 time_t time,
                                 double price,
                                 int size,
                                 const TickAttribLast& tickAttribLast,
                                 const std::string& exchange,
                                 const std::string& specialConditions) {
  receiveData("tickByTickAllLast",knk(8,
                                      kj(reqId),
                                      kts(time),
                                      kf(price),
                                      kj(size),
                                      kb(tickAttribLast.pastLimit),
                                      kb(tickAttribLast.unreported),
                                      kis(exchange),
                                      kip(specialConditions)));}
void IBClient::tickByTickBidAsk(int reqId,
                                time_t time,
                                double bidPrice,
                                double askPrice,
                                int bidSize,
                                int askSize,
                                const TickAttribBidAsk& tickAttribBidAsk) {
  receiveData("tickByTickBidAsk",knk(8,
                                      kj(reqId),
                                      kts(time),
                                      kf(bidPrice),
                                      kf(askPrice),
                                      kj(bidSize),
                                      kj(askSize),
                                      kb(tickAttribBidAsk.bidPastLow),
                                      kb(tickAttribBidAsk.askPastHigh)));}
void IBClient::tickByTickMidPoint(int reqId,
                                  time_t time,
                                  double midPoint) {
  receiveData("tickByTickMidPoint",knk(3,
                                     kj(reqId),
                                     kts(time),
                                     kf(midPoint)));}
void IBClient::realtimeBar(TickerId reqId,
                           long time,
                           double open,
                           double high,
                           double low,
                           double close,
                           long volume,
                           double wap,
                           int count) {
  receiveData("realtimeBar",knk(9,
                                kj(reqId),
                                kts(time),
                                kf(open),
                                kf(high),
                                kf(low),
                                kf(close),
                                kj(volume),
                                kf(wap),
                                ki(count)));}
void IBClient::smartComponents(int reqId,
                                 const SmartComponentsMap& theMap) {
  int n=theMap.size();
  K x=knk(4,
          ktn(KJ,n),
          ktn(KJ,n),
          ktn(KS,n),
          ktn(KC,n));
  for (SmartComponentsMap::const_iterator it = theMap.begin(); it != theMap.end(); it++) {
    int i=std::distance(theMap.begin(), it);
    kJ(kK(x)[0])[i]=reqId;
    kJ(kK(x)[1])[i]=it->first;
    kS(kK(x)[2])[i]=ss((S)std::get<0>(it->second).c_str());
    kC(kK(x)[3])[i]=std::get<1>(it->second);
  }
  receiveData("smartComponents",x);
}
void IBClient::rerouteMktDataReq(int reqId,
                                 int conid,
                                 const std::string& exchange) {
  receiveData("rerouteMktDataReq", knk(3,
                                       kj(reqId),
                                       kj(conid),
                                       ks((S)exchange.c_str())));}

////////////////////////////////////////////
///        Historical Data Events       //////
//////////////////////////////////////////

void IBClient::headTimestamp(int reqId,
                             const std::string& headTimestamp) {
  receiveData("headTimestamp", knk(2,
                                   kj(reqId),
                                   kts(atoi(headTimestamp.c_str()))));}

void IBClient::historicalTicks(int reqId,
                               const std::vector<HistoricalTick>& ticks,
                               bool done) {
  int n=ticks.size();
  K x=knk(3,
          ktn(KP,n),
          ktn(KF,n),
          ktn(KJ,n));
  DO(n,
     kJ(kK(x)[0])[i]=pu(ticks[i].time);
     kF(kK(x)[1])[i]=ticks[i].price;
     kJ(kK(x)[2])[i]=ticks[i].size;);
  receiveData("historicalTicks",knk(3,kj(reqId),kb(done),x));
}
void IBClient::historicalTicksLast(int reqId,
                                   const std::vector<HistoricalTickLast>& ticks,
                                   bool done) {
  int n=ticks.size();
  K x=knk(7,
          ktn(KP,n),
          ktn(KF,n),
          ktn(KJ,n),
          ktn(KB,n),
          ktn(KB,n),
          ktn(KS,n),
          ktn(0,n));
  DO(n,
     kJ(kK(x)[0])[i]=pu(ticks[i].time);
     kF(kK(x)[1])[i]=ticks[i].price;
     kJ(kK(x)[2])[i]=ticks[i].size;
     kG(kK(x)[3])[i]=ticks[i].tickAttribLast.pastLimit;
     kG(kK(x)[4])[i]=ticks[i].tickAttribLast.unreported;
     kS(kK(x)[5])[i]=ss((S)ticks[i].exchange.c_str());
     kK(kK(x)[6])[i]=kip(ticks[i].specialConditions));
  receiveData("historicalTicksLast",knk(3,kj(reqId),kb(done),x));
}

void IBClient::historicalTicksBidAsk(int reqId,
                                     const std::vector<HistoricalTickBidAsk>& ticks,
                                     bool done) {
  int n=ticks.size();
  K x=knk(7,
          ktn(KP,n),
          ktn(KF,n),
          ktn(KF,n),
          ktn(KJ,n),
          ktn(KJ,n),
          ktn(KB,n),
          ktn(KB,n));
  DO(n,
     kJ(kK(x)[0])[i]=pu(ticks[i].time);
     kF(kK(x)[1])[i]=ticks[i].priceBid;
     kF(kK(x)[2])[i]=ticks[i].priceAsk;
     kJ(kK(x)[3])[i]=ticks[i].sizeBid;
     kJ(kK(x)[4])[i]=ticks[i].sizeAsk;
     kG(kK(x)[5])[i]=ticks[i].tickAttribBidAsk.bidPastLow;
     kG(kK(x)[6])[i]=ticks[i].tickAttribBidAsk.askPastHigh);
  receiveData("historicalTicksBidAsk",knk(3,kj(reqId),kb(done),x));
}
void IBClient::historicalData(TickerId reqId,
                              const Bar& bar) {
  receiveData("historicalData",knk(9,
                                   kj(reqId),
                                   kip(bar.time),
                                   kf(bar.open),
                                   kf(bar.high),
                                   kf(bar.low),
                                   kf(bar.close),
                                   kj(bar.volume),
                                   kf(bar.wap),
                                   ki(bar.count)));}
void IBClient::historicalDataUpdate(TickerId reqId,
                                    const Bar& bar) {
  receiveData("historicalDataUpdate",knk(9,
                                         kj(reqId),
                                         kip(bar.time),
                                         kf(bar.open),
                                         kf(bar.high),
                                         kf(bar.low),
                                         kf(bar.close),
                                         kj(bar.volume),
                                         kf(bar.wap),
                                         ki(bar.count)));}
void IBClient::historicalDataEnd(int reqId,
                                 const std::string& startDateStr,
                                 const std::string& endDateStr) {
  receiveData("historicalDataEnd",knk(3,
                                      kj(reqId),
                                      kip(startDateStr),
                                      kip(endDateStr)));}
void IBClient::histogramData(int reqId,
                             const HistogramDataVector& data) {
  int n=data.size();
  K x=knk(2,
          ktn(KF,n),
          ktn(KJ,n));
  DO(n,
     kF(kK(x)[0])[i]=data[i].price;
     kJ(kK(x)[1])[i]=data[i].size);
  receiveData("histogramData",knk(2,kj(reqId),x));}
////////////////////////////////////////////
///        Market Depth Events       //////
//////////////////////////////////////////

void IBClient::updateMktDepth(TickerId id,
                              int position,
                              int operation,
                              int side,
                              double price,
                              int size) {
  receiveData("updateMktDepth", knk(6,
                                    kj(id),
                                    kj(position),
                                    ki(operation),
                                    ki(side),
                                    kf(price),
                                    ki(size)));}
void IBClient::updateMktDepthL2(TickerId id,
                                int position,
                                const std::string& marketMaker,
                                int operation,
                                int side,
                                double price,
                                int size,
                                bool isSmartDepth) {
  receiveData("updateMktDepthL2", knk(8,
                                      kj(id),
                                      kj(position),
                                      kis(marketMaker),
                                      ki(operation),
                                      ki(side),
                                      kf(price),
                                      kj(size),
                                      kb(isSmartDepth)));}

void IBClient::rerouteMktDepthReq(int reqId,
                                  int conid,
                                  const std::string& exchange) {
receiveData("rerouteMktDepthReq", knk(3,
                                     kj(reqId),
                                     kj(conid),
                                     ks((S)exchange.c_str())));}
void IBClient::mktDepthExchanges(const std::vector<DepthMktDataDescription> &depthMktDataDescriptions){
  int n=depthMktDataDescriptions.size();
  K x=knk(5,
          ktn(KS,n),
          ktn(KS,n),
          ktn(KS,n),
          ktn(KS,n),
          ktn(KJ,n));
  for (unsigned int i = 0; i < depthMktDataDescriptions.size(); i++) {
    kS(kK(x)[0])[i]=ss((S)depthMktDataDescriptions[i].exchange.c_str());
    kS(kK(x)[1])[i]=ss((S)depthMktDataDescriptions[i].secType.c_str());
    kS(kK(x)[2])[i]=ss((S)depthMktDataDescriptions[i].listingExch.c_str());
    kS(kK(x)[3])[i]=ss((S)depthMktDataDescriptions[i].serviceDataType.c_str());
    kJ(kK(x)[4])[i]= depthMktDataDescriptions[i].aggGroup == INT_MAX ? nj : depthMktDataDescriptions[i].aggGroup;
      // : "");
  }
  receiveData("mktDepthExchanges",x);
}

////////////////////////////////////////////
///         Fundamental Events       //////
//////////////////////////////////////////

void IBClient::fundamentalData(TickerId reqId, const std::string& data) {
receiveData("fundamentalData", knk(2, kj(reqId), kip(data)));}

////////////////////////////////////////////
///             Scanner Events       //////
//////////////////////////////////////////

void IBClient::scannerParameters(const std::string& xml) {
  receiveData("scannerParameters", kip(xml));
}
void IBClient::scannerData(int reqId,
                           int rank,
                           const ContractDetails& contractDetails,
                           const std::string& distance,
                           const std::string& benchmark,
                           const std::string& projection,
                           const std::string& legsStr) {
  printf( "ScannerData. %d - Rank: %d, Symbol: %s, SecType: %s, Currency: %s, Distance: %s, Benchmark: %s, Projection: %s, Legs String: %s\n",
          reqId,
          rank,
          contractDetails.contract.symbol.c_str(),
          contractDetails.contract.secType.c_str(),
          contractDetails.contract.currency.c_str(),
          distance.c_str(),
          benchmark.c_str(),
          projection.c_str(),
          legsStr.c_str());
}
void IBClient::scannerDataEnd(int reqId) {
  receiveData("scannerDataEnd", kj(reqId));}

////////////////////////////////////////////
///             Account Events       //////
//////////////////////////////////////////


void IBClient::accountSummary( int reqId,
                               const std::string& account,
                               const std::string& tag,
                               const std::string& value,
                               const std::string& curency) {
  receiveData("accountSummary", knk(5,
                                    ki(reqId),
                                    kis(account),
                                    kis(tag),
                                    kip(value),
                                    kis(curency)));}
void IBClient::accountSummaryEnd( int reqId) {
  receiveData("accountSummaryEnd", ki(reqId));}
void IBClient::updateAccountValue(const std::string& key,
                                  const std::string& val,
                                  const std::string& currency,
                                  const std::string& accountName) {
  receiveData("updateAccountValue",
              knk(4,
                  kis(accountName),
                  kis(key),
                  kip(val),
                  kis(currency)));
  }
void IBClient::accountDownloadEnd(const std::string& accountName) {
  receiveData("accountDownloadEnd",kis(accountName));
}
void IBClient::updateAccountTime(const std::string& timeStamp) {
  receiveData("updateAccountTime", kip(timeStamp));}
void IBClient::accountUpdateMulti( int reqId,
                                   const std::string& account,
                                   const std::string& modelCode,
                                   const std::string& key,
                                   const std::string& value,
                                   const std::string& currency) {
  receiveData("accountUpdateMulti",
              knk(6,
                  ki(reqId),
                  kis(account),
                  kis(key),
                  kip(value),
                  kis(currency),
                  kis(modelCode)));}
void IBClient::accountUpdateMultiEnd( int reqId) {
  receiveData("accountUpdateMultiEnd", ki(reqId));}
void IBClient::position( const std::string& account,
                         const Contract& contract,
                         double position,
                         double avgCost) {

  K list = knk(3, kis(account), kj(position), kf(avgCost));
  jv(&list,convertContract2(contract));
  receiveData("position", list);
}
void IBClient::positionEnd() {receiveData("positionEnd", identity());}
void IBClient::positionMulti( int reqId,
                              const std::string& account,
                              const std::string& modelCode,
                              const Contract& contract,
                              double pos,
                              double avgCost) {
  K list = knk(2, ki(reqId), kis(modelCode));
  jv(&list, knk(3, kis(account), kj(pos), kf(avgCost)));
  jv(&list,convertContract2(contract));
  receiveData("positionMulti", list);
}
void IBClient::positionMultiEnd( int reqId) {
  receiveData("positionMultiEnd", ki(reqId));}
void IBClient::updatePortfolio( const Contract& contract,
                                double position,
                                double marketPrice,
                                double marketValue,
                                double averageCost,
                                double unrealizedPNL,
                                double realizedPNL,
                                const std::string& accountName) {
  K list = knk(3, kis(accountName), kj(position), kf(averageCost));
  jv(&list, knk(4,
                kf(marketPrice),
                kf(marketValue),
                kf(unrealizedPNL),
                kf(realizedPNL)));
  jv(&list,convertContract2(contract));
  receiveData("updatePortfolio", list);
}
void IBClient::pnl(int reqId,
                   double dailyPnL,
                   double unrealizedPnL,
                   double realizedPnL) {
  receiveData("pnl",
              knk(4,
                  kj(reqId),
                  kf(dailyPnL),
                  kf(unrealizedPnL),
                  kf(realizedPnL))
              );
}
void IBClient::pnlSingle(int reqId,
                         int pos,
                         double dailyPnL,
                         double unrealizedPnL,
                         double realizedPnL,
                         double value) {
  K list = knk(4,
               kj(reqId),
               kf(dailyPnL),
               kf(unrealizedPnL),
               kf(realizedPnL));
  jv(&list, knk(2,
                kj(pos),
                kf(value)));
  receiveData("pnlSingle", list);
}
////////////////////////////////////////////////////

void IBClient::tickNews(int tickerId, time_t timeStamp, const std::string& providerCode, const std::string& articleId, const std::string& headline, const std::string& extraData) { }
void IBClient::newsProviders(const std::vector<NewsProvider> &newsProviders) { }
void IBClient::newsArticle(int requestId, int articleType, const std::string& articleText) { }
void IBClient::historicalNews(int requestId, const std::string& time, const std::string& providerCode, const std::string& articleId, const std::string& headline) { }
void IBClient::historicalNewsEnd(int requestId, bool hasMore) { }
void IBClient::orderStatus( OrderId orderId, const std::string& status, double filled,
     double remaining, double avgFillPrice, int permId, int parentId,
     double lastFillPrice, int clientId, const std::string& whyHeld, double mktCapPrice) { }
void IBClient::openOrder( OrderId orderId, const Contract&, const Order&, const OrderState&) { }
void IBClient::openOrderEnd() { }
void IBClient::orderBound(long long orderId, int apiClientId, int apiOrderId) { }
void IBClient::softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers) { }
void IBClient::execDetails( int reqId, const Contract& contract, const Execution& execution) { }
void IBClient::execDetailsEnd( int reqId) { }
void IBClient::deltaNeutralValidation(int reqId, const DeltaNeutralContract& deltaNeutralContract) { }
void IBClient::commissionReport( const CommissionReport& commissionReport) { }
void IBClient::familyCodes(const std::vector<FamilyCode> &familyCodes) { }
void IBClient::receiveFA(faDataType pFaDataType, const std::string& cxml) { }
void IBClient::displayGroupList( int reqId, const std::string& groups) { }
void IBClient::displayGroupUpdated( int reqId, const std::string& contractInfo) { }
void IBClient::verifyMessageAPI( const std::string& apiData) { }
void IBClient::verifyCompleted( bool isSuccessful, const std::string& errorText) { }
void IBClient::verifyAndAuthMessageAPI( const std::string& apiData, const std::string& xyzChallange) { }
void IBClient::verifyAndAuthCompleted( bool isSuccessful, const std::string& errorText) { }
void IBClient::updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch) { }

////////////////////////////////////////////
///       Conversion functions       //////
//////////////////////////////////////////

K IBClient::convertContract(const Contract &contract){
    auto dict = createDictionary(std::map<std::string, K> {
        { "conId",              kj(contract.conId) },
        { "symbol",             kis(contract.symbol) },
        { "secType",            kis(contract.secType) },
        { "lastTradeDateOrContractMonth",kip(contract.lastTradeDateOrContractMonth ) },
        { "strike",             kf(contract.strike) },
        { "right",              kis(contract.right) },
        { "multiplier",         kip(contract.multiplier) },
        { "exchange",           kis(contract.exchange) },
        { "primaryExchange",    kis(contract.primaryExchange) },
        { "localSymbol",        kis(contract.localSymbol) },
        { "tradingClass",       kis(contract.tradingClass) },
        { "includeExpired",     kb(contract.includeExpired) },
        { "secIdType",          kis(contract.secIdType) },
        { "secId",              kis(contract.secId) },
        // TODO: Contract combo fields
        { "comboLegsDescrip",   identity() },
        { "comboLegList",       identity() }
    });
    R dict;
}

K IBClient::convertContract2(const Contract &contract){
  R knk(14,
        kj(contract.conId),
        kis(contract.symbol) ,
        kis(contract.secType) ,
        kip(contract.lastTradeDateOrContractMonth ) ,
        kf(contract.strike) ,
        kis(contract.right) ,
        kip(contract.multiplier) ,
        kis(contract.exchange) ,
        kis(contract.primaryExchange) ,
        kis(contract.localSymbol) ,
        kis(contract.tradingClass) ,
        kb(contract.includeExpired) ,
        kis(contract.secIdType) ,
        kis(contract.secId));
    // TODO: Contract comboLegsDescrip, comboLegList
}

K IBClient::convertContractDetails(const ContractDetails &contract){
  R knk(17,
        kip(contract.marketName),
        kf(contract.minTick) ,
        kip(contract.orderTypes) ,
        kip(contract.validExchanges) ,
        kj(contract.priceMagnifier) ,
        kj(contract.underConId) ,
        kip(contract.longName) ,
        kip(contract.contractMonth) ,
        kip(contract.industry) ,
        kip(contract.category) ,
        kip(contract.subcategory) ,
        kip(contract.timeZoneId) ,
        kip(contract.tradingHours) ,
        kip(contract.liquidHours) ,
        kip(contract.evRule) ,
        kf(contract.evMultiplier),
        kip(contract.marketRuleIds));
    // TODO: secIdList
}
