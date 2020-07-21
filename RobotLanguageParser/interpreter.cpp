/*Function:实现各指令语句的解释，并调用机器人运动控制API*/
# include<stdlib.h>
# include "semantics.h"
# include "interpreter.h"
# include <windows.h>
# include "robotInstruction.h"
# include <thread>

//查找子程序的根结点
struct AST *findRoot(char *varProgName)
{
	struct AST *subProgRoot = rootHead->next;
	while (subProgRoot != NULL)
	{
		if (strcmp(subProgRoot->varProgName, varProgName))
		{
			subProgRoot = subProgRoot->next;
		}
		else
		{
			break;
		}
	}
	return subProgRoot;
}

//开始执行程序
void progRun(struct AST *a)
{		
	if (a != NULL)
	{
		//程序激活状态下的暂停
		while ((a->thisRoot->activeProg == 1 && a->thisRoot->runProg == 0) || (mainProgRoot->activeProg == 1 && mainProgRoot->runProg == 0))
		{
			Sleep(1);
		}
		//程序终止
		if ((a->thisRoot->activeProg == 0) || (mainProgRoot->activeProg == 0))
		{
			return;
		}
		if ((!strcmp(a->name, "PTP_stm")) && (a->hasRun == 0))
		{
			PTP_interpreter(a);
		}
		else if ((!strcmp(a->name, "Lin_stm")) && (a->hasRun == 0))
		{
			Lin_interpreter(a);
		}
		else if ((!strcmp(a->name, "Circ_stm")) && (a->hasRun == 0))
		{
			Circ_interpreter(a);
		}
		else if ((!strcmp(a->name, "PTPRel_stm")) && (a->hasRun == 0))
		{
			PTPRel_interpreter(a);
		}
		else if ((!strcmp(a->name, "LinRel_stm")) && (a->hasRun == 0))
		{
			LinRel_interpreter(a);
		}
		else if ((!strcmp(a->name, "StopRobot_stm")) && (a->hasRun == 0))
		{
			StopRobot_interpreter(a);
		}
		else if ((!strcmp(a->name, "RefRobotAxis_stm")) && (a->hasRun == 0))
		{
			RefRobotAxis_interpreter(a);
		}
		else if ((!strcmp(a->name, "RefRobotAxisAsync_stm")) && (a->hasRun == 0))
		{
			RefRobotAxisAsync_interpreter(a);
		}
		else if ((!strcmp(a->name, "MoveRobotAxis_stm")) && (a->hasRun == 0))
		{
			MoveRobotAxis_interpreter(a);
		}
		else if ((!strcmp(a->name, "Dyn_stm")) && (a->hasRun == 0))
		{
			Dyn_interpreter(a);
		}
		else if ((!strcmp(a->name, "Dynovr_stm")) && (a->hasRun == 0))
		{
			Dynovr_interpreter(a);
		}
		else if ((!strcmp(a->name, "Ramp_stm")) && (a->hasRun == 0))
		{
			Ramp_interpreter(a);
		}
		else if ((!strcmp(a->name, "Refsys_stm")) && (a->hasRun == 0))
		{
			Refsys_interpreter(a);
		}
		else if ((!strcmp(a->name, "Tool_stm")) && (a->hasRun == 0))
		{
			Tool_interpreter(a);
		}
		else if ((!strcmp(a->name, "WaitTime_stm")) && (a->hasRun == 0))
		{
			WaitTime_interpreter(a);
		}
		else if ((!strcmp(a->name, "Stop_stm")) && (a->hasRun == 0))
		{
			Stop_interpreter(a);
		}
		else if ((!strcmp(a->name, "Notice_stm")) && (a->hasRun == 0))
		{
			Notice_interpreter(a);
		}
		else if ((!strcmp(a->name, "Warning_stm")) && (a->hasRun == 0))
		{
			Warning_interpreter(a);
		}
		else if ((!strcmp(a->name, "Error_stm")) && (a->hasRun == 0))
		{
			Error_interpreter(a);
		}
		else if ((!strcmp(a->name, "CALL_stm")) && (a->hasRun == 0))
		{
			CALL_interpreter(a);
		}
		else if ((!strcmp(a->name, "WAIT_stm")) && (a->hasRun == 0))
		{
			WAIT_interpreter(a);
		}
		else if ((!strcmp(a->name, "IF_stm")) && (a->hasRun == 0))
		{
			IF_interpreter(a);
		}
		else if ((!strcmp(a->name, "WHILE_stm")) && (a->hasRun == 0))
		{
			WHILE_interpreter(a);
		}
		else if ((!strcmp(a->name, "LOOP_stm")) && (a->hasRun == 0))
		{
			LOOP_interpreter(a);
		}
		else if ((!strcmp(a->name, "RUN_stm")) && (a->hasRun == 0))
		{
			RUN_interpreter(a);
		}
		else if ((!strcmp(a->name, "RETURN_stm")) && (a->hasRun == 0))
		{
			RETURN_interpreter(a);
		}
		else if ((!strcmp(a->name, "GOTO_stm")) && (a->hasRun == 0))
		{
			GOTO_interpreter(a);
		}
		progRun(a->l);
		progRun(a->r);
	}
}

