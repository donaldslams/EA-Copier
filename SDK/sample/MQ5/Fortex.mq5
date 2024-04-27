//+------------------------------------------------------------------+
//|                                                    FortexApi.mq4 |
//|                                              Copyright 2024, DS. |
//|                                       Version 1.0.0.1 2024/04/26 |
//+------------------------------------------------------------------+
#property copyright "Copyright 2024, DS"
#property link      ""
#property version   "1.00"
#property library
#property strict

#import "FortexWS64.dll"

void Init(char& [], int msgRecvType);
int GetState();
int Connect();
int Login(char& [], char& []);
void Logout();
int QueryAccountInfo(char& []);
int QuerySymbolList();
int SubscribeMarketDataEx(int, char& [], int, int);
int PlaceMarketOrderEx(char& [], int, double, char& [], int);
int PlaceLimitOrderEx(char& [], int, double, char& [], int, double);
int PlaceStopOrderEx(char& [], int, double, char& [], int, double);
int PlaceThresholdOrderEx(char& [], int, double, char& [], int, double, double);
int CancelOrderEx(char& [], char& [], char& [], int, int, double, char& [], int, char& [], char& [], int);
int ModifyOrderEx(char& []);
int EnterDoneawayTrade(char& [], char& [], int, char& [], int, double, double);
int GetMarketDataEx();
int GetMessageFromServerEx();

int ToInt(int idx);
void ToStringEx(int idx, char& [], int len);
double ToDouble(int idx);
long ToLong(int idx);
float ToFloat(int idx);

void PutInt(int, int, int);
void PutString(int, int, char& []);
void PutDouble(int, int, double);
void PutLongLong(int, int, long);
void PutFloat(int, int, float);

#import

char buffer[1024];

string GetStringMT(int idx) {
   ArrayInitialize(buffer, '\0');
   ToStringEx(idx, buffer, 1024);
   return CharArrayToString(buffer);
}

void PutStringMT(int objType, int index, string &val) {
   ArrayInitialize(buffer, '\0');
   StringToCharArray(val, buffer);
   PutString(objType, index, buffer);
}

enum MARKET_DATA_TYPE {
    DT_NA           = 0,
    L1              = 1,
    L2              = 2,
};

enum MARKET_DATA_SIDE {
    BID             = 0,
    ASK             = 1,
};

enum EXECUTION_MODE {
    MODE_NA         = 0,
    TICKET          = 1,
    POSITION        = 2,
};

enum CHART_TYPE{
    DAILY           = 0,
    MINUTE          = 1,
    CHART_TYPE_NA   = 2,
};

enum NET_POSITION_TYPE {
    POSITION_NA     = 0,
    OPEN_BUY        = 1,
    OPEN_SELL       = 2,
    CLOSE_BUY       = 3,
    CLOSE_SELL      = 4,
};

enum ORDER_SIDE {
    ORDER_NA        = 0,
    BUY             = 1,
    SELL            = 2,
};

enum TICKET_TYPE {
    TICKET_NA       = 0,
    TICKET_OPEN     = 1,
    TICKET_CLOSE    = 2,
};

enum TICKET_DIRECTION {
    ToOpen           = 1,
    ToClose          = 2,
    Aggregate        = 3,
};

enum CONNECT_STATUS {
     UnConnect      = 0,
     Disconnecting  = 1,
     Disconnected   = 2,
     MultipleReconnectFail = 3,
     Connecting     = 4,
     Connected      = 5,
     LoginInProgress = 6,
     LoginDone      = 7,
     LoginOut       = 8,
};

enum SECURITY_TYPE {
	SecurityTypeNa = 0,
	SecurityTypeStock = 1,
	SecurityTypeOptions = 2,
	SecurityTypeFOR = 3,
	SecurityTypeFutures = 4,
	SecurityTypeMetals = 5,
	SecurityTypeNDF = 6,
	SecurityTypeCFD = 7,
	SecurityTypePhysialForex = 8,
	SecurityTypeFXSwap = 9,
	SecurityTypeFXForward = 10,
	SecurityTypeBond = 11,
	SecurityTypeCrypto = 12,
};

