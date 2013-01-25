/*    author Wang Yuchen    */
/*    05111103            */

#include<stdio.h>
#include<stdlib.h>
#define NUM 20
#define MAXPRICE 500
#define MAXQTT 100
void menu();
void showinfo();
void addinfo();
void modifyinfo();
void delinfo();
void stat();
void randominfo();
void init();
void showinfoby();
void clrscr();

typedef struct{
    int year;
    int month;
    int day;
}DATE;
typedef struct{
    int num;
    char name[40];
    int kind;
    int price;
    int cost;
    int qtt;
    char origin[20];
    DATE date;
}GIFTINFO;

main()
{
int i,sel;      /*sel为目录选择*/
GIFTINFO g[NUM+1];
init(g);
randominfo(g);
do
{
    menu();
    scanf("%d",&sel);
switch(sel)
    {
    case 1:
    showinfo(g,0,1);
    break;

    case 2:
    addinfo(g);
    break;

    case 3:
    modifyinfo(g);
    break;

    case 4:
    clrscr();
    printf("Input Gift Number : ");
    scanf("%d",&i);
    showinfo(g,i,1);
    break;

    case 5:
    delinfo(g);
    break;

    case 6:
    stat(g,1);
    break;

    case 7:
    stat(g,2);
    break;

    case 8:
    showinfoby(g,1);
    break;

    case 9:
    showinfoby(g,2);
    break;

    case 0:
    break;

    default:
    printf("Input Error");
    getchar();
    break;
    }
getchar();
}while(sel!=0);
}

void menu()     /*菜单输出*/
{
clrscr();
printf("\n********Gift Manager*******\n");
printf("\n---------MENU---------");
printf("\n1. View    Gift Info");
printf("\n2. Add     Gift Info");
printf("\n3. Modify  Gift Info");
printf("\n4. Search  Gift Info");
printf("\n5. Delete  Gift Info");
printf("\n\n---------STAT---------");
printf("\n6. All   Goods Value");
printf("\n7. Kind  Goods Value");
printf("\n8. View Info sort ascending");
printf("\n9. View Info sort descending");
printf("\n\n0. Quit");
printf("\n\n*****copyright by WYc*****\n");
printf("\n\nselect:");
}

void showinfo(GIFTINFO a[],int i,int o)       /*输出数据*/
{                                             /*i控制功能，当i=0时输出全部商品信息，当i为货号时输出该货号商品信息，如不存在输出错误信息*/
int q,r=1;                                    /*o控制功能，当o=0时不输出清屏幕等信息*/
if(i==0)    /*全部输出*/
    {
    clrscr();
    printf("                * * * * * * * G I F T   L I S T * * * * * * * \n");
    printf("\n----NUMBER-----NAME----KIND---PRICE---COST---QUANTITY-----ORIGIN-----DATE---- \n");
    for(q=0;q<NUM;q++)
        {
        if(a[q].num<10000||a[q].name[0]=='\n') continue;
        printf("%10d",a[q].num);
        printf("%9s",a[q].name);
        printf("%8d",a[q].kind);
        printf("%8d",a[q].price);
        printf("%7d",a[q].cost);
        printf("%11d",a[q].qtt);
        printf("%11s",a[q].origin);
        printf("   %4d.%2d.%2d",a[q].date.year,a[q].date.month,a[q].date.day);
        printf("\n");
        r++;
        if(r%30==0)
            {
            getchar();
            printf("press any key to continue ...");
            getchar();
            }
        }
    }
else        /*输出指定信息*/
    {
    if(i>10000&&a[i-10001].num==i)
        {
        q=i-10001;
        if(o==1)
            {
            clrscr();
            printf("            * * * * * * * No. %d  G I F T   I N F O * * * * * * * \n",i);
            printf("\n----NUMBER-----NAME----KIND---PRICE---COST---QUANTITY-----ORIGIN-----DATE---- \n");
            }
        printf("%10d",a[q].num);
        printf("%9s",a[q].name);
        printf("%8d",a[q].kind);
        printf("%8d",a[q].price);
        printf("%7d",a[q].cost);
        printf("%11d",a[q].qtt);
        printf("%11s",a[q].origin);
        printf("   %4d.%2d.%2d",a[q].date.year,a[q].date.month,a[q].date.day);
        printf("\n");
        }
     else printf("No.%d Gift not found.\n",i);
    }
if(o==1)
    {
    printf("\nPress Enter return to menu ...");
    getchar();
    }
}

