//��ȡ��ĿƱ��
//��ĿƱ������ 

#include <string.h>
#include <stdio.h>
#include "SalesAnalysis.h"
//#include "../Persistence/Sale_Persist.h"
//#include "../Persistence/SalesAnalysis_Persist.h"
#include "Sale.h"
#include "Ticket.h"
#include "Play.h"
#include "Schedule.h"
#include "../Common/list.h"


//��Ͼ�ĿPlay.dat,ͳ���������ݣ��������۷���salesanalysis_list_t list��������list������
int SalesAnalysis_Srv_StaticSale(salesanalysis_list_t list)
{
	int cnt = 0;
	assert(NULL!=list);
	play_list_t playlist;
	play_node_t *pos;
	int sold;//�洢������ЧƱ��
	salesanalysis_list_t p;
	salesanalysis_node_t newNode;
	List_Free(list,salesanalysis_node_t);
	List_Init(playlist,play_node_t);
	Play_Srv_FetchAll(playlist);


	List_ForEach(playlist,pos)
	{
		p  =   (salesanalysis_list_t)malloc(sizeof(salesanalysis_node_t));
		strcpy(p->data.area,pos->data.area);
		p->data.duration = pos->data.duration;
		p->data.end_date = pos->data.end_date;
		strcpy(p->data.name,pos->data.name);
		p->data.play_id = pos->data.id;
		p->data.price = pos->data.price;
		p->data.start_date = pos->data.start_date;

		p->data.sales = Schedule_Srv_StatRevByPlay(pos->data.id,&sold);//Ʊ��
		p->data.totaltickets = sold;//����

		//List_AddTail(list,p);
		// printf("��Ŀid :%d\n",pos->data.id);
		cnt++;
	}

	List_Destroy(playlist,play_node_t);
	return cnt;

}


//�Ծ�Ժ��ӰƱ������
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list)
 { 
	salesanalysis_node_t *p;    
  	if(list == NULL) return ;   
    list->prev->next = NULL;//��ѭ���Ͽ�     
	salesanalysis_list_t listleft =  list->next;//listleftָ���һ�����ݽڵ� 
	list->next = list->prev = list;//�������������Ϊ��  
	while(listleft != NULL)    
	{   
		       p = listleft;     
			    listleft = listleft->next;       
    SalesAnalysis_Srv_addnode(list,p);//+    
	}     
	return ; 
} 

void SalesAnalysis_Srv_addnode(salesanalysis_list_t list,salesanalysis_node_t *p)
{ 	
	salesanalysis_node_t *cur;   
    if(list == NULL)  
    {         
		List_AddTail(list,p);     
	}     
	else   
  	{        
	   cur = list->next;       
	     while(cur != list)      
		    {            
				 if(p->data.sales > cur->data.sales) break;  
				            cur = cur->next;      
		   }      
		      List_InsertBefore(cur,p);    
	}   
	  return ;
 }




