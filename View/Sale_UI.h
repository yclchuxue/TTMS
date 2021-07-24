#ifndef SALE_UI_H_
#define SALE_UI_H_
#include"../Service/Seat.h"
#include"../Service/Ticket.h"
void Sale_UI_MgtEntry(void);

void Sale_UI_ShowScheduler(int playID);

//int Sale_UI_SellTicket(ticket_list_t ticketList,seat_list_t seatList);

int Sale_UI_ShowTicket(int schedule_id);

void Sale_UI_ReturnTicket();

int Sale_UI_SellTicket(int ticket_id);

char * Status(int i);

void Ticket_Info(ticket_t *data);
#endif
