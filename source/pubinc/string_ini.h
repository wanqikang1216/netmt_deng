#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char splitstr = ' ';

char** strsplit(char *str, char delim) {
	char *p = str, *h = str, *item = NULL;
	char **ret = NULL, **index;
	int size = 0;
	while(*p) {
		if(*p == delim)
			size++;
		p++;
	}
	ret = (char **)malloc((size+2) * sizeof(char *));
	if(ret == NULL)
		return NULL;
	p = str;
	index = ret;
	while(*p) {
		if(*p == delim) {
			size = p - h-1;
			item = (char *)malloc((size+1)*sizeof(char));
			if(h == str)
				memcpy(item, h,size+1);
			else
				memcpy(item, h+1,size);
			h = p;
			*index = item;
			index++;
		}
		p++;
	}
	size = p - h-1;
	item = (char *)malloc(size*sizeof(char));
	memcpy(item, h+1,size);
	*index = item;
	*(index + 1) = NULL;
	return ret;
}

void strfreesplitp(char **spl) {
	char **sp = spl;
	while(*sp) {
		sp++;
		free(*(sp-1));
	}
	free(spl);
}

//计算命令的个数
int commandNum(char *str, char delim) {
    int i = 1;
	for (int j = 0; j < strlen(str); j++) {
		if (str[j] == delim) {
			i++;
		}
	}
	return i;
}

// int main(int argc, char** argv)
// {
// 	char *str = "aa bb cc dd ee";
//     char delim = ' ';
//     char **command = strsplit(str, delim);
//     int num = commandNum(command);
    
//     printf("%d\n", num);
//     for (int i = 0; i < num; i++) {
//         printf("%s\n", command[i]);
//     }
  
//     strfreesplitp(command);
// }
