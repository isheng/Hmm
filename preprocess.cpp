#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <utility>

using namespace std;

class Preprocess{
public:
	explicit Preprocess()
	{
		Write_Transition_probabilities();
		Write_Emission_probability();
	}
	//create map
	map<string,int> Inittag_index()
	{
		map<string,int> tag_index;
		//0 名词
		tag_index.insert(make_pair(string("n"),0));
		tag_index.insert(make_pair("nr",0));
		tag_index.insert(make_pair("ns",0));
		tag_index.insert(make_pair("nt",0));
		tag_index.insert(make_pair("nz",0));
		tag_index.insert(make_pair("nx",0));
		tag_index.insert(make_pair("na",0));

		//1 时间词
		tag_index.insert(make_pair("t",1));

		//2 处所词
		tag_index.insert(make_pair("s",2));

		//3 方位词
		tag_index.insert(make_pair("f",3));

		//4 动词
		tag_index.insert(make_pair("v",4));
		tag_index.insert(make_pair("vn",4));
		tag_index.insert(make_pair("vd",4));

		//5 形容词
		tag_index.insert(make_pair("a",5));
		tag_index.insert(make_pair("ad",5));
		tag_index.insert(make_pair("an",5));

		//6 区别词
		tag_index.insert(make_pair("b",6));

		//7 状态词
		tag_index.insert(make_pair("z",7));

		//8 代词
		tag_index.insert(make_pair("r",8));

		//9 数词
		tag_index.insert(make_pair("m",9));

		//10 量词
		tag_index.insert(make_pair("q",10));

		//11 副词
		tag_index.insert(make_pair("d",11));

		//12 介词
		tag_index.insert(make_pair("p",12));

		//13 连词
		tag_index.insert(make_pair("c",13));

		//14 助词
		tag_index.insert(make_pair("u",14));

		//15 叹词
		tag_index.insert(make_pair("e",15));

		//16 语气词
		tag_index.insert(make_pair("y",16));

		//26 拟声词
		tag_index.insert(make_pair("o",17));

		//18 简称略语
		tag_index.insert(make_pair("j",18));

		//19 习用语
		tag_index.insert(make_pair("l",19));

		//20 成语
		tag_index.insert(make_pair("i",20));

		//21 前接成分
		tag_index.insert(make_pair("h",21));

		//22 后接成分
		tag_index.insert(make_pair("k",22));

		//23 语素字
		tag_index.insert(make_pair("g",23));
		tag_index.insert(make_pair("Ag",23));
		tag_index.insert(make_pair("Dg",23));
		tag_index.insert(make_pair("Ng",23));
		tag_index.insert(make_pair("Tg",23));
		tag_index.insert(make_pair("Vg",23));
		tag_index.insert(make_pair("Bg",23));
		tag_index.insert(make_pair("Rg",23));
		tag_index.insert(make_pair("Mg",23));
		tag_index.insert(make_pair("Yg",23));

		//24 非语素字
		tag_index.insert(make_pair("x",24));

		//25 标点符号
		tag_index.insert(make_pair("w",25));
		return tag_index;
	}
	void delete_line_fisrt_word()
	{
		fstream inFile;
		string line;
		inFile.open("1998.txt",ios::in);
		fstream outFile;
		outFile.open("1998_1.txt",ios::out);
		int tmp_int;
		while(!inFile.eof()){
			getline(inFile,line);
			tmp_int=line.find(" ",0);
			line=line.substr(tmp_int+1,line.length()-tmp_int-1);
			outFile<<line<<endl;
		}
		inFile.close();
		outFile.close();

	}
	//-----------------转移概率----------------------------
	//分割字符串，提取标记，如 n，把它写入vector中
	vector<string> preprocess_splite()
	{
		delete_line_fisrt_word();
		ifstream inFile;
		inFile.open("1998_1.txt");
		vector<string> tmp;
		vector<string> tokens;
		copy(istream_iterator<string>(inFile),
	         istream_iterator<string>(),
	         back_inserter<vector<string> >(tmp));//对空字符的分割
		int pos=0;
		string strtmp;
		for(int i=0;i<tmp.size();i++)//分割'/'跟'['这两个字符
		{
			pos=tmp[i].find('/',0)+1;
			strtmp=tmp[i].substr(pos,tmp[i].length()-pos);

			//把]括号右边的词去掉
			if((pos=strtmp.find(']',0))!=strtmp.npos)
				strtmp=strtmp.substr(0,pos);
			tokens.push_back(strtmp);
		}
		inFile.close();
		// for(int i=0;i<tokens.size();i++)  //for test
		// 	cout<<tokens[i]<<endl;
		return tokens;
	}

