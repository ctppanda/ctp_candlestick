#include "TraderSpi.h"
#include "TraderAgent.h"
#include "TraderApi.h"
#include "LoginSession.h"
#include "Poco/ScopedLock.h"

namespace Pheux { namespace Core {

    using Poco::Mutex;

    TraderSpi::TraderSpi(TraderAgent* agent)
        : agent(agent), api(agent->api)
    {
        logger->information("TraderSpi::TraderSpi construct.");
    }

    void TraderSpi::OnFrontConnected()
    {
        logger->information("TraderSpi::OnFrontConnected.");

        api->ReqUserLogin();
    }

    void TraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        if (bIsLast && !IsErrorRspInfo(pRspInfo))
        {
            int max = Converter::ToInt(pRspUserLogin->MaxOrderRef);
            LoginSession* session = new LoginSession(
                pRspUserLogin->FrontID,
                pRspUserLogin->SessionID,
                max,
                pRspUserLogin->TradingDay, pRspUserLogin->LoginTime
                );
            Order::max_order_ref = max;
            agent->session = session;

            logger->information(
                Poco::format("TraderSpi::OnRspUserLogin: Success. (Session): front_id=%d, session_id=%d, max_ref=%d.",
                session->front_id, session->session_id, max));
        }
    }

    void TraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        if (bIsLast && !IsErrorRspInfo(pRspInfo))
        {
            logger->information("TraderSpi::OnRspSettlementInfoConfirm: Success");
        }
    }

    void TraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        if (bIsLast && !IsErrorRspInfo(pRspInfo))
        {
            Mutex::ScopedLock slock(mutex);
            //agent->account->Update(pTradingAccount);
        }
    }

    void TraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        if (bIsLast && !IsErrorRspInfo(pRspInfo))
        {
            Mutex::ScopedLock slock(mutex);
            //agent->position->Update(pInvestorPosition);
        }
    }

    //
    void TraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        if (bIsLast && !IsErrorRspInfo(pRspInfo))
        {
            logger->information("TraderSpi::OnRspOrderInsert: Success");
        }
    }

    bool TraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
    {
        bool result = ((pRspInfo) && (pRspInfo->ErrorID != 0));
        if (result)
        {
            string _msg(pRspInfo->ErrorMsg);
            string msg = Poco::format("TraderSpi Rsp Error. ErrorID=%d, ErrorMsg=%s", pRspInfo->ErrorID, _msg);
            logger->error(msg);
        }
        return result;
    }
}}