void addinfo(GIFTINFO a[])      /*添加数据*/
{
int q;
do
{
clrscr();
printf("            * * * * * * * G I F T   I N F O    A D D * * * * * * * \n");
printf("Input Gift Number : ");
scanf("%d",&q);
    if(q>10000&&q<(10000+NUM))
        {
        a[q-10001].num=q;
        q=q-10001;
        getchar();
        printf("\nNAME input: ");
        fgets(a[q].name,20,stdin);
        printf("\nKIND input (1-9) : ");
        scanf("%d",&a[q].kind);
        printf("\nPRICE input : ");
        scanf("%d",&a[q].price);
        printf("\nCOST input : ");
        scanf("%d",&a[q].cost);
        printf("\nQUANTITY input : ");
        scanf("%d",&a[q].qtt);
        getchar();
        printf("\nORIGIN input :");
        fgets(a[q].origin,20,stdin);
        printf("\nDATE input YEAR : ");
        scanf("%d",&a[q].date.year);
        printf("\nDATE input MONTH : ");
        scanf("%d",&a[q].date.month);
        printf("\nDATE input input DAY : ");
        scanf("%d",&a[q].date.day);
        printf("\nAdd successful.");
        q=0;
        printf("\n\nPress Enter return to menu ...");
        getchar();
        }
    else
        {
        printf("\nInput error! input again.\n\nPress 0 return to menu ...");
        if(q==0) break;
        getchar();
        getchar();
        }
}while(q!=0);
}

void modifyinfo(GIFTINFO a[])       /*修改数据，与函数addinfo基本相同，只是printf出的内容不同*/
{
int q;
do
{
    clrscr();
    printf("        * * * * * * * G I F T   I N F O    M O D I F Y * * * * * * * \n");
    printf("Input Gift Number you want to change : ");
    scanf("%d",&q);
        if(q>10000&&q<(10000+NUM))
        {
        getchar();
        a[q-10001].num=q;
        q=q-10001;
        printf("\nNAME change : ");
        fgets(a[q].name,20,stdin);
        printf("\nKIND change (1-9) : ");
        scanf("%d",&a[q].kind);
        printf("\nPRICE change : ");
        scanf("%d",&a[q].price);
        printf("\nCOST change : ");
        scanf("%d",&a[q].cost);
        printf("\nQUANTITY change : ");
        scanf("%d",&a[q].qtt);
        getchar();
        printf("\nORIGIN change :");
        fgets(a[q].origin,20,stdin);
        printf("\nDATE change input YEAR : ");
        scanf("%d",&a[q].date.year);
        printf("\nDATE change input MONTH : ");
        scanf("%d",&a[q].date.month);
        printf("\nDATE change input DAY : ");
        scanf("%d",&a[q].date.day);
        printf("\nModify successful.");
        q=0;
        printf("\n\nPress Enter return to menu ...");
        getchar();
        }
    else
        {
        printf("\nInput error! input again.\n\nPress 0 return to menu ...");
        if(q==0) break;
        getchar();
        getchar();
        }
}while(q!=0);
}

void delinfo(GIFTINFO a[])      /*删除数据*/
{
int q;
char ch;
do
{
    clrscr();
    printf("        * * * * * * * G I F T   I N F O    D E L E T E * * * * * * * \n");
    printf("Input Gift Number you want to delete : ");
    scanf("%d",&q);
        if(q>10000&&q<(10000+NUM))
        {
        getchar();
        q=q-10001;

        printf("\nNo.%d Gift info will be deleted\n\nPress Y to aplly, press N to abort : ",q+10001);
        scanf("%c",&ch);
        if(ch=='n') printf("User abort.");
        else
            {
                if(ch=='y')
                {
                a[q].num=0;     /*将.num改为0、.name[0]改为\0既视为删除，之前showinfo函数已定义检查条件*/
                a[q].name[0]='\0';
                printf("\nInfo Deleted.");
                }
            }
        q=0;
        printf("\n\nPress Enter return to menu ...");
        getchar();
        }
    else
        {
        printf("\nInput error! input again.\n\nPress 0 return to menu ...");
        if(q==0) break;
        getchar();
        getchar();
        }
}while(q!=0);
}