	//求解转移概率的矩阵，存储在private d[26][26]
	void Write_Transition_probabilities()
	{
		//初始化二维数组a ,d 
		int a[26][26];
		double Transition_probabilities[26][26];
		for(int i=0;i<26;i++)
			for(int j=0;j<26;j++)
			{
				a[i][j]=0;
				Transition_probabilities[i][j]=0;	
			}

		map<string,int> tag_index=Inittag_index();//创建map, such as tag_index['n']=0,..

		vector<string> tag=preprocess_splite();

		int j,k;
		for(int i=0;i<tag.size()-1;i++)
		{
			j=tag_index[tag[i]];
			k=tag_index[tag[i+1]];
			a[j][k]++;
		}
		int tmp_sum;

		fstream outFile;
		outFile.open("Transition_probabilities.txt",ios::out);
		for(int i=0;i<26;i++)
		{
			tmp_sum=0;
			for(int j=0;j<26;j++)
				tmp_sum+=a[i][j];

			for(int j=0;j<26;j++)
			{
				if(tmp_sum==0)
					Transition_probabilities[i][j]=0.0f;
				else
					Transition_probabilities[i][j]=(double)a[i][j]/tmp_sum;  //转移概率矩阵Transition_probabilities
				outFile<<Transition_probabilities[i][j]<<" ";
			}
			outFile<<endl;

		}
	}
	//----------------求发射概率--------------
	map<string,int*> create_map()
	{
		delete_line_fisrt_word();
		fstream inFile;
		inFile.open("1998_1.txt",ios::in);
		vector<string> tmp_vector;
		copy(istream_iterator<string>(inFile),
	         istream_iterator<string>(),
	         back_inserter<vector<string> >(tmp_vector));

		map<string,int*> emission;			// [中国/ns  政府/n]nt 
		int pos;
		int start;
		string tmp_str;
		string tmp_tag;
		map<string,int> tag_index=Inittag_index();

		for(int i=0;i<tmp_vector.size();i++)
		{
			pos=tmp_vector[i].find('/',0);
			tmp_str=tmp_vector[i].substr(0,pos);  //提取中文字符   '[中国'
			if((start=tmp_str.find('[',0))!=tmp_str.npos)
				tmp_str=tmp_str.substr(start+1,tmp_str.length()-start-1);   // 去掉[符号  '中国'
			tmp_tag=tmp_vector[i].substr(pos+1,tmp_vector[i].length()-pos-1);   //获得tag   'ns'
			if((pos=tmp_tag.find(']',0))!=tmp_tag.npos)
				tmp_tag=tmp_tag.substr(0,pos);          //去掉]符号  获'n'

			if(emission.find(tmp_str)==emission.end())
			{
				int *array=new int[26];
				for(int ii=0;ii<26;ii++)
					array[ii]=0;
				array[tag_index[tmp_tag]]+=1;
				emission.insert(make_pair(tmp_str,array));
			}
			else{
				emission[tmp_str][tag_index[tmp_tag]]+=1;
			}

		}
		return emission;
	}

	//把发射概率写入到文件中
	void Write_Emission_probability()   //顺便把初始概率也写进去
	{
		fstream outFile;
		outFile.open("Emission_probability.txt",ios::out);
		cout<<"procesing..."<<endl;
		map<string,int*> emission=create_map();
		int array[26]={0};
		for(int i=0;i<26;i++)
		{
			for(map<string,int*>::const_iterator it=emission.begin();it!=emission.end();++it)
			{
				array[i]+=it->second[i];
			}

		}
		for(map<string,int*>::const_iterator it=emission.begin();it!=emission.end();++it)
		{
			outFile<<it->first<<" ";
			for(int i=0;i<26;i++)
			{
				if(array[i]!=0)
					outFile<<(double)(it->second[i])/array[i]<<" ";
				else
					outFile<<0.0f<<" ";
			}
			outFile<<endl;
		}
		int totalwords=0;
		for (int i = 0; i < 26; ++i)
		{
			totalwords+=array[i];
		}
		outFile<<"initial_probality"<<" ";
		for (int i = 0; i < 26; ++i)
		{
			outFile<<(double)array[i]/totalwords<<" ";
		}
		outFile.close();
	}
};
