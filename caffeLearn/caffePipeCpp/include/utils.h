#ifndef UTILS_H
#define UTILS_H
#include<iostream>
#include "opencv2/core.hpp"
#include<algorithm>
#include<fstream>
#include<string>
#include<map>
#include<unordered_map>
#include<iterator>
//using namespace std;
using namespace cv;
using std::string;
using std::map;
using std::unordered_map;
using std::cerr;
using std::endl;
using std::ifstream;
// 参数读取类
class ParameterReader
{
	public:
		ParameterReader( string filename="./parameters.txt" )
		{
			ifstream fin( filename.c_str() );
			if (!fin)
			{
				cerr<<"parameter file does not exist."<<endl;
				return;
			}
			while(!fin.eof())
			{
				string str;
				getline( fin, str );
				if (str[0] == '#')
				{
					// 以‘＃’开头的是注释
					continue;
				}

				int pos = str.find("=");
				if (pos == -1)
					continue;
				string key = str.substr( 0, pos );
				string value = str.substr( pos+1, str.length() );
				// remove spaces from string
				key.erase( std::remove_if(key.begin(), key.end(), [](char x){return std::isspace(x);}),
						key.end());
				value.erase( std::remove_if(value.begin(), value.end(), [](char x){return std::isspace(x);}),
						value.end());
				data[key] = value;

				if ( !fin.good() )
					break;
			}
			fin.close();
		}
		string getData( string key )
		{
			unordered_map<string, string>::iterator iter = data.find(key);
			if (iter == data.end())
			{
				cerr<<"Parameter name "<<key<<" not found!"<<endl;
				return string("NOT_FOUND");
			}
			return iter->second;
		}
	public:
		unordered_map<string, string> data;
};
#endif // UTILS_H
