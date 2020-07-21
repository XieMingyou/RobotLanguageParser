/*Function:�������������������Խ����������ƻ������˶�*/
# include<stdio.h>
# include<stdlib.h>
# include"lexicon.h"
# include"syntax.h"
# include"semantics.h"
# include"interpreter.h"
# include"robotInstruction.h"

int main()
{
	rootHead = (AST *)malloc(sizeof(AST));//������ͷָ��
	rootTail = rootHead;//������βָ��
	AST initial = { 0 };
	*rootHead = initial;
	varProgName = "program1";
	projName = "project1";
	varProgQueue.push(varProgName);
	if (!varProgCompile())
	{
		mainProgRoot = rootHead->next;
		//��ʼ���������˶�����������
		robotInitial();
		//��ʼִ�г���
		progRun(mainProgRoot); //rootTail����������ĸ����
		//�ͷ����н��������ڴ棬��ֹ�ڴ�й©
		while (rootHead != NULL)
		{
			AST *temp = rootHead->next;
			deleteAst(rootHead);
			rootHead = temp;
		}
	}
	else
	{
		printf("���벻ͨ����");
		return -1;
	}
	return 0;
}