void stat(GIFTINFO a[],int i)       /*统计*/
{                                   /*i为1时统计所有商品价值之和，为2时统计某种类(kind)之和*/
int q;
long sum=0,t1,t2;
clrscr();
printf("          * * * * * * * G I F T   I N F O   S T A T * * * * * * * \n");
switch(i)
    {
    case 1:
    for(q=0;q<NUM;q++)
        {
        t1=a[q].price;
        t2=a[q].qtt;
        if(a[q].name[0]=='\0') continue;
        sum+=t1*t2;
        }
    printf("\nTotal prices is %ld",sum);
    break;
    case 2:
    printf("Input Kind : ");
    scanf("%d",&i);
    for(q=0;q<NUM;q++)
        {
        t1=a[q].price;
        t2=a[q].qtt;
        if(a[q].name[0]=='\0'||a[q].kind!=i) continue;
        sum+=t1*t2;
        }
    printf("\nKind %d total prices is %ld",i,sum);
    break;
    }
printf("\n\nPress Enter return to menu ...");
getchar();
}

void randominfo(GIFTINFO a[])       /*随机产生数据*/
{
int q,i;
srand((unsigned)time(0));
for(q=0;q<NUM;q++)
    {
    a[q].num=q+10001;
    a[q].name[0]=(rand()%26)+65;
    for(i=1;i<(rand()%5)+3;i++)  a[q].name[i]=(rand()%26)+97;
    a[q].name[i] = '\0';
    a[q].kind=(rand()%9)+1;
    a[q].price=(rand() % MAXPRICE)+1;
    do                              /*dowhile循环作用为使cost(成本)始终小于等于price(价格)*/
        {
        a[q].cost=(rand() % MAXPRICE)+1;
        }while(a[q].cost>a[q].price);
    a[q].qtt=rand() % MAXQTT;
    a[q].origin[0]=(rand()%26)+65;
    for(i=1;i<(rand()%5)+2;i++)  a[q].origin[i]=(rand()%26)+97;
    a[q].origin[i] = '\0';
    a[q].date.year=(rand()%5)+2000;
    a[q].date.month=(rand()%12)+1;
    a[q].date.day=(rand()%31)+1;
    }
}

void init(GIFTINFO a[])     /*初始化数据*/
{                           /*貌似a[NUM-1]总是出错，所以在程序开始时运行*/
int q;                      /*作用是把所有数据清0*/
for(q=0;q<NUM;q++)
    {
    a[q].num=0;
    a[q].name[0]='\0';
    a[q].kind=0;
    a[q].price=0;
    a[q].cost=0;
    a[q].qtt=0;
    a[q].origin[0]='\0';
    a[q].date.year=0;
    a[q].date.month=0;
    a[q].date.day=0;
    }
}

void showinfoby(GIFTINFO a[],int s)     /*升降序输出,s为1时升序，其他为降序*/
{
int i,j,m,n;
int list[NUM];
n=0;
for(j=0;j<NUM;j++)
{
    m=MAXPRICE;
    for(i=0;i<NUM;i++)
    {
    if(a[i].price<=m&&a[i].price>n)
        {
        m=a[i].price;
        list[j]=i;
        }
    }
n=m;
}
clrscr();
printf("                * * * * * * * G I F T   L I S T * * * * * * * \n");
printf("\n----NUMBER-----NAME----KIND---PRICE---COST---QUANTITY-----ORIGIN-----DATE---- \n");
if(s==1) for(i=0;i<NUM;i++)  showinfo(a,list[i]+10001,0);
else    for(i=NUM-1;i>=0;i--)  showinfo(a,list[i]+10001,0);
printf("\nPress Enter return to menu ...");
getchar();
}

void clrscr()
{
	int i;
	for (i = 0; i < 100; i++) printf("\n");
}