enum FortexObjectType {
    OBJ_TYPE_NA = 0,
    OBJ_TYPE_ACCOUNT_INFO = 1,
    OBJ_TYPE_SYMBOL = 2,
    OBJ_TYPE_ORDER = 3,
    OBJ_TYPE_MARKET_DATA = 4,
    OBJ_TYPE_NET_POSITION = 5,
    OBJ_TYPE_TRADE_HISTORY_REQUEST = 6,
    OBJ_TYPE_FORTEX_ERROR_CODE = 7,
    OBJ_TYPE_FORTEX_ORDER_REJECT_INFO = 8,
    OBJ_TYPE_TICKET_SUMMARY = 9,
    OBJ_TYPE_TICKET = 10,
    OBJ_TYPE_NET_TICKET = 11,
    OBJ_TYPE_EXEC_REPORT_ORDER = 12,
    OBJ_TYPE_EXECUTION_REPORT = 13,
    OBJ_TYPE_EXECUTION_ALL_DATA = 14,
    OBJ_TYPE_CHART_REQUEST = 15,
    OBJ_TYPE_CHART_DATA = 16,
    OBJ_TYPE_EVENT = 17,
};

struct FortexMargin {
   double liguidation;
   double maintain;
   double required;
};

struct FortexAcctSetting {
   string login;
   int credit;
   int loginCate;
   int enable;
   int viewOnly;
   string settleCcy;
   int financeType;
   string enabledOrderTifs[];
   string enabledTifs[];
   int lastOrderId;
   double minQtyOnOrderEntry;
   FortexMargin margin;
};

struct FortexAcctValue {
   string account;
   double dayCumQty;
   double depositWithDraw;
   int numTickets;
   double bodBalance;
   double balance;
   double clsPl;
   double commission;
   FortexMargin margin;
};

struct FortexAcctValueCalculated {
   double equity;
   double openPl;
   double totalPl;
   double freeMargin;
   double buyingPower;
   double leverageRatio;
   double positionValue;
   double pctEquity;
};

struct FortexLinkedAccount {
   string account;
   int loginCate;
};

struct FortexAccountInfo {
   string login;
   FortexAcctSetting setting;
   FortexAcctValue value;
   FortexAcctValueCalculated value2;
   FortexLinkedAccount linkedAccounts[];
};

FortexAccountInfo GetAccountInfo() {
   FortexAccountInfo info;
   info.login = GetStringMT(0);
   info.setting.login = GetStringMT(1);
   info.setting.credit = ToInt(2);
   info.setting.loginCate = ToInt(3);
   info.setting.enable = ToInt(4);
   info.setting.viewOnly = ToInt(5);
   info.setting.settleCcy = GetStringMT(6);
   info.setting.financeType = ToInt(7);
   int enabledOrderTifsLen = ToInt(8);
   int idx = 9;
   ArrayResize(info.setting.enabledOrderTifs, enabledOrderTifsLen);
   for (int i=0; i<enabledOrderTifsLen; i++) {
      info.setting.enabledOrderTifs[i] = GetStringMT(idx++);
   }
   int enabledTifsLen = ToInt(idx++);
   ArrayResize(info.setting.enabledTifs, enabledTifsLen);
   for (int i=0; i<enabledTifsLen; i++) {
      info.setting.enabledTifs[i] = GetStringMT(idx++);
   }
   info.setting.lastOrderId = ToInt(idx++);
   info.setting.minQtyOnOrderEntry = ToDouble(idx++);
   info.setting.margin.liguidation = ToDouble(idx++);
   info.setting.margin.maintain = ToDouble(idx++);
   info.setting.margin.required = ToDouble(idx++);
   info.value.account = GetStringMT(idx++);
   info.value.dayCumQty = ToDouble(idx++);
   info.value.depositWithDraw = ToDouble(idx++);
   info.value.numTickets = ToInt(idx++);
   info.value.bodBalance = ToDouble(idx++);
   info.value.balance = ToDouble(idx++);
   info.value.clsPl = ToDouble(idx++);
   info.value.commission = ToDouble(idx++);
   info.value.margin.liguidation = ToDouble(idx++);
   info.value.margin.maintain = ToDouble(idx++);
   info.value.margin.required = ToDouble(idx++);
   info.value2.equity = ToDouble(idx++);
   info.value2.openPl = ToDouble(idx++);
   info.value2.totalPl = ToDouble(idx++);
   info.value2.freeMargin = ToDouble(idx++);
   info.value2.buyingPower = ToDouble(idx++);
   info.value2.leverageRatio = ToDouble(idx++);
   info.value2.positionValue = ToDouble(idx++);
   info.value2.pctEquity = ToDouble(idx++);
   int linkedAccountsLen = ToInt(idx++);
   ArrayResize(info.linkedAccounts, linkedAccountsLen);
   for (int i=0; i<linkedAccountsLen; i++) {
      FortexLinkedAccount account;
      account.account = GetStringMT(idx++);
      account.loginCate = ToInt(idx++);
      info.linkedAccounts[i] = account;
   }
   return info;
}

