#ifndef Account_PERSIST_H_
#define  Account_PERSIST_H_
#include "../Service/Account.h" 
int Account_Perst_CheckAccFile();
int Account_Perst_Insert(account_t *data) ;
int Account_Perst_Update(const account_t * data);
int Account_Perst_SelectAll(account_list_t list);
int Account_Perst_RemByID(int id);
int Account_Perst_DeleteByID(int id);

int Account_Perst_SelByName(char usrName[], account_t *buf);

#endif
