#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cliente.h"
extern "C"{


    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

    void MainWindow::on_carregar_clicked()
    {
            int sockfd;
             char buffer[MAXLINE];
             char hello [MAXLINE];
             hello[0]='o';
             struct sockaddr_in	 servaddr;

             // Creating socket file descriptor
             if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
             {
                 perror("socket creation failed");
                 exit(EXIT_FAILURE);
             }

             memset(&servaddr, 0, sizeof(servaddr));

             // Filling server information
             servaddr.sin_family = AF_INET;
             servaddr.sin_port = htons(PORT);
             servaddr.sin_addr.s_addr = INADDR_ANY;

             unsigned int n, len;

             sendto(sockfd, (const char *)hello, strlen(hello),
                    MSG_CONFIRM, (const struct sockaddr *) &servaddr,
                    sizeof(servaddr));
             printf("Hello message sent.\n");

             do{
                 n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                              0, (struct sockaddr *) &servaddr,
                              &len);
                 buffer[n] = '\0';

                 printf("Server : %s\n", buffer);
                 ui->listaArquivos->addItem(buffer);
             }while(n!=0);
    }



void MainWindow::on_receber_clicked()
{
    int sockfd;
     char buffer[MAXLINE];

     struct sockaddr_in	 servaddr;

     // Creating socket file descriptor
     if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
     {
         perror("socket creation failed");
         exit(EXIT_FAILURE);
     }

     memset(&servaddr, 0, sizeof(servaddr));

     // Filling server information
     servaddr.sin_family = AF_INET;
     servaddr.sin_port = htons(PORT);
     servaddr.sin_addr.s_addr = INADDR_ANY;

     unsigned int n, len;


     QString string;
     string=ui->lineEdit->text();

       QByteArray ba = string.toLocal8Bit();
       const char *buff = ba.data();
    sendto(sockfd, (const char *)buff, sizeof(buff),
                   0, (const struct sockaddr *) &servaddr,
                   sizeof(servaddr));


            char arq[2];

            FILE *arquivo;

            char caminhoSaida[MAXLINE];

            strcpy(caminhoSaida, "./Saida/");
            strcat(caminhoSaida, buff);

            arquivo= fopen(caminhoSaida, "w+");
            if(arquivo==NULL)
                printf("Falha ao abrir o arquivo");

            do
            {
                n=recvfrom(sockfd, arq, sizeof(arq), 0, (struct sockaddr *) &servaddr, &len);
                printf("%s", arq);
                fwrite(arq, sizeof(char), 1, arquivo);
            }while(n!=0);

            fclose(arquivo);


}
}
