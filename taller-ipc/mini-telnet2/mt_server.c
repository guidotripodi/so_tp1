#include "mt.h"
int main()
{
	int sock, s1, t;
	struct sockaddr_in name;
	char buf[MAX_MSG_LENGTH];
	/* Crear socket sobre el que se lee: dominio INET, protocolo UDP (DGRAM). */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("abriendo socket");
		exit(1);
	}
	/* Crear nombre, usamos INADDR_ANY para indicar que cualquiera puede enviar aquí. */
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = INADDR_ANY;
	name.sin_port = htons(PORT);
	if (bind(sock, (void*) &name, sizeof(name))) {
		perror("binding datagram socket");
		exit(1);
	}

	/* Escuchar en el socket y permitir 5 conexiones en espera. */
    if (listen(sock, 5) == -1) {
        perror("escuchando");
        exit(1);
    }

	/* Aceptar una conexión entrante. */
    t = sizeof(name);
    if ((s1 = accept(sock, (struct sockaddr*) &name, (socklen_t*) &t)) == -1) {
        perror("aceptando la conexión entrante");
        exit(1);
    }

	/* Recibimos mensajes hasta que alguno sea el que marca el final. */
	for (;;) {
		read(s1, buf, MAX_MSG_LENGTH);
		if (strncmp(buf, END_STRING, MAX_MSG_LENGTH) == 0)
			break;

		if (strlen(buf) != 0) {
			printf("Comando: %s", buf);
			system(buf);			
			write(s1, stdout, MAX_MSG_LENGTH);
		}

	}
	/* Cerrar socket de recepción. */
	close(sock);

	close(s1);
	return 0;
}

