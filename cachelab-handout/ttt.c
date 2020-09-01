#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
int blockBits;
int setBits;
int lines;
char *filename;
int blocksize;
int setSize;
int time;
int hit;
int miss;
int evict;

typedef struct Line
{
  /* data */

  int valid;
  int sign;
  int start;
  int time;
}  Line;
void** s;
int getSetIdx(int addr){
	return addr>>blockBits & (1<<setBits - 1);
}

int getSign(int addr){
	return addr>>(32 - setBits-blockBits);
}
int getOffset(int addr){
	return addr& (1<<blockBits - 1);
}
int findLines(int addr){
	int k = getSetIdx(addr);
	Line* p = s[k];
	for(int i =0; i < lines; i ++){
		if (p[i].valid){
			if (p[i].sign == getSign(addr)){
				if(p[i].start <= getOffset(addr) && (p[i].start + blocksize >=  getOffset(addr)))
					return i;
			}
		}
	}
	return -1;
}

int  findOld(Line* p){
	int old = 100000;
	int oldidx = -1;
	for(int i =0; i < lines; i ++){
		if (p[i].time <= old){
			oldidx = i;
			old = p[i].time;
		}
		}
	return oldidx;
}


void setCache(Line* p, int idx, int addr){
	p[idx].valid = 1;
	p[idx].sign = getSign(addr);
	p[idx].start = addr/blocksize * blocksize;
	p[idx].time = time;
}
int f(int addr){
	time ++;
	int k = getSetIdx(addr);
	Line* p = (Line*)(s[k]);
  int j=-1;
	for(int i =0; i < lines; i ++){
		if (p[i].valid){
			if (p[i].sign == getSign(addr)){
				if(p[i].start <= addr && (p[i].start + blocksize >  addr))
					j = i;
			}
		}
	}
	//return -1;
	if (j != -1){
		p[j].time = time;
	}else{
    int idx = findOld(p);
		setCache(p, idx,addr);
	}
  return j;
}




int
main (int argc, char **argv){
  int len;
	char buf[100];
	parse(argc,argv);
  blocksize = (int)(pow(2,blockBits));
  setSize= (int)(pow(2,setBits));
  s = malloc(setSize *sizeof(void*));
  for(int i =0;i < setSize;i++){
    s[i] = malloc(sizeof(Line) * lines);
    for(int j =0;j < lines;j++){
      Line* p = (Line*)(s[i]);
      p[j].valid = 0;
      p[j].sign = 0;
      p[j].start = 0;
      p[j].time = 0;
    }
  }
  FILE *fp;
if((fp = fopen(filename,"r")) == NULL)
 {
 perror("fail to read");
 exit (1) ;
 }
 printf("open success\n");
 while(fgets(buf,100,fp) != NULL)
 {
 int len = strlen(buf);
 //printf("len %d\n", len);
 buf[len-1] = '\0';  /*去掉换行符*/
 //printf("%s ",buf);
int p  = buf[1];
int addr = atoi(strtok(buf+2,","));

int tj =f(addr);
printf("%d\n",addr );

switch (p){
	case  'L':
    
	case 'S':
    if (tj == -1){
      miss += 1;
    }else{
      hit += 1;
    }
    break;
	case 'M':
  if (tj == -1){
      miss += 1;
      hit += 1;
    }else{
      hit += 1;
    }

break;
	case' ':
		break;
}


 }
 printf("hit %d, miss %d, evict %d\n",hit,miss,evict);
}
	
	




void parse(int argc, char **argv){
	int aflag = 0;
  int bflag = 0;
  char *cvalue = NULL;
  int index;
  int c;

  opterr = 0;

  while ((c = getopt (argc, argv, "s:E:b:t:v")) != -1)
    switch (c)
      {
      case 's':
        setBits = atoi(optarg);
        break;
      case 'E':
        lines = atoi(optarg);
        break;
      case 'b':
        blockBits = atoi(optarg);
        break;
      case 't':
      	filename = optarg;
      case 'v':
          fprintf (stdout, "Option -%c requested\n", optopt);
        
      default:
	continue;
      }
      printf("%d %d %d %s\n", setBits,lines,blockBits,filename);


}