//将解释过的语句结点的hasRun成员置1，或执行完子程序后将子程序的所有结点的hasRun成员置为0，以便子程序可以被再次调用
void changeHasRun(struct AST *a, int num)
{
	if (a != NULL)
	{
		a->hasRun = num;
		changeHasRun(a->l, num);
		changeHasRun(a->r, num);
	}
}

//运动指令组
void PTP_interpreter(struct AST *a)
{
	int j = 0; //j标记位置参数的类型，j==0时位置类型为AXISPOS，j==1时位置类型为CARTPOS
	int hasDyn = 0; //标记是否有DYNAMIC参数
	a->hasRun = 1;
	a = a->l;
	struct AXISPOS axisPos;
	struct CARTPOS cartPos;
	struct DYNAMIC dynamic;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "id"))
		{
			if (!strcmp(a->type, "AXISPOS"))
			{
				axisPos = a->axisPos;
			}
			else if (!strcmp(a->type, "CARTPOS"))
			{
				j = 1;
				cartPos = a->cartPos;
			}
			if (!strcmp(a->type, "DYNAMIC"))
			{
				dynamic = a->dynamic;
				hasDyn = 1;
			}
		}
	}
	if (hasDyn == 0)
	{
		if (j == 0)
		{
			PTP_AP(axisPos);
		}
		else if (j == 1)
		{
			PTP_CP(cartPos);
		}
	}
	else if (hasDyn == 1)
	{
		if (j == 0)
		{
			PTP_AP_DYN(axisPos, dynamic);
		}
		else if (j == 1)
		{
			PTP_CP_DYN(cartPos, dynamic);
		}
	}
}

void Lin_interpreter(struct AST *a)
{
	int j = 0; //j标记位置参数的类型，j==0时位置类型为AXISPOS，j==1时位置类型为CARTPOS
	int hasDyn = 0; //标记是否有DYNAMIC参数
	a->hasRun = 1;
	a = a->l;
	struct AXISPOS axisPos;
	struct CARTPOS cartPos;
	struct DYNAMIC dynamic;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "id"))
		{
			if (!strcmp(a->type, "AXISPOS"))
			{
				axisPos = a->axisPos;
			}
			else if (!strcmp(a->type, "CARTPOS"))
			{
				j = 1;
				cartPos = a->cartPos;
			}
			if (!strcmp(a->type, "DYNAMIC"))
			{
				dynamic = a->dynamic;
				hasDyn = 1;
			}
		}
	}
	if (hasDyn == 0)
	{
		if (j == 0)
		{
			Lin_AP(axisPos);
		}
		else if (j == 1)
		{
			Lin_CP(cartPos);
		}
	}
	else if (hasDyn == 1)
	{
		if (j == 0)
		{
			Lin_AP_DYN(axisPos, dynamic);
		}
		else if (j == 1)
		{
			Lin_CP_DYN(cartPos, dynamic);
		}
	}
}

