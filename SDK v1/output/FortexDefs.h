#ifndef FORTEX_DEFS_H
#define FORTEX_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef const char cchar;

typedef enum {
    DT_NA           = 0,
    L1              = 1,
    L2              = 2,
} MARKET_DATA_TYPE;

typedef enum {
    BID             = 0,
    ASK             = 1,
} MARKET_DATA_SIDE;

typedef enum {
    MODE_NA         = 0,
    TICKET          = 1,
    POSITION        = 2,
} EXECUTION_MODE;

typedef enum {
    DAILY           = 0,
    MINUTE          = 1,
    CHART_TYPE_NA   = 2,
} CHART_TYPE;

typedef enum {
    POSITION_NA     = 0,
    OPEN_BUY        = 1,
    OPEN_SELL       = 2,
    CLOSE_BUY       = 3,
    CLOSE_SELL      = 4,
} NET_POSITION_TYPE;

typedef enum {
    ORDER_NA        = 0,
    BUY             = 1,
    SELL            = 2,
} ORDER_SIDE;

typedef enum {
    TICKET_NA       = 0,
    TICKET_OPEN     = 1,
    TICKET_CLOSE    = 2,
} TICKET_TYPE;

typedef enum {
    ToOpen           = 1,
    ToClose          = 2,
    Aggregate        = 3,
} TICKET_DIRECTION;

typedef enum {
     NotConnect      = 0,
     Disconnecting  = 1,
     Disconnected   = 2,
     MultipleReconnectFail = 3,
     Connecting     = 4,
     Connected      = 5,
     LoginInProgress = 6,
     LoginDone      = 7,
     LoggingOut       = 8,
} CONNECT_STATUS;

typedef enum {
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
} SECURITY_TYPE;

typedef enum {
	ExecTypeNew = 0,
	ExecTypePartialFill = 1,
	ExecTypeFill = 2,
	ExecTypeDoneForDay = 3,
	ExecTypeCanceled = 4,
	ExecTypeReplaced = 5,
	ExecTypePendingCancel = 6,
	ExecTypeStopped = 7,
	ExecTypeRejected = 8,
	ExecTypeSuspended = 9,
	ExecTypePendingNew = 10,
	ExecTypeCalculated = 11,
	ExecTypeExpired = 12,
	ExecTypeRestated = 13,
	ExecTypePendingReplace = 14,
	ExecTypeTrade = 15,
	ExecTypeTradeCorrect = 16,
	ExecTypeTraceCancel = 17,
	ExecTypeOrderStatus = 18,
    ExecTypeAllocation = 19,
    ExecTypeDistribution = 20,
    ExecTypeTicket = 21,
} EXEC_TYPE;

typedef struct {
    double Liguidation;
    double Maintain;
    double Required;
} Margin;

typedef struct {
    int AlertTransparent;
    int SltpType;
    int IsTradeByLots;
    int PriceIncrement;
    int AlertAppearSec;
    char* Language;
    int ChartDays;
    int IsShowTradeLevels;
    int IsShowEntryTips;
    int IsShowOrderConfirm;
    int IsRefreshOrderBox;
    int IsShowNewsUpdate;
    char* Theme;
    char* TimezoneCity;
    int IsShowAskLine;
} Profile;

typedef struct {
    char* Login;
    int Credit;
    int Login_cate;
    int Enable;
    int View_only;
    char* Settl_ccy;
    int Finance_type;
    char** Enabled_order_types;
    int Enabled_order_types_len;
    char** Enabled_tifs;
    int Enabled_tifs_len;
    int Last_order_id; 
    double Min_qty_on_order_entry;
    Margin Margin;
} AcctSetting;

typedef struct {
    char* Account;
    double Day_cum_qty;
    double Deposit_withdraw;
    int Num_tickets;
    double Bod_balance;
    double Balance;
    double Cls_pl;
    double Commission;
    Margin Margin;
} AcctValue;

typedef struct {
    char* Key;
    double Value;
} StringDouble;

typedef struct{
    double Equity;
    double Open_pl;
    double Total_pl;
    double Free_margin;
    double Buying_power;
    double Leverage_ratio;
    double Position_value;
    double Pct_equity;
} AcctValueCalculated;

typedef struct {
    char* Account;
    int Login_cate;
} LinkedAccount;

