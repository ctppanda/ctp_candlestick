#include "Position.h"

namespace Pheux { namespace Core {

    Position::Position()
    {
        instrument = "";
        Init();
    }

    Position::Position(const std::string& inst)
        : instrument(inst)
    {
        Init();
    }

    void Position::Init()
    {
        close_profit = 0;
        position_cost = 0;
        position_profit = 0;
        settlement_price = 0;
        user_margin = 0;
        comb_position = 0;
        today_position = 0;
        yesterday_position = 0;
        volume = 0;
        hedge_flag = Position::Speculation;
        direction = Position::Net;
        date = Position::Today;
    }

    Position::Position(CThostFtdcInvestorPositionField* pos)
    {
        Update(pos);
    }

    void Position::Update(CThostFtdcInvestorPositionField* pos)
    {
        // TODO: the first position will be NULL, give the default value.
        if (pos == NULL)
            return;

        broker = pos->BrokerID;
        investor = pos->InvestorID;
        instrument = pos->InstrumentID;
        close_profit = pos->CloseProfit;
        position_cost = pos->PositionCost;
        position_profit = pos->PositionProfit;
        settlement_price = pos->SettlementPrice;
        user_margin = pos->UseMargin;
        comb_position = pos->CombPosition;
        today_position = pos->TodayPosition;
        yesterday_position = pos->YdPosition;
        volume = pos->Position;
        hedge_flag = (HedgeFlag)pos->HedgeFlag;
        direction = (PositionDirection)pos->PosiDirection;
        date = (PositionDate)pos->PositionDate;
    }

    string Position::ToString()
    {
        std::ostringstream oss;
        oss << "(Position)broker=" << broker
            << ",investor=" << investor
            << ",instrument=" << instrument
            << ",volume=" << volume
            << ",direction=" << direction
            << ",close_profit=" << close_profit
            << ",position_cost=" << position_cost
            << ",position_profit=" << position_profit
            << ",settlement_price=" << settlement_price
            << ",user_margin=" << user_margin
            << ",comb_position=" << comb_position
            << ",today_position=" << today_position
            << ",yesterday_position=" << yesterday_position
            << ",hedge_flag=" << hedge_flag
            << ",date=" << date;
        return oss.str();
    }

}}
