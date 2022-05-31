#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <windows.h>
#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include <malloc.h>
#include <stdlib.h>
#include <iomanip>
#include <conio.h>
#include <sstream>
#include <io.h>
#include <Windows.h>
extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();//固定窗口
#define MAX 1000
#define N 100
using namespace std;
char username[N];//用户名
char tmp[100];
char sql[MAX],sqltmp[MAX];//存放sql整理之后关键语句。sqltmp临时存放
int t_num=0;    //表的数量
int i_num=0;    //索引的数量
int v_num=0;    //视图的数量
int u_num=0;    //用户的个数
int now;//目前用户标号；
//用户
class users
{
private:
    string name;
    string password;
    string table;//表名；
public://权限说明

    int revoke;
    int rselect;
    int rdelete;
    int rupdate;
    int rinsert;
    int rgrant;
    int rflag;
    users():revoke(1),rselect(1),rdelete(1),rupdate(1),rinsert(1),rgrant(1),rflag(1)
    {}//初始化
public:
    string n()
    {
        return name;
    }
    string t()
    {
        return table;
    }
    string p()
    {
        return password;
    }
    void n(string na)
    {
        name=na;
    }
    void p(string pa)
    {
        password=pa;
    }
       void t(string tt)
    {
        table=tt;
    }
};
users user[100];//存放用户
//表
struct Table
{
    char t_name[N];              //表名
    char property_name[N][N];    //属性名
    char property_type[N][N];    //属性类型
    int property_len;            //属性长度
    int property_num;           //属性的个数
    char condition[N][N];           //属性约束条件
    int record_num;              //记录的个数

    char record[N][N][N];		//三维表,保存的内容是记录的值
    Table()             //初始化
    {
        property_len = record_num = property_num = 0;
        memset(t_name,0,sizeof(t_name));
        memset(property_name,0,sizeof(property_name));
        memset(property_type,0,sizeof(property_type));
        memset(condition,0,sizeof(condition));
    }
};

//索引结构
struct Index
{
    char i_name[N];   //索引名
    char t_name[N];   //索引所对应的表名
    char key[N];       //
    char pro_name[N][N];
    int  pro_num;//数量
    Index()  //初始化
    {
        pro_num = 0;
         memset(i_name,0,sizeof(i_name));
          memset(t_name,0,sizeof(t_name));
           memset(pro_name,0,sizeof(pro_name));
    }
};

