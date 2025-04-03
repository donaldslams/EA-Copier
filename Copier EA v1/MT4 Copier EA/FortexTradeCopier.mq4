//+------------------------------------------------------------------+
//|                                            FortexTradeCopier.mq4 |
//+------------------------------------------------------------------+

#property version   "1.00"

#include "Fortex.mq4"
input string   APIEndPoint="demo.fortex.com/WEBTRADER";
input string   APIKey="";
input string   APIPwd="";
input string   APIAccount="";
input bool     ConvertFXSymbol = true;

char fortexaccount[];

bool api_connected = false;   

enum OrderItem{
ticket=0,
cmd,
};

struct TradeData{
 int count;
 int orders[][2];
};

TradeData prevData; 

void fillData(TradeData& data){
   data.count = OrdersTotal();
   ArrayResize(data.orders,data.count);   
   for (int i =0;i<data.count;i++){
      data.orders[i][OrderItem::ticket]=0;
      if(!OrderSelect(i,SELECT_BY_POS,MODE_TRADES))continue;
      data.orders[i][OrderItem::ticket] = OrderTicket();
      data.orders[i][OrderItem::cmd] = OrderType();
   }
   
   if (data.count>0) ArraySort(data.orders);
}

//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+


int OnInit()
  {
//---
   StringToCharArray(APIAccount, fortexaccount);
   char address[];
   StringToCharArray(APIEndPoint, address);
   char apikey[];
   StringToCharArray(APIKey, apikey);
   char apikeypwd[];
   StringToCharArray(APIPwd,apikeypwd);
   Init(address, 2);
   if(Connect() !=0 || Login(apikey, apikeypwd) != 0){
      Print("login failed, reason:", GetLastError());
   }   
   else{
      api_connected = true;   
   }
   
   fillData(prevData);
   EventSetTimer(1);
   
//---
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
//---
   EventKillTimer();
  }
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
//---

  }
  
