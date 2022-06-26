#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <Windows.h>
#define INT_MAX 999999
#define MaxNum 344
/*
����վ����Ϣ
the structure of subway station
*/
typedef struct {
	int num;
	char name[16];    //��վ���� 
	int IsTransferStation;   //�Ƿ��ǻ��˳�վ
	int SubwayNum[3];   //��վ������·��
}station;


typedef struct {
	char name[16];
	int num;
}name_search_stationnum;
/*
ȫ�ֱ��� 
*/
station s[MaxNum];   //��ų�վ��Ϣ
char subway_name[31][16] = { "�׶�������","1����","2����" ,"3����" ,"4����" ,"5����" ,"6����" ,"7����" ,"8���߱���","9����","10����","11����","12����",
							"13����","14���߶���","15����","16����","17����","18����","19����","������","��ƽ��","��ͨ��","��ׯ��","������","���˻�����",
	                        "�෿��","��ɽ��","S1��","8�����϶�","14��������" };
name_search_stationnum r[MaxNum];
int AdjMatrix[MaxNum][MaxNum];   //�ڽӾ��� 
int D[MaxNum][MaxNum];    //��ʱ���� 
int pre[MaxNum][MaxNum];    

/*
�����ļ���ʼ������վ����Ϣ
Initialize metro station information with file
*/
void InitStation(FILE* p) {
	int num=0, way1 = -1, way2 = -1, way3 = -1;   //-1��ʾ��
	char name[16]="";
	while (!feof(p)) {
		fscanf(p, "%d %s %d,%d,%d", &num, name, &way1, &way2, &way3);
	    //printf( "%d %s %d,%d,%d\n", num, name, way1, way2, way3);   //����¼����

		s[num - 1].num = num;
		strcpy(s[num - 1].name, name);
		s[num - 1].SubwayNum[0] = way1;
		s[num - 1].SubwayNum[1] = way2;
		s[num - 1].SubwayNum[2] = way3;
		s[num - 1].IsTransferStation = way2 > 0 ? 1 : 0;   //�Ƿ�Ϊ���˳�վ
		way1 = way2 = way3 = -1;   //����
		strcpy(r[num].name, name);	//0�ŵ�Ԫ�����ڱ��� 
		r[num].num = num;
	}
}    //ʹ���ļ���ʼ����վ��Ϣ
/*
�����ļ������ڽӾ������ڴ洢��ͼ��Ϣ 
Using files to construct adjacency matrix for storing map information
*/
void MakeMatrix(FILE* p) {
	int i, j, value;
	for (i = 0;i < MaxNum;i++)
		for (j = 0;j < MaxNum;j++) {
			if (i == j) {
				AdjMatrix[i][j] = 0;
				D[i][j] = 0;
				pre[i][j] = j;
			}
			else {
				AdjMatrix[i][j] = INT_MAX;
				D[i][j] = INT_MAX;
				pre[i][j] = j;
			}
		}           //��ʼ���ڽӾ����D����,��������pre

	while (!feof(p)) {
		fscanf(p, "%d %d %d", &i, &value, &j);
		AdjMatrix[i - 1][j - 1] = value;
		AdjMatrix[j - 1][i - 1] = value;
		D[i - 1][j - 1] = value;
		D[j - 1][i - 1] = value;
	}
	//ͨ���ļ������ڽӾ���ͳ�ʼ��D����
}

/*
�������·�� 
compute the shortest path
*/
void ShortestPath() {
	int i, j, k;
	for (k = 0;k < MaxNum;k++)
		for (i = 0;i < MaxNum;i++)
			for (j = 0;j < MaxNum;j++) {
				if (D[i][j] > D[i][k] + D[k][j]) {
					D[i][j] = D[i][k] + D[k][j];
					pre[i][j] = pre[i][k];
				}    //����k���i��j�ľ����С���ʸ�����̾��룬����¼������һ����
			}
}

int Partition(int low, int high) {
	r[0] = r[low];
	char pivotloc[16];
	strcpy(pivotloc, r[low].name);
	while (low < high) {
		while (low < high && strcmp(r[high].name, pivotloc) >= 0) --high;
		r[low] = r[high];
		while (low < high && strcmp(r[low].name, pivotloc) <= 0) ++low;
		r[high] = r[low];
	}
	r[low] = r[0];
	return low;
}

void QSort(int low, int high) {
	int t;
	if (low < high) {
		t = Partition(low, high);
		QSort(low, t - 1);
		QSort(t + 1, high);
	}
}
/*
�ж���һ��վ���뵱ǰվ���Ƿ���һ��·�� 
Determine whether the previous site and the current site are the same route
*/
int sameway(int pre_stationnum, int now_stationnum, station pre, station now) {
	int i, j;

	for (i = 0;i < 3;i++)
		for (j = 0;j < 3;j++)
			if (pre.SubwayNum[i] >= 0 && now.SubwayNum[j] >= 0 && pre.SubwayNum[i] == now.SubwayNum[j])
				return pre.SubwayNum[i];
	return -1;

}

void QuickSort() {
	QSort(1, MaxNum);
}