//视图结构
struct View
{
    char proname[N][N];	//可见的属性
	char sql[100];      //sql语句
    char v_name[N];   //视图名
    char t_name[N];   //视图所对应的表名
    int property_num; //属性的个数
     View()  //初始化
    {
      property_num = 0;
         memset(v_name,0,sizeof(v_name));
          memset(t_name,0,sizeof(t_name));
           memset(proname,0,sizeof(proname));
    }
};
Table tab[MAX];
View vie[MAX];
Index ind[MAX];
//更改权限到本地；
void  add_grant(char id[],char pass[],int f)
{
	 FILE *tf, *nf;
    tf = fopen("data.txt", "rb");//存放用户
    nf = fopen("datatemp.txt", "a+");//临时文件
    char reid[10], repass[10],ff[2];
  char fla[2],sel[2],del[2],upd[2],ins[2],gra[2],rev[2];
    while (!feof(tf))
    {
         fscanf(tf, "%s%s%s%s%s%s%s%s%s", reid, repass,fla,sel,del,upd,ins,gra,rev);
        if(strcmp(reid, id)!=0)
        {
            fprintf(nf, " %s %s %s %s %s %s %s %s %s",reid, repass,fla,sel,del,upd,ins,gra,rev);
        }
        else  if(strcmp(reid, id)==0)
        {
             if(f==1)
    {
             fprintf(nf, " %s %s %s 1 %s %s %s %s %s",reid, repass,fla,del,upd,ins,gra,rev);

    }
    else if(f==2)
    {
           fprintf(nf, " %s %s %s %s 1 %s %s %s %s",reid, repass,fla,sel,upd,ins,gra,rev);
    }

     else if(f==3)
     {
              fprintf(nf, " %s %s %s %s %s 1 %s %s %s",reid, repass,fla,sel,del,ins,gra,rev);
     }

     else if(f==4)
     {
            fprintf(nf, " %s %s %s %s %s %s 1 %s %s",reid, repass,fla,sel,del,upd,gra,rev);
     }

     else if(f==5)
     {
          fprintf(nf, " %s %s %s 0 %s %s %s %s %s",reid, repass,fla,del,upd,ins,gra,rev);
     }

     else if(f==6)
     {
          fprintf(nf, " %s %s %s %s %s %s 0 %s %s",reid, repass,fla,sel,del,upd,gra,rev);
     }

    else  if(f==7)
    {
          fprintf(nf, " %s %s %s %s 0 %s %s %s %s",reid, repass,fla,sel,upd,ins,gra,rev);
    }

     else if(f==8)
     {
           fprintf(nf, "%s %s %s %s %s 1 %s %s %s ",reid, repass,fla,sel,del,ins,gra,rev);
     }
        }

    }
    fclose(tf);
	fclose(nf);
	remove("data.txt");
	rename("datatemp.txt","data.txt");

}
//初始化user数组，将本地数据同步到数组中；
void readuser()
{
      int i=0;
     FILE *fp;
    char reid[10], repass[10];
    char fla[2],sel[2],del[2],upd[2],ins[2],gra[2],rev[2];
    fp = fopen("data.txt", "rw");
     while (!feof(fp))
    {
        fscanf(fp, "%s%s%s%s%s%s%s%s%s", reid, repass,fla,sel,del,upd,ins,gra,rev);
        user[i].n(reid);
        user[i].p(repass);
        user[i].rflag=fla[0]-'0';
        user[i].revoke=rev[0]-'0';
        user[i].rdelete=del[0]-'0';
        user[i].rinsert=ins[0]-'0';
        user[i].rselect=sel[0]-'0';
        user[i].rupdate=upd[0]-'0';
        user[i]. rgrant=gra[0]-'0';
        i++;
    }
   fclose(fp);
   u_num=i;
   return ;
}
//查找已注册账户的函数
bool search1(char id[],char pass[])
{
      int i;
      for(i=0;i<u_num;i++)
      {
          char reid[20];
          strcpy(reid,user[i].n().c_str());
           char repass[20];
          strcpy(repass,user[i].p().c_str());
          if(strcmp(reid,id)==0&&strcmp(repass,pass)==0)
          {
              now=i;
              return true;
          }

      }
      return false;
}
//账号注册查重
bool search2(char id[],string pass)
{

    FILE *fp;
    char reid[10], repass[10],ff[2];
  char fla[2],sel[2],del[2],upd[2],ins[2],gra[2],rev[2];
    fp = fopen("data.txt", "rw");
    while (!feof(fp))
    {
         fscanf(fp, "%s%s%s%s%s%s%s%s%s", reid, repass,fla,sel,del,upd,ins,gra,rev);
        if(strcmp(reid, id)==0)
        {
            fclose(fp);
            return true;
        }
    }
    fclose(fp);
    return false;
}
//系统登录函数
bool dwx_login()
{
    char id[10],pass[10];
    printf("+*-*+*-*+*-*欢迎登录DBMS系统+*-*+*-*+*-*\n\n");
    printf("请输入账号：");
    scanf("%s",id);
    printf("\n");
    printf("请输入密码：");
     char c;
     int i;
    for( i=0;c=getch();i++)
    {
        putch('*');
        if(c==13)
            break;
        pass[i]=c;
        if(i>=10)
        {
            break;
        }

    }
    pass[i]='\0';
   // scanf("%s",pass);
    printf("\n");
    if(search1(id,pass)==1)
    {
        printf("\n登陆成功\n\n");
        printf("\n正在跳转........\n\n");
        Sleep(800);

         system("cls");
        return true;
    }
    else
    {
        return false;
    }
}
//添加用户存到本地,更新user
void dwx_add(char id[],char pass[],int flag)
{
    FILE *fp;
    fp=fopen("data.txt","a+");
    if(flag)
    fprintf(fp," %s %s %d 1 1 1 1 1  1",id,pass,flag);
    else
    {
       fprintf(fp," %s %s %d 0 0 0 0 0  0",id,pass,flag);
    }
    user[u_num].n(id);
    user[u_num].p(pass);
    user[u_num].rflag=flag;
    if(flag)
    {
       user[u_num].revoke=1;
    user[u_num].rdelete=1;
    user[u_num].rinsert=1;
    user[u_num].rselect=1;
    user[u_num].rupdate=1;
    user[u_num]. rgrant=1;
    }
    else
    {
    user[u_num].revoke=0;
    user[u_num].rdelete=0;
    user[u_num].rinsert=0;
    user[u_num].rselect=0;
    user[u_num].rupdate=0;
    user[u_num]. rgrant=0;
    }

    u_num++;
    fclose(fp);
}
//管理员注册与普通用户注册
void dwx_register(int flag)
{
    int x=0;
    string Pass,Repass;
    char id[10],pass[10],repass[10];
    system("cls");
    if(flag)
    {
        printf("         +*-*+*-*+*-*欢迎注册管理员+*-*+*-*+*-*\n");
        printf("       +*-*+*-*+*-*请输入注册管理员通行证+*-*+*-*+*-*\n");
        printf("通行证：");
        int m;
        scanf("%d",&m);
        if(m!=2019)//设置了通行证密码。
        {
            printf("*------------对不起，您输入有误！-------------\n\n");
            printf("正在返回.....");
            Sleep(1500);
            system("cls");
            return ;
        }
    }
    else
        printf("+*-*+*-*+*-*注册普通用户+*-*+*-*+*-*\n");
    printf("\n");
    printf("请输入账号：");
    scanf("%s",id);
    printf("\n");
    while(true)
    {

        printf("提示：密码需要十个以内英文字母、阿拉伯数字、以及英文符号。\n");
    char ch;
    int i = 0;
    printf("请输入密码：");
    while((ch = _getch()) != '\r')
    {
    if(ch == '\b' && i > 0)
      {printf("\b \b");
        --i;
      }
    else if(ch != '\b')
     {
     pass[i++] = ch;
     printf("*");
     }
     if(i>=10)
      break;
     }
      pass[i] = '\0';
      Pass=pass;
    printf("\n");
    printf("请再次确认密码：");
    char ch2;
     i = 0;
    while((ch2 = _getch()) != '\r')
    {
    if(ch2 == '\b' && i > 0)
      {printf("\b \b");
        --i;
      }
    else if(ch2 != '\b')
    {
     repass[i++] = ch2;
     printf("*");
     }
     if(i>=10)
      break;
     }
     repass[i]='\0';
     Repass=repass;
        printf("\n");
        if(Pass!=Repass)
        {
            printf("您两次输入的密码不一致，请重新确认！\n");
            printf("\n");
            printf("正在返回.....");
            Sleep(1000);
            system("cls");

        }
        else if(search2(id,Pass)==1)
        {
             printf("账号已存在，请更换其他用户名尝试\n");
             printf("正在返回.....");
            Sleep(1000);
              system("cls");
             x=1;
               break;
        }
        else
        {
            break;
        }

    }
    if(x==1)
    {
        dwx_register(flag);
    }
    dwx_add(id,pass,flag);
    printf("注册成功！\n ");
     printf("正在返回.....");
            Sleep(1000);
            system("cls");
}
//将输入的大写字母转化为小写字母
void tolower(char *s)
{
    int len = strlen(s);
    for(int i = 0; i < len; ++i)
    {
        if(s[i] >= 65 && s[i] <= 90) s[i] += 32;
    }
}//sql语句在输入时大小写均可