void Circ_interpreter(struct AST *a)
{
	int i = 0, j = 0, k = 0; //i记录第几个位置，j、k标记位置参数的类型，j、k==0时位置类型为AXISPOS，j、k==1时位置类型为CARTPOS
	int hasDyn = 0;
	a->hasRun = 1;
	a = a->l;
	struct AXISPOS axisPos1;
	struct AXISPOS axisPos2;
	struct CARTPOS cartPos1;
	struct CARTPOS cartPos2;
	struct DYNAMIC dynamic;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "id"))
		{
			i++;
			if (i == 1)
			{
				if (!strcmp(a->type, "AXISPOS"))
				{
					axisPos1 = a->axisPos;
				}
				else if (!strcmp(a->type, "CARTPOS"))
				{
					j = 1;
					cartPos1 = a->cartPos;
				}
			}
			else if (i == 2)
			{
				if (!strcmp(a->type, "AXISPOS"))
				{
					axisPos2 = a->axisPos;
				}
				else if (!strcmp(a->type, "CARTPOS"))
				{
					k = 1;
					cartPos2 = a->cartPos;
				}
			}
			if (!strcmp(a->type, "DYNAMIC"))
			{
				dynamic = a->dynamic;
				hasDyn = 1;
			}
		}
	}
	if (hasDyn == 0)
	{
		if (j == 0 && k == 0)
		{
			Circ_AP_AP(axisPos1, axisPos2);
		}
		else if (j == 1 && k == 0)
		{
			Circ_CP_AP(cartPos1, axisPos2);
		}
		else if (j == 0 && k == 1)
		{
			Circ_AP_CP(axisPos1, cartPos2);
		}
		else if (j == 1 && k == 1)
		{
			Circ_CP_CP(cartPos1, cartPos2);
		}
	}
	else if (hasDyn == 1)
	{
		if (j == 0 && k == 0)
		{
			Circ_AP_AP_DYN(axisPos1, axisPos2, dynamic);
		}
		else if (j == 1 && k == 0)
		{
			Circ_CP_AP_DYN(cartPos1, axisPos2, dynamic);
		}
		else if (j == 0 && k == 1)
		{
			Circ_AP_CP_DYN(axisPos1, cartPos2, dynamic);
		}
		else if (j == 1 && k == 1)
		{
			Circ_CP_CP_DYN(cartPos1, cartPos2, dynamic);
		}
	}
}

void PTPRel_interpreter(struct AST *a)
{
	int j = 0; //j标记位置参数的类型，j==0时位置类型为AXISPOS，j==1时位置类型为CARTPOS
	int hasDyn = 0; //标记是否有DYNAMIC参数
	a->hasRun = 1;
	a = a->l;
	struct AXISPOS axisPos;
	struct CARTPOS cartPos;
	struct DYNAMIC dynamic;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "id"))
		{
			if (!strcmp(a->type, "AXISPOS"))
			{
				axisPos = a->axisPos;
			}
			else if (!strcmp(a->type, "CARTPOS"))
			{
				j = 1;
				cartPos = a->cartPos;
			}
			if (!strcmp(a->type, "DYNAMIC"))
			{
				dynamic = a->dynamic;
				hasDyn = 1;
			}
		}
	}
	if (hasDyn == 0)
	{
		if (j == 0)
		{
			PTPRel_AP(axisPos);
		}
		else if (j == 1)
		{
			PTPRel_CP(cartPos);
		}
	}
	else if (hasDyn == 1)
	{
		if (j == 0)
		{
			PTPRel_AP_DYN(axisPos, dynamic);
		}
		else if (j == 1)
		{
			PTPRel_CP_DYN(cartPos, dynamic);
		}
	}
}

void LinRel_interpreter(struct AST *a)
{
	int j = 0; //j标记位置参数的类型，j==0时位置类型为AXISPOS，j==1时位置类型为CARTPOS
	int hasDyn = 0; //标记是否有DYNAMIC参数
	a->hasRun = 1;
	a = a->l;
	struct AXISPOS axisPos;
	struct CARTPOS cartPos;
	struct DYNAMIC dynamic;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "id"))
		{
			if (!strcmp(a->type, "AXISPOS"))
			{
				axisPos = a->axisPos;
			}
			else if (!strcmp(a->type, "CARTPOS"))
			{
				j = 1;
				cartPos = a->cartPos;
			}
			if (!strcmp(a->type, "DYNAMIC"))
			{
				dynamic = a->dynamic;
				hasDyn = 1;
			}
		}
	}
	if (hasDyn == 0)
	{
		if (j == 0)
		{
			LinRel_AP(axisPos);
		}
		else if (j == 1)
		{
			LinRel_CP(cartPos);
		}
	}
	else if (hasDyn == 1)
	{
		if (j == 0)
		{
			LinRel_AP_DYN(axisPos, dynamic);
		}
		else if (j == 1)
		{
			LinRel_CP_DYN(cartPos, dynamic);
		}
	}
}

