/*Librerias requeridas*/
//GNU\Linux implementa el protocolo IP, version 4, descrito en RFC 791 y RFC 1122.
//Tambien contiene un router IP y un filtro de paquetes. 
//referencia: https://linux.die.net/man/7/ip

#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>


int main()
{
  char str[100];

  //////////////////////////////////////
  // Definir la direccion del server
  //////////////////////////////////////

  struct sockaddr_in  my_server_addr;
  //Struct para contener una direccion IP y un numero de Puerto

  //Limpiar o borrar my_server_addr.
  bzero( &my_server_addr, sizeof(my_server_addr));

  my_server_addr.sin_family = AF_INET;
  my_server_addr.sin_addr.s_addr = htons(INADDR_ANY);
  my_server_addr.sin_port = htons(22000);

  
  //////////////////////////////////////
  // Definir el socket
  //////////////////////////////////////


  //File Descriptors (descriptores de archivo) a ser usados
  int listen_fd, comm_fd;
  
  //Cada servidor necesita escuchar “listen” por conecciones. La siguiente funcion crea un socket 
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  
  bind(listen_fd, (struct sockaddr *) &my_server_addr, sizeof(my_server_addr));
  //Prepara para escuchar por conecciones de direcciones/puertos especificados en my_server_addr ( cualquier IP en el puerto 22000 ).

  listen(listen_fd, 10);
  //Empiezo a escuchar por connecciones

    
  comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
  //Acepta una coneccion, si no hay nadie espera. Se retorna un file descriptor comm_fd que se usa para comunicarse. Todo lo que se envia al servidor se puede leer de comm_fd, todo lo que se escriba en comm_fd es enviado al otro dispositivo que se conectó.


  ///////////////////////////////////////////
  // Empieza el ciclo principal del servidor
  ///////////////////////////////////////////

  bool salir_ciclo = false;
  while(!salir_ciclo){
    //El servidor hace lo siguiente en cada iteracion

    //limpiamos el buffer str
    bzero(str, 100);

    //la funcion read() lee hasta 100 bytes del file descriptor comm_fd y lo guarda en el buffer str
    //si tiene exito, el numero de bytes leidos es retornado 
    read(comm_fd, str, 100);
    char letraClave[100];
    strncpy(letraClave, str, 5);
    letraClave[5] = '\0';

    //////////////////////////////////////
    // Opcion: Start
    //////////////////////////////////////
    if(strcmp(letraClave,"start")==0){
      write(comm_fd, "start", strlen("start")+1); 
      printf("\nEscriba un numero del 0 al 256: ");
      fgets(str, 100, stdin);
      write(comm_fd, str, strlen(str)+1);
      bool salir= false;
      while(!salir){
	bzero(str, 100);
	read(comm_fd, str, 100);
	char letraClave[100];
	strncpy(letraClave, str, 5);
	letraClave[5] = '\0';
	
	if(strcmp(letraClave,"mayor")==0){
	  //////////////////////////////////////
	  // Opcion: Mayor
	  //////////////////////////////////////
	  printf("\nEl cliente dice que el numero a advinar es %s.",letraClave);
	  printf("\nEscriba otro numero del 0 al 256: ");
	  fgets(str, 100, stdin);
	  write(comm_fd, str, strlen(str)+1);

	}else if(strcmp(letraClave,"menor")==0){	  
	  //////////////////////////////////////
	  // Opcion: Menor
	  //////////////////////////////////////
	  printf("\nEl cliente dice que el numero a advinar es %s.",letraClave);
	  printf("\nEscriba otro del 0 al 256: ");
	  fgets(str, 100, stdin);
	  write(comm_fd, str, strlen(str)+1);
	  
	}else if(strcmp(letraClave,"exact")==0){
	  //////////////////////////////////////
	  // Opcion: Exacto
	  //////////////////////////////////////	    
	  printf("\nEl cliente dice que el numero es correcto.");
	  printf("\n¡Ganaste!\n");
	  write(comm_fd,"gane", strlen("gane")+1);
	  salir=true;
	  
	}else if(strcmp(letraClave,"abrir escotilla")==0){
	  //////////////////////////////////////
	  // Opcion: Game Over en caso de tener un contador de vidas/intentos
	  //////////////////////////////////////
	  strncpy(letraClave, str, 15);
	  letraClave[15] = '\0';
	  printf("\nEl cliente dice que se te acabaron los intentos.");
	  printf("\n¡Perdiste!\n");
	  write(comm_fd,"over", strlen("over")+1);
	  salir=true;
	  
	}else{
	  write(comm_fd, "Error al escribir\n", strlen("Error al escribir\n")+1);
	}				
      }
      //////////////////////////////////////
      // Opcion: Salir del juego totalmente
      //////////////////////////////////////       
    }else if(strcmp(letraClave,"salir")==0){
      write(comm_fd,"chau", strlen("chau")+1); 
      salir_ciclo=true;
    }else{
      //devolvemos un eco al cliente
      write(comm_fd, str, strlen(str)+1);
    }
    
  }
  return 0; //EXIT_SUCCESS;
}