int Search(char key[]) {
	int low = 1;
	int high = MaxNum;
	int mid;
	while (low <= high) {
		mid = (low + high) / 2;
		if (strcmp(key, r[mid].name) == 0)	return r[mid].num;
		else if (strcmp(key, r[mid].name) <= 0)	high = mid - 1;
		else low = mid + 1;
	}
	return 0;
}
int StationCounter(int start,int end) {
	int count = 1;
	int i = start-1;
	while(1){
		i = pre[i][end-1];
		if (i == end-1) break;
		count++;
	}
	return count;
}   //���������յ�֮���վ����Ŀ
/*�޸�pre_way��ʼ�� �޸�pre_stationnum��ʼ��3 �޸��յ�վ��·���*/
/*
����Ƽ�·����Ϣ�����˴���������Ʊ�� 
Output recommended route information, transfer times and required fares
*/
void Print(char startname[], char endname[]) {
	int startnum = Search(startname);
	int endnum = Search(endname);

	printf("\n         ");

	if (startnum == 0) {
		printf("��������վ������\n");
		return;
	}
	if (endnum == 0) {
		printf("������յ�վ������\n");
		return;
	}
	station start = s[startnum - 1];
	station end = s[endnum - 1];


	printf("�϶�·�� ��%s", start.name);							//������

	int pre_way = start.SubwayNum[0];
	int pre_stationnum = start.num;
	int now_stationnum = pre[start.num - 1][end.num - 1] + 1;
	int way_start = start.num;
	station pre_station, now_station;
    int Tcount=0;
	while (1) {
		pre_station = s[pre_stationnum - 1];
		now_station = s[now_stationnum - 1];
		if (pre_station.IsTransferStation == 1) {
			int way = sameway(pre_stationnum, now_stationnum, pre_station, now_station);//��ͬ·�� 
			if (way != pre_way && way >= 0) {
				if (pre_stationnum != startnum) {
					printf("  --%s(%dվ)--->  %s", subway_name[pre_way], StationCounter(pre_stationnum, way_start), pre_station.name);
					way_start = pre_stationnum;
					Tcount++;
				}	
				pre_way = way;
			}
		}

		if (now_stationnum == end.num)	break;

		pre_stationnum = now_stationnum;
		now_stationnum = pre[pre_stationnum - 1][end.num - 1] + 1;


	}

	printf("  --%s(%dվ)--->  %s\n", subway_name[pre_way],StationCounter(end.num, way_start), end.name);

	printf("         ����Ҫ %d Ԫ         ������%d��\n", price(startnum - 1, endnum - 1),Tcount);
}
void resetP(station* platform) {
	platform->IsTransferStation = 0;
	platform->num = 0;
	platform->SubwayNum[0] = -1;
	platform->SubwayNum[1] = -1;
	platform->SubwayNum[2] = -1;
}
/*
������˴������ٵ�·�� 
Output the route with the least transfer times
*/ 
void PrintLeastTransfer(char startname[], char endname[]) {

	station PathPoint[20];
	int PointLocation = 0;
	for (int i = 0;i < 10;i++) {
		PathPoint[i].IsTransferStation = 0;
		PathPoint[i].num = 0;
		PathPoint[i].SubwayNum[0] = -1;
		PathPoint[i].SubwayNum[1] = -1;
		PathPoint[i].SubwayNum[2] = -1;
	}    //��·����¼��ʼ�� 
	int startnum = Search(startname);
	int endnum = Search(endname);

	printf("\n         ");

	if (startnum == 0) {
		printf("��������վ������\n");
		return;
	}
	if (endnum == 0) {
		printf("������յ�վ������\n");
		return;
	}

	station start = s[startnum - 1];
	station end = s[endnum - 1];
	PathPoint[PointLocation] = start;
	PointLocation++;
	int pre_way = start.SubwayNum[0];
	int pre_stationnum = start.num;
	int now_stationnum = pre[start.num - 1][end.num - 1] + 1;
    int Tcount=-1;

	station pre_station, now_station;

	while (1) {
		pre_station = s[pre_stationnum - 1];
		now_station = s[now_stationnum - 1];

		if (pre_station.IsTransferStation == 1) {
			int way = sameway(pre_stationnum, now_stationnum, pre_station, now_station);//��ͬ·�� 
			if (way != pre_way && way >= 0) {
				if (pre_stationnum != startnum)
				{
					PathPoint[PointLocation] = pre_station;
					PointLocation++;
				}
				pre_way = way;
			}
		}

		if (now_stationnum == end.num)	break;

		pre_stationnum = now_stationnum;
		now_stationnum = pre[pre_stationnum - 1][end.num - 1] + 1;

	}
	PathPoint[PointLocation] = end;  //����㵽�յ��֮������л���վ�����¼�� 
	//printf("--%d����--->%s",pre_way,end.name);
	for (int i = 0;i < PointLocation;i++) {
		for (int j = PointLocation;j >= i + 2;j--) {
			if (sameway(0, 0, PathPoint[i], PathPoint[j]) != -1) {
				for (int n = i + 1;n < j;n++) {
					if (n == 0 || n == PointLocation) continue; //�޷�ȥ�������յ� 
					resetP(&PathPoint[n]);
				}   //����ȥ������Ļ���վ,
				i = j;
				break;
			}
		}
	}    //�������2ȥ������Ļ���վ

	for (int i = 0;i < PointLocation - 1;i++) {
		if (!PathPoint[i].num) continue;
		for (int j = PointLocation;j > i + 1;j--) {
			if (sameway(0, 0, PathPoint[i], PathPoint[j]) != -1) continue;  //��������ͬһ����������Ҫ��ӵ� 
			for (int n = 0;n < MaxNum;n++) {
				if (s[n].IsTransferStation) {
					if (sameway(0, 0, s[n], PathPoint[i]) != -1 && sameway(0, 0, s[n], PathPoint[j]) != -1) {
						if (D[PathPoint[i].num - 1][s[n].num - 1] + D[PathPoint[j].num - 1][s[n].num - 1] <= D[PathPoint[i].num - 1][PathPoint[j].num - 1] + (3000) * (j - i - 2)) {
							PathPoint[i + 1] = s[n];
							for (int s = i + 2;s < j;s++) {
								resetP(&PathPoint[s]);
							}
						}
					}
				}
			}
		}
		//�ڳ�վ˳�����Ѱ���滻���˳�վ 
	}   //�������1 ��һ������վ�����������໻��վ 
	station pre, now;
	pre = start;
	pre_way = -1;
	printf("\n         ���ٻ��ˣ�");
	for (int i = 1;i <= PointLocation;i++) {
		if (PathPoint[i].num) {
			now = PathPoint[i];
			if (pre_way == sameway(0, 0, pre, now)) continue;
			printf("%s  ---%s--->  ", pre.name, subway_name[sameway(0, 0, pre, now)], StationCounter(pre.num, now.num));
			pre_way = sameway(0, 0, pre, now);
			Tcount++;
			pre = now;
		}
		else continue;
	}
	printf("%s\n", end.name);   //����Ľ���· 
	printf("         ����Ҫ %d Ԫ         ������%d��\n", price(startnum - 1, endnum - 1),Tcount);
}  //��ӡ���ٻ��˵�·�� 
/*
����Ʊ�� 
compute ticket price 
*/ 
int price(int start, int end) {
	int d = D[start][end];
	//printf("%d\n",d);
	if (d <= 6000)	return 3;
	else if (d <= 12000)	return 4;
	else if (d <= 22000)	return 5;
	else if (d <= 32000)	return 6;
	else	return (7+ (d - 32000) / 20000);
}
/*
ϵͳ���� 
the system interface 
*/ 
void InitInterface() {
	//system("cls"); 
	printf("        ------------------------------------------\n");
	printf("\n");
	printf("                 ��ӭʹ�ñ���������ѯϵͳ         \n");
	printf("\n");
	printf("        ------------------------------------------\n");
	printf("         ϵͳ���� ��\n");
	printf("\n");
	printf("          1���鿴��������\n");
	printf("          2����ѯ����·��\n");
	printf("\n");
	printf("          0���˳�����ϵͳ\n");
	printf("\n");
	printf("        ------------------------------------------\n");
	printf("\n");
}

