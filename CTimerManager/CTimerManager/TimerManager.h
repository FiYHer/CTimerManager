/*
��ʱ��ʵ��
*/
#pragma once
#include <list>

#include <windows.h>
#include <process.h>
using namespace std;


///��ʱ���������ռ�
namespace CTimerManagerSpace
{

	//��ʱ����Ϣ�ṹ��
	typedef struct strTimerManagerInfo
	{
		unsigned int timerInterval;//��ʱ�����м��
		unsigned int timerCreateTime;//��ʱ������ʱ��
		unsigned int timerLastRun;//��ʱ����һ�����е�ʱ��
		int timerID;//��ʱ����ID��
		bool timerDelete;//��ʱ���ǲ���Ҫɾ��
		strTimerManagerInfo()
		{
			timerInterval = 0;
			timerCreateTime = 0;
			timerLastRun = 0;
			timerID = -1;
			timerDelete = false;
		}
	}TimerManagerInfo,*PTimerManagerInfo;
	
	typedef list<TimerManagerInfo> TimerManagerInfoList;

	//��ʱ����
	class CTimerManager
	{
	private:
		//��ʱ���б�
		TimerManagerInfoList m_TimerList;
	public:
		CTimerManager();
		virtual ~CTimerManager();

		CTimerManager(const CTimerManager&) = delete;
		CTimerManager& operator=(const CTimerManager&) = delete;
		CTimerManager(CTimerManager&&) = delete;
	public:
		//���麯��
		virtual int OnTimer(int)=0;

		//��ʱ���߳�
		static unsigned int __stdcall TimerRunThread(PVOID);
	public:
		//���һ����ʱ��
		void AddTimer(int,unsigned int);

		//ɾ��һ����ʱ��
		void DeleteTimer(int);

		//������ж�ʱ��
		void ClearTimer();

		//���ɾ����ʱ��
		void InspectDeleteTimer();

		//��Ҫ���еĶ�ʱ��ID
		bool CallBackTimerRun();

	};

}

