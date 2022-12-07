#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

// *************************************************
// _______________Struct Binary File_________________
// **************************************************

struct PictureBin
{
    char *Buffer;
    unsigned int sizeSumbFileInp;
};


struct PictureBin * PictureBinNew(unsigned int sizeSumbFile){
    struct PictureBin *newObj=malloc(sizeof(struct PictureBin));
    newObj->Buffer=(char*)malloc(sizeSumbFile);
    newObj->sizeSumbFileInp=sizeSumbFile;
    return newObj;
}

void PictureBinDestroy(struct PictureBin * PB){
    free(PB);
}


// *************************************************
// _______________UNIT TEST_________________
// **************************************************

void assert_that(int statement,const char *message){

    if (!statement){perror(message);}
}
void initial_create_memory(unsigned int sizeSumbFile ){
    struct PictureBin* pb=PictureBinNew(sizeSumbFile);

    assert_that(pb->Buffer!=NULL,"memory does not create \n" );

}

void MessageSocket(int sizeSumbFile ){

    assert_that(sizeSumbFile != -1,"socket read/write error \n" );

}
void initial_file_not_empty(FILE * fl){

    assert_that(fl!=NULL,"the file does not open \n" );

}
// *************************************************
// _______________methood restore image binary_________________
// **************************************************
#define PATHImage "../imageNew/image1New.jpeg"


void RestoreImage(struct PictureBin *BinPcture){
    FILE *fout;
    fout = fopen(PATHImage, "wb");

    initial_file_not_empty(fout);

    fwrite(BinPcture->Buffer,sizeof(char),BinPcture->sizeSumbFileInp,fout);

    fclose(fout);
}
// *************************************************
// _______________detected image_________________
// **************************************************
char DetectedImage(char *pathName){
//set image in algorithm

}

int main(int argc,char **argv){

// *************************************************
// _______________Create SOCKET_________________
// **************************************************


//create socet | descriptor
    int MasterSocket=socket(
            AF_INET /* IPv4 */,
            SOCK_STREAM /* TCP */,
            IPPROTO_TCP);

//BIND | create struct for ipv4
    struct sockaddr_in SockAddr;
    SockAddr.sin_family=AF_INET; //DOMEN
    SockAddr.sin_port=htons(12345);//port
    SockAddr.sin_addr.s_addr=htonl(INADDR_ANY); //IP address =0.0.0.0
    bind(MasterSocket,(struct sockaddr *)(&SockAddr),sizeof(SockAddr));

//LISTEN
    listen(MasterSocket,SOMAXCONN);

    unsigned int MAXSIZEPICTURE=20000;

    while(1){

        struct PictureBin *BinPcture=PictureBinNew(MAXSIZEPICTURE);

//create copy socket
        int SlaveSocket=accept(MasterSocket,0,0);
        int sizeSumbFile=recv(SlaveSocket,BinPcture->Buffer,BinPcture->sizeSumbFileInp,MSG_NOSIGNAL);
        MessageSocket(sizeSumbFile);
        if (sizeSumbFile) {
            //restoring the image using a binary file
            RestoreImage(BinPcture);
        }
        shutdown(SlaveSocket,SHUT_RDWR);
        close(SlaveSocket);

        //DetectedImage(PATHImage);

        PictureBinDestroy(BinPcture);

    }

    return 0;
}