//读入SQL语句，并进行格式化分离单词， 以分号结束， ESC退出整个程序/
int read()
{
    char c;
    int i;
    for(i = 0; c = getch(); i++)//getch（）不回显函数，当用户按下某个字符时，函数自动读取
    {
        if(c == ';')
            break;
        if(c == 27)//ESC
          exit(0);
        if(c == 8)  //退格
        {
            i -= 2;
            if(i < -1)
                i = -1;
            system("cls");
            cout<<"-->请输入SQL语句:"<<endl;
            for(int j = 0; j <= i; j++)
            {
                if(sqltmp[j] == 13)  //回车
                {
                    puts("");
                }
                cout<<sqltmp[j];
            }
            continue;
        }
        sqltmp[i] = c;
        if(c == 13)//回车
        {
            puts("");
            sql[i] = ' ';
        }
        else if(c== '=' || c == '(' || c == ')' || c == '\'' || c == '\n')
        {
            cout<<c;
            sql[i] = ' ';
        }
        else if(c == ',')
        {
            cout<<c;
            sql[i] = ' ';
            sql[i + 1] = ',';
            sql[i + 2] = ' ';
            i += 2;
        }
        else
        {
            cout<<c;
            sql[i] = c;
        }
    }
    sql[i] = 0;
    printf(";");
    puts("");
    tolower(sql);//提取出sql语句中关键词语，存放至sql数组，并转换成小写。
    return 0;
}

