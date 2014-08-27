#ifndef MD_SPI_H
#define MD_SPI_H

#include "Util.h"
#include "ThostFtdcMdApi.h"

namespace Pheux { namespace Core {

    class MdApi;
    class MdAgent;

    class MdSpi : public CThostFtdcMdSpi
    {
    public:
        MdSpi(MdAgent*);
        ~MdSpi() {}

    public:
        virtual void OnFrontConnected();

        virtual void OnFrontDisconnected(int);

        virtual void OnHeartBeatWarning(int);

        virtual void OnRspUserLogin(CThostFtdcRspUserLoginField*, CThostFtdcRspInfoField*, int, bool);

        virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField*, CThostFtdcRspInfoField*, int, bool);

        virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField*, CThostFtdcRspInfoField*, int, bool);

        virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField*);

        virtual void OnRspError(CThostFtdcRspInfoField*, int, bool);

    private:
        MdAgent* agent;
        MdApi* api;

        bool IsErrorRspInfo(CThostFtdcRspInfoField*);
    };

}}

#endif
