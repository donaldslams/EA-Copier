//+------------------------------------------------------------------+
//|                                                  FortexAgent.mq4 |
//|                                               Copyright 2024, DS |
//|                                      Version 1.1.4.0, 2024/04/26 |
//+------------------------------------------------------------------+
#include "Fortex.mq4"

// Global Variables
// fillout the following information
string straddress = "x.x.x.x/API/V1";         // api endpoint, no leading ws://
string strapikey = "XXXXXXX";                 // apikey
string strapikeypwd = "XXXX";                 // apikeypwd
string straccount = "XXXX";                   // account name    

string strsymbol = "EUR/USD";

#property strict
//+------------------------------------------------------------------+
//| Script program start function                                    |
//+------------------------------------------------------------------+

//+------------------------------------------------------------------+
//| expert initialization function                                   |
//+------------------------------------------------------------------+
int init()
  {
//----
   char address[1024];
   StringToCharArray(straddress, address);
   char apikey[16];
   StringToCharArray(strapikey, apikey);
   char apikeypwd[16];
   StringToCharArray(strapikeypwd,apikeypwd);
   Init(address, 2);
   if(Connect()==0){
      Login(apikey, apikeypwd);
   }
//----
   return(0);
  }
//+------------------------------------------------------------------+
//| expert deinitialization function                                 |
//+------------------------------------------------------------------+
int deinit()
  {
//----
   Logout();
//----
   return(0);
  }

