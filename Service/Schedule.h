#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include "../Common/Common.h"



typedef struct {//�ݳ��ƻ����Ͷ���
	long int id;            //�ݳ��ƻ�ID��
	int play_id;       //�ݳ��ľ�ĿID��
	int studio_id;     //�ݳ��ķ�ӳ��ID��
	user_date_t date;  //�ݳ�����
	user_time_t time;  //�ݳ�ʱ��
	int seat_count;    //��λ����
}schedule_t;

//�����ݳ��ƻ���Ϣ��˫������ṹ
typedef struct schedule_node {
	schedule_t data;
	struct schedule_node *next, *prev;
} schedule_node_t, *schedule_list_t;


//ȡ�������ݳ��ƻ���Ϣ������һ���������� list�ݳ��ƻ���Ϣ�����ͷָ��
//����������ز��ҵ��ļ�¼��Ŀ
int Schedule_Srv_FetchAll(schedule_list_t list);

//���ݾ�ĿID�����Ҿ�Ŀ������������ݳ��ƻ�
//int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id);



//������ݳ��ƻ�
//������date����Ϊʵ�ε��ó־û��㴢�����ݳ��ƻ������������־û��㺯������ֵ���ݸ�����㺯��
int Schedule_Srv_Add(schedule_t * date);
//�޸��ݳ��ƻ�
//�ò���date�е��ݳ��ƻ�����Ϊʵ�Σ����ó־û��㺯�����޸��ļ��оɵ��ݳ��ƻ���Ϣ
int Schedule_Srv_Modify(const schedule_t * date);

//����IDɾ���ݳ��ƻ�
//����ID�����õ��ó־û��㺯��ɾ���ݳ��ƻ�
int Schedule_Srv_DeleteByID(int id);

//���վ�ĿID�ţ�������ݳ��ƻ���Ϣȡ��������һ������
//���� listΪ���ҵ����ݳ��ƻ���Ϣ�����ͷָ�룬play_idΪ��Ŀ��ID�ţ������
//���ز��ҵ��ļ�¼��Ŀ
int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id);

int Schedule_Srv_FetchByID(int id,schedule_t *buf);
 
 int Schedule_Srv_StatRevByPlay(int play_id, int *soldCount);
 

#endif //SCHEDULE_H_