void StopRobot_interpreter(struct AST *a)
{
	int stopMode = 0;
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->content, "HARDSTOP"))
		{
			stopMode = 0;
		}
		else if (!strcmp(a->content, "SOFTSTOP"))
		{
			stopMode = 1;
		}
	}
	StopRobot_(stopMode);
}

void RefRobotAxis_interpreter(struct AST *a)
{
	int j = 0; //记录要回零的轴
	int hasAngle = 0, hasDyn = 0; //标记是否有angle，DYNAMIC参数
	double angle; //回零后要转动的角度
	struct DYNAMIC dynamic;
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "id"))
		{
			if (!strcmp(a->content, "A1"))
			{
				j = 1;
			}
			else if (!strcmp(a->content, "A2"))
			{
				j = 2;
			}
			else if (!strcmp(a->content, "A3"))
			{
				j = 3;
			}
			else if (!strcmp(a->content, "A4"))
			{
				j = 4;
			}
			else if (!strcmp(a->content, "A5"))
			{
				j = 5;
			}
			else if (!strcmp(a->content, "A6"))
			{
				j = 6;
			}
			if (!strcmp(a->type, "FLOAT"))
			{
				angle = a->flt;
				hasAngle = 1;
			}
			else if (!strcmp(a->type, "INTEGER"))
			{
				angle = a->intgr;
				hasAngle = 1;
			}
			if (!strcmp(a->type, "DYNAMIC"))
			{
				dynamic = a->dynamic;
				hasDyn = 1;
			}
		}
		if (!strcmp(a->name, "real"))
		{
			angle = a->flt;
			hasAngle = 1;
		}
	}
	if (hasAngle == 0 && hasDyn == 0)
	{
		RefRobotAxis_(j);
	}
	else if (hasAngle == 1 && hasDyn == 0)
	{
		RefRobotAxis_AG(j, angle);
	}
	else if (hasAngle == 0 && hasDyn == 1)
	{
		RefRobotAxis_DYN(j, dynamic);
	}
	else if (hasAngle == 1 && hasDyn == 1)
	{
		RefRobotAxis_AG_DYN(j, angle, dynamic);
	}
}
void RefRobotAxisAsync_interpreter(struct AST *a)
{
	int hasDyn = 0; //标记是否有DYNAMIC参数
	struct DYNAMIC dynamic;
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "id"))
		{
			if (!strcmp(a->type, "DYNAMIC"))
			{
				dynamic = a->dynamic;
				hasDyn = 1;
			}
		}
	}
	if (hasDyn == 0)
	{
		RefRobotAxisAsync_();
	}
	else if (hasDyn == 1)
	{
		RefRobotAxisAsync_DYN(dynamic);
	}
}

void MoveRobotAxis_interpreter(struct AST *a)
{
	int j = 0; //标记要转动的轴
	int hasDyn = 0; //标记是否有DYNAMIC参数
	double angle; //要转动的角度
	struct DYNAMIC dynamic;
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "id"))
		{
			if (!strcmp(a->content, "A1"))
			{
				j = 1;
			}
			else if (!strcmp(a->content, "A2"))
			{
				j = 2;
			}
			else if (!strcmp(a->content, "A3"))
			{
				j = 3;
			}
			else if (!strcmp(a->content, "A4"))
			{
				j = 4;
			}
			else if (!strcmp(a->content, "A5"))
			{
				j = 5;
			}
			else if (!strcmp(a->content, "A6"))
			{
				j = 6;
			}
			if (!strcmp(a->type, "FLOAT"))
			{
				angle = a->flt;
			}
			else if (!strcmp(a->type, "INTEGER"))
			{
				angle = a->intgr;
			}
			if (!strcmp(a->type, "DYNAMIC"))
			{
				dynamic = a->dynamic;
			}
		}
		if (!strcmp(a->name, "real"))
		{
			angle = a->flt;
		}
	}
	if (hasDyn == 0)
	{
		MoveRobotAxis_(j, angle);
	}
	else if (hasDyn == 1)
	{
		MoveRobotAxis_DYN(j, angle, dynamic);
	}
}

