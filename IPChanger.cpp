// IPChanger.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

int SelectNetCard();
int SelectPlace();
void SetUpConfig(WCHAR * nowNetName);

std::vector<WCHAR *> netNameVector;


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Korean"); //로케일설정

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

	printf("\n설정완료!");
	getch();

	netNameVector.clear();
	free(netInfo);
	

	return 0;  
}

int SelectNetCard()
{
	printf("네트워크 카드를 선택하세요!(무선 or 유선)\n");
	


	for(int i = 0; i<netNameVector.size(); i++)
	{
		printf("%d . ",i+1);
		wprintf(L"%s\n",netNameVector[i]);
	}
	printf("선택 : ");

	int selec;
	scanf("%d",&selec);

	if(selec > netNameVector.size() || selec <= 0)
	{
		system("cls");
		SelectNetCard();
	}
	else
	{
		return selec;
	}

}

int SelectPlace()
{
	system("cls");

	printf("현재 위치를 선택해주세요!\n");
	printf("1 . 전공실(유선/수동할당)\n");
	printf("2 . 매점  (유선/수동할당)\n");
	printf("3 . 작화실(유선/수동할당)\n");
	printf("4 . 작화실(무선/수동할당)\n");
	printf("5 . 전공실(무선/자동할당)\n");
	printf("6 . 랩실  (무선/자동할당)\n");
	printf("7 . 기타  (집, Egg사용)\n");

	printf("선택 : ");
	int selec=0;
	scanf("%d",&selec);

	if(selec <=0 || selec >4)
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
	system("cls");
	wprintf(L"현재 선택한 네트워크 카드 : %s",nowNetName);
	printf("\n계속하려면 엔터를 눌러주세요.");
	_getch();

	WCHAR command1[128];
	WCHAR command2[128];
	WCHAR command3[128];

	switch(SelectPlace())
	{
		system("cls");
	case 1:
	case 2:
	case 3:
		//번호 입력
		printf("\n학번을 입력해주세요 : ");
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
		//번호 입력
		printf("\n자리번호를 입력해주세요 : ");
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

