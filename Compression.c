#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define Compressed "_compressed"
#define deCompressed "_decompressed"

void printError()
{
	printf("Error : 입력 파일 없음. \n");
}

int isNull(char buffer)
{
	if(buffer == '\0'){
		return 1;
	}
	else{
		return 0;
	}
}

void writeNull(FILE *pWrite, unsigned short nullCount)
{
	char nullCountArray[2];

	nullCountArray[0] = nullCount & 0xff;
	nullCountArray[1] = (nullCount >> 8) & 0xff;
	fputc('\0', pWrite);	//null을 한 번 써준다. 
	printf("1\n");
	fputc(nullCountArray[1], pWrite);
	fputc(nullCountArray[0], pWrite);
}

void compressing(char *fileName)
{	
	char buffer;
	unsigned short nullCount = 0;
	FILE *pRead = fopen(fileName, "rb");
	char *compressedFileName = strcat(fileName, Compressed);
	FILE *pWrite = fopen(compressedFileName, "wb");
	
	if(!pRead){
		printError();
		return;
	}
	while(!feof(pRead)){
		printf("파일을 읽기 시작합니다. \n");
		printf("%d\n",ftell(pRead));
		buffer = (char)fgetc(pRead);
		printf("%d\n",ftell(pRead));
		printf("%c\n", buffer);
		if(isNull(buffer)){		//읽어 온 char가 NULL일때,
			printf("3\n");
			nullCount++;
			continue;
		}
		else{
			if(nullCount != 0){
				printf("2\n");
				writeNull(pWrite, nullCount);
				nullCount = 0;
			}
			printf("읽어 온 cahr가 NUll이 아닐때, 그냥 씁니다. \n");
			fputc(buffer, pWrite);		//읽어 온 char가 NULL이 아닐때, 그냥 씁니다.
		}
	}
	fclose(pRead);
	fclose(pWrite);
}

void rewriteNull(FILE *pRead, FILE *pWrite)
{

	char nullCount1 = '\0';
	char nullCount2 = '\0';
	unsigned short nullCount = 0;

	nullCount1 = (char)fgetc(pRead);
	nullCount2 = (char)fgetc(pRead);
	nullCount = nullCount1;
	nullCount = nullCount << 8;
	nullCount = nullCount | (unsigned short)nullCount2;

	while(nullCount != 0){
		fputc('\0', pWrite);
		nullCount--;
	}
}

void deCompressing(char *compressedFileName)
{
	char buffer;
	unsigned short nullCount = 0;
	FILE *pRead = fopen(compressedFileName, "rb");
	char *deCompressedFileName = strcat(compressedFileName, deCompressed);
	FILE *pWrite = fopen(deCompressedFileName, "wb");

	if(!pRead){
		printError();
		return;
	}
	while(!feof(pRead)){
		buffer = (char)fgetc(pRead);
		if(isNull(buffer)){		//NULL문자를 읽었을 때, 
			rewriteNull(pRead, pWrite);
		}
		else{					//NULL문자가 아닌것을 읽었을 때, 
			fputc(buffer, pWrite);
		}
	}
	fclose(pRead);
	fclose(pWrite);
}

int printMenu()
{
	int menuSelection = 0;

	printf("1. 압축하기 \n");
	printf("2. 압축풀기 \n");
	printf("3. 종료\n");
	scanf("%d", &menuSelection);

	return menuSelection;
}

void main(int argc, char* argv[])
{	
	int menuSelection = printMenu();
	
	if(menuSelection == 1){
		printf("압축시작\n");
		compressing(argv[1]);
	}
	else if(menuSelection == 2){
		printf("압축 풀기 시작 \n");
		deCompressing(argv[1]);
	}
	else{
		return;	
	}
}
