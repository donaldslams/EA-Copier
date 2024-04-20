#include <iostream>
#include <chrono>
#include <thread>
#ifdef _X64
#include "../output/fortexws64.h"
#else
#include "../output/fortexws32.h"
#endif

const char* szServer = nullptr;
const char* apiKey = nullptr;
const char* password = nullptr;
const char* account = nullptr;

double bid_price = 0.0;
void OnLogin() {
    std::cout << "login retCode:" << std::endl;
}
void OnLogout() {
    std::cout << "logout." << std::endl;
}
const char* name = "";
void OnAccountInfo(AccountInfo* acc) {
    std::cout << "===========account info============" << std::endl;
    std::cout << "account info:" << acc->Login << std::endl;
    for (int i = 0; i < acc->Linked_account_len; i++) {
        std::cout << "linked account:" << acc->Linked_account[i].Account << std::endl;
    }
    name = acc->Login;
    FortexObject msg;
    msg.Type = OBJ_TYPE_ACCOUNT_INFO;
    msg.Message = (char*)acc;
    ReleaseFortexObject(msg);
}

void OnSymbolList(Symbol* sym, int count) {
    std::cout << "===========onsymbolinfo============" << std::endl;
    std::cout << "onsymbolinfo:" << count << std::endl;
    for (int i = 0; i < count; i++) {
        std::cout << sym[i].Name << "."<< sym[i].Tenor << "." << sym[i].BaseCcy << "." << sym[i].TermCcy << "." << sym[i].MarginCcy << std::endl;
    }
    FortexObject msg;
    msg.Type = OBJ_TYPE_SYMBOL;
    msg.Message = (char*)sym;
    msg.ExtraInt = count;
    ReleaseFortexObject(msg);
}

void OnMarketData(MarketData* md) {
    std::cout << "==========onmarketdata=============" << std::endl;
    std::cout << "onmarketdata:" <<md->Type<< std::endl;
    std::cout << md->Domain;
    if (md->Sym != NULL) {
        std::cout << "." << md->Sym->Name;
    }
    std::cout << "." << md->Hight << "." << md->Low << "." << md->Open << "." << md->Close << "." << md->Domain << std::endl;
    for (int i = 0; i < md->Asks_len; i++) {
        std::cout << md->Asks[i].Price << "." << md->Asks[i].Size << std::endl;
    }
    for (int i = 0; i < md->Bids_len; i++) {
        std::cout << md->Bids[i].Price << "." << md->Bids[i].Size << std::endl;
    }
    bid_price = md->Hight;
    FortexObject obj;
    obj.Type = OBJ_TYPE_MARKET_DATA;
    obj.Message = (char*)md;
    ReleaseFortexObject(obj);
}

void OnNetPostion(NetPosition* pos, int count)  {
    std::cout << "==========OnNetPostion=============" << std::endl;
    std::cout << "onnetposition:" << count << std::endl;
    for (int i = 0; i < count; i++) {
        std::cout << pos[i].Account << "." << pos[i].SymName << std::endl;
    }
    FortexObject msg;
    msg.Type = OBJ_TYPE_NET_POSITION;
    msg.Message = (char*)pos;
    msg.ExtraInt = count;
    ReleaseFortexObject(msg);
}

