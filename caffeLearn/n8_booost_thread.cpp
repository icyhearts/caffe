//#define MYDEBUG
#include<memory> // smart pointer
#include <boost/thread.hpp>
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
int gCnt = 0;
boost::mutex mutex_;

void add2cnt(int num)
{
  boost::mutex::scoped_lock lock(mutex_);
  cout<<"adding "<<num<<" to gCnt\n";
  gCnt += num;
//  lock.unlock();
  cout<<"unlocking\n";
  num += 1;
}
int main( ) {
  std::ios::sync_with_stdio(false);
  boost::thread t1(add2cnt, 11);
  boost::thread t2(add2cnt, 21);
  t1.join();
  t2.join();
  return 0;
}

