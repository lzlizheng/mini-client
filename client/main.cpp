#include"client.h"

int main()
{
//======================================
//====FuncSCallProxy1��ʽ===============
//	char *pFuncName="add";
//	char *pContent="1";
//	long added=strlen(pContent);
//	void * pRet;

//	pRet=FuncSCallProxy1(pFuncName,2,'d',added,pContent,'s',8,"hello s");


//======================================
//====FuncSCallProxy3��ʽ===============
	int iret = 0;

	string senddata;
	string resdata;

	//���͡�hello server��
	iret = SendRequest("hello server", resdata);
	cout<<resdata<<endl;
	resdata = "";

	//��ָ���ļ��ж�ȡ��������
	iret = ReadSendingData(senddata);
	//�����ݷ��͸������������õ���������
	iret = SendRequest(senddata, resdata);

	cout<<resdata<<endl;

	return 0;
}
