MKDIR = -mkdir -p 2>/dev/null
CP = -cp -r 2>/dev/null
RM = -rm -r 2>/dev/null

PWD_DIR = ${shell pwd}

CC = gcc

#创建可执行文件存放目录
prepare:
	${MKDIR} ${PWD_DIR}/out/bin

INCLUDE := -I${PWD_DIR}/source/pubinc/ \
		   -I${PWD_DIR}/source/pubinc/hv \
		   -I${PWD_DIR}/source/pubinc/cmd

LIBS_STATIC = -lhv \
			  -lpthread

LIBS_SHARED = -lhv \
				-lpthread

LIB_INC = -L${PWD_DIR}/../pubsrc/

SERVER = Server
SERVERC = Server.c

CLIENT = Client
CLIENTC = Client.c

TOTAL = ${SERVER} ${CLIENT}

all : ${TOTAL}

# netmt: prepare
${SERVER} : ${SERVERC}
	$(CC) -g -o ${PWD_DIR}/out/bin/${SERVER} $? ${INCLUDE} ${LIB_INC}/libhv.so ${LIBS_STATIC} 

${CLIENT} : ${CLIENTC}
	$(CC) -g -o ${PWD_DIR}/out/bin/${CLIENT} $? ${INCLUDE} ${LIB_INC}/libhv.so ${LIBS_STATIC} 

.PHONY : clean
clean :
	${RM} -rf ${TOTAL} *.so *.a