//输出表
void tableprint(Table *t1)//输出表
{
    int m,n;
    printf("表名为：%s\n",t1->t_name);
    printf("\n-----------------------------------------\n");
    for(m=0; m<t1->property_num; m++)
        printf("%s\t",t1->property_name[m]);
    printf("\n-----------------------------------------\n");
    for(m=0; m<t1->record_num; m++)
    {
        for(n=0; n<t1->property_num; n++)
            printf("%s\t",t1->record[m][n]);
        printf("\n\n");
    }
}
////////////////////////////////////////////创建表/////////////////////////////////////////////////////
bool CreateTable()
{
    char tmp[N];//临时存放
    char table_name[N];//表名
    char view_name[N];//视图名
    char index_name[N];//索引名
    bool error = false;
    char type[6][10]= {"char","int","float","double","time","date"};//类型
    stringstream ss(sql);//输入流获取sql语句
    ss >> tmp;//关键字读至tmp
    if(strcmp(tmp,"create") != 0)  //strcmp比较字符串tmp是否与“create”相等，非零不相等
    {
        error = true;
        cout<<"\nThe word 'create' maybe error!"<<endl;
    }

    else
    {
        ss >> tmp;
        if(strcmp(tmp,"table") == 0)//创建表
        {
            Table table;//表结构
            ss >> table_name;//流读入名称
            strcpy(table.t_name,table_name);
         //    user[u_num].t(table.t_name);
            int len = strlen(table_name);
            char s[100];
             strcpy(s,table_name);
             string x="1";
             string x1=s;
             string x2=".txt";
             x1=x+x1+x2;
            const  char *p11=x1.data();
            strcat(table_name, ".txt");    //strcat生成table_name.txt文件
            if(access(table_name, 0) == 0) //access判断文件是否存在
            {
                cout<<"表已经存在，是否覆盖？ y/n"<<endl;
                char c;
                cin>>c;
                if(c == 'y')
                {
                  char *filename=table_name;
                   FILE *fcount;
                  fcount=fopen(filename,"w+");
                   fclose(fcount);
                      FILE *ffp11;
            ffp11=fopen(p11,"a+");
            fclose(ffp11);

                }
                else
                    {
                        printf("创建失败，表已存在！");
                        return false;
                    }
            }
            else
            {
                 char *filename=table_name;
                FILE *fcount;
                fcount=fopen(filename,"w+");
                fclose(fcount);
                FILE *ffp11;
            ffp11=fopen(p11,"a+");
            fclose(ffp11);
            }
            while(true)
            {
                ss >> tmp;
                if(tmp[0] == ',')
                {
                    continue;
                }
                if(strcmp(tmp, ";") == 0)//停止标志
                    break;
                strcpy(table.property_name[table.property_num], tmp);//表名放入表中

                if(!(ss >> tmp))
                    break;
                if(tmp[0] == ',')
                {
                    continue;
                }
                bool flg = false;
                for(int i = 0; i < 6; ++i)//6大类型
                {
                    if(strcmp(tmp,type[i])==0)
                    {
                        flg = true;
                        strcpy(table.property_type[table.property_num], tmp);//匹配成功，放至表中
                        break;
                    }
                }
                if(!flg)//错误判断
                {
                    cout<<"\nThe type maybe wrong (char,int,float,double,time,date)!"<<endl;
                    error = true;
                    break;
                }
                ss >> tmp;
                ss >> tmp;
                if(tmp[0] == ',')
                {
                    continue;
                }
                if(0 == strcmp(tmp,"primary"))
                {
                    char st[N];
                    ss >> st;
                    if( 0 == strcmp(st,"key"))
                    {
                        strcat(tmp, " key");
                        strcpy(table.condition[table.property_num], tmp);//约束条件
                    }
                    else
                    {
                        error = true;
                        cout<<"The word \'key\' maybe error!!"<<endl;
                        break;
                    }
                }
                //约束性条件
                if(strcmp(tmp,"unique")==0)
                {
                    strcpy(table.condition[table.property_num], tmp);
                }
                if(strcmp(tmp,"null")==0)
                {
                    strcpy(table.condition[table.property_num], tmp);
                }
                if(strcmp(tmp,"not")==0)
                {
                    char st[N];
                    ss >> st;
                    if( 0 == strcmp(st,"null"))
                    {
                        strcat(tmp, " null");
                        strcpy(table.condition[table.property_num], tmp);
                    }
                }
                if(strcmp(tmp,"")==0)
                    table.property_num++;//属性个数
                table.property_num++;
                table.record_num=0;   //记录表设置为空
                for(int i=0; i<N; i++)
                    for(int j=0; j<N; j++)
                        for(int k=0; k<N; k++)
                            table.record[i][j][k]= 0; //初始化记录表
            }
            tab[t_num++]=table;//表放到tab中

            /***********将建立的表信息存入文件中*************/
            ofstream table_file;
            table_file.open( table_name, ios::out );//以表名命名文件
            if(table_file.is_open())
            {
                table_file<<"-----------------------------------------"<<endl;
                table_file<<"name        type      condition          "<<endl;
                table_file<<"-----------------------------------------"<<endl;
                 FILE *ffp;
                 string mm="1";
                 mm=mm+table_name;
                 const char *pp=mm.data();
                 ffp=fopen(pp,"a+");
                for (int j=0; j<table.property_num; j++)
                {
                    table_file<<table.property_name[j]<<"    ";
                    if(table.property_type[j] != 0)
                        table_file<<table.property_type[j]<<"    ";
                    else
                        table_file<<"          ";
                    if(table.condition[j] != 0)
                        table_file<<table.condition[j];
                    else
                        table_file<<"                    ";
                    table_file<<endl;
                fprintf(ffp," %s",table.property_name[j]);
                }
                fprintf(ffp,"\n");
            fclose(ffp);
                table_file<<"------------------------------------------------------"<<endl;



            }
            table_file.close();

            //复制到总表中
            FILE *fp1=fopen(table_name, "r");
             FILE *fp2=fopen("table.txt", "at");
            char ch;
           while( (ch=fgetc(fp1) )!= EOF )
                  fputc(ch,fp2);
                    fclose(fp1);
                    fclose(fp2);

        }
        //创建索引
        else if(strcmp(tmp,"index")==0)
        {
            Index index;
            char tt_name[N];
            ss >> index_name;//读入名
            ss >> tmp;//读入信息
            ss >> tt_name;
            if(t_num==0)//表空
            {
                cout<<"   -----------请先创建表！---------"<<endl;
                error = true;
            }
            else
            {

                int kk = 0;
                for(int i=0; i<t_num; i++)
                {
                    strcpy(index.t_name,tt_name);//名字复制
                    if(strcmp(tt_name,tab[i].t_name) == 0)
                    {
                         for(int mm=0;mm<i_num;mm++)
                {
                    if((strcmp(index_name,ind[mm].i_name) == 0)&&(strcmp(ind[mm].t_name,tt_name)==0))
                    {
                        cout<<"创建失败，已存在！"<<endl;
                        return 0;
                    }
                }
                        while(true)
                        {
                            ss >> tmp;
                            cout<<tmp<<endl;
                            if(tmp[0] == ',')
                            {
                                continue;
                            }
                            if(strcmp(tmp, ";") == 0)
                                break;
                            strcpy(index.pro_name[index.pro_num], tmp);
                            index.pro_num++;
                            if(!(ss >> tmp))
                                break;
                        }
                        strcpy(index.i_name,index_name);
                        for(int j=0; j<tab[i].property_num; j++)
                        {
                            if(strcmp(tab[i].condition[j],"primary key") == 0)
                                strcpy(index.key,tab[i].condition[j]);
                        }
                        kk = 1;
                        ind[i_num++]=index;
                        printf("------在表%s上成功建立了索引%s------\n",tt_name,index_name);
                        /**存到文件中*/
                        ofstream index_file;//输出流
                        index_file.open( "index.txt", ios::out );
                        if(index_file.is_open())//打开文件
                        {
                            index_file<<"----------------------------------------------"<<endl;
                            index_file<<"  索引表名："<<index.i_name<<"   索引的主键为："<<ind[i].key<<endl;
                            index_file<<"----------------------------------------------"<<endl;
                            for(int j=0; j<=t_num; j++)
                            {
                                if(strcmp(index.t_name,tab[j].t_name) == 0)
                                {
                                    index_file<<"----------------------------------------------"<<endl;
                                    for(int k=0; k<index.pro_num; k++)
                                        index_file<<index.pro_name[k]<<"     ";

                                    index_file<<"----------------------------------------------"<<endl;
                                    for(int k=0; k<tab[j].record_num; k++)
                                    {
                                        for(int f=0; f<index.pro_num; f++)
                                        {
                                            for(int n=0; n<tab[j].property_num; n++)
                                            {
                                                if(strcmp(tab[i].property_name[n], index.pro_name[f]) == 0)
                                                {
                                                    index_file<<tab[j].record[k][f]<<"      ";

                                                }

                                            }

                                        }
                                        index_file<<endl;
                                    }
                                }
                            }
                        }
                        index_file.close();//关闭文件
                    }
                }
                if(kk == 0)
                {
                    error = true;
                    cout<<"\nThe table do not exist!!"<<endl;
                }
            }
        }
        //创建视图
        else if(strcmp(tmp,"view")==0)
        {
            View view;
            ss >> view.v_name;
            ss>>tmp;
            ss>>tmp;
            ss>>tmp;
            ss>>tmp;
            ss>>view.t_name;
            if(t_num==0)//表空
            {
                cout<<"   -----------请先创建表！---------"<<endl;
                error = true;
            }
            else
            {
                int kk = 0;
                for(int i=0; i<t_num; i++)
                {
                    if(strcmp(view.t_name,tab[i].t_name) == 0)
                    {
                        for(int mm=0;mm<v_num;mm++)
                {
                    if((strcmp(view.v_name,vie[mm].v_name) == 0)&&(strcmp(vie[mm].t_name,view.t_name)==0))
                    {
                        cout<<"创建失败，已存在！"<<endl;
                        return 0;
                    }
                }
                         cout<<"视图创建成功！"<<endl;
                         view.property_num++;
                         vie[v_num++]=view;
                          kk=1;
                    }
                }
                if(kk == 0)
                {
                    error = true;
                    cout<<"\nThe table do not exist!!"<<endl;
                }
            }

        }
    }
    return (!error);
}
//插入数据
bool Insert()
{
    char tmp[N];
    char tmq[N];
    char mn[N];
    bool error = false;
    if(user[now].rinsert == 0)//判断是否有权限
    {
        error = true;
        cout<<"-------对不起，您没有Insert权限！"<<endl;
    }
    else
    {
        puts(sql);
        stringstream ss(sql);//输出流
        ss >> tmp;//存至tmp
        if(strcmp(tmp,"insert") != 0)  //strcmp比较字符串tmp是否与“insert”相等，非零不相等
        {
            error = true;
            cout<<"\nThe word 'insert' maybe error!"<<endl;
        }
        else
        {
            ss >> tmp;
            if(strcmp(tmp,"into") != 0)//错误判断
            {
                error = true;
                cout<<"\nThe word 'into' maybe error!"<<endl;
            }
            else
            {
                int kk=0;
                ss >> tmp;
                for(int i=0; i<t_num; i++)
                {
                    if(strcmp(tmp,tab[i].t_name) == 0)//找到表
                    {
                        strcpy(tmq,tmp);
                        ss >> mn; FILE *ffp;
                         string s=tmp;
                        string s1=".txt";
                        string s2="1";
                        s=s2+s+s1;
                        const  char *p=s.data();
                        ffp=fopen(p,"a+");
                        for(int j=0; j<tab[i].property_num; j++)//进行插入
                        {
                            tab[i].property_name[j];
                            ss >> mn;
                            for(int ii=0; ii<tab[i].record_num;ii++)
                            {
                                if(strcmp(mn,tab[i].record[ii][0]) == 0)
                                {
                                    cout<<"该数据已存在！"<<endl;
                                     fclose(ffp);
                                    return 0;
                                }

                            }
                            strcpy(tab[i].record[tab[i].record_num][j],mn);
                            fprintf(ffp," %s",tab[i].record[tab[i].record_num][j]);
                            ss >> mn;
                        }
                         fprintf(ffp,"\n");
                        fclose(ffp);
                        tab[i].record_num++;//记录++
                        kk=1;//flag
                    }
                }
                if(kk == 0)
                {
                    error = true;
                    cout<<"\nThe table do not exist!!"<<endl;
                }
            }
        }
    }
    return (!error);
}

