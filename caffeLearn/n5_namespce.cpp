//#define MYDEBUG
#include<memory> // smart pointer
#include<fstream>
#include<functional>
#include<utility>
#include<map>
#include<unordered_map>
#include<stack>
#include<stdio.h> 
#include<ctype.h>
#include<iterator>
#include<string>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
namespace keli
{
	class Student
	{
	public:
		static void Registry()
		{
			static int gMap = 31;
			++gMap;
			cout<<"gMap="<<gMap<<endl;
		}
	};
	int gCount = 3;
}
int main( ) {
	std::ios::sync_with_stdio(false);
	keli::gCount = 4;
//	keli::Student::gMap = 37;
	keli::Student::Registry();
	keli::Student::Registry();
	keli::Student::Registry();
	keli::Student::Registry();
	return 0;
}