void OnOrderList(Order* ord, int count, int is_required) {
    std::cout << "==========OnOrderList=============" << std::endl;
    std::cout << "onorderinfo:" << count << std::endl;
    for (int i = 0; i < count; i++) {
        std::cout << ord[i].Account << "." << ord[i].SymName << "." << ord[i].ClientOrderID << "." << ord[i].TicketID << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    for (int i=0; i<count; i++) {
        if ( i % 3 == 0) {
            std::cout << "Call ModifyOrder " << std::endl;
            ord[i].Quantity = 1000;
            int ret = ModifyOrder(ord[i].Account, ord[i]);
            if (ret != 0) {
                std::cout << GetLastErrorMessage() << std::endl;
                return;
            } 
        } else { 
            int ret = CancelOrderEx(ord[i].Account,
                ord[i].User,
                ord[i].ClientOrderID,
                ord[i].Type,
                ord[i].Side,
                ord[i].Quantity,
                ord[i].SymName,
                5,
                ord[i].ExecDest,
                ord[i].Tag,
                ord[i].Tif
            );
            if (ret != 0) {
                std::cout << GetLastErrorMessage() << std::endl;
            }
        }
    }
    FortexObject msg;
    msg.Type = OBJ_TYPE_ORDER;
    msg.Message = (char*)ord;
    msg.ExtraInt = count;
    ReleaseFortexObject(msg);
}

void OnNetTicket(NetTicket* ticket, int count) {
    std::cout << "==========OnNetTicket=============" << std::endl;
    std::cout << "onnetticket:" << count << std::endl;
    for (int i = 0; i < count; i++) {
        std::cout << ticket[i].Account << "." << ticket[i].SymName << "." << ticket[i].Side << std::endl;
    }
    FortexObject msg;
    msg.Type = OBJ_TYPE_NET_TICKET;
    msg.Message = (char*)ticket;
    msg.ExtraInt = count;
    ReleaseFortexObject(msg);
}

void OnTicket(Ticket* ticket, int count) {
    std::cout << "==========OnTicket=============" << std::endl;
    std::cout << "onticket:" << count << std::endl;
    for (int i = 0; i < count; i++) {
        std::cout << ticket[i].Account << "." << ticket[i].SymName << std::endl;
    }
    FortexObject msg;
    msg.Type = OBJ_TYPE_TICKET;
    msg.Message = (char*)ticket;
    msg.ExtraInt = count;
    ReleaseFortexObject(msg);
}

void OnExecutionReport(ExecutionReport* report) {
    std::cout << "==========OnExecutionReport=============" << std::endl;
    std::cout << "onexecutionreport:" << report->Account << std::endl;
    std::cout << report->Account << "." << report->SymName << "." << report->ClientOrdId << std::endl;
    int ret = CancelOrderEx(report->Account,
        report->OrigOrdUser,
        report->OrdId,
        report->OrdType,
        report->Side,
        report->OriginOrdQty,
        report->SymName,
        5,
        report->OriginExecDst,
        report->OrdTag,
        report->OriginTif
    );
    if (ret != 0) {
        std::cout << GetLastErrorMessage() << std::endl;
    }

    FortexObject obj;
    obj.Type = OBJ_TYPE_EXECUTION_REPORT;
    obj.Message = (char*)report;
    ReleaseFortexObject(obj);
}

void OnExectutionData(ExecutionAllData* exec) {
    std::cout << "==========OnExectutionData=============" << std::endl;

    if(exec->OrdInfo != NULL) {
        std::cout << exec->OrdInfo->Account << "." << exec->OrdInfo->SymName << "." << exec->OrdInfo->ClientOrderID << "." << exec->OrdInfo->TicketID << std::endl;
        // std::cout << "onexecutiondata:" << exec->OrdInfo->Account << std::endl;
        // int ret = CancelOrderEx(exec->OrdInfo->Account,
        //     exec->OrdInfo->Account,
        //     exec->OrdInfo->FortexClientOrderID,
        //     exec->OrdInfo->Type,
        //     exec->OrdInfo->Side,
        //     exec->OrdInfo->Quantity,
        //     exec->OrdInfo->SymName,
        //     5,
        //     exec->OrdInfo->ExecDest,
        //     exec->OrdInfo->Tag,
        //     exec->OrdInfo->Tif
        // );
        // if (ret != 0) {
        //     std::cout << GetLastErrorMessage() << std::endl;
        // }
    }
    FortexObject obj;
    obj.Type = OBJ_TYPE_EXECUTION_ALL_DATA;
    obj.Message = (char*)exec;
    ReleaseFortexObject(obj);
    std::cout << "onexecutiondata" << std::endl;
}

void OnChartData(ChartData* chart) {
    std::cout << "==========OnChartData=============" << std::endl;
    std::cout << "onchartdata:" << chart->Bars_len << std::endl;
    if (chart->Bars != NULL) {
        for (int i = 0; i<chart->Bars_len; i++) {
            std::cout << chart->Bars[i].High << "." << chart->Bars[i].Low << "." << chart->Bars[i].Open << "." << chart->Bars[i].Close << "." << chart->Bars[i].Volume << std::endl;
        }
    }
    FortexObject obj;
    obj.Type = OBJ_TYPE_CHART_DATA;
    obj.Message = (char*)chart;
    ReleaseFortexObject(obj);
}

void OnEvent(Event* event, int ecount, char** type, int tcount) {
    std::cout << "==========OnOrderList=============" << std::endl;
    std::cout << "onevent:" << std::endl;
    for (int i = 0; i < ecount; i++) {
        std::cout << type[i] << std::endl;
    }
    FortexObject msg;
    msg.Type = OBJ_TYPE_EVENT;
    msg.Message = (char*)event;
    msg.ExtraInt = ecount;
    ReleaseFortexObject(msg);
    for (int i = 0; i < tcount; i++) {
        std::cout << type[i] << " ";
        free(type[i]);
    
    free(type);
    }
}

void onMarketData(MarketData* md) {
    std::cout << "==========onmarketdata =" << std::endl;
    if (md->Sym != NULL) {
        std::cout << "onmarketdata:" << md->Sym->Name <<"." << md->Hight << "." << md->Low << std::endl;
    }
    FortexObject obj;
    obj.Type = OBJ_TYPE_MARKET_DATA;
    obj.Message = (char*)md;
    ReleaseFortexObject(obj);
}

void onRejectData(FortexErrorCode* code) {
    std::cout << "==========onRejectData=" << std::endl;
    std::cout << "onRejectData:" << code->ErrorMessage << "." << code->ErrorCode << std::endl;
    FortexObject obj;
    obj.Type = OBJ_TYPE_FORTEX_ERROR_CODE;
    obj.Message = (char*)code;
    ReleaseFortexObject(obj);
}

void onOrderReject(FortexErrorCode* code, FortexOrderRejectInfo* info) {
    std::cout << "==========onOrderReject=" << std::endl;
    std::cout << "onOrderReject:" << code->ErrorMessage << std::endl;
    std::cout << "onOrderReject:" << info->Acct << "." << info->ClOrdId << "." << info->CxlRejRsn << std::endl;
    FortexObject obj;
    obj.Type = OBJ_TYPE_FORTEX_ERROR_CODE;
    obj.Message = (char*)code;
    ReleaseFortexObject(obj);
    FortexObject obj1;
    obj1.Type = OBJ_TYPE_FORTEX_ORDER_REJECT_INFO;
    obj1.Message = (char*)info;
    ReleaseFortexObject(obj1);
}

void callback_sample() {
    RegisterEventAccountInfo(OnAccountInfo);
    RegisterEventSymbolList(OnSymbolList);
    RegisterEventNetPosition(OnNetPostion);
    RegisterEventLogin(OnLogin);
    RegisterEventLogout(OnLogout);
    RegisterEventOpenOrder(OnOrderList);
    RegisterEventNetTicket(OnNetTicket);
    RegisterEventExecutionReport(OnExecutionReport);
    RegisterEventExecutionData(OnExectutionData);
    RegisterEventChartData(OnChartData);
    RegisterEventEvent(OnEvent);
    RegisterEventTicket(OnTicket);
    RegisterEventMarketData(onMarketData);
    RegisterEventRejectData(onRejectData);
    RegisterEventOrdRejectData(onOrderReject);


    std::cout << "test fortex library." << std::endl;    
    Init((char*)szServer, 1);
    int ret = Connect();
    if (ret != 0) {
        std::cout << GetLastErrorMessage() << std::endl;
        return;
    }
    ret = Login((char*)apiKey, (char*)password);
    if (ret != 0) {
        std::cout << GetLastErrorMessage() << std::endl;
        return;
    } else {
        std::cout << "login success." << std::endl;
    }
    ret = -1;
    for (int i = 0; i<100; i++) {
        ret = GetState();
        std::cout << "state:" << ret << std::endl;
        if (GetState() != CONNECT_STATUS::LoginDone) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        break;
    }
    if (ret !=CONNECT_STATUS::LoginDone) {
        return;
    }
    Symbol sym;
    sym.Name = (char*)"EUR/USD";
    sym.Tenor = (char*) "";
    sym.BaseCcy = (char*) "";
    sym.TermCcy = (char*) "";
    sym.MarginCcy = (char*) "";

    ret = SubscribeMarketData(sym, L1, 0);
    if (ret != 0) {
        std::cout << GetLastErrorMessage() << std::endl;
        return;
    }
    ChartRequest req;
    req.Sym = sym.Name;
    req.ChartType = MINUTE;
    req.Days = 2;
    req.Id = 1;

    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    req.EndTime = millis;
    req.GroupSize = 15;
    ret = QueryChart(req);
    if (ret != 0) {
        std::cout << GetLastErrorMessage() << std::endl;
        return;
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));

    sym.SecType = 5;
    ret = PlaceMarketOrder((char*)account, SELL, 100, sym);
    if (ret != 0) {
        std::cout << GetLastErrorMessage() << std::endl;
        return;
    }   

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "Call PlaceMarketOrder " << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ret = PlaceMarketOrder((char*)account, SELL, 100, sym);
        if (ret != 0) {
            std::cout << GetLastErrorMessage() << std::endl;
            return;
        }
    
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ret = PlaceLimitOrder((char*)account, BUY, 100, sym, 1.2);
        if (ret != 0) {
            std::cout << GetLastErrorMessage() << std::endl;
            return;
        }
        std::cout << name << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
        ret = PlaceStopOrder((char*)account, BUY, 100, sym, 1.2);
        if (ret != 0) {
            std::cout << GetLastErrorMessage() << std::endl;
            return;
        }
        std::cout << name << std::endl;
    }

    //std::this_thread::sleep_for(std::chrono::seconds(30));
}