//删除数据
bool Delete()
{
    char tmp[N];//存放临时输入语句
    char tmq[N];
    char sno[N];
    char snober[N];
    bool error = false;
    if(user[now].rdelete == 0)//权限判断
    {
        error = true;
        cout<<"-------对不起，您没有Delete权限！"<<endl;
    }
    else
    {
        puts(sql);
        stringstream ss(sql);
        ss >> tmp;
        if(strcmp(tmp,"delete") != 0)  //strcmp比较字符串tmp是否与“delete”相等，非零不相等
        {
            error = true;
            cout<<"\nThe word 'delete' maybe error!"<<endl;
        }
        else
        {
            ss >> tmp;
            if(strcmp(tmp,"from") != 0)//检测错误
            {
                error = true;
                cout<<"\nThe word 'from' maybe error!"<<endl;
            }
            else
            {
                int kk=0;
                ss >> tmp;
                for(int i=0; i<t_num; i++)//寻找表
                {
                    if(strcmp(tmp,tab[i].t_name) == 0)//依据表名寻找
                    {
                        ss >> tmp;
                        ss >> sno;
                        cout<<sno<<endl;
                        ss >> snober;
                        cout<<snober<<endl;
                        for(int j=0; j<tab[i].record_num; j++)
                        {
                            for(int k=0; k<tab[i].property_num; k++)
                            {
                                if((strcmp(tab[i].record[j][k],snober) == 0))
                                {
                                    for(; j<tab[i].record_num-1; j++)
                                    {
                                        for(int f=0; f<tab[i].property_num; f++)
                                            strcpy(tab[i].record[j][f],tab[i].record[j+1][f]);
                                    }
                                    tab[i].record_num--;
                                }
                            }
                        }
                        kk=1;
                    }
                }
                if(kk == 0)
                {
                    error = true;
                    cout<<"\nThe table do not exist!!"<<endl;
                }
            }
        }
    }
    return (!error);
}

//更新数据
bool Update()
{
    char tmp[N];
    char tmq[N];
    char mn[N];
    char sno[N];
    char snober[N];
    char snum[N];
    char s[N];
    bool error = false;
    if(user[now].rupdate == 0)
    {
        error = true;
        cout<<"-------对不起，您没有Update权限！"<<endl;
    }
    else
    {
        puts(sql);
        stringstream ss(sql);
        ss >> tmp;
        if(strcmp(tmp,"update") != 0)
        {
            error = true;
            cout<<"\nThe word 'update' maybe error!"<<endl;
        }
        else
        {

            int kk=0;
            ss >> tmp;
            for(int i=0; i<t_num; i++)
            {
                if(strcmp(tmp,tab[i].t_name) == 0)
                {
                    ss >> mn;
                    if(strcmp(mn,"set") != 0)
                    {
                        error = true;
                        cout<<"\nThe word 'set' maybe error!"<<endl;
                    }
                    else
                    {
                        ss >> s;
                        ss >> snum;
                        ss >> mn;
                        ss >> sno;
                        ss >> snober;
                        for(int j=0; j<tab[i].record_num; j++)
                        {
                            for(int k=0; k<tab[i].property_num; k++)
                            {
                                if(strcmp(tab[i].record[j][k],snober) == 0)
                                {
                                    for(int f=0; f<tab[i].property_num; f++)
                                    {
                                        if(strcmp(tab[i].property_name[f],s) == 0)
                                            strcpy(tab[i].record[j][f],snum);
                                    }
                                }
                            }
                        }

                    }
                    kk=1;
                }
                if(kk == 0)
                {
                    error = true;
                    cout<<"\nThe table do not exist!!"<<endl;
                }
            }
        }
    }

    return (!error);
}

