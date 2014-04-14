#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

#define SIZEOFWORD 3  //here define the word of size

//自定义的数据结构
struct mytype
{
	string str;
	int num;
	double p;
};
class Cutword{
public:
	//利用map标准库把字典映射成，[成语]=概率，的数据模型
	void maptheword(string str1,map<string,double> *namemap) 
	{
		string token;
		istringstream iss(str1);
		string p1;
		double p2,p3;					
		getline(iss, token, ',');//
		p1=token;				// same as the java split(',')
		getline(iss, token, ',');//
		p2=atof(token.c_str())/100;
		getline(iss,token,',');
		p3=atof(token.c_str());
		(*namemap)[p1]=p3;// map the word with his counts
	}
	map<string,double> creatmap(string filename)
	{
		map<string, double> namemap;
	 	char buffer[1024];  
	    fstream outFile;  
	    outFile.open(filename.c_str(),ios::in);  
	    while(!outFile.eof())  
	    {  
	        outFile.getline(buffer,256,'\n');
	        maptheword(buffer,&namemap);
	    }  
	    outFile.close(); 	
		return namemap;
	}

	/*得到类似与这样的数据
	1	携	0.0013
	2	眷	1.17e-09
	3	出	1.28817e-10
	4	出席	2.0826e-11
	4	席	2.44752e-13	
	*/
	vector<mytype> creatvector(string sentence,int maxlen)
	{
		vector<mytype> v;
		map<string,double> namemap=creatmap("inFile.txt");
		mytype int_p;
		string word;
		for(int i=0;i<sentence.length();i+=SIZEOFWORD)
		{
			for(int j=SIZEOFWORD;j<=maxlen;j+=SIZEOFWORD)
			{
				if(i+j>sentence.length())
					break;
				word=sentence.substr(i,j);
				// cout<<word<<endl;
				if(namemap[word]==0)
				{
					if (j==SIZEOFWORD)
					{
						int_p.str=word;
						int_p.num=i/SIZEOFWORD+1;
						int_p.p=0.0;
					}
					else continue;
				}
				else{
					int_p.str=word;
					int_p.num=(i+word.length())/SIZEOFWORD;
					int_p.p=0.0;
				}
				v.push_back(int_p);
			}

		}

		double maxp=1;
		int pos=0;
		for(int i=0;i<v.size();i++)
		{
			pos=v[i].num-v[i].str.length()/SIZEOFWORD;
			if(pos==0)
			{
				if(namemap[v[i].str]==0)
					v[i].p=0.0000009;
				else
					v[i].p=namemap[v[i].str];
			}
			else
			{
				maxp=findleftword(pos,v);
				if(namemap[v[i].str]==0)
					v[i].p=0.0000009*maxp;
				else
					v[i].p=namemap[v[i].str]*maxp;
			}
		}
		// for(int i=0;i<v.size();i++)  //also just for test
		// {
		// 	cout<<v[i].num<<'\t'<<v[i].str<<'\t'<<v[i].p<<endl;
		// }
		return v;
	}

	//找出最佳左邻词
	double findleftword(int pos,vector<mytype> v)
	{
		double tmp=0;
		for(int i=0;i<v.size();i++)
		{

			if(v[i].num!=pos)	continue;
			else
			{
				if(v[i].p>tmp)
					tmp=v[i].p;
			}
		}
		return tmp;
	}

	//按照从右到左顺序，依次将每个词的最佳左邻词输出
	vector<string> segmentation(string sentence,int maxlen)
	{
		string res="";
		vector<string> vstr;
		string stmp;
		double maxp;
		int pos=sentence.length()/SIZEOFWORD;
		int k=maxlen/SIZEOFWORD;
		vector<mytype> v=creatvector(sentence,maxlen);
		for(int i=v.size()-1;i>=0;i--)
		{
			if(pos<=0) break;
			maxp=0;
			for(int t=0;t<v.size();t++)
			{
				if(v[t].num!=pos) continue;					
				if(v[t].p>maxp)
				{
					maxp=v[t].p;
					stmp=v[t].str;
				}	
			}
			vstr.push_back(stmp);
			pos=pos-stmp.length()/SIZEOFWORD;
		}
		reverse(vstr.begin(), vstr.end());
		return vstr;
	}

};
