#include<bits/stdc++.h>
using namespace std;
signed main(){
  const int MOD = 1e9+7,B=1e8+7;
  long long res=0;
  string s;
  while(getline(cin,s)){
    string t=s;
    if(s.find("//")!=string::npos)
      t=s.substr(0,s.find("//"));
    for(char c:t){
      if(c==' ') continue;
      res=(res*B+c)%MOD;
    }
    cout<<s;
    if(s.size() >= 3 && s.substr(s.size() - 3, 3)=="///") {
      cout << res;
      res = 0;
    }
    cout<<endl;
  }
  cout<<res<<endl;
  return 0;
}///
