#include "Schedule.h"
#include "../Common/List.h"
#include "../Persistence/Schedule_Persist.h"
//#include "Ticket.h"
#include <stdio.h>

 int Schedule_Srv_FetchByID(int id,schedule_t *buf)
{
	return Schedule_Perst_SelectByID(id,buf);
}

//���ݾ�ĿID�����Ҿ�Ŀ������������ݳ��ƻ�
int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id)
{
	int ret;
	ret = Schedule_Perst_SelectByPlay(list, play_id);

	return ret;
}

//ȡ�������ݳ��ƻ���Ϣ������һ���������� list�ݳ��ƻ���Ϣ�����ͷָ��
//����������ز��ҵ��ļ�¼��Ŀ
inline int Schedule_Srv_FetchAll(schedule_list_t list) {
	return Schedule_Perst_SelectAll(list);
}

//���վ�ĿID�ţ�������ݳ��ƻ���Ϣȡ��������һ������
//���� listΪ���ҵ����ݳ��ƻ���Ϣ�����ͷָ�룬play_idΪ��Ŀ��ID�ţ������
//���ز��ҵ��ļ�¼��Ŀ
//int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id) {
//	return Schedule_Perst_SelectByPlay(list, play_id);
//}

//������ݳ��ƻ�
//������date����Ϊʵ�ε��ó־û��㴢�����ݳ��ƻ������������־û��㺯������ֵ���ݸ�����㺯��
int Schedule_Srv_Add(schedule_t * date)
{
	int ret;
	ret = Schedule_Perst_Insert(date);   //���ݳ��ƻ��ļ�Schedule.dat��ĩβ���һ�����ݳ��ƻ�

	return ret;
}

//�޸��ݳ��ƻ�
//�ò���date�е��ݳ��ƻ�����Ϊʵ�Σ����ó־û��㺯�����޸��ļ��оɵ��ݳ��ƻ���Ϣ
int Schedule_Srv_Modify(const schedule_t * date)
{
	int ret = 0;
	ret = Schedule_Perst_Update(date);

	return ret;
}


//���ݾ�ĿID��ȡƱ�� 
int Schedule_Srv_StatRevByPlay(int play_id, int *soldCount)
{
	int value = 0;//�洢Ʊ��
	int sold = 0; //�洢��Ч��Ʊ����
	schedule_list_t list;
	schedule_node_t *p;
	*soldCount = 0;
	List_Init(list,schedule_node_t);
	Schedule_Perst_SelectByPlay(list,play_id);//�����ݳ��ƻ�����list
	List_ForEach(list,p)
	{
		value += Ticket_Srv_StatRevSchID(p->data.id, &sold);     //
		*soldCount = *soldCount+sold;
	}
	List_Destroy(list,schedule_node_t);//��������
	return value;
}


//����IDɾ���ݳ��ƻ�
//����ID�����õ��ó־û��㺯��ɾ���ݳ��ƻ�
int Schedule_Srv_DeleteByID(int id)
{
	int ret = 0;
	ret = Schedule_Perst_RemByID(id);

	return ret;
}