struct FortexErrorCode {
   int code;
   string message;
   string hint;
   string desc;
};

FortexErrorCode GetFortexErrorCode () {
   FortexErrorCode code;
   code.code = ToInt(0);
   code.message = GetStringMT(1);
   code.hint = GetStringMT(2);
   code.desc = GetStringMT(3);
   return code;
};

struct FortexOrderRejectInfo {
   string cxlRejRespTo;
   string ordStatus;
   string clOrdId;
   string txTime;
   string acct;
   string cxlRejRsn;
};

FortexOrderRejectInfo GetFortexOrderRejectInfo(){
   FortexOrderRejectInfo info;
   info.cxlRejRespTo = GetStringMT(0);
   info.ordStatus = GetStringMT(1);
   info.clOrdId = GetStringMT(2);
   info.txTime = GetStringMT(3);
   info.acct = GetStringMT(4);
   info.cxlRejRsn = GetStringMT(5);
   return info;
}

struct FortexMDEntry {
   double price;
   double size;
   string mmid;
};

struct FortexMarketData {
   int type;
   string symName;
   int secType;
   int domain;
   long sendTime;
   long recvTime;
   double high;
   double low;
   double open;
   double close;
   FortexMDEntry bids[];
   FortexMDEntry asks[];
};

FortexMarketData MDGetMarketData(){
   FortexMarketData md;
   md.type = ToInt(0);
   md.symName = GetStringMT(1);
   md.secType = ToInt(2);
   md.domain = ToInt(3);
   // md.sendTime = ToLong(4);
   // md.recvTime = ToLong(5);
   md.high = ToDouble(6);
   md.low = ToDouble(7);
   md.open = ToDouble(8);
   md.close = ToDouble(9);
   int bidLen = ToInt(10);
   int idx = 11;
   ArrayResize(md.bids, bidLen);
   for (int i=0; i<bidLen; i++) {
      FortexMDEntry entry;
      entry.price = ToDouble(idx++);
      entry.size = ToDouble(idx++);
      entry.mmid = GetStringMT(idx++);
      md.bids[i] = entry;
   }
   int askLen = ToInt(idx++);
   ArrayResize(md.asks, askLen);
   for (int i=0; i<askLen; i++) {
      FortexMDEntry entry;
      entry.price = ToDouble(idx++);
      entry.size = ToDouble(idx++);
      entry.mmid = GetStringMT(idx++);
      md.asks[i] = entry;
   }
   return md;
}

struct FortexSymbol {
   int index;
   int secType;
   string tenor;
   string name;
   string baseCcy;
   string termCcy;
   string marginCcy;
   int bidChangeType;
   int askChangeType;
   double lastBid;
   double lastAsk;
   double contractSize;
   double lotSize;
   double minLotSize;
   double minQty;
   double maxQty;
   int qtyDec;
   int pxDex;
   int pipDec;
   long updateTime;
};

FortexSymbol GetSymbol() {
   FortexSymbol sym;
   sym.index = ToInt(0);
   sym.secType = ToInt(1);
   sym.tenor = GetStringMT(2);
   sym.name = GetStringMT(3);
   sym.baseCcy = GetStringMT(4);
   sym.termCcy = GetStringMT(5);
   sym.marginCcy = GetStringMT(6);
   sym.bidChangeType = ToInt(7);
   sym.askChangeType = ToInt(8);
   sym.lastBid = ToDouble(9);
   sym.lastAsk = ToDouble(10);
   sym.contractSize = ToDouble(11);
   sym.lotSize = ToDouble(12);
   sym.minLotSize = ToDouble(13);
   sym.minQty = ToDouble(14);
   sym.maxQty = ToDouble(15);
   sym.qtyDec = ToInt(16);
   sym.pxDex = ToInt(17);
   sym.pipDec = ToInt(18);
   // sym.updateTime = ToLong(19);
   return sym;
}