//设置指令组
void Dyn_interpreter(struct AST *a)
{
	struct DYNAMIC dynamic;
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "id"))
		{
			if (!strcmp(a->type, "DYNAMIC"))
			{
				dynamic = a->dynamic;
			}
		}
	}
	Dyn_DYN(dynamic, 0);
}

void Dynovr_interpreter(struct AST *a)
{
	double ovr;
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "id"))
		{
			if (!strcmp(a->type, "INTEGER"))
			{
				ovr = a->intgr;
			}
			else if (!strcmp(a->type, "FLOAT"))
			{
				ovr = a->flt;
			}
		}
		else if (!strcmp(a->name, "intgr"))
		{
			ovr = a->intgr;
		}
	}
	Dynovr_(ovr);
}

void Ramp_interpreter(struct AST *a)
{
	int j = 0; //标记速度曲线类型，j==0为S型，j==1为T型
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "id"))
		{
			if (!strcmp(a->content, "S"))
			{
				j = 0;
			}
			else if (!strcmp(a->content, "T"))
			{
				j = 1;
			}
		}
	}
	Ramp_(j);
}

void Refsys_interpreter(struct AST *a)
{
	struct CARTREFSYS cartRefSys;
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "id"))
		{
			cartRefSys = a->cartRefSys;
		}
	}
	Refsys_(cartRefSys);
}

void Tool_interpreter(struct AST *a)
{
	struct CARTSYS cartSys;
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "id"))
		{
			cartSys = a->cartSys;
		}
	}
	Tool_(cartSys);
}

//系统功能指令组
void WaitTime_interpreter(struct AST *a)
{
	double time;
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "intgr"))
		{
			time = a->intgr;
		}
		else if (!strcmp(a->name, "flt"))
		{
			time = a->flt;
		}
		if (!strcmp(a->name, "id"))
		{
			if (!strcmp(a->type, "INTEGER"))
			{
				time = a->intgr;
			}
			else if (!strcmp(a->type, "FLOAT"))
			{
				time = a->flt;
			}
		}
	}
	WaitTime_(time);
}

void Stop_interpreter(struct AST *a)
{
	int stopMode = 0;
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->content, "HARDSTOP"))
		{
			stopMode = 0;
		}
		else if (!strcmp(a->content, "SOFTSTOP"))
		{
			stopMode = 1;
		}
	}
	Stop_(stopMode);
}

void Notice_interpreter(struct AST *a)
{
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "str"))
		{
			char *text = a->content;
			Notice_(text);
		}
	}
}

void Warning_interpreter(struct AST *a)
{
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "str"))
		{
			char *text = a->content;
			Warning_(text);
		}
	}
}

void Error_interpreter(struct AST *a)
{
	a->hasRun = 1;
	a = a->l;
	while (strcmp(a->name, "RP"))
	{
		a = a->r;
		if (!strcmp(a->name, "str"))
		{
			char *text = a->content;
			Error_(text);
		}
	};
}

//流程控制指令组
void CALL_interpreter(struct AST *a)
{
	a->hasRun = 1;
	a = a->l;
	a = a->r;
	struct AST *subProgRoot = findRoot(a->content); //查找子程序的根结点
	subProgRoot->activeProg = 1; //激活子程序
	subProgRoot->runProg = 1; //开始子程序
	changeHasRun(subProgRoot, 0); //将子程序的结点全部置为0，以便子程序可以被再次调用
	progRun(subProgRoot); //开始执行子程序
}

void WAIT_interpreter(struct AST *a)
{
	a->hasRun = 1;
	while ((a->l->r->bl) == 0)
	{
		//Sleep(1);
		//程序激活状态下的暂停
		while ((a->thisRoot->activeProg == 1 && a->thisRoot->runProg == 0) || (mainProgRoot->activeProg == 1 && mainProgRoot->runProg == 0))
		{
			Sleep(1);
		}
		//程序终止
		if ((a->thisRoot->activeProg == 0) || (mainProgRoot->activeProg == 0))
		{
			return;
		}
	}
}

