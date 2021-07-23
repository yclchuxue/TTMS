//��ȡ��ĿƱ��
//��ĿƱ������ 

#ifndef SALESANALYSIS_H_
#define SALESANALYSIS_H_

#include "Sale.h"
#include "Ticket.h"
#include "Play.h"
#include "Schedule.h"
#include "../Common/List.h"


typedef struct {
	int play_id; 	//��Ŀ���
	char name[31]; 	//��Ŀ����
	char area[9]; 	//��Ŀ����
	int duration; 	//�Է���Ϊ��λ
	long totaltickets; //��ĿƱ��������
	long sales; 	//��ӰƱ��
	int price; 		//��ĿƱ��
	ttms_date_t start_date; 	//��Ŀ��ӳ����
	ttms_date_t end_date; 	//��Ŀ��ӳ����
} salesanalysis_t;

//˫������
typedef struct salesanalysis_node {
	salesanalysis_t data;
	struct salesanalysis_node *next, *prev;
} salesanalysis_node_t, *salesanalysis_list_t;


//��ȡ��ĿƱ�� 
int Salesanalysis_Srv_StaticSale(salesanalysis_list_t list);

//��ĿƱ������ 
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list);

void SalesAnalysis_Srv_addnode(salesanalysis_list_t list,salesanalysis_node_t *p);

#endif //SALESANALYSIS_H_
