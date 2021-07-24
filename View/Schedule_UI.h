#ifndef SCHEDULE_UI_H_
	#define SCHEDULE_UI_H_

	#include "../Common/List.h"
	#include "../Common/Common.h"
	#include "../Service/Schedule.h"
	#include "../View/Play_UI.h"

	static const int SCHEDULE_PAGE_SIZE=5;

	
	
	void Schedule_UI_MgtEntry(int play_id);
	int Schedule_UI_Add(int play_id);
	int Schedule_UI_Modify(int id);
	int Schedule_UI_Delete(int id);
	
	void Schedule_UI_ListAll(void);
	
	//����ID�Ų�ѯ�ݳ��ƻ���Ϣ,��������ҵľ�ĿID�ţ�������ҵ��ľ�Ŀ��Ϣ��
	//���� 0��ʾδ�ҵ���1��ʾ�ҵ���
	int Schedule_UI_Query(int id);

#endif // 