struct FortexOrder {
   string account;
   string clientOrderId;
   string symName;
   int secType;
   string user;
   string origUser;
   string fortexClientOrderId;
   string fortexTicketExecSeqNo;
   string origClientOrderId;
   string ticketId;
   string refTicketId;
   string tag;
   string origTag;
   string cliendtOrderIdFillNo;
   string execDest;
   string execBroker;
   string dealCcy;
   string settDate;
   string fillSeqNo;
   string instType;
   long transactTime;
   int isPrincipalAgency;
   int side;
   int type;
   int msgType;
   int status;
   int tif;
   int ticketType; 
   int execType;
   int execInst;
   int handInst;
   double price;
   double price2;
   double refPrice;
   double avgPrice;
   double slipPage;
   double quantity;
   double minQty;
   double reservedQty;
   double maxShow;
   double leaveQty;
   double cumQty;
   double filledQty;
   double stopLoss;
   double takeProfit;
   double stopPx;
   double lastPx;
   double lastQty;
};

FortexOrder GetOrder() {
   FortexOrder order;
   order.account = GetStringMT(0);
   order.clientOrderId = GetStringMT(1);
   order.symName = GetStringMT(2);
   order.secType = ToInt(3);
   order.user = GetStringMT(4);
   order.origUser = GetStringMT(5);
   order.fortexClientOrderId = GetStringMT(6);
   order.fortexTicketExecSeqNo = GetStringMT(7);
   order.origClientOrderId = GetStringMT(8);
   order.ticketId = GetStringMT(9);
   order.refTicketId = GetStringMT(10);
   order.tag = GetStringMT(11);
   order.origTag = GetStringMT(12);
   order.cliendtOrderIdFillNo = GetStringMT(13);
   order.execDest = GetStringMT(14);
   order.execBroker = GetStringMT(15);
   order.dealCcy = GetStringMT(16);
   order.settDate = GetStringMT(17);
   order.fillSeqNo = GetStringMT(18);
   order.instType = GetStringMT(19);
   // order.transactTime = ToLong(20);
   order.isPrincipalAgency = ToInt(21);
   order.side = ToInt(22);
   order.type = ToInt(23);
   order.msgType = ToInt(24);
   order.status = ToInt(25);
   order.tif = ToInt(26);
   order.ticketType = ToInt(27);
   order.execType = ToInt(28);
   order.execInst = ToInt(29);
   order.handInst = ToInt(30);
   order.price = ToDouble(31);
   order.price2 = ToDouble(32);
   order.refPrice = ToDouble(33);
   order.avgPrice = ToDouble(34);
   order.slipPage = ToDouble(35);
   order.quantity = ToDouble(36);
   order.minQty = ToDouble(37);
   order.reservedQty = ToDouble(38);
   order.maxShow = ToDouble(39);
   order.leaveQty = ToDouble(40);
   order.cumQty = ToDouble(41);
   order.filledQty = ToDouble(42);
   order.stopLoss = ToDouble(43);
   order.takeProfit = ToDouble(44);
   order.stopPx = ToDouble(45);
   order.lastPx = ToDouble(46);
   order.lastQty = ToDouble(47);
   return order;
}