void OnTimer()
   {
      TradeData newData;
      fillData(newData);
      
      int iPrev=0,iNew=0;
      
      while (iPrev<prevData.count || iNew < newData.count){
         if (iPrev<prevData.count && prevData.orders[iPrev][OrderItem::ticket]==0)
         {
            iPrev ++; continue;
         }
         
         if (iNew<newData.count && newData.orders[iNew][OrderItem::ticket]==0)
         {
            iNew ++; continue;
         }
         
         if(iPrev<prevData.count && iNew<newData.count)
         {  
            int cur_ticket_Prev = prevData.orders[iPrev][OrderItem::ticket];
            int cur_ticket_New = newData.orders[iNew][OrderItem::ticket];
            
            if(cur_ticket_Prev<cur_ticket_New){
               if(OrderSelect(cur_ticket_Prev,SELECT_BY_TICKET)){
                  if(prevData.orders[iPrev][OrderItem::cmd]>OP_SELL){
                     if(OrderType()==OP_BUY||OrderType()==OP_SELL){
                        //send open
                        sendDoneaway(OrderSymbol(),TICKET_DIRECTION::ToOpen,OrderTicket(),OrderType(),OrderLots(),OrderOpenPrice()); 
                        
                        //send close
                        sendDoneaway(OrderSymbol(),TICKET_DIRECTION::ToClose,OrderTicket(),OrderType()==OP_BUY?OP_SELL:OP_BUY,OrderLots(),OrderClosePrice());
                     }
                  }
                  else{
                     if(OrderType()==OP_BUY||OrderType()==OP_SELL){                        
                        //send close
                        if(StringSubstr(OrderComment(),0,4)!="to #") {
                           sendDoneaway(OrderSymbol(),TICKET_DIRECTION::ToClose,OrderTicket(),OrderType()==OP_BUY?OP_SELL:OP_BUY,OrderLots(),OrderClosePrice());
                        }else{
                        // one of the partial fills to close a ticket, no way to know the original ticket number
                           string ticketTo = StringSubstr(OrderComment(),4);
                           int new_ticket=StringToInteger(ticketTo);
                           if(new_ticket>0){ 
                              double close_lot = OrderLots();
                              double close_price = OrderClosePrice();
                              OrderSelect(new_ticket,SELECT_BY_TICKET);
                              sendDoneaway(OrderSymbol(),TICKET_DIRECTION::ToClose,cur_ticket_Prev,OrderType()==OP_BUY?OP_SELL:OP_BUY,close_lot + OrderLots(),close_price);
                           }
                           else{
                              sendDoneaway(OrderSymbol(),TICKET_DIRECTION::ToClose,OrderTicket(),OrderType()==OP_BUY?OP_SELL:OP_BUY,OrderLots(),OrderClosePrice());
                           }                           
                        }
                     }
                  }
               }
               iPrev++;            
            }
            else if (cur_ticket_Prev>cur_ticket_New)
            {
               if(OrderSelect(cur_ticket_New,SELECT_BY_TICKET)){
                  if(newData.orders[iNew][OrderItem::cmd]==OP_BUY||newData.orders[iNew][OrderItem::cmd]==OP_SELL){
                     //send open
                     if(StringSubstr(OrderComment(),0,6)!="from #"){
                        sendDoneaway(OrderSymbol(),TICKET_DIRECTION::ToOpen,OrderTicket(),OrderType(),OrderLots(),OrderOpenPrice());
                     }
                  }
               }
               iNew++;
            }else{
               if(OrderSelect(cur_ticket_New,SELECT_BY_TICKET)){
                  if(prevData.orders[iPrev][OrderItem::cmd]>OP_SELL
                     && (newData.orders[iNew][OrderItem::cmd]==OP_BUY||newData.orders[iNew][OrderItem::cmd]==OP_SELL)){
                     //activated, send open
                     sendDoneaway(OrderSymbol(),TICKET_DIRECTION::ToOpen,OrderTicket(),OrderType(),OrderLots(),OrderOpenPrice());
                  }
               }
               iPrev++;
               iNew++;
            }
         }else if(iPrev<prevData.count)
         {
            int cur_ticket_Prev = prevData.orders[iPrev][OrderItem::ticket];
            if(OrderSelect(cur_ticket_Prev,SELECT_BY_TICKET)){
               if(prevData.orders[iPrev][OrderItem::cmd]>OP_SELL){
                  if(OrderType()==OP_BUY||OrderType()==OP_SELL){
                     //send open
                     sendDoneaway(OrderSymbol(),TICKET_DIRECTION::ToOpen,OrderTicket(),OrderType(),OrderLots(),OrderOpenPrice());   
                     
                     //send close
                     sendDoneaway(OrderSymbol(),TICKET_DIRECTION::ToClose,OrderTicket(),OrderType()==OP_BUY?OP_SELL:OP_BUY,OrderLots(),OrderClosePrice());
                  }
               }else{
                  if(OrderType()==OP_BUY||OrderType()==OP_SELL){                        
                     //send close
                     if(StringSubstr(OrderComment(),0,6)!="from #") {
                        sendDoneaway(OrderSymbol(),TICKET_DIRECTION::ToClose,OrderTicket(),OrderType()==OP_BUY?OP_SELL:OP_BUY,OrderLots(),OrderClosePrice());
                     }else{
                     /* the last fill of multiple fills to close a ticket
                        string ticketFrom = StringSubstr(OrderComment(),6);
                        int new_ticket=StringToInteger(ticketFrom);
                        if(new_ticket>0){ 
                           double close_lot = OrderLots();
                           double close_price = OrderClosePrice();
                           OrderSelect(new_ticket,SELECT_BY_TICKET);
                           sendDoneaway(OrderSymbol(),TICKET_DIRECTION::ToClose,cur_ticket_Prev,OrderType()==OP_BUY?OP_SELL:OP_BUY,close_lot + OrderLots(),close_price);
                        }
                        else{
                           sendDoneaway(OrderSymbol(),TICKET_DIRECTION::ToClose,OrderTicket(),OrderType()==OP_BUY?OP_SELL:OP_BUY,OrderLots(),OrderClosePrice());
                        }                           
                     */
                     }
                  }
               }
            }
            iPrev++;
         }else if(iNew<newData.count){
            int cur_ticket_New = newData.orders[iNew][OrderItem::ticket];
            if(OrderSelect(cur_ticket_New,SELECT_BY_TICKET)){
               if(newData.orders[iNew][OrderItem::cmd]==OP_BUY||newData.orders[iNew][OrderItem::cmd]==OP_SELL){
                  //send open
                  if(StringSubstr(OrderComment(),0,6)!="from #") {
                     sendDoneaway(OrderSymbol(),TICKET_DIRECTION::ToOpen,OrderTicket(),OrderType(),OrderLots(),OrderOpenPrice());
                  }
               }
            }
            iNew++;
         }      
      }
      
      prevData=newData;
  }
  
 /* 
//+------------------------------------------------------------------+
//| TradeTransaction function                                        |
//+------------------------------------------------------------------+
void OnTradeTransaction(const MqlTradeTransaction& trans,
                        const MqlTradeRequest& request,
                        const MqlTradeResult& result)
  {
//---Only monitors the executions
      if(trans.type == TRADE_TRANSACTION_DEAL_ADD && (trans.deal_type == DEAL_TYPE_BUY || trans.deal_type == DEAL_TYPE_SELL)){
         TICKET_DIRECTION direction = (trans.position==trans.order)?ToOpen:ToClose;
         ORDER_SIDE side = (trans.deal_type==DEAL_TYPE_BUY)?BUY:SELL;
         char symbol[];
         StringToCharArray(trans.symbol.Substr(0, 3) + "/" + trans.symbol.Substr(3, 3),  symbol);
         char refticketnum[];
         string login = StringFormat("%I64u", AccountInfoInteger(ACCOUNT_LOGIN));
         string ticketnum = StringFormat("%I64u", trans.position);
         StringToCharArray("#" + login + "-" + ticketnum, refticketnum); // add "#" before the ticket number, fortex will use this as lookup keys to match with fortex assigned ticket number
         double amount = trans.volume * SymbolInfoDouble(trans.symbol,SYMBOL_TRADE_CONTRACT_SIZE); // need to * the contract size for the trans.symbol         
         EnterDoneawayTrade(fortexaccount, symbol, direction, refticketnum, side, amount, trans.price);
      }
  }
//+------------------------------------------------------------------+
*/

void sendDoneaway(string sym, TICKET_DIRECTION direction, int ticket, int cmd, double lot, double price){
   ORDER_SIDE side = (cmd==OP_BUY)?BUY:SELL;
   char symbol[];
   if(ConvertFXSymbol &&  StringLen(sym) == 6){
      StringToCharArray(StringSubstr(sym,0,3) + "/" + StringSubstr(sym,3,3),  symbol);
   }
   else{
      StringToCharArray(sym,  symbol);
   }
   char refticketnum[];
   string login = StringFormat("%d", AccountInfoInteger(ACCOUNT_LOGIN));
   string ticketnum = StringFormat("%d", ticket);
   StringToCharArray("#4-" + login + "-" + ticketnum, refticketnum); // add "#" before the ticket number, fortex will use this as lookup keys to match with fortex assigned ticket number
   double amount = lot * SymbolInfoDouble(sym,SYMBOL_TRADE_CONTRACT_SIZE); // need to * the contract size for the trans.symbol         
   EnterDoneawayTrade(fortexaccount, symbol, direction, refticketnum, side, amount, price);
}