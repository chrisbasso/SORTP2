/* Simple Raw Sniffer                                                    */
/* Author: Luis Martin Garcia. luis.martingarcia [.at.] gmail [d0t] com  */
/*                                                                       */
/* This code is distributed under the GPL License. For more info check:  */
/* http://www.gnu.org/copyleft/gpl.html                                  */

//bibliografia:
//sacado de:    https://github.com/lsanotes/libpcap-tutorial
//explicacion en:   http://recursos.aldabaknocking.com/libpcapHakin9LuisMartinGarcia.pdf

//comandos utiles:
//para ver los nombres de interfaz de red disponibles:     /sbin/ifconfig -a
//para compilar:     gcc sniffer.c -o sniffer -lpcap
//para ejecutar:     sudo ./sniffer
//para ejecutar sobre una interaz de red en particular (wlan0):   sudo ./sniffer wlan0
//para generar captura con tcpdump: sudo tcpdump -i lo -v
//para obtener trafico remoto:    ssh --passsword andrew andrew@localhost "sudo /usr/sbin/tcpdump -s0 -w - 'port 8080'" | wireshark -k -i -


#include <pcap.h>
#include <string.h>
#include <stdlib.h>

#define MAXBYTES2CAPTURE 2048

/////////////////////////////////////////////////////////////////////////////////
/* procesarPaquete(): Callback function called by pcap_loop() everytime a packet */
/* arrives to the network card. This function prints the captured raw data in  */
/* hexadecimal.                                                            	*/
/////////////////////////////////////////////////////////////////////////////////
void procesarPaquete(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char * packet){

  int i=0, *counter = (int *)arg;

  printf("Packet Count: %d\n", ++(*counter));
  printf("Received Packet Size: %d\n", pkthdr->len);
  printf("Payload:\n");
  for (i=0; i<pkthdr->len; i++){

	if ( isprint(packet[i]) ) /* If it is a printable character, print it */
  	printf("%c ", packet[i]);
	else
  	printf(". ");
    
	if( (i%16 == 0 && i!=0) || i==pkthdr->len-1 )
  	printf("\n");
  }
  return;
}



/////////////////////////////////////////////////////////////////////////////////
/* main(): Main function. Opens network interface and calls pcap_loop() */
/////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[] ){
    
  int i=0, count=0;
  pcap_t *descr = NULL;
  char errbuf[PCAP_ERRBUF_SIZE], *device=NULL;
  memset(errbuf,0,PCAP_ERRBUF_SIZE);

  if( argc > 1){  /* If user supplied interface name, use it. */
	device = argv[1];
  }
  else{  /* Get the name of the first device suitable for capture */

	if ( (device = pcap_lookupdev(errbuf)) == NULL){
  	fprintf(stderr, "ERROR: %s\n", errbuf);
  	exit(1);
	}
  }

  /* Open device in promiscuous mode */
  printf("Abriendo (en modo promiscuo) dispositivo de red: %s\n", device);
  if ( (descr = pcap_open_live(device, MAXBYTES2CAPTURE, 1,  512, errbuf)) == NULL){
	fprintf(stderr, "ERROR: %s\n", errbuf);
	exit(1);
  }

  /* Loop forever & call processPacket() for every received packet*/
  printf("Ciclar y llamar a procesarPaquete() por cada paquete recibido \n");
  if ( pcap_loop(descr, -1, procesarPaquete, (u_char *)&count) == -1){
	fprintf(stderr, "ERROR: %s\n", pcap_geterr(descr) );
	exit(1);
  }

  return 0;
}

/* EOF*/
