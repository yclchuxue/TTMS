#ifndef TICKET_PERSIST_H_
#define TICKET_PERSIST_H_

#include"../Service/Ticket.h"
#include"../Service/Schedule.h"
#include"../Service/Seat.h"

int Ticket_Perst_SelectAll(ticket_list_t list);

int  Ticket_Perst_Insert(seat_list_t list,int schedule_id);      

int Ticket_Perst_Update(const ticket_t *data);

int  Ticket_Perst_Rem(int schedule_id);    

int Ticket_Perst_SelBySchID(int ID,ticket_list_t list);

int Ticket_Perst_SelByID(int id,ticket_t *buf); 

int Ticket_Perst_SelByticketID(int ticket_id,ticket_t *buf);

int Ticket_Perst_SelectBySchID(ticket_list_t list, int schedule_id);
#endif
