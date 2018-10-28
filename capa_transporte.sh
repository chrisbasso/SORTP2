#!/bin/bash
#------------------------------------------------------
# PALETA DE COLORES
#------------------------------------------------------
#setaf para color de letras/setab: color de fondo
    red=`tput setaf 1`;
    green=`tput setaf 2`;
    blue=`tput setaf 4`;
    bg_blue=`tput setab 4`;
    reset=`tput sgr0`;
    bold=`tput setaf bold`;
#------------------------------------------------------
# VARIABLES GLOBALES
#------------------------------------------------------
proyectoActual="/home/sor1";
proyectos="/home/sor1/Documents/repo_GitLab/repos.txt";

#------------------------------------------------------
# DISPLAY MENU
#------------------------------------------------------
imprimir_menu () {
       imprimir_encabezado "\t  C A P A   D E   R E D";
    
    echo -e "\t\t El proyecto actual es:";
    echo -e "\t\t $proyectoActual";
    
    echo -e "\t\t";
    echo -e "\t\t Opciones:";
    echo "";
    echo -e "\t\t\t a.  Mostrar interfaces de red disponibles";
    echo -e "\t\t\t b.  Escanear los puertos 1 hasta 32000 para localhost";
    echo -e "\t\t\t c.  Escanear los puertos 1 hasta 32000 seleccionando IP";   
    echo -e "\t\t\t q.  Salir";
    echo "";
    echo -e "Escriba la opción y presione ENTER";
}

#------------------------------------------------------
# FUNCTIONES AUXILIARES
#------------------------------------------------------

imprimir_encabezado () {
    clear;
    #Se le agrega formato a la fecha que muestra
    #Se agrega variable $USER para ver que usuario está ejecutando
    echo -e "`date +"%d-%m-%Y %T" `\t\t\t\t\t USERNAME:$USER";
    echo "";
    #Se agregan colores a encabezado
    echo -e "\t\t ${bg_blue} ${red} ${bold}--------------------------------------\t${reset}";
    echo -e "\t\t ${bold}${bg_blue}${red}$1\t\t${reset}";
    echo -e "\t\t ${bg_blue}${red} ${bold} --------------------------------------\t${reset}";
    echo "";
}

esperar () {
    echo "";
    echo -e "Presione enter para continuar";
    read ENTER ;
}

malaEleccion () {
    echo -e "Selección Inválida ..." ;
}

decidir () {
    echo $1;
    while true; do
        echo "desea ejecutar? (s/n)";
            read respuesta;
            case $respuesta in
                [Nn]* ) break;;
                   [Ss]* ) eval $1
                break;;
                * ) echo "Por favor tipear S/s ó N/n.";;
            esac
    done
}

#------------------------------------------------------
# FUNCTIONES del MENU
#------------------------------------------------------
a_funcion () {
        imprimir_encabezado "\tOpción a";
	    ifconfig -a
}

b_funcion () {
           imprimir_encabezado "\tOpción b";
           netcat -z -v 127.0.0.1 1-32000 2>&1 | grep succeeded
}

c_funcion () {
          imprimir_encabezado "\tOpción c";
          echo "Ingrese una IP"
          read ip
          netcat -z -v $ip 1-32000 2>&1 | grep succeeded
          
}


#------------------------------------------------------
# LOGICA PRINCIPAL
#------------------------------------------------------
while  true
do
    # 1. mostrar el menu
    imprimir_menu;
    # 2. leer la opcion del usuario
    read opcion;
    
    case $opcion in
        a|A) a_funcion;;
        b|B) b_funcion;;
        c|C) c_funcion;;
        q|Q) break;;
        *) malaEleccion;;
    esac
    esperar;
done
 