void ShowMap(){
	ShellExecute(NULL,"open","https://map.bjsubway.com/",NULL,NULL,SW_HIDE);  //�򿪱��������Ĺٷ���ַ 
}

void start() {

	while (1) {

		InitInterface();
		int select;
		printf("         ��������Ӧ�������֣� ");
		scanf("%d",&select);

		switch (select) {
			case 1: {
				//�������·�� 
				ShowMap();
				printf("\n");
				printf("         ��ͼ����ҳ���д� ~ \n");
				printf("\n\n         ------------------------------------------");
				printf("\n         ");
				fflush(stdin);  //������̻����������� 
				system("pause");
				system("cls");
				break;
				;
			}
	
			case 2: {
				printf("\n");
				char s1[16], s2[16];
				printf("         ���������վ���յ�վ��");
				scanf("%s %s", s1, s2);
				Print(s1,s2);
				PrintLeastTransfer(s1, s2);
				printf("\n\n         ------------------------------------------");
				printf("\n         ");
				fflush(stdin);
				system("pause");
				system("cls");
				break;
				;
			}
	
			case 0: {
				printf("\n\n         ");
				printf("              ��ӭ�´�ʹ��  ");
				printf("\n\n         ------------------------------------------");
				printf("\n\n\n         ");
				system("pause");
				printf("\n\n\n");
				return;
				break;
			}
	
			default: {
				printf("\n\n         ");
				printf("������������������ ");
				printf("\n\n         ------------------------------------------");
				printf("\n\n         ");
				fflush(stdin);
				system("pause");
				system("cls");
				break;
			}
		}
	}


	return;
}


int main() {
	FILE* p = fopen("station.txt", "r");
	FILE* g = fopen("distance.txt", "r");
	InitStation(p);
	fclose(p);
	MakeMatrix(g);
	fclose(g);
	ShortestPath();
	QuickSort();
	start();
	return 0;
}
