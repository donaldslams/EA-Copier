//+------------------------------------------------------------------+
//|                                            FortexTradeCopier.mq5 |
//+------------------------------------------------------------------+

#property version   "1.00"

#include "Fortex.mq5"
input string   APIEndPoint="demo4.fortex.com/WEBTRADER";
input string   APIKey;
input string   APIPwd;
input string   APIAccount;
input bool     ConvertFXSymbol = true;

char fortexaccount[];

bool api_connected = false;   

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
   
//---
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
//---
   
  }
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
//---
   
  }
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
         if(ConvertFXSymbol && StringLen(trans.symbol) == 6){
            StringToCharArray(trans.symbol.Substr(0, 3) + "/" + trans.symbol.Substr(3, 3), symbol);
         }
         else{
            StringToCharArray(trans.symbol, symbol);
         }
         char refticketnum[];
         string login = StringFormat("%I64u", AccountInfoInteger(ACCOUNT_LOGIN));
         string ticketnum = StringFormat("%I64u", trans.position);
         StringToCharArray("#5-" + login + "-" + ticketnum, refticketnum); // add "#" before the ticket number, fortex will use this as lookup keys to match with fortex assigned ticket number
         double amount = trans.volume * SymbolInfoDouble(trans.symbol,SYMBOL_TRADE_CONTRACT_SIZE); // need to * the contract size for the trans.symbol         
         EnterDoneawayTrade(fortexaccount, symbol, direction, refticketnum, side, amount, trans.price);
      }
  }
//+------------------------------------------------------------------+
