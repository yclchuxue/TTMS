#include "Seat_UI.h"
#include "../Service/Seat.h"

#include "../Service/Studio.h"
#include "../Common/List.h"
#include <stdio.h>

/*
��ʶ����TTMS_SCU_Seat_UI_S2C 
�������ܣ�������λ״̬��ȡ������ʾ���š�
����˵����statusΪseat_status_t���ͣ���ʾ��λ״̬��
�� �� ֵ���ַ��ͣ���ʾ��λ�Ľ�����ʾ���š�
*/
inline char Seat_UI_Status2Char(seat_status_t status) {
	
	char statusChar;
	
	switch (status) 
	{
		case SEAT_GOOD:	
			statusChar = '#';
			break;
		case SEAT_BROKEN:
			statusChar = '~';
			break;
		case SEAT_NONE:
		default:
			statusChar = ' ';
			break;
	}
	
	return statusChar;
}

/*
��ʶ����TTMS_SCU_Seat_UI_C2S
�������ܣ�����������Ż�ȡ��λ״̬��
����˵����statusCharΪ�ַ��ͣ���ʾ������λ��������š�
�� �� ֵ��seat_status_t���ͣ���ʾ��λ��״̬��
*/
inline seat_status_t Seat_UI_Char2Status(char statusChar) {
	
	seat_status_t status;
	
	switch (statusChar) 
	{
		case '#':	
			status = SEAT_GOOD;
			break;
		case '~':	
			status = SEAT_BROKEN;
			break;
		default:
			status = SEAT_NONE;
			break;
	}
	
	return status;
}

/*
��ʶ����TTMS_SCU_Seat_UI_MgtEnt
�������ܣ�����������λ����ں�������ʾ��ǰ����λ���ݣ����ṩ��λ������ӡ��޸ġ�ɾ�����ܲ�������ڡ�
����˵����roomIDΪ���ͣ�����Ҫ������λ���ݳ���ID��
�� �� ֵ���ޡ�
*/ 
void Seat_UI_MgtEntry(int roomID) {
	
	studio_t stu;
	seat_list_t list;
	seat_node_t *node;
	seat_node_t *p;
	char choice;
	int changedCount, i, j;
	
	List_Init(list, seat_node_t);
	
	if(!Studio_Srv_FetchByID(roomID, &stu))
	{
		printf("�ݳ���������\n");
		return ;
	}
	
	if(!Seat_Srv_FetchByRoomID(list, roomID))
	{
		stu.seatsCount = Seat_Srv_RoomInit(list, roomID, stu.rowsCount, stu.colsCount);
		Studio_Srv_Modify(&stu);
	}
	
	while(1)
	{
		system("cls");
		printf("======================================================================\n");
		printf("===============================��λ��=================================\n");
		for(i = 1; i < stu.rowsCount+1; i++)
		{
			j = 1;
			printf("\t");
			List_ForEach(list, node)
			{
				if (node->data.row == i) {
					while (node->data.column != j) {
						printf(" ");
						j++;
					}
					printf("%c", Seat_UI_Status2Char(node->data.status));
					j++;
				}
			}
			printf("\n");	
		}
		printf("======================================================================\n");
		printf("----------------------------\n");
		printf("1.�����λ(a/A)\n");
		printf("2.�޸���λ(u/U)\n");
		printf("3.ɾ����λ(d/D)\n");
		printf("4.������һ��(r/R)\n");
		printf("----------------------------\n");
		printf("������Ӧ��ĸ��"); 
		fflush(stdin);
		choice = getchar();
		
		if((choice == 'a') || (choice == 'A'))
		{
			changedCount = Seat_UI_Add(list, roomID, stu.rowsCount, stu.colsCount);
			if(changedCount > 0)
			{
				stu.seatsCount += changedCount;
				Studio_Srv_Modify(&stu);
			}
		}else if((choice == 'u') || (choice == 'U'))
		{
			Seat_UI_Modify(list, stu.rowsCount, stu.colsCount);
		}else if((choice == 'd') || (choice == 'D'))
		{
			changedCount = Seat_UI_Delete(list, stu.rowsCount, stu.colsCount);
			if(changedCount > 0)
			{
				stu.seatsCount -= changedCount;
				Studio_Srv_Modify(&stu);
			}
		}else if((choice == 'r') || (choice == 'R'))
			return ;
	}
}