void polling_sample() {
    std::cout << "test fortex library." << std::endl;
    Init((char*)szServer, 2);
    int ret = Connect();
    if (ret != 0) {
        std::cout << GetLastErrorMessage() << std::endl;
        return;
    }
    ret = Login((char*)apiKey, (char*)password);
    if (ret != 0) {
        std::cout << GetLastErrorMessage() << std::endl;
        return;
    } else {
        std::cout << "login success." << std::endl;
    }
    ret = -1;
    for (int i = 0; i<100; i++) {
        ret = GetState();
        std::cout << "state:" << ret << std::endl;
        if (GetState() != CONNECT_STATUS::LoginDone) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        break;
    }
    if (ret !=CONNECT_STATUS::LoginDone) {
        return;
    }

    Symbol sym;
    sym.Name = (char*)"EUR/USD";
    sym.Tenor = (char*) "";
    sym.BaseCcy = (char*) "";
    sym.TermCcy = (char*) "";
    sym.MarginCcy = (char*) "";
    sym.SecType = 5;
    // ret = QueryOpenOrders((char*)account);
    // if (ret != 0) {
    //     std::cout << GetLastErrorMessage() << std::endl;
    //     return;
    // }
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    ret = SubscribeMarketData(sym, L1, 0);
    if (ret != 0) {
        std::cout << GetLastErrorMessage() << std::endl;
        return;
    }
    int counter = 0;
//    for (int i = 0; i<100; i++) {
    while(true){
        int i = 0;
        FortexObject msg = GetMessageFromServer();
        std::cout << "FM: idx=" << i++ << ", msg=" << msg.Type << std::endl;

        switch (msg.Type) {
        case OBJ_TYPE_ACCOUNT_INFO: {
            std::cout << "account info:" << std::endl;
            AccountInfo *accountInfo = (AccountInfo*)msg.Message;
            std::cout << accountInfo->Login << std::endl;
            for (int i = 0; i <accountInfo->Linked_account_len; i++) {
                std::cout << accountInfo->Linked_account[i].Account << std::endl;
            }
            ReleaseFortexObject(msg);
            break;
        }
        case OBJ_TYPE_SYMBOL: {
            std::cout << "symbol:" << std::endl;
            Symbol *symbol = (Symbol*)msg.Message;
            std::cout << symbol->Name << "." << symbol->SecType << std::endl;
            ReleaseFortexObject(msg);
            break;
        }
        case OBJ_TYPE_MARKET_DATA: {
            std::cout << "market data:" << std::endl;
            MarketData *marketData = (MarketData*)msg.Message;
            std::cout << marketData->Sym->Name << "." << marketData->Hight << "." << marketData->Low << std::endl;
            ReleaseFortexObject(msg);
            break;
        }
        case OBJ_TYPE_NET_POSITION: {
            std::cout << "net position:" << std::endl;
            NetPosition *netPosition = (NetPosition*)msg.Message;
            std::cout << netPosition->Account << "." << netPosition->SymName << std::endl;
            ReleaseFortexObject(msg);
            break;
        }
        case OBJ_TYPE_ORDER:
        { 
            Order* order = (Order*)msg.Message;
            if (counter % 3 == 0) {
                std::cout << "Call ModifyOrder " << std::endl;
                order->Quantity = 1000;
                ret = ModifyOrder(order->Account, *order);
                if (ret != 0) {
                    std::cout << GetLastErrorMessage() << std::endl;
                    return;
                }
            } else {         
                std::cout << "Call CancelOrder " << std::endl;
                std::cout << order->OrigUser << "." << order->OrigClientOrderID << "." << order->Type << "." << order->Side << "." << order->Quantity << "." << order->ExecDest << "." << order->OrigTag << "." << order->Tif << std::endl;
                if (order->Sym != nullptr) {
                    // (char*)account, (char*)account, (char*)("1"), 1, SELL, 100, sym, (char*)"INTX", (char*)"", 1
                    ret = CancelOrder(order->Account, 
                    order->OrigUser, 
                    order->ClientOrderID, 
                    order->Type,/*orderType is missing*/ 
                    order->Side,/* what is the order side*/
                    order->Quantity, /* what is the orderqty?*/
                    order->Sym, 
                    order->ExecDest, 
                    order->Tag, 
                    order->Tif /*tif is missing*/);
                    std::cout << "CancelOrder result: " << ret << std::endl;
                    if (ret != 0) {
                    std::cout << GetLastErrorMessage() << std::endl;
                    return;
                    }
                }
            }
            counter++;
            ReleaseFortexObject(msg);
        }
            break;
        case OBJ_TYPE_NET_TICKET: {
            std::cout << "net ticket:" << std::endl;
            NetTicket *netTicket = (NetTicket*)msg.Message;
            std::cout << netTicket->Account << "." << netTicket->SymName << "." << netTicket->Side << std::endl;
            ReleaseFortexObject(msg);
            break;
        }
        case OBJ_TYPE_EXECUTION_REPORT: {
            std::cout << "execution report:" << std::endl;
            ExecutionReport *executionReport = (ExecutionReport*)msg.Message;
            std::cout << executionReport->Account << "." << executionReport->SymName << "." << executionReport->ClientOrdId << std::endl;
            int ret = CancelOrderEx(executionReport->Account,
                executionReport->OrigOrdUser,
                executionReport->OrdId,
                executionReport->OrdType,
                executionReport->Side,
                executionReport->OriginOrdQty,
                executionReport->SymName,
                5,
                executionReport->OriginExecDst,
                executionReport->OrdTag,
                executionReport->OriginTif
            );
            if (ret != 0) {
                std::cout << GetLastErrorMessage() << std::endl;
                return;
            }
            ReleaseFortexObject(msg);
            break;
        }
        case OBJ_TYPE_EXECUTION_ALL_DATA: {
            std::cout << "execution all data:" << std::endl;
            ExecutionAllData *executionAllData = (ExecutionAllData*)msg.Message;
            std::cout << executionAllData->Login << std::endl;
            for (int i = 0; i < executionAllData->NetPos_len; i++) {
                std::cout << executionAllData->NetPos[i].Account << "." << executionAllData->NetPos[i].SymName << std::endl;
            }
            for (int i = 0; i<executionAllData->NetTicket_len; i++) {
                std::cout << executionAllData->NetTicket[i].Account << "." << executionAllData->NetTicket[i].SymName << "." << executionAllData->NetTicket[i].Side << std::endl;
            }
            ReleaseFortexObject(msg);
            break;
        }
        case OBJ_TYPE_EVENT: {
            std::cout << "event:" << std::endl;
            Event *event = (Event*)msg.Message;
            std::cout << event->Type <<"." << event->Account << std::endl;
            ReleaseFortexObject(msg);
            break;
        }
        case OBJ_TYPE_FORTEX_ERROR_CODE: {
            std::cout << "onRejectData:" << ((FortexErrorCode*)msg.Message)->ErrorMessage << std::endl;
            FortexErrorCode* code = (FortexErrorCode*)msg.Message;
            std::cout << "onRejectData:" << code->ErrorMessage << "." << code->ErrorCode << std::endl;
            ReleaseFortexObject(msg);
            break;
        }
        default:
            std::cout << "unknown message type." << std::endl;
            break;
        }

        MarketData* md = GetMarketData();
        std::cout << "MD: idx=" << i << ", msg=" << md << std::endl;
        if (md != nullptr) {
            std::cout << "md:" << md->Type <<  std::endl;
            FortexObject obj;
            obj.Type = OBJ_TYPE_MARKET_DATA;
            obj.Message = (char*)md;
            ReleaseFortexObject(obj);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        std::cout << "Call EnterDoneawayTrade " << std::endl;
        ret = EnterDoneawayTrade((char*)account, sym.Name,TICKET_DIRECTION::ToOpen, (char*)("123"), ORDER_SIDE::BUY, 1000, 1.2);
        if (ret != 0) {
          std::cout << GetLastErrorMessage() << std::endl;
          return;
        }

        std::cout << "Call PlaceMarketOrder " << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ret = PlaceMarketOrder((char*)account, SELL, 100, sym);
        if (ret != 0) {
          std::cout << GetLastErrorMessage() << std::endl;
          return;
        }

        std::cout << "Call PlaceLimitOrder " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        ret = PlaceLimitOrder((char*)account, BUY, 100, sym, 1.0);
        if (ret != 0) {
          std::cout << GetLastErrorMessage() << std::endl;
          return;
        }

        std::cout << "Call PlaceStopOrder " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        ret = PlaceStopOrder((char*)account, BUY, 100, sym, 1.2);
        if (ret != 0) {
          std::cout << GetLastErrorMessage() << std::endl;
          return;
        }
    }
}

void polling_sample_2() {
    std::cout << "test fortex library." << std::endl;
    Init((char*)szServer, 2);
    int ret = Connect();
    if (ret != 0) {
        std::cout << GetLastErrorMessage() << std::endl;
        return;
    }
    ret = Login((char*)apiKey, (char*)password);
    if (ret != 0) {
        std::cout << GetLastErrorMessage() << std::endl;
        return;
    }
    else {
        std::cout << "login success." << std::endl;
    }
    ret = -1;
    for (int i = 0; i < 100; i++) {
        ret = GetState();
        std::cout << "state:" << ret << std::endl;
        if (GetState() != CONNECT_STATUS::LoginDone) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        break;
    }
    if (ret != CONNECT_STATUS::LoginDone) {
        return;
    }

    char* symName = (char*)"EUR/USD";

    ret = SubscribeMarketDataEx(1, symName, L1, 0);
    if (ret != 0) {
        std::cout << GetLastErrorMessage() << std::endl;
        return;
    }

    int counter = 0;
    while (true) {
        int i = 0;

        while (true) {
          FortexObject msg = GetMessageFromServer();
          if (msg.Message == nullptr) {
            std::cout << "no message" << std::endl;
            break;
          }
          std::cout << "FM: idx=" << i++ << ", msg=" << msg.Type << std::endl;

          switch (msg.Type) {            
          case OBJ_TYPE_ACCOUNT_INFO: {
              std::cout << "account info:" << std::endl;
              AccountInfo *accountInfo = (AccountInfo*)msg.Message;
              std::cout << accountInfo->Login << std::endl;
              for (int i = 0; i <accountInfo->Linked_account_len; i++) {
                  std::cout << accountInfo->Linked_account[i].Account << std::endl;
              }
              std::cout << accountInfo->Value.Account << accountInfo->Value.Balance << std::endl;
              ReleaseFortexObject(msg);
              break;
          }
          case OBJ_TYPE_SYMBOL: {
              std::cout << "symbol info:" << std::endl;
              Symbol *symbol = (Symbol*)msg.Message;
              std::cout << symbol->Name << "." << symbol->SecType  << std::endl;
              ReleaseFortexObject(msg);
              break;
          }
          case OBJ_TYPE_MARKET_DATA: {
              std::cout << "market data:" << std::endl;
              MarketData *marketData = (MarketData*)msg.Message;
              std::cout << marketData->Sym->Name << "." << marketData->Hight << "." << marketData->Low << std::endl;
              for (int i =0; i< marketData->Asks_len; i++) {
                  std::cout << marketData->Asks[i].Price << "." << marketData->Asks[i].Size << std::endl;
              }
              for (int i = 0; i < marketData->Bids_len; i++) {
                  std::cout << marketData->Bids[i].Price << "." << marketData->Bids[i].Size << std::endl;
              }
              ReleaseFortexObject(msg);
              break;
          }
          case OBJ_TYPE_NET_POSITION: {
              std::cout << "net position:" << std::endl;
              NetPosition *netPosition = (NetPosition*)msg.Message;
              std::cout << netPosition->Account << netPosition->SymName << std::endl;
              if (netPosition->Sym != nullptr) {
                  std::cout << netPosition->Sym->Name << std::endl;
              }
              ReleaseFortexObject(msg);
              break;
          }
          case OBJ_TYPE_ORDER: {
            Order* order = (Order*)msg.Message;
            if (counter % 3 == 0) {
                std::cout << "Call ModifyOrder " << std::endl;
                order->Quantity = 1000;
                ret = ModifyOrder(order->Account, *order);
                if (ret != 0) {
                    std::cout << GetLastErrorMessage() << std::endl;
                    return;
                }
            } else {
                std::cout << "Call CancelOrderEx " << std::endl;
                // (char*)account, (char*)account, (char*)("1"), 1, SELL, 100, sym, (char*)"INTX", (char*)"", 1
                //CancelOrderEx(char* acct, char* origOrdUser, char* origOrdClOrder, int origOrdType, int origOrdSide, double origOrdQty, char* symName, int secType, char* origOrExecDest, char* origOrdTag, int tif);
                ret = CancelOrderEx(order->Account,
                order->User,
                order->ClientOrderID,
                order->Type,
                order->Side,
                order->Quantity,
                order->SymName,
                SECURITY_TYPE::SecurityTypeFOR, // asset type should match the symbol
                order->ExecDest,
                order->Tag,
                order->Tif);
                if (ret != 0) {
                std::cout << GetLastErrorMessage() << std::endl;
                return;
                }
            }
            counter++;
            ReleaseFortexObject(msg);
            break;
          }
          case OBJ_TYPE_NET_TICKET: {
              NetTicket *netTicket = (NetTicket*)msg.Message;
              std::cout << "net ticket:" << std::endl;
              std::cout << netTicket->Account <<"." << netTicket->SymName <<"."<<netTicket->Side << std::endl;
              if (netTicket->Sym != nullptr) {
                  std::cout << netTicket->Sym->Name << std::endl;
              }
              ReleaseFortexObject(msg);
              break;
          }
          case OBJ_TYPE_EXECUTION_REPORT: {
              ExecutionReport *report = (ExecutionReport*)msg.Message;
              std::cout << "Call CancelOrder " << std::endl;
              ret = CancelOrderEx(report->Account,
                  report->OrigOrdUser,
                  report->OrigClientOrdId,
                  report->OrdType,
                  report->Side,
                  report->OrdQty,
                  report->SymName,
                  5,
                  report->OriginExecDst,
                  report->OrdTag,
                  report->OriginTif
              );

            ReleaseFortexObject(msg);
            break;
        }
        case OBJ_TYPE_EXECUTION_ALL_DATA: {
            ExecutionAllData *data = (ExecutionAllData*)msg.Message;
            std::cout << "Call CancelOrder " << std::endl;
            Order* order = data->OrdInfo;
            if(order != nullptr) {  
                Symbol sym;
                sym.Name = nullptr;
                sym.Tenor = nullptr;
                sym.BaseCcy = nullptr;
                sym.TermCcy = nullptr;
                sym.MarginCcy = nullptr;
                if (order->Sym != nullptr) {
                    sym.Name = order->Sym->Name;
                    sym.SecType = order->Sym->SecType;
                } else {
                    sym.Name = order->SymName;
                    sym.SecType = SECURITY_TYPE::SecurityTypeFOR;
                }
                ret = CancelOrder(order->Account,
                order->OrigUser,
                order->OrigClientOrderID,
                order->Type,/*orderType is missing*/
                order->Side,/* what is the order side*/
                order->Quantity, /* what is the orderqty?*/
                &sym,
                order->ExecDest,
                order->OrigTag,
                order->Tif /*tif is missing*/);
                if (ret != 0) {
                    std::cout << GetLastErrorMessage() << std::endl;
                    return;
                }
            }
            ReleaseFortexObject(msg);
            break;
        }
        case OBJ_TYPE_EVENT: {
            std::cout << "event:" << std::endl;
            Event *event = (Event*)msg.Message;
            std::cout << event->Type <<"." << event->Account << std::endl;
            ReleaseFortexObject(msg);
            break;
        }
        case OBJ_TYPE_FORTEX_ERROR_CODE: {
            std::cout << "onRejectData:" << ((FortexErrorCode*)msg.Message)->ErrorMessage << std::endl;
            FortexErrorCode* code = (FortexErrorCode*)msg.Message;
            std::cout << "onRejectData:" << code->ErrorMessage << "." << code->ErrorCode << std::endl;
            ReleaseFortexObject(msg);
            break;
        }
        case OBJ_TYPE_FORTEX_ORDER_REJECT_INFO: {
            std::cout << "onOrderReject:" << std::endl;
            FortexOrderRejectInfo* rejectInfo = (FortexOrderRejectInfo*)msg.Message;
            std::cout << rejectInfo->Acct << "." << rejectInfo->ClOrdId << "." << rejectInfo->CxlRejRespTo << "." << rejectInfo->CxlRejRsn << std::endl;
            ReleaseFortexObject(msg);
            break;
        }
        default:
            std::cout << "unknown message type." << std::endl;
            ReleaseFortexObject(msg);
            break;
          }
        }

        MarketData* md = GetMarketData();
        std::cout << "MD: idx=" << i << ", msg=" << md << std::endl;
        if (md != nullptr) {
            std::cout << "md:" << md->Type << std::endl;
            FortexObject obj;
            obj.Type = OBJ_TYPE_MARKET_DATA;
            obj.Message = (char*)md;
            ReleaseFortexObject(obj);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        std::cout << "Call EnterDoneawayTrade " << std::endl;
        ret = EnterDoneawayTrade((char*)account, symName, TICKET_DIRECTION::ToOpen, (char*)("123"), ORDER_SIDE::BUY, 1000, 1.2);
        if (ret != 0) {
            std::cout << GetLastErrorMessage() << std::endl;
            return;
        }

        std::cout << "Call PlaceMarketOrder " << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ret = PlaceMarketOrderEx((char*)account, SELL, 100, symName, SECURITY_TYPE::SecurityTypeFOR);
        if (ret != 0) {
            std::cout << GetLastErrorMessage() << std::endl;
            return;
        }

        std::cout << "Call PlaceStopOrder " << std::endl;
        //std::this_thread::sleep_for(std::chrono::seconds(5));
        ret = PlaceStopOrderEx((char*)account, BUY, 100, symName, SECURITY_TYPE::SecurityTypeFOR, 1.2);
        if (ret != 0) {
            std::cout << GetLastErrorMessage() << std::endl;
            return;
        }

        std::cout << "Call PlaceThresholdOrder" << std::endl;
        ret = PlaceThresholdOrderEx((char*)account, BUY, 100, symName, SECURITY_TYPE::SecurityTypeFOR, 1.2, 1.0);
        if (ret != 0) {
            std::cout << GetLastErrorMessage() << std::endl;
            return;
        }

        std::cout << "Call PlaceLimitOrder " << std::endl;
        //std::this_thread::sleep_for(std::chrono::seconds(5));
        ret = PlaceLimitOrderEx((char*)account, BUY, 100, symName, SECURITY_TYPE::SecurityTypeFOR, 1.0);
        if (ret != 0) {
          std::cout << GetLastErrorMessage() << std::endl;
          return;
        }
    }
}

void demo(){
  std::cout << "test fortex library." << std::endl;
  Init((char*)szServer, 2);
  int ret = Connect();
  if (ret != 0) {
    std::cout << GetLastErrorMessage() << std::endl;
    return;
  }
  ret = Login((char*)apiKey, (char*)password);
  if (ret != 0) {
    std::cout << GetLastErrorMessage() << std::endl;
    return;
  }
  else {
    std::cout << "login success." << std::endl;
  }
  ret = -1;
  for (int i = 0; i < 100; i++) {
    ret = GetState();
    std::cout << "state:" << ret << std::endl;
    if (GetState() != CONNECT_STATUS::LoginDone) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      continue;
    }
    break;
  }
  if (ret != CONNECT_STATUS::LoginDone) {
    return;
  }

  char* symName = (char*)"EUR/USD";

  while (true) {
    std::cout << "Call PlaceMarketOrder buy EUR/USD, 1000" << std::endl;
    ret = PlaceMarketOrderEx((char*)account, BUY, 1000, symName, SECURITY_TYPE::SecurityTypeFOR);
    if (ret != 0) {
      std::cout << GetLastErrorMessage() << std::endl;
      return;
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "Call PlaceMarketOrder sell EUR/USD, 1000" << std::endl;
    ret = PlaceMarketOrderEx((char*)account, SELL, 1000, symName, SECURITY_TYPE::SecurityTypeFOR);
    if (ret != 0) {
      std::cout << GetLastErrorMessage() << std::endl;
      return;
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));
  }
}

int main() { 

// local dev
// fillout the apikey info below
    szServer = (const char*)("x.x.x.x/API/V1");         // api end point, do not include "ws://"
    apiKey = (const char*)("xxxxxxxxxxx");                   // apikey
    password = (const char*)("xxxxxxxxx");                // apikeypwd
    account = (const char*)("ABCDEFG");                   // trading account, use all caps


    int i = 0;
//    std::cin >> i;
//    if (i == 1) {
        // callback_sample();
//    } else {
//        polling_sample_2();
//    }

    demo();

    std::cout << "Bye" << std::endl;
    return 0;
}