#include "TraderApi.h"

namespace Pheux { namespace Core {

    static string status(int ret)
    {
        return (ret == 0) ? "Success." : "Fail.";
    }

    TraderApi::TraderApi(const string& addr, const string& broker, const string& user, const string& passwd, const string& path)
        : request_id(0)
    {
        proxy = CThostFtdcTraderApi::CreateFtdcTraderApi(path.c_str());
        address = Converter::StringToCharArray(addr);
        broker_id = Converter::StringToCharArray(broker);
        user_id = Converter::StringToCharArray(user);
        password = Converter::StringToCharArray(passwd);

        logger->information(
            Poco::format("TraderApi::TraderApi. address=%s, broker_id=%s, user_id=%s", addr, broker, user));
    }

    void TraderApi::Init(CThostFtdcTraderSpi* spi)
    {
        proxy->RegisterSpi((CThostFtdcTraderSpi*)spi);
        proxy->SubscribePublicTopic(THOST_TERT_QUICK);
        proxy->SubscribePrivateTopic(THOST_TERT_QUICK);
        proxy->RegisterFront(address);
        proxy->Init();

        logger->information("TraderApi::Init.");
    }

    bool TraderApi::ReqUserLogin()
    {
        CThostFtdcReqUserLoginField req;
        memset(&req, 0, sizeof(req));
        strcpy(req.BrokerID, broker_id);
        strcpy(req.UserID, user_id);
        strcpy(req.Password, password);

        int ret = proxy->ReqUserLogin(&req, ++request_id);
        logger->information("TraderApi::ReqUserLogin: " + status(ret));

        return (ret == 0);
    }

    bool TraderApi::ReqSettlementInfoConfirm()
    {
        CThostFtdcSettlementInfoConfirmField req;
        memset(&req, 0, sizeof(req));
        strcpy(req.BrokerID, broker_id);
        strcpy(req.InvestorID, user_id);

        int ret = proxy->ReqSettlementInfoConfirm(&req, ++request_id);
        logger->information("TraderApi::ReqSettlementInfoConfirm: " + status(ret));

        return (ret == 0);
    }

    bool TraderApi::ReqQryTradingAccount()
    {
        CThostFtdcQryTradingAccountField req;
        memset(&req, 0, sizeof(req));
        strcpy(req.BrokerID, broker_id);
        strcpy(req.InvestorID, user_id);

        int ret = proxy->ReqQryTradingAccount(&req, ++request_id);
        logger->information("TraderApi::ReqQryTradingAccount: " + status(ret));

        return (ret == 0);
    }

    bool TraderApi::ReqQryInvestorPosition(const std::string& inst)
    {
        CThostFtdcQryInvestorPositionField req;
        memset(&req, 0, sizeof(req));
        strcpy(req.BrokerID, broker_id);
        strcpy(req.InvestorID, user_id);
        strcpy(req.InstrumentID, inst.c_str());

        int ret = proxy->ReqQryInvestorPosition(&req, ++request_id);
        logger->information("TraderApi::ReqQryInvestorPosition: " + status(ret));

        return (ret == 0);
    }


    bool TraderApi::ReqOrderInsert(const Order* order)
    {
        CThostFtdcInputOrderField req;
        memset(&req, 0, sizeof(req));
        strcpy(req.BrokerID, order->broker.c_str());
        strcpy(req.InvestorID, order->investor.c_str());
        strcpy(req.InstrumentID, order->instrument.c_str());
        sprintf(req.OrderRef, "%d", ++(order->max_order_ref));

        req.LimitPrice = order->limit_price;
        req.StopPrice = order->stop_price;
        req.VolumeTotalOriginal = order->volume;
        req.Direction = order->direction;
        req.CombOffsetFlag[0] = order->open_close;
        req.ContingentCondition = order->condition;

        req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
        req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
        req.TimeCondition = THOST_FTDC_TC_GFD;
        req.VolumeCondition = THOST_FTDC_VC_AV;
        req.MinVolume = 1;
        req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
        req.IsAutoSuspend = 0;
        req.UserForceClose = 0;

        int ret = proxy->ReqOrderInsert(&req, ++request_id);
        logger->information("TraderApi::ReqOrderInsert: " + status(ret));

        return (ret == 0);
    }
}}
