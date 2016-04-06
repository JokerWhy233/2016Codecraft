
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream> 
using namespace std;

#define INLINE  static __inline
#define PRINT   printf
#define MAX_RECORD  100
#define MAX_LINE_LEN 4000
#define Error( Str )   FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )
//�ڽӱ��ͼ��
//ǰ����Щ��ͼ�����ڽӱ�
typedef struct ListNode *Position;//λ��
typedef Position List;//����
typedef struct Tbl *Table;//�ڽӱ�
typedef int ElementType;
typedef int Vertex;//����
typedef struct VertexTable *VerTable;//����4������
#define Infinity 65536 //���������ʼ��
#define NotAVertex -1
#define nowstart 0//��ǰ��㣬��ʼ��Ϊ0
#define NotFound -1//�۰����
#define LeftChild(i) (2*(i)+1)//������							
typedef struct StackRecord *Stack;
#define EmptyTOS -1

int read_file(char ** const buff, const unsigned int spec, const char * const filename)
{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		PRINT("Fail to open file %s, %s.\n", filename, strerror(errno));
		return 0;
	}
	PRINT("Open file %s OK.\n", filename);

	char line[MAX_LINE_LEN + 2];
	unsigned int cnt = 0;
	while ((cnt < spec) && !feof(fp))
	{
		line[0] = 0;
		fgets(line, MAX_LINE_LEN + 2, fp);
		if (line[0] == 0)   continue;
		buff[cnt] = (char *)malloc(MAX_LINE_LEN + 2);
		strncpy(buff[cnt], line, MAX_LINE_LEN + 2 - 1);
		buff[cnt][4001] = 0;
		cnt++;
	}
	fclose(fp);
	PRINT("There are %d lines in file %s.\n", cnt, filename);
	return cnt;
}

struct StackRecord
{
	int Capacity;
	int TopOfStack;
	ElementType *Array;
};
////����ջ
Stack CreateStack(int MaxElements)
{
	Stack S;
	S = (struct StackRecord*)malloc(sizeof(struct StackRecord));
	if (S == NULL)
		FatalError("Out of space!!!");
	S->Array = (int*)malloc(sizeof(ElementType)*MaxElements);
	if (S->Array == NULL)
		FatalError("Out of space!!!");
	S->Capacity = MaxElements;
	S->TopOfStack = EmptyTOS;
	return S;
}
//��ջ
void Pop(Stack S)
{
	if (S->TopOfStack == EmptyTOS)
		Error("Empty Stack");
	else
		S->TopOfStack--;
}
//��ջ
void Push(ElementType X, Stack S)
{
	if (S->TopOfStack == S->Capacity - 1)
		Error("Full stack");
	else
		S->Array[++S->TopOfStack] = X;
}
//����ջ
void DisposeStack(Stack S)
{
	if (S != NULL)
	{
		free(S->Array);
		free(S);
	}
}
ElementType Top(Stack S)
{
	if (S->TopOfStack != EmptyTOS)
	{
		return S->Array[S->TopOfStack];
	}
	else
	{
		Error("Empty Stack");
		return 0;
	}
}


//����ÿ���洢�˶���Element��Ȩ��Cost��ָ����һ����Next��
struct ListNode
{
	ElementType ELement;
	int Cost;
	int Priority;
	int Edge;
	Position Next;
};//0->1,1   Element=1,cost=1;

  //�ڽӱ�
struct Tbl
{
	int TableSize;
	List *TheLists;
};

