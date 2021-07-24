//ͳ��Ʊ������ 

#include "SalesAnalysis_UI.h"
#include "../Common/list.h"
#include "../Common/common.h"
#include "../Service/SalesAnalysis.h"
#include <conio.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

//�ⲿ����gl_CurUser ���ڴ洢��¼�û���Ϣ
extern account_t gl_CurUser;

//�����ҳ������ҳ���СΪ5
static const int SALESANALYSIS_PAGE_SIZE = 5;

//��Ժ�������а���,������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�
void SalesAnalysis_UI_MgtEntry() {
	int i;
	char choice;

	salesanalysis_list_t head;
	salesanalysis_node_t *pos;
	Pagination_t paging;

	List_Init(head, salesanalysis_node_t);
	paging.offset = 0;
	paging.pageSize = SALESANALYSIS_PAGE_SIZE;

	//��������
	paging.totalRecords = SalesAnalysis_Srv_StaticSale(head);
	SalesAnalysis_Srv_SortBySale(head);   //����Ʊ������

	Paging_Locate_FirstPage(head, paging);
	do{
		printf("\t\t\t=========================================================================\n");
		printf("\t\t\t\t\t\t\t""Ʊ��ͼ��\n");
		printf("\t\t\t-------------------------------------------------------------------------\n");
		printf("\t\t\t��Ŀ��\t\t����\t\t��Ʊ��\tƱ��\t��ӳʱ��\t\t��ӳʱ��\n");

			for (i = 0, pos = (salesanalysis_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("\t\t\t%-10s\t%-10s\t%-5ld\t%-5ld\t%d-%d-%d\t%d-%d-%d\t\n",
					pos->data.name, pos->data.area, pos->data.totaltickets,pos->data.sales,
					pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,
					pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
			pos = pos->next;
		}
		printf("\t\t\t---------- �� %2d �� --------------------------- �� %2d/%2d ҳ --------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("\t\t\t*************************************************************************\n");
		printf("\t\t\t[P]��һҳ	|	[N]��һҳ	|	[R]����\n");
		printf("\n\t\t\t=========================================================================\n");
		fflush(stdin);
		printf("\t\t\t��ѡ���ܣ�");
		scanf("%c",&choice);

		switch(choice)
		{
			case 'P':
			case 'p':
					if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, salesanalysis_node_t);
					}
				break;
			case 'N':
			case 'n':
					if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, salesanalysis_node_t);
					}
				break;
		}
	} while(choice!='R' && choice!='r');
		List_Destroy(head, salesanalysis_node_t);
}

