#include "TimerManager.h"

using namespace CTimerManagerSpace;

CTimerManager::CTimerManager()
{

}


CTimerManager::~CTimerManager()
{
	//������еĶ�ʱ��
	ClearTimer();
}

//��ʱ���߳�
unsigned int __stdcall CTimerManagerSpace::CTimerManager::TimerRunThread(PVOID pThis)
{
	CTimerManager* pTimerManager = (CTimerManager*)pThis;
	if (pThis == NULL)return 0;

	while (true)
	{
		//ɾ����Ҫɾ���Ķ�ʱ��
		pTimerManager->InspectDeleteTimer();

		//�鿴��һ����ʱ����Ҫ������
		if (!pTimerManager->CallBackTimerRun())
			_endthreadex(0);
		
		//��һ��
		Sleep(20);
	}
}

//���һ����ʱ��
//#TimerID ��ʱ����ID
//#TimerMs ��ʱ���ļ��
void CTimerManagerSpace::CTimerManager::AddTimer(int TimerID, unsigned int TimerMs)
{
	//��ʱ���������Ϊ0
	if (TimerMs <= NULL)return;

	//��ȡ��ǰ��ʱ��
	int NowTime = ::GetTickCount();

	//��ʱ����Ϣ��д
	TimerManagerInfo TimerInfoTemp ;
	TimerInfoTemp.timerDelete = false;
	TimerInfoTemp.timerID = TimerID;
	TimerInfoTemp.timerCreateTime = NowTime;
	TimerInfoTemp.timerLastRun = NowTime;
	TimerInfoTemp.timerInterval = TimerMs;

	//���붨ʱ���б�
	m_TimerList.push_back(TimerInfoTemp);

	//��һ����ʱ���Ļ��Ϳ���һ����ʱ���̣߳�����thisָ��
	if (m_TimerList.size() == 1)
	{
		_beginthreadex(NULL, NULL, TimerRunThread, this, NULL, NULL);
	}

}

//ɾ��һ����ʱ��
//#TimerID ��ʱ��ID
void CTimerManagerSpace::CTimerManager::DeleteTimer(int TimerID)
{
	for (TimerManagerInfoList::iterator it = m_TimerList.begin();
		it != m_TimerList.end();it++)
	{
		if (it->timerID == TimerID)
		{
			//��������ɾ����־λ
			it->timerDelete = true;
			break;
		}
	}
}

//������ж�ʱ��
void CTimerManagerSpace::CTimerManager::ClearTimer()
{
	for (TimerManagerInfoList::iterator it = m_TimerList.begin();
		it != m_TimerList.end(); it++)
	{
		//ȫ���Ķ�ʱ����ɾ����־λ������Ϊ1
		it->timerDelete = true;
	}
}

//���ɾ����ʱ��
void CTimerManagerSpace::CTimerManager::InspectDeleteTimer()
{
	for (TimerManagerInfoList::iterator it = m_TimerList.begin(); it != m_TimerList.end();)
	{
		if (it->timerDelete)
		{
			//erase�����������������Ҫ������...
			m_TimerList.erase(it);
			it = m_TimerList.begin();
			continue;
		}
		++it;
	}
}

//��Ҫ���еĶ�ʱ��ID
bool CTimerManagerSpace::CTimerManager::CallBackTimerRun()
{
	if (m_TimerList.size() == NULL)return false;

	int NowTimer = ::GetTickCount();

	for (TimerManagerInfoList::iterator it = m_TimerList.begin();
		it != m_TimerList.end(); it++)
	{
		if (NowTimer - it->timerLastRun >= it->timerInterval)
		{
			//���ڵ�ʱ���ȥ��һ�����е�ʱ��õ�һ��ʱ����
			//������ʱ�������ڶ�ʱ�����м��
			//�����ӵĻ������������ʱ��
			it->timerLastRun = NowTimer;
			if (OnTimer(it->timerID) == NULL)
			{
				//���ؿվ�ɾ�������ʱ��
				it->timerDelete = true;
			}
			continue;
		}
	}
	return true;
}

