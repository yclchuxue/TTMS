#include "StaSales_UI.h"

#include "../Common/List.h"
#include "../Service/Sale.h"
#include "../Service/Account.h"
#include "../Service/StaSales.h"
#include <stdio.h>
#include <time.h>

extern account_t gl_CurUser;

void StaSales_UI_MgtEntry ()
{

    if(gl_CurUser.type==USR_CLERK)        //sale
    {
        StaSales_UI_Self();
    }
    else if(gl_CurUser.type==USR_MANG  )      //jingli
    {
        StaSales_UI_Clerk();
    }
    else
    {
        printf("�˺�û��Ȩ�ޣ�");
        printf("��{ENTER}�����ϲ�!\n");
        getchar();
        
    }
    system("cls");
    return ;
}


void StaSales_UI_Self(){
    int id;
    id=gl_CurUser.id;

    ttms_date_t curdate;
    ttms_date_t startdate;
    ttms_date_t enddate;

    struct tm *local;
    time_t t;
    t=time(&t);
    local=localtime(&t); 
    
    curdate.year=local->tm_year+1900;
    curdate.month=local->tm_mon+1;
    curdate.day=local->tm_mday;

    startdate.year=local->tm_year+1900;
    startdate.month=local->tm_mon+1;
    startdate.day=1;

    startdate.year=local->tm_year+1900;
    startdate.month=local->tm_mon+1;
    startdate.day=31;

    int choice;
    do
    {
        system("cls");
    printf("=======================================\n");
    printf("[1]�������۶� | [2]�������۶� | [0]����\n");
    printf("---------------------------------------\n");

    printf("��ѡ�������ѯ��ѡ�");
    setbuf(stdin,NULL);
    scanf("%d",&choice);
    setbuf(stdin,NULL);
    system("cls");

    int count;
    switch(choice)
    {
        case 1: 
            system("cls");
            count=StaSales_Srv_CompSaleVal(id,curdate,curdate);
            printf("===========================================\n");
            printf("%5s  %5s  %5s","ID","����","����\n");
            printf("%5d  %5d  %5s\n",gl_CurUser.id,gl_CurUser.type,gl_CurUser.username);
            printf("�����ܶ�Ϊ��%5d\n",count);
            printf("******************************************\n");
            break;
        case 2:
            system("cls");
            count=StaSales_Srv_CompSaleVal(id,startdate,enddate);
            printf("===========================================\n");
            printf("%5s  %5s  %5s","ID","����","����\n");
            printf("%5d  %5d  %5s\n",gl_CurUser.id,gl_CurUser.type,gl_CurUser.username);
            printf("�����ܶ�Ϊ��%5d\n",count);
            printf("******************************************\n");
            break;
    }
    system("pause");
    }while(choice != 0);
}


void StaSales_UI_Clerk()
{
    int id;
    char username[100];
    ttms_date_t startdate;
    ttms_date_t enddate;
    system("cls");
    printf("��������ƱԱ������");
    setbuf(stdin,NULL);
    scanf("%s",username);
    setbuf(stdin,NULL);
    system("cls");

    account_t user;//�洢�û���Ϣ
    int i;
    i=Account_Srv_FetchByName(username,&user);
    id=user.id;
    if(i==0)
    {
        printf("���û������ڣ�");
        return;
    }
    printf("������Ҫ��ѯ�Ŀ�ʼ����(x/y/z):\n");
    setbuf(stdin,NULL);
    scanf("%d/%d/%d",&startdate.year,&startdate.month,&startdate.day);
    setbuf(stdin,NULL);
    printf("����Ҫ��ѯ�Ľ�������(x/y/z)\n");
    setbuf(stdin,NULL);
    scanf("%d/%d/%d",&enddate.year,&enddate.month,&enddate.day);
    int count;
    count=StaSales_Srv_CompSaleVal(id,startdate,enddate);
    printf("\n\n\n\n");
    printf("===========================================\n");
    printf("%5s  %5s  %5s","ID","����","����\n");
    printf("%5d  %5d  %5s\n",user.id,user.type,user.username);
    printf("�����ܶ�Ϊ��%5d\n",count);
    printf("*******************************************\n");
    system("pause");
    setbuf(stdin,NULL);
    getchar();


}
