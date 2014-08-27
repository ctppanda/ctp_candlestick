#ifndef POSITION_H
#define POSITION_H

#include "ThostFtdcTraderApi.h"
#include "Util.h"

namespace Pheux { namespace Core {

    // CThostFtdcInvestorPositionField
    class Position
    {
    public:
        // TFtdcHedgeFlagType
        enum HedgeFlag
        {
            Speculation = 49,
            Arbitrage = 50,
            Hedge = 51,
        };

        // TFtdcPosiDirectionType
        enum PositionDirection
        {
            Net = 49,    // '1'
            Long = 50,    // '2'
            Short = 51,    // '3'
        };

        // TFtdcPositionDateType
        enum PositionDate
        {
            Today = 49,
            History = 50,
        };

    public:
        std::string broker;
        std::string investor;
        std::string instrument;
        double close_profit;
        double position_cost;
        double position_profit;
        //double price;
        double settlement_price;
        double user_margin;
        int comb_position;
        int today_position;
        int yesterday_position;
        int volume;
        HedgeFlag hedge_flag;
        PositionDirection direction;
        PositionDate date;

    public:
        Position();

        Position(const std::string& inst);

        Position(CThostFtdcInvestorPositionField* pos);

        void Update(CThostFtdcInvestorPositionField* pos);

        string ToString();

    private:
        void Init();
    };

}}
#endif
