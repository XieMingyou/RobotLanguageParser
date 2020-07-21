/*Function:�������ű�&����������&��������*/
#ifndef Semantics_H
#define Semantics_H
# include<queue>

using namespace std;
extern char *varProgName; //�����������
extern char *projName; //������������Ŀ������
extern struct AST *mainProgRoot; //�������Ӧ�����
extern queue<char*> varProgQueue; //��Ҫ����ı����ͳ����ļ�����
extern char *curVarProg; //��ǰ���ڱ���ı����ͳ����ļ�����
extern struct AST *rootHead, *rootTail; //�������ͷָ���βָ��
extern struct VAR *varHead, *varTail; //���ű��ͷָ���βָ��
extern struct LABEL *labelHead, *labelTail; //��ǩ�����ͷָ���βָ��
extern struct CARTSYS worldRefSys; //��������ϵ
extern int flag;//�ı������Ƿ�����ǣ�0��ʾû�д�������ֵ��ʾ�д���


//����������ؽ�����ϵ�µ�λ������
struct AXISPOS
{
	double a1, a2, a3, a4, a5, a6, aux1, aux2, aux3, aux4, aux5;
};

//����������������ϵ�µ�λ������
struct CARTPOS
{
	double x, y, z, a, b, c, aux1, aux2, aux3, aux4, aux5;
	int mode;
};

//�����˸����������
struct AUXAXISPOS
{
	double aux1, aux2, aux3, aux4, aux5;
};

//�������ŷ������λ������
struct DRIVEPOS
{
	double d1, d2, d3, d4, d5, d6;
};

//ֱ������ϵ
struct CARTSYS
{
	double x, y, z, a, b, c;
};

//ֱ�ǲο�����ϵ
struct CARTREFSYS
{
	char *baseRefSys;
	double x, y, z, a, b, c;

};

//�������˶�����
struct DYNAMIC
{
	double velAxis, accAxis, decAxis, jerkAxis, velPath, accPath, decPath, jerkPath, velOri, accOri, decOri, jerkOri;
};

/*�����﷨���Ľ��*/
struct AST
{
	struct AST *l;//���ӽ��
	struct AST *r;//�ֵܽ��
	struct AST *f;//���׽��
	int line; //�к�
	char *name;//�﷨��Ԫ������
	char *content;//�﷨��Ԫ����ֵ(��¼id��intgr��flt��str��bl������)
	char *type;//�﷨��Ԫ��������:��Ҫ���ڲ�������������ƥ���жϣ��������Ͱ�����INTEGER REAL STRING BOOL AXISPOS CARTPOS ROBSXISPOS ROBCARTPOS WORLDREFSYS CARTREFSYS TOOL DYNAMIC
	int intgr;
	double flt;
	char *str;
	bool bl;
	struct AXISPOS axisPos;
	struct CARTPOS cartPos;
	struct AUXAXISPOS auxAxisPos;
	struct CARTSYS cartSys;
	struct CARTREFSYS cartRefSys;
	struct DYNAMIC dynamic;
	struct AST *next;//�����ָ��
	struct AST *thisRoot; //���ĸ����ָ��
	int activeProg; //�жϸ�����Ӧ�ĳ����Ƿ񱻼��activeProg==0ʱ��ֹ����activeProg==1ʱ�������
	int runProg; //�жϸ�����Ӧ�ĳ����Ƿ�ִ�У�runProg==0ʱֹͣ����runProg==1ʱ��������
	char *varProgName; //���ڼ�¼������Ӧ�ı����ͳ����ļ�
	int hasRun;//�ж�����Ƿ��Ѿ�����
};

/*�������ű�Ľ��*/
struct VAR
{
	char *content;//������
	char *type;//��������
	int intgr;
	double flt;
	char *str;
	bool bl;
	struct AXISPOS axisPos;
	struct CARTPOS cartPos;
	struct CARTSYS cartSys;
	struct CARTREFSYS cartRefSys;
	struct DYNAMIC dynamic;
	struct VAR *next;//ָ��
};

struct LABEL
{
	char *labelName; //��ǩ��
	struct LABEL *next; //����ָ��
};

/*=====�����﷨��========================*/
/*��������﷨��,�䳤������name:�﷨��Ԫ���֣�num:�䳤�������﷨������*/
struct AST *newAst(char *name, int num, ...);

/*�������﷨�����ӽ��ֵ���������*/
void ast2ast(struct AST *a1, struct AST *a2);

/*���������﷨����levelΪ���Ĳ���*/
void traverse(struct AST*, int level);

/*=====�������ű�========================*/
/*�����������ű�*/
void newVar(struct AST*, struct AST*);

/*���ұ����Ƿ��Ѿ�����,�Ƿ���1���񷵻�0*/
int  findVar(struct AST *a);

/*���ұ�������*/
char *typeVar(struct VAR *a);

/*�����ű�ı���ֵ��ֵ�������﷨����id���*/
void var2ast(struct AST *a);

/*�������﷨����id���ֵ��ֵ�����ű�ı���*/
void ast2var(struct AST *a);

/*�����ű�ĳ�Ա����ֵ��ֵ�������﷨����DOT_expr���*/
void dot_var2ast(struct AST *a1, struct AST *a2, struct AST *a3);

/*�������﷨����DOT_expr���ֵ��ֵ�����ű�ĳ�Ա����*/
void dot_ast2var(struct AST *a1, struct AST *a2, struct AST *a3);

/*=====��ǩ����========================*/
/*������ǩ����*/
void newLabel(struct AST *a);

/*���ұ�ǩ�Ƿ��Ѿ�����,�Ƿ���1���񷵻�0*/
int  findLabel(struct AST *a);

/*====��������================*/
/*���������������������Ƿ�ƥ��*/
void assignment(struct AST *a1, struct AST *a2);
void logical(struct AST *a1, struct AST *a2, struct AST *a3, struct AST *a4);
void arithmetic(struct AST *a1, struct AST *a2, struct AST *a3, struct AST *a4);
void minusOperator(struct AST *a1, struct AST *a2);
void notOperator(struct AST *a1, struct AST *a2);

/*��������ļ����������Ƿ���ȷ*/
void defTypeError(struct AST *a1, struct AST *a2);

/*�������ļ��еı����Ƿ��Ѷ���*/
void existVar(struct AST *a);

/*����������ʽ�������Ƿ�ΪBOOL*/
void isBool(struct AST *a);

/*������������*/
void yyerror(char *s, ...);

/*��������ͳ����ļ�*/
int varProgCompile();

/*�ͷ������������̬������ڴ棬��ֹ�ڴ�й©*/
void deleteAst(struct AST *a);

/*��������µ������ӽ���thisRootָ�����㱾��*/
void setThisRoot(struct AST *a);

#endif // Semantics_H