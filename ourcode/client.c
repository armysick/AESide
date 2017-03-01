#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

int s;
char res_buf[40];
int maior;
int atual;
int o_maior_alg[5];


int compare_int( const void* a, const void* b )
{
    if( *(int*)a == *(int*)b ) return 0;
    return *(int*)a < *(int*)b ? -1 : 1;
}

int testchar(int index, int alg){
	
	char to_send[5];
	int packetlen = sizeof to_send;
	char res_buf[40];
	int diffs[200];

	for(int o = 0; o<index; o++){
		to_send[o] = o_maior_alg[o] + '0';
	}
	to_send[index] = alg + '0';
	for(int x = index+1; x<5;x++){
		to_send[x] = '0';
	}


	for(int u = 0; u < 200; u++){
		//printf("to_send: %s\n", to_send);
		send(s,to_send, packetlen, 0);
		int bytes_received = recv(s, res_buf, sizeof res_buf, 0);
		diffs[u] = *(unsigned int *) (res_buf+36) - *(unsigned int *) (res_buf+32);
	}
	qsort(diffs, 200, sizeof(int), compare_int);

	int counter = 0;	
	for(int p = 49; p < 150; p++){
		counter+= diffs[p];
	}
	
	printf("atual: %i \n", (counter/100));
	return (counter / 100);
}

int main(int argc, char *argv[]) {
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(10000);
	
	while((s=socket(AF_INET, SOCK_DGRAM,0)) == -1) sleep(1);
	while(connect(s, (struct sockaddr *) &server, sizeof server) == -1) sleep(1);

	//char packet[5] = "42069";
	
	for(int i = 0 ; i<5; i++){   // key has 5 length
		maior = -9999999;
		for(int j = 0; j < 10; j++){  //10 possible combinations for each algarism
			atual = testchar(i,j);
			if(atual > maior){
				maior = atual;
				o_maior_alg[i] = j;
			}
		}
	}
	
	for(int y = 0; y < 5; y++){
		printf("o maior alg %i: %i\n", y, o_maior_alg[y]);
	}

	//printf("Recebeu de resposta: %s\n", res_buf);

	//printf("pre stamp: %i\n", *(unsigned int *) (res_buf+32));
	//printf("pos stamp: %i\n", *(unsigned int *) (res_buf+36));
	//printf("difference: %i\n", *(unsigned int *) (res_buf+36) - *(unsigned int *) (res_buf+32));
}
