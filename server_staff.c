#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#define ERR_MSG(msg) do{\
	fprintf(stderr, "__%d__ ", __LINE__);\
	perror(msg);\
}while(0)
struct net_work{
	int newfd;
	struct sockaddr_in cin;
};
struct staff{
	int type;
	int number;
	char name[128];
	char sex[20];
	char age[5];
	char id[19];
	char phone[12];
	char department[64];
	char pay[20];
	char username[20];
	char password[20];
	char power[5];
	char buf[128];
}info;
#define PORT 8989
//网络连接
int network();
//注册
int reg();
//登陆
int log_on();
//查询
int inquire();
//增加
int add();
//删除
int del();
//修改
int update();
//初始化数据库
int init();

void* rev_cli_net_work(void* arg);
int main(int argc, const char *argv[])
{
	init();
	network();
	return 0;
}
int init(){
	sqlite3 *db=NULL;
	if(sqlite3_open("./staff.db",&db)!=SQLITE_OK){
		printf("数据库打开失败\n");
		printf("%s\n",sqlite3_errmsg(db));
		printf("%d\n",sqlite3_errcode(db));
	}
	printf("sqlite3_open success\n");

	//创建员工信息表
	char sql[256]="create table if not exists staff (number int primary key ,name char,sex int ,age int,id char ,phone char ,department char , pay int);";
	char* errmsg=NULL;
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK){
		fprintf(stderr,"__%d__sqlite3_exec:%s\n",__LINE__,errmsg);
		return -1;
	}
	printf("sqlite3_exec create table success");

}
//修改
int update(){
	sqlite3 *db=NULL;
	if(sqlite3_open("./staff.db",&db)!=SQLITE_OK){
		printf("数据库打开失败\n");
		printf("%s\n",sqlite3_errmsg(db));
		printf("%d\n",sqlite3_errcode(db));
	}
	printf("sqlite3_open success\n");

	//创建员工信息表
	char sql[512];
	sprintf(sql,"update staff set name=\"%s\",sex=\"%s\",age=\"%s\",id=\"%s\",phone=\"%s\",\
			department=\"%s\",pay=\"%s\"  where number= %d",info.name,info.sex,info.age,\
			info.id,info.phone,info.department,info.pay,info.number);
	char* errmsg=NULL;
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK){
		fprintf(stderr,"__%d__sqlite3_exec:%s\n",__LINE__,errmsg);
		return -1;
	}
	printf("sqlite3_exec delete table success");

	return 0;

}
//删除
int del(){
	sqlite3 *db=NULL;
	if(sqlite3_open("./staff.db",&db)!=SQLITE_OK){
		printf("数据库打开失败\n");
		printf("%s\n",sqlite3_errmsg(db));
		printf("%d\n",sqlite3_errcode(db));
	}
	printf("sqlite3_open success\n");

	//创建员工信息表
	char sql[512];
	sprintf(sql,"delete from staff where number= %d",info.number);
	char* errmsg=NULL;
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK){
		fprintf(stderr,"__%d__sqlite3_exec:%s\n",__LINE__,errmsg);
		return -1;
	}
	printf("sqlite3_exec delete table success");

	return 0;
}
//增加
int add(){
	sqlite3 *db=NULL;
	if(sqlite3_open("./staff.db",&db)!=SQLITE_OK){
		printf("数据库打开失败\n");
		printf("%s\n",sqlite3_errmsg(db));
		printf("%d\n",sqlite3_errcode(db));
	}
	printf("sqlite3_open success\n");

	//创建员工信息表
	char sql[512];
	sprintf(sql,"insert into staff values(%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\");",info.number,info.name,\
			info.sex,info.age,info.id,info.phone,info.department,info.pay);
	char* errmsg=NULL;
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK){
		fprintf(stderr,"__%d__sqlite3_exec:%s\n",__LINE__,errmsg);
		return -1;
	}
	printf("sqlite3_exec insert table success");
	return 0;
}
//查询
int inquire(){
	sqlite3 *db=NULL;
	char sql[256];
	if(sqlite3_open("./staff.db",&db)!=SQLITE_OK){
		printf("数据库打开失败\n");
		printf("%s\n",sqlite3_errmsg(db));
		printf("%d\n",sqlite3_errcode(db));
	}
	printf("sqlite3_open success\n");

	bzero(sql,sizeof(sql));
    sprintf(sql,"select * from staff where number=\"%d\"",info.number);
    char **pres = NULL;
    int row, column;
  	char* errmsg = NULL;
    if(sqlite3_get_table(db, sql, &pres, &row, &column, &errmsg) != SQLITE_OK)
    {
        fprintf(stderr, "__%d__ sqlite3_get_table:%s\n", __LINE__, errmsg);
        return -1;
    }
    printf("get table success\n");
    printf("row=%d column=%d\n", row, column);
    if(row==0){
        printf("员工信息不存在\n");
        return 0;
    }
	int j=0;
	int line=0,list=0;
	for(line=0;line<row+1;line++){
		for(list=0;list<column;list++){
			printf("%10s ",pres[j++]);

		}
		putchar(10);
	}
	strcpy(info.name,pres[9]);
	strcpy(info.sex,pres[10]);
	strcpy(info.age,pres[11]);
	strcpy(info.id,pres[12]);
	strcpy(info.phone,pres[13]);
	strcpy(info.department,pres[14]);
	strcpy(info.pay,pres[15]);
    //释放获取到的结果
    sqlite3_free_table(pres);

}
//登陆
int log_on(){
	sqlite3 *db=NULL;
	if(sqlite3_open("./staff.db",&db)!=SQLITE_OK){
		printf("数据库打开失败\n");
		printf("%s\n",sqlite3_errmsg(db));
		printf("%d\n",sqlite3_errcode(db));
	}
	printf("sqlite3_open success\n");

	//创建员工表
	char sql[256]="create table if not exists personal_information (username char primary key ,password char ,power int);";
	char* errmsg=NULL;
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK){
		fprintf(stderr,"__%d__sqlite3_exec:%s\n",__LINE__,errmsg);
		return -1;
	}
	printf("sqlite3_exec create table success");

	bzero(sql,sizeof(sql));
    sprintf(sql,"select * from personal_information where username=\"%s\" and password=\"%s\"",info.username,info.password);
    char **pres = NULL;
    int row, column;
  	errmsg = NULL;
    if(sqlite3_get_table(db, sql, &pres, &row, &column, &errmsg) != SQLITE_OK)
    {
        fprintf(stderr, "__%d__ sqlite3_get_table:%s\n", __LINE__, errmsg);
        return -1;
    }
    printf("get table success\n");
    printf("row=%d column=%d\n", row, column);
	strcpy(info.power,pres[5]);
    if(row==0){
        printf("账号不存在\n");
		strcpy(info.buf,"账号不存在");
        return 0;
    }
    //释放获取到的结果
    sqlite3_free_table(pres);
	if(strcmp(info.power,"1")==0){
		sprintf(info.buf,"%c%s",0x01,"管理员登陆成功");
	}else if(strcmp(info.power,"2")==0){	
		sprintf(info.buf,"%c%s",0x02,"普通用户登陆成功");
	}
	return 1;
}
//注册
int reg(){
	printf("%s %s\n",info.username,info.password);
	sqlite3 *db=NULL;
	if(sqlite3_open("./staff.db",&db)!=SQLITE_OK){
		printf("数据库打开失败\n");
		printf("%s\n",sqlite3_errmsg(db));
		printf("%d\n",sqlite3_errcode(db));
	}
	printf("sqlite3_open success\n");
	//创建员工表
	char sql[256]="create table if not exists personal_information (username char primary key ,password char ,power int);";
	char* errmsg=NULL;
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK){
		fprintf(stderr,"__%d__sqlite3_exec:%s\n",__LINE__,errmsg);
		return -1;
	}
	printf("sqlite3_exec create table success");

	bzero(sql,sizeof(sql));
    sprintf(sql,"select * from personal_information where username=\"%s\"",info.username);
    char **pres = NULL;
    int row, column;
    errmsg = NULL;
    if(sqlite3_get_table(db, sql, &pres, &row, &column, &errmsg) != SQLITE_OK)
    {
        fprintf(stderr, "__%d__ sqlite3_get_table:%s\n", __LINE__, errmsg);
        return -1;
    }
    printf("get table success\n");
    printf("row=%d column=%d\n", row, column);
    if(row!=0){
        printf("账号已被注册\n");
		strcpy(info.buf,"账号已被注册");
        return 0;
    }
    //释放获取到的结果
    sqlite3_free_table(pres);

    //插入注册用户
    sprintf(sql,"insert into personal_information values (\"%s\",\"%s\",%s);",info.username,info.password,info.power);
    errmsg=NULL;
    if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK){
        fprintf(stderr,"__%d__sqlite3_exec:%s\n",__LINE__,errmsg);
        return -1;
    }
	strcpy(info.buf,"账号注册成功");
	return 1;
}
int network(){
	//创建流式套接字
	int sfd = socket(AF_INET,SOCK_STREAM,0);	
	if(sfd < 0){
		ERR_MSG("socket");
		return -1;
	}
	//允许端口快速重用
	int reuse = 1;
	if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
	{
		ERR_MSG("setsockopt");
		return -1;
	}
	//填充地址信息结构体
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port        = htons(PORT);  //端口号的网络字节序，1024~49151
	sin.sin_addr.s_addr = inet_addr("10.102.133.130");
	//绑定服务器的IP地址和端口号    bind
	if(bind(sfd, (struct sockaddr*)&sin, sizeof(sin)) < 0) 
	{
		ERR_MSG("bind"); 
		return -1;
	}
	printf("bind success\n");
	//将套接字设置为被动监听状态
	if(listen(sfd, 10) < 0)
	{
		ERR_MSG("listen");
		return -1;
	}
	printf("listen success\n");
	struct sockaddr_in cin;
	socklen_t addrlen = sizeof(cin);
	struct net_work msg;
	pthread_t tid=0;
	//新的文件描述符
	int newfd;
	char buf[128] = "";
	ssize_t res = 0;
	int flag=2;
	while(1)
		{
			int newfd = accept(sfd, (struct sockaddr*)&cin, &addrlen);
			if(newfd < 0)
			{
				ERR_MSG("accept");
				return -1;
			}
			printf("[%s | %d]newfd = %d 连接成功\n", \
			inet_ntoa(cin.sin_addr), ntohs(cin.sin_port), newfd);
			
			msg.newfd = newfd;
			msg.cin = cin;
			if(pthread_create(&tid,NULL,rev_cli_net_work,(void*)&msg)!=0){
				ERR_MSG("pthread_create");
				return -1;
			}
		}
	close(newfd);
	close(sfd);
	return 0;
}

void* rev_cli_net_work(void* arg){
	//分离线程
	pthread_detach(pthread_self());
	struct net_work msg = *(struct net_work*)arg;
	int newfd = msg.newfd;
	struct sockaddr_in cin = msg.cin;
	ssize_t res = 0;
	while(1)
		{
			
			bzero(&info, sizeof(info));
			//接收数据
			res = recv(newfd, &info, sizeof(info), 0);
			if(res < 0)
			{
				ERR_MSG("recv");
				return NULL;
			}
			else if(0 == res)
			{
				printf("[%s | %d]newfd = %d 客户端退出\n", \
						inet_ntoa(cin.sin_addr), ntohs(cin.sin_port), newfd);
				break;
			}
			printf("info.type:%d\n",info.type);
			if(info.type==1){
				log_on();
			}else if(info.type==2){
				//注册
				reg();
			}else if(info.type==4){
				//增加
				add();
			}else if(info.type==5){
				//删除
				del();
			}else if(info.type==6){
				//查询
				inquire();
			}else if(info.type==7){
				//修改
				update();
			}
			//发送数据
			if(send(newfd, &info, sizeof(info), 0) < 0)
			{
				ERR_MSG("send");
				return NULL;
			}
			printf("发送成功\n");
		}


}

