#ifndef TRADER_SPI_H
#define TRADER_SPI_H

#include "Util.h"
#include "ThostFtdcTraderApi.h"
#include "Poco/Mutex.h"

namespace Pheux { namespace Core {

    class TraderAgent;
    class TraderApi;

    class TraderSpi : public CThostFtdcTraderSpi
    {
    public:
        TraderSpi(TraderAgent*);
        ~TraderSpi() {}

    public:
        virtual void OnFrontConnected();

        virtual void OnRspUserLogin(CThostFtdcRspUserLoginField*, CThostFtdcRspInfoField*, int, bool);

        virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField*, CThostFtdcRspInfoField*, int, bool);

        virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField*, CThostFtdcRspInfoField*, int, bool);

        virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField*, CThostFtdcRspInfoField*, int, bool);

        virtual void OnRspOrderInsert(CThostFtdcInputOrderField*, CThostFtdcRspInfoField*, int, bool);


        // unhandled
        virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
        {}

        virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

        virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

        virtual void OnFrontDisconnected(int nReason) {}

        virtual void OnHeartBeatWarning(int nTimeLapse) {}

        virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) {}

        virtual void OnRtnTrade(CThostFtdcTradeField *pTrade) {}

    private:
        TraderAgent* agent;
        TraderApi* api;
        Poco::Mutex mutex;

        bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
    };

}}
#endif
