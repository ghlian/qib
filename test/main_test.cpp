//
// Created by ghlian on 2020/12/28.
//

#include <iostream>
#include <k.h>
#include <Contract.h>
//#include "IBClient.h"
//#include "helpers.h"
//#include "config.h"
//#include "Contract.h"
//#include "Order.h"
//#include "Execution.h"
//#include "ScannerSubscription.h"
//#include <functional>
//#include "qib.h"

using namespace std;

//void reqMktData(TickerId id, const Contract &contract, const IBString &genericTicks, bool snapshot, const TagValueListSPtr& mktDataOptions);
Z Contract createContract(K dict, std::string &error);

int main(int argc, char *argv[]) {
    cout << "Running the main of the testing: " << endl;
    cout << "Trying to run reqMktData: " << endl;
    //    reqMktData();
    //    K reqdata = reqMktData(K id, K contract, K genericTicks, K snapshot);
    K tickerId = kj(100);

    cout << " the value of the kj = " << tickerId->j;

    cout << "Constructiong a contract " << endl;
//    K contractK = XD();
    K key = knk(4, ks("ID"), ks("secType"), ks("exchange"), ks("currency") );
    K value = knk(4, ks("ID"), ks("secType"), ks("exchange"), ks("currency") );
    K contractK = knk(2, key, value);
    std::string error ;
    auto contractC = createContract(contractK, error);

    return 0;
}