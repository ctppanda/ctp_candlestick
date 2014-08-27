#include "MdSpi.h"
#include "MdApi.h"
#include "MdAgent.h"
#include "Tick.h"

namespace Pheux { namespace Core {

    MdSpi::MdSpi(MdAgent* agent)
        : agent(agent), api(agent->api)
    {
        logger->information("MdSpi::MdSpi construct.");
    }

    void MdSpi::OnFrontConnected()
    {
        logger->information("MdSpi::OnFrontConnected.");

        api->ReqUserLogin();
    }

    void MdSpi::OnFrontDisconnected(int nReason)
    {
        logger->information("MdSpi::OnFrontDisconnected.");
    }

    void MdSpi::OnHeartBeatWarning(int nTimeLapse)
    {
        logger->information("MdSpi::OnHeartBeatWarning.");
    }

    void MdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        if (bIsLast && !IsErrorRspInfo(pRspInfo))
        {
            logger->information("MdSpi::OnRspUserLogin: Success.");
            api->SubscribeMarketData(agent->instruments);
        }
    }

    void MdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        //cerr << "===>>> receive market data: " << pSpecificInstrument->InstrumentID << " " << pRspInfo->ErrorMsg << " " << nRequestID << " " << bIsLast << endl;
        //cerr << __FUNCTION__ << endl;
    }

    void MdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        //cerr << __FUNCTION__ << endl;
    }

    void MdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pData)
    {
		logger->information("MdSpi::OnRtnDepthMarketData: Success.");
		cout << pData->InstrumentID << endl;
		cout <<pData->AskPrice1 << endl;
		cout <<pData->LastPrice << endl;
		cout <<pData->Volume << endl;
		cout << "here now tick!" << endl;
        Tick* tick = new Tick(pData);
		tick->ToString();
		cout << "here now tick2!" << endl;
        agent->TickEvent(agent, tick);
    }

    void MdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        IsErrorRspInfo(pRspInfo);
    }

    bool MdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
    {
        bool result = ((pRspInfo) && (pRspInfo->ErrorID != 0));
        if (result)
        {
            string _msg(pRspInfo->ErrorMsg);
            string msg = Poco::format("MdSpi Rsp Error. ErrorID=%d, ErrorMsg=%s", pRspInfo->ErrorID, _msg);
            logger->error(msg);
        }
        return result;
    }

}}