void demo2(){
   for (int i = 0; i<100; i++) {
      if (GetState() == LoginDone) {
         break;
      } else {
         Sleep(500);
         continue;
      }
   }
   if (GetState() != LoginDone) {
      Print("Login failed!");
      return;
   }
   
   // initialize username
   for (int i=0; i<10; i++) {
      int objType = GetMessageFromServerEx();
      if (objType == OBJ_TYPE_ACCOUNT_INFO) {
         FortexAccountInfo info = GetAccountInfo();
         straccount = info.login;
         break;
      } else {
         Print("Get message from server, type=", objType);
      }
      Sleep(500);
   }
   if (straccount == "") {
      Print("Cannot get account info from server");
      return;
   }

   char account[16];
   StringToCharArray(straccount, account);
   char symbol[16];
   StringToCharArray(strsymbol, symbol);
   char refticketnum[16];
   StringToCharArray("123", refticketnum);

   SubscribeMarketDataEx(1, symbol, L1, 0);

   int i = 0;
   Print("OnLoop...");
   while(i < 100000){
      int objType = GetMessageFromServerEx();
      Print("index=", i, ", objType=", objType);
      if (objType == OBJ_TYPE_ACCOUNT_INFO) {
         FortexAccountInfo info = GetAccountInfo();
      } else if (objType == OBJ_TYPE_SYMBOL) {
         FortexSymbol sym = GetSymbol();
         Print("symbol: ", sym.name, ":", sym.secType);
      } else if(objType == OBJ_TYPE_FORTEX_ERROR_CODE) {
         FortexErrorCode code = GetFortexErrorCode();
         Print("code=", code.code, ", message=", code.message);
      } else if (objType == OBJ_TYPE_ORDER){
         FortexOrder ord = GetOrder();
         Print("get order object, account:", ord.account);
         if (i%3 == 1) {
            char acct[16];
            StringToCharArray(ord.account, acct);
            ord.quantity = 1200;
            Print("Call ModifyOrderMT...");
            ModifyOrderMT(acct, ord);
         } else if (ord.execType == ExecTypeNew) {
            char acct[16];
            char symName[16];
            char origUser[16];
            char ordId[30];
            char origExecDest[16];
            char origTag[16];
            StringToCharArray(ord.account, acct);
            StringToCharArray(ord.symName, symName);
            StringToCharArray(ord.origUser, origUser);
            StringToCharArray(ord.clientOrderId, ordId);
            StringToCharArray(ord.execDest, origExecDest);
            StringToCharArray(ord.tag, origTag);
            Print("Call CancelOrderEx...");
            CancelOrderEx(acct, origUser, ordId, ord.type, ord.side, ord.quantity, symName, ord.secType, origExecDest, origTag, ord.tif);
         }
      } else if (objType == OBJ_TYPE_MARKET_DATA) {
         FortexMarketData md = MDGetMarketData();
         Print("get market data object, symbol:", md.symName);
      } else if (objType == OBJ_TYPE_NET_POSITION) {
         FortexNetPosition np = GetNetPosition();
         Print("get net position object, account:", np.account, "symbol:", np.symName);
      } else if (objType == OBJ_TYPE_NET_TICKET) {
         FortexNetTicket nt = GetNetTicket();
         Print("get net ticket object, account:", nt.account, "symbol:", nt.symName);
      } else if (objType == OBJ_TYPE_EXECUTION_REPORT) {
         FortexExecutionReport er = GetExecutionReport();
         Print("get execution report object, account:", er.account, "symbol:", er.symName);
      } else if (objType == OBJ_TYPE_FORTEX_ORDER_REJECT_INFO) {
         FortexOrderRejectInfo oi = GetFortexOrderRejectInfo();
         Print("get order reject info object, account:", oi.acct, "reason:", oi.cxlRejRsn);
      }

      GetMarketDataEx();
      FortexMarketData md = MDGetMarketData();
      Print("get market data object, symbol...:", md.symName);
      Sleep(100);

      Print("Call SubscribeMarketDataEx...");
      SubscribeMarketDataEx(1, symbol, L1, 0);

      Print("Call EnterDoneawayTrade...");
      EnterDoneawayTrade(account, symbol, ToOpen, refticketnum, BUY, 1000, 1.2);

      Print("Call PlaceMarketOrderEx...");
      PlaceMarketOrderEx(account, SELL, 1000, symbol, SecurityTypeFOR, TimeInForceGTC, ExecInstPartialFill, 0.0, 0.0, 0.0);
      Print("Call PlaceLimitOrderEx...");
      PlaceLimitOrderEx(account, BUY, 1000, symbol, SecurityTypeFOR, 1.0, TimeInForceGTC, ExecInstPartialFill, 0.0, 0.0, 0.0);
      Print("Call PlaceStopOrderEx...");
      PlaceStopOrderEx(account, BUY, 1000, symbol, SecurityTypeFOR, 1.2, TimeInForceGTC, ExecInstPartialFill, 0.0, 0.0);
      Print("Call PlaceThresholdOrderEx...");
      PlaceThresholdOrderEx(account, BUY, 1000, symbol, SecurityTypeFOR, 1.2, 1.1, TimeInForceGTC, ExecInstPartialFill, 0.0, 0.0);
      Sleep(100);  
      //i = i + 10000;
      i++;
   }
}

void demo(){
   char account[16];
   StringToCharArray(straccount, account);
   char symbol[16];
   StringToCharArray(strsymbol, symbol);
   char refticketnum[16];
   StringToCharArray("123", refticketnum);
   Sleep(5000);
   
   for(int i=0; i<5; i++) {
      Print("Call PlaceMarketOrderEx...");   
      PlaceMarketOrderEx(account, BUY, 10000, symbol, SecurityTypeFOR, TimeInForceGTC, ExecInstPartialFill, 0.0, 0.0, 0.0);
      Sleep(2500);  
      PlaceMarketOrderEx(account, SELL, 10000, symbol, SecurityTypeFOR, TimeInForceGTC, ExecInstPartialFill, 0.0, 0.0, 0.0);
      Sleep(2500);  
      PlaceMarketOrderEx(account, SELL, 10000, symbol, SecurityTypeFOR, TimeInForceGTC, ExecInstPartialFill, 0.0, 0.0, 0.0);
      Sleep(2500);  
      PlaceMarketOrderEx(account, BUY, 10000, symbol, SecurityTypeFOR, TimeInForceGTC, ExecInstPartialFill, 0.0, 0.0, 0.0);
      Sleep(2500);        
   } 
}

void OnStart()
  {
//---
   Print("OnStart..."); 
   demo();
   Print("Exit");
  }
//+------------------------------------------------------------------+