typedef struct {
    char* Login;
    AcctSetting Setting;
    AcctValue Value;
    AcctValueCalculated Value2;
    LinkedAccount* Linked_account;
    int Linked_account_len;
    Profile* Prof;
} AccountInfo;

typedef struct {
    int Index;
    int SecType;
    char* Tenor;
    char* Name;
    char* BaseCcy;
    char* TermCcy;
    char* MarginCcy;
    int BidChangeType;
    int AskChangeType;
    double LastBid;
    double LastAsk;
    double ContractSize;
    double LotSize;
    double MinLotSize;
    double MinQty;
    double MaxQty;
    int QtyDec;
    int PxDec;
    int PipDec;
    long long UpdateTime;
} Symbol;

typedef struct {
    char* Account;
    char* ClientOrderID;
    char* SymName;
    Symbol* Sym;
    char* User;
    char* OrigUser;
    char* FortexClientOrderID;
    char* FortexTicketExecSeqNo;
    char* OrigClientOrderID;
    char* TicketID;
    char* RefTicketID;
    char* Tag;
    char* OrigTag;
    char* ClientOrderIDFillNo;
    char* ExecDest;
    char* ExecBroker;
    char* DealCcy;
    char* SettDate;
    char* FillSeqNo;
    char* InstType;
    long long TransactTime;
    
    int IsPrincipalAgency;
    int Side;
    int Type;
    int MsgType;
    int Status;
    int Tif;
    int TicketType;
    int ExecType;
    int ExecInst;
    int HandInst;

    double Price;
    double Price2;
    double RefPrice;
    double AvgPrice;
    double SlipPage;
    double Quantity;
    double MinQty;
    double ReservedQty;
    double MaxShow;
    double LeaveQty;
    double CumQty;
    double FilledQty;
    double StopLoss;
    double TakeProfit;
    double StopPx;
    double LastPx;
    double LastQty;
} Order;

typedef struct {
    double Price;
    double Size;
    char* Mmid;
} MDEntry;

typedef struct {
    int Type;
    Symbol* Sym;
    int Domain;
    long long SendTime;
    long long RecvTime;
    MDEntry* Bids;
    int Bids_len;
    MDEntry* Asks;
    int Asks_len;
    double Hight;
    double Low;
    double Open;
    double Close;
} MarketData;

typedef struct {
    int Method;
    char* Account;
    char* SymName;
    Symbol* Sym;
    int Type;
    double OpenAvgPx;
    double OpenRefPx;
    double CloseAvgPx;
    double CloseRefPx;
    double Quantity;
    double ClosedPl;
    double Commission;
    char* DealCcy;
} NetPosition;

typedef struct {
    char** Accts;
    int Accts_len;
    long long StartTime;
    long long EndTime;
    int Offset;
    int Count;
    char* Sort; 
} TradeHistoryRequest;

typedef struct {
    int ErrorCode;
    char* ErrorMessage;
    char* Hint;
    char* Desc;
} FortexErrorCode;

typedef struct {
    char* CxlRejRespTo;
    char* OrdStatus;
    char* ClOrdId;
    char* OrigClOrdId;
    char* TxTime;
    char* Acct;
    char* CxlRejRsn;
} FortexOrderRejectInfo;

typedef struct {
    double ClosePl;
    double Commission;
    double OpenPl;
    double Swap;
    double Tp;
    double Sl;
}TicketSummary;

typedef struct {
    int Method;
    char* SymName;
    Symbol* Sym;
    Order* Order;
    int Side;
    char* Account;
    char* User;
    char* DealCcy;
    char* TicketId;
    char* SubId;
    char* OpenTime;
    int Status;
    double OpenPx;
    double OpenQty;
    double OpenConvRate;
    double ClosePx;
    double ClosedPl;
    double CloseQty;
    double OpenCommission;
    double StopLoss;
    double TakeProfit;
    double Quote;
    double OpenPl;
    double TicketCost;
    double TicketValue;
    TicketSummary Summary;
} Ticket;

typedef struct {
    int Method;
    char* SymName;
    Symbol* Sym;
    int Side;
    char* Account;
    char* DealCcy;
    int Status;
    double OpenPx;
    double OpenQty;
    double OpenConvRate;
    double ClosePx;
    double ClosedPl;
    double CloseQty;
    double CloseConvRate;
    double Commission;
} NetTicket;

