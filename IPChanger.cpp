// IPChanger.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

	printf("\n설정완료!(종료하실려면 엔터를 눌러주세요.)");
	getch();

	netNameVector.clear();
	free(netInfo);
	

	return 0;  
}

void ScreenClear();

int SelectNetCard()
{
	ScreenClear();

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

	printf("현재 위치를 선택해주세요!\n");
	placeNameVector.push_back(L"1 . 전공실(유선/수동할당)\n");
	placeNameVector.push_back(L"2 . 매점  (유선/수동할당)\n");
	placeNameVector.push_back(L"3 . 작화실(유선/수동할당)\n");
	placeNameVector.push_back(L"4 . 작화실(무선/수동할당)\n");
	placeNameVector.push_back(L"5 . 전공실(무선/자동할당)\n");
	placeNameVector.push_back(L"6 . 랩실  (무선/자동할당)\n");
	placeNameVector.push_back(L"7 . 기타  (집, Egg사용)\n");
	for(int i =0; i<placeNameVector.size(); i++)
	{
		wprintf(L"%s",placeNameVector[i]);
	}

	printf("선택 : ");
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
	wprintf(L"현재 선택한 네트워크 카드 : %s",nowNetName);
	printf("\n계속하려면 엔터를 눌러주세요.");
	_getch();

	WCHAR command1[128];
	WCHAR command2[128];
	WCHAR command3[128];

	int selec = SelectPlace();

	ScreenClear();
	wprintf(L"현재 선택한 장소 : %s",placeNameVector[selec-1]);
	printf("\n계속하려면 엔터를 눌러주세요.");
	_getch();

	switch(selec)
	{
	ScreenClear();
	case 1:
	case 2:
	case 3:
		//번호 입력
		printf("\n번호를 입력해주세요(현재 3학년 게임과만 지원) : ");
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
		printf("\n자리번호를 입력해주세요(현재 3학년 작화실만 지원) : ");
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

	wprintf(L"->애니원 네트워크 도우미 %s<-\n",VER);
	printf("->제작자 : 이강희(rkdgml6332@gmail.com)<-\n\n");
}