void PutOrder(FortexOrder &order) {
   PutStringMT(OBJ_TYPE_ORDER, 0, order.account);
   PutStringMT(OBJ_TYPE_ORDER, 1, order.clientOrderId);
   PutStringMT(OBJ_TYPE_ORDER, 2, order.symName);
   PutInt(OBJ_TYPE_ORDER, 3, order.secType);
   PutStringMT(OBJ_TYPE_ORDER, 4, order.user);
   PutStringMT(OBJ_TYPE_ORDER, 5, order.origUser);
   PutStringMT(OBJ_TYPE_ORDER, 6, order.fortexClientOrderId);
   PutStringMT(OBJ_TYPE_ORDER, 7, order.fortexTicketExecSeqNo);
   PutStringMT(OBJ_TYPE_ORDER, 8, order.origClientOrderId);
   PutStringMT(OBJ_TYPE_ORDER, 9, order.ticketId);
   PutStringMT(OBJ_TYPE_ORDER, 10, order.refTicketId);
   PutStringMT(OBJ_TYPE_ORDER, 11, order.tag);
   PutStringMT(OBJ_TYPE_ORDER, 12, order.origTag);
   PutStringMT(OBJ_TYPE_ORDER, 13, order.cliendtOrderIdFillNo);
   PutStringMT(OBJ_TYPE_ORDER, 14, order.execDest);
   PutStringMT(OBJ_TYPE_ORDER, 15, order.execBroker);
   PutStringMT(OBJ_TYPE_ORDER, 16, order.dealCcy);
   PutStringMT(OBJ_TYPE_ORDER, 17, order.settDate);
   PutStringMT(OBJ_TYPE_ORDER, 18, order.fillSeqNo);
   PutStringMT(OBJ_TYPE_ORDER, 19, order.instType);
   // PutLongLong(OBJ_TYPE_ORDER, 20, order.transactTime);
   PutInt(OBJ_TYPE_ORDER, 21, order.isPrincipalAgency);
   PutInt(OBJ_TYPE_ORDER, 22, order.side);
   PutInt(OBJ_TYPE_ORDER, 23, order.type);
   PutInt(OBJ_TYPE_ORDER, 24, order.msgType);
   PutInt(OBJ_TYPE_ORDER, 25, order.status);
   PutInt(OBJ_TYPE_ORDER, 26, order.tif);
   PutInt(OBJ_TYPE_ORDER, 27, order.ticketType);
   PutInt(OBJ_TYPE_ORDER, 28, order.execType);
   PutInt(OBJ_TYPE_ORDER, 29, order.execInst);
   PutInt(OBJ_TYPE_ORDER, 30, order.handInst);
   PutDouble(OBJ_TYPE_ORDER, 31, order.price);
   PutDouble(OBJ_TYPE_ORDER, 32, order.price2);
   PutDouble(OBJ_TYPE_ORDER, 33, order.refPrice);
   PutDouble(OBJ_TYPE_ORDER, 34, order.avgPrice);
   PutDouble(OBJ_TYPE_ORDER, 35, order.slipPage);
   PutDouble(OBJ_TYPE_ORDER, 36, order.quantity);
   PutDouble(OBJ_TYPE_ORDER, 37, order.minQty);
   PutDouble(OBJ_TYPE_ORDER, 38, order.reservedQty);
   PutDouble(OBJ_TYPE_ORDER, 39, order.maxShow);
   PutDouble(OBJ_TYPE_ORDER, 40, order.leaveQty);
   PutDouble(OBJ_TYPE_ORDER, 41, order.cumQty);
   PutDouble(OBJ_TYPE_ORDER, 42, order.filledQty);
   PutDouble(OBJ_TYPE_ORDER, 43, order.stopLoss);
   PutDouble(OBJ_TYPE_ORDER, 44, order.takeProfit);
   PutDouble(OBJ_TYPE_ORDER, 45, order.stopPx);
   PutDouble(OBJ_TYPE_ORDER, 46, order.lastPx);
   PutDouble(OBJ_TYPE_ORDER, 47, order.lastQty);
}

struct FortexNetPosition {
   string account;
   string symName;
   int secType;
   int type;
   double openAvgPx;
   double openRefPx;
   double closeAvgPx;
   double closeRefPx;
   double quantity;
   double closedPl;
   double commission;
   string dealCcy;
};

FortexNetPosition GetNetPosition() {
   FortexNetPosition pos;
   pos.account = GetStringMT(0);
   pos.symName = GetStringMT(1);
   pos.secType = ToInt(2);
   pos.type = ToInt(3);
   pos.openAvgPx = ToDouble(4);
   pos.openRefPx = ToDouble(5);
   pos.closeAvgPx = ToDouble(6);
   pos.closeRefPx = ToDouble(7);
   pos.quantity = ToDouble(8);
   pos.closedPl = ToDouble(9);
   pos.commission = ToDouble(10);
   pos.dealCcy = GetStringMT(11);
   return pos;
}

struct FortexNetTicket {
   string symName;
   int secType;
   int side;
   string account;
   string dealCcy;
   int status;
   double openPx;
   double openQty;
   double openConvRate;;
   double closePx;
   double closePl;
   double closeQty;
   double closeConvRate;
   double commission;
};

FortexNetTicket GetNetTicket() {
   FortexNetTicket ticket;
   ticket.symName = GetStringMT(0);
   ticket.secType = ToInt(1);
   ticket.side = ToInt(2);
   ticket.account = GetStringMT(3);
   ticket.dealCcy = GetStringMT(4);
   ticket.status = ToInt(5);
   ticket.openPx = ToDouble(6);
   ticket.openQty = ToDouble(7);
   ticket.openConvRate = ToDouble(8);
   ticket.closePx = ToDouble(9);
   ticket.closePl = ToDouble(10);
   ticket.closeQty = ToDouble(11);
   ticket.closeConvRate = ToDouble(12);
   ticket.commission = ToDouble(13);
   return ticket;
};