//�����
struct VertexTable
{
	int known;
	int Dist;
	Vertex Path;
};
//�����Ǹ��ڽӱ�ĳ��򣬲�����600�����ֱ������T->TheLists[0]��T->TheLists[599]
Table InitializeTable(int TableSize)
{
	Table T;
	int i;
	T = (struct Tbl*)malloc(sizeof(struct Tbl));
	if (T == NULL)
		FatalError("Out of space!!!");
	T->TableSize = TableSize;
	T->TheLists = (struct ListNode**)malloc(sizeof(List)*T->TableSize);
	for (i = 0;i<T->TableSize;i++)
	{
		T->TheLists[i] = (struct ListNode*)malloc(sizeof(struct ListNode));
		if (T->TheLists[i] == NULL)
			FatalError("Out of space!!!");
		else
			T->TheLists[i]->Next = NULL;
	}
	return T;
}
//Pos�����λ��,Key���յ�,Cost��Ȩ�أ����潲��
//Insert��������Ĳ���
void Insert(int Edge, ElementType Pos, ElementType Key, ElementType Cost, ElementType Priority, Table T)
{
	Position NewCell;
	List L;
	NewCell = (struct ListNode*)malloc(sizeof(struct ListNode));
	if (NewCell == NULL)
		FatalError("Out of space!!!");
	else
	{
		L = T->TheLists[Pos];
		NewCell->Next = L->Next;
		NewCell->Edge = Edge;
		NewCell->ELement = Key;
		NewCell->Cost = Cost;
		NewCell->Priority = Priority;
		L->Next = NewCell;
	}
}
//Start�����0��VerTable��ʼ��
VerTable InitialWeighted(Vertex Start, VerTable V, int NumVertex)//VΪ���㼯�ϣ�ÿ�����������ֱ��
{
	int i;
	V = (struct VertexTable*)malloc(sizeof(struct VertexTable)*NumVertex);
	for (i = 0;i<NumVertex;i++)
	{
		V[i].known = 0;
		V[i].Dist = 0;
		V[i].Path = NotAVertex;
	}
	V[Start].Dist = 0;
	return V;
}

//�Էֲ���
int BinarySearch(const ElementType A[], ElementType X, int N)
{
	int Low, Mid, High;
	Low = 0;High = N - 1;
	while (Low <= High)
	{
		Mid = (Low + High) / 2;
		if (A[Mid]<X)
			Low = Mid + 1;
		else
			if (A[Mid]>X)
				High = Mid - 1;
			else
				return Mid;
	}
	return NotFound;
}

void MakeEmpty(List L)
{
	Position P, Tmp;
	P = L->Next;
	L->Next = NULL;
	while (P != NULL)
	{
		Tmp = P->Next;
		free(P);
		P = Tmp;
	}
}
void Disposable(Table T)
{
	int i;
	for (i = 0;i<T->TableSize;i++)
	{
		MakeEmpty(T->TheLists[i]);
	}
	free(T->TheLists);
	free(T);
}