//查询数据
bool Select()
{
    char tmp[N];
    char tmq[N];
    char mn[N];
    char pro[N][N];	//将要投影的属性名称集合
    bool error = false;
    if(user[now].rselect == 0)
    {
        error = true;
        cout<<"-------对不起，您没有Select权限！"<<endl;
    }
    else
    {
        puts(sql);
        stringstream ss(sql);
        ss >> tmp;
        if(strcmp(tmp,"select") != 0)  //strcmp比较字符串tmp是否与“select”相等，非零不相等
        {
            error = true;
            cout<<"\nThe word 'select' maybe error!"<<endl;
        }
        else
        {
            int kk = 0;
            ss >> tmp;
            if(strcmp(tmp,"*") == 0)//全部
            {
                int i,j;
                for(i=0; i<t_num; i++)
                {
                    ss >> tmp;
                    ss >> tmp;
                    if(strcmp(tmp,tab[i].t_name) == 0)//表存在
                    {
                        tableprint(&tab[i]);//输出表
                        kk=1;
                        break;
                    }
                }
                for(j=0; j<tab[i].property_num; j++)
                    strcpy(pro[j],tab[i].property_name[j]);
                if(kk == 0)
                {
                    error = true;
                    cout<<"\nThe table do not exist!!"<<endl;
                }
            }
        }
    }

    return (!error);
}

