#include "mt.h"

int main(int argc, char* argv[]) {
	int sock;
	struct in_addr inp;
	struct sockaddr_in name;
	char input[MAX_MSG_LENGTH];
	char output[MAX_MSG_LENGTH];
	int llegoalgo = 0;

	/* Crear socket sobre el que se lee: dominio INET, protocolo UDP (DGRAM). */
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (inet_aton(argv[1], &inp) != 0) {
		name.sin_family = AF_INET;
		name.sin_addr = inp;
		name.sin_port = htons(PORT);

	   	/* Conectarse. */
	    if (connect(sock, (struct sockaddr *)&name, sizeof(name)) == -1) {
	        perror("conectandose");
	        exit(1);
	    }

		while (fgets(input, MAX_MSG_LENGTH, stdin)) {
		    if (send(sock, input, strlen(input), 0) == -1) {
		        perror("enviando");
		        exit(1);
		    }
			
			read(sock, output, MAX_MSG_LENGTH);
			printf("RESPUESTA SERVIDOR %s", output);

			if (strncmp(input, END_STRING, MAX_MSG_LENGTH) == 0) {
				close(sock);
				break;
			}
		}

	}

	return 0;
}