typedef struct {
    int Side;
    char* SymName;
    double Price;
    double OrdQty;
    double Price2;
    int Tif;
    double Sl;
    double Tp;
    int OrdType;
} ExecReportOrder;

typedef struct {
    int Method;
    char* SymName;
    Symbol* Sym;
    char* Account;
    char* OrdUser;
    char* OrigOrdUser;
    char* OrdId;
    char* ClientOrdId;
    char* OrigClientOrdId;
    char* TicketId;
    char* RefTktId;
    char* OrdTag;
    char* FillSeqNo;
    char* Mmid;
    char* SettDate;
    char* DealCcy;
    char* ExecDst;
    int ActionType;
    int ExecType;
    char* ExecCode;
    int OrdStatus;
    int Side;
    int OrdType;
    long long TransactionTime;
    int Tif;
    double Price;
    double Price2;
    double StopPx;
    double AvgPx;
    double OrdQty;
    double LeaveQty;
    double CumQty;
    double LastPx;
    double LastQty;
    double Commission;
    double Sl;
    double Tp;
    double OriginSl;
    double OriginTp;
    double OriginPrice;
    double OriginPrice2;
    double OriginOrdQty;
    int OriginOrdType;
    int OriginOrdSide;
    int OriginTif;
    char* OriginExecDst;
    ExecReportOrder ReplaceOrderRef;
    FortexErrorCode ErrorCode;
    char* Comment;
    char* RefEvent;
    char* RefMsgType;
} ExecutionReport;

typedef struct {
    ExecutionReport* ExecRp;
    FortexErrorCode ErrCode;
    AcctValue AcctVal;
    Order* OrdInfo;
    Ticket* Ticket;
    int Ticket_len;
    NetTicket* NetTicket;
    int NetTicket_len;
    NetPosition* NetPos;
    int NetPos_len;
    char* Login;
} ExecutionAllData;

typedef struct {
    int Id;
    char* Sym;
    int ChartType;
    int Days;
    long long EndTime;
    int GroupSize;
} ChartRequest;

typedef struct {
    double Open;
    double Close;
    double High;
    double Low;
    double Volume;
    long long Time;
} ChartBar;

typedef struct {
    ChartRequest* ChartReq;
    ChartBar* Bars;
    int Bars_len;
} ChartData;

typedef struct {
    int Method;
    Symbol* Sym;
    char* Account;
    int Side;
    char* OpenTime;
    char* CloseTime;
    double OpenPl;
    double ClosePl;
    double OpenPx;
    double ClosePx;
    double OpQty;
    double ClsQty;
    double OpCnvRt;
    double ClCntRt;
    double Commission;
    double Swap;
    double Tp;
    double Sl;
    char* TicketId;
    char* Type;
    char* Ccy;
    char* TxTime;
    double CcyVal;
    int Status;
    TicketSummary Summary;
} Event;

typedef enum {
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
} FortexObjectType;

typedef struct {
    void* Message;
    FortexObjectType Type;
    int ExtraInt;
} FortexObject;

/*============================================================*/
// define callback function
typedef void (*CBInitSequence)();
typedef void (*CBLogin)();
typedef void (*CBLogout)();
typedef void (*CBAccountInfo)(AccountInfo*);
typedef void (*CBSymbolInfo)(Symbol*, int);
typedef void (*CBMarketData)(MarketData*);
typedef void (*CBNetPosition)(NetPosition*, int);
typedef void (*CBOpenOrder)(Order*, int, int);
typedef void (*CBNetTicket)(NetTicket*, int);
typedef void (*CBTicket)(Ticket*, int);
typedef void (*CBExecutionReport)(ExecutionReport*);
typedef void (*CBExecutionData)(ExecutionAllData*);
typedef void (*CBChartData)(ChartData*);
typedef void (*CBTradeHistory)(TradeHistoryRequest*, ExecutionReport*, int);
typedef void (*CBOrdRejectData)(FortexErrorCode*, FortexOrderRejectInfo*);
typedef void (*CBRejectData)(FortexErrorCode*);
typedef void (*CBEvent)(Event*, int, char**, int);
typedef void (*CBCheckWebServer)();
typedef void (*CBReloginToRest)();
typedef void (*CBDepositedWithdrawn)(double, char*, char*, char*);
typedef void (*CBCertRejectData)();

#ifdef __cplusplus
 }
#endif

#endif