#define KEY_SIZE 5
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

char key[] = {'4','2','0','6','9'}; 
struct sockaddr_in server;
struct sockaddr_in client;
int s;
char in_buf[1537];
char resposta[40];

unsigned int timestamp(void){
	unsigned int bottom;
	unsigned int top;
	asm volatile(".byte 15;.byte 49" : "=a"(bottom),"=d"(top));
	return bottom;
}


void computacao(char in_buf[]){
	for(int i = 0 ; i <KEY_SIZE ; i++){
		if(in_buf[i] != key[i]){
			resposta[0] = 'f';   // 'f' -> Fail
			//printf("resposta_fail: %s \n", resposta);
			return;
		}
	}
	resposta[0] = 's'; // 's' -> success
	//printf("resposta_suc: %s \n", resposta);
}
// Assuming we know that the key is sized 5
void processar(char resposta[40], char in_buf[], int inblen){
	for(int i = 0; i<40;i++) resposta[i] = 0;
	if(in_buf[KEY_SIZE] != NULL){
		resposta[0] = 'n'; // 'n'-> NULL
	 	return;
	}
	printf("in_buf: %s \n", in_buf);
	*(unsigned int *) (resposta + 32) = timestamp();
	for(int i = 0 ; i < 15000; i++)
		computacao(in_buf);
	*(unsigned int *) (resposta + 36) = timestamp();

}
int main(void){

	server.sin_family = AF_INET;
	server.sin_port = htons(10000);

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s==-1) return 111;
	if(bind(s,(struct sockaddr *) &server,sizeof server) == -1) return 111;

	
	for(;;){
		int clilength = sizeof client;
		//printf("pretestes\n");
		int bytes_received = recvfrom(s, in_buf, sizeof in_buf, 0, (struct sockaddr *) &client, &clilength);
		//printf("testes: %x \n", bytes_received);
		// if( bytes_received >= sizeof in_buf) continue;
		processar(resposta, in_buf, KEY_SIZE);
		sendto(s, resposta, sizeof resposta, 0, (struct sockaddr *) &client, clilength);
	}
}
