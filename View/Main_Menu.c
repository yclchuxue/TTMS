/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* File name: Main_Menu.c			 	 
* Description : TTMS 系统主菜单	
* Author:   XUPT  		 
* Version:  v.1 	 
* Da
*/

#include "Main_Menu.h"

#include <stdio.h>

#include "Studio_UI.h"

#include "Sale_UI.h"

#include "SalesAnalysis_UI.h"

//系统主菜单 
void Main_Menu_Adinm(void) {
	char choice;
	do { 
		printf("\n==================================================================\n");
		printf("**************************** 河西村剧院管理系统****************************\n");
		printf("[S] 管理演出厅\n");
		printf("[A] 账户管理\n");
		printf("[E] 退出\n");
		printf("\n==================================================================\n");
		printf("请输入你的选择:");
		fflush(stdin);
		choice = getchar();
		switch (choice) {
		case 'S':
		case 's':
			Studio_UI_MgtEntry();
			break;
		case 'A':
		case 'a':
			Account_UI_MgtEntry();
			break;
		}
	} while ('E' != choice && 'e' != choice);
}
void Main_Menu_manger(void) {
	char choice;
	do { 
		printf("\n==================================================================\n");
		printf("**************************** 河西村剧院管理系统****************************\n");
		//printf("[S] 管理演出厅\n");
		printf("[P] 管理剧目\n");
		//printf("[T] 售票\n");
		//printf("[R] 退票\n");
		printf("[Q] 查询\n");
		printf("[N] 票房统计\n");
		printf("[D] 统计销售额\n");
		//printf("[A] 账户管理\n");
		printf("[E] 退出\n");
		printf("\n==================================================================\n");
		printf("请输入你的选择:");
		fflush(stdin);
		choice = getchar();
		switch (choice) {
		//case 'S':
		//case 's':
			//Studio_UI_MgtEntry();
			//break;
		case 'P':
		case 'p':
			Play_UI_MgtEntry(0);
			break;
		case 'Q':
		case 'q':
			Queries_Menu();
			break;
		case 'd':
		case 'D':
			StaSales_UI_MgtEntry();
			break;
			
		//case 'T':
		//case 't':
			//Sale_UI_MgtEntry();
			//break;
/*
		case 'R':
		case 'r':
			Sale_UI_ReturnTicket();
			break;*/
		case 'N':
		case 'n':
			SalesAnalysis_UI_MgtEntry();
			break;

/*	    case 'A':
		case 'a':
			Account_UI_MgtEntry();
			break;*/
		}
	} while ('E' != choice && 'e' != choice);
}
void Main_Menu_Sale(void) {
	char choice;
	do { 
		printf("\n==================================================================\n");
		printf("**************************** 河西村剧院管理系统****************************\n");
		//printf("[S] 管理演出厅\n");
		//printf("[P] 管理剧目\n");
		printf("[T] 售票\n");
		printf("[R] 退票\n");
		printf("[Q] 查询\n");
		//printf("[N] 票房统计\n");
		printf("[D] 统计销售额\n");
		//printf("[A] 账户管理\n");
		printf("[E] 退出\n");
		printf("\n==================================================================\n");
		printf("请输入你的选择:");
		fflush(stdin);
		choice = getchar();
		switch (choice) {
	/*	case 'S':
		case 's':
			Studio_UI_MgtEntry();
			break;
		case 'P':
		case 'p':
			Play_UI_MgtEntry(0);
			break;*/
		case 'd':
		case 'D':
			StaSales_UI_MgtEntry();
			break;
		case 'Q':
		case 'q':
			Queries_Menu();
			break;
		case 'T':
		case 't':
			Sale_UI_MgtEntry();
			break;
/*
		case 'R':
		case 'r':
			Sale_UI_ReturnTicket();
			break;
		case 'N':
		case 'n':
			SalesAanalysis_UI_MgtEntry();
			break;
*/
	/*	case 'A':
		case 'a':
			Account_UI_MgtEntry();
			break;*/
		}
	} while ('E' != choice && 'e' != choice);
}

void Queries_Menu()
{
	system("cls");
	printf("\n\n\n\n\n------------------------------------\n");
	printf("1.查询演出\n");
	printf("2.查询演出票\n");
	printf("3.退出\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	int choice;
	scanf("%d",&choice);
	switch(choice)
	{
		case 1:Schedule_UI_ListAll();
			break;
		case 2:Ticket_UI_Query();
			break;
		case 3:return ;
		default :
			printf("\n\n\n输入有误");
			system("pause");
	}
}