//输出函数
void Help()
{
    char tmp[N];
    stringstream ss(sql);
    ss >> tmp;
    if(strcmp(tmp,"help") != 0)
    {
        cout<<"\nThe word 'help' maybe error!"<<endl;
    }
    else
    {
        ss >> tmp;
        //输出数据表中所有属性的详细信息，包括属性名称、数据类型、约束条件等
        if(strcmp(tmp, "table") == 0)
        {
            ss >> tmp;
            int kk=0;
            if(t_num <= 0)
            {
                cout<<"\n------请先创建表！------\n"<<endl;
            }
            for(int i=0; i<t_num; i++)
            {

                if(strcmp(tab[i].t_name,tmp) == 0)
                {
                    cout<<"表名为： "<<tmp<<endl;
                    cout<<"-----------------------------------------"<<endl;
                    cout<<"name        type      condition          "<<endl;
                    cout<<"-----------------------------------------"<<endl;
                    for (int j=0; j<tab[i].property_num; j++)
                    {
                        printf("%-12s",tab[i].property_name[j]);
                        if(tab[i].property_type[j] != 0)
                            printf("%-10s",tab[i].property_type[j]);
                        else
                            printf("          ");
                        if(tab[i].condition[j] != 0)
                            printf("%-20s",tab[i].condition[j]);
                        else
                            printf("                    ");
                        cout<<endl;
                    }
                    kk=1;
                    cout<<"------------------------------------------"<<endl;
                }
                if(kk==0)
                {
                    cout<<"The table do not exist!!"<<endl;
                    return;
                }
            }
        }
        //输出索引的详细信息
        else if(strcmp(tmp,"index") == 0)
        {
            if(i_num <= 0)
            {
                cout<<"\n------请先创建索引！------\n"<<endl;
            }
            ss >> tmp;
            for(int i=0; i<i_num; i++)
            {

//                if(strcmp(tmp,ind[i].i_name) != 0)
//                {
//                    cout<<"The index do not exist!!"<<endl;
//                    return ;
//                }
//                else
//                {
                       if(strcmp(tmp,ind[i].i_name) == 0)
                       {
                                printf("----------------------------------------------\n");
                    cout<<"  索引表名："<<ind[i].i_name<<"   索引的主键为："<<ind[i].key<<endl;
                    printf("----------------------------------------------\n");
                    for(int j=0; j<=t_num; j++)
                    {
                        if(strcmp(ind[i].t_name,tab[j].t_name) == 0)
                        {
                            printf("-----------------------------------------\n");
                            for(int k=0; k<ind[i].pro_num; k++)
                                printf("%s\t",ind[i].pro_name[k]);
                            printf("\n-----------------------------------------\n");
//                            for(int k=0; k<tab[j].record_num; k++)
//                            {
//                                for(int f=0; f<ind[i].pro_num; f++)
//                                {
//                                    for(int n=0; n<tab[j].property_num; n++)
//                                    {
//                                        if(strcmp(tab[i].property_name[n], ind[i].pro_name[f]) == 0)
//                                        {
//                                            printf("%s\t",tab[j].record[k][f]);
//                                        }
//                                    }
//                                }
//                                cout<<endl;
//                            }
                        }
                    }
                       }

               // }
            }
        }
        //输出视图的定义语句
        else if(strcmp(tmp, "view") == 0)
        {
            if(v_num <= 0)
            {
                cout<<"\n------请先创建视图！------\n"<<endl;
            }
           else
           {
               ss >> tmp;
               int kk=0;
               for(int i=0; i<v_num; i++)
                {
                    if(strcmp(vie[i].v_name,tmp) == 0)
                    {
                        for(int ii=0;ii<t_num;ii++)
                        {
                             if(strcmp(vie[i].t_name,tab[ii].t_name) == 0)
                                cout<<"视图结构"<<endl;
                            cout<<"create view "<<vie[i].v_name<<" as select * from "<<vie[i].t_name<< endl;

                                //tableprint(&tab[ii]);
                                kk=1;
                        }


                    }
                }
              if(kk==0)
              {
                  cout<<"该视图不存在！"<<endl;
              }
           }

        }
        //输出所有数据表、视图和索引的信息，同时显示其对象类型
        else if(strcmp(tmp, "database") == 0)
        {
            for(int i=0; i<t_num; i++)
            {
                cout<<"表名为："<<tab[i].t_name<<endl;
                cout<<"-----------------------------------------"<<endl;
                cout<<"name        type      condition          "<<endl;
                cout<<"-----------------------------------------"<<endl;
                for (int j=0; j<tab[i].property_num; j++)
                {
                    printf("%-12s",tab[i].property_name[j]);
                    if(tab[i].property_type[j] != 0)
                        printf("%-10s",tab[i].property_type[j]);
                    else
                        printf("          ");
                    if(tab[i].condition[j] != 0)
                        printf("%-20s",tab[i].condition[j]);
                    else
                        printf("                    ");
                    cout<<endl;
                }
                cout<<"------------------------------------------"<<endl;

                for(int ii=0; ii<i_num; ii++)
                {
                   if(strcmp(ind[ii].t_name,tab[i].t_name) == 0)
                   {
                       printf("----------------------------------------------\n");
                    cout<<"  索引表名："<<ind[ii].i_name<<"   索引对应表名为： "<<ind[ii].t_name<<endl;
                    cout<<"   索引的主键为："<<ind[ii].key<<endl;
                    printf("----------------------------------------------\n");
                //    for(int j=0; j<=t_num; j++)
                //    {
                      //  if(strcmp(ind[ii].t_name,tab[j].t_name) == 0)
                  //      {
                            for(int k=0; k<ind[ii].pro_num; k++)
                                printf("%s\t",ind[ii].pro_name[k]);
                            printf("\n---------------------------------------------\n");
//                            for(int k=0; k<tab[i].record_num; k++)
//                            {
//                                for(int f=0; f<ind[ii].pro_num; f++)
//                                {
//                                    for(int n=0; n<tab[i].property_num; n++)
//                                    {
//                                        if(strcmp(tab[i].property_name[n], ind[i].pro_name[f]) == 0)
//                                        {
//                                            printf("%s\t",tab[i].record[k][f]);
//                                    }
//                                    }
//                                }
//                                cout<<endl;
//                            }
                            printf("\n---------------------------------------------\n\n");
                   //     }
                  //  }
                   }

                }
                for(int xx=0;xx<v_num;xx++)
                {
                  if(strcmp(vie[xx].t_name,tab[i].t_name) == 0)
                  {
                          cout<<"视图表名为："<<vie[xx].v_name<<"  对应表名为："<<vie[xx].t_name<<endl;
                       // tableprint(&tab[i]);
                          cout<<"create view "<<vie[xx].v_name<<" as select * from "<<vie[xx].t_name<< endl;
                  }

                }
                }
            }
    }
}
//授权
bool Grant()
{
    int ff=0;
    bool error=false;
    if(user[now].rgrant == 0)
    {
        cout<<"-------对不起，您没有Grant权限！"<<endl;
        return false;
    }
    else
    {
    char tmp[N];
    char ta[N];
    char use[N];
    char oper[N];
    puts(sql);
    stringstream ss(sql);
    ss >> tmp;
    if(strcmp(tmp,"grant") != 0)
    {
        cout<<"\nThe word 'grant' maybe error!"<<endl;
    }
    else
    {
        ss >> oper;
        ss >> tmp;
        ss >> tmp;
        ss >> ta;
        ss >> tmp;
        ss >> use;
        if(strcmp(oper,"select") == 0)
        {
            for(int i=0; i<u_num; i++)
            {

                if(user[i].n() == use)
                {    user[i].rselect = 1;
                      char pass[20];
                      strcpy(pass,user[i].p().c_str());
                       ff=1;
                    add_grant(use,pass,ff);
                    cout<<"------Select授权成功！------"<<endl;
                    return true;
                }
            }
        }
        if(strcmp(oper,"insert") == 0)
        {
            for(int i=0; i<u_num; i++)
            {
                if(user[i].n() == use)
                {
                    user[i].rinsert = 1;
                     char pass[20];
                    strcpy(pass,user[i].p().c_str());
                    user[i].rselect = 1;
                    ff=4;
                    add_grant(use,pass,ff);
                    cout<<"------Insert授权成功！------"<<endl;
                    return true;
                }
            }
        }
        if(strcmp(oper,"delete") == 0)
        {
            for(int i=0; i<u_num; i++)
            {
                if(user[i].n() == use)
                {
                    user[i].rdelete = 1;
                    char pass[20];
                    strcpy(pass,user[i].p().c_str());
                     ff=2;
                    add_grant(use,pass,ff);
                    cout<<"------Delete授权成功！------"<<endl;
                    return true;
                }
            }
        }
        if(strcmp(oper,"update") == 0)
        {
            for(int i=0; i<u_num; i++)
            {
                if(user[i].n() == use)
                {
                    user[i].rupdate = 1;
                    char pass[20];
                    strcpy(pass,user[i].p().c_str());
                       ff=3;
                    add_grant(use,pass,ff);
                    cout<<"------Update授权成功！------"<<endl;
                    return true;
                }
            }
        }
    }
    }
    return error;
}
//取消权利
bool Revoke()
{
    bool error=false;
    int ff;//选择更新权限类型
  if(user[now].rflag == 0)
    {
        cout<<"-------对不起，您没有Revoke权限！"<<endl;
        return false;
    }
    else
    {
         char tmp[N];
    char ta[N];
    char use[N];
    char oper[N];
    puts(sql);
    stringstream ss(sql);
    ss >> tmp;
    if(strcmp(tmp,"revoke") != 0)
    {
        cout<<"\nThe word 'revoke' maybe error!"<<endl;
    }
    else
    {
        ss >> oper;
        ss >> tmp;
        ss >> tmp;
        ss >> ta;
        ss >> tmp;
        ss >> use;
        if(strcmp(oper,"select") == 0)
        {
            for(int i=0; i<u_num; i++)
            {
                if(user[i].n() == use)
                {
                    user[i].rselect = 0;
                    char pass[20];
                    strcpy(pass,user[i].p().c_str());
                    ff=5;//方便更改本地权限flag
                    add_grant(use,pass,ff);
                    cout<<"------取消Select权利成功！------"<<endl;
                    return true;
                }
            }
        }
        if(strcmp(oper,"insert") == 0)
        {
            for(int i=0; i<u_num; i++)
            {
                if(user[i].n() == use)
                {
                    user[i].rinsert = 0;
                    char pass[20];
                    strcpy(pass,user[i].p().c_str());
                       ff=6;
                    add_grant(use,pass,ff);
                    cout<<"------取消Insert权利成功！------"<<endl;
                    return true;
                }
            }
        }
        if(strcmp(oper,"delete") == 0)
        {
            for(int i=0; i<u_num; i++)
            {
                if(user[i].n() == use)
                {
                    user[i].rdelete = 0;
                    char pass[20];
                    strcpy(pass,user[i].p().c_str());
                        ff=7;
                    add_grant(use,pass,ff);
                    cout<<"------取消Delete权利成功！------"<<endl;
                    return true;
                }
            }
        }
        if(strcmp(oper,"update") == 0)
        {
            for(int i=0; i<u_num; i++)
            {
                if(user[i].n() == use)//查找用户
                {
                    user[i].rupdate = 0;
                    char pass[20];
                    strcpy(pass,user[i].p().c_str());
                                ff=8;
                    add_grant(use,pass,ff);
                    cout<<"------取消Update权利成功！------"<<endl;
                    return true;
                }
            }
        }
    }
    }
   return false;
}

