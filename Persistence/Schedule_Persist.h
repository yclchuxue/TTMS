#ifndef SCHEDPERSIST_H_
#define SCHEDPERSIST_H_

	#include "../Service/Schedule.h"


int Schedule_Perst_SelectByID(int ID, schedule_t *buf);

//�������ݳ��ƻ���Ϣ������һ����������listΪ�ݳ��ƻ���Ϣ�����ͷָ��
//��� ��ʾ��������ʱ������ռ�ʧ�ܣ����ز��ҵ��ļ�¼��Ŀ
int Schedule_Perst_SelectAll(schedule_list_t list); 

//����ID�����ݳ��ƻ�
//���ݳ��ƻ������ļ���������ָ����Ŀ��������ݳ��ƻ���Ϣ��������list
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id);

//�������ݳ��ƻ�
//���ݳ��ƻ��ļ�Schedule.dat��ĩβ���һ�����ݳ��ƻ�
int Schedule_Perst_Insert(schedule_t * date);
//�����ݳ��ƻ�
//�����ݳ��ƻ������ļ�Schedule.dat�е�һ���ݳ��ƻ���Ϣ
int Schedule_Perst_Update(const schedule_t * date);

//����IDȥ���ݳ��ƻ�
//ȥ���ݳ��ƻ������ļ�Schedule.dat��ָ��ID���ݳ��ƻ�
int Schedule_Perst_RemByID(int id);

int Schedule_Perst_SelectByID_ticket(int id,schedule_t *buf);






#endif // SCHEDPERSIST_H_
