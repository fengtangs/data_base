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
extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();//�̶�����
#define MAX 1000
#define N 100
using namespace std;
char username[N];//�û���
char tmp[100];
char sql[MAX],sqltmp[MAX];//���sql����֮��ؼ���䡣sqltmp��ʱ���
int t_num=0;    //�������
int i_num=0;    //����������
int v_num=0;    //��ͼ������
int u_num=0;    //�û��ĸ���
int now;//Ŀǰ�û���ţ�
//�û�
class users
{
private:
    string name;
    string password;
    string table;//������
public://Ȩ��˵��

    int revoke;
    int rselect;
    int rdelete;
    int rupdate;
    int rinsert;
    int rgrant;
    int rflag;
    users():revoke(1),rselect(1),rdelete(1),rupdate(1),rinsert(1),rgrant(1),rflag(1)
    {}//��ʼ��
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
users user[100];//����û�
//��
struct Table
{
    char t_name[N];              //����
    char property_name[N][N];    //������
    char property_type[N][N];    //��������
    int property_len;            //���Գ���
    int property_num;           //���Եĸ���
    char condition[N][N];           //����Լ������
    int record_num;              //��¼�ĸ���

    char record[N][N][N];		//��ά��,����������Ǽ�¼��ֵ
    Table()             //��ʼ��
    {
        property_len = record_num = property_num = 0;
        memset(t_name,0,sizeof(t_name));
        memset(property_name,0,sizeof(property_name));
        memset(property_type,0,sizeof(property_type));
        memset(condition,0,sizeof(condition));
    }
};

//�����ṹ
struct Index
{
    char i_name[N];   //������
    char t_name[N];   //��������Ӧ�ı���
    char key[N];       //
    char pro_name[N][N];
    int  pro_num;//����
    Index()  //��ʼ��
    {
        pro_num = 0;
         memset(i_name,0,sizeof(i_name));
          memset(t_name,0,sizeof(t_name));
           memset(pro_name,0,sizeof(pro_name));
    }
};

//��ͼ�ṹ
struct View
{
    char proname[N][N];	//�ɼ�������
	char sql[100];      //sql���
    char v_name[N];   //��ͼ��
    char t_name[N];   //��ͼ����Ӧ�ı���
    int property_num; //���Եĸ���
     View()  //��ʼ��
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
//����Ȩ�޵����أ�
void  add_grant(char id[],char pass[],int f)
{
	 FILE *tf, *nf;
    tf = fopen("data.txt", "rb");//����û�
    nf = fopen("datatemp.txt", "a+");//��ʱ�ļ�
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
//��ʼ��user���飬����������ͬ���������У�
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
//������ע���˻��ĺ���
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
//�˺�ע�����
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
//ϵͳ��¼����
bool dwx_login()
{
    char id[10],pass[10];
    printf("+*-*+*-*+*-*��ӭ��¼DBMSϵͳ+*-*+*-*+*-*\n\n");
    printf("�������˺ţ�");
    scanf("%s",id);
    printf("\n");
    printf("���������룺");
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
        printf("\n��½�ɹ�\n\n");
        printf("\n������ת........\n\n");
        Sleep(800);

         system("cls");
        return true;
    }
    else
    {
        return false;
    }
}
//����û��浽����,����user
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
//����Աע������ͨ�û�ע��
void dwx_register(int flag)
{
    int x=0;
    string Pass,Repass;
    char id[10],pass[10],repass[10];
    system("cls");
    if(flag)
    {
        printf("         +*-*+*-*+*-*��ӭע�����Ա+*-*+*-*+*-*\n");
        printf("       +*-*+*-*+*-*������ע�����Աͨ��֤+*-*+*-*+*-*\n");
        printf("ͨ��֤��");
        int m;
        scanf("%d",&m);
        if(m!=2019)//������ͨ��֤���롣
        {
            printf("*------------�Բ�������������-------------\n\n");
            printf("���ڷ���.....");
            Sleep(1500);
            system("cls");
            return ;
        }
    }
    else
        printf("+*-*+*-*+*-*ע����ͨ�û�+*-*+*-*+*-*\n");
    printf("\n");
    printf("�������˺ţ�");
    scanf("%s",id);
    printf("\n");
    while(true)
    {

        printf("��ʾ��������Ҫʮ������Ӣ����ĸ�����������֡��Լ�Ӣ�ķ��š�\n");
    char ch;
    int i = 0;
    printf("���������룺");
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
    printf("���ٴ�ȷ�����룺");
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
            printf("��������������벻һ�£�������ȷ�ϣ�\n");
            printf("\n");
            printf("���ڷ���.....");
            Sleep(1000);
            system("cls");

        }
        else if(search2(id,Pass)==1)
        {
             printf("�˺��Ѵ��ڣ�����������û�������\n");
             printf("���ڷ���.....");
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
    printf("ע��ɹ���\n ");
     printf("���ڷ���.....");
            Sleep(1000);
            system("cls");
}
//������Ĵ�д��ĸת��ΪСд��ĸ
void tolower(char *s)
{
    int len = strlen(s);
    for(int i = 0; i < len; ++i)
    {
        if(s[i] >= 65 && s[i] <= 90) s[i] += 32;
    }
}//sql���������ʱ��Сд����

//����SQL��䣬�����и�ʽ�����뵥�ʣ� �ԷֺŽ����� ESC�˳���������/
int read()
{
    char c;
    int i;
    for(i = 0; c = getch(); i++)//getch���������Ժ��������û�����ĳ���ַ�ʱ�������Զ���ȡ
    {
        if(c == ';')
            break;
        if(c == 27)//ESC
          exit(0);
        if(c == 8)  //�˸�
        {
            i -= 2;
            if(i < -1)
                i = -1;
            system("cls");
            cout<<"-->������SQL���:"<<endl;
            for(int j = 0; j <= i; j++)
            {
                if(sqltmp[j] == 13)  //�س�
                {
                    puts("");
                }
                cout<<sqltmp[j];
            }
            continue;
        }
        sqltmp[i] = c;
        if(c == 13)//�س�
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
    tolower(sql);//��ȡ��sql����йؼ���������sql���飬��ת����Сд��
    return 0;
}

//�����
void tableprint(Table *t1)//�����
{
    int m,n;
    printf("����Ϊ��%s\n",t1->t_name);
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
////////////////////////////////////////////������/////////////////////////////////////////////////////
bool CreateTable()
{
    char tmp[N];//��ʱ���
    char table_name[N];//����
    char view_name[N];//��ͼ��
    char index_name[N];//������
    bool error = false;
    char type[6][10]= {"char","int","float","double","time","date"};//����
    stringstream ss(sql);//��������ȡsql���
    ss >> tmp;//�ؼ��ֶ���tmp
    if(strcmp(tmp,"create") != 0)  //strcmp�Ƚ��ַ���tmp�Ƿ��롰create����ȣ����㲻���
    {
        error = true;
        cout<<"\nThe word 'create' maybe error!"<<endl;
    }

    else
    {
        ss >> tmp;
        if(strcmp(tmp,"table") == 0)//������
        {
            Table table;//��ṹ
            ss >> table_name;//����������
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
            strcat(table_name, ".txt");    //strcat����table_name.txt�ļ�
            if(access(table_name, 0) == 0) //access�ж��ļ��Ƿ����
            {
                cout<<"���Ѿ����ڣ��Ƿ񸲸ǣ� y/n"<<endl;
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
                        printf("����ʧ�ܣ����Ѵ��ڣ�");
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
                if(strcmp(tmp, ";") == 0)//ֹͣ��־
                    break;
                strcpy(table.property_name[table.property_num], tmp);//�����������

                if(!(ss >> tmp))
                    break;
                if(tmp[0] == ',')
                {
                    continue;
                }
                bool flg = false;
                for(int i = 0; i < 6; ++i)//6������
                {
                    if(strcmp(tmp,type[i])==0)
                    {
                        flg = true;
                        strcpy(table.property_type[table.property_num], tmp);//ƥ��ɹ�����������
                        break;
                    }
                }
                if(!flg)//�����ж�
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
                        strcpy(table.condition[table.property_num], tmp);//Լ������
                    }
                    else
                    {
                        error = true;
                        cout<<"The word \'key\' maybe error!!"<<endl;
                        break;
                    }
                }
                //Լ��������
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
                    table.property_num++;//���Ը���
                table.property_num++;
                table.record_num=0;   //��¼������Ϊ��
                for(int i=0; i<N; i++)
                    for(int j=0; j<N; j++)
                        for(int k=0; k<N; k++)
                            table.record[i][j][k]= 0; //��ʼ����¼��
            }
            tab[t_num++]=table;//��ŵ�tab��

            /***********�������ı���Ϣ�����ļ���*************/
            ofstream table_file;
            table_file.open( table_name, ios::out );//�Ա��������ļ�
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

            //���Ƶ��ܱ���
            FILE *fp1=fopen(table_name, "r");
             FILE *fp2=fopen("table.txt", "at");
            char ch;
           while( (ch=fgetc(fp1) )!= EOF )
                  fputc(ch,fp2);
                    fclose(fp1);
                    fclose(fp2);

        }
        //��������
        else if(strcmp(tmp,"index")==0)
        {
            Index index;
            char tt_name[N];
            ss >> index_name;//������
            ss >> tmp;//������Ϣ
            ss >> tt_name;
            if(t_num==0)//���
            {
                cout<<"   -----------���ȴ�����---------"<<endl;
                error = true;
            }
            else
            {

                int kk = 0;
                for(int i=0; i<t_num; i++)
                {
                    strcpy(index.t_name,tt_name);//���ָ���
                    if(strcmp(tt_name,tab[i].t_name) == 0)
                    {
                         for(int mm=0;mm<i_num;mm++)
                {
                    if((strcmp(index_name,ind[mm].i_name) == 0)&&(strcmp(ind[mm].t_name,tt_name)==0))
                    {
                        cout<<"����ʧ�ܣ��Ѵ��ڣ�"<<endl;
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
                        printf("------�ڱ�%s�ϳɹ�����������%s------\n",tt_name,index_name);
                        /**�浽�ļ���*/
                        ofstream index_file;//�����
                        index_file.open( "index.txt", ios::out );
                        if(index_file.is_open())//���ļ�
                        {
                            index_file<<"----------------------------------------------"<<endl;
                            index_file<<"  ����������"<<index.i_name<<"   ����������Ϊ��"<<ind[i].key<<endl;
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
                        index_file.close();//�ر��ļ�
                    }
                }
                if(kk == 0)
                {
                    error = true;
                    cout<<"\nThe table do not exist!!"<<endl;
                }
            }
        }
        //������ͼ
        else if(strcmp(tmp,"view")==0)
        {
            View view;
            ss >> view.v_name;
            ss>>tmp;
            ss>>tmp;
            ss>>tmp;
            ss>>tmp;
            ss>>view.t_name;
            if(t_num==0)//���
            {
                cout<<"   -----------���ȴ�����---------"<<endl;
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
                        cout<<"����ʧ�ܣ��Ѵ��ڣ�"<<endl;
                        return 0;
                    }
                }
                         cout<<"��ͼ�����ɹ���"<<endl;
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
//��������
bool Insert()
{
    char tmp[N];
    char tmq[N];
    char mn[N];
    bool error = false;
    if(user[now].rinsert == 0)//�ж��Ƿ���Ȩ��
    {
        error = true;
        cout<<"-------�Բ�����û��InsertȨ�ޣ�"<<endl;
    }
    else
    {
        puts(sql);
        stringstream ss(sql);//�����
        ss >> tmp;//����tmp
        if(strcmp(tmp,"insert") != 0)  //strcmp�Ƚ��ַ���tmp�Ƿ��롰insert����ȣ����㲻���
        {
            error = true;
            cout<<"\nThe word 'insert' maybe error!"<<endl;
        }
        else
        {
            ss >> tmp;
            if(strcmp(tmp,"into") != 0)//�����ж�
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
                    if(strcmp(tmp,tab[i].t_name) == 0)//�ҵ���
                    {
                        strcpy(tmq,tmp);
                        ss >> mn; FILE *ffp;
                         string s=tmp;
                        string s1=".txt";
                        string s2="1";
                        s=s2+s+s1;
                        const  char *p=s.data();
                        ffp=fopen(p,"a+");
                        for(int j=0; j<tab[i].property_num; j++)//���в���
                        {
                            tab[i].property_name[j];
                            ss >> mn;
                            for(int ii=0; ii<tab[i].record_num;ii++)
                            {
                                if(strcmp(mn,tab[i].record[ii][0]) == 0)
                                {
                                    cout<<"�������Ѵ��ڣ�"<<endl;
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
                        tab[i].record_num++;//��¼++
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

//ɾ������
bool Delete()
{
    char tmp[N];//�����ʱ�������
    char tmq[N];
    char sno[N];
    char snober[N];
    bool error = false;
    if(user[now].rdelete == 0)//Ȩ���ж�
    {
        error = true;
        cout<<"-------�Բ�����û��DeleteȨ�ޣ�"<<endl;
    }
    else
    {
        puts(sql);
        stringstream ss(sql);
        ss >> tmp;
        if(strcmp(tmp,"delete") != 0)  //strcmp�Ƚ��ַ���tmp�Ƿ��롰delete����ȣ����㲻���
        {
            error = true;
            cout<<"\nThe word 'delete' maybe error!"<<endl;
        }
        else
        {
            ss >> tmp;
            if(strcmp(tmp,"from") != 0)//������
            {
                error = true;
                cout<<"\nThe word 'from' maybe error!"<<endl;
            }
            else
            {
                int kk=0;
                ss >> tmp;
                for(int i=0; i<t_num; i++)//Ѱ�ұ�
                {
                    if(strcmp(tmp,tab[i].t_name) == 0)//���ݱ���Ѱ��
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

//��������
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
        cout<<"-------�Բ�����û��UpdateȨ�ޣ�"<<endl;
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

//��ѯ����
bool Select()
{
    char tmp[N];
    char tmq[N];
    char mn[N];
    char pro[N][N];	//��ҪͶӰ���������Ƽ���
    bool error = false;
    if(user[now].rselect == 0)
    {
        error = true;
        cout<<"-------�Բ�����û��SelectȨ�ޣ�"<<endl;
    }
    else
    {
        puts(sql);
        stringstream ss(sql);
        ss >> tmp;
        if(strcmp(tmp,"select") != 0)  //strcmp�Ƚ��ַ���tmp�Ƿ��롰select����ȣ����㲻���
        {
            error = true;
            cout<<"\nThe word 'select' maybe error!"<<endl;
        }
        else
        {
            int kk = 0;
            ss >> tmp;
            if(strcmp(tmp,"*") == 0)//ȫ��
            {
                int i,j;
                for(i=0; i<t_num; i++)
                {
                    ss >> tmp;
                    ss >> tmp;
                    if(strcmp(tmp,tab[i].t_name) == 0)//�����
                    {
                        tableprint(&tab[i]);//�����
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

//�������
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
        //������ݱ����������Ե���ϸ��Ϣ�������������ơ��������͡�Լ��������
        if(strcmp(tmp, "table") == 0)
        {
            ss >> tmp;
            int kk=0;
            if(t_num <= 0)
            {
                cout<<"\n------���ȴ�����------\n"<<endl;
            }
            for(int i=0; i<t_num; i++)
            {

                if(strcmp(tab[i].t_name,tmp) == 0)
                {
                    cout<<"����Ϊ�� "<<tmp<<endl;
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
        //�����������ϸ��Ϣ
        else if(strcmp(tmp,"index") == 0)
        {
            if(i_num <= 0)
            {
                cout<<"\n------���ȴ���������------\n"<<endl;
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
                    cout<<"  ����������"<<ind[i].i_name<<"   ����������Ϊ��"<<ind[i].key<<endl;
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
        //�����ͼ�Ķ������
        else if(strcmp(tmp, "view") == 0)
        {
            if(v_num <= 0)
            {
                cout<<"\n------���ȴ�����ͼ��------\n"<<endl;
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
                                cout<<"��ͼ�ṹ"<<endl;
                            cout<<"create view "<<vie[i].v_name<<" as select * from "<<vie[i].t_name<< endl;

                                //tableprint(&tab[ii]);
                                kk=1;
                        }


                    }
                }
              if(kk==0)
              {
                  cout<<"����ͼ�����ڣ�"<<endl;
              }
           }

        }
        //����������ݱ���ͼ����������Ϣ��ͬʱ��ʾ���������
        else if(strcmp(tmp, "database") == 0)
        {
            for(int i=0; i<t_num; i++)
            {
                cout<<"����Ϊ��"<<tab[i].t_name<<endl;
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
                    cout<<"  ����������"<<ind[ii].i_name<<"   ������Ӧ����Ϊ�� "<<ind[ii].t_name<<endl;
                    cout<<"   ����������Ϊ��"<<ind[ii].key<<endl;
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
                          cout<<"��ͼ����Ϊ��"<<vie[xx].v_name<<"  ��Ӧ����Ϊ��"<<vie[xx].t_name<<endl;
                       // tableprint(&tab[i]);
                          cout<<"create view "<<vie[xx].v_name<<" as select * from "<<vie[xx].t_name<< endl;
                  }

                }
                }
            }
    }
}
//��Ȩ
bool Grant()
{
    int ff=0;
    bool error=false;
    if(user[now].rgrant == 0)
    {
        cout<<"-------�Բ�����û��GrantȨ�ޣ�"<<endl;
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
                    cout<<"------Select��Ȩ�ɹ���------"<<endl;
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
                    cout<<"------Insert��Ȩ�ɹ���------"<<endl;
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
                    cout<<"------Delete��Ȩ�ɹ���------"<<endl;
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
                    cout<<"------Update��Ȩ�ɹ���------"<<endl;
                    return true;
                }
            }
        }
    }
    }
    return error;
}
//ȡ��Ȩ��
bool Revoke()
{
    bool error=false;
    int ff;//ѡ�����Ȩ������
  if(user[now].rflag == 0)
    {
        cout<<"-------�Բ�����û��RevokeȨ�ޣ�"<<endl;
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
                    ff=5;//������ı���Ȩ��flag
                    add_grant(use,pass,ff);
                    cout<<"------ȡ��SelectȨ���ɹ���------"<<endl;
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
                    cout<<"------ȡ��InsertȨ���ɹ���------"<<endl;
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
                    cout<<"------ȡ��DeleteȨ���ɹ���------"<<endl;
                    return true;
                }
            }
        }
        if(strcmp(oper,"update") == 0)
        {
            for(int i=0; i<u_num; i++)
            {
                if(user[i].n() == use)//�����û�
                {
                    user[i].rupdate = 0;
                    char pass[20];
                    strcpy(pass,user[i].p().c_str());
                                ff=8;
                    add_grant(use,pass,ff);
                    cout<<"------ȡ��UpdateȨ���ɹ���------"<<endl;
                    return true;
                }
            }
        }
    }
    }
   return false;
}

//��¼ѡ��ҳ��
void StartUI()
{
    system("color 84");//���Ʊ�����������ɫ
      //system("cls");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("||                          ��ӭ                          ||\n");
    printf("||--------------------------------------------------------||\n");
    printf("||--------------------------------------------------------||\n");
    printf("||                       ��½������1                      ||\n");
    printf("||--------------------------------------------------------||\n");
    printf("||        ����Աע��������2     ��ͨ�û�ע��������3       ||\n");
    printf("||                       �˳�������4                      ||\n");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

//����ѡ��ҳ��
void menu1()
{
    cout<<(" =======================================================")<<endl;
    cout<<("|                          ��ӭ��                       |")<<endl;
    cout<<("|                                                       |")<<endl;
    cout<<("|                      �밴��ʾ����                     |")<<endl;
    cout<<("|                                                       |")<<endl;
    cout<<("|    1.���½���Create Table  5.��ɾ����Delete         |")<<endl;
    cout<<("|    2.��������HELP            6.�����£�Update         |")<<endl;
    cout<<("|    3.�����룩Insert          7.�����Ȩ�ޣ�Grant      |")<<endl;
    cout<<("|    4.����ѯ��Select          8.���Ƴ�Ȩ�ޣ�Revoke     |")<<endl;
    cout<<("|                     0.�˳�exit                        |")<<endl;
    cout<<(" =======================================================")<<endl;
}

void menu2()
{
     system("cls");
    cout<<(" ====================================================")<<endl;
    cout<<("|                                                   |")<<endl;
    cout<<("|           �Բ�������������������������      |")<<endl;
    cout<<("|                                                   |")<<endl;
    cout<<(" ====================================================")<<endl;
}
//�˵�����
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
        cout<<"-->������SQL���:"<<endl;
        read();
        stringstream str_s(sql);
        str_s>>tmp;
        //������
        if(option=="1")
        {
            str_s >> tmp;
            if(strcmp(tmp,"table")==0)
            {
                if(CreateTable())
                    cout<<"------------����ɹ���-------------"<<endl;
                else
                    cout<<"------------����ʧ�ܣ�-------------"<<endl;
                menu1();
            }
            else if(strcmp(tmp,"index")==0)
            {
                if(CreateTable())
                    cout<<"------------���������ɹ���-------------"<<endl;
                else
                    cout<<"------------��������ʧ�ܣ�-------------"<<endl;
                menu1();
            }
            else if(strcmp(tmp,"view")==0)
            {
                if(CreateTable())
                    cout<<"------------��ͼ�����ɹ���-------------"<<endl;
                else
                    cout<<"------------��ͼ����ʧ�ܣ�-------------"<<endl;
                menu1();
            }
        }
        //��ʾ
        if(option=="2")
        {
           Help();
            menu1();
        }
        //��������
        if(option=="3")
        {
            if(Insert())
                cout<<"------------���ݲ���ɹ���-------------"<<endl;
            else
                cout<<"------------���ݲ���ʧ�ܣ�-------------"<<endl;
            menu1();
        }
        //��ѯ����
        if(option=="4")
        {
            if(Select())
                cout<<"------------���ݲ�ѯ�ɹ���-------------"<<endl;
            else
                cout<<"------------���ݲ�ѯʧ�ܣ�-------------"<<endl;
            menu1();
        }
        //ɾ������
        if(option=="5")
        {
            if(Delete())
                cout<<"------------����ɾ���ɹ���-------------"<<endl;
            else
                cout<<"------------����ɾ��ʧ�ܣ�-------------"<<endl;
            menu1();
        }
        //��������
        if(option=="6")
        {
            if(Update())
                cout<<"------------���ݸ��³ɹ���-------------"<<endl;
            else
                cout<<"------------���ݸ���ʧ�ܣ�-------------"<<endl;
            menu1();
        }
        //��Ȩ
        if(option=="7")
        {
            if(Grant())
                cout<<"------------��Ȩ�ɹ���-------------"<<endl;
            else
                cout<<"------------��Ȩʧ�ܣ�-------------"<<endl;
            menu1();
        }
        //ȡ��Ȩ��
        if(option=="8")
        {
             if(Revoke())
                cout<<"------------ȡ����Ȩ�ɹ���-------------"<<endl;
            else
                cout<<"------------ȡ����Ȩʧ�ܣ�------------"<<endl;
            menu1();
        }
    }
}
int main()
{

     HWND hWnd = GetConsoleWindow(); //���cmd���ھ��
	RECT rc;
	GetWindowRect(hWnd, &rc); //���cmd���ڶ�Ӧ����

	//�ı�cmd���ڷ��
	SetWindowLongPtr(hWnd,
		GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
	//��Ϊ����漰���߿�ı䣬�������SetWindowPos��������Ч��
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
    int flag=1;//panduan�����û����͡�
    readuser();//��ʼ��user ͬ������
    while(cin>>select1)
    {
        if(select1==1)//��¼
        {
            if (!dwx_login())
                {
                    printf("�û������ڻ����������!\n\n");
                }
                else
                {
                    menu1();
                    selectOperation();
                }
        }
        else   if(select1==2)//����Ա�û�ע��
        {
             dwx_register(1);
        }

        else   if(select1==3)//��ͨ�û�ע��
         dwx_register(0);
        else if(select1==4)//�˳�
            return 0;
       else
        {
            cout<<endl;
            menu2();
        }

         StartUI();
    }
}

