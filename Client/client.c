#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>


// *******************************************************************
// ____________________________Struct Binary File______________________________
// ********************************************************************

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

void initial_file_not_empty(FILE * fl){

    assert_that(fl!=NULL,"the file does not open \n" );

}
void initial_create_memory(unsigned int sizeSumbFile ){
    struct PictureBin* pb=PictureBinNew(sizeSumbFile);

    assert_that(pb->Buffer!=NULL,"memory does not create \n" );

}
void initial_image_not_empty(int countSumb){

    assert_that(countSumb!=0,"the image does not exist \n" );

}

void MessageSocket(int sizeSumbFile ){

    assert_that(sizeSumbFile != -1,"socket read/write error \n" );

}

//test environment
void run_test(void(*unit_test)(), const char* name) {
    unit_test();
}


// *************************************************
// _______________methood get picture_________________
// **************************************************

void GetPicture(char *NamePicture,struct PictureBin* BinPcture){
    FILE *fileObj=fopen(NamePicture,"rb");
    initial_file_not_empty(fileObj);

    int countSumb=fread(BinPcture->Buffer,sizeof(char),BinPcture->sizeSumbFileInp,fileObj);
    initial_image_not_empty(countSumb);
    if (countSumb<BinPcture->sizeSumbFileInp){BinPcture->sizeSumbFileInp=countSumb ;}
    fclose(fileObj);
}

int main(int argc,char **argv){

// *************************************************
// _______________Create SOCKET_________________
// **************************************************

    int Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    struct sockaddr_in SockAddr;
    SockAddr.sin_family=AF_INET;
    SockAddr.sin_port=htons(12345);
    SockAddr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);

    connect(Socket,(struct sockaddr *)(&SockAddr),sizeof(SockAddr));


// *************************************************
// _______________GETTING THE IMAGE_________________
// **************************************************

    unsigned int MAXSIZEPICTURE=20000;
    struct PictureBin *BinPcture=PictureBinNew(MAXSIZEPICTURE);
    GetPicture("../Image/Image1.jpeg",BinPcture);

    int sizeSumbFile=send(Socket,BinPcture->Buffer,BinPcture->sizeSumbFileInp,MSG_NOSIGNAL);
    MessageSocket(sizeSumbFile);

    PictureBinDestroy(BinPcture);

    shutdown(Socket,SHUT_RDWR);
    close(Socket);


    return 0;
}

