/*
 * Common.cpp
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#include "Common.h"


Common::Common() {
	// TODO Auto-generated constructor stub

}

Common::~Common() {
	// TODO Auto-generated destructor stub
}

void  Common::InfoDisp(int flag,char *info){
	cout<<"Info: ret= "<<flag<<"\t message: "<<info<<endl;
}
void  Common::ErrDisp(int flag,char *info){
	//cout<<"Err: ret= "<<flag<<"\t error: "<<info<<endl;
	perror(info);
	exit(flag);
}
void  Common::WarnDisp(int flag,char *info){
	//cout<<"Err: ret= "<<flag<<"\t error: "<<info<<endl;
	//exit(flag);
	perror(info);
	//return flag;
}
