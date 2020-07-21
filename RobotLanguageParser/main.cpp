/*Function:主程序启动机器人语言解释器并控制机器人运动*/
# include<stdio.h>
# include<stdlib.h>
# include"lexicon.h"
# include"syntax.h"
# include"semantics.h"
# include"interpreter.h"
# include"robotInstruction.h"

int main()
{
	rootHead = (AST *)malloc(sizeof(AST));//根结点表头指针
	rootTail = rootHead;//根结点表尾指针
	AST initial = { 0 };
	*rootHead = initial;
	varProgName = "program1";
	projName = "project1";
	varProgQueue.push(varProgName);
	if (!varProgCompile())
	{
		mainProgRoot = rootHead->next;
		//初始化机器人运动参数及设置
		robotInitial();
		//开始执行程序
		progRun(mainProgRoot); //rootTail才是主程序的根结点
		//释放所有结点申请的内存，防止内存泄漏
		while (rootHead != NULL)
		{
			AST *temp = rootHead->next;
			deleteAst(rootHead);
			rootHead = temp;
		}
	}
	else
	{
		printf("编译不通过！");
		return -1;
	}
	return 0;
}