//登录选择页面
void StartUI()
{
    system("color 84");//控制背景和字体颜色
      //system("cls");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("||                          欢迎                          ||\n");
    printf("||--------------------------------------------------------||\n");
    printf("||--------------------------------------------------------||\n");
    printf("||                       登陆请输入1                      ||\n");
    printf("||--------------------------------------------------------||\n");
    printf("||        管理员注册请输入2     普通用户注册请输入3       ||\n");
    printf("||                       退出请输入4                      ||\n");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

//功能选择页面
void menu1()
{
    cout<<(" =======================================================")<<endl;
    cout<<("|                          欢迎！                       |")<<endl;
    cout<<("|                                                       |")<<endl;
    cout<<("|                      请按提示操作                     |")<<endl;
    cout<<("|                                                       |")<<endl;
    cout<<("|    1.（新建表）Create Table  5.（删除）Delete         |")<<endl;
    cout<<("|    2.（帮助）HELP            6.（更新）Update         |")<<endl;
    cout<<("|    3.（插入）Insert          7.（添加权限）Grant      |")<<endl;
    cout<<("|    4.（查询）Select          8.（移除权限）Revoke     |")<<endl;
    cout<<("|                     0.退出exit                        |")<<endl;
    cout<<(" =======================================================")<<endl;
}

void menu2()
{
     system("cls");
    cout<<(" ====================================================")<<endl;
    cout<<("|                                                   |")<<endl;
    cout<<("|           对不起，您操作错误，请检查您的输入      |")<<endl;
    cout<<("|                                                   |")<<endl;
    cout<<(" ====================================================")<<endl;
}
//菜单操作
void selectOperation()
{
    string option;
    while(cin>>option)
    {
        if(option=="0")
        {
              system("cls");
             return ;

        }

        system("cls");
        cout<<"-->请输入SQL语句:"<<endl;
        read();
        stringstream str_s(sql);
        str_s>>tmp;
        //建立表
        if(option=="1")
        {
            str_s >> tmp;
            if(strcmp(tmp,"table")==0)
            {
                if(CreateTable())
                    cout<<"------------建表成功！-------------"<<endl;
                else
                    cout<<"------------建表失败！-------------"<<endl;
                menu1();
            }
            else if(strcmp(tmp,"index")==0)
            {
                if(CreateTable())
                    cout<<"------------索引创建成功！-------------"<<endl;
                else
                    cout<<"------------索引创建失败！-------------"<<endl;
                menu1();
            }
            else if(strcmp(tmp,"view")==0)
            {
                if(CreateTable())
                    cout<<"------------视图创建成功！-------------"<<endl;
                else
                    cout<<"------------视图创建失败！-------------"<<endl;
                menu1();
            }
        }
        //显示
        if(option=="2")
        {
           Help();
            menu1();
        }
        //插入数据
        if(option=="3")
        {
            if(Insert())
                cout<<"------------数据插入成功！-------------"<<endl;
            else
                cout<<"------------数据插入失败！-------------"<<endl;
            menu1();
        }
        //查询数据
        if(option=="4")
        {
            if(Select())
                cout<<"------------数据查询成功！-------------"<<endl;
            else
                cout<<"------------数据查询失败！-------------"<<endl;
            menu1();
        }
        //删除数据
        if(option=="5")
        {
            if(Delete())
                cout<<"------------数据删除成功！-------------"<<endl;
            else
                cout<<"------------数据删除失败！-------------"<<endl;
            menu1();
        }
        //更新数据
        if(option=="6")
        {
            if(Update())
                cout<<"------------数据更新成功！-------------"<<endl;
            else
                cout<<"------------数据更新失败！-------------"<<endl;
            menu1();
        }
        //授权
        if(option=="7")
        {
            if(Grant())
                cout<<"------------授权成功！-------------"<<endl;
            else
                cout<<"------------授权失败！-------------"<<endl;
            menu1();
        }
        //取消权利
        if(option=="8")
        {
             if(Revoke())
                cout<<"------------取消授权成功！-------------"<<endl;
            else
                cout<<"------------取消授权失败！------------"<<endl;
            menu1();
        }
    }
}
int main()
{

     HWND hWnd = GetConsoleWindow(); //获得cmd窗口句柄
	RECT rc;
	GetWindowRect(hWnd, &rc); //获得cmd窗口对应矩形

	//改变cmd窗口风格
	SetWindowLongPtr(hWnd,
		GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
	//因为风格涉及到边框改变，必须调用SetWindowPos，否则无效果
SetWindowPos(hWnd,
		NULL,
		100,
		20,
		530,
		490,
		NULL);

    StartUI();
    int select1;
    int command;
    int flag=1;//panduan创建用户类型。
    readuser();//初始化user 同步数据
    while(cin>>select1)
    {
        if(select1==1)//登录
        {
            if (!dwx_login())
                {
                    printf("用户不存在或者密码错误!\n\n");
                }
                else
                {
                    menu1();
                    selectOperation();
                }
        }
        else   if(select1==2)//管理员用户注册
        {
             dwx_register(1);
        }

        else   if(select1==3)//普通用户注册
         dwx_register(0);
        else if(select1==4)//退出
            return 0;
       else
        {
            cout<<endl;
            menu2();
        }

         StartUI();
    }
}