/*
ʶ����TTMS_SCU_Seat_UI_Add
�������ܣ��������һ���µ���λ���ݡ�
����˵������һ������listΪseat_list_t����ָ�룬ָ����λ����ͷָ�룬
         �ڶ�������rowsCountΪ���ͣ���ʾ��λ�����кţ�����������colsCountΪ���ͣ���ʾ��λ�����кš�
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ��������λ�ı�־��
*/
int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {  //����һ����λ
	char choice;
	int newRecCount = 0;
	seat_node_t *p;
	seat_t rec;

	while(1)
	{
		while(1)
		{
			printf("�����кţ�");
			scanf("%d", &rec.row);
			printf("�����кţ�");
			scanf("%d", &rec.column);
			if(rec.row <= row && rec.column <= column)       //����Ҫ��
				break;
			else
				printf("��������ȷ���\n");
		}

		p = Seat_Srv_FindByRowCol(list, rec.row, rec.column);   //�ж���λ�Ƿ��Ѿ�����
		if(p)
			printf("����λ�Ѿ�����\n");
		else
		{
			rec.id = EntKey_Perst_GetNewKeys("Seat", 1);	
			rec.roomID = roomID;
			rec.status = SEAT_GOOD;    
			if(Seat_Srv_Add(&rec))
			{
				newRecCount++;
				printf("��ӳɹ�\n");
				p = (seat_node_t *)malloc(sizeof(seat_node_t));
				p->data = rec;
				Seat_Srv_AddToSoftedList(list, p);
			}
			else
				printf("����λ���ʧ�ܣ�\n");
		}
		
		printf("-------------------------------------------\n");
		printf("1.�������(a/A)\n2.����(r/R)\n");
		printf("�������ѡ��:");
		fflush(stdin);
		scanf("%c", &choice);
		if(choice == 'r' || choice == 'R')
		{
			break;
		}
	}
	return newRecCount;
}

/*
��ʶ����TTMS_SCU_Seat_UI_Mod 
�������ܣ������޸�һ����λ���ݡ�
����˵������һ������listΪseat_list_t����ָ�룬ָ����λ����ͷָ�룬�ڶ�������rowsCountΪ���ͣ���ʾ��λ�����кţ�����������colsCountΪ���ͣ���ʾ��λ�����кš�
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ��޸�����λ�ı�־��
*/
int Seat_UI_Modify(seat_list_t list, int row, int column) {
	
	int ret = 0;
	int newrow, newcolumn;
	char choice;
	seat_node_t *p;

	while (1)
	{
		while(1)
		{
			printf("�������кţ�");
			scanf("%d", &newrow);
			printf("�������к�: ");
			scanf("%d", &newcolumn);
			if(newrow <= row && newcolumn <= column)
				break;
			else
				printf("��λ������\n");
		}

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn); 
		if(p)
		{
			printf("-----------------\n");
			printf(" :����λ\n");
			printf("~:����λ\n");
			printf("#:��ѡ����λ\n"); 
			printf("-----------------\n");
			printf("ѡ����λ״̬:");
			fflush(stdin);
			p->data.status = Seat_UI_Char2Status(getchar());                         
			if (Seat_Srv_Modify(&(p->data))) 
			{
				ret = 1;
				printf("��λ���³ɹ�\n");
			} 
			else
				printf("��λ����ʧ��\n");
		}
		else
		{
			printf("��λ������\n");
		}
		printf("-----------------------------------\n");
		printf("1.�������(u/U)\n2.����(r/R)\n");
		fflush(stdin);
		scanf("%c", &choice);
		if(choice == 'r' || choice == 'R')
		{
			break;
		}
	}
	
	return ret;
}

/*
��ʶ����TTMS_SCU_Seat_UI_Del
�������ܣ�����ɾ��һ����λ�����ݡ�
����˵������һ������listΪseat_list_t����ָ�룬ָ����λ����ͷָ�룬�ڶ�������rowsCountΪ���ͣ���ʾ��λ�����кţ�����������colsCountΪ���ͣ���ʾ��λ������
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ�ɾ������λ�ı�־��
*/
int Seat_UI_Delete(seat_list_t list, int row, int column) {
	
	int ret = 0;
	int newrow, newcolumn;
	seat_node_t *p;
	char choice;

	do 
	{
		do 
		{
			printf("��λ�кţ� ");
			scanf("%d", &(newrow));
			printf("��λ�кţ� ");
			scanf("%d", &(newcolumn));
		} while ((newrow > row) || (newcolumn > column));

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn);
		if (p) 
		{
			if (Seat_Srv_DeleteByID(p->data.id)) 
			{
				printf("��λɾ���ɹ�\n");
				ret++;
				List_FreeNode(p);	
			}
		} 
		else 
		{
			printf("��λ������\n");
		}

		printf("----------------------------\n");
		printf("1.����ɾ��(d/D)\n2.����(r/R)\n");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('d' == choice || 'D' == choice);
	
	return ret;

}