struct FortexExecReportOrder{
   int side;
   string symName;
   double price;
   double ordQty;
   double price2;
   int tif;
   double sl;
   double tp;
   int ordType;
};

struct FortexExecutionReport {
   string symName;
   int secType;
   string account;
   string ordUser;
   string origOrdUser;
   string OrdId;
   string clientOrdId;
   string origClientOrdId;
   string ticketId;
   string refTktId;
   string ordTag;
   string fillSeqNo;
   string mmid;
   string settDate;
   string dealCcy;
   string execDst;
   int actionType;
   int execType;
   string execCode;
   int ordStatus;
   int side;
   int ordType;
   long transactionTime;
   int tif;
   double price;
   double price2;
   double stopPx;
   double avgPx;
   double ordQty;
   double leaveQty;
   double cumQty;
   double lastPx;
   double lastQty;
   double commission;
   double sl;
   double tp;
   double originSl;
   double originTp;
   double originPrice;
   double originPrice2;
   double originOrdQty;
   int originOrdType;
   int originOrdSide;
   int originTif;
   string originExecDst;
   FortexExecReportOrder replaceOrderRef;
   FortexErrorCode errorCode;
   string comment;
   string refEvent;
   string refMsgType;
};

FortexExecutionReport GetExecutionReport() {
   FortexExecutionReport report;
   report.symName = GetStringMT(0);
   report.secType = ToInt(1);
   report.account = GetStringMT(2);
   report.ordUser = GetStringMT(3);
   report.origOrdUser = GetStringMT(4);
   report.OrdId = GetStringMT(5);
   report.clientOrdId = GetStringMT(6);
   report.origClientOrdId = GetStringMT(7);
   report.ticketId = GetStringMT(8);
   report.refTktId = GetStringMT(9);
   report.ordTag = GetStringMT(10);
   report.fillSeqNo = GetStringMT(11);
   report.mmid = GetStringMT(12);
   report.settDate = GetStringMT(13);
   report.dealCcy = GetStringMT(14);
   report.execDst = GetStringMT(15);
   report.actionType = ToInt(16);
   report.execType = ToInt(17);
   report.execCode = GetStringMT(18);
   report.ordStatus = ToInt(19);
   report.side = ToInt(20);
   report.ordType = ToInt(21);
   // report.transactionTime = ToLong(22);
   report.tif = ToInt(23);
   report.price = ToDouble(24);
   report.price2 = ToDouble(25);
   report.stopPx = ToDouble(26);
   report.avgPx = ToDouble(27);
   report.ordQty = ToDouble(28);
   report.leaveQty = ToDouble(29);
   report.cumQty = ToDouble(30);
   report.lastPx = ToDouble(31);
   report.lastQty = ToDouble(32);
   report.commission = ToDouble(33);
   report.sl = ToDouble(34);
   report.tp = ToDouble(35);
   report.originSl = ToDouble(36);
   report.originTp = ToDouble(37);
   report.originPrice = ToDouble(38);
   report.originPrice2 = ToDouble(39);
   report.originOrdQty = ToDouble(40);
   report.originOrdType = ToInt(41);
   report.originOrdSide = ToInt(42);
   report.originTif = ToInt(43);
   report.originExecDst = GetStringMT(44);
   report.replaceOrderRef.side = ToInt(45);
   report.replaceOrderRef.symName = GetStringMT(46);
   report.replaceOrderRef.price = ToDouble(47);
   report.replaceOrderRef.ordQty = ToDouble(48);
   report.replaceOrderRef.price2 = ToDouble(49);
   report.replaceOrderRef.tif = ToInt(50);
   report.replaceOrderRef.sl = ToDouble(51);
   report.replaceOrderRef.tp = ToDouble(52);
   report.replaceOrderRef.ordType = ToInt(53);
   report.errorCode.code = ToInt(54);
   report.errorCode.message = GetStringMT(55);
   report.errorCode.hint = GetStringMT(56);
   report.errorCode.desc = GetStringMT(57);
   report.comment = GetStringMT(58);
   report.refEvent = GetStringMT(59);
   report.refMsgType = GetStringMT(60);
   return report;
}

int ModifyOrderMT(char& account[], FortexOrder &ord){
   PutOrder(ord);
   return ModifyOrderEx(account);
}