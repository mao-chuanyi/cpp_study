INCLUDES:=./  ./include ./rpc_signup ./rpc_signin
SRCS:=$(shell find ./ -name "*.cc") 
LIBS:=  -lwfrest -lworkflow -lssl -lcrypto -lcrypt -fno-rtti -lalibabacloud-oss-cpp-sdk -lcurl -lpthread -lSimpleAmqpClient -lsrpc -lprotobuf -llz4 -lsnappy
OBJS:=$(patsubst %.cc, %.o, $(SRCS))
SERVER:= CloudiskServer

# 目标 : 依赖
$(SERVER): CloudiskServer.o Hash.o main.o Token.o rpc_signup/signup.pb.o rpc_signin/signin.pb.o
	g++ $^ -o $@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

%.o:%.cc
	g++ -c $^ -o $@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

HASH:=testHash
TOKEN:=testToken

$(HASH): Hash.o testHash.o
	g++ $^ -o $@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

$(TOKEN): Token.o testToken.o
	g++ $^ -o $@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

echo:
	echo $(INCLUDES)
	echo $(SRCS)

clean:
	rm -rf $(OBJS) $(SERVER) $(HASH) $(TOKEN) signup.pb.o signin.pb.o
