#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <utility>

#include "segmentation.cpp"
#include "preprocess.cpp"
using namespace std;

typedef struct viterbi_tag
{
	double p;
	int index;
};
class Hmm{
public:
//-----initial the Hmm 
	//类Hmm的构造函数
	Hmm(){
		Set_the_Transition_probabilities();
		Set_the_emission_probalility();
		Set_The_Initial_probability();
	}

	//获取发射概率
	void Set_the_emission_probalility()
	{
		fstream inFile;
		inFile.open("Emission_probability.txt",ios::in);
		vector<string> tokens;
		copy(istream_iterator<string>(inFile),
	         istream_iterator<string>(),
	         back_inserter<vector<string> >(tokens));
		string tmp_str;
		for(int i=0;i<tokens.size();)
		{
			tmp_str=tokens[i];
			++i;
			double *array=new double[26];
			for(int j=0;j<26;i++,j++)
				array[j]=atof(tokens[i].c_str());
			emission_probability.insert(make_pair(tmp_str,array));
		}
	}

	//获取转移概率
	void Set_the_Transition_probabilities()
	{
		fstream inFile;
		inFile.open("Transition_probabilities.txt",ios::in);
		vector<string> tokens;
		copy(istream_iterator<string>(inFile),
	         istream_iterator<string>(),
	         back_inserter<vector<string> >(tokens));
	    // double d[26][26];
	    for(int i=0;i<tokens.size();)
	    {
	    	for(int j=0;j<26;j++)
	    		for(int k=0;k<26;k++,i++)
	    			Transition_probabilities[j][k]=atof(tokens[i].c_str());
	    }
	}

	//设置初始概率
	void Set_The_Initial_probability()  
	{
		initial_probality=emission_probability["initial_probality"];
	}
	

//--------------hmm----------------------------------

	string viterbi(vector<string> v_word)
	{
		string index_table[26]={"n","t","s","f","v","a","b","z","r","m","q","d","p","c","u","e","y","o","j","l","i","h","k","g","x","w"};
		int size_vector=v_word.size();
		viterbi_tag probality[size_vector][26];
		int max_index;
		vector<string> v_result;
		double tmp;
		double maxp;
		int index=0;
		for(int t=0;t<size_vector;t++)
		{
			if(t==0)
			{
				for(int j=0;j<26;j++)
				{
					probality[t][j].p=emission_probability[v_word[t]][j]*initial_probality[j];
					probality[t][j].index=-1;
					// cout<<probality[t][j].p<<" ";
				}
				// cout<<endl;
			}
			else
			{
				for(int j=0;j<26;j++)
				{
					maxp=0.0f;
					for (int i = 0; i < 26; ++i)
					{
						tmp=probality[t-1][i].p*Transition_probabilities[i][j];
						if(tmp>maxp)
						{
							maxp=tmp;
							index=i;
						}
					}
					probality[t][j].p=maxp*emission_probability[v_word[t]][j];
					probality[t][j].index=index;
					// cout<<probality[t][j].p<<":"<<probality[t][j].index<<" ";

				}
				// cout<<endl;
			}
		}

		maxp=0.0f;
		int probality_index;
		for (int i = 0; i < 26; ++i)
		{
			if(probality[size_vector-1][i].p>maxp)
			{
				maxp=probality[size_vector-1][i].p;
				probality_index=i;
			}
		}
		v_result.push_back(index_table[probality_index]);
		for(int t=size_vector-1;t>=0;t--)
		{
			probality_index=probality[t][probality_index].index;
			if(probality_index==-1)
				break;
			v_result.push_back(index_table[probality_index]);
		}
		reverse(v_result.begin(),v_result.end());
		string res="";
		for(int i=0;i<size_vector;i++)
		{
			res+=v_word[i]+"/"+v_result[i]+" ";
		}
		return res;
	}

private:
	double Transition_probabilities[26][26];
	double* initial_probality;
	map<string,double*> emission_probability;
};

int main() {
	// Preprocess p;	
	Hmm hmm;
	string s="在这一年中，中国的改革开放和现代化建设继续向前迈进";
	Cutword cut;
	vector<string> v_word=cut.segmentation(s,4*SIZEOFWORD);
	cout<<hmm.viterbi(v_word)<<endl;
}
