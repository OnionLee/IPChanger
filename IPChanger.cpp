// IPChanger.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#define VER L"1.0"

int SelectNetCard();
int SelectPlace();
void SetUpConfig(WCHAR * nowNetName);

std::vector<WCHAR *> netNameVector;
std::vector<WCHAR *> placeNameVector;


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Korean"); //�����ϼ���

	IP_ADAPTER_ADDRESSES * netInfo;
	ULONG buf = 1500000;
	netInfo = (IP_ADAPTER_ADDRESSES *) malloc(buf);
	GetAdaptersAddresses(AF_INET,GAA_FLAG_INCLUDE_GATEWAYS,NULL,netInfo,&buf);

	WCHAR * nowNetName;

	IP_ADAPTER_ADDRESSES * p = netInfo;
	while(p)
	{
		netNameVector.push_back(p->FriendlyName);
		p = p->Next;
	}

	nowNetName = netNameVector[SelectNetCard() - 1];

	SetUpConfig(nowNetName);

	printf("\n�����Ϸ�!(�����ϽǷ��� ���͸� �����ּ���.)");
	getch();

	netNameVector.clear();
	free(netInfo);
	

	return 0;  
}

void ScreenClear();

int SelectNetCard()
{
	ScreenClear();

	printf("��Ʈ��ũ ī�带 �����ϼ���!(���� or ����)\n");
	


	for(int i = 0; i<netNameVector.size(); i++)
	{
		printf("%d . ",i+1);
		wprintf(L"%s\n",netNameVector[i]);
	}
	printf("���� : ");

	int selec;
	scanf("%d",&selec);

	if(selec > netNameVector.size() || selec <= 0)
	{
		SelectNetCard();
	}
	else
	{
		return selec;
	}

}

int SelectPlace()
{
	ScreenClear();

	printf("���� ��ġ�� �������ּ���!\n");
	placeNameVector.push_back(L"1 . ������(����/�����Ҵ�)\n");
	placeNameVector.push_back(L"2 . ����  (����/�����Ҵ�)\n");
	placeNameVector.push_back(L"3 . ��ȭ��(����/�����Ҵ�)\n");
	placeNameVector.push_back(L"4 . ��ȭ��(����/�����Ҵ�)\n");
	placeNameVector.push_back(L"5 . ������(����/�ڵ��Ҵ�)\n");
	placeNameVector.push_back(L"6 . ����  (����/�ڵ��Ҵ�)\n");
	placeNameVector.push_back(L"7 . ��Ÿ  (��, Egg���)\n");
	for(int i =0; i<placeNameVector.size(); i++)
	{
		wprintf(L"%s",placeNameVector[i]);
	}

	printf("���� : ");
	int selec=0;
	scanf("%d",&selec);

	if(selec <=0 || selec >placeNameVector.size())
	{
		SelectPlace();
	}
	else
	{
		return selec;
	}
}

void SetUpConfig(WCHAR * nowNetName)
{
	ScreenClear();
	wprintf(L"���� ������ ��Ʈ��ũ ī�� : %s",nowNetName);
	printf("\n����Ϸ��� ���͸� �����ּ���.");
	_getch();

	WCHAR command1[128];
	WCHAR command2[128];
	WCHAR command3[128];

	int selec = SelectPlace();

	ScreenClear();
	wprintf(L"���� ������ ��� : %s",placeNameVector[selec-1]);
	printf("\n����Ϸ��� ���͸� �����ּ���.");
	_getch();

	switch(selec)
	{
	ScreenClear();
	case 1:
	case 2:
	case 3:
		//��ȣ �Է�
		printf("\n��ȣ�� �Է����ּ���(���� 3�г� ���Ӱ��� ����) : ");
		int number1;
		scanf("%d",&number1);
		wsprintf(command1,L"interface ip set address name=\"%s\" source=static address=\"172.216.19.%d\" mask=\"255.255.248.0\" gateway=\"172.216.16.1\"",nowNetName,number1 + 174);
		wsprintf(command2,L"interface ip set dns name=\"%s\" source=static address=\"168.126.63.1\" primary",nowNetName);
		wsprintf(command3,L"interface ip add dns name=\"%s\" source=static address=\"203.248.252.2\" index=2",nowNetName);
		ShellExecuteW( NULL , L"runas" , L"netsh.exe" , command1, NULL, SW_SHOW );
		ShellExecuteW( NULL , L"runas" , L"netsh.exe" , command2, NULL, SW_SHOW );
		ShellExecuteW( NULL , L"runas" , L"netsh.exe" , command3, NULL, SW_SHOW );
		break;
	case 4:
		//��ȣ �Է�
		printf("\n�ڸ���ȣ�� �Է����ּ���(���� 3�г� ��ȭ�Ǹ� ����) : ");
		int number2;
		scanf("%d",&number2);
		wsprintf(command1,L"interface ip set address name=\"%s\" source=static address=\"172.216.21.%d\" mask=\"255.255.248.0\" gateway=\"172.216.16.1\"",nowNetName,number2);
		wsprintf(command2,L"interface ip set dns name=\"%s\" source=static address=\"168.126.63.1\" primary",nowNetName);
		wsprintf(command3,L"interface ip add dns name=\"%s\" source=static address=\"203.248.252.2\" index=2",nowNetName);
		ShellExecuteW( NULL , L"runas" , L"netsh.exe" , command1, NULL, SW_SHOW );
		ShellExecuteW( NULL , L"runas" , L"netsh.exe" , command2, NULL, SW_SHOW );
		ShellExecuteW( NULL , L"runas" , L"netsh.exe" , command3, NULL, SW_SHOW );
		break;
	case 5:
	case 6:
	case 7:
		wsprintf(command1,L"interface ip set address name=\"%s\" source=dhcp",nowNetName);
		wsprintf(command2,L"interface ip set dns name=\"%s\" source=dhcp",nowNetName);
		ShellExecuteW( NULL , L"runas" , L"netsh.exe" , command1, NULL, SW_SHOW );
		ShellExecuteW( NULL , L"runas" , L"netsh.exe" , command2, NULL, SW_SHOW );
		break;
	}


}

void ScreenClear()
{
	system("cls");

	wprintf(L"->�ִϿ� ��Ʈ��ũ ����� %s<-\n",VER);
	printf("->������ : �̰���(rkdgml6332@gmail.com)<-\n\n");
}

