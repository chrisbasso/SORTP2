#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc,char **argv)
{
  //////////////////////////////////////
  // Definir la direccion del server
  //////////////////////////////////////
  struct sockaddr_in my_server_addr;
  bzero(&my_server_addr,sizeof my_server_addr); 
  my_server_addr.sin_family=AF_INET;
  my_server_addr.sin_port=htons(22000);

  //Setear la direccion IP en my_server_addr a “127.0.0.1” (localhost) si el servidor esta en la misma maquina.
  // inet_pton - convierte direcciones IPv4/IPv6 de texto a forma binaria
  inet_pton(AF_INET,"127.0.0.1",&(my_server_addr.sin_addr));


  //////////////////////////////////////
  // Definir el socket
  //////////////////////////////////////
  int socket_client_fd;
  socket_client_fd = socket(AF_INET, SOCK_STREAM, 0);
 

  //la funcion connect() conecta al dispositivo cuya direccion y puerto es my_server_addr. 
  connect(socket_client_fd, (struct sockaddr *)&my_server_addr,sizeof(my_server_addr));

  //Ahora el cliente puede enviar y recibir datos  
  char sendline[100];
  char recvline[100];

  //////////////////////////////////////////
  // Empieza el ciclo principal del cliente
  //////////////////////////////////////////
  bool salir_ciclo=false;
  while(!salir_ciclo){
    //Primero limpiar los buffers sendline and recvline
    bzero( sendline, 100);
    bzero( recvline, 100);
	
    //leer un string de stdin (la terminal) y guardarlo en sendline 
    //el usuario tiene que escribir algo	
    printf("\n¿Quiere iniciar o salir del juego? (start/salir): ");
    fgets(sendline, 100, stdin); /*stdin = 0 , for standard input */
    char letraClave[100];
    strncpy(letraClave, sendline, 5);
    letraClave[5] = '\0';

    if(strcmp(letraClave,"start")==0){
      //////////////////////////////////////
      // Opcion: start
      //////////////////////////////////////
      write(socket_client_fd, sendline, strlen(sendline)+1);
      read(socket_client_fd,recvline,100);
      char letraClave[100];
      strncpy(letraClave, recvline, 5);
      letraClave[5] = '\0';
      //////////////////////////////////////
      // Empezar el juego
      //////////////////////////////////////
      bool salir= false;
      while(!salir){
	bzero( sendline, 100);
	bzero( recvline, 100);
	read(socket_client_fd,recvline,100);
	char letraClave[100];
	strncpy(letraClave, recvline, 4);
	letraClave[4] = '\0';

	if(strcmp(letraClave,"gane")==0){
	  //////////////////////////////////////
	  // Opcion: Gano
	  //////////////////////////////////////
	  printf("\n¡Servidor acerto la palabra!\n");
	  salir=true;
	    
	} else if(strcmp(letraClave,"over")==0){
	  //////////////////////////////////////
	  // Opcion: Perdio
	  //////////////////////////////////////
	  printf("\n¡Servidor perdio!\n");
	  salir=true;
	      
	}else{
	  //////////////////////////////////////
	  // Opcion: Dar pista al servidor
	  //////////////////////////////////////
	  printf("\nServidor responde: %s",recvline);
	  printf("Dar pista (mayor/menor/exact): ");
	  fgets(sendline, 100, stdin);
	  write(socket_client_fd, sendline, strlen(sendline)+1);
	    
	}
      }
    }else{
      write(socket_client_fd, sendline, strlen(sendline)+1);//necesario para que no se trabe tratando de leer despues
      read(socket_client_fd,recvline,100);
      char letraC[100];
      strncpy(letraC, recvline, 4);
      letraC[4] = '\0';
      
      //////////////////////////////////////
      // Opcion: Salir del juego definitivamente
      //////////////////////////////////////
      if(strcmp(letraC,"chau")==0){
	salir_ciclo=true;
      }
      //read(socket_client_fd,recvline,100);
    }
  }
  return 0; //EXIT_SUCCESS;   
}