//�Զ��������һ������㷨
int find_start(VerTable V, Table T, ElementType demand[], int start_now, int known_p, int end, Stack S, int N)
{
	//����Ĳ����ֱ�Ϊ:�����konwn,dis,path�������ȵ㼯����ǰ�������
	//����ֵΪ��һ���������TempV���Լ���������ջ
	Position tmp;
	int min = Infinity;//��ͨ����СȨֵ
	int min_sp = Infinity;//���ȵ���СȨֵ
	int count_sp = 0;//���ȵ�����������ж������㣩
	int count_normal = 0;//��ͨ�����
	int normal[8] = { 0 };//��ͨ�����飨�ݴ棩
	int special[8] = { 0 };//���ȵ����飨�ݴ棩
	Vertex TempV = -1;//��ʼ���
	int flag = 0;//�յ���
				 //TMP = T->TheLists[start_now];
	tmp = T->TheLists[start_now]->Next;
	while (tmp != NULL) //0->3->1->NULL  �����ڽӵ���δ�����
	{
		if (V[tmp->ELement].known != 1) {
			if (tmp->Priority == 1)//����ö��������ȵ�
			{
				count_sp++;//��ǰ�㼶���ȵ���+1
				if (tmp->Cost < min_sp) //��ǰ��Ȩֵ��С
				{
					if (count_sp > 1)
					{
						special[count_sp - 2] = TempV;
						V[TempV].Dist = 0;
					}
					min_sp = tmp->Cost;//����min_sp					
					TempV = tmp->ELement;//����ֵ
					V[TempV].Dist = V[start_now].Dist + tmp->Cost;
				}
				else
				{
					if (count_sp > 1)
					{
						special[count_sp - 2] = tmp->ELement;;
					}
				}
			}
			else if (tmp->ELement == end)//����ö������յ�
			{
				flag = 1;//������һ�����յ㣬����ʱ��������
				V[end].Dist = V[start_now].Dist + tmp->Cost;
				V[end].Path = start_now;
			}
			else //����ö�������ͨ��
			{
				count_normal++;//��ͨ�����
				normal[count_normal - 1] = tmp->ELement;//��ͨ���ݴ�
				if (count_sp == 0 && tmp->Cost < min)//(1.����ͨ��û���յ�û�����ȵ�2.����ͨ�����յ�û�����ȵ�)
				{
					min = tmp->Cost;//����min
					V[TempV].Dist = 0;
					TempV = tmp->ELement;//����ֵ	
					V[TempV].Dist = V[start_now].Dist + tmp->Cost;
				}
			}
		}
		tmp = tmp->Next;
	}
	///////////*************************LOOK***************************///////////////
	if (count_sp == 0) //����û�����ȵ㣬��Ѷ������ͨ��ȫ����ջ
	{
		for (int i = 0;i < count_normal;i++)
		{
			if (TempV != normal[i])
			{
				Push(normal[i], S);Push(start_now, S);//V[normal[i]].Path = start_now;
			}
		}//��ͨ���֧��ջ		
	}
	else//�����ȵ���>=1ʱ
	{
		if (count_sp == 1)//����պ�ֻ��һ�����ȵ㣬��ô��ͨ����ջ������ֵpath
		{
			for (int i = 0;i < count_normal;i++) { Push(normal[i], S); Push(start_now, S); }//V[normal[i]].Path = start_now;
		}
		else//�����ȵ�������1ʱ����ͨ����ȫ����ջ��Ȼ���������ȵ���ջ
		{
			for (int i = 0;i < count_normal;i++) { Push(normal[i], S); Push(start_now, S); } //V[normal[i]].Path = start_now;
			for (int i = 0;i < count_sp - 1;i++) { Push(special[i], S);Push(start_now, S); }//V[special[i]].Path = start_now;
		}
	}
	if (flag == 1 && known_p == N)//�ѵ��յ�,����һ��û�����ȵ��ˣ���Ҫ�ж��Ƿ��Ѿ����������ȵ���ȫ����·���У�
	{
		TempV = 10000; //10000������־,�����յ������е����ȵ�����·����
	}
	if (TempV == -1) { TempV = -start_now; if (start_now == 0) { TempV = -1000; } }//����ֹͣ�㣨�����ţ����㴦��
	if (TempV != 10000 && TempV >= 0) { V[TempV].Path = start_now; }
	return TempV;//������һ�������
}

//��ʾ·��

void PrintPath(Vertex Ver, VerTable V, int C[])
{
	if (V[Ver].Path != NotAVertex)
	{
		PrintPath(V[Ver].Path, V, C);
		printf("->");
	}
	printf("%d", C[Ver]);
}
//�߱��
int find_route(int stop, Table T, VerTable V)
{
	Position Tmp;
	int result = -1;
	Tmp = T->TheLists[V[stop].Path]->Next;//
	while (Tmp != NULL)
	{
		if (Tmp->ELement == stop) { result = Tmp->Edge;break; }
		Tmp = Tmp->Next;
	}
	return result;
}
//�����㷨��D�㷨��չ
int DF(VerTable V, Table T, ElementType demand[], Stack S, int N, int start_now, int end, unsigned short *result)
{
	//Vertex Ver, W;//��������
	Position Tmp;
	//int end = 5;//�ٶ�һ���յ�
	int startone=start_now ;//�����
	int known_p = 0;//��¼�ѵ��������ȵ�
	int flag = 0;//���������ȵ���
	int min_dist = 10000;
	int ok = 1;
	for (;;)
	{
		if (start_now == 10000)//��ǰ���Ϊ-100�����ҳɹ�
		{
			flag++;//��������
			if (N < 16)
			{
				if (min_dist > V[end].Dist) {
					int tmp = end;
					int count = 0;
					while (tmp != startone)
					{
						result[count] = find_route(tmp, T, V);
						count++;
						ok = count;
						tmp = V[tmp].Path;
					}
					min_dist = V[end].Dist;
				}
				else
					V[end].Dist=min_dist;
				if (S->TopOfStack == -1) break;
				/////////////////////////////////////////////////////////////////////////
				int pass = Top(S);//������������� 0->1->3					
				Pop(S);//��ջ
				start_now = Top(S);//����ǰ����Ϊջ��Ԫ��		
				int stop = V[end].Path;//��������
				while (stop != pass)
				{
					V[stop].known = 0;
					if (BinarySearch(demand, stop, N) >= 0) { known_p--; }
					stop = V[stop].Path;
					//V[stop].Path = NotAVertex;
				}
				Pop(S);//��ջ	
				V[start_now].Path = pass;//��ջ���·����Ϣ
				Tmp = T->TheLists[pass]->Next;
				while (Tmp != NULL)
				{
					if (Tmp->ELement == start_now) { V[start_now].Dist = V[pass].Dist + Tmp->Cost; }
					Tmp = Tmp->Next;
				}
				/////////////////////////////////////////////////////////////////////////////
			}
			else
				break;
			if(flag>15) 
				break;
		}		
		if (start_now < 0 && S->TopOfStack == -1) { ok = 0;V[end].Dist = 0;break; }//ջ�գ��򷵻��޽�
		if (start_now <0)//��ͷ
		{
			int stop = -start_now;//ֹͣ��
			if (start_now == -1000) { stop = 0; }//0Ϊֹͣ������
			int pass = Top(S);//������������� 0->1->3					
			Pop(S);//��ջ
			printf("%d��ջ��\n", pass);
			start_now = Top(S);//����ǰ����Ϊջ��Ԫ��			
			while (stop != pass)
			{
				V[stop].known = 0;
				if (BinarySearch(demand, stop, N) >= 0) { known_p--; }
				printf("%d�س���\n", stop);
				stop = V[stop].Path;
				//V[stop].Path = NotAVertex;
			}
			Pop(S);//��ջ	
			printf("%d��ջ��\n", start_now);
			V[start_now].Path = pass;//��ջ���·����Ϣ
			Tmp = T->TheLists[pass]->Next;
			while (Tmp != NULL)
			{
				if (Tmp->ELement == start_now) { V[start_now].Dist = V[pass].Dist + Tmp->Cost; }
				Tmp = Tmp->Next;
			}
		}
		V[start_now].known = 1;//����ǰ�����Ϊ��֪
		if (BinarySearch(demand, start_now, N) >= 0) { known_p++; }//��¼��������ȵ���
		start_now = find_start(V, T, demand, start_now, known_p, end, S, N);//���Զ��巽������һ�����
		printf("��ǰ�㣺%d\n",start_now);
	}
	return ok;
}
//������
void PercDown(ElementType A[], int i, int N)
{
	int Child;
	ElementType Tmp;
	for (Tmp = A[i];LeftChild(i)<N;i = Child)
	{
		Child = LeftChild(i);
		if (Child != N - 1 && A[Child + 1]>A[Child])
			Child++;
		if (Tmp<A[Child])
			A[i] = A[Child];
		else
			break;
	}
	A[i] = Tmp;
}
void Swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
void Heapsort(ElementType A[], int N)
{
	int i;
	for (i = N / 2;i >= 0;i--)
		PercDown(A, i, N);
	for (i = N - 1;i>0;i--)
	{
		Swap(&A[0], &A[i]);
		PercDown(A, 0, i);
	}
}