void IF_interpreter(struct AST *a)
{
	a->hasRun = 1;
	a = a->l; 
	a = a->r; 
	if ((a->bl) != 0)
	{
		a = a->r;
		a = a->r;
		if (!strcmp(a->name, "StatementList"))
		{
			progRun(a->l);
			changeHasRun(a->r, 1);
		}
		else
		{
			changeHasRun(a, 1);
		}
	}
	else
	{
		a = a->r;
		a = a->r;
		if (!strcmp(a->name, "StatementList"))
		{
			changeHasRun(a->l, 1);
			a = a->r;
		}
		int juge = 0;//判断是否满足ELSEIF的条件而跳出循环
		while (!strcmp(a->name, "ELSEIF_stm"))
		{
			a = a->l;
			a = a->r;
			if ((a->bl) != 0)
			{
				a = a->r;
				a = a->r;
				if (!strcmp(a->name, "StatementList"))
				{
					progRun(a->l);
					changeHasRun(a->r, 1);
				}
				else
				{
					changeHasRun(a, 1);
				}
				juge = 1;
				break;

			}
			else
			{
				a = a->r;
				a = a->r;
				if (!strcmp(a->name, "StatementList"))
				{
					changeHasRun(a->l, 1);
					a = a->r;
				}
			}	 
		}
		if ((juge == 0) && (!strcmp(a->name, "ELSE_stm")))
		{
			a = a->l;
			a = a->r;
			if (!strcmp(a->name, "StatementList"))
			{
				progRun(a->l);
			}
		}
	}
}

void WHILE_interpreter(struct AST *a)
{
	a->hasRun = 1;
	a = a->l;
	a = a->r;
	if (!strcmp(a->r->r->name, "StatementList"))
	{
		while ((a->bl) != 0)
		{
			a = a->r;
			a = a->r;
			progRun(a->l);
			changeHasRun(a->l, 0);
		}
	}
}

void LOOP_interpreter(struct AST *a)
{
	a->hasRun = 1;
	a = a->l;
	a = a->r;
	int loopTime = a->intgr;
	a = a->r;
	a = a->r;
	if (strcmp(a->name, "END_LOOP"))
	{
		for (int i = 0; i < loopTime; i++)
		{
			changeHasRun(a->l, 0);
			progRun(a->l);
		}
	}
}

void RUN_interpreter(struct AST *a)
{
	a->hasRun = 1;
	a = a->l;
	a = a->r;
	struct AST *subProgRoot = findRoot(a->content); //查找子程序的根结点
	subProgRoot->activeProg = 1; //激活子程序
	subProgRoot->runProg = 1; //开始子程序
	changeHasRun(subProgRoot, 0); //将子程序的结点全部置为0，以便子程序可以被再次调用
	std::thread parallelProgThread(progRun, subProgRoot);
	parallelProgThread.detach();
}

void KILL_interpreter(struct AST *a)
{
	a->hasRun = 1;
	a = a->l;
	a = a->r;
	struct AST *subProgRoot = findRoot(a->content); //查找子程序的根结点
	subProgRoot->activeProg = 0; //终止子程序
	subProgRoot->runProg = 0; //停止子程序
	changeHasRun(subProgRoot, 1); //将子程序的结点全部置为1，用于结束子程序使其不再运行
}

void RETURN_interpreter(struct AST *a)
{
	a->thisRoot->activeProg = 0; //终止子程序
	a->thisRoot->runProg = 0; //停止子程序
	changeHasRun(a->thisRoot, 1); //将所在程序的结点全部置为1，用于结束该程序使其不再运行
}

void searchLabel(struct AST *a, char *labelName)
{
	if (a != NULL)
	{
		if (!strcmp(a->content, labelName))
		{
			if (a->r != NULL)
			{
				if (!strcmp(a->r->content, ":"))
				{
					a = a->f->f;
					progRun(a);
					changeHasRun(a, 1);
					return;
				}
			}
		}
		searchLabel(a->l, labelName);
		searchLabel(a->r, labelName);
	}
}


void GOTO_interpreter(struct AST *a)
{
	char *labelName;
	if (!strcmp(a->l->name, "IF"))
	{
		a = a->l;
		a = a->r;
		labelName = a->r->r->content;
		if ((a->bl) != 0)
		{
			a = a->thisRoot;
			searchLabel(a, labelName);
		}
	}
	else if (!strcmp(a->l->name, "GOTO"))
	{
		labelName = a->l->r->content;
		a = a->thisRoot;
		searchLabel(a, labelName);
	}
}