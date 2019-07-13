#include "client.h"
#include<fstream>
#include<cassert>  //�쳣����

int ReadSendingData(string& sendbuf)
{
	ifstream infile;
	string file("callfunc.txt");

    infile.open(file.data());   //���ļ����������ļ��������� 
	bool is_open = infile.is_open();
    assert(is_open);   //��ʧ��,�����������Ϣ,����ֹ��������

	infile>>sendbuf;
//	cout<<sendbuf<<endl;

	infile.close();

	return 0;
}


int SendRequest(const string& sendbuf, string& resbuf)
{
	SOCKET soc;
	SOCKADDR_IN serveraddr;

	int conerror;

	WSADATA wsa;
	WSAStartup(MAKEWORD(1,1),&wsa);	//ͬserver

	if((soc = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP)) <= 0)	//ͬserver
	{
		printf("Create socket fail!\n");
	//	return -1;
	}

	//���÷�������ַ���˿ںŵ�
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(789);
	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");	//ָ������ַ


	//����
	cout<<"Try to connect..."<<endl;
	
	/*
	connect()���ڽ�����ָ��socket������
	��һ����������ʶһ��δ����socket
	�ڶ���������ָ��Ҫ�����׽��ֵ�sockaddr�ṹ���ָ��
	������������sockaddr�ṹ����ֽڳ���
	����ֵ�����޴���������connect()����0������Ļ�������SOCKET_ERROR����
	*/

	if((conerror=connect(soc,(SOCKADDR *)&serveraddr,sizeof(serveraddr))) != 0)
	{			
		cout<<"Connect fail:"<<endl;
		cout<<WSAGetLastError()<<endl;
		//����ʧ��
		return -1;
	}

	cout<<"Connected!"<<endl;



	//������Ϣ
	//����hello server!,���Է������Ƿ���������
	if(send(soc, "hello server", 12, 0) <= 0)
	{
		cout<<"send Error!"<<endl;
	}
	else
	{
		cout<<"send hello world!"<<endl;
	}

	//ȷ�Ϸ������ظ�����Ϣ
	char buf[64] = {0};
	int buflen = recv(soc, buf, 9, 0);
	string strRecvServerOK(buf);

	if(buflen < 0)
	{
		//����ʧ��
		return -2;
	}

	if (strRecvServerOK != "server ok")
	{
		cout<<"strRecvServerOK fail"<<endl;
		return -3;
	}
	else
	{
		cout<<strRecvServerOK<<endl;
	}

	//�ͻ��˷����û��������룬������������Ȩ
	string name_password("lz 123");
	if(send(soc,(const char *)name_password.c_str(), name_password.size(), 0) <= 0)
	{
		cout<<"send Error!"<<endl;
	}
	else
	{
		cout<<"send  "<<name_password<<endl;
	}

	//�鿴�������Ƿ񷵻�server ok
	buflen = recv(soc, buf, 9, 0);
	strRecvServerOK = buf;
	
	if(buflen < 0)
	{
		//����ʧ��
		return -2;
	}

	if (strRecvServerOK != "server ok")
	{
		cout<<"strRecvServerOK fail"<<endl;
		return -3;
	}
	else
	{
		cout<<strRecvServerOK<<endl;
	}

	//������������
	if(send(soc,(const char *)sendbuf.c_str(), sendbuf.size(), 0) <= 0)
	{
		cout<<"send Error!"<<endl;
	}
	else
	{
		cout<<"send  "<<name_password<<endl;
	}

	//������Ϣ�����϶�ȡЭ����ջ�����
	memset(buf, 0, sizeof(buf));
	int rs = 1;
	while(rs)
	{
		buflen = recv(soc, buf, sizeof(buf), 0);
		resbuf += buf;
		if(buflen < 0)
		{
			// �����Ƿ�������ģʽ,���Ե�buflenΪEAGAINʱ,��ʾ��ǰ�������������ݿɶ�
			// ������͵����Ǹô��¼��Ѵ���
			if(buflen == EAGAIN)
				break;
			else
			{
				//����ʧ��
				return -2;
			}
		}
		else if(buflen == 0)
		{
			// �����ʾ�Զ˵�socket�������ر�.
			break;
		}
		if(buflen != sizeof(buf))
			rs = 0;
		else
		{
			memset(buf, 0, sizeof(buf));
			rs = 1;// ��Ҫ�ٴζ�ȡ
		}
	}

	//�ͷ��׽���������
	closesocket(soc);

	//�ر��첽�׽���
	WSACleanup();

	return 0;
}


//void * FuncSCallProxy1(char * pFuncName,int ParaNum,...)
//{
//	SOCKET soc;
//	SOCKADDR_IN serveraddr;
//
//	//��ȡ��������
//	va_list arg_ptr; //����һ��ָ������б�ı���
//	va_start(arg_ptr,ParaNum); //��ָ������б�ı�����ʼ��
//
//	int conerror;
//
//	WSADATA wsa;
//	WSAStartup(MAKEWORD(1,1),&wsa);	//ͬserver
//
//	if((soc = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP)) <= 0)	//ͬserver
//	{
//		printf("Create socket fail!\n");
//	//	return -1;
//	}
//
//	//���÷�������ַ���˿ںŵ�
//	serveraddr.sin_family = AF_INET;
//	serveraddr.sin_port = htons(789);
//	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");	//ָ������ַ
//
//
//	//����
//	cout<<"Try to connect..."<<endl;
//	
//	/*
//	connect()���ڽ�����ָ��socket������
//	��һ����������ʶһ��δ����socket
//	�ڶ���������ָ��Ҫ�����׽��ֵ�sockaddr�ṹ���ָ��
//	������������sockaddr�ṹ����ֽڳ���
//	����ֵ�����޴���������connect()����0������Ļ�������SOCKET_ERROR����
//	*/
//
//	if((conerror=connect(soc,(SOCKADDR *)&serveraddr,sizeof(serveraddr))) != 0)
//	{			
//		cout<<"Connect fail:"<<endl;
//		cout<<WSAGetLastError()<<endl;
////		return -1;
//	}
//
//	cout<<"Connected!"<<endl;
//
//
//	char buf[256] = {0};
//	//������Ϣ
//	sprintf(buf,"%s %d",pFuncName,ParaNum);
//	for(int i=0;i<ParaNum;i++)
//	{
//		char buf2[1024]={0};
//		char Type=va_arg(arg_ptr,char);		//��ȡ��һ��char�͵Ĳ���
//		long Lenth=va_arg(arg_ptr,long);	//��ȡ��һ��long�͵Ĳ���
//		char * temp=va_arg(arg_ptr,char *);	//��ȡ��һ��char *�͵Ĳ���
//
//		strcpy(buf2,buf);
//		sprintf(buf,"%s %c %ld %s",buf2,Type,Lenth,temp);
//	}
//
//	if(send(soc,(const char *)buf,strlen((const char *)buf)+1, 0) <= 0)
//	{
//		cout<<"send Error!"<<endl;
//	}
//
//
//	//������Ϣ
//	if(recv(soc, buf, 20, 0) <= 0)
//	{
//		cout<<"recive error"<<endl;
//	}
//	cout<<buf<<endl;
//
//
//
//	//�ͷ��׽���������
//	closesocket(soc);
//
//	//�ر��첽�׽���
//	WSACleanup();
//
//	//return buf;
//	return NULL;
//}
