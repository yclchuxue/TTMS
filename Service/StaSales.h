
#ifndef STASALES_H_
#define STASALES_H_

#include "../View/Play_UI.h"
#include "../Common/List.h"
#include "../Common/common.h"
#include "../Persistence/Sale_Persist.h"
#include "../Persistence/Account_Persist.h"

int StaSales_Srv_CompSaleVal(int usrID, ttms_date_t stDate, ttms_date_t endDate);

#endif 