void search_route(char *graph[5000], int edge_num, char *condition)//�ַ�������(topo)���������ַ�����demand��
{
	Table T = InitializeTable(600);//����
	VerTable V = NULL;
	Stack S = CreateStack(1000);//����ջ�����֧�㣩
	unsigned short result[100] = { 0 };//ʾ���е�һ���� 
	const int n = edge_num;
	int  N, start_now, end, demand[100] = { -1 }, test[261][4] = { 0 };
	char *save_ptr, *line;//��ʣ���
	for (int i = 0;i<n;i++)//test��ʼ��
	{
		char *hello = graph[i];
		char *p, *token;
		int j = 0;
		for (p = hello; ; p = NULL, j++)
		{
			token = strtok_s(p, ",", &save_ptr);
			if (NULL == token)   break;
			test[i][j] = atoi(token);
		}
	}
	start_now = atoi(strtok_s(condition, ",", &save_ptr));//���
	printf("start_now=%d \n", start_now);
	end = atoi(strtok_s(NULL, ",", &save_ptr));//�յ�
	printf("end=%d \n", end);
	line = strtok_s(NULL, ",", &save_ptr);//���ȵ��ַ���
	printf("special_line=%s \n", line);
	char *get, *token;
	save_ptr = NULL;
	int d_count = 0;
	for (get = line; ; get = NULL, d_count++)
	{
		token = strtok_s(get, "|", &save_ptr);
		if (NULL == token)   break;
		demand[d_count] = atoi(token);
		// printf("demand[%d]=%d \n",d_count,demand[d_count]);
	}
	N = d_count;
	int Sort[261 * 2] = { -1 }, Max, *C, p = 0, Stemp, Etemp, *Fdemand;
	for (int i = 0; i < n; i++)
	{
		Sort[2 * i] = test[i][1];
		Sort[2 * i + 1] = test[i][2];
	}//�������յ�����ĵ㶼�����
	Heapsort(Sort, n * 2);//�Զ����������
	Max = Sort[2 * n - 1];
	C = (int*)malloc(sizeof(int)*(Max + 1));
	if (NULL == C)
	{
		FatalError("Out of space!!!");
	}
	for (int i = 0;i<2 * n;i++)
	{
		if (Sort[i] != Sort[i + 1])
		{
			C[p] = Sort[i];
			p++;
		}
	}
	printf("edge_num=%d \n", n);
	printf("point_num=%d \n", p);
	printf("special_num=%d \n", N);
	Fdemand = (int*)malloc(sizeof(int)*(N));;
	for (int i = 0;i<N;i++)
	{
		Stemp = BinarySearch(C, demand[i], p);
		Fdemand[i] = Stemp;
	}
	Heapsort(Fdemand, N);
	for (int j = 0;j<n;j++)
	{
		Stemp = BinarySearch(C, test[j][1], p);
		Etemp = BinarySearch(C, test[j][2], p);
		if (BinarySearch(Fdemand, Etemp, N) >= 0)
		{
			Insert(test[j][0], Stemp, Etemp, test[j][3], 1, T);//start,stop,cost,List����3�ع������
			T->TheLists[Stemp]->ELement++;//�м������ȼ��ĵ�
		}
		else
		{
			Insert(test[j][0], Stemp, Etemp, test[j][3], 0, T);//start,stop,cost,List����3�ع������
			T->TheLists[Stemp]->ELement++;//�м������ȼ��ĵ�
		}
	}
	start_now = BinarySearch(C, start_now, p);
	end = BinarySearch(C, end, p);
	V = InitialWeighted(0, V, n);
	int route = DF(V, T, Fdemand, S, N, start_now, end, result);//���·��D�㷨		
	int tmp = end;
	int result_count = route;//��ĸ���
	if (route == 0) { printf("NA\n"); }
	else
	{
		if (N>=16) {
	    result_count =0;
		while (tmp != start_now)
		{
			result[result_count] = find_route(tmp, T, V);
			result_count++;
			tmp = V[tmp].Path;
		}
		}
		PrintPath(end, V, C);
		printf(" COST=%4d\n", V[end].Dist);
		for (int i = result_count - 1;i >= 0;i--)
		{
			printf("%d | ", result[i]);
			//record_result(result[i]);
		}
		printf("\n");
	}
	Disposable(T);
	DisposeStack(S);
	free(V);
	free(C);
	free(Fdemand);
	//record_result(result[i]);
}

int main()
{
	char *topo[5000];
	int edge_num;
	char *demand;
	int demand_num;
	edge_num = read_file(topo, 5000, "topo.csv");
	demand_num = read_file(&demand, 1, "demand.csv");
	search_route(topo, edge_num, demand);
	getchar();
	return 0;
}