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
//加载登陆界面
int load();
//进行功能选择 管理员
int query();
//进行功能选择 普通用户
int low_query();
int main(int argc, const char *argv[])
{
	network();
	return 0;
}
int low_query(){
	int choose=0;
	printf("******************\n");
	printf("******3.查询******\n");
	printf("******5.退出******\n");
	printf("******************\n");
	printf("输入>>>");
	scanf("%d",&choose);
	system("clear");
	switch(choose){
		case 3:
			info.type=6;
			printf("输入你要查询员工的编号\n");
			scanf("%d",&info.number);	
			break;
		case 5:
			printf("酱紫退出啊\n");
			exit(0);
		default:
			printf("请输入3和5数字\n");
			break;
	}

}

int query(){
	int choose=0;
	printf("******************\n");
	printf("******1.增加******\n");
	printf("******2.删除******\n");
	printf("******3.查询******\n");
	printf("******4.修改******\n");
	printf("******5.退出******\n");
	printf("******************\n");
	printf("输入>>>");
	scanf("%d",&choose);
	system("clear");
	switch(choose){
		case 1:
			info.type=4;
			scanf("%d %s %s %s %s %s %s %s",&info.number,info.name,\
					info.sex,info.age,info.id,info.phone,\
					info.department,info.pay);
			break;
		case 2:
			info.type=5;
			printf("输入你要删除的员工编号\n");
			scanf("%d",&info.number);
			break;
		case 3:
			info.type=6;
			printf("输入你要查询员工的编号\n");
			scanf("%d",&info.number);	
			break;
		case 4:
			info.type=7;
			scanf("%d %s %s %s %s %s %s %s",&info.number,info.name,\
					info.sex,info.age,info.id,info.phone,\
					info.department,info.pay);

			break;
		case 5:
			printf("酱紫退出啊\n");
			exit(0);
		default:
			printf("请输入1---5数字\n");
			break;
	}

}
int load(){
	int choose=0;
	printf("******************\n");
	printf("******1.登陆******\n");
	printf("******2.注册******\n");
	printf("******3.退出******\n");
	printf("******************\n");
	printf("输入>>>");
	scanf("%d",&choose);
	system("clear");
	switch(choose){
		case 1:
			info.type=1;
			printf("请输入账号密码：\n");
			scanf("%s\n%s",info.username,info.password);
			break;
		case 2:
			info.type=2;
			printf("请输入账号密码和权限(管理员1，普通2)：\n");
			scanf("%s\n%s\n%s",info.username,info.password,info.power);
			break;
		case 3:
			printf("酱紫退出啊\n");
			exit(0);
		default:
			printf("请输入1---3数字\n");
			break;
	}
}

int network(){
	//创建流式套接字
	int sfd = socket(AF_INET, SOCK_STREAM, 0); 
	if(sfd < 0)
	{
		ERR_MSG("socket"); 
		return -1;
	}
	//绑定客户端的IP和端口---->非必须 
	//填充要连接的服务器的地址信息结构体
	struct sockaddr_in sin;
	sin.sin_family      = AF_INET;
	sin.sin_port        = htons(PORT);     //服务器的端口
	sin.sin_addr.s_addr = inet_addr("10.102.133.130");   //服务器绑定的IP
	//连接服务器
	if(connect(sfd, (struct sockaddr*)&sin, sizeof(sin)) < 0) 
	{
		ERR_MSG("connect"); 
		return -1;
	}
	printf("connect success\n");
	ssize_t res = 0;
	char buf[128] = "";
	//用于页面切换
	int page=0;
	while(1){
		bzero(&info,sizeof(info));
		if(page==0){
			load();
		}else if(page==1){
			query();
		}else if(page==2){
			low_query();
		}

		printf("info.type:%d",info.type);
		//发送
		if(send(sfd,&info, sizeof(info), 0) < 0)
		{
			ERR_MSG("send");
			return -1;
		}
		printf("发送成功\n");
		//接收
		bzero(&info, sizeof(info));
		res = recv(sfd, &info, sizeof(info), 0);
		if(res < 0)
		{
			ERR_MSG("recv");
			return -1;
		}else if(0 == res){
			printf("服务器关闭\n");
			break;
		}
		printf("%c",info.buf[0]);
		if(info.buf[0]==0x01){
			page=1;
		}else if(info.buf[0]==0x02){
			page=2;
		}
		printf("name=%s sex=%s age=%s id=%s phone=%s department=%s pay=%s\n",\
				info.name,info.sex,info.age,info.id,info.phone,info.department,info.pay);
		//printf(":%s\n",info.buf);
	}
	close(sfd);

}

